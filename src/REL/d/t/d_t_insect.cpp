#include "d/t/d_t_insect.h"

#include "c/c_lib.h"
#include "c/c_math.h"
#include "common.h"
#include "d/a/d_a_base.h"
#include "d/a/d_a_insect_butterfly.h"
#include "d/a/d_a_item.h"
#include "d/a/d_a_itembase.h"
#include "d/a/obj/d_a_obj_base.h"
#include "d/a/obj/d_a_obj_soil.h"
#include "d/a/obj/d_a_obj_warp.h"
#include "d/col/bg/d_bg_s.h"
#include "d/col/bg/d_bg_s_gnd_chk.h"
#include "d/col/bg/d_bg_s_lin_chk.h"
#include "d/flag/itemflag_manager.h"
#include "d/flag/sceneflag_manager.h"
#include "d/flag/storyflag_manager.h"
#include "f/f_base.h"
#include "f/f_manager.h"
#include "f/f_profile.h"
#include "f/f_profile_name.h"
#include "m/m_angle.h"
#include "m/m_vec.h"
#include "nw4r/math/math_geometry.h"
#include "nw4r/math/math_types.h"
#include "rvl/MTX/mtx.h"

SPECIAL_ACTOR_PROFILE(TAG_INSECT, dTgInsect_c, fProfile::TAG_INSECT, 0x167, 0, 0);

STATE_DEFINE(dTgInsect_c, Wait);
STATE_DEFINE(dTgInsect_c, WaitCreate);
STATE_DEFINE(dTgInsect_c, WaitForceEscape);
STATE_DEFINE(dTgInsect_c, End);

static const s32 unused[] = {
    0x001E0100, 0, 0, 0, 0, 0, 0, 0,
};
static const f32 SCALE_X = 100;
static const f32 SCALE_Y = 100;

static const f32 unused2[] = {150, 0.5f};

const s32 *useUnused() {
    return unused + (s32)unused2;
}

int dTgInsect_c::actorCreate() {
    if (getSubtype() == 0) {
        return FAILED;
    }
    if (getSubtype() == 1) {
        return FAILED;
    }
    if (getSubtype() == 2) {
        return FAILED;
    }
    mInsectCount = getInsectCount();
    if (mInsectCount > 0xF) {
        mInsectCount = 0xF;
    } else {
        if (mInsectCount == 0xF) {
            mInsectCount = 1;
        }
    }
    mScale *= 0.01f;
    mRevealed = 0;
    mRevealedSpawnPos = mPosition;
    if (isTrialGateType() || isGossipStoneType() || isGoddessWallType() || isSpawnSubtype(SPAWN_BUG_MINIGAME)) {
        unk24E = 1;
    }
    return SUCCEEDED;
}

inline bool checkProfile(u16 prof, u32 target) {
    return prof == target;
}

