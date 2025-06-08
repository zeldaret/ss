#include "nw4r/snd/snd_WsdPlayer.h"

/* Original source:
 * kiwi515/ogws
 * src/nw4r/snd/snd_WsdPlayer.cpp
 */

/*******************************************************************************
 * headers
 */

#include "common.h"

#include "nw4r/snd/snd_BasicPlayer.h"
#include "nw4r/snd/snd_Channel.h"
#include "nw4r/snd/snd_DisposeCallbackManager.h"
#include "nw4r/snd/snd_global.h"
#include "nw4r/snd/snd_Voice.h"
#include "nw4r/snd/snd_SoundThread.h"
#include "nw4r/snd/snd_WaveFile.h"

#include "nw4r/ut/ut_algorithm.h" // ut::Min

#include "nw4r/NW4RAssert.hpp"

/*******************************************************************************
 * functions
 */

namespace nw4r { namespace snd { namespace detail {

WsdPlayer::WsdPlayer() :
	mActiveFlag	(false)
{
}

void WsdPlayer::InitParam(int voiceOutCount, WsdCallback const *callback,
                          u32 callbackData)
{
	BasicPlayer::InitParam();

	mStartedFlag				= false;
	mPauseFlag					= false;
	mReleasePriorityFixFlag		= false;
	mPanRange					= 1.0f;
	mVoiceOutCount				= voiceOutCount;
	mPriority					= DEFAULT_PRIORITY;
	mCallback					= callback;
	mCallbackData				= callbackData;
	mWsdData					= nullptr;
	mWsdIndex					= -1;
	mWaveSoundInfo.pitch		= 1.0f;
	mWaveSoundInfo.pan			= 64;
	mWaveSoundInfo.surroundPan	= 0;
	mWaveSoundInfo.fxSendA		= 0;
	mWaveSoundInfo.fxSendB		= 0;
	mWaveSoundInfo.fxSendC		= 0;
	mWaveSoundInfo.mainSend		= 127;
	mLfoParam.Init();

	mWavePlayFlag = false;

	mChannel = nullptr;
}

bool WsdPlayer::Prepare(void const *waveSoundBase, int index,
                        StartOffsetType startOffsetType, int startOffset,
                        int voiceOutCount, WsdCallback const *callback,
                        u32 callbackData)
{
	SoundThread::AutoLock lock;

	if (mActiveFlag)
		FinishPlayer();

	InitParam(voiceOutCount, callback, callbackData);

	mWsdData			= waveSoundBase;
	mWsdIndex			= index;
	mStartOffsetType	= startOffsetType;
	mStartOffset		= startOffset;

	DisposeCallbackManager::GetInstance().RegisterDisposeCallback(this);

	mActiveFlag = true;

	return true;
}

bool WsdPlayer::Start()
{
	SoundThread::AutoLock lock;

	SoundThread::GetInstance().RegisterPlayerCallback(this);

	mStartedFlag = true;

	return true;
}

void WsdPlayer::Stop()
{
	SoundThread::AutoLock lock;

	FinishPlayer();
}

void WsdPlayer::Pause(bool flag)
{
	SoundThread::AutoLock lock;

	mPauseFlag = static_cast<u8>(flag); // ???

	if (IsChannelActive() && flag != mChannel->IsPause())
		mChannel->Pause(flag);
}

void WsdPlayer::SetChannelPriority(int priority)
{
	// specificallly not the source variant
	NW4RAssertHeaderClampedLRValue_Line(229, priority, 0, 127);

	mPriority = priority;
}

void WsdPlayer::SetReleasePriorityFix(bool fix)
{
	mReleasePriorityFixFlag = fix;
}

void WsdPlayer::InvalidateData(void const *start, void const *end)
{
	SoundThread::AutoLock lock;

	if (mActiveFlag)
	{
		void const *current = GetWsdDataAddress();

		if (start <= current && current <= end)
			FinishPlayer();
	}
}

bool WsdPlayer::ReadWsdDataInfo(WsdDataInfo *info) const
{
	SoundThread::AutoLock lock;

	WaveSoundInfo wsInfo;
	WaveSoundNoteInfo wsNoteInfo;
	WaveInfo waveInfo;
	bool result = mCallback->GetWaveSoundData(&wsInfo, &wsNoteInfo, &waveInfo, mWsdData, mWsdIndex, 0, mCallbackData);
	if (result) {
		info->loopFlag = waveInfo.loopFlag != false;
		info->sampleRate = waveInfo.sampleRate;
		info->loopStart = waveInfo.loopStart;
		info->loopEnd = waveInfo.loopEnd;
	}
	return result;
}

void WsdPlayer::FinishPlayer()
{
	SoundThread::AutoLock lock;

	if (mStartedFlag)
	{
		SoundThread::GetInstance().UnregisterPlayerCallback(this);

		mStartedFlag = false;
	}

	if (mActiveFlag)
	{
		DisposeCallbackManager::GetInstance().UnregisterDisposeCallback(this);

		mActiveFlag = false;
	}

	CloseChannel();
}

u32 WsdPlayer::GetPlaySamplePosition() const
{
	SoundThread::AutoLock lock;

	if (!mChannel)
		return -1;

	return mChannel->GetVoice()->GetCurrentPlayingSample();
}

void WsdPlayer::Update()
{
	SoundThread::AutoLock lock;

	NW4RAssert_Line(362, mActiveFlag);
	if (!mActiveFlag)
		return;

	if (!mStartedFlag)
		return;

	if (!mPauseFlag)
	{
		if (mWavePlayFlag && !mChannel)
		{
			FinishPlayer();
			return;
		}

		if (!mWavePlayFlag && !StartChannel(mCallback, mCallbackData))
		{
			FinishPlayer();
			return;
		}
	}

	UpdateChannel();
}

bool WsdPlayer::StartChannel(WsdCallback const *callback,
                             u32 callbackData)
{
	SoundThread::AutoLock lock;

	int priority = DEFAULT_PRIORITY + GetChannelPriority();

	WaveInfo waveData;
	WaveSoundNoteInfo noteInfo;
	bool result =
		callback->GetWaveSoundData(&mWaveSoundInfo, &noteInfo, &waveData,
	                               mWsdData, mWsdIndex, 0, callbackData);
	if (!result)
		return false;

	u32 startOffsetSamples;
	if (mStartOffsetType == START_OFFSET_TYPE_SAMPLE)
	{
		startOffsetSamples = mStartOffset;
	}
	else if (mStartOffsetType == START_OFFSET_TYPE_MILLISEC)
	{
		startOffsetSamples =
			static_cast<s64>(mStartOffset) * waveData.sampleRate / 1000;
	}

	// NOTE: another case of start offset thing

	if (startOffsetSamples > waveData.loopEnd)
		return false;

	Channel *channel = Channel::AllocChannel(
		ut::Min(waveData.numChannels, Channel::CHANNEL_MAX), GetVoiceOutCount(),
		priority, ChannelCallbackFunc, reinterpret_cast<u32>(this));
	if (!channel)
		return false;

	channel->SetAttack(noteInfo.attack);
	channel->SetHold(noteInfo.hold);
	channel->SetDecay(noteInfo.decay);
	channel->SetSustain(noteInfo.sustain);
	channel->SetRelease(noteInfo.release);
	channel->SetReleasePriorityFix(mReleasePriorityFixFlag);

	channel->Start(waveData, -1, startOffsetSamples);
	mChannel = channel;

	mWavePlayFlag = true;

	return true;
}

void WsdPlayer::CloseChannel()
{
	SoundThread::AutoLock lock;

	if (IsChannelActive())
	{
		UpdateChannel();

		mChannel->Release();
	}

	if (mChannel)
		Channel::FreeChannel(mChannel);

	mChannel = nullptr;
}

void WsdPlayer::UpdateChannel()
{
	SoundThread::AutoLock lock;

	if (!mChannel)
		return;

	f32 volume = 1.0f;
	volume *= GetVolume();

	f32 pitchRatio = 1.0f;
	pitchRatio *= GetPitch();
	pitchRatio *= mWaveSoundInfo.pitch;

	f32 pan = 0.0f;
	if (mWaveSoundInfo.pan <= 1)
		pan += (mWaveSoundInfo.pan - 63) / 63.0f;
	else
		pan += (mWaveSoundInfo.pan - 64) / 63.0f;

	pan *= GetPanRange();
	pan += GetPan();

	f32 surroundPan = 0.0f;
	if (mWaveSoundInfo.surroundPan <= 1)
		surroundPan += (mWaveSoundInfo.surroundPan + 1) / 63.0f;
	else
		surroundPan += mWaveSoundInfo.surroundPan / 63.0f;

	surroundPan += mWaveSoundInfo.surroundPan / 64.0f;
	surroundPan += GetSurroundPan();

	f32 lpfFreq = 0.0f;
	lpfFreq += GetLpfFreq();

	int biquadType = GetBiquadType();
	f32 biquadValue = GetBiquadValue();

	int remoteFilter = 0;
	remoteFilter += GetRemoteFilter();

	f32 mainSend = 0.0f;
	mainSend += mWaveSoundInfo.mainSend / 127.0f - 1.0f;
	mainSend += GetMainSend();

	f32 fxSend[AUX_BUS_NUM];

	u8 infoSend[AUX_BUS_NUM];
	infoSend[AUX_A] = mWaveSoundInfo.fxSendA;
	infoSend[AUX_B] = mWaveSoundInfo.fxSendB;
	infoSend[AUX_C] = mWaveSoundInfo.fxSendC;

	for (int i = 0; i < AUX_BUS_NUM; i++)
	{
		fxSend[i] = 0.0f;
		fxSend[i] += infoSend[i] / 127.0f;
		fxSend[i] += GetFxSend(static_cast<AuxBus>(i));
	}

	mChannel->SetPanMode(GetPanMode());
	mChannel->SetPanCurve(GetPanCurve());
	mChannel->SetUserVolume(volume);
	mChannel->SetUserPitchRatio(pitchRatio);
	mChannel->SetUserPan(pan);
	mChannel->SetUserSurroundPan(surroundPan);
	mChannel->SetUserLpfFreq(lpfFreq);
	mChannel->SetBiquadFilter(biquadType, biquadValue);
	mChannel->SetRemoteFilter(remoteFilter);
	mChannel->SetOutputLine(GetOutputLine());
	mChannel->SetMainOutVolume(GetMainOutVolume());
	mChannel->SetMainSend(mainSend);

	for (int i = 0; i < AUX_BUS_NUM; i++)
	{
		AuxBus bus = static_cast<AuxBus>(i);

		mChannel->SetFxSend(bus, fxSend[i]);
	}

	for (int i = 0; i < 4; i++)
		mChannel->SetRemoteOutVolume(i, GetRemoteOutVolume(i));

	for (int i = 0; i < mVoiceOutCount; i++)
		mChannel->SetVoiceOutParam(i, GetVoiceOutParam(i));

	mChannel->SetLfoParam(mLfoParam);
}

void WsdPlayer::ChannelCallbackFunc(Channel *dropChannel,
                                    Channel::ChannelCallbackStatus status,
                                    u32 userData)
{
	SoundThread::AutoLock lock;

	WsdPlayer *player = reinterpret_cast<WsdPlayer *>(userData);

	NW4RAssertPointerNonnull_Line(643, dropChannel);
	NW4RAssertPointerNonnull_Line(644, player);
	NW4RAssert_Line(645, dropChannel == player->mChannel);

	if (status == Channel::CALLBACK_STATUS_FINISH)
		Channel::FreeChannel(dropChannel);

	player->mChannel = nullptr;
}

}}} // namespace nw4r::snd::detail
