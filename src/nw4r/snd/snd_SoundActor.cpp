#include "nw4r/snd/snd_SoundActor.h"

#include "nw4r/snd/snd_SoundArchivePlayer.h"

namespace nw4r {
namespace snd {

SoundActor::SoundActor(SoundArchivePlayer &rPlayer) : mSoundArchivePlayer(rPlayer) {
    mActorParam.volume = 1.0f;
    mActorParam.pitch = 1.0f;
    mActorParam.pan = 0.0f;
    for (int i = 0; i < ACTOR_PLAYER_COUNT; i++) {
        mActorPlayer[i].SetPlayableSoundCount(i == 0 ? INT_MAX : 1);
    }
}

SoundActor::~SoundActor() {
    for (int i = 0; i < ACTOR_PLAYER_COUNT; i++) {
        mActorPlayer[i].DetachSoundActorAll(this);
    }
}

void SoundActor::StopAllSound(int arg) {
    for (int i = 0; i < ACTOR_PLAYER_COUNT; i++) {
        mActorPlayer[i].StopAllSound(arg);
    }
}

void SoundActor::PauseAllSound(bool b, int arg) {
    for (int i = 0; i < ACTOR_PLAYER_COUNT; i++) {
        mActorPlayer[i].PauseAllSound(b, arg);
    }
}

int SoundActor::GetPlayingSoundCount(int i) const {
    return mActorPlayer[i].GetPlayingSoundCount();
}

SoundStartable::StartResult SoundActor::SetupSound(
    nw4r::snd::SoundHandle *pHandle, u32 id, const nw4r::snd::SoundStartable::StartInfo *pStartInfo, void *initArg
) {
    return mSoundArchivePlayer.detail_SetupSoundImpl(pHandle, id, NULL, this, *(bool *)initArg, pStartInfo);
}

SoundStartable::StartResult SoundActor::detail_SetupSoundWithAmbientInfo(
    nw4r::snd::SoundHandle *pHandle, u32 id, const nw4r::snd::SoundStartable::StartInfo *pStartInfo, detail::BasicSound::AmbientInfo* ambientArg, void *initArg
) {
    return mSoundArchivePlayer.detail_SetupSoundImpl(pHandle, id, ambientArg, this, *(bool *)initArg, pStartInfo);
}

SoundStartable::StartResult
SoundActor::detail_SetupSound(SoundHandle *pHandle, u32 id, bool hold, const StartInfo *pStartInfo) {
    bool arg = hold;
    return SetupSound(pHandle, id, pStartInfo, (void*)&hold);
}


} // namespace snd
} // namespace nw4r