// non matching: the inline vecs from the distance checks
// are in the wrong order on the stack
int dTgInsect_c::actorPostCreate() {
    s32 subtype = (mParams >> 4 & 0xF);
    // ??? doesn't match without the double comparison
    if (subtype == 5 || subtype == 5 || subtype == 6 || subtype == 3 || subtype == 9) {
        if (!someGroundCheck(mPosition, 1)) {
            return FAILED;
        }
    }
    dAcBase_c *obj = nullptr;
    do {
        obj = searchNextActor(obj);
        if (obj == nullptr) {
            break;
        }
        u16 prof = obj->mProfileName;
        if ((
                checkProfile(prof, fProfile::OBJ_TUBO) || checkProfile(prof, fProfile::OBJ_CARRY_STONE) ||
                checkProfile(prof, fProfile::OBJ_OCT_GRASS_LEAF) || checkProfile(prof, fProfile::OBJ_FRUIT) ||
                checkProfile(prof, fProfile::OBJ_BARREL) || checkProfile(prof, fProfile::OBJ_VSD) ||
                checkProfile(prof, fProfile::OBJ_SOIL)
                // inline vec 1
            ) &&
            getSquareDistanceTo(obj->mPosition) < 25) {
            if (subtype == 7 || subtype == 0xB || subtype == 8 || subtype == 0xC) {
                if (prof == fProfile::OBJ_SOIL && static_cast<dAcOsoil_c *>(obj)->isStateHole()) {
                    return FAILED;
                }
                setActorRef(obj);
            }
            break;
        }
    } while (true);
    if (mActorNode.isLinked()) {
        mStateMgr.changeState(StateID_WaitCreate);
    } else {
        if (isSpawnSubtype(SPAWN_BUG_MINIGAME)) {
            for (s32 i = 0; i < 16; i++) {
                unk1F8[i] = 0;
            }
            mStateMgr.changeState(StateID_Wait);
        } else if (isTrialGateType()) {
            dAcOWarp_c *warp = static_cast<dAcOWarp_c *>(fManager_c::searchBaseByProfName(fProfile::OBJ_WARP));
            if (warp == nullptr) {
                return FAILED;
            }
            if (!warp->checkThisHasSongItem() || warp->isTrialComplete()) {
                return FAILED;
            }
            // inline vec 2
            if ((warp->checkInRadius(mPosition, 1000000))) {
                mWarpRef.link(warp);
            } else {
                return FAILED;
            }
            mStateMgr.changeState(StateID_WaitForceEscape);
        } else if (isGossipStoneType()) {
            if (!ItemflagManager::sInstance->getFlagDirect(ITEM_BALLAD_OF_THE_GODDESS)) {
                return FAILED;
            }
            if (!StoryflagManager::sInstance->getFlag(STORYFLAG_IMPRISONED_1_DEFEATED)) {
                return FAILED;
            }
            bool flag = SceneflagManager::sInstance->checkFlag(mRoomID, mParams >> 0xC & 0xFF);
            if (flag) {
                return FAILED;
            }
        } else if (isGoddessWallType()) {
            if (!ItemflagManager::sInstance->getFlagDirect(ITEM_BALLAD_OF_THE_GODDESS)) {
                return FAILED;
            }
            if (!StoryflagManager::sInstance->getFlag(STORYFLAG_FARON_TRIAL_COMPLETE)) {
                return FAILED;
            }
            mStateMgr.changeState(StateID_WaitForceEscape);
        } else {
            mStateMgr.changeState(StateID_End);
        }
        spawnAll();
    }
    return SUCCEEDED;
}

void dTgInsect_c::initializeState_Wait() {}

