#include "nw4r/snd/snd_AxManager.h"

/* Original source:
 * kiwi515/ogws
 * src/nw4r/snd/snd_AxManager.cpp
 */

/*******************************************************************************
 * headers
 */

#include <cstring> // std::memset

#include "common.h"

#include "nw4r/snd/snd_BiquadFilterPreset.h"
#include "nw4r/snd/snd_FxBase.h"
#include "nw4r/snd/snd_global.h"
#include "nw4r/snd/snd_MoveValue.h"
#include "nw4r/snd/snd_Voice.h"
#include "nw4r/snd/snd_VoiceManager.h"

#include "nw4r/ut/ut_algorithm.h" // ut::Clamp
#include "nw4r/ut/ut_Lock.h" // ut::AutoInterruptLock

#include <rvl/OS/OSCache.h> // DCFlushRange
#include <rvl/AI/ai.h> // AICheckInit
#include <rvl/AX/AXAux.h>
#include <rvl/AX/AXCL.h>
#include <rvl/AX/AXOut.h>

#include "nw4r/NW4RAssert.hpp"

/*******************************************************************************
 * types
 */

// forward declarations
namespace nw4r { namespace snd { class BiquadFilterCallback; }}

/*******************************************************************************
 * variables
 */

namespace nw4r { namespace snd { namespace detail
{
	// .bss
	byte_t AxManager::sZeroBuffer[ZERO_BUFFER_SIZE];
	BiquadFilterCallback const *AxManager::sBiquadFilterCallbackTable[128];

	// .sbss
	BiquadFilterLpf AxManager::sBiquadFilterLpf;
	BiquadFilterHpf AxManager::sBiquadFilterHpf;
	BiquadFilterBpf512 AxManager::sBiquadFilterBpf512;
	BiquadFilterBpf1024 AxManager::sBiquadFilterBpf1024;
	BiquadFilterBpf2048 AxManager::sBiquadFilterBpf2048;
}}} // namespace nw4r::snd::detail

/*******************************************************************************
 * functions
 */

