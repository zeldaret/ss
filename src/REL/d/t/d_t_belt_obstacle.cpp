#include "d/t/d_t_belt_obstacle.h"

#include "d/a/d_a_item.h"
#include "d/a/obj/d_a_obj_base.h"
#include "f/f_profile_name.h"
#include "s/s_Math.h"
#include "toBeSorted/time_area_mgr.h"

SPECIAL_ACTOR_PROFILE(TAG_BELT_OBSTACLE, dTgBeltObstacle_c, fProfile::TAG_BELT_OBSTACLE, 0x225, 0, 0);

int dTgBeltObstacle_c::create() {
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
    mSpawnType = getFromParams(18, 0xF);
    if (mSpawnType == 0xF) {
        mSpawnType = SPAWN_BELT_OBSTACLE;
    }
    mObstacleGroupId = getFromParams(0, 0xF);
    mObstacleSceneflag = getFromParams(4, 0xFF);
    mObstacleSceneflagInverted = getFromParams(12, 0x1);
    mObstacleTimeVariant = getFromParams(13, 0xF);
    mObstacleCheckPastState = getFromParams(17, 0x1);
    mSpawnTimer = mFirstSpawnDelay;
    return SUCCEEDED;
}

int dTgBeltObstacle_c::doDelete() {
    return SUCCEEDED;
}

int dTgBeltObstacle_c::actorExecute() {
    mPastState = dTimeAreaMgr_c::GetInstance()->checkPositionIsInPastState(mRoomID, mPosition, nullptr, 100.0f);
    if (mPastState > 0.0f && sLib::calcTimer(&mSpawnTimer) == 0) {
        if (mSpawnType == SPAWN_BELT_OBSTACLE) {
            spawnBeltObstacle();
        } else if (mSpawnType == SPAWN_STAMINA_FRUIT) {
            spawnStaminaFruit();
        }
        mSpawnTimer = mSpawnInterval;
    }
    return SUCCEEDED;
}

int dTgBeltObstacle_c::draw() {
    return SUCCEEDED;
}

void dTgBeltObstacle_c::spawnBeltObstacle() {
    mVec3_c scale = mScale * 0.01f;
    u32 params = (mObstacleCheckPastState << 17 | (mObstacleTimeVariant & 0xF) << 13) |
                 ((mObstacleGroupId & 0xF) | (mObstacleSceneflag << 4) | ((mObstacleSceneflagInverted & 1) << 12));
    dAcObjBase_c *obstacle =
        dAcObjBase_c::create(fProfile::OBJ_BELT_OBSTACLE, mRoomID, params, &mPosition, &mRotation, &scale, -1);
    mObstacle.link(static_cast<dAcObeltObstacle_c *>(obstacle));
}

void dTgBeltObstacle_c::spawnStaminaFruit() {
    u32 params2 = dAcBase_c::buildParams2(-1, dAcItem_c::fn_8024A2A0(0, 0));
    dAcItem_c::spawnItem(ITEM_STAMINA_FRUIT, mRoomID, mPosition, mRotation, params2, 1);
}
