#include "d/snd/d_snd_3d_actor.h"

#include "common.h"
#include "d/a/d_a_player.h"
#include "d/snd/d_snd_3d_manager.h"
#include "nw4r/math/math_types.h"
#include "toBeSorted/music_mgrs.h"

#include <cmath>

dSnd3DActor_c::dSnd3DActor_c(UNKTYPE *a1, u8 a2)
    : nw4r::snd::Sound3DActor(
          dSndPlayerMgr_c::GetInstance()->getSoundArchivePlayerForType(a2), dSnd3DManager_c::GetInstance()->getManager()
      ),
      a_field_0x7D(0),
      a_field_0x7E(0),
      a_field_0x7F(0),
      a_field_0x80(0),
      a_field_0x84(0.0f),
      a_field_0x88(0.0f),
      a_field_0x8C(0.0f),
      a_field_0x90(INFINITY),
      mDistanceToListener(INFINITY),
      a_field_0x98(0.0f),
      a_field_0x9C(0.0f),
      a_field_0xC8(a1),
      mFlags(0),
      mDistanceToPlayer(INFINITY),
      a_field_0xE0(0.0f) {
    resetFloats();
    // Portability hazard
    SetUserParam(reinterpret_cast<u32>(this));
    if (fn_8035ED90(BGM_MGR, a2)) {
        a_field_0x7F = 1;
    }
}

void dSnd3DActor_c::d_vt_0x34(const nw4r::math::VEC3 &rPosition) {
    SetPosition(rPosition);
    // TODO - 0xE0
    mFlags = 0;
}

void dSnd3DActor_c::updatePositionRelativeToListener() {
    if (!checkFlag(1)) {
        calculatePositionRelativeToListener();
        setFlag(1);
    }
}

void dSnd3DActor_c::calculatePositionRelativeToListener() {
    nw4r::math::VEC3Sub(
        &mPositionRelativeToListener, &GetPosition(), &dSnd3DManager_c::GetInstance()->getSndListenerPos()
    );
}

void dSnd3DActor_c::updateDistanceToListener() {
    if (!checkFlag(2)) {
        updatePositionRelativeToListener();
        mDistanceToListener = VEC3Len(&mPositionRelativeToListener);
        setFlag(2);
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