void dTgInsect_c::executeState_Wait() {
    if (isSpawnSubtype(SPAWN_BUG_MINIGAME)) {
        s32 i = 0;
        for (; i < mInsectCount; i++) {
            if (!mLinks[i].isLinked()) {
                if (mInsectRespanwTimers[i] > 0) {
                    mInsectRespanwTimers[i]--;
                } else if (mInsectRespanwTimers[i] == 0) {
                    nw4r::math::MTX34 mtx;
                    PSMTXTrans(mtx, mPosition.x, mPosition.y, mPosition.z);
                    nw4r::math::MTX34 scale;
                    PSMTXScale(scale, mScale.x, mScale.y, mScale.x);
                    PSMTXConcat(mtx, scale, mtx);
                    nw4r::math::AABB aabb(-100, 0, -100, 100, 100, 100);
                    aabb.Set(&aabb, &mtx);
                    if (fn_801BB750(&aabb, 10000)) {
                        spawnInsect(i);
                    }
                } else {
                    mInsectRespanwTimers[i] = 900;
                }
            }
        }
    }
}
void dTgInsect_c::finalizeState_Wait() {}
void dTgInsect_c::initializeState_WaitCreate() {
    if (!isSpawnSubtype(SPAWN_BUG_MINIGAME)) {
        unk24F = 1;
        for (s32 i = 0; i < mInsectCount; i++) {
            mShouldSpawn[i] = shouldSpawn();
        }
    }
}
#pragma opt_strength_reduction on
void dTgInsect_c::executeState_WaitCreate() {
    if (mActorNode.isLinked()) {
        if (mActorNode.get()->mProfileName == fProfile::OBJ_VSD ||
            mActorNode.get()->mProfileName == fProfile::OBJ_SOIL) {
            if (mRevealed == 0) {
                return;
            }
        } else {
            if (mPosition.squareDistanceToXZ(mActorNode.get()->mPosition) < 25) {
                return;
            }
        }
    }
    mActorNode.unlink();
    for (s32 i = 0; i < mInsectCount; i++) {
        if (!isSpawnSubtype(SPAWN_BUG_MINIGAME)) {
            if (mShouldSpawn[i] != 0) {
                mShouldSpawn[i] = 0;
            } else {
                continue;
            }
        }
        dAcBase_c *insect = nullptr;
        mVec3_c tmp1 = mPosition;
        if (mRevealed) {
            tmp1 = mRevealedSpawnPos;
        }
        mAng3_c rot(0, cM::rndFX(65536), 0);
        switch (getSubtype()) {
            case SUBTYPE_LANAYRU_ANT:
                insect = dAcObjBase_c::createInsectActor(this, fProfile::INSECT_ANT, 1, &tmp1, &rot, nullptr, 0, 0x3f);
                break;
            case SUBTYPE_FARON_GRASSHOPPER:
                insect = dAcObjBase_c::createInsectActor(
                    this, fProfile::INSECT_GRASSHOPPER, 1, &tmp1, &rot, nullptr, 0, 0x3f
                );
                break;
            case SUBTYPE_SKYLOFT_MANTIS:
                insect = dAcObjBase_c::createInsectActor(
                    this, fProfile::INSECT_GRASSHOPPER, 0x10000001, &tmp1, &rot, nullptr, 0, 0x3f
                );
                break;
            case SUBTYPE_EDLIN_ROLLER:
                insect =
                    dAcObjBase_c::createInsectActor(this, fProfile::INSECT_SCARAB, 1, &tmp1, &rot, nullptr, 0, 0x3f);
                break;
        }
        if (insect != nullptr) {
            mLinks[i].link(insect);
            mInsectRespanwTimers[i] = -1;
        }
    }
    if (isSpawnSubtype(SPAWN_BUG_MINIGAME)) {
        mStateMgr.changeState(StateID_Wait);
    } else {
        mStateMgr.changeState(StateID_End);
    }
}
void dTgInsect_c::finalizeState_WaitCreate() {}

void dTgInsect_c::initializeState_WaitForceEscape() {
    for (s32 i = 0; i < mInsectCount; i++) {
        dAcOInsect_c* tmp = static_cast<dAcOInsect_c*>(mLinks[i].get());
        if (tmp) {
            tmp->setActorProperty(AC_PROP_0x4);
        }
    }
}

void dTgInsect_c::executeState_WaitForceEscape() {
    if (isTrialGateType()) {
        if (mWarpRef.isLinked()) {
            dAcOWarp_c *warp = static_cast<dAcOWarp_c *>(mWarpRef.get());
            if (warp->fn_0x90()) {
                for (s32 i = 0; i < mInsectCount; i++) {
                    if (mLinks[i].isLinked()) {
                        static_cast<dAcInsectButterfly_c *>(mLinks[i].get())->setKillSignal();
                        mStateMgr.changeState(StateID_End);
                    }
                }
            }
        }
    } else if (isGoddessWallType() && mKillSignal) {
        for (s32 i = 0; i < mInsectCount; i++) {
            if (mLinks[i].isLinked()) {
                static_cast<dAcInsectButterfly_c *>(mLinks[i].get())->setKillSignal();
                mStateMgr.changeState(StateID_End);
            }
        }
    }
}
void dTgInsect_c::finalizeState_WaitForceEscape() {}
void dTgInsect_c::initializeState_End() {}
void dTgInsect_c::executeState_End() {}
void dTgInsect_c::finalizeState_End() {}

