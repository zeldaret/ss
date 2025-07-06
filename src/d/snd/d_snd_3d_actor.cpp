#include "d/snd/d_snd_3d_actor.h"

#include "common.h"
#include "d/a/d_a_player.h"
#include "d/snd/d_snd_3d_manager.h"
#include "d/snd/d_snd_checkers.h"
#include "d/snd/d_snd_mgr.h"
#include "d/snd/d_snd_player_mgr.h"
#include "d/snd/d_snd_state_mgr.h"
#include "nw4r/math/math_types.h"

#include <cmath>

dSnd3DActor_c::dSnd3DActor_c(dSndSourceParam *pSourceParam, s32 sourceType)
    : nw4r::snd::Sound3DActor(
          dSndPlayerMgr_c::GetInstance()->getSoundArchivePlayerForType(sourceType),
          dSnd3DManager_c::GetInstance()->getManager()
      ),
      a_field_0x7D(0),
      mIsDisabled(false),
      mIsDemoActor(false),
      mIsPaused(false),
      a_field_0x84(0.0f),
      a_field_0x88(0.0f),
      a_field_0x8C(0.0f),
      a_field_0x90(INFINITY),
      mDistanceToListener(INFINITY),
      mCameraDirectionDot(0.0f),
      mSpeed(0.0f),
      mpSourceParam(pSourceParam),
      mFlags(0),
      mDistanceToPlayer(INFINITY),
      a_field_0xE0(0.0f) {
    resetCachedRelativePositions();
    // Portability hazard
    SetUserParam(reinterpret_cast<u32>(this));
    if (dSndPlayerMgr_c::GetInstance()->shouldUseDemoPlayer(sourceType)) {
        mIsDemoActor = true;
    }
}

void dSnd3DActor_c::setPosition(const nw4r::math::VEC3 &rPosition) {
    SetPosition(rPosition);
    a_field_0xE0 = dSndStateMgr_c::GetInstance()->getField_0x49C();
    mFlags = 0;
}

void dSnd3DActor_c::resetCachedRelativePositions() {
    mPositionRelativeToPlayer.x = INFINITY;
    mPositionRelativeToPlayer.y = INFINITY;
    mPositionRelativeToPlayer.z = INFINITY;

    mPositionRelativeToListener.x = INFINITY;
    mPositionRelativeToListener.y = INFINITY;
    mPositionRelativeToListener.z = INFINITY;

    mPositionTransformedByListener.x = INFINITY;
    mPositionTransformedByListener.y = INFINITY;
    mPositionTransformedByListener.z = INFINITY;
}

void dSnd3DActor_c::setSourceParam(const dSndSourceParam *param) {
    *mpSourceParam = *param;
}

void dSnd3DActor_c::updatePositionRelativeToListener() {
    if (!checkFlag(0x1)) {
        calculatePositionRelativeToListener();
        setFlag(0x1);
    }
}

void dSnd3DActor_c::calculatePositionRelativeToListener() {
    nw4r::math::VEC3Sub(
        &mPositionRelativeToListener, &GetPosition(), &dSnd3DManager_c::GetInstance()->getSndListenerPos()
    );
}

void dSnd3DActor_c::updateDistanceToListener() {
    if (!checkFlag(0x2)) {
        updatePositionRelativeToListener();
        mDistanceToListener = VEC3Len(&mPositionRelativeToListener);
        setFlag(0x2);
    }
}

void dSnd3DActor_c::updatePositionTransformedByListener() {
    if (!checkFlag(0x10)) {
        MTXMultVec(
            dSnd3DManager_c::GetInstance()->getSndListenerMatrix(), GetPosition(), mPositionTransformedByListener
        );
        setFlag(0x10);
    }
}

void dSnd3DActor_c::updateCameraDirectionDot() {
    if (!checkFlag(0x20)) {
        updatePositionRelativeToListener();
        nw4r::math::VEC3 norm;
        VECNormalize(mPositionRelativeToListener, norm);
        mCameraDirectionDot = nw4r::math::VEC3Dot(&norm, &dSnd3DManager_c::GetInstance()->getNrmCameraDirection());
        setFlag(0x20);
    }
}

void dSnd3DActor_c::updatePositionRelativeToCameraTarget() {
    if (!checkFlag(0x4)) {
        calculatePositionRelativeToCameraTarget();
        setFlag(0x4);
    }
}

void dSnd3DActor_c::calculatePositionRelativeToCameraTarget() {
    nw4r::math::VEC3Sub(
        &mPositionRelativeToCameraTarget, &GetPosition(), &dSnd3DManager_c::GetInstance()->getCameraTargetPos()
    );
}

void dSnd3DActor_c::updateDistanceToCameraTarget() {
    if (!checkFlag(0x8)) {
        updatePositionRelativeToCameraTarget();
        mDistanceToCameraTarget = VEC3Len(&mPositionRelativeToCameraTarget);
        setFlag(0x8);
    }
}

void dSnd3DActor_c::updatePositionRelativeToPlayer() {
    if (!checkFlag(0x40)) {
        nw4r::math::VEC3 linkTranslation = dAcPy_c::GetLink()->getCenterTranslation();
        nw4r::math::VEC3 pos = GetPosition();
        nw4r::math::VEC3Sub(&mPositionRelativeToPlayer, &pos, &linkTranslation);

        setFlag(0x40);
    }
}

const nw4r::math::VEC3 &dSnd3DActor_c::getPositionRelativeToPlayer() {
    updatePositionRelativeToPlayer();
    return mPositionRelativeToPlayer;
}

void dSnd3DActor_c::updateDistanceToPlayer() {
    if (!checkFlag(0x80)) {
        updatePositionRelativeToPlayer();
        nw4r::math::VEC3 linkTranslation = dAcPy_c::GetLink()->getCenterTranslation();
        nw4r::math::VEC3 pos = GetPosition();
        if ((linkTranslation.x == 0.0f && linkTranslation.y == 0.0f && linkTranslation.z == 0.0f) ||
            (pos.x == 0.0f && pos.y == 0.0f && pos.z == 0.0f)) {
            // Don't bother if we received dummy values
            mDistanceToPlayer = INFINITY;
        } else {
            mDistanceToPlayer = nw4r::math::VEC3Len(&mPositionRelativeToPlayer);
        }
        setFlag(0x80);
    }
}

f32 dSnd3DActor_c::getDistanceToPlayer() {
    updateDistanceToPlayer();
    return mDistanceToPlayer;
}

// here are functions that are probably in part copied from nw4r::snd::Sound3DCalculator

bool dSnd3DActor_c::hasPlayingSounds() const {
    for (int i = 0; i < ACTOR_PLAYER_COUNT; i++) {
        if (GetPlayingSoundCount(i) > 0) {
            return true;
        }
    }
    return false;
}

bool dSnd3DActor_c::isPlayingSound(u32 id) {
    bool result = false;

    IsCurrentSoundIdChecker handler(id, &result);
    ForEachSound(handler, false);

    return result;
}

u32 dSnd3DActor_c::soundLabelToSoundId(const char *soundLabel) const {
    return dSndPlayerMgr_c::GetInstance()->convertLabelStringToSoundId(soundLabel);
}

const char *dSnd3DActor_c::soundIdToSoundLabel(u32 soundId) const {
    if (mIsDemoActor) {
        return dSndPlayerMgr_c::GetInstance()->getDemoArchiveDirectly()->GetSoundLabelString(soundId);
    } else {
        return dSndMgr_c::getSoundLabelString(soundId);
    }
}
