#include "d/a/obj/d_a_obj_chair.h"

#include "common.h"
#include "d/a/d_a_item.h"
#include "d/a/d_a_player.h"
#include "d/a/obj/d_a_obj_base.h"
#include "d/col/bg/d_bg_s.h"
#include "d/col/cc/d_cc_d.h"
#include "d/col/cc/d_cc_s.h"
#include "d/flag/sceneflag_manager.h"
#include "f/f_base.h"
#include "m/m_angle.h"
#include "m/m_quat.h"
#include "m/m_vec.h"
#include "nw4r/g3d/res/g3d_resfile.h"
#include "nw4r/math/math_arithmetic.h"
#include "s/s_Math.h"
#include "toBeSorted/attention.h"

SPECIAL_ACTOR_PROFILE(OBJ_CHAIR, dAcOChair_c, fProfile::OBJ_CHAIR, 0x196, 0, 7);

STATE_DEFINE(dAcOChair_c, Wait);

// Honestly, I dont know. This just made the most sense to me
static const u8 _heal_cooldowns[] = {0, 60, 30};
#define HEAL_COOLDOWN (_heal_cooldowns[2])
#define FIRST_HEAL_COOLDOWN (_heal_cooldowns[1])

char *CHAIR_FILES[9] = {
    "CharA", "CharB", "CharC", "CharD", "CharE", "CharF", "CharG", "CharH", "CharI",
};

static dCcD_SrcCyl sChairColData = {
    /* mObjInf */
    {/* mObjAt */ {0, 0, {0, 0, 0}, 0, 0, 0, 0, 0, 0},
     /* mObjTg */
     {~(AT_TYPE_BUGNET | AT_TYPE_0x80000 | AT_TYPE_BEETLE | AT_TYPE_WIND | AT_TYPE_0x8000),
      0x111,
      {0, 0x7, 0x407},
      0x0,
      0x0},
     /* mObjCo */ {0xE9}},
    /* mCylInf */
    {40.f, 50.f}
};

bool dAcOChair_c::createHeap() {
    if (!isBench()) {
        mRes = nw4r::g3d::ResFile(getOarcResFile(CHAIR_FILES[mChairType]));
        TRY_CREATE(mMdl.create(mRes.GetResMdl(CHAIR_FILES[mChairType]), &heap_allocator, 0x120, 1, NULL));
    }
    return true;
}

int dAcOChair_c::create() {
    u8 paramType = params & 0xF;
    mChairType = getChairType(paramType);
    mSceneflag = params >> 4;

    CREATE_ALLOCATOR(dAcOChair_c);

    if (!isBench()) {
        mObjAcch.Set(this, 1, &mAcchCir);
        mAcchCir.SetWall(20.f, 50.f);
    }

    if (!isBench()) {
        mStts.SetRank(13);
        mCyl.Set(sChairColData);

        mCyl.SetStts(mStts);
        mCyl.SetTg_0x4C(0x2000);

        if (mChairType == CHAIR_H) {
            mCyl.SetTgInfo_0x1(6);
        }

        f32 height = 55.f;
        f32 radius = 35.f;

        if (mChairType == CHAIR_B) {
            height = 130.f;
            radius = 45.f;
        } else if (mChairType == CHAIR_C) {
            height = 160.f;
            radius = 65.f;
        }

        mCyl.SetR(radius);
        mCyl.SetH(height);

        mSeatNodeID = mMdl.getResMdl().GetResNode("SetLink").GetID();
    }

    updateMatrix();

    if (isChairTypeIdk1()) {
        mMdl.setPriorityDraw(0x1C, 9);

    } else if (!isBench()) {
        mObjAcch.CrrPos(*dBgS::GetInstance());
    }

    forwardAccel = -2.f;
    forwardMaxSpeed = -40.f;

    field_0xB1B = 1;
    field_0xB1A = 1;

    mChairRot = rotation.y;
    mStateMgr.changeState(StateID_Wait);

    boundingBox.Set(mVec3_c(-60.f, -0.f, -60.f), mVec3_c(60.f, 160.f, 60.f));

    return SUCCEEDED;
}

int dAcOChair_c::doDelete() {
    return SUCCEEDED;
}

inline bool isPlayerSitting() {
    bool ret = false;
    if (
        dAcPy_c::GetLink()->checkCurrentAction(173) /* SITTING*/ ||
        dAcPy_c::GetLink()->checkCurrentAction(174) /*   ???  */
    ) {
        ret = true;
    }
    return ret;
}