static const f32 FLOAT_ARRAY[] = {
    0.1f, -0.4f, 0.3f, 0.7f, -0.7f,
};

void dTgInsect_c::spawnAll() {
    for (s32 i = 0; i < mInsectCount; i++) {
        if (shouldSpawn()) {
            spawnInsect(i);
        }
    }
}

// non matching: rodata is weird, using multiple different 100.0f instead of just one
// also the registers are wrong
void dTgInsect_c::spawnInsect(s32 index) {
    f32 scaledScaleY = mScale.y * SCALE_Y;
    f32 scaledScaleX = SCALE_X * mScale.x *  0.85f;
    bool spawnFound;
    mVec3_c pos;
    mVec3_c tmp;
    mAng3_c rot(0, 0, 0);
    spawnFound = false;
    s32 tries;
    tries = 5;
    do {
        f32 scale = cM::rndF(scaledScaleX);
        s16 angle1 = cM::rndFX(65536.0f);
        rot.y = cM::rndFX(65536.0f);
        mVec3_c v2 = mVec3_c::Ez * scale;
        cLib::offsetPos(pos, mPosition, angle1, v2);
        switch (getSubtype()) {
            case SUBTYPE_VOLCANIC_LADYBUG:
            case SUBTYPE_WOODLAND_RHINO_BEETLE:
            case SUBTYPE_SAND_CICADA:
            case SUBTYPE_SKY_STAG_BEETLE:
                pos = mPosition;
                if (getSubtype() == SUBTYPE_SAND_CICADA) {
                    tmp = mVec3_c::Ez;
                    tmp.rotY(mAng(mRotation.y + 0x4000));
                    f32 arrayResult = FLOAT_ARRAY[index % (s32)ARRAY_LENGTH(FLOAT_ARRAY)];
                    f32 scaledX = SCALE_X * mScale.x;
                    tmp.multScalar(arrayResult * scaledX);
                    pos = mPosition + tmp;
                    f32 scaledScale = SCALE_Y * mScale.y;
                    pos.y = mPosition.y + scaledScale * 0.3f;
                    pos.y += cM::rndF(scaledScale * 0.5f);
                    if (!someGroundCheck(pos, 0)) {
                        return;
                    }
                } else {
                    pos.y += cM::rndF(scaledScaleY);
                }
                spawnFound = true;
                break;
            case SUBTYPE_FARON_GRASSHOPPER:
            case SUBTYPE_LANAYRU_ANT:
            case SUBTYPE_SKYLOFT_MANTIS:
            case SUBTYPE_EDLIN_ROLLER:
                pos.y = 100 + mPosition.y;
                if (dBgS_ObjGndChk::CheckPos(pos)) {
                    spawnFound = true;
                }
                break;
            case SUBTYPE_GERUDO_DRAGONFLY:
            case SUBTYPE_BUTTERFLY:
            case SUBTYPE_STARRY_FIREFLY:
                spawnFound = true;
                break;
        }
    } while (!spawnFound && --tries);
    dAcBase_c *ref = nullptr;
    switch (getSubtype()) {
        case SUBTYPE_VOLCANIC_LADYBUG:
            ref = dAcObjBase_c::createInsectActor(this, fProfile::INSECT_LADYBUG, 0, &pos, &rot, NULL, 0, 0x3f);
            break;
        case SUBTYPE_GERUDO_DRAGONFLY:
            ref = dAcObjBase_c::createInsectActor(this, fProfile::INSECT_DRAGONFLY, 0, &pos, &rot, NULL, 0, 0x3f);
            break;
        case SUBTYPE_WOODLAND_RHINO_BEETLE:
            ref = dAcObjBase_c::createInsectActor(this, fProfile::INSECT_BEETLE, 0, &pos, &rot, NULL, 0, 0x3f);
            break;
        case SUBTYPE_FARON_GRASSHOPPER:
            ref = dAcObjBase_c::createInsectActor(this, fProfile::INSECT_GRASSHOPPER, 0, &pos, &rot, NULL, 0, 0x3f);
            break;
        case SUBTYPE_SAND_CICADA:
            ref = dAcObjBase_c::createInsectActor(this, fProfile::INSECT_CICADA, 0, &pos, &rot, NULL, 0, 0x3f);
            break;
        case SUBTYPE_LANAYRU_ANT:
            ref = dAcObjBase_c::createInsectActor(this, fProfile::INSECT_ANT, 0, &pos, &rot, NULL, 0, 0x3f);
            break;
        case SUBTYPE_SKY_STAG_BEETLE:
            ref = dAcObjBase_c::createInsectActor(this, fProfile::INSECT_BEETLE, 0x10000000, &pos, &rot, NULL, 0, 0x3f);
            break;
        case SUBTYPE_BUTTERFLY:
            ref = dAcObjBase_c::createInsectActor(
                this, fProfile::INSECT_BUTTERFLY, (s16)mViewClipIdx, &pos, &rot, NULL, 0, 0x3f
            );
            break;
        case SUBTYPE_SKYLOFT_MANTIS:
            ref = dAcObjBase_c::createInsectActor(
                this, fProfile::INSECT_GRASSHOPPER, 0x10000000, &pos, &rot, NULL, 0, 0x3f
            );
            break;
        case SUBTYPE_EDLIN_ROLLER:
            ref = dAcObjBase_c::createInsectActor(this, fProfile::INSECT_SCARAB, 0, &pos, &rot, NULL, 0, 0x3f);
            break;
        case SUBTYPE_STARRY_FIREFLY:
            ref = dAcObjBase_c::createInsectActor(this, fProfile::INSECT_FIREFLY, 0, &pos, &rot, NULL, 0, 0x3f);
            break;
    }
    if (ref != nullptr) {
        mLinks[index].link(ref);
        mInsectRespanwTimers[index] = -1;
    }
    return;
}

