#include "nw4r/snd/snd_WsdPlayer.h"

#include "nw4r/snd/snd_DisposeCallbackManager.h"
#include "nw4r/snd/snd_WaveFile.h"

namespace nw4r {
namespace snd {
namespace detail {

WsdPlayer::WsdPlayer() : mActiveFlag(false) {}

void WsdPlayer::InitParam(int voices, const WsdCallback *pCallback, u32 callbackArg) {
    BasicPlayer::InitParam();

    mStartedFlag = false;
    mPauseFlag = false;
    mReleasePriorityFixFlag = false;

    mPanRange = 1.0f;
    mVoiceOutCount = voices;
    mPriority = DEFAULT_PRIORITY;

    mCallback = pCallback;
    mCallbackData = callbackArg;

    mWsdData = NULL;
    mWsdIndex = -1;

    mWaveSoundInfo.pitch = 1.0f;
    mWaveSoundInfo.pan = 64;
    mWaveSoundInfo.surroundPan = 0;
    mWaveSoundInfo.fxSendA = 0;
    mWaveSoundInfo.fxSendB = 0;
    mWaveSoundInfo.fxSendC = 0;
    mWaveSoundInfo.mainSend = 127;

    mLfoParam.Init();
    mWavePlayFlag = false;
    mChannel = NULL;
}

bool WsdPlayer::Prepare(
    const void *pWsdData, int index, StartOffsetType startType, int startOffset, int voices,
    const WsdCallback *pCallback, u32 callbackArg
) {
    SoundThread::AutoLock lock;

    if (mActiveFlag) {
        FinishPlayer();
    }

    InitParam(voices, pCallback, callbackArg);

    mWsdData = pWsdData;
    mWsdIndex = index;
    mStartOffsetType = startType;
    mStartOffset = startOffset;

    DisposeCallbackManager::GetInstance().RegisterDisposeCallback(this);
    mActiveFlag = true;

    return true;
}

bool WsdPlayer::Start() {
    SoundThread::AutoLock lock;

    SoundThread::GetInstance().RegisterPlayerCallback(this);
    mStartedFlag = true;

    return true;
}

void WsdPlayer::Stop() {
    SoundThread::AutoLock lock;

    FinishPlayer();
}

void WsdPlayer::Pause(bool flag) {
    SoundThread::AutoLock lock;

    mPauseFlag = static_cast<u8>(flag);

    if (IsChannelActive() && flag != mChannel->IsPause()) {
        mChannel->Pause(flag);
    }
}

void WsdPlayer::SetChannelPriority(int priority) {
    mPriority = priority;
}

void WsdPlayer::SetReleasePriorityFix(bool flag) {
    mReleasePriorityFixFlag = flag;
}

void WsdPlayer::InvalidateData(const void *pStart, const void *pEnd) {
    SoundThread::AutoLock lock;

    if (mActiveFlag) {
        const void *pWsdData = GetWsdDataAddress();

        if (pStart <= pWsdData && pWsdData <= pEnd) {
            FinishPlayer();
        }
    }
}

bool WsdPlayer::ReadWsdDataInfo(WsdDataInfo *info) const {
    SoundThread::AutoLock lock;
    WaveSoundInfo waveInfo;
    WaveSoundNoteInfo noteData;
    WaveInfo waveData;
    bool ok = mCallback->GetWaveSoundData(&waveInfo, &noteData, &waveData, mWsdData, mWsdIndex, 0, mCallbackData);

    if (ok) {
        info->loopFlag = waveData.loopFlag;
        info->sampleRate = waveData.sampleRate;
        info->loopStart = waveData.loopStart;
        info->loopEnd = waveData.loopEnd;
    }

    return ok;
}

int WsdPlayer::GetPlaySamplePosition() const {
    SoundThread::AutoLock lock;
    if (mChannel == NULL) {
        return -1;
    }
    return mChannel->GetCurrentPlayingSample();
}

void WsdPlayer::FinishPlayer() {
    SoundThread::AutoLock lock;

    if (mStartedFlag) {
        SoundThread::GetInstance().UnregisterPlayerCallback(this);
        mStartedFlag = false;
    }

    if (mActiveFlag) {
        DisposeCallbackManager::GetInstance().UnregisterDisposeCallback(this);
        mActiveFlag = false;
    }

    CloseChannel();
}

void WsdPlayer::Update() {
    SoundThread::AutoLock lock;

    if (!mActiveFlag) {
        return;
    }

    if (!mStartedFlag) {
        return;
    }

    if (!mPauseFlag) {
        if (mWavePlayFlag && mChannel == NULL) {
            FinishPlayer();
            return;
        }

        if (!mWavePlayFlag && !StartChannel(mCallback, mCallbackData)) {
            FinishPlayer();
            return;
        }
    }

    UpdateChannel();
}

bool WsdPlayer::StartChannel(const WsdCallback *pCallback, u32 callbackArg) {
    SoundThread::AutoLock lock;

    int priority = GetChannelPriority() + DEFAULT_PRIORITY;

    WaveInfo waveData;
    WaveSoundNoteInfo waveSoundNoteInfo;
    if (!pCallback->GetWaveSoundData(
            &mWaveSoundInfo, &waveSoundNoteInfo, &waveData, mWsdData, mWsdIndex, 0, callbackArg
        )) {
        return false;
    }

    u32 startSample;
    if (mStartOffsetType == START_OFFSET_TYPE_SAMPLE) {
        startSample = mStartOffset;
    } else if (mStartOffsetType == START_OFFSET_TYPE_MILLISEC) {
        startSample = (static_cast<s64>(mStartOffset) * waveData.sampleRate) / 1000;
    }

    if (startSample > waveData.loopEnd) {
        return false;
    }

    Channel *pChannel = Channel::AllocChannel(
        ut::Min<int>(waveData.numChannels, CHANNEL_MAX), GetVoiceOutCount(), priority, ChannelCallbackFunc,
        reinterpret_cast<u32>(this)
    );

    if (pChannel == NULL) {
        return false;
    }

    pChannel->SetAttack(waveSoundNoteInfo.attack);
    pChannel->SetHold(waveSoundNoteInfo.hold);
    pChannel->SetDecay(waveSoundNoteInfo.decay);
    pChannel->SetSustain(waveSoundNoteInfo.sustain);
    pChannel->SetRelease(waveSoundNoteInfo.release);
    pChannel->SetReleasePriorityFix(mReleasePriorityFixFlag);

    pChannel->Start(waveData, -1, startSample);
    mChannel = pChannel;
    mWavePlayFlag = true;

    return true;
}

void WsdPlayer::CloseChannel() {
    SoundThread::AutoLock lock;

    if (IsChannelActive()) {
        UpdateChannel();
        mChannel->Release();
    }

    if (mChannel != NULL) {
        Channel::FreeChannel(mChannel);
    }

    mChannel = NULL;
}

void WsdPlayer::UpdateChannel() {
    SoundThread::AutoLock lock;

    if (mChannel == NULL) {
        return;
    }

    f32 volume = 1.0f;
    volume *= GetVolume();

    f32 pitchRatio = 1.0f;
    pitchRatio *= GetPitch();
    pitchRatio *= mWaveSoundInfo.pitch;

    f32 pan = 0.0f;
    if (mWaveSoundInfo.pan <= 1) {
        pan += (mWaveSoundInfo.pan - 63) / 63.0f;
    } else {
        pan += (mWaveSoundInfo.pan - 64) / 63.0f;
    }

    pan *= GetPanRange();
    pan += GetPan();

    f32 surroundPan = 0.0f;
    if (mWaveSoundInfo.surroundPan <= 1) {
        surroundPan += (mWaveSoundInfo.surroundPan + 1) / 63.0f;
    } else {
        surroundPan += mWaveSoundInfo.surroundPan / 63.0f;
    }

    surroundPan += mWaveSoundInfo.surroundPan / 64.0f;
    surroundPan += GetSurroundPan();

    f32 lpfFreq = 0.0f;
    lpfFreq += GetLpfFreq();

    int biquadFilterType = GetBiquadFilterType();
    f32 biquadValue = GetBiquadFilterValue();

    int remoteFilter = 0;
    remoteFilter += GetRemoteFilter();

    f32 mainSend = 0.0f;
    mainSend += (mWaveSoundInfo.mainSend / 127.0f) - 1.0f;
    mainSend += GetMainSend();

    u8 infoSend[AUX_BUS_NUM];
    infoSend[AUX_A] = mWaveSoundInfo.fxSendA;
    infoSend[AUX_B] = mWaveSoundInfo.fxSendB;
    infoSend[AUX_C] = mWaveSoundInfo.fxSendC;

    f32 fxSend[AUX_BUS_NUM];
    for (int i = 0; i < AUX_BUS_NUM; i++) {
        fxSend[i] = 0.0f;
        fxSend[i] += infoSend[i] / 127.0f;
        fxSend[i] += GetFxSend(static_cast<AuxBus>(i));
    }

    /*
    f32 remoteSend[WPAD_MAX_CONTROLLERS];
    f32 remoteFxSend[WPAD_MAX_CONTROLLERS];
    for (int i = 0; i < WPAD_MAX_CONTROLLERS; i++) {
        remoteSend[i] = 0.0f;
        remoteSend[i] += GetRemoteSend(i);

        remoteFxSend[i] = 0.0f;
        remoteFxSend[i] += GetRemoteFxSend(i);
    }
    */

    mChannel->SetPanMode(GetPanMode());
    mChannel->SetPanCurve(GetPanCurve());
    mChannel->SetUserVolume(volume);
    mChannel->SetUserPitchRatio(pitchRatio);
    mChannel->SetUserPan(pan);
    mChannel->SetUserSurroundPan(surroundPan);
    mChannel->SetUserLpfFreq(lpfFreq);
    mChannel->SetBiquadFilter(biquadFilterType, biquadValue);
    mChannel->SetRemoteFilter(remoteFilter);
    mChannel->SetOutputLine(GetOutputLine());
    mChannel->SetMainOutVolume(GetMainOutVolume());
    mChannel->SetMainSend(mainSend);

    for (int i = 0; i < AUX_BUS_NUM; i++) {
        mChannel->SetFxSend(static_cast<AuxBus>(i), fxSend[i]);
    }

    for (int i = 0; i < WPAD_MAX_CONTROLLERS; i++) {
        mChannel->SetRemoteOutVolume(i, GetRemoteOutVolume(i));
        // mChannel->SetRemoteSend(i, remoteSend[i]);
        // mChannel->SetRemoteFxSend(i, remoteFxSend[i]);
    }

    for (int i = 0; i < GetVoiceOutCount(); i++) {
        mChannel->SetVoiceOutParam(i, GetVoiceOutParam(i));
    }

    mChannel->SetLfoParam(mLfoParam);
}

void WsdPlayer::ChannelCallbackFunc(Channel *pDropChannel, Channel::ChannelCallbackStatus status, u32 callbackArg) {
    SoundThread::AutoLock lock;

    WsdPlayer *p = reinterpret_cast<WsdPlayer *>(callbackArg);

    if (status == Channel::CALLBACK_STATUS_FINISH) {
        Channel::FreeChannel(pDropChannel);
    }

    p->mChannel = NULL;
}

} // namespace detail
} // namespace snd
} // namespace nw4r
