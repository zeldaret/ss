#include "d/a/obj/d_a_obj_scattersand.h"

#include "c/c_lib.h"
#include "c/c_math.h"
#include "common.h"
#include "d/a/d_a_base.h"
#include "d/a/d_a_itembase.h"
#include "d/a/d_a_player.h"
#include "d/a/obj/d_a_obj_base.h"
#include "d/col/bg/d_bg_s.h"
#include "d/col/c/c_cc_d.h"
#include "d/col/cc/d_cc_s.h"
#include "d/flag/sceneflag_manager.h"
#include "d/snd/d_snd_wzsound.h"
#include "d/t/d_t_insect.h"
#include "f/f_base.h"
#include "f/f_manager.h"
#include "f/f_profile_name.h"
#include "m/m_angle.h"
#include "m/m_vec.h"
#include "nw4r/g3d/res/g3d_resfile.h"
#include "s/s_Math.h"

struct dAcOScatterSand_HIO_c {
    f32 field_0x00;
    f32 field_0x04;
    f32 field_0x08;
    f32 field_0x0C;
    f32 field_0x10;
    f32 field_0x14;
    static const dAcOScatterSand_HIO_c sHio;
};
const dAcOScatterSand_HIO_c dAcOScatterSand_HIO_c::sHio = {0.08f, 0.12f, 0.0f, 0.4f, 20.0f, 0.5f};

SPECIAL_ACTOR_PROFILE(OBJ_VSD, dAcOScatterSand, fProfile::OBJ_VSD, 0xD8, 0, 3);

static dCcD_SrcCyl sSrcCyl = {
    /* mObjInf */
    {/* mObjAt */ {AT_TYPE_NONE, 0, {0, 0, 0}, 0, 0, 0, 0, CUT_DIR_NONE, 0},
     /* mObjTg */ {AT_TYPE_BELLOWS, 0x13, {0, 0, 0x40F}, 8, CUT_DIR_NONE},
     /* mObjCo */ {0}},
    /* mSphInf */
    {50.0f, 41.65f}
};

bool dAcOScatterSand::createHeap() {
    mRes = nw4r::g3d::ResFile(getOarcResFile("SandHill"));
    TRY_CREATE(mMdl.create(mRes.GetResMdl("SandHill"), &mAllocator, 0x120));
    mMdl.setPriorityDraw(0x24, 9);
    cBgD_t *pDzb = static_cast<cBgD_t *>(getOarcFile("SandHill", "dzb/SandHill.dzb"));
    PLC *pPlc = static_cast<PLC *>(getOarcFile("SandHill", "dat/SandHill.plc"));
    updateMatrix();
    return !mBgW.Set(pDzb, pPlc, cBgW::MOVE_BG_e, &mWorldMtx, &mScale);
}

int dAcOScatterSand::actorCreate() {
    CREATE_ALLOCATOR(dAcOScatterSand);
    mClearedSceneflag = getFromParams(12, 0xFF);
    if (mClearedSceneflag < 0xFF && SceneflagManager::sInstance->checkBoolFlag(mRoomID, mClearedSceneflag)) {
        return FAILED;
    }

    dBgS::GetInstance()->Regist(&mBgW, this);
    mBgW.SetCrrFunc(dBgS_MoveBGProc_Typical);

    mSpawnScaleY = getScale().y;
    mSpawnScale = getScale();

    mMdl.setScale(mScale);

    mStts.SetRank(13);

    mCyl.Set(sSrcCyl);
    mCyl.SetStts(mStts);

    mGravity = 0.0f;
    mMaxFallSpeed = -40.0f;

    mBoundingBox.Set(
        mVec3_c(mScale.x * -60.0f, -50.0f, mScale.z * -60.0f),
        mVec3_c(mScale.x * 60.0f, mScale.y * 60.0f, mScale.z * 60.0f)
    );
    field_0x6D8 = 0;
    field_0x6DA = 0;
    field_0x6E1 = 0;
    field_0x6E2 = 0;
    field_0x6E3 = 0;
    field_0x6DE = 0;
    field_0x6E4 = 0;
    return SUCCEEDED;
}

int dAcOScatterSand::actorPostCreate() {
    fBase_c *pParent = nullptr;
    f32 f;

    while ((pParent = fManager_c::searchBaseByProfName(fProfile::TAG_INSECT, pParent))) {
        if (static_cast<dTgInsect_c *>(pParent)->mPosition.squareDistance(mPosition) < 2500.0f) {
            mInsect.link(static_cast<dTgInsect_c *>(pParent));
            break;
        }
    }

    mTimeArea.check(mRoomID, mPosition, 0, 20.0f, 1.0f);
    if (mTimeArea.getDistMaybe() >= 1.0f) {
        field_0x6E2 = 1;
        field_0x6E3 = 1;
        mScale.setZero();
    }

    return SUCCEEDED;
}

