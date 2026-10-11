#include "d/t/d_t_drum.h"

#include "d/a/obj/d_a_obj_base.h"
#include "f/f_profile_name.h"
#include "s/s_Math.h"
#include "toBeSorted/time_area_mgr.h"

SPECIAL_ACTOR_PROFILE(TAG_DRUM, dTgDrum_c, fProfile::TAG_DRUM, 0x226, 0, 0);

int dTgDrum_c::create() {
    mFirstSpawnDelay = mRotation.x;
    mSpawnInterval = mRotation.z;
    mRotation.x.set(0);
    mRotation.z.set(0);
    if (mFirstSpawnDelay == 0xFFFF || mFirstSpawnDelay == 0) {
        mFirstSpawnDelay = 1;
    }
    if (mSpawnInterval == 0xFFFF || mSpawnInterval == 0) {
        mSpawnInterval = 1;
    }
    mDrumParam0 = getFromParams(0, 0xF);
    mDrumParam1 = getFromParams(4, 0xFF);
    mDrumParam2 = getFromParams(12, 0x1);
    mDrumParam3 = getFromParams(13, 0x1);
    mSpawnTimer = mFirstSpawnDelay;
    return SUCCEEDED;
}

int dTgDrum_c::doDelete() {
    return SUCCEEDED;
}

int dTgDrum_c::actorExecute() {
    mPastState =
        dTimeAreaMgr_c::GetInstance()->checkPositionIsInPastState(mRoomID, mPosition, nullptr, lbl_447_rodata_0);
    if (mPastState > lbl_447_rodata_4) {
        if (mCanSpawn && sLib::calcTimer(&mSpawnTimer) == 0) {
            mVec3_c scale(mScale.x * lbl_447_rodata_8, mScale.y * lbl_447_rodata_8, mScale.z * lbl_447_rodata_8);
            u32 params = ((mDrumParam3 << 13) | ((mDrumParam2 & 1) << 12)) | ((mDrumParam0 & 0xF) | (mDrumParam1 << 4));
            dAcObjBase_c *drum =
                dAcObjBase_c::create(fProfile::OBJ_DRUM, mRoomID, params, &mPosition, &mRotation, &scale, -1);
            mDrum.link(static_cast<dAcOdrum_c *>(drum));
            mSpawnTimer = mSpawnInterval;
        }
        if (mDrum.get() != nullptr && !mDrum.get()->getField_0xC48()) {
            mCanSpawn = false;
        } else {
            mCanSpawn = true;
        }
    }
    return SUCCEEDED;
}

int dTgDrum_c::draw() {
    return SUCCEEDED;
}

// Defined after use so that actorExecute does not load them through a rodata pool base register
const f32 dTgDrum_c::lbl_447_rodata_0 = 100.0f;
const f32 dTgDrum_c::lbl_447_rodata_4 = 0.0f;
const f32 dTgDrum_c::lbl_447_rodata_8 = 0.01f;
