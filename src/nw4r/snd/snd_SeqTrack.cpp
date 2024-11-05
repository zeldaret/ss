#include "nw4r/snd/snd_SeqTrack.h"

#include "nw4r/snd/snd_NoteOnCallback.h"
#include "nw4r/snd/snd_SeqPlayer.h"
#include "nw4r/snd/snd_SoundThread.h"

namespace nw4r {
namespace snd {
namespace detail {

void SeqTrack::SetPlayerTrackNo(int no) {
    mPlayerTrackNo = no;
}

SeqTrack::SeqTrack() : mOpenFlag(false), mPlayer(NULL), mChannelList(NULL) {
    InitParam();
}

SeqTrack::~SeqTrack() {
    Close();
}

void SeqTrack::InitParam() {
    mExtVolume = 1.0f;
    mExtPitch = 1.0f;
    mExtPan = 0.0f;
    mExtSurroundPan = 0.0f;
    mPanRange = 1.0f;
    mParserTrackParam.baseAddr = NULL;
    mParserTrackParam.currentAddr = NULL;
    mParserTrackParam.cmpFlag = true;
    mParserTrackParam.noteWaitFlag = true;
    mParserTrackParam.tieFlag = false;
    mParserTrackParam.monophonicFlag = false;

    mParserTrackParam.callStackDepth = 0;
    mParserTrackParam.wait = 0;

    mParserTrackParam.muteFlag = false;
    mParserTrackParam.silenceFlag = false;
    mParserTrackParam.noteFinishWait = false;
    mParserTrackParam.portaFlag = false;
    mParserTrackParam.damperFlag = false;

    mParserTrackParam.bankNo = 0;
    mParserTrackParam.prgNo = 0;
    mParserTrackParam.lfoParam.Init();
    mParserTrackParam.lfoTarget = Channel::LFO_TARGET_PITCH;

    mParserTrackParam.sweepPitch = 1.0f;
    mParserTrackParam.volume.InitValue(127);

    mParserTrackParam.pan.InitValue(0);
    mParserTrackParam.surroundPan.InitValue(0);
    mParserTrackParam.volume2 = 127;
    mParserTrackParam.velocityRange = 127;
    mParserTrackParam.pitchBend.InitValue(0);
    mParserTrackParam.bendRange = DEFAULT_BENDRANGE;
    mParserTrackParam.initPan = 0;
    mParserTrackParam.transpose = 0;
    mParserTrackParam.priority = DEFAULT_PRIORITY;
    mParserTrackParam.portaKey = DEFAULT_PORTA_KEY;
    mParserTrackParam.portaTime = 0;

    mParserTrackParam.attack = 0xFF;
    mParserTrackParam.decay = 0xFF;
    mParserTrackParam.sustain = 0xFF;
    mParserTrackParam.release = 0xFF;
    mParserTrackParam.envHold = 0xFF;

    mParserTrackParam.mainSend = 127;
    for (int i = 0; i < AUX_BUS_NUM; i++) {
        mParserTrackParam.fxSend[i] = 0;
    }

    mParserTrackParam.lpfFreq = 1.0f;
    mParserTrackParam.biquadType = 0;
    mParserTrackParam.biquadValue = 1.0f;

    for (int i = 0; i < VARIABLE_NUM; i++) {
        mTrackVariable[i] = DEFAULT_VARIABLE_VALUE;
    }
}

void SeqTrack::SetSeqData(const void *pBase, s32 offset) {
    mParserTrackParam.baseAddr = static_cast<const u8 *>(pBase);
    mParserTrackParam.currentAddr = mParserTrackParam.baseAddr + offset;
}

void SeqTrack::Open() {
    mParserTrackParam.noteFinishWait = false;
    mParserTrackParam.callStackDepth = 0;
    mParserTrackParam.wait = 0;
    mOpenFlag = true;
}

void SeqTrack::Close() {
    SoundThread::AutoLock lock;

    ReleaseAllChannel(-1);
    FreeAllChannel();

    mOpenFlag = false;
}

void SeqTrack::UpdateChannelLength() {
    SoundThread::AutoLock lock;

    if (!mOpenFlag) {
        return;
    }

    for (Channel *pIt = mChannelList; pIt != NULL; pIt = pIt->GetNextTrackChannel()) {
        if (pIt->GetLength() > 0) {
            pIt->SetLength(pIt->GetLength() - 1);
        }

        UpdateChannelRelease(pIt);

        if (!pIt->IsAutoUpdateSweep()) {
            pIt->UpdateSweep(1);
        }
    }
}

void SeqTrack::UpdateChannelRelease(Channel *pChannel) {
    if (pChannel->GetLength() == 0 && !pChannel->IsRelease() && !mParserTrackParam.damperFlag) {
        pChannel->Release();
    }
}

int SeqTrack::ParseNextTick(bool doNoteOn) {
    SoundThread::AutoLock lock;

    if (!mOpenFlag) {
        return 0;
    }

    mParserTrackParam.volume.Update();
    mParserTrackParam.pan.Update();
    mParserTrackParam.surroundPan.Update();
    mParserTrackParam.pitchBend.Update();

    if (mParserTrackParam.noteFinishWait) {
        if (mChannelList != NULL) {
            return 1;
        }

        mParserTrackParam.noteFinishWait = false;
    }

    if (mParserTrackParam.wait > 0 && --mParserTrackParam.wait > 0) {
        return 1;
    }

    if (mParserTrackParam.currentAddr != NULL) {
        while (mParserTrackParam.wait == 0 && !mParserTrackParam.noteFinishWait) {
            if (Parse(doNoteOn) == PARSE_RESULT_FINISH) {
                return -1;
            }
        }
    }

    return 1;
}

void SeqTrack::StopAllChannel() {
    SoundThread::AutoLock lock;

    for (Channel *pIt = mChannelList; pIt != NULL; pIt = pIt->GetNextTrackChannel()) {
        Channel::FreeChannel(pIt);
        pIt->Stop();
    }

    mChannelList = NULL;
}

void SeqTrack::ReleaseAllChannel(int release) {
    SoundThread::AutoLock lock;

    UpdateChannelParam();

    for (Channel *pIt = mChannelList; pIt != NULL; pIt = pIt->GetNextTrackChannel()) {
        if (pIt->IsActive()) {
            if (release >= 0) {
                pIt->SetRelease(static_cast<u8>(release));
            }

            pIt->Release();
        }
    }
}

void SeqTrack::PauseAllChannel(bool flag) {
    SoundThread::AutoLock lock;

    for (Channel *pIt = mChannelList; pIt != NULL; pIt = pIt->GetNextTrackChannel()) {
        if (pIt->IsActive() && flag != pIt->IsPause()) {
            pIt->Pause(flag);
        }
    }
}

void SeqTrack::AddChannel(Channel *pChannel) {
    SoundThread::AutoLock lock;

    pChannel->SetNextTrackChannel(mChannelList);
    mChannelList = pChannel;
}

void SeqTrack::UpdateChannelParam() {
    // TODO 
    SoundThread::AutoLock lock;

    if (!mOpenFlag) {
        return;
    }

    if (mChannelList == NULL) {
        return;
    }

    f32 volume = 1.0f;
    f32 parserVolume = mParserTrackParam.volume.GetValue() / 127.0f;
    f32 parserVolume2 = mParserTrackParam.volume2 / 127.0f;
    f32 parserMainVolume = mPlayer->GetParserPlayerParam().volume / 127.0f;

    volume *= (parserVolume * parserVolume);
    volume *= (parserVolume2 * parserVolume2);
    volume *= (parserMainVolume * parserMainVolume);
    volume *= mExtVolume;
    volume *= mPlayer->GetVolume();

    f32 pitch = (mParserTrackParam.pitchBend.GetValue() / 128.0f) * mParserTrackParam.bendRange;

    f32 pitchRatio = 1.0f;
    pitchRatio *= mPlayer->GetPitch();
    pitchRatio *= mExtPitch;

    f32 pan = 0.0f;
    pan += ut::Clamp(mParserTrackParam.pan.GetValue() / 63.0f, -1.0f, 1.0f);
    pan *= mPanRange;
    pan *= mPlayer->GetPanRange();
    pan += mExtPan;
    pan += mPlayer->GetPan();

    f32 surroundPan = 0.0f;
    surroundPan += ut::Clamp(mParserTrackParam.surroundPan.GetValue() / 63.0f, 0.0f, 2.0f);
    surroundPan += mExtSurroundPan;
    surroundPan += mPlayer->GetSurroundPan();

    f32 lpfFreq = 0.0f;
    lpfFreq += (mParserTrackParam.lpfFreq - 64) / 64.0f;
    lpfFreq += mExtLpfFreq;
    lpfFreq += mPlayer->GetLpfFreq();

    f32 mBiquadValue = mPlayer->GetBiquadFilterValue();

    int remoteFilter = 0;
    remoteFilter += mPlayer->GetRemoteFilter();

    f32 mainSend = 0.0f;
    mainSend += (mParserTrackParam.mainSend / 127.0f) - 1.0f;
    mainSend += mExtMainSend;
    mainSend += mPlayer->GetMainSend();

    f32 fxSend[AUX_BUS_NUM];
    for (int i = 0; i < AUX_BUS_NUM; i++) {
        fxSend[i] = 0.0f;
        fxSend[i] += mParserTrackParam.fxSend[i] / 127.0f;
        fxSend[i] += mExtFxSend[i];
        fxSend[i] += mPlayer->GetFxSend(static_cast<AuxBus>(i));
    }

    for (Channel *pIt = mChannelList; pIt != NULL; pIt = pIt->GetNextTrackChannel()) {
        pIt->SetUserVolume(volume);
        pIt->SetUserPitch(pitch);
        pIt->SetUserPitchRatio(pitchRatio);
        pIt->SetUserPan(pan);
        pIt->SetUserSurroundPan(surroundPan);
        pIt->SetUserLpfFreq(lpfFreq);
        pIt->SetBiquadFilter(remoteFilter, mBiquadValue);
        pIt->SetRemoteFilter(remoteFilter);
        pIt->SetOutputLine(mPlayer->GetOutputLine());
        pIt->SetMainOutVolume(mPlayer->GetMainOutVolume());
        pIt->SetMainSend(mainSend);

        for (int i = 0; i < AUX_BUS_NUM; i++) {
            pIt->SetFxSend(static_cast<AuxBus>(i), fxSend[i]);
        }

        for (int i = 0; i < WPAD_MAX_CONTROLLERS; i++) {
            pIt->SetRemoteOutVolume(i, mPlayer->GetRemoteOutVolume(i));
        }

        for (int i = 0; i < mPlayer->GetVoiceOutCount(); i++) {
            pIt->SetVoiceOutParam(i, mPlayer->GetVoiceOutParam(i));
        }

        pIt->SetLfoParam(mParserTrackParam.lfoParam);
        pIt->SetLfoTarget(static_cast<Channel::LfoTarget>(mParserTrackParam.lfoTarget));
    }
}

void SeqTrack::FreeAllChannel() {
    SoundThread::AutoLock lock;

    for (Channel *pIt = mChannelList; pIt != NULL; pIt = pIt->GetNextTrackChannel()) {
        Channel::FreeChannel(pIt);
    }

    mChannelList = NULL;
}

void SeqTrack::ChannelCallbackFunc(Channel *pDropChannel, Channel::ChannelCallbackStatus status, u32 callbackArg) {
    SoundThread::AutoLock lock;
    SeqTrack *p = reinterpret_cast<SeqTrack *>(callbackArg);

    switch (status) {
        case Channel::CALLBACK_STATUS_STOPPED:
        case Channel::CALLBACK_STATUS_FINISH:  Channel::FreeChannel(pDropChannel); break;
    }

    if (p->mPlayer != NULL) {
        p->mPlayer->ChannelCallback(pDropChannel);
    }

    if (p->mChannelList == pDropChannel) {
        p->mChannelList = pDropChannel->GetNextTrackChannel();
        return;
    }

    for (Channel *pIt = p->mChannelList; pIt->GetNextTrackChannel() != NULL; pIt = pIt->GetNextTrackChannel()) {
        if (pIt->GetNextTrackChannel() == pDropChannel) {
            pIt->SetNextTrackChannel(pDropChannel->GetNextTrackChannel());
            return;
        }
    }
}

void SeqTrack::SetMute(SeqMute mute) {
    SoundThread::AutoLock lock;

    switch (mute) {
        case MUTE_OFF: {
            mParserTrackParam.muteFlag = false;
            break;
        }

        case MUTE_STOP: {
            StopAllChannel();
            mParserTrackParam.muteFlag = true;
            break;
        }

        case MUTE_RELEASE: {
            ReleaseAllChannel(-1);
            FreeAllChannel();
            mParserTrackParam.muteFlag = true;
            break;
        }

        case MUTE_NO_STOP: {
            mParserTrackParam.muteFlag = true;
            break;
        }
    }
}

void SeqTrack::SetVolume(f32 volume) {
    mExtVolume = volume;
}

void SeqTrack::SetPitch(f32 pitch) {
    mExtPitch = pitch;
}

volatile s16 *SeqTrack::GetVariablePtr(int i) {
    if (i < VARIABLE_NUM) {
        return &mTrackVariable[i];
    }

    return NULL;
}

Channel *SeqTrack::NoteOn(int key, int velocity, s32 length, bool tie) {
    SoundThread::AutoLock lock;

    SeqPlayer *pPlayer = GetSeqPlayer();
    Channel *pChannel = NULL;

    if (tie) {
        pChannel = GetLastChannel();
        if (pChannel != NULL) {
            pChannel->SetKey(static_cast<u8>(key));

            f32 initVolume = velocity / 127.0f;
            pChannel->SetInitVolume(initVolume * initVolume);
        }
    }

    if (pChannel == NULL) {
        NoteOnInfo info = {
            mParserTrackParam.prgNo,   // prgNo
            key,                       // key
            velocity,                  // velocity
            tie ? -1 : length,         // length
            mParserTrackParam.initPan, // initPan

            pPlayer->GetParserPlayerParam().priority + // priority
                GetParserTrackParam().priority,

            mPlayer->GetVoiceOutCount(), // voiceOutCount
            ChannelCallbackFunc,         // channelCallback
            reinterpret_cast<u32>(this)  // channelCallbackData
        };

        pChannel = mPlayer->NoteOn(mParserTrackParam.bankNo, info);
        if (pChannel == NULL) {
            return NULL;
        }

        AddChannel(pChannel);
    }

    if (mParserTrackParam.attack != 0xFF) {
        pChannel->SetAttack(mParserTrackParam.attack);
    }
    if (mParserTrackParam.decay != 0xFF) {
        pChannel->SetDecay(mParserTrackParam.decay);
    }
    if (mParserTrackParam.sustain != 0xFF) {
        pChannel->SetSustain(mParserTrackParam.sustain);
    }
    if (mParserTrackParam.release != 0xFF) {
        pChannel->SetRelease(mParserTrackParam.release);
    }

    f32 sweepPitch = mParserTrackParam.sweepPitch;
    if (mParserTrackParam.portaFlag) {
        sweepPitch += mParserTrackParam.portaKey - key;
    }

    if (mParserTrackParam.portaTime == 0) {
        pChannel->SetSweepParam(sweepPitch, length, false);
    } else {
        int time = mParserTrackParam.portaTime;
        time *= time;
        time *= sweepPitch >= 0.0f ? sweepPitch : -sweepPitch;
        time = static_cast<u32>(time >> 5);
        time = static_cast<u32>(time << 2) + time;
        pChannel->SetSweepParam(sweepPitch, time, true);
    }

    mParserTrackParam.portaKey = key;

    pChannel->SetSilence(mParserTrackParam.silenceFlag != 0, 0);
    pChannel->SetReleasePriorityFix(mPlayer->IsReleasePriorityFix());
    pChannel->SetPanMode(mPlayer->GetPanMode());
    pChannel->SetPanCurve(mPlayer->GetPanCurve());

    return pChannel;
}

} // namespace detail
} // namespace snd
} // namespace nw4r
