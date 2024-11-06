#include "nw4r/snd/snd_SoundPlayer.h"

#include "nw4r/snd/snd_ExternalSoundPlayer.h"
#include "nw4r/snd/snd_SeqSound.h"
#include "nw4r/snd/snd_SoundInstanceManager.h"
#include "nw4r/snd/snd_SoundThread.h"
#include "nw4r/snd/snd_StrmSound.h"
#include "nw4r/snd/snd_WaveSound.h"
#include "string.h"

namespace nw4r {
namespace snd {

SoundPlayer::SoundPlayer()
    : mPlayableCount(1), mPlayableLimit(INT_MAX), mVolume(1.0f), mLpfFreq(0.0f), mOutputLineFlag(OUTPUT_LINE_MAIN),
      mMainOutVolume(1.0f), mMainSend(0.0f), mBiquadType(0), mBiquadValue(0.0f) {
    for (int i = 0; i < WPAD_MAX_CONTROLLERS; i++) {
        mRemoteOutVolume[i] = 1.0f;
    }

    for (int i = 0; i < AUX_BUS_NUM; i++) {
        mFxSend[i] = 0.0f;
    }
}

SoundPlayer::~SoundPlayer() {
    StopAllSound(0);
}

void SoundPlayer::Update() {
    detail::SoundThread::AutoLock lock;

    NW4R_UT_LIST_SAFE_FOREACH(mSoundList, it->Update();)

    detail_SortPriorityList();
}

void SoundPlayer::StopAllSound(int frames) {
    detail::SoundThread::AutoLock lock;

    NW4R_UT_LIST_SAFE_FOREACH(mSoundList, it->Stop(frames);)
}

void SoundPlayer::PauseAllSound(bool flag, int frames) {
    detail::SoundThread::AutoLock lock;

    NW4R_UT_LIST_SAFE_FOREACH(mSoundList, it->Pause(flag, frames);)
}

void SoundPlayer::SetVolume(f32 volume) {
    if (volume < 0.0f) {
        volume = 0.0f;
    }
    mVolume = volume;
}

void SoundPlayer::SetLpfFreq(f32 freq) {
    mLpfFreq = freq;
}

f32 SoundPlayer::GetRemoteOutVolume(int remote) const {
    return mRemoteOutVolume[remote];
}

void SoundPlayer::SetFxSend(AuxBus bus, f32 value) {
    mFxSend[bus] = value;
}

int SoundPlayer::detail_GetOutputLine() const {
    return mOutputLineFlag;
}

f32 SoundPlayer::detail_GetRemoteOutVolume(int i) const {
    return mRemoteOutVolume[i];
}

void SoundPlayer::detail_InsertSoundList(detail::BasicSound *pSound) {
    mSoundList.PushBack(pSound);
    pSound->AttachSoundPlayer(this);
}

void SoundPlayer::detail_RemoveSoundList(detail::BasicSound *pSound) {
    detail::SoundThread::AutoLock lock;

    mSoundList.Erase(pSound);
    pSound->DetachSoundPlayer(this);
}

void SoundPlayer::detail_InsertPriorityList(detail::BasicSound *pSound) {
    detail::SoundThread::AutoLock lock;

    detail::BasicSoundPlayerPrioList::Iterator it = mPriorityList.GetBeginIter();

    for (; it != mPriorityList.GetEndIter(); ++it) {
        if (pSound->CalcCurrentPlayerPriority() < it->CalcCurrentPlayerPriority()) {
            break;
        }
    }

    mPriorityList.Insert(it, pSound);
}

void SoundPlayer::detail_RemovePriorityList(detail::BasicSound *pSound) {
    detail::SoundThread::AutoLock lock;
    mPriorityList.Erase(pSound);
}

void SoundPlayer::detail_SortPriorityList(detail::BasicSound *pSound) {
    detail_RemovePriorityList(pSound);
    detail_InsertPriorityList(pSound);
}

void SoundPlayer::detail_SortPriorityList() {
    detail::SoundThread::AutoLock lock;

    if (mPriorityList.GetSize() < 2) {
        return;
    }

    static detail::BasicSoundPlayerPrioList listsByPrio[detail::BasicSound::PRIORITY_MAX + 1];

    while (!mPriorityList.IsEmpty()) {
        detail::BasicSound &rSound = mPriorityList.GetFront();
        mPriorityList.PopFront();
        listsByPrio[rSound.CalcCurrentPlayerPriority()].PushBack(&rSound);
    }

    for (int i = 0; i < detail::BasicSound::PRIORITY_MAX + 1; i++) {
        while (!listsByPrio[i].IsEmpty()) {
            detail::BasicSound &rSound = listsByPrio[i].GetFront();
            listsByPrio[i].PopFront();
            mPriorityList.PushBack(&rSound);
        }
    }
}
/*
detail::SeqSound *SoundPlayer::detail_AllocSeqSound(
    int priority, int startPriority, detail::BasicSound::AmbientInfo *pArgInfo, detail::ExternalSoundPlayer *pExtPlayer,
    u32 id, detail::SoundInstanceManager<detail::SeqSound> *pManager
) {
    detail::SoundThread::AutoLock lock;

    if (pManager == NULL) {
        return NULL;
    }

    int priorityReduction = CalcPriorityReduction(pArgInfo, id);

    startPriority = ut::Clamp(startPriority + priorityReduction, 0, detail::BasicSound::PRIORITY_MAX);

    if (!CheckPlayableSoundCount(startPriority, pExtPlayer)) {
        return NULL;
    }

    detail::SeqSound *pSound = pManager->Alloc(startPriority);
    if (pSound == NULL) {
        return NULL;
    }

    detail_AllocPlayerHeap(pSound);

    if (pArgInfo != NULL) {
        InitAmbientArg(pSound, pArgInfo);
    }

    pSound->SetPriority(priority);
    pSound->GetAmbientParam().priority = priorityReduction;

    detail_InsertSoundList(pSound);

    if (pExtPlayer != NULL) {
        pExtPlayer->InsertSoundList(pSound);
    }

    detail_InsertPriorityList(pSound);

    return pSound;
}

detail::StrmSound *SoundPlayer::detail_AllocStrmSound(
    int priority, int startPriority, detail::BasicSound::AmbientInfo *pArgInfo, detail::ExternalSoundPlayer *pExtPlayer,
    u32 id, detail::SoundInstanceManager<detail::StrmSound> *pManager
) {
    detail::SoundThread::AutoLock lock;

    if (pManager == NULL) {
        return NULL;
    }

    int priorityReduction = CalcPriorityReduction(pArgInfo, id);

    startPriority = ut::Clamp(startPriority + priorityReduction, 0, detail::BasicSound::PRIORITY_MAX);

    if (!CheckPlayableSoundCount(startPriority, pExtPlayer)) {
        return NULL;
    }

    detail::StrmSound *pSound = pManager->Alloc(startPriority);
    if (pSound == NULL) {
        return NULL;
    }

    detail_AllocPlayerHeap(pSound);

    if (pArgInfo != NULL) {
        InitAmbientArg(pSound, pArgInfo);
    }

    pSound->SetPriority(priority);
    pSound->GetAmbientParam().priority = priorityReduction;

    detail_InsertSoundList(pSound);

    if (pExtPlayer != NULL) {
        pExtPlayer->InsertSoundList(pSound);
    }

    detail_InsertPriorityList(pSound);

    return pSound;
}

detail::WaveSound *SoundPlayer::detail_AllocWaveSound(
    int priority, int startPriority, detail::BasicSound::AmbientInfo *pArgInfo, detail::ExternalSoundPlayer *pExtPlayer,
    u32 id, detail::SoundInstanceManager<detail::WaveSound> *pManager
) {
    detail::SoundThread::AutoLock lock;

    if (pManager == NULL) {
        return NULL;
    }

    int priorityReduction = CalcPriorityReduction(pArgInfo, id);

    startPriority = ut::Clamp(startPriority + priorityReduction, 0, detail::BasicSound::PRIORITY_MAX);

    if (!CheckPlayableSoundCount(startPriority, pExtPlayer)) {
        return NULL;
    }

    detail::WaveSound *pSound = pManager->Alloc(startPriority);
    if (pSound == NULL) {
        return NULL;
    }

    detail_AllocPlayerHeap(pSound);

    if (pArgInfo != NULL) {
        InitAmbientArg(pSound, pArgInfo);
    }

    pSound->SetPriority(priority);
    pSound->GetAmbientParam().priority = priorityReduction;

    detail_InsertSoundList(pSound);

    if (pExtPlayer != NULL) {
        pExtPlayer->InsertSoundList(pSound);
    }

    detail_InsertPriorityList(pSound);

    return pSound;
}
*/
int SoundPlayer::CalcPriorityReduction(detail::BasicSound::AmbientInfo *pArgInfo, u32 id) {
    int priority = 0;

    if (pArgInfo != NULL) {
        SoundParam param;

        pArgInfo->paramUpdateCallback->detail_Update(
            &param, id, NULL, pArgInfo->arg, detail::BasicSound::AmbientParamUpdateCallback::PARAM_UPDATE_PRIORITY
        );

        priority = param.priority;
    }

    return priority;
}

void SoundPlayer::InitAmbientArg(detail::BasicSound *pSound, detail::BasicSound::AmbientInfo *pArgInfo) {
    if (pArgInfo == NULL) {
        return;
    }

    void *pExtArg = pArgInfo->argAllocaterCallback->detail_AllocAmbientArg(pArgInfo->argSize);

    if (pExtArg == NULL) {
        return;
    }

    memcpy(pExtArg, pArgInfo->arg, pArgInfo->argSize);

    pSound->SetAmbientParamCallback(
        pArgInfo->paramUpdateCallback, pArgInfo->argUpdateCallback, pArgInfo->argAllocaterCallback, pExtArg
    );
}

bool SoundPlayer::detail_AppendSound(detail::BasicSound *pSound) {
    detail::SoundThread::AutoLock lock;
    int allocPriority = pSound->CalcCurrentPlayerPriority();
    if (GetPlayableSoundCount() == 0) {
        return false;
    }

    while (GetPlayingSoundCount() >= GetPlayableSoundCount()) {
        detail::BasicSound *pDropSound = detail_GetLowestPrioritySound();
        if (pDropSound == NULL) {
            return false;
        }
        if (allocPriority < pDropSound->CalcCurrentPlayerPriority()) {
            return false;
        }
        pDropSound->Shutdown();
    }

    mSoundList.PushBack(pSound);
    detail_InsertPriorityList(pSound);
    pSound->AttachSoundPlayer(this);
    return true;
}

void SoundPlayer::detail_RemoveSound(detail::BasicSound *pSound) {
    detail_RemovePriorityList(pSound);
    detail_RemoveSoundList(pSound);
}

void SoundPlayer::SetPlayableSoundCount(int count) {
    detail::SoundThread::AutoLock lock;

    mPlayableCount = nw4r::ut::Clamp(count, 0, mPlayableLimit);

    while (GetPlayingSoundCount() > GetPlayableSoundCount()) {
        detail::BasicSound *pDropSound = detail_GetLowestPrioritySound();
        pDropSound->Shutdown();
    }
}

void SoundPlayer::detail_SetPlayableSoundLimit(int limit) {
    mPlayableLimit = limit;
}

bool SoundPlayer::detail_CanPlaySound(int startPriority) {
    detail::SoundThread::AutoLock lock;

    if (GetPlayableSoundCount() == 0) {
        return false;
    }

    if (GetPlayingSoundCount() >= GetPlayableSoundCount()) {
        detail::BasicSound *dropSound = detail_GetLowestPrioritySound();
        if (dropSound == NULL) {
            return false;
        }
        if (startPriority < dropSound->CalcCurrentPlayerPriority()) {
            return false;
        }
    }
    return true;
}

bool SoundPlayer::CheckPlayableSoundCount(int startPriority, detail::ExternalSoundPlayer *pExtPlayer) {
    detail::SoundThread::AutoLock lock;

    if (GetPlayableSoundCount() == 0) {
        return false;
    }

    while (GetPlayingSoundCount() >= GetPlayableSoundCount()) {
        detail::BasicSound *pDropSound = detail_GetLowestPrioritySound();

        if (pDropSound == NULL) {
            return false;
        }

        if (startPriority < pDropSound->CalcCurrentPlayerPriority()) {
            return false;
        }

        pDropSound->Shutdown();
    }

    if (pExtPlayer != NULL) {
        if (pExtPlayer->GetPlayableSoundCount() == 0) {
            return false;
        }

        while (pExtPlayer->GetPlayingSoundCount() >= pExtPlayer->GetPlayableSoundCount()) {
            detail::BasicSound *pDropSound = pExtPlayer->GetLowestPrioritySound();

            if (pDropSound == NULL) {
                return false;
            }

            if (startPriority < pDropSound->CalcCurrentPlayerPriority()) {
                return false;
            }

            pDropSound->Shutdown();
        }
    }

    return true;
}

void SoundPlayer::detail_AppendPlayerHeap(detail::PlayerHeap *pHeap) {
    detail::SoundThread::AutoLock lock;

    pHeap->SetSoundPlayer(this);
    mHeapList.PushBack(pHeap);
}

detail::PlayerHeap *SoundPlayer::detail_AllocPlayerHeap(detail::BasicSound *pSound) {
    detail::SoundThread::AutoLock lock;

    if (mHeapList.IsEmpty()) {
        return NULL;
    }

    detail::PlayerHeap &rHeap = mHeapList.GetFront();
    mHeapList.PopFront();

    rHeap.AttachSound(pSound);
    pSound->AttachPlayerHeap(&rHeap);
    rHeap.Clear();

    return &rHeap;
}

void SoundPlayer::detail_FreePlayerHeap(detail::BasicSound *pSound) {
    detail::SoundThread::AutoLock lock;

    detail::PlayerHeap *pHeap = pSound->GetPlayerHeap();
    if (pHeap == NULL) {
        return;
    }

    pHeap->DetachSound(pSound);
    pSound->DetachPlayerHeap(pHeap);
    mHeapList.PushBack(pHeap);
}

} // namespace snd
} // namespace nw4r
