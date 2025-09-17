#include "d/a/obj/d_a_obj_clearness_wall.h"

#include "d/a/obj/d_a_obj_base.h"
#include "d/col/bg/d_bg_s.h"
#include "d/flag/sceneflag_manager.h"
#include "f/f_base.h"
#include "toBeSorted/arc_managers/oarc_manager.h"

SPECIAL_ACTOR_PROFILE(OBJ_CLEARNESS_WALL, dAcOclearnessWall_c, fProfile::OBJ_CLEARNESS_WALL, 0x15B, 0, 0);

bool dAcOclearnessWall_c::createHeap() {
    const char *arcName = getArcName();
    const char *modelName = getModelName();
    void *dzb = OarcManager::GetInstance()->getDzbFromArc(arcName, modelName);
    void *plc = OarcManager::GetInstance()->getPlcFromArc(arcName, modelName);
    TRY_CREATE(!mBgW.Set((cBgD_t *)dzb, (PLC *)plc, cBgW::MOVE_BG_e, &mWorldMtx, &mScale));
    return SUCCEEDED;
}

int dAcOclearnessWall_c::create() {
    mSceneflag = mParams;
    mVariant = getFromParams(8, 0xF);
    field_0x542 = getFromParams(0xC, 0xF);
    if (field_0x542 == 0xF) {
        field_0x542 = 0;
    }
    updateMatrix();
    CREATE_ALLOCATOR(dAcOclearnessWall_c);
    dBgS::GetInstance()->Regist(&mBgW, this);
    mBgW.Lock();
    if (field_0x542 == 2) {
        dBgS::GetInstance()->Release(&mBgW);
    }
    if (mVariant == 4) {
        mBgW.field_0x22 = 4;
    }
    return SUCCEEDED;
}

int dAcOclearnessWall_c::doDelete() {
    return SUCCEEDED;
}

int dAcOclearnessWall_c::actorExecute() {
    switch (field_0x542) {
        case 0: {
            bool flag = SceneflagManager::sInstance->checkFlag(roomid, mSceneflag);
            if (flag) {
                deleteRequest();
            }
            break;
        }
        case 1: {
            bool flag = SceneflagManager::sInstance->checkFlag(roomid, mSceneflag);
            if (flag && field_0x543) {
                dBgS::GetInstance()->Release(&mBgW);
                field_0x543 = false;
            } else {
                bool flag = SceneflagManager::sInstance->checkFlag(roomid, mSceneflag);
                if (!flag && !field_0x543) {
                    dBgS::GetInstance()->Regist(&mBgW, this);
                    field_0x543 = true;
                }
            }
            break;
        }
        case 2: {
            bool flag = SceneflagManager::sInstance->checkFlag(roomid, mSceneflag);
            if (flag && field_0x543) {
                dBgS::GetInstance()->Regist(&mBgW, this);
                field_0x543 = false;
            } else {
                bool flag = SceneflagManager::sInstance->checkFlag(roomid, mSceneflag);
                if (!flag && !field_0x543) {
                    dBgS::GetInstance()->Release(&mBgW);
                    field_0x543 = true;
                }
            }
            break;
        }
        default: {
            bool flag = SceneflagManager::sInstance->checkFlag(roomid, mSceneflag);
            if (flag) {
                deleteRequest();
            }
        }
    }
    return SUCCEEDED;
}

int dAcOclearnessWall_c::draw() {
    return SUCCEEDED;
}

const char *dAcOclearnessWall_c::getArcName() {
    switch (mVariant) {
        case 0:  return "InvisibleWall";
        case 1:  return "InvisibleWallB";
        case 2:  return "InvisibleWallC";
        case 3:  return "InvisibleWallD";
        case 4:  return "InvisibleWallE";
        default: return nullptr;
    }
}

const char *dAcOclearnessWall_c::getModelName() {
    switch (mVariant) {
        case 0:  return "InvisibleWall";
        case 1:  return "InvisibleWall_yahane";
        case 2:  return "InvisibleWall_AirNaraku";
        case 3:  return "InvisibleWall_Camera";
        case 4:  return "InvisibleWall_tekitome";
        default: return nullptr;
    }
}
