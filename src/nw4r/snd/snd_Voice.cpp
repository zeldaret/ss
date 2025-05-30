#include "nw4r/snd/snd_Voice.h"

/* Original source:
 * kiwi515/ogws
 * src/nw4r/snd/snd_Voice.cpp
 */

/*******************************************************************************
 * headers
 */

#include <decomp.h>
#include "common.h"

#include "nw4r/snd/snd_AxManager.h"
#include "nw4r/snd/snd_AxVoiceManager.h"
#include "nw4r/snd/snd_AxVoice.h"
#include "nw4r/snd/snd_adpcm.h"
#include "nw4r/snd/snd_global.h"
#include "nw4r/snd/snd_Util.h"
#include "nw4r/snd/snd_VoiceManager.h"
#include "nw4r/snd/snd_WaveFile.h"

#include "nw4r/ut/ut_algorithm.h"
#include "nw4r/ut/ut_Lock.h"

#include <rvl/AX/AXAlloc.h> // AX_MAX_VOLUME

#include "nw4r/NW4RAssert.hpp"

/*******************************************************************************
 * local function declarations
 */

namespace nw4r { namespace snd { namespace detail
{
	inline u16 CalcMixVolume(f32 volume)
	{
		if (volume <= 0.0f)
			return 0;

		return ut::Min<u32>(65535, AX_MAX_VOLUME * volume);
	}
}}} // namespace nw4r::snd::detail

/*******************************************************************************
 * variables
 */

namespace nw4r { namespace snd { namespace detail
{
#if defined(BETTER_OBJDIFF_DIFF)
# define SEND_MAX			1.0f
# define SEND_MIN			0.0f
# define BIQUAD_VALUE_MAX	1.0f
# define BIQUAD_VALUE_MIN	0.0f
# define PAN_CENTER			0.0f
# define PAN_RIGHT			1.0f
# define PAN_LEFT			-1.0f
# define VOLUME_MAX			1.0f
# define VOLUME_MIN			0.0f
#else
	f32 const Voice::SEND_MAX = 1.0f;
	f32 const Voice::SEND_MIN = 0.0f;
	f32 const Voice::BIQUAD_VALUE_MAX = 1.0f;
	f32 const Voice::BIQUAD_VALUE_MIN = 0.0f;
	f32 const Voice::PAN_CENTER = 0.0f;
	f32 const Voice::PAN_RIGHT = 1.0f;
	f32 const Voice::PAN_LEFT = -1.0f;
	f32 const Voice::VOLUME_MAX = 1.0f;
	f32 const Voice::VOLUME_MIN = 0.0f;
#endif
}}} // namespace nw4r::snd::detail

/*******************************************************************************
 * functions
 */