int dAcOScatterSand::doDelete() {
    return SUCCEEDED;
}

f32 dAcOScatterSand::fn_110_480() const {
    int scale = mParams >> 4 & 0xFF;
    if (scale < 0 || mSpawnScaleY > scale) {
        return 0.4f;
    }
    return scale * 10.0f / 41.65f;
}

extern "C" void spawnDrop(ITEM_ID, s32 roomId, const mVec3_c &pos, const mAng3_c &rot);

void dAcOScatterSand::revealInside() {
    s32 item = getFromParams(0, 0xF);
    mVec3_c spawnPos = mPosition;
    if (mScale.y > 2.0f) {
        spawnPos.y += 5.0f * (mScale.y - 2.0f);
    }
    if (mInsect.isLinked()) {
        mInsect.get()->reveal(spawnPos);
    } else {
        switch (item) {
            default:      itemDroppingAndGivingRelated(&spawnPos, 0); break;
            case 0 ... 4: {
                int spawnItem;
                switch (item) {
                    case 0: spawnItem = ITEM_GREEN_RUPEE; break;
                    case 1: spawnItem = ITEM_BLUE_RUPEE; break;
                    case 2: spawnItem = ITEM_RED_RUPEE; break;
                    case 3: spawnItem = ITEM_SILVER_RUPEE; break;
                    case 4: spawnItem = ITEM_GOLD_RUPEE; break;
                }
                spawnDrop((ITEM_ID)spawnItem, mRoomID, spawnPos, mRotation);
            } break;
            case 5: {
                mAng3_c spawnRot(0, cLib::targetAngleY(mPosition, dAcPy_c::GetLink()->mPosition), 0);
                if (item == 5) {
                    dAcObjBase_c::create(fProfile::E_KG, mRoomID, 0x10FF8FF, &spawnPos, &spawnRot, nullptr, 0xFFFFFFFF);
                }
            } break;
        }
    }

    field_0x6E1 = 1;
    return;
}

void dAcOScatterSand::calcTimer() {
    if (field_0x6DE > 0) {
        field_0x6DE--;
    }
}

