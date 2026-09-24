#include "d/t/d_t_view_clip.h"

#include "common.h"
#include "d/d_camera.h"
#include "d/d_sc_game.h"
#include "f/f_base.h"
#include "toBeSorted/area_math.h"
#include "toBeSorted/area_utils.h"

SPECIAL_ACTOR_PROFILE(VIEW_CLIP_TAG, dTgViewClip_c, fProfile::VIEW_CLIP_TAG, 0xB, 0, 0);

u8 dTgViewClip_c::sViewClips[dTgViewClip_c::sNumEntries] = {};

int dTgViewClip_c::create() {
    mIndex = getFromParams(0, 0x3F);
    mType = getFromParams(8, 0xF);
    if (mType == 0xF) {
        mType = TYPE_FORCE_OFF;
    }

    mAreaIndex = getFromParams(12, 0xFF);
    matrixCreateFromPosRotYScale(mArea, mPosition, mRotation.y, mScale, nullptr, 0.0f);

    return SUCCEEDED;
}

int dTgViewClip_c::doDelete() {
    return SUCCEEDED;
}

int dTgViewClip_c::actorExecute() {
    return SUCCEEDED;
}

int dTgViewClip_c::draw() {
    dCamera_c *pCamera = dScGame_c::getCamera();
    if (!checkAreaBox(mArea, pCamera->getPosition())) {
        if (mAreaIndex == 0xFF) {
            return SUCCEEDED;
        }
        if (!checkPosInArea(mAreaIndex, mRoomID, pCamera->getPosition(), nullptr)) {
            return SUCCEEDED;
        }
    }

    switch (mType) {
        case TYPE_FORCE_ON:  setViewClip(FLAG_FORCE_ON); break;
        case TYPE_FORCE_OFF: setViewClip(FLAG_FORCE_OFF); break;
    }

    return SUCCEEDED;
}

void dTgViewClip_c::setViewClip(u8 setBit) {
    setViewClipBit(mIndex, setBit);
}

void dTgViewClip_c::resetViewClip() {
    for (int i = 0; i < sNumEntries - 1; ++i) {
        setViewClip(i, 0);
    }
}

f32 dTgViewClip_c::getCull(s32 idx, f32 cullDistance) {
    if (idx > ARRAY_LENGTH(sViewClips) - 2) {
        return cullDistance;
    }

    if (getViewClipBit(idx, FLAG_FORCE_ON)) {
        return FLOAT_MAX;
    } else if (getViewClipBit(idx, FLAG_FORCE_OFF)) {
        return -1.0f;
    }

    return cullDistance;
}
bool dTgViewClip_c::checkForceOn(s32 idx) {
    if (idx <= ARRAY_LENGTH(sViewClips) - 2) {
        return getViewClipBit(idx, FLAG_FORCE_ON);
    }
    return false;
}

u8 dTgViewClip_c::getViewClipBit(s32 idx, u8 setBit) {
    return sViewClips[idx] & setBit;
}
void dTgViewClip_c::setViewClipBit(s32 idx, u8 setBit) {
    sViewClips[idx] |= setBit;
}

void dTgViewClip_c::setViewClip(s32 idx, u8 value) {
    sViewClips[idx] = value;
}
