#include "nw4r/snd/snd_Sound3DActor.h"

#include "nw4r/snd/snd_BasicSound.h"
#include "nw4r/snd/snd_ExternalSoundPlayer.h"
#include "nw4r/snd/snd_Sound3DManager.h"
#include "nw4r/snd/snd_SoundArchive.h"
#include "nw4r/snd/snd_SoundArchivePlayer.h"
#include "nw4r/snd/snd_SoundHandle.h"
#include "nw4r/snd/snd_SoundStartable.h"
#include "nw4r/snd/snd_global.h"

namespace nw4r {
namespace snd {

Sound3DActor::Sound3DActor(SoundArchivePlayer &rPlayer, Sound3DManager &rManager)
    : SoundActor(rPlayer),
      m3DManager(rManager),
      mSoundArchivePlayer(&rPlayer),
      mUserParam(0),
      mPosition(0.0f, 0.0f, 0.0f),
      mVelocity(0.0f, 0.0f, 0.0f),
      mSkipVelocityUpdate(true) {}


Sound3DActor::~Sound3DActor() {
    ForEachSound(ClearUpdateCallback, false);
}

SoundStartable::StartResult
Sound3DActor::SetupSound(SoundHandle *pHandle, u32 soundId, const StartInfo *pStartInfo, void *pArg) {
    Sound3DParam param;
    param.position = mPosition;
    param.velocity = mVelocity;
    param.userParam = mUserParam;

    if (mSoundArchivePlayer != NULL) {
        const SoundArchive &archive = mSoundArchivePlayer->GetSoundArchive();
        nw4r::snd::SoundArchive::Sound3DParam arParam;
        if (archive.detail_ReadSound3DParam(soundId, &arParam)) {
            param.field_0x18 = arParam.flags;
            param.decayRatio = arParam.decayRatio;
            param.field_0x1E = arParam.field_0x06;
            switch (arParam.decayCurve) {
                case 1:  param.decayCurve = 1; break;
                case 2:  param.decayCurve = 2; break;
                default: param.decayCurve = 1; break;
            }
        }
        param.field_0x24 = archive.GetSoundUserParam(soundId);
    }

    detail::BasicSound::AmbientInfo info = {
        &m3DManager, this, &m3DManager, &param, sizeof(Sound3DParam),

    };
    SoundStartable::StartResult result =
        SoundActor::detail_SetupSoundWithAmbientInfo(pHandle, soundId, pStartInfo, &info, pArg);
    if (pHandle->detail_GetAttachedSound() != NULL) {
        pHandle->detail_GetAttachedSound()->SetPanCurve(PAN_CURVE_SINCOS);
    }
    return result;
}

void Sound3DActor::SetPosition(const math::VEC3 &position) {
    if (!mSkipVelocityUpdate) {
        VEC3Sub(&mVelocity, &position, &mPosition);
    }

    mPosition = position;
    mSkipVelocityUpdate = 0;
}

void Sound3DActor::at_0x0c(void *arg, detail::BasicSound *sound) {
    Sound3DParam *p = static_cast<Sound3DParam *>(arg);
    p->position = mPosition;
    p->velocity = mVelocity;
    p->userParam = mUserParam;
}

void Sound3DActor::ClearUpdateCallback(SoundHandle& rHandle) {
    if (rHandle.IsAttachedSound()) {
        rHandle.detail_GetAttachedSound()->ClearAmbientArgUpdateCallback();
    }
}

} // namespace snd
} // namespace nw4r
