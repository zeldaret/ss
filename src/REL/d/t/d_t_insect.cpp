#include "d/t/d_t_insect.h"
#include "c/c_lib.h"
#include "c/c_math.h"
#include "common.h"
#include "d/a/d_a_base.h"
#include "d/a/d_a_item.h"
#include "d/a/d_a_itembase.h"
#include "d/a/obj/d_a_obj_base.h"
#include "d/col/bg/d_bg_s_gnd_chk.h"
#include "d/flag/itemflag_manager.h"
#include "d/flag/sceneflag_manager.h"
#include "f/f_profile.h"
#include "f/f_profile_name.h"
#include "m/m_vec.h"

SPECIAL_ACTOR_PROFILE(TAG_INSECT, dTgInsect_c, fProfile::TAG_INSECT, 0x167, 0, 0);

STATE_DEFINE(dTgInsect_c, Wait);
STATE_DEFINE(dTgInsect_c, WaitCreate);
STATE_DEFINE(dTgInsect_c, WaitForceEscape);
STATE_DEFINE(dTgInsect_c, End);

void dTgInsect_c::initializeState_Wait() {}
void dTgInsect_c::executeState_Wait() {}
void dTgInsect_c::finalizeState_Wait() {}
void dTgInsect_c::initializeState_WaitCreate() {
    if (getSpawnSubtype() != 2) {
        unk24F = 1;
        for (s32 i = 0; i < mInsectCount; i++) {
            unk250[i] = shouldSpawn();
        }
    }
}
void dTgInsect_c::executeState_WaitCreate() {}
void dTgInsect_c::finalizeState_WaitCreate() {}
void dTgInsect_c::initializeState_WaitForceEscape() {}
void dTgInsect_c::executeState_WaitForceEscape() {}
void dTgInsect_c::finalizeState_WaitForceEscape() {}
void dTgInsect_c::initializeState_End() {}
void dTgInsect_c::executeState_End() {}
void dTgInsect_c::finalizeState_End() {}

inline bool isButterfly3(int subtype, int spawnSubtype) {
    if (subtype != 10) return false;
    if (spawnSubtype != 3) return false;
    return true;
}
inline bool isButterfly4(int subtype, int spawnSubtype) {
    if (subtype != 10) return false;
    if (spawnSubtype != 4) return false;
    return true;
}
inline bool isButterfly5(int subtype, int spawnSubtype) {
    if (subtype != 10) return false;
    if (spawnSubtype != 5) return false;
    return true;
}

static const f32 FLOAT_ARRAY[] = {
    0.1f,
    -0.4f,
    0.3f,
    0.7f,
    -0.7f,
};

static const f32 WHATEVER[] = {
    100,
    150,
    0.5f
};

static const f32 different100[] =  { 100 };



