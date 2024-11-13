#include "d/t/d_t_reaction.h"

#include "c/c_lib.h"
#include "c/c_math.h"
#include "d/a/d_a_item.h"
#include "d/a/d_a_player.h"
#include "d/col/c/c_cc_d.h"
#include "d/col/cc/d_cc_s.h"
#include "d/flag/sceneflag_manager.h"

SPECIAL_ACTOR_PROFILE(TAG_REACTION, dTgReaction_c, fProfile::TAG_REACTION, 0x0151, 0, 0);

STATE_DEFINE(dTgReaction_c, Wait);
#pragma explicit_zero_data on
const u32 fZero = 0;
#pragma explicit_zero_data off
const f32 dTgReaction_c::sRadius = 100.0f;
const f32 dTgReaction_c::sHeight = 100.0f;

// clang-format off
dCcD_SrcCyl dTgReaction_c::sCcSrc = {
    {{0, 0, 0, 0, 0, 0, 0, 0, 0}, 
    {AT_TYPE_BELLOWS, 0x213, 0, 0x8, 0x8,}, 
    {0xE8}},
    {dTgReaction_c::sRadius, dTgReaction_c::sHeight}
};
// clang-format on

#pragma explicit_zero_data on
static u32 initialRotX = 0;
#pragma explicit_zero_data off

bool dTgReaction_c::createHeap() {
    return true;
}

extern "C" bool isHeroMode();
extern "C" u32 adventurePouchCountItem(u32 itemId);

int dTgReaction_c::create() {
    // Note the double _c here
    if (!initAllocatorWork1Heap(0x1000, "dTgReaction_c_c::m_allocator", 0x20)) {
        return FAILED;
    }

    if (getReactType() != REACT_UNDERWATER) {
        if (getSceneFlag() >= 0xFF) {
            return FAILED;
        }
        if (SceneflagManager::sInstance->checkBoolFlag(roomid, getSceneFlag())) {
            return FAILED;
        }
    }

    field_0x4E0 = rotation.x & 0xFF;
    angle.x = initialRotX;
    rotation.x = angle.x;
    if (field_0x4E0 < 0xFF && !SceneflagManager::sInstance->checkBoolFlag(roomid, field_0x4E0)) {
        return FAILED;
    }

    if (isHeroMode()) {
        if (getParam0x10() == 1) {
            field_0x4DE = 1;
        }

        if (field_0x4DE) {
            u32 heartMedalCount = adventurePouchCountItem(ITEM_HEART_MEDAL);
            if (heartMedalCount == 0) {
                return FAILED;
            } else if (heartMedalCount == 1 && cM::rnd() < 0.5f) {
                return FAILED;
            }
        }
    }

    mStts.SetRank(0);
    mCollision.Set(sCcSrc);
    mCollision.SetStts(mStts);

    switch (getReactType()) {
        case REACT_SLINGSHOT:
            mCollision.SetTgType(AT_TYPE_SLINGSHOT);
            mCollision.SetTgFlag_0xA(0);
            mCollision.SetR(sCcSrc.mCylInf.mRadius * mScale.x);
            mCollision.SetH(sCcSrc.mCylInf.mHeight * mScale.y);
            break;
        case REACT_GUST_BELLOWS:
            mCollision.SetTgType(AT_TYPE_BELLOWS);
            mCollision.SetTgFlag_0xA(8);
            mCollision.SetR(sCcSrc.mCylInf.mRadius * mScale.x);
            mCollision.SetH(sCcSrc.mCylInf.mHeight * mScale.y);
            break;
        case REACT_UNDERWATER:
            mCollision.SetTgType(AT_TYPE_BUBBLE);
            mCollision.SetTgFlag_0xA(0);
            mCollision.SetR(sCcSrc.mCylInf.mRadius * mScale.x);
            mCollision.SetH(sCcSrc.mCylInf.mHeight * mScale.y);
            break;
        case REACT_BONK:
        case REACT_4:    break;
    }

    int item = getParam0x10();
    if (item != 0xF) {
        DowsingTarget::DowsingSlot slot;
        if (item == 0) {
            slot = DowsingTarget::SLOT_RUPEE;
        } else if (item == 1) {
            slot = DowsingTarget::SLOT_HEART;
        } else {
            return FAILED;
        }
        mVec3_c dwsOffset;
        if (!getParam0x14()) {
            field_0x4E4 = mScale.y * sCcSrc.mCylInf.mHeight * 0.5f;
            dwsOffset = mVec3_c(0.0f, sCcSrc.mCylInf.mHeight * 0.5f, 0.0f);
        } else {
            dwsOffset = mVec3_c::Zero;
        }
        mDowsingTarget.initialize(slot, 0, &dwsOffset, 5000.0f);
        mDowsingTarget.doRegister();
        mHasDowsingTarget = true;
    }

    mStateMgr.changeState(StateID_Wait);
    return SUCCEEDED;
}

int dTgReaction_c::doDelete() {
    onDelete();
    return SUCCEEDED;
}

int dTgReaction_c::actorExecute() {
    if (SceneflagManager::sInstance->checkBoolFlag(roomid, getSceneFlag())) {
        onDelete();
        return SUCCEEDED;
    } else {
        mStateMgr.executeState();
        return SUCCEEDED;
    }
}

int dTgReaction_c::draw() {
    return SUCCEEDED;
}

void dTgReaction_c::initializeState_Wait() {}
void dTgReaction_c::executeState_Wait() {
    switch (getReactType()) {
        case REACT_BONK:
        case REACT_4:            checkForBonkItem(); break;
        case REACT_SLINGSHOT:
        case REACT_GUST_BELLOWS: checkForSlingBellowsItem(); break;
        case REACT_UNDERWATER:   checkForBubble(); break;
    }
}
void dTgReaction_c::finalizeState_Wait() {}

