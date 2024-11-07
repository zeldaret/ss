#include "nw4r/snd/snd_SeqPlayer.h"

#include "nw4r/snd/snd_DisposeCallbackManager.h"
#include "nw4r/snd/snd_NoteOnCallback.h"
#include "nw4r/snd/snd_SeqTrack.h"
#include "nw4r/snd/snd_SeqTrackAllocator.h"

namespace nw4r {
namespace snd {
namespace detail {

volatile s16 SeqPlayer::mGlobalVariable[GLOBAL_VARIABLE_NUM];

void SeqPlayer::InitSeqPlayer() {
    for (int i = 0; i < GLOBAL_VARIABLE_NUM; i++) {
        mGlobalVariable[i] = DEFAULT_VARIABLE_VALUE;
    }
}

SeqPlayer::SeqPlayer() {
    mActiveFlag = false;
    mStartedFlag = false;
    mPauseFlag = false;
    mReleasePriorityFixFlag = false;

    mTempoRatio = 1.0f;
    mTickFraction = 0.0f;
    mSkipTickCounter = 0;
    mSkipTimeCounter = 0.0f;
    mPanRange = 1.0f;
    mTickCounter = 0;
    mVoiceOutCount = 0;

    mSeqUserprocCallback = NULL;
    mSeqUserprocCallbackArg = NULL;

    mParserParam.tempo = DEFAULT_TEMPO;
    mParserParam.timebase = DEFAULT_TIMEBASE;
    mParserParam.volume = 127;
    mParserParam.priority = DEFAULT_PRIORITY;
    mParserParam.callback = NULL;

    for (int i = 0; i < LOCAL_VARIABLE_NUM; i++) {
        mLocalVariable[i] = DEFAULT_VARIABLE_VALUE;
    }
    for (int i = 0; i < TRACK_NUM; i++) {
        mTracks[i] = NULL;
    }
}

SeqPlayer::~SeqPlayer() {
    SeqPlayer::Stop();
}

void SeqPlayer::InitParam(int voices, NoteOnCallback *pCallback) {
    BasicPlayer::InitParam();

    mStartedFlag = false;
    mPauseFlag = false;
    mTempoRatio = 1.0f;
    mSkipTickCounter = 0;
    mSkipTimeCounter = 0.0f;
    mPanRange = 1.0f;
    mTickCounter = 0;
    mVoiceOutCount = voices;

    mParserParam.tempo = DEFAULT_TEMPO;
    mParserParam.timebase = DEFAULT_TIMEBASE;
    mParserParam.volume = 127;
    mParserParam.priority = 64;
    mParserParam.callback = pCallback;

    mTickFraction = 0.0f;

    for (int i = 0; i < LOCAL_VARIABLE_NUM; i++) {
        mLocalVariable[i] = DEFAULT_VARIABLE_VALUE;
    }
    for (int i = 0; i < TRACK_NUM; i++) {
        mTracks[i] = NULL;
    }
}

SeqPlayer::SetupResult
SeqPlayer::Setup(SeqTrackAllocator *pAllocator, u32 allocTrackFlags, int voices, NoteOnCallback *pCallback) {
    SoundThread::AutoLock lock;

    SeqPlayer::Stop();
    InitParam(voices, pCallback);
    {
        ut::AutoInterruptLock lock;
        int tracks = 0;

        {
            u32 trackFlags = allocTrackFlags;

            for (; trackFlags != 0; trackFlags >>= 1) {
                if (trackFlags & 1) {
                    tracks++;
                }
            }
        }

        if (tracks > pAllocator->GetAllocatableTrackCount()) {
            return SETUP_ERR_CANNOT_ALLOCATE_TRACK;
        }

        {
            u32 trackFlags = allocTrackFlags;

            for (int i = 0; trackFlags != 0; trackFlags >>= 1, i++) {
                if (trackFlags & 1) {
                    SeqTrack *pTrack = pAllocator->AllocTrack(this);
                    SetPlayerTrack(i, pTrack);
                }
            }
        }
    }
    DisposeCallbackManager::GetInstance().RegisterDisposeCallback(this);

    mSeqTrackAllocator = pAllocator;
    mActiveFlag = true;

    return SETUP_SUCCESS;
}

void SeqPlayer::SetSeqData(const void *pBase, s32 offset) {
    SoundThread::AutoLock lock;

    SeqTrack *pTrack = GetPlayerTrack(0);

    if (pBase != NULL) {
        pTrack->SetSeqData(pBase, offset);
        pTrack->Open();
    }
}

bool SeqPlayer::Start() {
    SoundThread::AutoLock lock;

    SoundThread::GetInstance().RegisterPlayerCallback(this);
    mStartedFlag = true;

    return true;
}

void SeqPlayer::Stop() {
    SoundThread::AutoLock lock;

    FinishPlayer();
}

void SeqPlayer::Pause(bool flag) {
    SoundThread::AutoLock lock;

    mPauseFlag = flag;

    for (int i = 0; i < TRACK_NUM; i++) {
        SeqTrack *pTrack = GetPlayerTrack(i);

        if (pTrack != NULL) {
            pTrack->PauseAllChannel(flag);
        }
    }
}

void SeqPlayer::Skip(OffsetType type, int offset) {
    SoundThread::AutoLock lock;

    if (!mActiveFlag) {
        return;
    }

    switch (type) {
        case OFFSET_TYPE_TICK: {
            mSkipTickCounter += offset;
            break;
        }

        case OFFSET_TYPE_MILLISEC: {
            mSkipTimeCounter += offset;
            break;
        }
    }
}

void SeqPlayer::SetTempoRatio(f32 tempo) {
    mTempoRatio = tempo;
}

void SeqPlayer::SetChannelPriority(int priority) {
    mParserParam.priority = priority;
}

void SeqPlayer::SetReleasePriorityFix(bool flag) {
    mReleasePriorityFixFlag = flag;
}

void SeqPlayer::SetSeqUserprocCallback(SeqUserprocCallback cb, void *cbData) {
    mSeqUserprocCallback = cb;
    mSeqUserprocCallbackArg = cbData;
}

void SeqPlayer::CallSeqUserprocCallback(u16 id, SeqTrack *track) {
    if (mSeqUserprocCallback == NULL) {
        return;
    }

    SeqTrack::ParserTrackParam &trackP = track->GetParserTrackParam();

    SeqUserprocCallbackParam p;
    p.localVariable = mLocalVariable;
    p.globalVariable = mGlobalVariable;
    p.trackVariable = track->GetVariablePtr(0);
    p.cmpFlag = trackP.cmpFlag;
    (mSeqUserprocCallback)(id, &p, mSeqUserprocCallbackArg);
    trackP.cmpFlag = p.cmpFlag;
}

s16 SeqPlayer::GetLocalVariable(int i) const {
    return mLocalVariable[i];
}

void SeqPlayer::SetLocalVariable(int i, s16 value) {
    mLocalVariable[i] = value;
}

void SeqPlayer::SetGlobalVariable(int i, s16 value) {
    mGlobalVariable[i] = value;
}

void SeqPlayer::SetTrackMute(u32 trackFlags, SeqMute mute) {
    SetTrackParam<SeqMute>(trackFlags, &SeqTrack::SetMute, mute);
}
void SeqPlayer::SetTrackSilence(u32 trackFlags, bool b, int i) {
    SetTrackParam<bool, int>(trackFlags, &SeqTrack::SetSilence, b, i);
}

void SeqPlayer::SetTrackVolume(u32 trackFlags, f32 volume) {
    SetTrackParam<f32>(trackFlags, &SeqTrack::SetVolume, volume);
}

void SeqPlayer::SetTrackPitch(u32 trackFlags, f32 pitch) {
    SetTrackParam<f32>(trackFlags, &SeqTrack::SetPitch, pitch);
}

void SeqPlayer::InvalidateData(const void *pStart, const void *pEnd) {
    SoundThread::AutoLock lock;

    if (mActiveFlag) {
        for (int i = 0; i < TRACK_NUM; i++) {
            SeqTrack *pTrack = GetPlayerTrack(i);
            if (pTrack == NULL) {
                continue;
            }

            const u8 *pBase = pTrack->GetParserTrackParam().baseAddr;
            if (pStart <= pBase && pBase <= pEnd) {
                SeqPlayer::Stop();
                break;
            }
        }
    }
}

SeqTrack *SeqPlayer::GetPlayerTrack(int i) {
    if (i > TRACK_NUM - 1) {
        return NULL;
    }

    return mTracks[i];
}

void SeqPlayer::CloseTrack(int i) {
    SoundThread::AutoLock lock;

    SeqTrack *pTrack = GetPlayerTrack(i);
    if (pTrack == NULL) {
        return;
    }

    pTrack->Close();

    mSeqTrackAllocator->FreeTrack(mTracks[i]);
    mTracks[i] = NULL;
}

void SeqPlayer::SetPlayerTrack(int i, SeqTrack *pTrack) {
    SoundThread::AutoLock lock;

    if (i > TRACK_NUM - 1) {
        return;
    }

    mTracks[i] = pTrack;
    pTrack->SetPlayerTrackNo(i);
}

void SeqPlayer::FinishPlayer() {
    SoundThread::AutoLock lock;

    if (mStartedFlag) {
        SoundThread::GetInstance().UnregisterPlayerCallback(this);
        mStartedFlag = false;
    }

    if (mActiveFlag) {
        DisposeCallbackManager::GetInstance().UnregisterDisposeCallback(this);
        mActiveFlag = false;
    }

    for (int i = 0; i < TRACK_NUM; i++) {
        CloseTrack(i);
    }
}

void SeqPlayer::UpdateChannelParam() {
    SoundThread::AutoLock lock;

    for (int i = 0; i < TRACK_NUM; i++) {
        SeqTrack *pTrack = GetPlayerTrack(i);

        if (pTrack != NULL) {
            pTrack->UpdateChannelParam();
        }
    }
}

int SeqPlayer::ParseNextTick(bool doNoteOn) {
    SoundThread::AutoLock lock;

    bool active = false;

    for (int i = 0; i < TRACK_NUM; i++) {
        SeqTrack *pTrack = GetPlayerTrack(i);
        if (pTrack == NULL) {
            continue;
        }

        pTrack->UpdateChannelLength();

        if (pTrack->ParseNextTick(doNoteOn) < 0) {
            CloseTrack(i);
        }

        if (pTrack->IsOpened()) {
            active = true;
        }
    }

    if (!active) {
        return 1;
    }

    return 0;
}

volatile s16 *SeqPlayer::GetVariablePtr(int i) {
    if (i < LOCAL_VARIABLE_NUM) {
        return &mLocalVariable[i];
    }

    if (i < VARIABLE_NUM) {
        return &mGlobalVariable[i - LOCAL_VARIABLE_NUM];
    }

    return NULL;
}

void SeqPlayer::Update() {
    SoundThread::AutoLock lock;

    if (!mActiveFlag) {
        return;
    }

    if (!mStartedFlag) {
        return;
    }

    if (mSkipTickCounter != 0 || mSkipTimeCounter > 0.0f) {
        SkipTick();

    } else if (!mPauseFlag) {
        UpdateTick(3);
    }

    UpdateChannelParam();
}

void SeqPlayer::UpdateTick(int msec) {
    f32 tickPerMsec = GetBaseTempo();
    if (tickPerMsec == 0.0f) {
        return;
    }

    f32 restMsec = static_cast<f32>(msec);
    f32 nextMsec = mTickFraction / tickPerMsec;

    while (nextMsec < restMsec) {
        restMsec -= nextMsec;

        if (ParseNextTick(true) != 0) {
            FinishPlayer();
            return;
        }

        mTickCounter++;

        tickPerMsec = GetBaseTempo();
        if (tickPerMsec == 0.0f) {
            return;
        }

        nextMsec = 1.0f / tickPerMsec;
    }

    nextMsec -= restMsec;
    mTickFraction = nextMsec * tickPerMsec;
}

void SeqPlayer::SkipTick() {
    for (int i = 0; i < TRACK_NUM; i++) {
        SeqTrack *pTrack = GetPlayerTrack(i);

        if (pTrack != NULL) {
            pTrack->ReleaseAllChannel(127);
            pTrack->FreeAllChannel();
        }
    }

    int skipCount = 0;
    while (mSkipTickCounter != 0 || mSkipTimeCounter * GetBaseTempo() >= 1.0f) {
        if (skipCount >= MAX_SKIP_TICK_PER_FRAME) {
            return;
        }

        if (mSkipTickCounter != 0) {
            mSkipTickCounter--;
        } else {
            f32 tickPerMsec = GetBaseTempo();
            f32 msecPerTick = 1.0f / tickPerMsec;

            mSkipTimeCounter -= msecPerTick;
        }

        if (ParseNextTick(false) != 0) {
            FinishPlayer();
            return;
        }

        skipCount++;
        mTickCounter++;
    }

    mSkipTimeCounter = 0.0f;
}

void SeqPlayer::InitGlobalVariable() {
    for (int i = 0; i < GLOBAL_VARIABLE_NUM; i++) {
        mGlobalVariable[i] = DEFAULT_VARIABLE_VALUE;
    }
}

Channel *SeqPlayer::NoteOn(int bankNo, const NoteOnInfo &rInfo) {
    return mParserParam.callback->NoteOn(this, bankNo, rInfo);
}

} // namespace detail
} // namespace snd
} // namespace nw4r
