#include "nw4r/snd/snd_Sound3DActor.h"

#include "nw4r/math/math_types.h"
#include "nw4r/snd/snd_Sound3DManager.h"
#include "nw4r/snd/snd_SoundHandle.h"

namespace nw4r {
namespace snd {

Sound3DActor::Sound3DActor(SoundArchivePlayer &rPlayer, Sound3DManager &rManager)
    : SoundActor(rPlayer), m3DManager(rManager), mSoundArchivePlayer(&rPlayer), mUserParam(0),
      mPosition(0.0f, 0.0, 0.0f), mVelocity(0.0f, 0.0, 0.0f), field_0x7C(1) {}

Sound3DActor::~Sound3DActor() {
    ForEachSound(ClearUpdateCallback, false);
}

SoundStartable::StartResult
Sound3DActor::SetupSound(SoundHandle *pHandle, u32 id, const StartInfo *pStartInfo, void *arg) {
    Sound3DManager::Sound3DActorParam actorParam;
    actorParam.position = mPosition;
    actorParam.velocity = mVelocity;
    actorParam.userParam = mUserParam;

    SoundArchive::Sound3DParam param;
    if (mSoundArchivePlayer != NULL && mSoundArchivePlayer->GetSoundArchive().detail_ReadSound3DParam(id, &param)) {
        actorParam.soundParam = param;
    }

    detail::BasicSound::AmbientInfo argInfo = {
        &m3DManager,                              // paramUpdateCallback
        this,                                     // argUpdateCallback
        &m3DManager,                              // argAllocaterCallback
        &actorParam,                              // arg
        sizeof(Sound3DManager::Sound3DActorParam) // argSize
    };

    StartResult result = SoundActor::detail_SetupSoundWithAmbientInfo(pHandle, id, pStartInfo, &argInfo, arg);

    if (pHandle->IsAttachedSound()) {
        pHandle->detail_GetAttachedSound()->SetPanCurve(PAN_CURVE_SINCOS);
    }

    return result;
}

void Sound3DActor::SetPosition(const math::VEC3 &rPosition) {
    if (!field_0x7C) {
        math::VEC3Sub(&mVelocity, &rPosition, &mPosition);
    }
    mPosition = rPosition;
    field_0x7C = 0;
}

/*
void Sound3DActor::detail_Update(void* pArg, const detail::BasicSound* pSound) {
#pragma unused(pSound)

    Sound3DManager::Sound3DActorParam* pParam =
        static_cast<Sound3DManager::Sound3DActorParam*>(pArg);

    pParam->position = mPosition;
    pParam->userParam = mUserParam;
}
*/

void Sound3DActor::ClearUpdateCallback(SoundHandle &rHandle) {
    if (rHandle.IsAttachedSound()) {
        rHandle.detail_GetAttachedSound()->ClearAmbientArgUpdateCallback();
    }
}

} // namespace snd
} // namespace nw4r
