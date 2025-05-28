#include "nw4r/snd/snd_BasicSound.h"

/* Original source:
 * kiwi515/ogws
 * src/nw4r/snd/snd_BasicSound.cpp
 */

/*******************************************************************************
 * headers
 */

#include <climits> // ULONG_MAX
#include <cstring>

#include "common.h"

#include "nw4r/snd/snd_BasicPlayer.h"
#include "nw4r/snd/snd_ExternalSoundPlayer.h"
#include "nw4r/snd/snd_global.h"
#include "nw4r/snd/snd_MoveValue.h"
#include "nw4r/snd/snd_SoundActor.h"
#include "nw4r/snd/snd_SoundHandle.h"
#include "nw4r/snd/snd_SoundPlayer.h"

#include "nw4r/ut/ut_algorithm.h" // ut::Clamp
#include "nw4r/ut/ut_RuntimeTypeInfo.h"

#include "nw4r/NW4RAssert.hpp"

/*******************************************************************************
 * types
 */

// forward declarations
namespace nw4r { namespace snd { namespace detail { class PlayerHeap; }}}

/*******************************************************************************
 * variables
 */

namespace nw4r { namespace snd { namespace detail
{
	ut::detail::RuntimeTypeInfo const BasicSound::typeInfo(nullptr);
}}} // namespace nw4r::snd::detail

/*******************************************************************************
 * functions
 */