namespace nw4r { namespace snd { namespace detail {

AxManager::AxManager() :
	mOutputMode				(OUTPUT_MODE_STEREO),
	mZeroBufferAddress		(nullptr),
	mInitialized			(false),
	mUpdateVoicePrioFlag	(true),
	mOldAidCallback			(nullptr),
	mResetReadyCounter		(-1),
	field_0x100(5)
{
	mMainOutVolume.InitValue(1.0f);
	mMasterVolume.InitValue(1.0f);
	mVolumeForReset.InitValue(1.0f);

	for (int i = 0; i < AUX_BUS_NUM; i++)
	{
		mAuxFadeVolume[i].InitValue(1.0f);
		mAuxUserVolume[i].InitValue(1.0f);

		mAuxCallback[i]			= nullptr;
		mAuxCallbackContext[i]	= 0;
		mEffectProcessTick[i]	= 0;
	}
}

AxManager &AxManager::GetInstance()
{
	static AxManager instance;

	return instance;
}

#pragma push

#pragma ppc_iro_level 0 // somehow this got turned off???

void AxManager::Init()
{
	if (!mInitialized)
	{
		NW4RAssertMessage_Line(104, AICheckInit(), "not initialized AI \n");

		std::memset(sZeroBuffer, 0, ZERO_BUFFER_SIZE);
		DCFlushRange(sZeroBuffer, ZERO_BUFFER_SIZE);
		mZeroBufferAddress = sZeroBuffer;

		ut::AutoInterruptLock lock;

		AXGetAuxACallback(&mAuxCallback[AUX_A], &mAuxCallbackContext[AUX_A]);
		AXGetAuxBCallback(&mAuxCallback[AUX_B], &mAuxCallbackContext[AUX_B]);
		AXGetAuxCCallback(&mAuxCallback[AUX_C], &mAuxCallbackContext[AUX_C]);

		AXRegisterAuxACallback(nullptr, nullptr);
		AXRegisterAuxBCallback(nullptr, nullptr);
		AXRegisterAuxCCallback(nullptr, nullptr);

		mNextAxRegisterCallback = AXRegisterCallback(AxCallbackFunc);

		std::memset(sBiquadFilterCallbackTable, 0,
		            sizeof sBiquadFilterCallbackTable);

		SetBiquadFilterCallback(1, &sBiquadFilterLpf);
		SetBiquadFilterCallback(2, &sBiquadFilterHpf);
		SetBiquadFilterCallback(3, &sBiquadFilterBpf512);
		SetBiquadFilterCallback(4, &sBiquadFilterBpf1024);
		SetBiquadFilterCallback(5, &sBiquadFilterBpf2048);

		mInitialized = true;
	}
}

#pragma pop

void AxManager::Shutdown()
{
	if (!mInitialized)
		return;

	AXRegisterCallback(mNextAxRegisterCallback);

	ShutdownEffect(AUX_A);
	ShutdownEffect(AUX_B);
	ShutdownEffect(AUX_C);

	AXRegisterAuxACallback(mAuxCallback[AUX_A], mAuxCallbackContext[AUX_A]);
	AXRegisterAuxBCallback(mAuxCallback[AUX_B], mAuxCallbackContext[AUX_B]);
	AXRegisterAuxCCallback(mAuxCallback[AUX_C], mAuxCallbackContext[AUX_C]);

	for (int i = 0; i < AUX_BUS_NUM; i++)
	{
		mAuxCallback[i]			= nullptr;
		mAuxCallbackContext[i]	= nullptr;
	}

	mZeroBufferAddress = nullptr;

	mInitialized = false;
}

f32 AxManager::GetOutputVolume() const
{
	f32 volume = mMasterVolume.GetValue();

	return volume;
}

void AxManager::Update()
{
	for (int i = AUX_A; i < AUX_BUS_NUM; i++)
	{
		bool updateFlag = false;

		if (!mAuxUserVolume[i].IsFinished())
		{
			mAuxUserVolume[i].Update();
			updateFlag = true;
		}

		if (!mAuxFadeVolume[i].IsFinished())
		{
			mAuxFadeVolume[i].Update();

			if (mAuxFadeVolume[i].IsFinished())
				ShutdownEffect(static_cast<AuxBus>(i));

			updateFlag = true;
		}

		if (updateFlag)
		{
			f32 returnVolumeFloat = 1.0f;
			returnVolumeFloat *=
				ut::Clamp(mAuxUserVolume[i].GetValue(), 0.0f, 1.0f);
			returnVolumeFloat *=
				ut::Clamp(mAuxFadeVolume[i].GetValue(), 0.0f, 1.0f);

			u16 returnVolume = AUX_RETURN_VOLUME_MAX * returnVolumeFloat;

			switch (i)
			{
			case AUX_A:
				AXSetAuxAReturnVolume(returnVolume);
				break;

			case AUX_B:
				AXSetAuxBReturnVolume(returnVolume);
				break;

			case AUX_C:
				AXSetAuxCReturnVolume(returnVolume);
				break;
			}
		}
	}

	if (!mMasterVolume.IsFinished())
	{
		mMasterVolume.Update();

		VoiceManager::GetInstance().UpdateAllVoicesSync(Voice::UPDATE_VE);
	}

	if (!mVolumeForReset.IsFinished())
		mVolumeForReset.Update();

	if (!mMainOutVolume.IsFinished())
		mMainOutVolume.Update();

	f32 volume = mMainOutVolume.GetValue();
	volume *= mVolumeForReset.GetValue();
	volume = ut::Clamp(volume, 0.0f, 1.0f);

	AXSetMasterVolume(AUX_RETURN_VOLUME_MAX * volume);
}

void const *AxManager::GetZeroBufferAddress()
{
	NW4RAssertMessage_Line(261, mZeroBufferAddress,
	                       "Zero buffer is not created.");

	return mZeroBufferAddress;
}

void AxManager::RegisterCallback(CallbackListNode *node,
                                 AXFrameCallback *callback)
{
	ut::AutoInterruptLock lock;

	node->callback = callback;
	mCallbackList.PushBack(node);
}

void AxManager::UnregisterCallback(CallbackListNode *node)
{
	ut::AutoInterruptLock lock;

	mCallbackList.Erase(node);
}

void AxManager::SetOutputMode(OutputMode mode)
{
	if (mOutputMode == mode)
		return;

	ut::AutoInterruptLock lock;

	mOutputMode = mode;

	switch (mode)
	{
	case OUTPUT_MODE_STEREO:
		AXSetMode(AX_CL_MODE_STEREO);
		break;

	case OUTPUT_MODE_SURROUND:
		AXSetMode(AX_CL_MODE_SURROUND);
		break;

	case OUTPUT_MODE_DPL2:
		AXSetMode(AX_CL_MODE_DPL2);
		break;

	case OUTPUT_MODE_MONO:
		AXSetMode(AX_CL_MODE_STEREO);
		break;
	}

	VoiceManager::GetInstance().UpdateAllVoicesSync(Voice::UPDATE_MIX);

	for (int bus = AUX_A; bus < AUX_BUS_NUM; bus++)
	{
		FxBase::LinkList &fxList = GetEffectList(static_cast<AuxBus>(bus));

		NW4R_RANGE_FOR(itr, fxList)
			itr->OnChangeOutputMode();
	}

	if (mode == OUTPUT_MODE_DPL2)
		mEffectProcessTick[AUX_C] = 0;
}

OutputMode AxManager::GetOutputMode()
{
	return mOutputMode;
}

void AxManager::SetMasterVolume(f32 volume, int frame) {
	if (volume < 0.0f) {
		volume = 0.0f;
	}
    mMasterVolume.SetTarget(volume, (frame + 2) / 3);

    if (frame == 0) {
        VoiceManager::GetInstance().UpdateAllVoicesSync(Voice::SYNC_AX_VE);
    }
}

void AxManager::SetMainOutVolume(f32 volume, int frames)
{
	volume = ut::Clamp(volume, 0.0f, 1.0f);

	mMainOutVolume.SetTarget(volume, (frames + 2) / 3);
}

void AxManager::AxCallbackFunc()
{
	NW4R_RANGE_FOR_NO_AUTO_INC(itr, GetInstance().mCallbackList)
	{
		decltype(itr) curItr = itr++;

		(*curItr->callback)();
	}

	if (GetInstance().mNextAxRegisterCallback)
		(*GetInstance().mNextAxRegisterCallback)();
}

bool AxManager::AppendEffect(AuxBus bus, FxBase* pFx) {
    if (!mAuxFadeVolume[bus].IsFinished()) {
        ShutdownEffect(bus);
    }

    mAuxFadeVolume[bus].SetTarget(1.0f, 0);

    switch (bus) {
    case AUX_A: {
        AXSetAuxAReturnVolume(AX_MAX_VOLUME);
        break;
    }

    case AUX_B: {
        AXSetAuxBReturnVolume(AX_MAX_VOLUME);
        break;
    }

    case AUX_C: {
        AXSetAuxCReturnVolume(AX_MAX_VOLUME);
        break;
    }
    }

    if (!pFx->StartUp()) {
        return false;
    }

    ut::AutoInterruptLock lock;

    if (GetEffectList(bus).IsEmpty()) {
        switch (bus) {
        case AUX_A: {
            AXRegisterAuxACallback(AuxCallbackFunc,
                                   reinterpret_cast<void*>(bus));
            break;
        }

        case AUX_B: {
            AXRegisterAuxBCallback(AuxCallbackFunc,
                                   reinterpret_cast<void*>(bus));
            break;
        }

        case AUX_C: {
            AXRegisterAuxCCallback(AuxCallbackFunc,
                                   reinterpret_cast<void*>(bus));
            break;
        }
        }

        mAuxCallbackWaitCounter[bus] = 2;
    }

    GetEffectList(bus).PushBack(pFx);
    return true;
}

void AxManager::ClearEffect(AuxBus bus, int frame) {
    if (frame == 0) {
        ShutdownEffect(bus);
        if (!mAuxFadeVolume[bus].IsFinished()) {
            mAuxFadeVolume[bus].SetTarget(0.0f, 0);
        }
        return;
    }

    mAuxFadeVolume[bus].SetTarget(0.0f, (frame + 2) / 3);
}

void AxManager::ShutdownEffect(AuxBus bus)
{
	ut::AutoInterruptLock lock;

	FxBase::LinkList &list = GetEffectList(bus);

	if (list.IsEmpty())
		return;

	NW4R_RANGE_FOR(itr, list)
		itr->Shutdown();

	list.Clear();

	switch (bus)
	{
	case AUX_A:
		AXRegisterAuxACallback(nullptr, nullptr);
		mEffectProcessTick[AUX_A] = 0;
		break;

	case AUX_B:
		AXRegisterAuxBCallback(nullptr, nullptr);
		mEffectProcessTick[AUX_B] = 0;
		break;

	case AUX_C:
		AXRegisterAuxCCallback(nullptr, nullptr);
		mEffectProcessTick[AUX_C] = 0;
		break;
	}
}

void AxManager::SetBiquadFilterCallback(int type,
                                        BiquadFilterCallback const *biquad)
{
	sBiquadFilterCallbackTable[type] = biquad;
}

void AxManager::AuxCallbackFunc(void* pChans, void* pContext) {
    int num;
    void* buffer[AX_DPL2_MAX];

    void** ppChans = static_cast<void**>(pChans);
    AuxBus bus = static_cast<AuxBus>(reinterpret_cast<u32>(pContext));

	int tick = OSGetTick();

    if (GetInstance().GetOutputMode() == OUTPUT_MODE_DPL2) {
        num = AX_DPL2_MAX;

        buffer[AX_DPL2_L] = ppChans[AX_DPL2_L];
        buffer[AX_DPL2_R] = ppChans[AX_DPL2_R];
        buffer[AX_DPL2_LS] = ppChans[AX_DPL2_LS];
        buffer[AX_DPL2_RS] = ppChans[AX_DPL2_RS];
    } else {
        num = AX_STEREO_MAX;

        buffer[AX_STEREO_L] = ppChans[AX_STEREO_L];
        buffer[AX_STEREO_R] = ppChans[AX_STEREO_R];
        buffer[AX_STEREO_S] = ppChans[AX_STEREO_S];
    }

    if (GetInstance().mAuxCallbackWaitCounter[bus] > 0) {
        GetInstance().mAuxCallbackWaitCounter[bus]--;

        for (int i = 0; i < num; i++) {
            std::memset(buffer[i], 0, FX_BUFFER_SIZE);
        }
		GetInstance().mEffectProcessTick[bus] = OSGetTick() - tick;
    } else if (GetInstance().GetEffectList(bus).IsEmpty()) {
        for (int i = 0; i < num; i++) {
            std::memset(buffer[i], 0, FX_BUFFER_SIZE);
        }
		GetInstance().mEffectProcessTick[bus] = OSGetTick() - tick;
    } else {
        for (FxBase::LinkList::Iterator it =
                 GetInstance().GetEffectList(bus).GetBeginIter();
             it != GetInstance().GetEffectList(bus).GetEndIter(); ++it) {

            it->UpdateBuffer(num, buffer, FX_BUFFER_SIZE, FX_SAMPLE_FORMAT,
                             FX_SAMPLE_RATE, GetInstance().GetOutputMode());
        }
		GetInstance().mEffectProcessTick[bus] = OSGetTick() - tick;
    }
}

void AxManager::PrepareReset() {
    if (mOldAidCallback != NULL) {
        return;
    }

    mVolumeForReset.SetTarget(0.0f, 3);
    mResetReadyCounter = -1;
    mOldAidCallback = AIRegisterDMACallback(AiDmaCallbackFunc);
}

void AxManager::AiDmaCallbackFunc() {
    static bool finishedFlag = false;

    AxManager& r = GetInstance();
    r.mOldAidCallback();

    if (finishedFlag) {
        if (r.mResetReadyCounter < 0) {
            AXSetMaxDspCycles(0);
            r.mResetReadyCounter = AUX_CALLBACK_WAIT_FRAME;
        }
    } else if (r.mVolumeForReset.GetValue() == 0.0f) {
        finishedFlag = true;
    }

    if (r.mResetReadyCounter > 0) {
        r.mResetReadyCounter--;
    }
}

}}} // namespace nw4r::snd::detail