namespace nw4r { namespace snd { namespace detail {

Voice::Voice() :
	mCallback		(nullptr),
	mActiveFlag		(false),
	mStartFlag		(false),
	mStartedFlag	(false),
	mPauseFlag		(false),
	mSyncFlag		(0)
{
	for (int channelIndex = 0; channelIndex < CHANNEL_MAX; channelIndex++)
	{
		for (int voiceOutIndex = 0; voiceOutIndex < 4; voiceOutIndex++)
			mAxVoice[channelIndex][voiceOutIndex] = nullptr;
	}
}

Voice::~Voice()
{
	for (int channelIndex = 0; channelIndex < CHANNEL_MAX; channelIndex++)
	{
		for (int voiceOutIndex = 0; voiceOutIndex < 4; voiceOutIndex++)
		{
			if (AxVoice *axVoice = mAxVoice[channelIndex][voiceOutIndex])
				AxVoiceManager::GetInstance().FreeAxVoice(axVoice);
		}
	}
}

void Voice::InitParam(int channelCount, int voiceOutCount, Callback *callback,
                      void *callbackData)
{
	// specifically not the source variants
	NW4RAssertHeaderClampedLRValue_Line(128, channelCount, 1, CHANNEL_MAX);
	NW4RAssertHeaderClampedLRValue_Line(129, voiceOutCount, 1, 4);

	mChannelCount					= channelCount;
	mVoiceOutCount					= voiceOutCount;
	mCallback						= callback;
	mCallbackData					= callbackData;
	mSyncFlag						= 0;
	mPauseFlag						= false;
	mPausingFlag					= false;
	mStartedFlag					= false;
	mVoiceOutParamPitchDisableFlag	= false;
	mVolume							= 1.0f;
	mVeInitVolume					= 0.0f;
	mVeTargetVolume					= 1.0f;
	mLpfFreq						= 1.0f;
	mBiquadType						= 0;
	mBiquadValue					= 0.0f;
	mPan							= 0.0f;
	mSurroundPan					= 0.0f;
	mOutputLineFlag					= 1;
	mMainOutVolume					= 1.0f;
	mMainSend						= 1.0f;

	for (int i = 0; i < AUX_BUS_NUM; i++)
		mFxSend[i] = 0.0f;

	for (int i = 0; i < 4; i++)
		mRemoteOutVolume[i] = 1.0f;

	mPitch							= 1.0f;
	mRemoteFilter					= 0;
	mPanMode						= PAN_MODE_DUAL;
	mPanCurve						= PAN_CURVE_SQRT;
}

void Voice::StopFinished()
{
	if (mActiveFlag && mStartedFlag && IsPlayFinished())
	{
		if (mCallback)
			(*mCallback)(this, CALLBACK_STATUS_FINISH_WAVE, mCallbackData);

		mStartedFlag	= false;
		mStartFlag		= false;
	}
}

void Voice::Calc()
{
	if (!mStartFlag)
		return;

	if (mSyncFlag & UPDATE_SRC)
	{
		CalcAxSrc(false);
		mSyncFlag &= ~UPDATE_SRC;
	}

	if (mSyncFlag & UPDATE_VE)
	{
		CalcAxVe();
		mSyncFlag &= ~UPDATE_VE;
	}

	if (mSyncFlag & UPDATE_MIX)
	{
		bool nextUpdateFlag = CalcAxMix();

		if (!nextUpdateFlag)
			mSyncFlag &= ~UPDATE_MIX;
	}

	if (mSyncFlag & UPDATE_LPF)
	{
		CalcAxLpf();
		mSyncFlag &= ~UPDATE_LPF;
	}

	if (mSyncFlag & UPDATE_BIQUAD)
	{
		CalcAxBiquadFilter();
		mSyncFlag &= ~UPDATE_BIQUAD;
	}

	if (mSyncFlag & UPDATE_REMOTE_FILTER)
	{
		CalcAxRemoteFilter();
		mSyncFlag &= ~UPDATE_REMOTE_FILTER;
	}
}

void Voice::Update()
{
	ut::AutoInterruptLock lock;

	if (!mActiveFlag)
		return;

	enum
	{
		NONE,

		RUN,
		STOP,
	} runFlag = NONE;

	if (mSyncFlag & UPDATE_START && mStartFlag && !mStartedFlag)
	{
		CalcAxSrc(true);

		runFlag = RUN;

		mStartedFlag = true;

		mSyncFlag &= ~UPDATE_START;
		mSyncFlag &= ~UPDATE_SRC;
	}

	if (mStartedFlag)
	{
		if (mSyncFlag & UPDATE_PAUSE && mStartFlag)
		{
			if (mPauseFlag)
			{
				mPausingFlag = true;
				runFlag = STOP;
			}
			else
			{
				mPausingFlag = false;
				runFlag = RUN;
			}

			mSyncFlag &= ~UPDATE_PAUSE;
		}

		SyncAxVoice();
	}

	switch (runFlag)
	{
	case RUN:
		RunAllAxVoice();
		break;

	case STOP:
		StopAllAxVoice();
		break;
	}
}

bool Voice::Acquire(int channelCount, int voiceOutCount, int priority,
                    Callback *callback, void *callbackData)
{

	NW4RAssertHeaderClampedLRValue_Line(336, channelCount, 1, CHANNEL_MAX);
	channelCount = ut::Clamp(channelCount, 1, CHANNEL_MAX);

	NW4RAssertHeaderClampedLRValue_Line(339, voiceOutCount, 1, 4);
	voiceOutCount = ut::Clamp(voiceOutCount, 1, 4);

	ut::AutoInterruptLock lock;

	u32 axPriority =
		priority == PRIORITY_MAX ? VOICE_PRIORITY_MAX : 16;

	NW4RAssert_Line(346, ! mActiveFlag);

	int requiredVoiceCount = channelCount * voiceOutCount;
	AxVoice *voiceTable[CHANNEL_MAX * 4];

	for (int i = 0; i < requiredVoiceCount; i++)
	{
		AxVoice *axVoice = nullptr;

		axVoice = AxVoiceManager::GetInstance().AcquireAxVoice(
			axPriority, &AxVoiceCallbackFunc, this);

		if (!axVoice)
		{
			int restAXVPBCount = requiredVoiceCount - i;

			Voice::LinkList const &voiceList =
				VoiceManager::GetInstance().GetVoiceList();

			NW4R_RANGE_FOR(itr, voiceList)
			{
				if (priority < itr->GetPriority())
					break;

				restAXVPBCount -= itr->GetPhysicalVoiceCount();
				if (restAXVPBCount <= 0)
					break;
			}

			if (restAXVPBCount > 0)
			{
				for (int j = 0; j < i; j++)
					AxVoiceManager::GetInstance().FreeAxVoice(voiceTable[j]);

				return false;
			}

			u32 allocPriority = axPriority == VOICE_PRIORITY_MAX
			                      ? VOICE_PRIORITY_MAX
			                      : 17;

			axVoice = AxVoiceManager::GetInstance().AcquireAxVoice(
				allocPriority, &AxVoiceCallbackFunc, this);
		}

		NW4RAssertPointerNonnull_Line(399, axVoice);

		if (!axVoice)
		{
			for (int j = 0; j < i; j++)
				AxVoiceManager::GetInstance().FreeAxVoice(voiceTable[j]);

			return false;
		}

		voiceTable[i] = axVoice;
	}

	int axVoiceIndex = 0;
	for (int channelIndex = 0; channelIndex < channelCount; channelIndex++)
	{
		for (int voiceOutIndex = 0; voiceOutIndex < voiceOutCount;
		     voiceOutIndex++)
		{
			voiceTable[axVoiceIndex]->SetPriority(axPriority);
			mAxVoice[channelIndex][voiceOutIndex] = voiceTable[axVoiceIndex];

			axVoiceIndex++;
		}
	}

	InitParam(channelCount, voiceOutCount, callback, callbackData);

	mActiveFlag = true;
	return true;
}

void Voice::Free()
{
	ut::AutoInterruptLock lock;

	if (!mActiveFlag)
		return;

	for (int channelIndex = 0; channelIndex < mChannelCount; channelIndex++)
	{
		for (int voiceOutIndex = 0; voiceOutIndex < mVoiceOutCount;
		     voiceOutIndex++)
		{
			if (AxVoice *axVoice = mAxVoice[channelIndex][voiceOutIndex])
			{
				AxVoiceManager::GetInstance().FreeAxVoice(axVoice);
				mAxVoice[channelIndex][voiceOutIndex] = nullptr;
			}
		}
	}

	mChannelCount = 0;
	VoiceManager::GetInstance().FreeVoice(this);

	mActiveFlag = false;
}

void Voice::Setup(WaveInfo const &waveParam, u32 startOffset)
{
	int sampleRate = waveParam.sampleRate;

	for (int channelIndex = 0; channelIndex < mChannelCount; channelIndex++)
	{
		if (!mAxVoice[channelIndex][0])
			continue;

		NW4RAssertPointerNonnull_Line(
			477, waveParam.channelParam[channelIndex].dataAddr);
		void *dataAddr = waveParam.channelParam[channelIndex].dataAddr;

		AdpcmParam adpcmParam;
		if (waveParam.sampleFormat == SAMPLE_FORMAT_DSP_ADPCM)
		{
			adpcmParam = waveParam.channelParam[channelIndex].adpcmParam;
			AxVoice::CalcOffsetAdpcmParam(&adpcmParam.pred_scale,
			                              &adpcmParam.yn1, &adpcmParam.yn2,
			                              startOffset, dataAddr, adpcmParam);
		}

		for (int voiceOutIndex = 0; voiceOutIndex < mVoiceOutCount;
		     voiceOutIndex++)
		{
			AxVoice *axVoice = mAxVoice[channelIndex][voiceOutIndex];
			if (!axVoice)
				continue;

			axVoice->Setup(waveParam.channelParam[channelIndex].dataAddr,
			               waveParam.sampleFormat, sampleRate);
			axVoice->SetAddr(waveParam.loopFlag, dataAddr, startOffset,
			                 waveParam.loopStart, waveParam.loopEnd);

			if (waveParam.sampleFormat == SAMPLE_FORMAT_DSP_ADPCM)
			{
				axVoice->SetAdpcm(&adpcmParam);
				axVoice->SetAdpcmLoop(
					&waveParam.channelParam[channelIndex].adpcmLoopParam);
			}

			axVoice->SetSrcType(AxManager::GetInstance().GetSrcType(), mPitch);
			axVoice->SetVoiceType(AxVoice::VOICE_TYPE_NORMAL);
		}
	}

	for (int voiceOutIndex = 0; voiceOutIndex < mVoiceOutCount; voiceOutIndex++)
	{
		mVoiceOutParam[voiceOutIndex].volume		= 1.0f;
		mVoiceOutParam[voiceOutIndex].pitch			= 1.0f;
		mVoiceOutParam[voiceOutIndex].pan			= 0.0f;
		mVoiceOutParam[voiceOutIndex].surroundPan	= 0.0f;
		mVoiceOutParam[voiceOutIndex].fxSend		= 0.0f;
		mVoiceOutParam[voiceOutIndex].lpf			= 0.0f;
	}

	mPauseFlag		= false;
	mPausingFlag	= false;
	mStartFlag		= false;
	mStartedFlag	= false;

	mSyncFlag |= UPDATE_MIX;
	mSyncFlag |= UPDATE_VE;
	mSyncFlag |= UPDATE_LPF;
}

void Voice::Start()
{
	mStartFlag	= true;
	mPauseFlag	= false;

	mSyncFlag |= UPDATE_START;
}

void Voice::Stop()
{
	if (mStartedFlag)
	{
		StopAllAxVoice();

		mStartedFlag = false;
	}

	mPausingFlag	= false;
	mPauseFlag		= false;
	mStartFlag		= false;
}

void Voice::Pause(bool flag)
{
	if (mPauseFlag != flag)
	{
		mPauseFlag = flag;

		mSyncFlag |= UPDATE_PAUSE;
	}
}

SampleFormat Voice::GetFormat() const
{
	NW4RAssert_Line(583, IsActive());

	if (IsActive())
		return mAxVoice[0][0]->GetFormat();

	return SAMPLE_FORMAT_PCM_S16;
}

void Voice::SetVolume(f32 volume)
{
	if (volume < 0.0f)
		volume = 0.0f;

	if (volume != mVolume)
	{
		mVolume = volume;

		mSyncFlag |= UPDATE_VE;
	}
}

void Voice::SetVeVolume(f32 targetVolume, f32 initVolume)
{
	if (targetVolume < 0.0f)
		targetVolume = 0.0f;
	if (initVolume < 0.0f)
		initVolume = 0.0f;

	if (initVolume < 0.0f)
	{
		// NOTE: unreachable (initVolume was clamped)

		if (targetVolume == mVeTargetVolume)
			return;

		mVeTargetVolume = targetVolume;

		mSyncFlag |= UPDATE_VE;
		return;
	}

	if (initVolume != mVeInitVolume || targetVolume != mVeTargetVolume)
	{
		mVeInitVolume = initVolume;
		mVeTargetVolume = targetVolume;

		mSyncFlag |= UPDATE_VE;
	}
}

void Voice::SetPitch(f32 pitch)
{
	if (pitch != mPitch)
	{
		mPitch = pitch;

		mSyncFlag |= UPDATE_SRC;
	}
}

void Voice::SetPanMode(PanMode panMode)
{
	if (panMode != mPanMode)
	{
		mPanMode = panMode;

		mSyncFlag |= UPDATE_MIX;
	}
}

void Voice::SetPanCurve(PanCurve panCurve)
{
	if (panCurve != mPanCurve)
	{
		mPanCurve = panCurve;

		mSyncFlag |= UPDATE_MIX;
	}
}

void Voice::SetPan(f32 pan)
{
	if (pan != mPan)
	{
		mPan = pan;

		mSyncFlag |= UPDATE_MIX;
	}
}

void Voice::SetSurroundPan(f32 pan)
{
	if (pan != mSurroundPan)
	{
		mSurroundPan = pan;

		mSyncFlag |= UPDATE_MIX;
	}
}

void Voice::SetLpfFreq(f32 freq)
{
	if (freq != mLpfFreq)
	{
		mLpfFreq = freq;

		mSyncFlag |= UPDATE_LPF;
	}
}

void Voice::SetBiquadFilter(int type, f32 value)
{
	// specifically not the source variant
	NW4RAssertHeaderClampedLRValue_Line(680, type, 0, 127);

	value = ut::Clamp(value, BIQUAD_VALUE_MIN, BIQUAD_VALUE_MAX);

	bool isUpdate = false;

	if (type != mBiquadType)
	{
		mBiquadType = type;
		isUpdate = true;
	}

	if (value != mBiquadValue)
	{
		mBiquadValue = value;
		isUpdate = true;
	}

	if (isUpdate)
		mSyncFlag |= UPDATE_BIQUAD;
}

void Voice::SetRemoteFilter(int filter)
{
	filter = ut::Clamp(filter, REMOTE_FILTER_MIN, REMOTE_FILTER_MAX);

	if (filter != mRemoteFilter)
	{
		mRemoteFilter = filter;

		mSyncFlag |= UPDATE_REMOTE_FILTER;
	}
}

void Voice::SetOutputLine(int lineFlag)
{
	if (lineFlag != mOutputLineFlag)
	{
		mOutputLineFlag = lineFlag;

		mSyncFlag |= UPDATE_MIX;
	}
}

void Voice::SetMainOutVolume(f32 volume)
{
	if (volume < 0.0f)
		volume = 0.0f;

	if (volume != mMainOutVolume)
	{
		mMainOutVolume = volume;

		mSyncFlag |= UPDATE_MIX;
	}
}

void Voice::SetMainSend(f32 send)
{
	send += 1.0f;
	if (send < SEND_MIN)
		send = SEND_MIN;

	if (send != mMainSend)
	{
		mMainSend = send;

		mSyncFlag |= UPDATE_MIX;
	}
}

void Voice::SetFxSend(AuxBus bus, f32 send)
{
	// specifically not the source variant
	NW4RAssertHeaderClampedLValue_Line(748, bus, 0, AUX_BUS_NUM);

	if (send < SEND_MIN)
		send = SEND_MIN;

	if (send != mFxSend[bus])
	{
		mFxSend[bus] = send;

		mSyncFlag |= UPDATE_MIX;
	}
}

void Voice::SetRemoteOutVolume(int remote, f32 volume)
{
	if (volume < 0.0f)
		volume = 0.0f;

	if (mRemoteOutVolume[remote] == volume)
		return;

	mRemoteOutVolume[remote] = volume;
	mSyncFlag |= UPDATE_MIX;
}

void Voice::SetVoiceOutParam(int voiceOutIndex,
                             VoiceOutParam const &voiceOutParam)
{
	// specifically not the source variant
	NW4RAssertHeaderClampedLRValue_Line(820, voiceOutIndex, 0, 4);

	mVoiceOutParam[voiceOutIndex] = voiceOutParam;

	mSyncFlag |= UPDATE_SRC | UPDATE_VE | UPDATE_MIX | UPDATE_LPF;
}

void Voice::SetPriority(int priority)
{
	// specifically not the source variant
	NW4RAssertHeaderClampedLRValue_Line(828, priority, PRIORITY_MIN,
	                                    PRIORITY_MAX);

	mPriority = priority;
	VoiceManager::GetInstance().ChangeVoicePriority(this);

	if (mPriority != AX_PRIORITY_MIN)
		return;

	for (int channelIndex = 0; channelIndex < mChannelCount; channelIndex++)
	{
		for (int voiceOutIndex = 0; voiceOutIndex < mVoiceOutCount;
		     voiceOutIndex++)
		{
			if (AxVoice *axVoice = mAxVoice[channelIndex][voiceOutIndex])
				axVoice->SetPriority(15);
		}
	}
}

void Voice::UpdateVoicesPriority()
{
	if (mPriority == AX_PRIORITY_MIN)
		return;

	for (int channelIndex = 0; channelIndex < mChannelCount; channelIndex++)
	{
		for (int voiceOutIndex = 0; voiceOutIndex < mVoiceOutCount;
		     voiceOutIndex++)
		{
			if (AxVoice *axVoice = mAxVoice[channelIndex][voiceOutIndex])
				axVoice->SetPriority(16);
		}
	}
}
#if 0
// Voice::GetAxVoice ([R89JEL]:/bin/RVL/Debug/mainD.MAP:14824)
DECOMP_FORCE(NW4RAssert_String(channelIndex < CHANNEL_MAX));
#endif

void Voice::SetAdpcmLoop(int channelIndex, AdpcmLoopParam const *param)
{
	for (int voiceOutIndex = 0; voiceOutIndex < mVoiceOutCount; voiceOutIndex++)
	{
		if (AxVoice *axVoice = mAxVoice[channelIndex][voiceOutIndex])
			axVoice->SetAdpcmLoop(param);
	}
}

u32 Voice::GetCurrentPlayingSample() const
{
	if (IsActive())
		return mAxVoice[0][0]->GetCurrentPlayingSample();

	return 0;
}

void Voice::SetLoopStart(int channelIndex, void const *baseAddress, u32 samples)
{
	for (int voiceOutIndex = 0; voiceOutIndex < mVoiceOutCount; voiceOutIndex++)
	{
		if (AxVoice *axVoice = mAxVoice[channelIndex][voiceOutIndex])
			axVoice->SetLoopStart(baseAddress, samples);
	}
}

void Voice::SetLoopEnd(int channelIndex, void const *baseAddress, u32 samples)
{
	for (int voiceOutIndex = 0; voiceOutIndex < mVoiceOutCount; voiceOutIndex++)
	{
		if (AxVoice *axVoice = mAxVoice[channelIndex][voiceOutIndex])
			axVoice->SetLoopEnd(baseAddress, samples);
	}
}

void Voice::SetLoopFlag(bool loopFlag)
{
	for (int channelIndex = 0; channelIndex < mChannelCount; channelIndex++)
	{
		for (int voiceOutIndex = 0; voiceOutIndex < mVoiceOutCount;
		     voiceOutIndex++)
		{
			if (AxVoice *axVoice = mAxVoice[channelIndex][voiceOutIndex])
				axVoice->SetLoopFlag(loopFlag);
		}
	}
}

void Voice::StopAtPoint(int channelIndex, void const *baseAddress, u32 samples)
{
	for (int voiceOutIndex = 0; voiceOutIndex < mVoiceOutCount; voiceOutIndex++)
	{
		if (AxVoice *axVoice = mAxVoice[channelIndex][voiceOutIndex])
			axVoice->StopAtPoint(baseAddress, samples);
	}
}

void Voice::SetVoiceType(AxVoice::VoiceType type)
{
	for (int channelIndex = 0; channelIndex < mChannelCount; channelIndex++)
	{
		for (int voiceOutIndex = 0; voiceOutIndex < mVoiceOutCount;
		     voiceOutIndex++)
		{
			if (AxVoice *axVoice = mAxVoice[channelIndex][voiceOutIndex])
				axVoice->SetVoiceType(type);
		}
	}
}

void Voice::CalcAxSrc(bool initialUpdate)
{
	for (int voiceOutIndex = 0; voiceOutIndex < mVoiceOutCount; voiceOutIndex++)
	{
		f32 ratio = mPitch;

		if (!mVoiceOutParamPitchDisableFlag)
			ratio *= mVoiceOutParam[voiceOutIndex].pitch;

		for (int channelIndex = 0; channelIndex < mChannelCount; channelIndex++)
		{
			if (AxVoice *axVoice = mAxVoice[channelIndex][voiceOutIndex])
				axVoice->SetSrc(ratio, initialUpdate);
		}
	}
}

void Voice::CalcAxVe()
{
	f32 baseVolume = 1.0f;
	baseVolume *= mVolume;
	baseVolume *= AxManager::GetInstance().GetOutputVolume();

	for (int voiceOutIndex = 0; voiceOutIndex < mVoiceOutCount; voiceOutIndex++)
	{
		f32 volume = baseVolume * mVoiceOutParam[voiceOutIndex].volume;
		f32 targetVolume = volume * mVeTargetVolume;
		f32 initVolume = volume * mVeInitVolume;

		for (int channelIndex = 0; channelIndex < mChannelCount; channelIndex++)
		{
			if (AxVoice *axVoice = mAxVoice[channelIndex][voiceOutIndex])
				axVoice->SetVe(targetVolume, initVolume);
		}
	}
}

bool Voice::CalcAxMix()
{
	bool nextUpdateFlag = false;

	AxVoice::MixParam mix;

	/* The address is taken and the members are set, but the members aren't used
	 * after that
	 */
	AxVoice::RemoteMixParam rmtmix;

	for (int channelIndex = 0; channelIndex < mChannelCount; channelIndex++)
	{
		for (int voiceOutIndex = 0; voiceOutIndex < mVoiceOutCount;
		     voiceOutIndex++)
		{
			if (AxVoice *axVoice = mAxVoice[channelIndex][voiceOutIndex])
			{
				CalcMixParam(channelIndex, voiceOutIndex, &mix, &rmtmix);

				nextUpdateFlag |= axVoice->SetMix(mix);
				if (mOutputLineFlag == 0 || mOutputLineFlag == 1) {
					axVoice->EnableRemote(false);
				} else {
					axVoice->EnableRemote(true);
					axVoice->SetRmtMix(rmtmix);
				}
			}
		}
	}

	return nextUpdateFlag;
}

void Voice::CalcAxLpf()
{
	for (int voiceOutIndex = 0; voiceOutIndex < mVoiceOutCount; voiceOutIndex++)
	{
		u16 freq =
			Util::CalcLpfFreq(mLpfFreq + mVoiceOutParam[voiceOutIndex].lpf);

		for (int channelIndex = 0; channelIndex < mChannelCount; channelIndex++)
		{
			if (AxVoice *axVoice = mAxVoice[channelIndex][voiceOutIndex])
				axVoice->SetLpf(freq);
		}
	}
}

void Voice::CalcAxBiquadFilter()
{
	for (int voiceOutIndex = 0; voiceOutIndex < mVoiceOutCount; voiceOutIndex++)
	{
		for (int channelIndex = 0; channelIndex < mChannelCount; channelIndex++)
		{
			if (AxVoice *axVoice = mAxVoice[channelIndex][voiceOutIndex])
				axVoice->SetBiquad(mBiquadType, mBiquadValue);
		}
	}
}

void Voice::CalcAxRemoteFilter()
{
	for (int voiceOutIndex = 0; voiceOutIndex < mVoiceOutCount; voiceOutIndex++)
	{
		for (int channelIndex = 0; channelIndex < mChannelCount; channelIndex++)
		{
			if (AxVoice *axVoice = mAxVoice[channelIndex][voiceOutIndex])
				axVoice->SetRemoteFilter(mRemoteFilter);
		}
	}
}

void Voice::SyncAxVoice()
{
	for (int channelIndex = 0; channelIndex < mChannelCount; channelIndex++)
	{
		for (int voiceOutIndex = 0; voiceOutIndex < mVoiceOutCount;
		     voiceOutIndex++)
		{
			// What
			if (mAxVoice[channelIndex][voiceOutIndex])
				mAxVoice[channelIndex][voiceOutIndex]->Sync();
		}
	}
}

void Voice::ResetDelta()
{
	for (int voiceOutIndex = 0; voiceOutIndex < mVoiceOutCount; voiceOutIndex++)
	{
		for (int channelIndex = 0; channelIndex < mChannelCount; channelIndex++)
		{
			if (AxVoice *axVoice = mAxVoice[channelIndex][voiceOutIndex])
				axVoice->ResetDelta();
		}
	}
}

void Voice::AxVoiceCallbackFunc(AxVoice *dropVoice,
                                AxVoice::AxVoiceCallbackStatus status,
                                void *callbackData)
{
	Voice *voice = static_cast<Voice *>(callbackData);
	NW4RAssertPointerNonnull_Line(1165, voice);

	VoiceCallbackStatus voiceStatus;
	bool freeDropVoice = false;

	switch (status)
	{
	case AxVoice::CALLBACK_STATUS_CANCEL:
		voiceStatus = CALLBACK_STATUS_CANCEL;
		break;

	case AxVoice::CALLBACK_STATUS_DROP_DSP:
		voiceStatus = CALLBACK_STATUS_DROP_DSP;
		freeDropVoice = true;
		break;
	}

	for (int channelIndex = 0; channelIndex < voice->mChannelCount;
	     channelIndex++)
	{
		for (int voiceOutIndex = 0; voiceOutIndex < voice->mVoiceOutCount;
		     voiceOutIndex++)
		{
			AxVoice *axVoice = voice->mAxVoice[channelIndex][voiceOutIndex];
			if (!axVoice)
				continue;

			if (axVoice == dropVoice)
			{
				if (!freeDropVoice)
					AxVoiceManager::GetInstance().FreeAxVoice(axVoice);
			}
			else
			{
				axVoice->Stop();
				AxVoiceManager::GetInstance().FreeAxVoice(axVoice);
			}

			voice->mAxVoice[channelIndex][voiceOutIndex] = nullptr;
		}
	}

	voice->mPauseFlag = false;
	voice->mStartFlag = false;
	voice->mChannelCount = 0;

	if (freeDropVoice)
		voice->Free();

	if (voice->mCallback)
		(*voice->mCallback)(voice, voiceStatus, voice->mCallbackData);
}

void Voice::TransformDpl2Pan(f32 *outPan, f32 *outSurroundPan, f32 inPan,
                             f32 inSurroundPan)
{
	inSurroundPan -= 1.0f;

	if (ut::Abs(inPan) <= ut::Abs(inSurroundPan))
	{
		if (inSurroundPan <= 0.0f)
		{
			*outPan = inPan;
			*outSurroundPan = -0.12f + 0.88f * inSurroundPan;
		}
		else
		{
			*outPan = 0.5f * inPan;
			*outSurroundPan = -0.12f + 1.12f * inSurroundPan;
		}
	}
	else if (inPan >= 0.0f)
	{
		/* NOTE: do not constant-fold 1.0f - 0.85f or 1.0f - 0.65f; they differ
		 * by 1 digit
		 */
		if (inSurroundPan <= 0.0f)
		{
			*outPan = (0.85f + (1.0f - 0.85f) * (-inSurroundPan / inPan))
			        * ut::Abs(inPan);
			*outSurroundPan = -0.12f + (2.0f * inSurroundPan + 0.88f * inPan);
		}
		else
		{
			*outPan = (0.85f + (1.0f - 0.65f) * (-inSurroundPan / inPan))
			        * ut::Abs(inPan);
			*outSurroundPan = -0.12f + 1.12f * inPan;
		}
	}
	else
	{
		if (inSurroundPan <= 0.0f)
		{
			*outPan = ((1.0f - 0.85f) * (-inSurroundPan / inPan) - 0.85f)
			        * ut::Abs(inPan);
			*outSurroundPan = -0.12f + (2.0f * inSurroundPan - 1.12f * inPan);
		}
		else
		{
			*outPan = ((1.0f - 0.65f) * (-inSurroundPan / inPan) - 0.85f)
			        * ut::Abs(inPan);
			*outSurroundPan = -0.12f + 1.12f * -inPan;
		}
	}

	*outSurroundPan += 1.0f;
}

void Voice::CalcMixParam(int channelIndex, int voiceOutIndex,
                         AxVoice::MixParam *mix,
                         AxVoice::RemoteMixParam *rmtmix)
{
	NW4RAssertPointerNonnull_Line(1284, mix);

	f32 mainVolume = 0.0f;
	f32 mainSend = 0.0f;

	f32 fxSendA = 0.0f;
	f32 fxSendB = 0.0f;
	f32 fxSendC = 0.0f;

	if (mOutputLineFlag & 1) // OUTPUT_LINE_MAIN
	{
		mainVolume = mMainOutVolume;
		mainSend = mMainSend;

		fxSendA = mFxSend[AUX_A] + mVoiceOutParam[voiceOutIndex].fxSend;
		if (fxSendA < SEND_MIN) {
			fxSendA = SEND_MIN;
		}
		fxSendB = mFxSend[AUX_B];
		fxSendC = mFxSend[AUX_C];
	}

	f32 main = mainVolume * nw4r::ut::Clamp(mainSend, 0.0f, 1.0f);
	f32 fx_a = mainVolume * nw4r::ut::Clamp(fxSendA, 0.0f, 1.0f);
	f32 fx_b = mainVolume * nw4r::ut::Clamp(fxSendB, 0.0f, 1.0f);
	f32 fx_c = mainVolume * nw4r::ut::Clamp(fxSendC, 0.0f, 1.0f);

	f32 remoteOutVolumeOrig[4];
	for (int i = 0; i < 4; i++)
	{
		if (mOutputLineFlag & (2 << i))
		{
			remoteOutVolumeOrig[i] = mRemoteOutVolume[i];
		}
		else
		{
			remoteOutVolumeOrig[i] = 0.0f;
		}
	}

	f32 left, right, surround, lrMixed;
	f32 front, rear;

	Util::PanInfo panInfo;
	panInfo.curve			= Util::PAN_CURVE_SQRT;
	panInfo.centerZeroFlag	= false;
	panInfo.zeroClampFlag	= false;

	switch (mPanCurve)
	{
	case PAN_CURVE_SQRT:
		panInfo.curve			= Util::PAN_CURVE_SQRT;
		break;

	case PAN_CURVE_SQRT_0DB:
		panInfo.curve			= Util::PAN_CURVE_SQRT;
		panInfo.centerZeroFlag	= true;
		break;

	case PAN_CURVE_SQRT_0DB_CLAMP:
		panInfo.curve			= Util::PAN_CURVE_SQRT;
		panInfo.centerZeroFlag	= true;
		panInfo.zeroClampFlag	= true;
		break;

	case PAN_CURVE_SINCOS:
		panInfo.curve			= Util::PAN_CURVE_SINCOS;
		break;

	case PAN_CURVE_SINCOS_0DB:
		panInfo.curve			= Util::PAN_CURVE_SINCOS;
		panInfo.centerZeroFlag	= true;
		break;

	case PAN_CURVE_SINCOS_0DB_CLAMP:
		panInfo.curve			= Util::PAN_CURVE_SINCOS;
		panInfo.centerZeroFlag	= true;
		panInfo.zeroClampFlag	= true;
		break;

	case PAN_CURVE_LINEAR:
		panInfo.curve			= Util::PAN_CURVE_LINEAR;
		break;

	case PAN_CURVE_LINEAR_0DB:
		panInfo.curve			= Util::PAN_CURVE_LINEAR;
		panInfo.centerZeroFlag	= true;
		break;

	case PAN_CURVE_LINEAR_0DB_CLAMP:
		panInfo.curve			= Util::PAN_CURVE_LINEAR;
		panInfo.centerZeroFlag	= true;
		panInfo.zeroClampFlag	= true;
		break;

	default:
		panInfo.curve			= Util::PAN_CURVE_SQRT;
		break;
	}

	if (mChannelCount > 1 && mPanMode == PAN_MODE_BALANCE)
	{
		f32 pan = mPan + mVoiceOutParam[voiceOutIndex].pan;
		f32 surroundPan =
			mSurroundPan + mVoiceOutParam[voiceOutIndex].surroundPan;

		if (channelIndex == 0)
		{
			left = Util::CalcPanRatio(pan, panInfo);
			right = 0.0f;
		}
		else if (channelIndex == 1)
		{
			left = 0.0f;
			right = Util::CalcPanRatio(-pan, panInfo);
		}

		/* ERRATUM: left and right are used uninitialized if channelIndex is
		 * neither 0 nor 1
		 */

		front = Util::CalcSurroundPanRatio(surroundPan, panInfo);
		rear = Util::CalcSurroundPanRatio(2.0f - surroundPan, panInfo);
	}
	else
	{
		f32 voicePan = PAN_CENTER;
		f32 pan, surroundPan;

		if (mChannelCount == 2)
		{
			if (channelIndex == 0)
				voicePan = PAN_LEFT;
			if (channelIndex == 1)
				voicePan = PAN_RIGHT;
		}

		switch (AxManager::GetInstance().GetOutputMode())
		{
		case OUTPUT_MODE_DPL2:
			TransformDpl2Pan(
				&pan, &surroundPan,
				mPan + voicePan + mVoiceOutParam[voiceOutIndex].pan,
				mSurroundPan + mVoiceOutParam[voiceOutIndex].surroundPan);
			break;

		case OUTPUT_MODE_STEREO:
		case OUTPUT_MODE_SURROUND:
		case OUTPUT_MODE_MONO:
		default:
			pan = mPan + voicePan + mVoiceOutParam[voiceOutIndex].pan;
			surroundPan =
				mSurroundPan + mVoiceOutParam[voiceOutIndex].surroundPan;
			break;
		}

		left = Util::CalcPanRatio(pan, panInfo);
		right = Util::CalcPanRatio(-pan, panInfo);
		front = Util::CalcSurroundPanRatio(surroundPan, panInfo);
		rear = Util::CalcSurroundPanRatio(2.0f - surroundPan, panInfo);
	}

	surround = Util::CalcVolumeRatio(-3.0f);
	lrMixed = 0.5f * (left + right);

	f32 m_l;
	f32 m_r;
	f32 m_s;
	f32 a_l;
	f32 a_r;
	f32 a_s;
	f32 b_l;
	f32 b_r;
	f32 b_s;
	f32 c_l;
	f32 c_r;
	f32 c_s;

	f32 &m_sl = m_s;
	f32 &m_sr = c_l;

	f32 &a_sl = a_s;
	f32 &a_sr = c_r;

	f32 &b_sl = b_s;
	f32 &b_sr = c_s;

	switch (AxManager::GetInstance().GetOutputMode())
	{
	case OUTPUT_MODE_STEREO:
		m_l = main * left;
		m_r = main * right;
		m_s = 0.0f;

		a_l = fx_a * left;
		a_r = fx_a * right;
		a_s = 0.0f;

		b_l = fx_b * left;
		b_r = fx_b * right;
		b_s = 0.0f;

		c_l = fx_c * left;
		c_r = fx_c * right;
		c_s = 0.0f;

		break;

	case OUTPUT_MODE_MONO:
		m_l = main * lrMixed;
		m_r = main * lrMixed;
		m_s = 0.0f;

		a_l = fx_a * lrMixed;
		a_r = fx_a * lrMixed;
		a_s = 0.0f;

		b_l = fx_b * lrMixed;
		b_r = fx_b * lrMixed;
		b_s = 0.0f;

		c_l = fx_c * lrMixed;
		c_r = fx_c * lrMixed;
		c_s = 0.0f;

		break;

	case OUTPUT_MODE_SURROUND:
	{
		f32 fl = left * front;
		f32 fr = right * front;
		f32 rs = surround * rear;

		m_l = main * fl;
		m_r = main * fr;
		m_s = main * rs;

		a_l = fx_a * fl;
		a_r = fx_a * fr;
		a_s = fx_a * rs;

		b_l = fx_b * fl;
		b_r = fx_b * fr;
		b_s = fx_b * rs;

		c_l = fx_c * fl;
		c_r = fx_c * fr;
		c_s = fx_c * rs;
	}
		break;

	case OUTPUT_MODE_DPL2:
	{
		f32 fl = left * front;
		f32 fr = right * front;
		f32 rl = left * rear;
		f32 rr = right * rear;

		m_l = main * fl;
		m_r = main * fr;
		m_sl = main * rl;
		m_sr = main * rr;

		a_l = fx_a * fl;
		a_r = fx_a * fr;
		a_sl = fx_a * rl;
		a_sr = fx_a * rr;

		b_l = fx_b * fl;
		b_r = fx_b * fr;
		b_sl = fx_b * rl;
		b_sr = fx_b * rr;
	}
		break;
	}

	f32 remoteOutVolume[4];
	for (int i = 0; i < 4; i++)
		remoteOutVolume[i] = lrMixed * remoteOutVolumeOrig[i];

	mix->vL			= CalcMixVolume(m_l);
	mix->vR			= CalcMixVolume(m_r);
	mix->vS			= CalcMixVolume(m_s);
	mix->vAuxAL		= CalcMixVolume(a_l);
	mix->vAuxAR		= CalcMixVolume(a_r);
	mix->vAuxAS		= CalcMixVolume(a_s);
	mix->vAuxBL		= CalcMixVolume(b_l);
	mix->vAuxBR		= CalcMixVolume(b_r);
	mix->vAuxBS		= CalcMixVolume(b_s);
	mix->vAuxCL		= CalcMixVolume(c_l);
	mix->vAuxCR		= CalcMixVolume(c_r);
	mix->vAuxCS		= CalcMixVolume(c_s);

	rmtmix->vMain0	= CalcMixVolume(remoteOutVolume[0]);
	rmtmix->vAux0	= 0;
	rmtmix->vMain1	= CalcMixVolume(remoteOutVolume[1]);
	rmtmix->vAux1	= 0;
	rmtmix->vMain2	= CalcMixVolume(remoteOutVolume[2]);
	rmtmix->vAux2	= 0;
	rmtmix->vMain3	= CalcMixVolume(remoteOutVolume[3]);
	rmtmix->vAux3	= 0;
}

void Voice::RunAllAxVoice()
{
	for (int channelIndex = 0; channelIndex < mChannelCount; channelIndex++)
	{
		for (int voiceOutIndex = 0; voiceOutIndex < mVoiceOutCount;
		     voiceOutIndex++)
		{
			if (AxVoice *axVoice = mAxVoice[channelIndex][voiceOutIndex])
				axVoice->Run();
		}
	}
}

void Voice::StopAllAxVoice()
{
	for (int channelIndex = 0; channelIndex < mChannelCount; channelIndex++)
	{
		for (int voiceOutIndex = 0; voiceOutIndex < mVoiceOutCount;
		     voiceOutIndex++)
		{
			if (AxVoice *axVoice = mAxVoice[channelIndex][voiceOutIndex])
				axVoice->Stop();
		}
	}
}

void Voice::InvalidateWaveData(void const *start, void const *end)
{
	bool disposeFlag = false;

	for (int channelIndex = 0; channelIndex < mChannelCount; channelIndex++)
	{
		AxVoice *axVoice = mAxVoice[channelIndex][0];

		if (axVoice && axVoice->IsDataAddressCoverd(start, end))
		{
			disposeFlag = true;
			break;
		}
	}

	if (disposeFlag)
	{
		Stop();

		if (mCallback)
			(*mCallback)(this, CALLBACK_STATUS_CANCEL, mCallbackData);
	}
}

}}} // namespace nw4r::snd::detail