bool dTgInsect_c::shouldSpawn() {
    if (isSpawnSubtype(SPAWN_BUG_MINIGAME)) {
        return true;
    } else if (isSpawnSubtype(SPAWN_SKYLOFT_BUGKID_TREE)) {
        bool tmp = SceneflagManager::sInstance->checkFlag(mRoomID, 0x3a);
        return !tmp;
    } else {
        if (isTrialGateType()) {
            return true;
        } else if (isGossipStoneType() || isGoddessWallType()) {
            return true;
        } else if ((isSubtype(SUBTYPE_STARRY_FIREFLY)) && !dAcItem_c::checkFlag(ITEM_STARRY_FIREFLY)) {
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

bool dTgInsect_c::someGroundCheck(const mVec3_c &pos, s32 updateRotation) {
    mVec3_c tmp = mVec3_c::Ez * (mScale.x * 100);
    tmp.rotY(mRotation.y);
    tmp += pos;
    dBgS_LinChk linChk;
    linChk.mBackFlag = true;
    linChk.mFrontFlag = false;
    linChk.Set(&pos, &tmp, nullptr);
    if (!dBgS::GetInstance()->LineCross(&linChk)) {
        mAng yRot = mRotation.y + 0x7FFF;
        if (updateRotation != 0) {
            mRotation.y = yRot;
        }
        tmp = mVec3_c::Ez * (mScale.x * 100);
        tmp.rotY(yRot);
        tmp += pos;
        linChk.Set(&pos, &tmp, nullptr);
        if (!dBgS::GetInstance()->LineCross(&linChk)) {
            return false;
        }
    }
    return true;
}
