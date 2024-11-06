#include "nw4r/snd/snd_ExternalSoundPlayer.h"

#include "nw4r/snd/snd_SoundThread.h"

namespace nw4r {
namespace snd {
namespace detail {

ExternalSoundPlayer::ExternalSoundPlayer() : mPlayableCount(1) {}

ExternalSoundPlayer::~ExternalSoundPlayer() {
    NW4R_UT_LIST_SAFE_FOREACH(mSoundList, it->DetachExternalSoundPlayer(this););
}

void ExternalSoundPlayer::StopAllSound(int arg) {
    NW4R_UT_LIST_SAFE_FOREACH(mSoundList, it->Stop(arg););
}

void ExternalSoundPlayer::PauseAllSound(bool b, int arg) {
    NW4R_UT_LIST_SAFE_FOREACH(mSoundList, it->Pause(b, arg););
}

void ExternalSoundPlayer::DetachSoundActorAll(SoundActor *actor) {
    NW4R_UT_LIST_SAFE_FOREACH(mSoundList, it->DetachSoundActor(actor););
}

bool ExternalSoundPlayer::AppendSound(BasicSound *pSound) {
    SoundThread::AutoLock lock;

    int allocPriority = pSound->CalcCurrentPlayerPriority();
    if (GetPlayableSoundCount() == 0) {
        return false;
    }

    while (GetPlayingSoundCount() >= GetPlayableSoundCount()) {
        BasicSound *pDropSound = GetLowestPrioritySound();
        if (pDropSound == NULL) {
            return false;
        }
        if (allocPriority < pDropSound->CalcCurrentPlayerPriority()) {
            return false;
        }
        pDropSound->Shutdown();
    }

    mSoundList.PushBack(pSound);
    pSound->AttachExternalSoundPlayer(this);
    return true;
}

void ExternalSoundPlayer::SetPlayableSoundCount(int count) {
    mPlayableCount = count;

    while (GetPlayingSoundCount() > GetPlayableSoundCount()) {
        GetLowestPrioritySound()->Shutdown();
    }
}

void ExternalSoundPlayer::RemoveSound(BasicSound *pSound) {
    mSoundList.Erase(pSound);
    pSound->DetachExternalSoundPlayer(this);
}

void ExternalSoundPlayer::InsertSoundList(BasicSound *pSound) {
    mSoundList.PushBack(pSound);
    pSound->SetExternalSoundPlayer(this);
}

void ExternalSoundPlayer::RemoveSoundList(BasicSound *pSound) {
    mSoundList.Erase(pSound);
    pSound->SetExternalSoundPlayer(NULL);
}

BasicSound *ExternalSoundPlayer::GetLowestPrioritySound() {
    int lowestPrio = BasicSound::PRIORITY_MAX + 1;
    BasicSound *pLowest = NULL;

    for (BasicSoundExtPlayList::Iterator it = mSoundList.GetBeginIter(); it != mSoundList.GetEndIter(); ++it) {
        int priority = it->CalcCurrentPlayerPriority();

        if (lowestPrio > priority) {
            pLowest = &*it;
            lowestPrio = priority;
        }
    }

    return pLowest;
}

bool ExternalSoundPlayer::detail_CanPlaySound(int startPriority) {

    if (GetPlayableSoundCount() == 0) {
        return false;
    }

    if (GetPlayingSoundCount() >= GetPlayableSoundCount()) {
        detail::BasicSound *dropSound = GetLowestPrioritySound();
        if (dropSound == NULL) {
            return false;
        }
        if (startPriority < dropSound->CalcCurrentPlayerPriority()) {
            return false;
        }
    }
    return true;
}

} // namespace detail
} // namespace snd
} // namespace nw4r