// regalloc (probably)
void dTgInsect_c::spawnInsect(s32 index) {
    f32 scaledScale = 100 * mScale.y;
    f32 scaled2 = mScale.x * WHATEVER[0] * 0.85f;
    mAng3_c rot(0,0,0);
    mVec3_c pos;
    bool spawnFound = false;
    s32 tries = 5;
    do {
        f32 scale = cM::rndF(scaled2);
        s16 angle1 = cM::rndFX(65536.0f);
        s16 angle2 = cM::rndFX(65536.0f);
        rot.y = angle2;
        mVec3_c v2 = mVec3_c::Ez * scale;
        cLib::offsetPos(pos, mPosition, angle1, v2);
        s32 subtype = mParams >> 4 & 0xF;
        switch (subtype) {
            case 3:
            case 5:
            case 6:
            case 9:
                pos = mPosition;
                if (subtype == 6) {
                    mVec3_c tmp = mVec3_c::Ez;
                    tmp.rotY(mAng(mRotation.y + 0x4000));
                    f32 arrayResult = FLOAT_ARRAY[index % 5];
                    f32 scaledX = WHATEVER[0] * mScale.x;
                    tmp.multScalar(arrayResult * scaledX);
                    pos = mPosition + tmp;
                    f32 scaledScale = mScale.y * 100;
                    pos.y = mPosition.y + scaledScale * 0.3f;
                    pos.y += cM::rndF(scaledScale * 0.5f);
                    if (!someGroundCheck(pos, 0)) {
                        return;
                    }
                } else {
                    pos.y += cM::rndF(scaledScale);
                }
                spawnFound = true;
                break;
            case 7:
            case 8:
            case 0xB:
            case 0xC:
                pos.y = different100[0] + mPosition.y;
                if (dBgS_ObjGndChk::CheckPos(pos)) {
                    spawnFound = true;
                }
                break;
            case 4:
            case 0xA:
            case 0xD:
                spawnFound = true;
                break;
        }
    } while (!spawnFound && --tries);
    s32 subtype = mParams >> 4 & 0xF;
    dAcBase_c* ref = nullptr;
    if (subtype == 3) {
        
    }
    switch (subtype) {
        case 3:
            ref = dAcObjBase_c::createInsectActor(this, fProfile::INSECT_LADYBUG, 0, &pos, &rot, NULL, 0, 0x3f);
            break;
        case 4:
            ref = dAcObjBase_c::createInsectActor(this, fProfile::INSECT_DRAGONFLY, 0, &pos, &rot, NULL, 0, 0x3f);
            break;
        case 5:
            ref = dAcObjBase_c::createInsectActor(this, fProfile::INSECT_BEETLE, 0, &pos, &rot, NULL, 0, 0x3f);
            break;
        case 6:
            ref = dAcObjBase_c::createInsectActor(this, fProfile::INSECT_GRASSHOPPER, 0, &pos, &rot, NULL, 0, 0x3f);
            break;
        case 7:
            ref = dAcObjBase_c::createInsectActor(this, fProfile::INSECT_CICADA, 0, &pos, &rot, NULL, 0, 0x3f);
            break;
        case 8:
            ref = dAcObjBase_c::createInsectActor(this, fProfile::INSECT_ANT, 0, &pos, &rot, NULL, 0, 0x3f);
            break;
        case 9:
            ref = dAcObjBase_c::createInsectActor(this, fProfile::INSECT_BEETLE, 0x10000000, &pos, &rot, NULL, 0, 0x3f);
            break;
        case 0xA:
            ref = dAcObjBase_c::createInsectActor(this, fProfile::INSECT_BUTTERFLY, mViewClipIdx, &pos, &rot, NULL, 0, 0x3f);
            break;
        case 0xB:
            ref = dAcObjBase_c::createInsectActor(this, fProfile::INSECT_GRASSHOPPER, 0x10000000, &pos, &rot, NULL, 0, 0x3f);
            break;
        case 0xC:
            ref = dAcObjBase_c::createInsectActor(this, fProfile::INSECT_SCARAB, 0, &pos, &rot, NULL, 0, 0x3f);
            break;
        case 0xD:
            ref = dAcObjBase_c::createInsectActor(this, fProfile::INSECT_FIREFLY, 0, &pos, &rot, NULL, 0, 0x3f);
            break;
    }
    if (ref != nullptr) {
        mLinks[index].link(ref);
        unk208[index] = -1;
    }
    return;
}

bool dTgInsect_c::shouldSpawn() {
    int spawnSubtype = mParams >> 8 & 0xF;
    if (spawnSubtype == 2) {
        return true;
    } else if (spawnSubtype == 1) {
        bool tmp = SceneflagManager::sInstance->checkFlag(mRoomID, 0x3a);
        return !tmp;
    } else {
        int subtype = mParams >> 4 & 0xF;
        if (isButterfly3(subtype, spawnSubtype)) {
            return true;
        } else if (isButterfly4(subtype, spawnSubtype) || isButterfly5(subtype, spawnSubtype)) {
            return true;
        } else if((subtype == 0xD) && !dAcItem_c::checkFlag(ITEM_STARRY_FIREFLY)) {
            return true;
        } else {
            if (cM::rndF(1.0f) >= 0.5f) {
                return false;
            } else {
                return true;
            }
        }
    }
}