extern "C" void *SOUND_EFFECT_SOUND_MGR;
extern "C" void SmallSoundManager__playSound(void *, u16);

void dTgReaction_c::checkForBonkItem() {
    if (dAcPy_c::LINK != nullptr && dAcPy_c::LINK->checkFlags0x350(0x2000)) {
        mVec3_c diff = position - dAcPy_c::LINK->position;
        f32 dist = diff.x * diff.x + diff.z * diff.z;
        f32 rad = mScale.x * 100.0f;
        if (!(dist < rad * rad)) {
            return;
        }

        if (getReactType() == REACT_4) {
            if (field_0x4DD == 0) {
                mVec3_c c = mVec3_c::Ez * rad;
                mVec3_c c2 = position;
                c.rotY(rotation.y);
                c2 += c;
                c2.y += field_0x4E4;
                u32 newItemParms = dAcItem_c::createItemParams(ITEM_HEART_PIECE, 1, 0, getSceneFlag(), 1, 0xFF);
                if (dAcObjBase_c::create(fProfile::ITEM, roomid, newItemParms, &c2, nullptr, nullptr, 0xFFFFFFFF)) {
                    field_0x4DD = 1;
                    onDelete();
                    SmallSoundManager__playSound(SOUND_EFFECT_SOUND_MGR, 0x13AD); // TODO (Sound ID)
                }
            }
        } else {
            u32 uVar3;
            mVec3_c pos = position;
            int p = getParam0x08();
            if (p == 0) {
                uVar3 = 6;
            } else if (p == 0xFF) {
                uVar3 = 5;
            } else if (p == 1) {
                uVar3 = 5;
                mVec3_c c = mVec3_c::Ez * rad;
                c.rotY(rotation.y);
                pos += c;
            } else {
                uVar3 = 6;
            }

            pos.y += field_0x4E4;
            if (fn_578_DB0(pos, uVar3)) {
                SmallSoundManager__playSound(SOUND_EFFECT_SOUND_MGR, 0x13AE); // TODO (Sound ID)
            }
            SceneflagManager::sInstance->setFlag(roomid, getSceneFlag());
            onDelete();
        }
    }
}

void dTgReaction_c::checkForBubble() {
    if (mCollision.ChkTgHit() && mCollision.ChkTgAtHitType(AT_TYPE_BUBBLE)) {
        if (dAcPy_c::LINK != nullptr && dAcPy_c::LINK->checkFlags0x350(0x40)) {
            mVec3_c spawnPos = position;
            dAcObjBase_c::create(fProfile::OBJ_BUBBLE, roomid, 0x4, &spawnPos, nullptr, nullptr, 0xFFFFFFFF);
        }
    }
    mCollision.SetC(position);
    dCcS::GetInstance()->Set(&mCollision);
}

void dTgReaction_c::checkForSlingBellowsItem() {
    if (mCollision.ChkTgHit()) {
        u8 p = getParam0x08();
        u32 uVar3;
        if (p == 0) {
            uVar3 = 6;
        } else {
            uVar3 = 6;
            if ((int)p == 0xFF) {
                uVar3 = 5;
            }
        }
        mVec3_c spawnPos = position;
        spawnPos.y += field_0x4E4;
        if (fn_578_DB0(spawnPos, uVar3)) {
            SmallSoundManager__playSound(SOUND_EFFECT_SOUND_MGR, 0x13AE); // TODO (Sound ID)
        }
        SceneflagManager::sInstance->setFlag(roomid, getSceneFlag());
        onDelete();
    }
    mCollision.SetC(position);
    dCcS::GetInstance()->Set(&mCollision);
}

void dTgReaction_c::onDelete() {
    if (mHasDowsingTarget) {
        mDowsingTarget.doUnregister();
        mHasDowsingTarget = false;
    }
}

bool dTgReaction_c::fn_578_DB0(const mVec3_c &position, u32 arg) {}

bool dTgReaction_c::spawnHearts(s32 params, const mVec3_c &pos, s32 arg, mAng angle) {
    int numHearts = params == 6 ? 3 : 1;
    mAng3_c ang(0, 0, 0);

    // This is annoying because we don't know which operators
    // mAng supports
    mAng tmp1;
    int tmp3;
    if (arg == 6) {
        tmp1 = angle;
        static s32 SOME_ANG = -3641;
        tmp3 = SOME_ANG;
    } else {
        tmp1 = cLib::targetAngleY(dAcPy_c::LINK->position, pos);
        tmp3 = -0x8000;
        tmp1 = tmp1 + mAng(0x4000);
    }

    mAng tmp2 = tmp3;
    s16 stepSize = tmp2 / numHearts;
    tmp2 = stepSize / 2;
    int step = tmp1 + tmp2;
    tmp2 = mAng(tmp2 / 2);

    for (int i = 0; i < numHearts; i++) {
        mAng offset = cM::rndRange(-tmp2, tmp2);
        ang.y = mAng(step) + offset;
        if (arg == 5) {
            dAcItem_c::spawnItem(ITEM_HEART, roomid, pos, ang, 0xFFFFFFFF, 1);
        } else if (arg == 6) {
            dAcItem_c::spawnItem(ITEM_HEART, roomid, pos, ang, 0xFFFFFFFF, 0);
        } else {
            dAcItem_c::spawnDrop(ITEM_HEART, roomid, pos, ang);
        }
        step = mAng(step) - stepSize;
    }
    return true;
}