int dAcOScatterSand::preExecute() {
    field_0x6E3 = isVisible();
    if (field_0x6E3 != field_0x6E2 || field_0x6E4) {
        unsetActorProperty(AC_PROP_0x1);
    }

    if (dAcObjBase_c::preExecute() != NOT_READY) {
        return SUCCEEDED;
    }
    if (!field_0x6E3 || field_0x6DA != 0) {
        mBgW.Move();
    }

    return NOT_READY;
}
int dAcOScatterSand::actorExecute() {
    if (field_0x6E4) {
        if (field_0x6DC > 0) {
            if (--field_0x6DC == 0) {
                dBgS::GetInstance()->Release(&mBgW);
                deleteRequest();
                if (!field_0x6E1) {
                    revealInside();
                }
                if (mClearedSceneflag < 0xFF &&
                    !SceneflagManager::sInstance->checkBoolFlag(mRoomID, mClearedSceneflag)) {
                    SceneflagManager::sInstance->setFlag(mRoomID, mClearedSceneflag);
                }
            } else {
                f32 factor = 1.0f / field_0x6DC;
                sLib::chase(&mScale.x, 0.0f, mScale.x * factor);
                sLib::chase(&mScale.y, 0.0f, mScale.y * factor);
                sLib::chase(&mScale.z, 0.0f, mScale.z * factor);
            }
        }
    } else if (field_0x6E3) {
        if (!field_0x6E2) {
            field_0x6E2 = true;
            field_0x6DA = 10;
        }
        if (field_0x6DA > 0) {
            if (--field_0x6DA == 0) {
                if (mClearedSceneflag < 0xFF &&
                    SceneflagManager::sInstance->checkBoolFlag(mRoomID, mClearedSceneflag)) {
                    deleteRequest();
                } else {
                    dBgS::GetInstance()->Release(&mBgW);
                    mScale.setZero();
                    setActorProperty(AC_PROP_0x1);
                }
            } else {
                f32 factor = 1.0f / field_0x6DA;
                sLib::chase(&mScale.x, 0.0f, mScale.x * factor);
                sLib::chase(&mScale.y, 0.0f, mScale.y * factor);
                sLib::chase(&mScale.z, 0.0f, mScale.z * factor);
            }
        }
    } else {
        if (field_0x6E2) {
            dBgS::GetInstance()->Regist(&mBgW, this);
            field_0x6E2 = false;
            field_0x6DA = 10;
        }
        if (mCyl.ChkTgHit() && mCyl.GetTgAtHitType() == AT_TYPE_BELLOWS) {
            calcTimer();
            holdSound(SE_vmSand_BLOW_LV);
            if (field_0x6D8 < 0) {
                field_0x6D8++;
                unsetActorProperty(AC_PROP_0x1);
            } else {
                // TODO - Im not dealing with this rn
                f32 scaleX;
                f32 scaleY = (mSpawnScale.x + mSpawnScale.z) * 0.5f;
                0.08f;
                0.2f;
                1.8f;
                0.12f;
                3.0f;
                if (mSpawnScale.y < 0.2f) {
                    scaleX = cM::minMaxLimit(3.0f / scaleY, 0.5f, 2.0f);
                    scaleX *= 0.12f;
                    scaleY = (mSpawnScale.y * 0.12f) / scaleY;
                } else {
                    scaleX = cM::minMaxLimit(3.0f / scaleY, 0.5f, 2.0f);
                    f32 temp = (mScale.y - 0.2f) / 1.8f;
                    scaleX = (1.0f - MAX(temp, 1.0f)) * scaleX * 0.12f;

                    scaleY =
                        MIN(1.0f / scaleY, 0.5f) * temp * 0.08f + ((1.0f - temp) * (mSpawnScale.y * 0.12f) / scaleY);
                }
                mSpawnScale.x -= scaleX;
                mSpawnScale.y -= scaleY;
                mSpawnScale.z -= scaleX;

                f32 rad = (mSpawnScale.x + mSpawnScale.z) / 2.0f;
                bool tooSmall = mSpawnScale.y < 0.5f && rad < 1.0f;
                if (tooSmall) {
                    deleteRequest();
                    if (!field_0x6E1) {
                        revealInside();
                    }
                    if (mClearedSceneflag < 0xFF &&
                        !SceneflagManager::sInstance->checkBoolFlag(mRoomID, mClearedSceneflag)) {
                        SceneflagManager::sInstance->setFlag(mRoomID, mClearedSceneflag);
                    }
                } else {
                    if (!field_0x6E1) {
                        if ((s32)getFromParams(0, 0xF) > 0) {
                            if (mScale.y < fn_110_480()) {
                                revealInside();
                            }
                        } else {
                            if (mScale.y < 0.4f) {
                                revealInside();
                            }
                        }
                    }

                    if (mScale.y < fn_110_480()) {
                        if (mClearedSceneflag < 0xFF &&
                            !SceneflagManager::sInstance->checkBoolFlag(mRoomID, mClearedSceneflag)) {
                            SceneflagManager::sInstance->setFlag(mRoomID, mClearedSceneflag);
                        }
                    }
                }
            }
        } else {
            field_0x6D8 = 0;
            field_0x6DE = 0;
            if (field_0x6DA <= 0) {
                setActorProperty(AC_PROP_0x1);
            }
        }

        if (field_0x6DA > 0) {
            f32 scale = 1.0f / field_0x6DA;
            mScale.set(mSpawnScale.x * scale, mSpawnScale.y * scale, mSpawnScale.z * scale);
            field_0x6DA--;
        } else {
            mScale = mSpawnScale;
            setActorProperty(AC_PROP_0x1);
        }

        mCyl.SetC(mPosition);

        f32 r = sSrcCyl.mCylInf.mRadius * (mScale.x + mScale.z) / 2.0f;
        if (r < 50.0f) {
            r = 50.0f;
        }
        mCyl.SetR(r);
        f32 h = sSrcCyl.mCylInf.mHeight * mScale.y;
        if (h < 50.0f) {
            h = 50.0f;
        }
        mCyl.SetH(h);
        dCcS::GetInstance()->Set(&mCyl);
    }

    mMdl.setScale(mScale);
    updateMatrix();
    mMdl.setLocalMtx(mWorldMtx);
    mBgW.Move();
    mMdl.calc(false);
    return SUCCEEDED;
}

bool dAcOScatterSand::isVisible() {
    mTimeArea.check(mRoomID, mPosition, 0, 20.0f, 0.1f);
    return mTimeArea.getDistMaybe() >= 0.5f;
}

int dAcOScatterSand::draw() {
    drawModel(&mMdl);
    return SUCCEEDED;
}
