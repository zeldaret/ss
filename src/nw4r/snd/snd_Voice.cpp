#include "nw4r/snd/snd_Voice.h"
#include "nw4r/snd/snd_AxManager.h"
#include "nw4r/snd/snd_AxVoiceManager.h"
#include "nw4r/snd/snd_VoiceManager.h"
#include "nw4r/snd/snd_WaveFile.h"
#include "nw4r/snd/snd_Util.h"

namespace nw4r {
namespace snd {
namespace detail {

Voice::Voice()
    : mCallback(NULL),
      mActiveFlag(false),
      mStartFlag(false),
      mStartedFlag(false),
      mPauseFlag(false),
      mSyncFlag(0) {

    for (int i = 0; i < CHANNEL_MAX; i++) {
        for (int j = 0; j < VOICES_MAX; j++) {
            mAxVoice[i][j] = NULL;
        }
    }
}

Voice::~Voice() {
    for (int i = 0; i < CHANNEL_MAX; i++) {
        for (int j = 0; j < VOICES_MAX; j++) {
            AxVoice* pVoice = mAxVoice[i][j];

            if (pVoice != NULL) {
                AxVoiceManager::GetInstance().FreeAxVoice(pVoice);
            }
        }
    }
}

void Voice::InitParam(int channels, int voices, VoiceCallback pCallback,
                      void* pCallbackArg) {
    mChannelCount = channels;
    mVoiceOutCount = voices;
    mCallback = pCallback;
    mCallbackData = pCallbackArg;

    mSyncFlag = 0;
    mPauseFlag = false;
    mPausingFlag = false;
    mStartedFlag = false;

    mVoiceOutParamPitchDisableFlag = 0;

    mVolume = 1.0f;
    mVeInitVolume = 0.0f;
    mVeTargetVolume = 1.0f;
    mLpfFreq = 1.0f;
    mBiquadType = 0;
    mBiquadValue = 0.0f;
    mPan = 0.0f;
    mSurroundPan = 0.0f;
    mOutputLineFlag = OUTPUT_LINE_MAIN;
    mMainOutVolume = 1.0f;
    mMainSend = 1.0f;

    for (int i = 0; i < AUX_BUS_NUM; i++) {
        mFxSend[i] = 0.0f;
    }

    for (int i = 0; i < WPAD_MAX_CONTROLLERS; i++) {
        mRemoteOutVolume[i] = 1.0f;
    }

    mPitch = 1.0f;
    mRemoteFilter = 0;
    mPanMode = PAN_MODE_DUAL;
    mPanCurve = PAN_CURVE_SQRT;
}

void Voice::StopFinished() {
    if (mActiveFlag && mStartedFlag && IsPlayFinished()) {
        if (mCallback != NULL) {
            mCallback(this, CALLBACK_STATUS_FINISH_WAVE, mCallbackData);
        }

        mStartedFlag = false;
        mStartFlag = false;
    }
}

void Voice::Calc() {
    if (!mStartFlag) {
        return;
    }

    if (mSyncFlag & SYNC_AX_SRC) {
        CalcAxSrc(false);
        mSyncFlag &= ~SYNC_AX_SRC;
    }

    if (mSyncFlag & SYNC_AX_VE) {
        CalcAxVe();
        mSyncFlag &= ~SYNC_AX_VE;
    }

    if (mSyncFlag & SYNC_AX_MIX) {
        if (!CalcAxMix()) {
            mSyncFlag &= ~SYNC_AX_MIX;
        }
    }

    if (mSyncFlag & SYNC_AX_LPF) {
        CalcAxLpf();
        mSyncFlag &= ~SYNC_AX_LPF;
    }

    if (mSyncFlag & SYNC_AX_REMOTE) {
        CalcAxRemoteFilter();
        mSyncFlag &= ~SYNC_AX_REMOTE;
    }
}

void Voice::Update() {
    ut::AutoInterruptLock lock;

    if (!mActiveFlag) {
        return;
    }

    if ((mSyncFlag & SYNC_AX_SRC_INITIAL) && mStartFlag && !mStartedFlag) {
        CalcAxSrc(true);
        RunAllAxVoice();

        mStartedFlag = true;
        mSyncFlag &= ~SYNC_AX_SRC_INITIAL;
        mSyncFlag &= ~SYNC_AX_SRC;
    }

    if (mStartedFlag) {
        if ((mSyncFlag & SYNC_AX_VOICE) && mStartFlag) {
            if (mPauseFlag || AxManager::GetInstance().IsDiskError()) {
                StopAllAxVoice();
                mPausingFlag = true;
            } else {
                RunAllAxVoice();
                mPausingFlag = false;
            }

            mSyncFlag &= ~SYNC_AX_VOICE;
        }

        SyncAxVoice();
    }
}

bool Voice::Acquire(int channels, int voices, int priority,
                    VoiceCallback pCallback, void* pCallbackArg) {
    channels = ut::Clamp(channels, CHANNEL_MIN, CHANNEL_MAX);
    voices = ut::Clamp(voices, VOICES_MIN, VOICES_MAX);

    ut::AutoInterruptLock lock;

    u32 axPrio;
    if (priority == PRIORITY_MAX) {
        axPrio = AX_PRIORITY_MAX;
    } else {
        axPrio = (AX_PRIORITY_MAX / 2) + 1;
    }

    int required = channels * voices;
    AxVoice* voiceTable[CHANNEL_MAX * VOICES_MAX];

    for (int i = 0; required > i; i++) {
        AxVoice* pAxVoice = AxVoiceManager::GetInstance().AcquireAxVoice(
            axPrio, AxVoiceCallbackFunc, this);

        if (pAxVoice == NULL) {
            int rest = required - i;

            const VoiceList& rVoiceList =
                VoiceManager::GetInstance().GetVoiceList();

            for (VoiceList::ConstIterator it = rVoiceList.GetBeginIter();
                 it != rVoiceList.GetEndIter(); ++it) {

                if (priority < it->GetPriority()) {
                    break;
                }

                rest -= it->GetAxVoiceCount();
                if (rest <= 0) {
                    break;
                }
            }

            if (rest > 0) {
                for (int j = 0; j < i; j++) {
                    AxVoiceManager::GetInstance().FreeAxVoice(voiceTable[j]);
                }

                return false;
            }

            u32 allocPrio;
            if (axPrio == AX_PRIORITY_MAX) {
                allocPrio = AX_PRIORITY_MAX;
            } else {
                allocPrio = (AX_PRIORITY_MAX / 2) + 2;
            }

            pAxVoice = AxVoiceManager::GetInstance().AcquireAxVoice(
                allocPrio, AxVoiceCallbackFunc, this);
        }

        if (pAxVoice == NULL) {
            for (int j = 0; j < i; j++) {
                AxVoiceManager::GetInstance().FreeAxVoice(voiceTable[j]);
            }

            return false;
        }

        voiceTable[i] = pAxVoice;
    }

    int idx = 0;
    for (int i = 0; i < channels; i++) {
        for (int j = 0; j < voices; j++) {
            voiceTable[idx]->SetPriority(axPrio);
            mAxVoice[i][j] = voiceTable[idx];
            idx++;
        }
    }

    InitParam(channels, voices, pCallback, pCallbackArg);
    mActiveFlag = true;
    return true;
}

void Voice::Free() {
    ut::AutoInterruptLock lock;

    if (!mActiveFlag) {
        return;
    }

    for (int i = 0; i < mChannelCount; i++) {
        for (int j = 0; j < mVoiceOutCount; j++) {
            AxVoice* pAxVoice = mAxVoice[i][j];

            if (pAxVoice != NULL) {
                AxVoiceManager::GetInstance().FreeAxVoice(pAxVoice);
                mAxVoice[i][j] = NULL;
            }
        }
    }

    mChannelCount = 0;
    VoiceManager::GetInstance().FreeVoice(this);
    mActiveFlag = false;
}

void Voice::Setup(const WaveInfo& rData, u32 offset) {
    AxVoice::Format format = WaveFormatToAxFormat(rData.sampleFormat);
    int sampleRate = rData.sampleRate;

    for (int i = 0; i < mChannelCount; i++) {
        if (mAxVoice[i][0] == NULL) {
            continue;
        }

        void* pAddr = rData.channelParam[i].dataAddr;
        const ChannelParam& rParam = rData.channelParam[i];
        const AdpcmInfo& rInfo = rData.channelParam[i].adpcmInfo;

        AdpcmParam param;
        if (format == AxVoice::FORMAT_ADPCM) {
            param = rInfo.param;
            AxVoice::CalcOffsetAdpcmParam(&param.pred_scale, &param.yn1,
                                          &param.yn2, offset, pAddr, param);
        }

        for (int j = 0; j < mVoiceOutCount; j++) {
            AxVoice* pAxVoice = mAxVoice[i][j];
            if (pAxVoice == NULL) {
                continue;
            }

            pAxVoice->Setup(rData.channelParam[i].dataAddr, format, sampleRate);
            pAxVoice->SetAddr(rData.loopFlag, pAddr, offset, rData.loopStart,
                              rData.loopEnd);

            if (format == AxVoice::FORMAT_ADPCM) {
                pAxVoice->SetAdpcm(&param);
                pAxVoice->SetAdpcmLoop(&rInfo.loopParam);
            }

            pAxVoice->SetSrcType(AxVoice::SRC_4TAP_AUTO, mPitch);
            pAxVoice->SetVoiceType(AxVoice::VOICE_TYPE_NORMAL);
        }
    }

    for (int i = 0; i < mVoiceOutCount; i++) {
        mVoiceOutParam[i].volume = 1.0f;
        mVoiceOutParam[i].pitch = 1.0f;
        mVoiceOutParam[i].pan = 0.0f;
        mVoiceOutParam[i].surroundPan = 0.0f;
        mVoiceOutParam[i].fxSend = 0.0f;
        mVoiceOutParam[i].lpf = 0.0f;
        // mVoiceOutParam[i].priority = 0;
    }

    mPauseFlag = false;
    mPausingFlag = false;

    mStartFlag = false;
    mStartedFlag = false;

    mSyncFlag |= (SYNC_AX_LPF | SYNC_AX_MIX | SYNC_AX_VE);
}

void Voice::Start() {
    mStartFlag = true;
    mPauseFlag = false;
    mSyncFlag |= SYNC_AX_SRC_INITIAL;
}

void Voice::Stop() {
    if (mStartedFlag) {
        StopAllAxVoice();
        mStartedFlag = false;
    }

    mPausingFlag = false;
    mPauseFlag = false;
    mStartFlag = false;
}

void Voice::Pause(bool flag) {
    if (mPauseFlag == flag) {
        return;
    }

    mPauseFlag = flag;
    mSyncFlag |= SYNC_AX_VOICE;
}

AxVoice::Format Voice::GetFormat() const {
    if (IsActive()) {
        return mAxVoice[0][0]->GetFormat();
    }

    return AxVoice::FORMAT_PCM16;
}

void Voice::SetVolume(f32 volume) {
    if (volume < 0.0f) {
        volume = 0.0f;
    }

    if (volume != mVolume) {
        mVolume = volume;
        mSyncFlag |= SYNC_AX_VE;
    }
}

void Voice::SetVeVolume(f32 target, f32 init) {
    if (target < 0.0f) {
        target = 0.0f;
    }
    if (init < 0.0f) {
        init = 0.0f;
    }

    // @bug Unreachable code
    if (init < 0.0f) {
        if (target == mVeTargetVolume) {
            return;
        }

        mVeTargetVolume = target;
        mSyncFlag |= SYNC_AX_VE;
        return;
    }

    if (init == mVeInitVolume && target == mVeTargetVolume) {
        return;
    }

    mVeInitVolume = init;
    mVeTargetVolume = target;
    mSyncFlag |= SYNC_AX_VE;
}

void Voice::SetPitch(f32 pitch) {
    if (pitch == mPitch) {
        return;
    }

    mPitch = pitch;
    mSyncFlag |= SYNC_AX_SRC;
}

void Voice::SetPanMode(PanMode mode) {
    if (mode == mPanMode) {
        return;
    }

    mPanMode = mode;
    mSyncFlag |= SYNC_AX_MIX;
}

void Voice::SetPanCurve(PanCurve curve) {
    if (curve == mPanCurve) {
        return;
    }

    mPanCurve = curve;
    mSyncFlag |= SYNC_AX_MIX;
}

void Voice::SetPan(f32 pan) {
    if (pan == mPan) {
        return;
    }

    mPan = pan;
    mSyncFlag |= SYNC_AX_MIX;
}

void Voice::SetSurroundPan(f32 pan) {
    if (pan == mSurroundPan) {
        return;
    }

    mSurroundPan = pan;
    mSyncFlag |= SYNC_AX_MIX;
}

void Voice::SetLpfFreq(f32 freq) {
    if (freq == mLpfFreq) {
        return;
    }

    mLpfFreq = freq;
    mSyncFlag |= SYNC_AX_LPF;
}

void Voice::SetRemoteFilter(int filter) {
    filter = ut::Clamp(filter, 0, REMOTE_FILTER_MAX);

    if (filter == mRemoteFilter) {
        return;
    }

    mRemoteFilter = filter;
    mSyncFlag |= SYNC_AX_REMOTE;
}

void Voice::SetOutputLine(int flag) {
    if (flag == mOutputLineFlag) {
        return;
    }

    mOutputLineFlag = flag;
    mSyncFlag |= SYNC_AX_MIX;
}

void Voice::SetMainOutVolume(f32 volume) {
    if (volume < 0.0f) {
        volume = 0.0f;
    }

    if (volume == mMainOutVolume) {
        return;
    }

    mMainOutVolume = volume;
    mSyncFlag |= SYNC_AX_MIX;
}

void Voice::SetMainSend(f32 send) {
    send += 1.0f;
    if (send < 0.0f) {
        send = 0.0f;
    }

    if (send == mMainSend) {
        return;
    }

    mMainSend = send;
    mSyncFlag |= SYNC_AX_MIX;
}

void Voice::SetFxSend(AuxBus bus, f32 send) {
    if (send < 0.0f) {
        send = 0.0f;
    }

    if (send == mFxSend[bus]) {
        return;
    }

    mFxSend[bus] = send;
    mSyncFlag |= SYNC_AX_MIX;
}

void Voice::SetRemoteOutVolume(int remote, f32 volume) {
    if (volume < 0.0f) {
        volume = 0.0f;
    }

    if (volume == mRemoteOutVolume[remote]) {
        return;
    }

    mRemoteOutVolume[remote] = volume;
    mSyncFlag |= SYNC_AX_MIX;
}

void Voice::SetPriority(int priority) {
    mPriority = priority;
    VoiceManager::GetInstance().ChangeVoicePriority(this);

    if (mPriority != 1) {
        return;
    }

    for (int i = 0; i < mChannelCount; i++) {
        for (int j = 0; j < mVoiceOutCount; j++) {
            AxVoice* pAxVoice = mAxVoice[i][j];

            if (pAxVoice != NULL) {
                pAxVoice->SetPriority(AX_PRIORITY_MAX / 2);
            }
        }
    }
}

void Voice::UpdateVoicesPriority() {
    if (mPriority == 1) {
        return;
    }

    for (int i = 0; i < mChannelCount; i++) {
        for (int j = 0; j < mVoiceOutCount; j++) {
            AxVoice* pAxVoice = mAxVoice[i][j];

            if (pAxVoice != NULL) {
                pAxVoice->SetPriority((AX_PRIORITY_MAX / 2) + 1);
            }
        }
    }
}

void Voice::SetAdpcmLoop(int channel, const AdpcmLoopParam* pParam) {
    for (int i = 0; i < mVoiceOutCount; i++) {
        AxVoice* pAxVoice = mAxVoice[channel][i];

        if (pAxVoice != NULL) {
            pAxVoice->SetAdpcmLoop(pParam);
        }
    }
}

u32 Voice::GetCurrentPlayingSample() const {
    if (IsActive()) {
        return mAxVoice[0][0]->GetCurrentPlayingSample();
    }

    return 0;
}

void Voice::SetLoopStart(int channel, const void* pBase, u32 samples) {
    for (int i = 0; i < mVoiceOutCount; i++) {
        AxVoice* pAxVoice = mAxVoice[channel][i];

        if (pAxVoice != NULL) {
            pAxVoice->SetLoopStart(pBase, samples);
        }
    }
}

void Voice::SetLoopEnd(int channel, const void* pBase, u32 samples) {
    for (int i = 0; i < mVoiceOutCount; i++) {
        AxVoice* pAxVoice = mAxVoice[channel][i];

        if (pAxVoice != NULL) {
            pAxVoice->SetLoopEnd(pBase, samples);
        }
    }
}

void Voice::SetLoopFlag(bool loop) {
    for (int i = 0; i < mChannelCount; i++) {
        for (int j = 0; j < mVoiceOutCount; j++) {
            AxVoice* pAxVoice = mAxVoice[i][j];

            if (pAxVoice != NULL) {
                pAxVoice->SetLoopFlag(loop);
            }
        }
    }
}

void Voice::StopAtPoint(int channel, const void* pBase, u32 samples) {
    for (int i = 0; i < mVoiceOutCount; i++) {
        AxVoice* pAxVoice = mAxVoice[channel][i];

        if (pAxVoice != NULL) {
            pAxVoice->StopAtPoint(pBase, samples);
        }
    }
}

void Voice::SetVoiceType(AxVoice::VoiceType type) {
    for (int i = 0; i < mChannelCount; i++) {
        for (int j = 0; j < mVoiceOutCount; j++) {
            AxVoice* pAxVoice = mAxVoice[i][j];

            if (pAxVoice != NULL) {
                pAxVoice->SetVoiceType(type);
            }
        }
    }
}

void Voice::CalcAxSrc(bool initial) {
    for (int i = 0; i < mVoiceOutCount; i++) {
        f32 ratio = ut::Clamp(mVoiceOutParam[i].pitch, 0.0f, 1.0f);
        ratio = mPitch * ratio;

        for (int j = 0; j < mChannelCount; j++) {
            AxVoice* pAxVoice = mAxVoice[j][i];

            if (pAxVoice != NULL) {
                pAxVoice->SetSrc(ratio, initial);
            }
        }
    }
}

void Voice::CalcAxVe() {
    f32 baseVolume = 1.0f;
    baseVolume *= mVolume;
    baseVolume *= AxManager::GetInstance().GetOutputVolume();

    for (int i = 0; i < mVoiceOutCount; i++) {
        const VoiceOutParam& rParam = mVoiceOutParam[i];
        f32 volume = baseVolume * rParam.volume;
        f32 target = volume * mVeTargetVolume;
        f32 init = volume * mVeInitVolume;

        for (int j = 0; j < mChannelCount; j++) {
            AxVoice* pAxVoice = mAxVoice[j][i];

            if (pAxVoice != NULL) {
                pAxVoice->SetVe(target, init);
            }
        }
    }
}

bool Voice::CalcAxMix() {
    AxVoice::MixParam param;
    AxVoice::RemoteMixParam rmtParam;

    bool nextUpdate = false;

    for (int i = 0; i < mChannelCount; i++) {
        for (int j = 0; j < mVoiceOutCount; j++) {
            AxVoice* pAxVoice = mAxVoice[i][j];
            if (pAxVoice == NULL) {
                continue;
            }

            CalcMixParam(i, j, &param, &rmtParam);
            nextUpdate |= pAxVoice->SetMix(param);

            if (mOutputLineFlag == 0 || mOutputLineFlag == OUTPUT_LINE_MAIN) {
                pAxVoice->EnableRemote(false);
            } else {
                pAxVoice->EnableRemote(true);
                pAxVoice->SetRmtMix(rmtParam);
            }
        }
    }

    return nextUpdate;
}

void Voice::CalcAxLpf() {
    for (int i = 0; i < mVoiceOutCount; i++) {
        int freq = Util::CalcLpfFreq(mLpfFreq + mVoiceOutParam[i].lpf);

        for (int j = 0; j < mChannelCount; j++) {
            AxVoice* pAxVoice = mAxVoice[j][i];

            if (pAxVoice != NULL) {
                pAxVoice->SetLpf(freq);
            }
        }
    }
}

void Voice::CalcAxRemoteFilter() {
    for (int i = 0; i < mVoiceOutCount; i++) {
        for (int j = 0; j < mChannelCount; j++) {
            AxVoice* pAxVoice = mAxVoice[j][i];

            if (pAxVoice != NULL) {
                pAxVoice->SetRemoteFilter(mRemoteFilter);
            }
        }
    }
}

void Voice::SyncAxVoice() {
    for (int i = 0; i < mChannelCount; i++) {
        for (int j = 0; j < mVoiceOutCount; j++) {
            AxVoice* pAxVoice = mAxVoice[i][j];

            if (pAxVoice != NULL) {
                pAxVoice->Sync();
            }
        }
    }
}

void Voice::ResetDelta() {
    for (int i = 0; i < mVoiceOutCount; i++) {
        for (int j = 0; j < mChannelCount; j++) {
            AxVoice* pAxVoice = mAxVoice[j][i];

            if (pAxVoice != NULL) {
                pAxVoice->ResetDelta();
            }
        }
    }
}

void Voice::AxVoiceCallbackFunc(AxVoice* pDropVoice,
                                AxVoice::AxVoiceCallbackStatus status,
                                void* pCallbackArg) {
    Voice* p = static_cast<Voice*>(pCallbackArg);

    VoiceCallbackStatus voiceStatus;
    bool freeDropVoice = false;

    switch (status) {
    case AxVoice::CALLBACK_STATUS_CANCEL: {
        voiceStatus = CALLBACK_STATUS_CANCEL;
        break;
    }

    case AxVoice::CALLBACK_STATUS_DROP_DSP: {
        voiceStatus = CALLBACK_STATUS_DROP_DSP;
        freeDropVoice = true;
        break;
    }
    }

    for (int i = 0; i < p->mChannelCount; i++) {
        for (int j = 0; j < p->mVoiceOutCount; j++) {
            AxVoice* pAxVoice = p->mAxVoice[i][j];

            if (pAxVoice != NULL) {
                if (pAxVoice == pDropVoice) {
                    if (!freeDropVoice) {
                        AxVoiceManager::GetInstance().FreeAxVoice(pAxVoice);
                    }
                } else {
                    pAxVoice->Stop();
                    AxVoiceManager::GetInstance().FreeAxVoice(pAxVoice);
                }

                p->mAxVoice[i][j] = NULL;
            }
        }
    }

    p->mPauseFlag = false;
    p->mStartFlag = false;
    p->mChannelCount = 0;

    if (freeDropVoice) {
        p->Free();
    }

    if (p->mCallback != NULL) {
        p->mCallback(p, voiceStatus, p->mCallbackData);
    }
}

void Voice::TransformDpl2Pan(f32* pPan, f32* pSurroundPan, f32 pan,
                             f32 surroundPan) {
    surroundPan -= 1.0f;

    if (ut::Abs(pan) <= ut::Abs(surroundPan)) {
        if (surroundPan <= 0.0f) {
            *pPan = pan;
            *pSurroundPan = -0.12f + 0.88f * surroundPan;
        } else {
            *pPan = 0.5f * pan;
            *pSurroundPan = -0.12f + 1.12f * surroundPan;
        }
    } else if (pan >= 0.0f) {
        if (surroundPan <= 0.0f) {
            *pPan =
                (0.85f + (1.0f - 0.85f) * (-surroundPan / pan)) * ut::Abs(pan);
            *pSurroundPan = -0.12f + (2.0f * surroundPan + 0.88f * pan);
        } else {
            *pPan =
                (0.85f + (1.0f - 0.65f) * (-surroundPan / pan)) * ut::Abs(pan);
            *pSurroundPan = -0.12f + 1.12f * pan;
        }
    } else if (surroundPan <= 0.0f) {
        *pPan = ((1.0f - 0.85f) * (-surroundPan / pan) - 0.85f) * ut::Abs(pan);
        *pSurroundPan = -0.12f + (2.0f * surroundPan - 1.12f * pan);
    } else {
        *pPan = ((1.0f - 0.65f) * (-surroundPan / pan) - 0.85f) * ut::Abs(pan);
        *pSurroundPan = -0.12f + 1.12f * -pan;
    }

    *pSurroundPan += 1.0f;
}

void Voice::CalcMixParam(int channel, int voice, AxVoice::MixParam* pMix,
                         AxVoice::RemoteMixParam* pRmtMix) {
    f32 mainVolume = 0.0f;
    f32 mainSend = 0.0f;

    f32 fxSendA = 0.0f;
    f32 fxSendB = 0.0f;
    f32 fxSendC = 0.0f;

    if (mOutputLineFlag & OUTPUT_LINE_MAIN) {
        mainVolume = mMainOutVolume;
        mainSend = mMainSend;

        fxSendA = ut::Clamp(mFxSend[AUX_A] + mVoiceOutParam[voice].fxSend, 0.0f,
                            1.0f);
        fxSendB = mFxSend[AUX_B];
        fxSendC = mFxSend[AUX_C];
    }

    f32 main = mainVolume * mainSend;
    f32 fx_a = mainVolume * fxSendA;
    f32 fx_b = mainVolume * fxSendB;
    f32 fx_c = mainVolume * fxSendC;

    f32 remote[WPAD_MAX_CONTROLLERS];
    f32 remoteFx[WPAD_MAX_CONTROLLERS];
    for (int i = 0; i < WPAD_MAX_CONTROLLERS; i++) {
        f32 remoteVolume = 0.0f;
        f32 remoteSend = 0.0f;
        f32 remoteFxSend = 0.0f;

        if (mOutputLineFlag & (OUTPUT_LINE_REMOTE_N << i)) {
            remoteVolume = mRemoteOutVolume[i];
        }

        remote[i] = remoteVolume * remoteSend;
        remoteFx[i] = remoteVolume * remoteFxSend;
    }

    f32 left, right, surround, lrMixed;
    f32 front, rear;

    Util::PanInfo panInfo;

    switch (mPanCurve) {
    case PAN_CURVE_SQRT: {
        panInfo.curve = Util::PAN_CURVE_SQRT;
        break;
    }
    case PAN_CURVE_SQRT_0DB: {
        panInfo.curve = Util::PAN_CURVE_SQRT;
        panInfo.centerZero = true;
        break;
    }
    case PAN_CURVE_SQRT_0DB_CLAMP: {
        panInfo.curve = Util::PAN_CURVE_SQRT;
        panInfo.centerZero = true;
        panInfo.zeroClamp = true;
        break;
    }

    case PAN_CURVE_SINCOS: {
        panInfo.curve = Util::PAN_CURVE_SINCOS;
        break;
    }
    case PAN_CURVE_SINCOS_0DB: {
        panInfo.curve = Util::PAN_CURVE_SINCOS;
        panInfo.centerZero = true;
        break;
    }
    case PAN_CURVE_SINCOS_0DB_CLAMP: {
        panInfo.curve = Util::PAN_CURVE_SINCOS;
        panInfo.centerZero = true;
        panInfo.zeroClamp = true;
        break;
    }

    case PAN_CURVE_LINEAR: {
        panInfo.curve = Util::PAN_CURVE_LINEAR;
        break;
    }
    case PAN_CURVE_LINEAR_0DB: {
        panInfo.curve = Util::PAN_CURVE_LINEAR;
        panInfo.centerZero = true;
        break;
    }
    case PAN_CURVE_LINEAR_0DB_CLAMP: {
        panInfo.curve = Util::PAN_CURVE_LINEAR;
        panInfo.centerZero = true;
        panInfo.zeroClamp = true;
        break;
    }

    default: {
        panInfo.curve = Util::PAN_CURVE_SQRT;
    }
    }

    if (mChannelCount > 1 && mPanMode == PAN_MODE_BALANCE) {
        f32 pan = mPan + mVoiceOutParam[voice].pan;
        f32 surroundPan = mSurroundPan + mVoiceOutParam[voice].surroundPan;

        if (channel == 0) {
            left = Util::CalcPanRatio(pan, panInfo);
            right = 0.0f;
        } else if (channel == 1) {
            left = 0.0f;
            right = Util::CalcPanRatio(-pan, panInfo);
        }

        front = Util::CalcSurroundPanRatio(surroundPan, panInfo);
        rear = Util::CalcSurroundPanRatio(2.0f - surroundPan, panInfo);
    } else {
        f32 voicePan = 0.0f;
        f32 pan, surroundPan;

        if (mChannelCount == 2) {
            if (channel == 0) {
                voicePan = -1.0f;
            }
            if (channel == 1) {
                voicePan = 1.0f;
            }
        }

        switch (AxManager::GetInstance().GetOutputMode()) {
        case OUTPUT_MODE_DPL2: {
            TransformDpl2Pan(&pan, &surroundPan,
                             mPan + voicePan + mVoiceOutParam[voice].pan,
                             mSurroundPan + mVoiceOutParam[voice].surroundPan);
            break;
        }

        case OUTPUT_MODE_STEREO:
        case OUTPUT_MODE_SURROUND:
        case OUTPUT_MODE_MONO:
        default: {
            pan = mPan + voicePan + mVoiceOutParam[voice].pan;
            surroundPan = mSurroundPan + mVoiceOutParam[voice].surroundPan;
            break;
        }
        }

        left = Util::CalcPanRatio(pan, panInfo);
        right = Util::CalcPanRatio(-pan, panInfo);
        front = Util::CalcSurroundPanRatio(surroundPan, panInfo);
        rear = Util::CalcSurroundPanRatio(2.0f - surroundPan, panInfo);
    }

    surround = Util::CalcVolumeRatio(-3.0f);
    lrMixed = 0.5f * (left + right);

    f32 m_l, m_r, m_s;
    f32 a_l, a_r, a_s;
    f32 b_l, b_r, b_s;
    f32 c_l, c_r, c_s;

    f32& m_sl = m_s;
    f32& m_sr = c_l;

    f32& a_sl = a_s;
    f32& a_sr = c_r;

    f32& b_sl = b_s;
    f32& b_sr = c_s;

    switch (AxManager::GetInstance().GetOutputMode()) {
    case OUTPUT_MODE_STEREO: {
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
    }

    case OUTPUT_MODE_MONO: {
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
    }

    case OUTPUT_MODE_SURROUND: {
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
        break;
    }

    case OUTPUT_MODE_DPL2: {
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
        break;
    }

    default: {
        break;
    }
    }

    f32 rmt[WPAD_MAX_CONTROLLERS];
    f32 rmtFx[WPAD_MAX_CONTROLLERS];
    for (int i = 0; i < WPAD_MAX_CONTROLLERS; i++) {
        rmt[i] = lrMixed * remote[i];
        rmtFx[i] = lrMixed * remoteFx[i];
    }

    pMix->vL = CalcMixVolume(m_l);
    pMix->vR = CalcMixVolume(m_r);
    pMix->vS = CalcMixVolume(m_s);

    pMix->vAuxAL = CalcMixVolume(a_l);
    pMix->vAuxAR = CalcMixVolume(a_r);
    pMix->vAuxAS = CalcMixVolume(a_s);

    pMix->vAuxBL = CalcMixVolume(b_l);
    pMix->vAuxBR = CalcMixVolume(b_r);
    pMix->vAuxBS = CalcMixVolume(b_s);

    pMix->vAuxCL = CalcMixVolume(c_l);
    pMix->vAuxCR = CalcMixVolume(c_r);
    pMix->vAuxCS = CalcMixVolume(c_s);

    pRmtMix->vMain0 = CalcMixVolume(rmt[0]);
    pRmtMix->vAux0 = 0;

    pRmtMix->vMain1 = CalcMixVolume(rmt[1]);
    pRmtMix->vAux1 = 0;

    pRmtMix->vMain2 = CalcMixVolume(rmt[2]);
    pRmtMix->vAux2 = 0;

    pRmtMix->vMain3 = CalcMixVolume(rmt[3]);
    pRmtMix->vAux3 = 0;
}

void Voice::RunAllAxVoice() {
    for (int i = 0; i < mChannelCount; i++) {
        for (int j = 0; j < mVoiceOutCount; j++) {
            if (mAxVoice[i][j] != NULL) {
                mAxVoice[i][j]->Run();
            }
        }
    }
}

void Voice::StopAllAxVoice() {
    for (int i = 0; i < mChannelCount; i++) {
        for (int j = 0; j < mVoiceOutCount; j++) {
            if (mAxVoice[i][j] != NULL) {
                mAxVoice[i][j]->Stop();
            }
        }
    }
}

void Voice::InvalidateWaveData(const void* pStart, const void* pEnd) {
    bool dispose = false;

    for (int i = 0; i < mChannelCount; i++) {
        AxVoice* pAxVoice = mAxVoice[i][0];

        if (pAxVoice != NULL && pAxVoice->IsDataAddressCoverd(pStart, pEnd)) {
            dispose = true;
            break;
        }
    }

    if (dispose) {
        Stop();

        if (mCallback != NULL) {
            mCallback(this, CALLBACK_STATUS_CANCEL, mCallbackData);
        }
    }
}

} // namespace detail
} // namespace snd
} // namespace nw4r
