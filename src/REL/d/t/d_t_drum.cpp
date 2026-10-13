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
    mDrumGroupId = getFromParams(0, 0xF);
    mDrumSceneflag = getFromParams(4, 0xFF);
    mDrumSceneflagInverted = getFromParams(12, 0x1);
    mDrumCheckPastState = getFromParams(13, 0x1);
    mSpawnTimer = mFirstSpawnDelay;
    return SUCCEEDED;
}

int dTgDrum_c::doDelete() {
    return SUCCEEDED;
}

int dTgDrum_c::actorExecute() {
    mPastState = dTimeAreaMgr_c::GetInstance()->checkPositionIsInPastState(mRoomID, mPosition, nullptr, 100.0f);
    if (mPastState > 0.0f) {
        if (mCanSpawn && sLib::calcTimer(&mSpawnTimer) == 0) {
            mVec3_c scale(mScale.x * sDrumScaleFactor, mScale.y * sDrumScaleFactor, mScale.z * sDrumScaleFactor);
            u32 params = ((mDrumCheckPastState << 13) | ((mDrumSceneflagInverted & 1) << 12)) |
                         ((mDrumGroupId & 0xF) | (mDrumSceneflag << 4));
            dAcObjBase_c *drum =
                dAcObjBase_c::create(fProfile::OBJ_DRUM, mRoomID, params, &mPosition, &mRotation, &scale, -1);
            mDrum.link(static_cast<dAcOdrum_c *>(drum));
            mSpawnTimer = mSpawnInterval;
        }
        if (mDrum.get() != nullptr && !mDrum.get()->isOnBelt()) {
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

// Only matches when defined after use (written as a literal or defined above, it ends up in the rodata pool)
const f32 dTgDrum_c::sDrumScaleFactor = 0.01f;