namespace nw4r { namespace snd { namespace detail {

BasicSound::BasicSound(int priority, int ambientPriority) :
	mPlayerHeap			(nullptr),
	mGeneralHandle		(nullptr),
	mTempGeneralHandle	(nullptr),
	mSoundPlayer		(nullptr),
	mSoundActor			(nullptr),
	mExtSoundPlayer		(nullptr),
	mId					(INVALID_ID)
	// mPauseNestCounter	(0)
{
	// specifically not the source variant
	NW4RAssertHeaderClampedLRValue_Line(53, priority, PRIORITY_MIN,
	                                    PRIORITY_MAX);

	mAmbientInfo.paramUpdateCallback	= nullptr;
	mAmbientInfo.argUpdateCallback		= nullptr;
	mAmbientInfo.argAllocaterCallback	= nullptr;
	mAmbientInfo.arg					= nullptr;
	mAmbientInfo.argSize				= 0;

	mVoiceOutCount						= 1;
	mPriority							= priority;
	mAmbientParam.priority				= ambientPriority;
}

void BasicSound::InitParam()
{
	mPauseState			= PAUSE_STATE_NORMAL;
	mUnPauseFlag		= false;
	mStartFlag			= false;
	mStartedFlag		= false;
	mAutoStopFlag		= false;
	mFadeOutFlag		= false;
	mAutoStopCounter	= 0;
	mUpdateCounter		= 0;

	mFadeVolume.InitValue(0.0f);
	mPauseFadeVolume.InitValue(1.0f);
	mFadeVolume.SetTarget(1.0f, 1);

	mInitVolume			= 1.0f;
	mExtPitch			= 1.0f;
	mExtPan				= 0.0f;
	mExtSurroundPan		= 0.0f;

	mExtMoveVolume.InitValue(1.0f);

	// clang-format off
	mLpfFreq			= 0.0f;
	mBiquadFilterType	= 0;
	mBiquadFilterValue	= 0.0f;
	mOutputLineFlag		= mSoundPlayer ? mSoundPlayer->GetDefaultOutputLine() : 1;
	mMainOutVolume		= 1.0f;
	mMainSend			= 0.0f;
	// clang-format on

	for (int i = 0; i < AUX_BUS_NUM; i++)
		mFxSend[i] = 0.0f;

	for (int i = 0; i < 4; i++)
		mRemoteOutVolume[i] = 1.0f;

	mAmbientParam.volume			= 1.0f;
	mAmbientParam.pitch				= 1.0f;
	mAmbientParam.pan				= 0.0f;
	mAmbientParam.surroundPan		= 0.0f;
	mAmbientParam.fxSend			= 0.0f;
	mAmbientParam.lpf				= 0.0f;
	mAmbientParam.biquadFilterValue	= 0.0f;
	mAmbientParam.biquadFilterType	= 0;
	mAmbientParam.priority			= 0;

	// mPauseNestCounter = 0;
}

void BasicSound::StartPrepared()
{
	if (!mStartedFlag)
		mStartFlag = true;
}

void BasicSound::Stop(int fadeFrames)
{
	BasicPlayer &basicPlayer = GetBasicPlayer();

	if (fadeFrames == 0 || !basicPlayer.IsActive() || !basicPlayer.IsStarted()
	    || basicPlayer.IsPause())
	{
		Shutdown();
		return;
	}

	int frames = fadeFrames * mFadeVolume.GetValue();
	mFadeVolume.SetTarget(0.0f, frames);

	SetPlayerPriority(0);

	mAutoStopFlag = false;
	mPauseState = PAUSE_STATE_NORMAL;
	mUnPauseFlag = false;
	mFadeOutFlag = true;
}

void BasicSound::Pause(bool flag, int fadeFrames)
{
	int frames;

	if (flag)
	{
		// mPauseNestCounter++;

		switch (mPauseState)
		{
		case PAUSE_STATE_NORMAL:
		case PAUSE_STATE_PAUSING:
		case PAUSE_STATE_UNPAUSING:
			frames = fadeFrames * mPauseFadeVolume.GetValue();
			if (frames <= 0)
				frames = 1;

			mPauseFadeVolume.SetTarget(0.0f, frames);
			mPauseState = PAUSE_STATE_PAUSING;
			mUnPauseFlag = false;

			return;

		case PAUSE_STATE_PAUSED:
			return;

		default:
			NW4RPanicMessage_Line(207, "Unexpected pause state %d",
			                      mPauseState);
			return;
		}
	}
	else
	{
//		if (mPauseNestCounter && --mPauseNestCounter)
//			return;

		switch (mPauseState)
		{
		case PAUSE_STATE_NORMAL:
			return;

		case PAUSE_STATE_PAUSING:
		case PAUSE_STATE_PAUSED:
		case PAUSE_STATE_UNPAUSING:
			frames = fadeFrames * (1.0f - mPauseFadeVolume.GetValue());
			if (frames <= 0)
				frames = 1;

			mPauseFadeVolume.SetTarget(1.0f, frames);
			mPauseState = PAUSE_STATE_UNPAUSING;
			mUnPauseFlag = true;

			return;

		default:
			NW4RPanicMessage_Line(234, "Unexpected pause state %d",
			                      mPauseState);
			return;
		}
	}
}

void BasicSound::SetAutoStopCounter(int count)
{
	mAutoStopCounter = count;
	mAutoStopFlag = count > 0;
}

void BasicSound::FadeIn(int fadeFrames)
{
	if (!mFadeOutFlag)
	{
		int duration = fadeFrames * (1.0f - mFadeVolume.GetValue());
		mFadeVolume.SetTarget(1.0f, duration);
	}
}

bool BasicSound::IsPause() const
{
	return mPauseState == PAUSE_STATE_PAUSING
	    || mPauseState == PAUSE_STATE_PAUSED;
}

void BasicSound::Update()
{
	BasicPlayer &basicPlayer = GetBasicPlayer();

	if (mAutoStopFlag && basicPlayer.IsActive())
	{
		if (mAutoStopCounter == 0)
		{
			if (mPauseState == PAUSE_STATE_NORMAL
			    || mPauseState == PAUSE_STATE_UNPAUSING)
			{
				Stop(0);
				return;
			}
		}
		else
		{
			mAutoStopCounter--;
		}
	}

	bool playerStartFlag = false;
	if (!mStartedFlag)
	{
		if (!mStartFlag)
			return;

		if (!IsPrepared())
			return;

		playerStartFlag = true;
	}

	if (basicPlayer.IsStarted() && mUpdateCounter < ULONG_MAX)
		mUpdateCounter++;

	if (!basicPlayer.IsActive())
	{
		Shutdown();
		return;
	}

	switch (mPauseState)
	{
	case PAUSE_STATE_PAUSING:
		mPauseFadeVolume.Update();
		break;

	case PAUSE_STATE_UNPAUSING:
		mPauseFadeVolume.Update();
		UpdateMoveValue();
		break;

	case PAUSE_STATE_NORMAL:
		UpdateMoveValue();
		break;
	}

	if (mAmbientInfo.argUpdateCallback)
		mAmbientInfo.argUpdateCallback->at_0x0c(mAmbientInfo.arg, this);

	if (mAmbientInfo.paramUpdateCallback)
	{
		SoundAmbientParam ambientParam;

		if (mUpdateCounter != 0)
		{
			ambientParam.volume				= mAmbientParam.volume;
			ambientParam.pitch				= mAmbientParam.pitch;
			ambientParam.pan				= mAmbientParam.pan;
			ambientParam.surroundPan		= mAmbientParam.surroundPan;
			ambientParam.fxSend				= mAmbientParam.fxSend;
			ambientParam.lpf				= mAmbientParam.lpf;
			ambientParam.biquadFilterValue	= mAmbientParam.biquadFilterValue;
			ambientParam.biquadFilterType	= mAmbientParam.biquadFilterType;
			ambientParam.priority			= mAmbientParam.priority;
			ambientParam.field_0x24			= mAmbientParam.field_0x24;
		}
		else
		{
			ambientParam.field_0x24 = 0;
		}

		for (int i = 0; i < mVoiceOutCount; i++)
			ambientParam.voiceOutParam[i] = basicPlayer.GetVoiceOutParam(i);

		mAmbientInfo.paramUpdateCallback->detail_UpdateAmbientParam(
			mAmbientInfo.arg, mId, mVoiceOutCount, &ambientParam);

		mAmbientParam.volume			= ambientParam.volume;
		mAmbientParam.pitch				= ambientParam.pitch;
		mAmbientParam.pan				= ambientParam.pan;
		mAmbientParam.surroundPan		= ambientParam.surroundPan;
		mAmbientParam.fxSend			= ambientParam.fxSend;
		mAmbientParam.lpf				= ambientParam.lpf;
		mAmbientParam.biquadFilterValue	= ambientParam.biquadFilterValue;
		mAmbientParam.biquadFilterType	= ambientParam.biquadFilterType;
		mAmbientParam.priority			= ambientParam.priority;
		mAmbientParam.field_0x24		= ambientParam.field_0x24;

		for (int i = 0; i < mVoiceOutCount; i++)
			basicPlayer.SetVoiceOutParam(i, ambientParam.voiceOutParam[i]);
	}

	if (mSoundActor)
		mActorParam = mSoundActor->detail_GetActorParam();

	UpdateParam();

	if (mFadeOutFlag && mFadeVolume.IsFinished())
	{
		mFadeOutFlag = false;

		Shutdown();
		return;
	}

	if (playerStartFlag)
	{
		if (basicPlayer.Start())
		{
			mStartedFlag = true;
			mStartFlag = false;
		}
		else
		{
			Shutdown();
			return;
		}
	}

	if (mPauseState == PAUSE_STATE_PAUSING)
	{
		if (mPauseFadeVolume.IsFinished())
		{
			basicPlayer.Pause(true);
			mPauseState = PAUSE_STATE_PAUSED;
		}
	}
	else if (mPauseState == PAUSE_STATE_UNPAUSING)
	{
		if (mPauseFadeVolume.IsFinished())
			mPauseState = PAUSE_STATE_NORMAL;
	}

	if (mUnPauseFlag)
	{
		basicPlayer.Pause(false);
		mUnPauseFlag = false;
	}
}

void BasicSound::UpdateMoveValue()
{
	mFadeVolume.Update();
	mExtMoveVolume.Update();
}

void BasicSound::UpdateParam()
{
	f32 volume = 1.0f;
	volume *= mInitVolume;
	volume *= GetSoundPlayer()->GetVolume();
	volume *= mExtMoveVolume.GetValue();
	volume *= mFadeVolume.GetValue();
	volume *= mPauseFadeVolume.GetValue();
	volume *= mAmbientParam.volume;
	volume *= mActorParam.volume;

	f32 pan = 0.0f;
	pan += mExtPan;
	pan += mAmbientParam.pan;
	pan += mActorParam.pan;

	f32 surroundPan = 0.0f;
	surroundPan += mExtSurroundPan;
	surroundPan += mAmbientParam.surroundPan;

	f32 pitch = 1.0f;
	pitch *= mExtPitch;
	pitch *= mAmbientParam.pitch;
	pitch *= mActorParam.pitch;

	f32 lpfFreq = mLpfFreq;
	lpfFreq += mAmbientParam.lpf;
	lpfFreq += GetSoundPlayer()->GetLpfFreq();

	int biquadFilterType = mBiquadFilterType;
	f32 biquadFilterValue = mBiquadFilterValue;

	if (biquadFilterType == 0)
	{
		biquadFilterType = GetSoundPlayer()->GetBiquadFilterType();
		biquadFilterValue = GetSoundPlayer()->GetBiquadFilterValue();

		if (biquadFilterType == 0)
		{
			biquadFilterType = mAmbientParam.biquadFilterType;
			biquadFilterValue = mAmbientParam.biquadFilterValue;
		}
	}

	int outputLineFlag = mOutputLineFlag;

	f32 mainOutVolume = 1.0f;
	mainOutVolume *= mMainOutVolume;
	mainOutVolume *= GetSoundPlayer()->GetMainOutVolume();

	f32 remoteOutVolume[4];
	for (int i = 0; i < 4; i++) {
		remoteOutVolume[i] = 1.0f;
		remoteOutVolume[i] *= GetSoundPlayer()->GetRemoteOutVolume(i);
		remoteOutVolume[i] *= mRemoteOutVolume[i];
	}

	f32 mainSend = 0.0f;
	mainSend += mMainSend;
	mainSend += GetSoundPlayer()->GetMainSend();

	f32 fxSend[AUX_BUS_NUM];
	for (int i = 0; i < AUX_BUS_NUM; i++)
	{
		fxSend[i] = 0.0f;
		fxSend[i] += mFxSend[i];
		fxSend[i] += GetSoundPlayer()->GetFxSend(i);
	}

	fxSend[AUX_A] += mAmbientParam.fxSend;

	BasicPlayer &basicPlayer = GetBasicPlayer();
	basicPlayer.SetVolume(volume);
	basicPlayer.SetPan(pan);
	basicPlayer.SetSurroundPan(surroundPan);
	basicPlayer.SetPitch(pitch);
	basicPlayer.SetLpfFreq(lpfFreq);
	basicPlayer.SetBiquadFilter(biquadFilterType, biquadFilterValue);
	basicPlayer.SetOutputLine(outputLineFlag);
	basicPlayer.SetMainOutVolume(mainOutVolume);

	for (int i = 0; i < 4; i++) {
		basicPlayer.SetRemoteOutVolume(i, remoteOutVolume[i]);
	}

	basicPlayer.SetMainSend(mainSend);

	for (int i = 0; i < AUX_BUS_NUM; i++)
		basicPlayer.SetFxSend(static_cast<AuxBus>(i), fxSend[i]);
}

void BasicSound::Shutdown()
{
	BasicPlayer &basicPlayer = GetBasicPlayer();

	if (basicPlayer.IsActive())
	{
		if (mFadeOutFlag)
			basicPlayer.SetVolume(0.0f);

		basicPlayer.Stop();
	}

	SetId(INVALID_ID);

	if (IsAttachedGeneralHandle())
		DetachGeneralHandle();

	if (IsAttachedTempGeneralHandle())
		DetachTempGeneralHandle();

	if (IsAttachedTempSpecialHandle())
		DetachTempSpecialHandle();

	if (mPlayerHeap)
		mSoundPlayer->detail_FreePlayerHeap(this);

	if (mSoundPlayer)
		mSoundPlayer->detail_RemoveSound(this);

	if (mExtSoundPlayer)
		mExtSoundPlayer->RemoveSound(this);

	if (mAmbientInfo.argAllocaterCallback)
	{
		mAmbientInfo.argAllocaterCallback->detail_FreeAmbientArg(mAmbientInfo.arg, this);

		mAmbientInfo.arg = nullptr;
	}

	mStartedFlag = false;
	mFadeOutFlag = false;
}

void BasicSound::AttachPlayerHeap(PlayerHeap *heap)
{
	NW4RAssertPointerNonnull_Line(615, heap);
	NW4RAssert_Line(616, mPlayerHeap == NULL);

	mPlayerHeap = heap;
}

void BasicSound::DetachPlayerHeap(PlayerHeap *heap)
{
	NW4RAssertPointerNonnull_Line(632, heap);
	NW4RAssert_Line(633, heap == mPlayerHeap);

	mPlayerHeap = nullptr;
}

void BasicSound::AttachSoundPlayer(SoundPlayer *player)
{
	NW4RAssertPointerNonnull_Line(650, player);
	NW4RAssert_Line(651, mSoundPlayer == NULL);

	mSoundPlayer = player;
}

void BasicSound::DetachSoundPlayer(SoundPlayer *player)
{
	NW4RAssertPointerNonnull_Line(667, player);
	NW4RAssert_Line(668, player == mSoundPlayer);

	mSoundPlayer = nullptr;
}

void BasicSound::AttachSoundActor(SoundActor *actor)
{
	NW4RAssertPointerNonnull_Line(685, actor);
	NW4RAssert_Line(686, mSoundActor == NULL);

	mSoundActor = actor;
}

void BasicSound::DetachSoundActor(SoundActor *actor)
{
	NW4RAssertPointerNonnull_Line(702, actor);
	NW4RAssert_Line(703, actor == mSoundActor);

	mSoundActor = nullptr;
}

void BasicSound::AttachExternalSoundPlayer(ExternalSoundPlayer *extPlayer)
{
	NW4RAssertPointerNonnull_Line(720, extPlayer);
	NW4RAssert_Line(721, mExtSoundPlayer == NULL);

	mExtSoundPlayer = extPlayer;
}

void BasicSound::DetachExternalSoundPlayer(ExternalSoundPlayer *extPlayer)
{
	NW4RAssertPointerNonnull_Line(737, extPlayer);
	NW4RAssert_Line(738, extPlayer == mExtSoundPlayer);

	mExtSoundPlayer = nullptr;
}

int BasicSound::GetRemainingFadeFrames() const
{
	return mPauseFadeVolume.GetRemainingTime();
}

int BasicSound::GetVoiceOutCount() const
{
	return mVoiceOutCount;
}

void BasicSound::SetPlayerPriority(int priority)
{
	// specifically not the source variant
	NW4RAssertHeaderClampedLRValue_Line(797, priority, PRIORITY_MIN,
	                                    PRIORITY_MAX);

	mPriority = priority;

	if (mSoundPlayer)
		mSoundPlayer->detail_SortPriorityList(this);

	OnUpdatePlayerPriority();
}

void BasicSound::SetInitialVolume(f32 volume)
{
	NW4RAssert_Line(818, volume >= 0.0f);
	if (volume < 0.0f) {
		volume = 0.0f;
	}

	mInitVolume = mInitVolume = volume;
}

void BasicSound::SetVolume(f32 volume, int frames)
{
	NW4RAssert_Line(833, volume >= 0.0f);
	if (volume < 0.0f) {
		volume = 0.0f;
	}

	mExtMoveVolume.SetTarget(volume, frames);
}

void BasicSound::SetPitch(f32 pitch)
{
	NW4RAssert_Line(848, pitch >= 0.0f);

	mExtPitch = pitch;
}

void BasicSound::SetPan(f32 pan)
{
	mExtPan = pan;
}

void BasicSound::SetSurroundPan(f32 pan)
{
	mExtSurroundPan = pan;
}

void BasicSound::SetLpfFreq(f32 freq)
{
	mLpfFreq = freq;
}

void BasicSound::SetOutputLineFlag(int flag)
{
	mOutputLineFlag = flag;
}

void BasicSound::SetFxSend(AuxBus bus, f32 send)
{
	// specifically not the source variant
	NW4RAssertHeaderClampedLValue_Line(979, bus, AUX_A, AUX_BUS_NUM);

	mFxSend[bus] = send;
}

void BasicSound::SetRemoteFilter(int filter)
{
	GetBasicPlayer().SetRemoteFilter(filter);
}

void BasicSound::SetPanMode(PanMode panMode)
{
	GetBasicPlayer().SetPanMode(panMode);
}

void BasicSound::SetPanCurve(PanCurve panCurve)
{
	GetBasicPlayer().SetPanCurve(panCurve);
}

void BasicSound::SetAmbientInfo(AmbientInfo const &ambientArgInfo)
{
	NW4RAssertPointerNonnull_Line(1090, ambientArgInfo.argAllocaterCallback);

	void *ambientArg =
		ambientArgInfo.argAllocaterCallback->detail_AllocAmbientArg(ambientArgInfo.argSize);
	if (!ambientArg)
	{
		NW4RCheckMessage_Line(1093, ambientArg, "Failed to alloc AmbientArg.");
		return;
	}

	std::memcpy(ambientArg, ambientArgInfo.arg, ambientArgInfo.argSize);
	mAmbientInfo = ambientArgInfo;
	mAmbientInfo.arg = ambientArg;

	if (ambientArgInfo.paramUpdateCallback)
	{
		int voiceOutCount =
			mAmbientInfo.paramUpdateCallback->detail_GetRequiredVoiceOutCount(mAmbientInfo.arg, mId);

		if (voiceOutCount > 4)
			voiceOutCount = 4;

		mVoiceOutCount = voiceOutCount;
	}
}

int BasicSound::GetAmbientPriority(AmbientInfo const &ambientInfo, u32 soundId)
{
	if (!ambientInfo.paramUpdateCallback)
		return PRIORITY_MIN;

	int priority =
		ambientInfo.paramUpdateCallback->detail_GetAmbientPriority(ambientInfo.arg, soundId);

	return priority;
}

bool BasicSound::IsAttachedGeneralHandle()
{
	return mGeneralHandle != nullptr;
}

bool BasicSound::IsAttachedTempGeneralHandle()
{
	return mTempGeneralHandle != nullptr;
}

void BasicSound::DetachGeneralHandle()
{
	mGeneralHandle->DetachSound();
}

void BasicSound::DetachTempGeneralHandle()
{
	mTempGeneralHandle->DetachSound();
}

void BasicSound::SetId(u32 id)
{
	mId = id;

	GetBasicPlayer().SetId(id);
}

}}} // namespace nw4r::snd::detail