int dAcOChair_c::actorExecute() {
    mStateMgr.executeState();

    // Calculate the HealCooldown (Heal link if needed)
    if (!checkObjectProperty(0x8000)) {
        if (!isBench()) {
            mCyl.SetC(GetPostion());
            dCcS::GetInstance()->Set(&mCyl);
        }
        mHealTimer = FIRST_HEAL_COOLDOWN;
    } else if (isPlayerSitting()) {
        if (sLib::calcTimer(&mHealTimer) == 0) {
            dAcItem_c::healLink(4, 1);
            mHealTimer = HEAL_COOLDOWN;
        }
    }

    // Set the flag that link is sitting
    if (checkObjectProperty(0x8000) && isPlayerSitting()) {
        if (mSceneflag < 0xFF && !SceneflagManager::sInstance->checkBoolFlag(roomid, mSceneflag)) {
            SceneflagManager::sInstance->setFlag(roomid, mSceneflag);
        }
    } else {
        if (mSceneflag < 0xFF && SceneflagManager::sInstance->checkBoolFlag(roomid, mSceneflag)) {
            SceneflagManager::sInstance->unsetFlag(roomid, mSceneflag);
        }
    }

    if (!isBench()) {
        if (isChairTypeIdk0() && !checkObjectProperty(0x8000)) {
            if (sLib::absDiff(GetRotation().y, getXZAngleToPlayer()) > 910) {
                GetRotation().y = getXZAngleToPlayer();
            }
        }

        updateMatrix();

        if (isChairTypeIdk0()) {
            mWorldMtx.YrotM(mChairRot - GetRotation().y);
        }
        mMdl.setLocalMtx(mWorldMtx);
        mMdl.calc(false);
    } else {
        if (isChairTypeIdk0() && !checkObjectProperty(0x8000)) {
            if (sLib::absDiff(GetRotation().y, getXZAngleToPlayer()) > 910) {
                GetRotation().y = getXZAngleToPlayer();
            }
        }
    }

    updateChairPos();

    return SUCCEEDED;
}

int dAcOChair_c::draw() {
    if (!isBench()) {
        drawModelType1(&mMdl);
        static mQuat_c shadowRot(0.f, 40.f, 0.f, 80.f);
        drawShadow(mShadow, nullptr, mWorldMtx, &shadowRot, -1, -1, -1, -1, -1, 0.f);
    }
    return SUCCEEDED;
}

void dAcOChair_c::initializeState_Wait() {}

void dAcOChair_c::executeState_Wait() {
    const f32 height_diff = position.y - dAcPy_c::GetLink()->position.y;
    if (!isBench() || (50.f < height_diff && height_diff < 60.f)) {
        if (field_0xB1A && field_0xB1B) {
            if (dAcPy_c::GetLink()->checkActionFlagsCont(0x1000)) {
                return;
            }
            if (!isBench() && nw4r::math::FAbs(height_diff) > 10.f) {
                return;
            }
            if (field_0xB1C && mChairType == CHAIR_E) {
                const f32 mag = (mChairPos - poscopy2).squareMagXZ();
                if (mag < 10000.f) {
                    return;
                }
            }
            AttentionManager::GetInstance()->addSitTarget(*this, isBench() ? 0 : 3, 120.f);
        }
    }
}

void dAcOChair_c::finalizeState_Wait() {}

dAcOChair_c::ChairType dAcOChair_c::getChairType(u8 &param) {
    switch (actor_subtype) {
        case 0: return CHAIR_A;
        case 1: return CHAIR_B;
        case 2: return CHAIR_C;
        case 3: return CHAIR_D;
        case 4: return CHAIR_E;
        case 5: {
            switch (param) {
                case 0:  return CHAIR_A;
                case 1:  return CHAIR_D;
                case 2:  return CHAIR_I;
                case 3:  return CHAIR_F;
                case 4:  return CHAIR_G;
                case 5:  return CHAIR_H;
                default: return CHAIR_A;
            }
        }
        default: return CHAIR_A;
    }
}

void dAcOChair_c::updateChairPos() {
    if (mChairType != CHAIR_E) {
        if (isBench()) {
            poscopy2 = position;
        } else {
            mMdl.getNodeWorldMtxMultVecZero(mSeatNodeID, poscopy2);
        }
        poscopy3 = poscopy2;
    } else {
        field_0xB1A = true;
        if (!checkObjectProperty(0x8000)) {
            mVec3_c work = mVec3_c::Ez * getDistToPlayer();
            mAng rot = getXZAngleToPlayer();
            work.rotY(rot - rotation.y);
            work.z = 0.f;
            work.rotY(rotation.y);
            poscopy2 = position;
            if (work.squareMagXZ() < 10000.f) {
                poscopy2 += work;
            } else {
                field_0xB1A = false;
            }
        }
        poscopy3 = poscopy2;
    }
}
