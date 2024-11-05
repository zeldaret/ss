#include "nw4r/snd.h"

#include "nw4r/snd/snd_Common.h"
#include "rvl/AX.h"

namespace nw4r {
namespace snd {
namespace detail {

ChannelManager& ChannelManager::GetInstance() {
    static ChannelManager instance;
    return instance;
}

ChannelManager::ChannelManager() : mInitialized(false), mChannelCount(0) {}

u32 ChannelManager::GetRequiredMemSize(int numVoices) {
    return (numVoices + VOICE_MARGIN) * sizeof(Channel);
}

void ChannelManager::Setup(void* pWork, u32 workSize) {
    ut::AutoInterruptLock lock;

    if (mInitialized) {
        return;
    }

    mChannelCount = mPool.Create(pWork, workSize);
    mMem = pWork;
    mMemSize = workSize;
    mInitialized = true;
}

void ChannelManager::Shutdown() {
    ut::AutoInterruptLock lock;

    if (!mInitialized) {
        return;
    }

    NW4R_UT_LIST_SAFE_FOREACH(mChannelList,
        it->Stop();
    );

    mPool.Destroy(mMem, mMemSize);
    mInitialized = false;
}

Channel* ChannelManager::Alloc() {
    Channel* pChannel = mPool.Alloc();
    mChannelList.PushBack(pChannel);
    return pChannel;
}

void ChannelManager::Free(Channel* pChannel) {
    mChannelList.Erase(pChannel);
    mPool.Free(pChannel);
}

void ChannelManager::UpdateAllChannel() {
    NW4R_UT_LIST_SAFE_FOREACH(mChannelList,
        it->Update(true);
    );
}

Channel::Channel()
    : mPauseFlag(false), mActiveFlag(false), mAllocFlag(false), mVoice(NULL) {}

Channel::~Channel() {}

void Channel::InitParam(ChannelCallback pCallback, u32 callbackArg) {
    mNextLink = NULL;

    mCallback = pCallback;
    mCallbackData = callbackArg;
    waveDataLocationCallback = NULL;
    mWaveInfo = NULL;

    mPauseFlag = false;
    mAutoSweep = true;
    mReleasePriorityFixFlag = false;
    field_0x3A = false;

    mLength = 0;
    mKey = KEY_INIT;
    mOriginalKey = ORIGINAL_KEY_INIT;

    mInitVolume = 1.0f;
    mInitPan = 0.0f;
    mInitSurroundPan = 0.0f;
    mTune = 1.0f;

    mUserVolume = 1.0f;
    mUserPitch = 0.0f;
    mUserPitchRatio = 1.0f;
    mUserPan = 0.0f;
    mUserSurroundPan = 0.0f;
    mUserLpfFreq = 0.0f;

    mBiquadFilter = 0;
    mBiquadValue = 0.0f;
    mRemoteFilter = 0;

    mOutputLineFlag = OUTPUT_LINE_MAIN;

    mMainOutVolume = 1.0f;
    mMainSend = 0.0f;

    for (int i = 0; i < AUX_BUS_NUM; i++) {
        mFxSend[i] = 0.0f;
    }

    for (int i = 0; i < WPAD_MAX_CONTROLLERS; i++) {
        mRemoteOutVolume[i] = 1.0f;
    }

    mSilenceVolume.InitValue(SILENCE_VOLUME_MAX);

    mSweepPitch = 0.0f;
    mSweepLength = 0;
    mSweepCounter = 0;

    mEnvelope.Init();
    mLfo.GetParam().Init();

    mLfoTarget = LFO_TARGET_PITCH;
    mPanMode = PAN_MODE_DUAL;
    mPanCurve = PAN_CURVE_SQRT;
    mAlternateAssign = 0;
}

void Channel::Update(bool periodic) {
    if (!mActiveFlag) {
        return;
    }

    if (mPauseFlag) {
        periodic = false;
    }

    f32 lfoValue = mLfo.GetValue();
    mSilenceVolume.Update();

    f32 volume = 1.0f;
    volume *= mInitVolume;
    volume *= mUserVolume;
    volume *= mSilenceVolume.GetValue() / static_cast<f32>(SILENCE_VOLUME_MAX);

    f32 veInitVolume = 1.0f;
    veInitVolume *= Util::CalcVolumeRatio(mEnvelope.GetValue());
    if (mLfoTarget == LFO_TARGET_VOLUME) {
        veInitVolume *= Util::CalcVolumeRatio(VOLUME_MAX_DB * lfoValue);
    }

    if (mEnvelope.GetStatus() == EnvGenerator::STATUS_RELEASE) {
        if (mCallback != NULL) {
            if (veInitVolume == 0.0f) {
                Stop();
                return;
            }
        } else if (volume * veInitVolume == 0.0f) {
            Stop();
            return;
        }
    }

    f32 cent = 0.0f;
    cent += mKey - mOriginalKey;
    cent += GetSweepValue();
    cent += mUserPitch;
    if (mLfoTarget == LFO_TARGET_PITCH) {
        cent += lfoValue;
    }

    f32 pitchRatio = 1.0f;
    pitchRatio *= mTune;
    pitchRatio *= mUserPitchRatio;

    f32 pitch = Util::CalcPitchRatio(MICROTONE_MAX * cent);
    pitch *= pitchRatio;

    f32 pan = 0.0f;
    pan += mInitPan;
    pan += mUserPan;
    if (mLfoTarget == LFO_TARGET_PAN) {
        pan += lfoValue;
    }

    f32 surroundPan = 0.0f;
    surroundPan += mInitSurroundPan;
    surroundPan += mUserSurroundPan;

    f32 lpfFreq = 1.0f;
    lpfFreq += mUserLpfFreq;


    int remoteFilter = 0;
    remoteFilter += mRemoteFilter;

    f32 mainOutVolume = 1.0f;
    mainOutVolume *= mMainOutVolume;

    f32 mainSend = 0.0f;
    mainSend += mMainSend;

    f32 fxSend[AUX_BUS_NUM];
    for (int i = 0; i < AUX_BUS_NUM; i++) {
        fxSend[i] = 0.0f;
        fxSend[i] += mFxSend[i];
    }

    f32 remoteOutVolume[WPAD_MAX_CONTROLLERS];
    f32 remoteSend[WPAD_MAX_CONTROLLERS];
    f32 remoteFxSend[WPAD_MAX_CONTROLLERS];
    for (int i = 0; i < WPAD_MAX_CONTROLLERS; i++) {
        remoteOutVolume[i] = 1.0f;
        remoteOutVolume[i] *= mRemoteOutVolume[i];
    }

    if (periodic) {
        if (mAutoSweep) {
            UpdateSweep(3);
        }

        mLfo.Update(3);
        mEnvelope.Update(3);
    }

    f32 nextLfoValue = mLfo.GetValue();

    f32 veTargetVolume = 1.0f;
    veTargetVolume *= Util::CalcVolumeRatio(mEnvelope.GetValue());
    if (mLfoTarget == LFO_TARGET_VOLUME) {
        veTargetVolume *= Util::CalcVolumeRatio(VOLUME_MAX_DB * nextLfoValue);
    }

    if (mVoice != NULL) {
        mVoice->SetPanMode(mPanMode);
        mVoice->SetPanCurve(mPanCurve);
        mVoice->SetVolume(volume);
        mVoice->SetVeVolume(veTargetVolume, veInitVolume);
        mVoice->SetPitch(pitch);
        mVoice->SetPan(pan);
        mVoice->SetSurroundPan(surroundPan);
        mVoice->SetLpfFreq(lpfFreq);
        mVoice->SetBiquadFilter(mBiquadFilter, mBiquadValue);
        mVoice->SetRemoteFilter(remoteFilter);
        mVoice->SetOutputLine(mOutputLineFlag);
        mVoice->SetMainOutVolume(mainOutVolume);
        mVoice->SetMainSend(mainSend);

        for (int i = 0; i < AUX_BUS_NUM; i++) {
            mVoice->SetFxSend(static_cast<AuxBus>(i), fxSend[i]);
        }

        for (int i = 0; i < WPAD_MAX_CONTROLLERS; i++) {
            mVoice->SetRemoteOutVolume(i, remoteOutVolume[i]);
        }
    }
}

void Channel::Start(const WaveInfo& rData, int length, u32 offset) {
    mLength = length;

    mLfo.Reset();
    mEnvelope.Reset();
    mSweepCounter = 0;

    mVoice->Setup(rData, offset);
    mVoice->Start();
    mActiveFlag = true;
}

void Channel::Release() {
    if (mEnvelope.GetStatus() != EnvGenerator::STATUS_RELEASE) {
        if (mVoice != NULL && !mReleasePriorityFixFlag) {
            mVoice->SetPriority(PRIORITY_RELEASE);
        }

        mEnvelope.SetStatus(EnvGenerator::STATUS_RELEASE);
    }

    mPauseFlag = false;
}

void Channel::NoteOff() {
    if (field_0x3A == 0) {
        Release();
    }
}

void Channel::Stop() {
    if (mVoice == NULL) {
        return;
    }

    mVoice->Stop();
    mVoice->Free();
    mVoice = NULL;

    mPauseFlag = false;
    mActiveFlag = false;

    if (mCallback != NULL) {
        mCallback(this, CALLBACK_STATUS_STOPPED, mCallbackData);
    }

    if (waveDataLocationCallback != NULL) {
        waveDataLocationCallback->Callback_0x0C(mWaveInfo);
    }

    if (mAllocFlag) {
        mAllocFlag = false;
        ChannelManager::GetInstance().Free(this);
    }

}

void Channel::UpdateSweep(int count) {
    mSweepCounter += count;

    if (mSweepCounter > mSweepLength) {
        mSweepCounter = mSweepLength;
    }
}

void Channel::SetSweepParam(f32 pitch, int time, bool autoUpdate) {
    mSweepPitch = pitch;
    mSweepLength = time;
    mAutoSweep = autoUpdate;
    mSweepCounter = 0;
}

void Channel::SetBiquadFilter(int enable, f32 value) {
    mBiquadFilter = enable;
    mBiquadValue = value;
}

f32 Channel::GetSweepValue() const {
    if (mSweepPitch == 0.0f) {
        return 0.0f;
    }

    if (mSweepCounter >= mSweepLength) {
        return 0.0f;
    }

    f32 sweep = mSweepPitch;
    sweep *= mSweepLength - mSweepCounter;
    sweep /= mSweepLength;

    return sweep;
}

void Channel::VoiceCallbackFunc(Voice* pDropVoice,
                                Voice::VoiceCallbackStatus status,
                                void* pCallbackArg) {
    ChannelCallbackStatus channelStatus;

    switch (status) {
    case Voice::CALLBACK_STATUS_FINISH_WAVE: {
        channelStatus = CALLBACK_STATUS_FINISH;
        pDropVoice->Free();
        break;
    }

    case Voice::CALLBACK_STATUS_CANCEL: {
        channelStatus = CALLBACK_STATUS_CANCEL;
        pDropVoice->Free();
        break;
    }
    case Voice::CALLBACK_STATUS_DROP_VOICE: {
        channelStatus = CALLBACK_STATUS_DROP;
        break;
    }

    case Voice::CALLBACK_STATUS_DROP_DSP: {
        channelStatus = CALLBACK_STATUS_DROP;
        break;
    }
    }

    Channel* pChannel = static_cast<Channel*>(pCallbackArg);

    if (pChannel->mCallback != NULL) {
        pChannel->mCallback(pChannel, channelStatus, pChannel->mCallbackData);
    }

    if (pChannel->waveDataLocationCallback != NULL) {
        pChannel->waveDataLocationCallback->Callback_0x0C(pChannel->mWaveInfo);
    }

    pChannel->mVoice = NULL;
    pChannel->mPauseFlag = false;
    pChannel->mActiveFlag = false;
    pChannel->mAllocFlag = false;

    ChannelManager::GetInstance().Free(pChannel);
}

Channel* Channel::AllocChannel(int channels, int voices, int priority,
                               ChannelCallback pCallback, u32 callbackArg) {
    Channel* pChannel = ChannelManager::GetInstance().Alloc();
    if (pChannel == NULL) {
        return NULL;
    }

    pChannel->mAllocFlag = true;

    Voice* pVoice = VoiceManager::GetInstance().AllocVoice(
        channels, voices, priority, VoiceCallbackFunc, pChannel);

    if (pVoice == NULL) {
        ChannelManager::GetInstance().Free(pChannel);
        return NULL;
    }

    pChannel->mVoice = pVoice;
    pChannel->InitParam(pCallback, callbackArg);
    return pChannel;
}

void Channel::FreeChannel(Channel* pChannel) {
    if (pChannel == NULL) {
        return;
    }

    pChannel->mCallback = NULL;
    pChannel->mCallbackData = 0;
}

} // namespace detail
} // namespace snd
} // namespace nw4r
