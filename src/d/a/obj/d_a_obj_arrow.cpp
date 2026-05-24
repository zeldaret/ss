#include "d/a/obj/d_a_obj_arrow.h"

#include "c/c_lib.h"
#include "c/c_math.h"
#include "common.h"
#include "d/a/d_a_item.h"
#include "d/a/d_a_itembase.h"
#include "d/a/d_a_player.h"
#include "d/a/obj/d_a_obj_base.h"
#include "d/col/bg/d_bg_pc.h"
#include "d/col/bg/d_bg_s.h"
#include "d/col/bg/d_bg_s_gnd_chk.h"
#include "d/col/bg/d_bg_s_lin_chk.h"
#include "d/col/c/c_bg_s_poly_info.h"
#include "d/col/c/c_cc_d.h"
#include "d/col/c/c_m3d_g_pla.h"
#include "d/col/cc/d_cc_s.h"
#include "d/d_angle.h"
#include "d/d_jnt_col.h"
#include "d/d_light_env.h"
#include "d/d_linkage.h"
#include "d/d_vec.h"
#include "d/snd/d_snd_source_if.h"
#include "d/snd/d_snd_wzsound.h"
#include "f/f_profile_name.h"
#include "m/m_mtx.h"
#include "m/m_vec.h"
#include "nw4r/math/math_types.h"
#include "nw4r/types_nw4r.h"
#include "s/s_Math.h"
#include "toBeSorted/d_emitter.h"

SPECIAL_ACTOR_PROFILE(ARROW, dAcArrow_c, fProfile::ARROW, 0x126, 0, 0x80);

static dBgS_ArrowLinChk sArrowLinChk;
u16 dAcArrow_c::sCounter;

inline static u32 getDamage1() {
    return 16;
}

inline static u32 getDamage2() {
    return 16;
}

inline static u32 getDamage3() {
    return 24;
}

inline static u32 getDamage4() {
    return 32;
}

const u8 dAcArrow_c::sDamageArrMaybe[] = {
    getDamage1(),
    getDamage2(),
    getDamage3(),
    getDamage4(),
};

STATE_DEFINE(dAcArrow_c, Wait);
STATE_DEFINE(dAcArrow_c, Move);
STATE_DEFINE(dAcArrow_c, ActorStop);
STATE_DEFINE(dAcArrow_c, BgStop);
STATE_DEFINE(dAcArrow_c, Bound);

// clang-format off

cCcD_SrcGObj dAcArrow_c::sCcSrcInf = {
    {AT_TYPE_ARROW, 0x8BB, {0x15,0, 0}, sDamageArrMaybe[0], 0, 0, 0, 0, 0}, 
    {
        ~(AT_TYPE_BUGNET | AT_TYPE_BEETLE | AT_TYPE_GLITTERING_SPORES | AT_TYPE_0x8000 | AT_TYPE_ARROW | AT_TYPE_0x100 | AT_TYPE_WIND | AT_TYPE_SLINGSHOT),
             0x210,  {0, 0, 0x407}, 0, 0
            }, 
    {0},
};

dCcD_SrcCps dAcArrow_c::sCc1 = {
    dAcArrow_c::sCcSrcInf,
    {5.0f},
};

const dCcD_SrcSph dAcArrow_c::sCc2 = {
    {{0x0, 0x0, 0x0, 0, 0, 0, 0, 0, 0}, 
    {AT_TYPE_SWORD, 0x211, {0, 0, 0x407}, 0, 0}, 
    {0xC28}},
    {100.0f},
};

// clang-format on

bool dAcArrow_atHitCallback(dAcObjBase_c *i_actorA, cCcD_Obj *i_objInfA, dAcObjBase_c *i_actorB, cCcD_Obj *i_objInfB) {
    static_cast<dAcArrow_c *>(i_actorA)->hitCallback(i_objInfA, i_actorB, i_objInfB);
    return true;
}

bool dAcArrow_c::createHeap() {
    mResFile = nw4r::g3d::ResFile(getOarcResFile("Alink"));
    nw4r::g3d::ResMdl mdl(nullptr);
    if ((isSlingshotProjectile()) != 0) {
        mdl = mResFile.GetResMdl("EquipPachinkoBullet");
    } else {
        mdl = mResFile.GetResMdl("EquipArrow");
    }
    // Looks weird; the more likely explanation here is that they did an `if (!create) return false;`
    // forgetting the `return true;` case, but this matches without UB so let's keep it that way?
    bool ok = mModel.create(mdl, &mAllocator, 0x120);
    if (!ok) {
        ok = false;
    }
    return ok;
}

int dAcArrow_c::create() {
    mArrowType = mParams & 0xFF;
    CREATE_ALLOCATOR(dAcArrow_c);

    setArrowFlag(ARROW_INITIAL_SPAWN);
    mStts.SetRank(2);
    mCcCps.Set(sCc1);
    mCcCps.SetStts(mStts);
    mCcCps.OnAt_0x2000();
    mCcCps.SetAtCallback(dAcArrow_atHitCallback);
    mCcSph.Set(sCc2);
    mCcSph.SetStts(mStts);

    if (mArrowType == 17) {
        mStateMgr.changeState(StateID_Move);
    } else {
        mStateMgr.changeState(StateID_Wait);
    }

    mBoundingBox.Set(mVec3_c(-6.0f, -6.0f, 0.0f), mVec3_c(6.0f, 6.0f, 110.0f));
    mCounterIdx = -1;
    field_0x68A = 300;
    field_0x688 = 50.0f - cM::rndF(20.0f);
    field_0x6A8 = 80.0f;
    mDespawnTimer = 10;
    updateRoomId();
    return SUCCEEDED;
}

dAcArrow_c::~dAcArrow_c() {}

void dAcArrow_c::hitCallback(cCcD_Obj *i_objInfA, dAcObjBase_c *i_actorB, cCcD_Obj *i_objInfB) {
    dJntCol_c *col = i_actorB->getLinkage().getJntCol();
    if (col != nullptr) {
        mBoundJntIdx = col->getArrowOffsetPosAndAngle(
            &i_objInfA->GetAtHitPos(), &mRotation, &field_0x6B0, &field_0x6BC, i_objInfB->ChkTg_0x4C(0x2000)
        );

        if (mBoundJntIdx >= 0) {
            mRef1.link(i_actorB);
        } else {
            mRef1.unlink();
        }
    }
}

bool dAcArrow_c::checkWaterHit(const mVec3_c &v1, mVec3_c &v2, bool noEffect) {
    if (dBgS_WtrLinChk::SetIsWater(&v1, &v2, this)) {
        const mVec3_c &end = dBgS_WtrLinChk::GetInstance().GetLinEnd();
        if (isSlingshotProjectile()) {
            v2 = end;
        } else {
            mVec3_c t1 = v2 - end;
            f32 sc;
            f32 dist = v2.distance(v1);
            if (dist > 0.0001f) {
                sc = t1.mag() / dist;
            } else {
                sc = 1.0f;
            }
            t1.normalize();
            v2 = end + t1 * field_0x6A8 * sc;
        }

        if (!noEffect) {
            dAcPy_c::fn_801E2FC0(end, dBgS_WtrLinChk::GetInstance(), 0.5f);
            startSound(SE_AW_HIT_WATER);
        }
        return true;
    }

    return false;
}

void dAcArrow_c::fn_8025DED0(f32 f) {
    f32 f1;
    dAcPy_c *link = dAcPy_c::GetLink2();
    if (isSlingshotProjectile()) {
        f1 = 12.0f;
    } else {
        f1 = 5.0f;
        f32 f2 = link->fn_802097C0();
        if (f > f2) {
            f32 f3 = link->fn_802097D0();
            if (f3 <= f2) {
                f1 += link->fn_802097B0();
            } else {
                f32 f4 = (f - f2) / (f3 - f2);
                if (f4 > 1.0f) {
                    f4 = 1.0f;
                }
                f1 += f4 * link->fn_802097B0();
            }
        }
    }

    mVec3_c next = mVec3_c(mPosition + mVelocity * (field_0x6A0 + 1.0f));
    mCcCps.OnAtSet();

    if (!checkArrowFlag(ARROW_WATER_HIT)) {
        if (!isSlingshotProjectile()) {
            holdSound(SE_AW_FLY_LV);
        }
        checkWaterHit(mPosition, next, true);
    } else {
        f32 diff = next.y - field_0x6D4.y;
        if (diff < -300.0f) {
            diff = -300.0f - diff;
            f32 diff2 = next.y - mPosition.y;
            diff2 = nw4r::math::FAbs(diff2);
            if (diff2 > diff) {
                next = mPosition + mVelocity * ((field_0x6A0 + 1.0f) * ((diff2 - diff) / diff2));
            }
        }
    }

    sArrowLinChk.Set(&mPosition, &next, this);
    if (dBgS::GetInstance()->LineCross(&sArrowLinChk)) {
        next = sArrowLinChk.GetLinEnd();
    }
    mCcCps.Set(mPosition, next, f1);
    mCcCps.unknownCalc();
    dCcS::GetInstance()->Set(&mCcCps);
    dCcS::GetInstance()->GetMassMng().SetObj(&mCcCps, 1);
}

void dAcArrow_c::fn_8025E160() {
    // NONMATCHING
    dAcPy_c *link = dAcPy_c::GetLink2();
    if (isSlingshotProjectile()) {
        field_0x6A0 = 0.0f;
        if (mArrowType != 17) {
            setRotXY(mAngle, -link->getField_0x1268(), link->getRotation().y + link->getField_0x126A());
        } else {
            mAngle.y = mRotation.y;
            mAngle.x = -mRotation.x;
        }
        link->fn_80209700(field_0x6A4, field_0x69C, false);
    } else {
        link->fn_80209700(field_0x6A4, field_0x69C, false);
        if (checkArrowFlag(ARROW_FULLY_CHARGED)) {
            const mVec3_c *v = link->fn_802097E0(mRotation.x, mRotation.y);
            if (v != nullptr) {
                mPosition = *v;
                mAngle.x = -mRotation.x;
                mAngle.y = mRotation.y;
            }
        }
        field_0x683 = 30;
        field_0x6A0 = 90.0f / field_0x69C;
        startSound(SE_AW_FLY);
    }

    if (checkArrowFlag(ARROW_0x40)) {
        unsetArrowFlag(ARROW_0x40);
        mVec3_c tmp = field_0x6EC - mPosition;
        mAngle.x = tmp.atan2sY_XZ() + field_0x6AC * cM::rndFX(400.0f);
        mAngle.y = tmp.atan2sX_Z() + field_0x6AC * cM::rndFX(400.0f);
        mRotation.x = -mAngle.x;
        mRotation.y = mAngle.y;
    }

    // Spawn additional projectiles for Mighty Scattershot
    if (isSlingshotProjectile() && checkArrowFlag(ARROW_FULLY_CHARGED)) {
        mVec3_c tmp = mVec3_c::Ez;
        tmp.rotX(mAngle.x);
        tmp.rotY(mAngle.y);

        mAng3_c ang(0, 0, 0);

        for (int i = 0; i < 8; i++) {
            mVec3_c tmp2 = mVec3_c::Ez;
            // TODO eliminate temps?
            mAng a1 = link->fn_80208CA0();
            mAng a2 = mAng::fromDeg(cM::rndF(2.5f));
            tmp2.rotX(mAngle.x + a1 - a2);
            tmp2.rotY(mAngle.y);

            mMtx_c mtx;
            mtx.setAxisRotation(tmp, i * mAng::s2r_c(0x2000) + mAng::d2r_c(cM::rndFX(2.5f)));
            MTXMultVecSR(mtx, tmp2, tmp2);
            ang.x = tmp2.atan2sY_XZ();
            ang.y = tmp2.atan2sX_Z();
            dAcObjBase_c::create(fProfile::ARROW, -1, 17, &mPosition, &ang, 0, -1);
        }
    }

    mSpeed = 100.0f;
    field_0x6C8 = mPosition;
    mVelocity.fromXY(mAngle.x, mAngle.y, field_0x69C);
    if (!isSlingshotProjectile()) {
        mCounterIdx = sCounter;
        sCounter++;
        if (sCounter == 6) {
            sCounter = 0;
        }
    }
    fn_8025DED0(0.0f);
}

void dAcArrow_c::updateRoomId() {
    if (dBgS_ObjGndChk::CheckPos(mPosition)) {
        mRoomID = dBgS_ObjGndChk::GetRoomID();
    } else if (mRoomID == -1) {
        mRoomID = dAcPy_c::GetLink2()->getRoomId();
    }
}

s16 dAcArrow_c::fn_8025E640() {
    if (sLib::calcTimer(&field_0x688)) {
        field_0x690 += (s16)(21243.0f - cM::rndF(0x1000));
        f32 a = field_0x688 * 0.02f;
        return a * 0x400 * a * field_0x690.sin();
    } else {
        return 0;
    }
}

void dAcArrow_c::fn_8025E720(dAcObjBase_c *obj, const mVec3_c &v) {
    mRef1.link(obj);
    mMtx_c mtx;
    obj->mWorldMtx.inverseTo(mtx);
    MTXMultVec(mtx, v, field_0x6B0);
    mVec3_c v1 = mVec3_c::createProjectionXZ(mRotation, 1.0f);
    v1.y *= -1.0f;
    MTXMultVecSR(mtx, v1, field_0x6BC);
}

void dAcArrow_c::updateMtx() {
    mWorldMtx.transS(mPosition);
    mWorldMtx.ZXYrotM(mRotation);
    if (mStateMgr.isState(StateID_BgStop)) {
        mMtx_c mtx;
        mtx.transS(0.0f, 0.0f, -90.0f);
        MTXConcat(mWorldMtx, mtx, mWorldMtx);
    } else if (mStateMgr.isState(StateID_Bound)) {
        mMtx_c mtx;
        mtx.transS(0.0f, 0.0f, -45.0f);
        MTXConcat(mWorldMtx, mtx, mWorldMtx);
    }
    mModel.setLocalMtx(mWorldMtx);
}

void dAcArrow_c::setInitialPosition() {
    dAcPy_c::GetLink2()->getWeaponLMtx(mWorldMtx);
    mModel.setLocalMtx(mWorldMtx);
    mWorldMtx.getTranslation(mPosition);
    mWorldMtx.toRot(mRotation);
    mAngle.y = mRotation.y;
    mAngle.x = -mRotation.x;
}

dAcObjBase_c *dAcArrow_c::fn_8025E960() {
    s16 a1 = fn_8025E640();
    dAcObjBase_c *o1 = mRef1.get();
    if (o1 == nullptr || o1->checkObjectProperty(OBJ_PROP_0x100)) {
        return nullptr;
    } else if (checkArrowFlag(ARROW_0x2000)) {
        if (checkArrowFlag(ARROW_0x10000)) {
            mWorldMtx.getTranslation(mPosition);
            mWorldMtx.toRot(mRotation);
            mMtx_c mtx;
            mtx.transS(0.0f, 0.0f, -90.0f);
            MTXConcat(mWorldMtx, mtx, mWorldMtx);
            mModel.setLocalMtx(mWorldMtx);
        }
        return o1;
    } else {
        dJntCol_c *col = o1->getLinkage().getJntCol();
        if (col != nullptr) {
            col->setArrowPosAndAngle(&field_0x6B0, &field_0x6BC, mBoundJntIdx, &mPosition, &mRotation);
        } else {
            MTXMultVec(o1->mWorldMtx, field_0x6B0, mPosition);
            mVec3_c tmp;
            MTXMultVecSR(o1->mWorldMtx, field_0x6BC, tmp);
            mRotation.x = tmp.atan2snY_XZ();
            mRotation.y = tmp.atan2sX_Z();
        }
        mWorldMtx.transS(mPosition);
        mWorldMtx.ZXYrotM(mRotation.x + a1, mRotation.y, mRotation.z);
        mMtx_c mtx;
        mtx.transS(0.0f, 0.0f, -90.0f);
        MTXConcat(mWorldMtx, mtx, mWorldMtx);
        mModel.setLocalMtx(mWorldMtx);
        return o1;
    }
}

void dAcArrow_c::initPickupCc() {
    mCcSph.SetR(25.0f);
    mCcSph.ClrTgSet();
    mCcSph.OnCoSet();
}

bool dAcArrow_c::checkPickup() {
    if (dAcPy_c::getCurrentBowType() != 0) {
        if (mCcSph.ChkCoHit()) {
            dAcItem_c::giveItem(ITEM_SINGLE_ARROW, 0, -1);
            setArrowFlag(ARROW_SHOULD_DESPAWN);
            return true;
        }
        mCcSph.SetC(mPosition);
        dCcS::GetInstance()->Set(&mCcSph);
    }

    return false;
}

void dAcArrow_c::initializeState_Wait() {
    setInitialPosition();
}

void dAcArrow_c::executeState_Wait() {
    setInitialPosition();
    if (checkArrowFlag(ARROW_0x800)) {
        mStateMgr.changeState(StateID_Move);
    } else if (isSlingshotProjectile()) {
        mHiddenTimer = 1;
    }
}

void dAcArrow_c::finalizeState_Wait() {}

void dAcArrow_c::initializeState_Move() {
    setArrowFlag(ARROW_0x1000);
    setActorProperty(AC_PROP_0x4);
    if (isSlingshotProjectile()) {
        mCcCps.SetAtType(AT_TYPE_SLINGSHOT);
        mCcCps.SetAtInfo_0x0(20);
        mCcCps.SetAtDamage(0);
        mEffectId = PARTICLE_RESOURCE_ID_MAPPING_207_;
    } else {
        u8 dmg = sDamageArrMaybe[dAcPy_c::getCurrentBowType()];
        if (checkArrowFlag(ARROW_FULLY_CHARGED)) {
            mCcCps.SetAtFlagsUpper(0x2000000);
            mEffectId = PARTICLE_RESOURCE_ID_MAPPING_2_;
            dmg = (s32)dmg * 1.5f + 0.5f;
        } else {
            mEffectId = PARTICLE_RESOURCE_ID_MAPPING_1_;
        }
        mCcCps.SetAtDamage(dmg);
        mCcCps.OnAt_0x1000();
    }
    mCcCps.OnTgSet();
    fn_8025E160();
    updateMtx();
}

void dAcArrow_c::executeState_Move() {
    mPosition += mVelocity;
    mVec3_c next = mVec3_c(mPosition + mVelocity * field_0x6A0);
    if (!checkArrowFlag(ARROW_WATER_HIT)) {
        sArrowLinChk.Set(&mOldPosition, &next, this);
        mVec3_c t;
        if (dBgS::GetInstance()->LineCross(&sArrowLinChk)) {
            t = sArrowLinChk.GetLinEnd();
        } else {
            t = next;
        }

        if (checkWaterHit(mOldPosition, t, false)) {
            setArrowFlag(ARROW_WATER_HIT);
            unsetArrowFlag(ARROW_0x200);
            field_0x6D4 = dBgS_WtrLinChk::GetInstance().GetLinEnd();
            mVelocity.normalize();
            mPosition = t - mVelocity * 90.0f;
            mVelocity *= field_0x6A8;

            if (!isSlingshotProjectile()) {
                field_0x6A0 = 90.0f / field_0x6A8;
            }
            next = t;
        }
    }

    sArrowLinChk.Set(&mOldPosition, &next, this);
    bool isCross = dBgS::GetInstance()->LineCross(&sArrowLinChk);
    // TODO the constness here is probably a bit messed up
    const cCcD_Obj *obj = nullptr;
    s32 flags = -1;
    if (mCcCps.ChkTgHit()) {
        obj = &mCcCps;
    }

    if (obj != nullptr) {
        flags = obj->GetTgDamageFlags();
    }

    // TODO magic numbers
    if (flags == 8) {
        mPosition = obj->GetTgHitPos();
        const mVec3_c &hitDir = obj->GetTgAtHitDir();
        if (hitDir.squareMagXZ() < 1.0f) {
            // TODO constness
            mAngle.y = cLib::targetAngleY(mPosition, const_cast<cCcD_Obj *>(obj)->GetTgActor()->mPosition);
        } else {
            mAngle.y = hitDir.atan2snX_nZ();
        }

        mRotation.y = mAngle.y;
        setArrowFlag(ARROW_0x400);
        mStateMgr.changeState(StateID_Bound);
    } else if (obj != nullptr) {
        mPosition = obj->GetTgHitPos();
        mStateMgr.changeState(StateID_Bound);
        startSound(SE_AW_FLIP);
    } else {
        dAcObjBase_c *atActor;
        s32 what;

        if (mCcCps.ChkAtHit()) {
            if (mCcCps.GetAtActor()->mProfileName == fProfile::PUMPKIN) {
                field_0x683 = 0;
            }
            dLightEnv_c::GetPInstance()->setBPM8_Type6(&mCcCps.GetAtHitPos());
            if (isSlingshotProjectile()) {
                onSlingshotHit(mCcCps.GetAtHitPos(), 0);
                return;
            }
            atActor = mCcCps.GetAtActor();
            what = 1;
            if (atActor != nullptr && atActor->getLinkage().hasJntCol()) {
                bool b = false;
                if (atActor != mRef1.get()) {
                    mBoundJntIdx = atActor->getLinkage().getArrowOffsetPosAndAngle(
                        &mCcCps.GetAtHitPos(), &mRotation, &field_0x6B0, &field_0x6BC, mCcCps.GetAtFlag0x2()
                    );
                    b = true;
                }

                if (mBoundJntIdx >= 0) {
                    what = 4;
                    if (b) {
                        mRef1.link(atActor);
                    }
                } else if (mBoundJntIdx == -2 || mCcCps.GetAtFlag0x2()) {
                    what = 2;
                }
            } else {
                if (mCcCps.GetAtFlag0x2()) {
                    what = 2;
                } else if (mCcCps.ChkAtArrowStick()) {
                    fn_8025E720(atActor, mCcCps.GetAtHitPos());
                    what = 4;
                }
            }

            if (what == 1 && atActor != nullptr && atActor->getLinkage().checkFlag(0x1000) &&
                !isSlingshotProjectile()) {
                if (!mRef2.isLinked()) {
                    atActor->getLinkage().tryAttach(atActor, this, &mRef2, dLinkage_c::CONNECTION_8, false);
                }
                what = 0;
            }
        } else {
            what = 0;
        }

        switch (what) {
            default: {
                if (what == 2) {
                    if (checkArrowFlag(ARROW_WATER_HIT)) {
                        setArrowFlag(ARROW_SHOULD_DESPAWN);
                    } else {
                        if (mCcCps.ChkAtHit()) {
                            mPosition = mCcCps.GetAtHitPos();
                        }
                        mStateMgr.changeState(StateID_Bound);
                    }
                } else if (what == 4) {
                    mStateMgr.changeState(StateID_ActorStop);
                } else {
                    setArrowFlag(ARROW_SHOULD_DESPAWN);
                    if (!checkArrowFlag(ARROW_WATER_HIT) && mCcCps.ChkAtHit()) {
                        mPosition = mCcCps.GetAtHitPos();
                    }
                }
                break;
            }
            case 0:
            case 3: {
                if (isCross) {
                    mPosition = sArrowLinChk.GetLinEnd();
                    if (checkArrowFlag(ARROW_WATER_HIT) &&
                        (isSlingshotProjectile() || mPosition.y - field_0x6D4.y < -300.0f)) {
                        setArrowFlag(ARROW_SHOULD_DESPAWN);
                    } else {
                        s32 polyAtt0 = dBgS::GetInstance()->GetPolyAtt0(sArrowLinChk);
                        s32 polyAtt1 = dBgS::GetInstance()->GetPolyAtt1(sArrowLinChk);
                        s32 specialCode = dBgS::GetInstance()->GetSpecialCode(sArrowLinChk);
                        if (isSlingshotProjectile() || specialCode == 19 || specialCode == 20 || specialCode == 16 ||
                            specialCode == 10 || specialCode == 11 || polyAtt0 == 6 || polyAtt0 == POLY_ATT_0_ROCK ||
                            polyAtt0 == POLY_ATT_0_NONE || polyAtt0 == POLY_ATT_0_NUMA ||
                            polyAtt0 == POLY_ATT_0_STONE || polyAtt0 == POLY_ATT_0_METAL ||
                            (polyAtt0 == POLY_ATT_0_LIFE && (polyAtt1 == 2 || polyAtt1 == 3))) {
                            if (checkArrowFlag(ARROW_WATER_HIT)) {
                                setArrowFlag(ARROW_SHOULD_DESPAWN);
                            } else {
                                dAcPy_c::fn_801E2FC0(mPosition, sArrowLinChk, 0.5f);
                                if (isSlingshotProjectile()) {
                                    onSlingshotHit(mPosition, polyAtt0 == POLY_ATT_0_LAVA);
                                } else {
                                    startBgHitSound(SE_AW_HIT, sArrowLinChk, nullptr);
                                    if (polyAtt0 == POLY_ATT_0_LAVA || polyAtt0 == POLY_ATT_0_NUMA ||
                                        polyAtt0 == POLY_ATT_0_NONE || specialCode == 16 || specialCode == 10 ||
                                        specialCode == 11) {
                                        setArrowFlag(ARROW_SHOULD_DESPAWN);
                                    } else {
                                        mStateMgr.changeState(StateID_Bound);
                                    }
                                }
                            }
                        } else {
                            mStateMgr.changeState(StateID_BgStop);
                        }
                    }
                } else if (checkArrowFlag(ARROW_WATER_HIT)) {
                    if (isSlingshotProjectile()) {
                        mPosition = next;
                        setArrowFlag(ARROW_SHOULD_DESPAWN);
                    } else {
                        f32 diff = mPosition.y - field_0x6D4.y;
                        if (diff < -300.0f) {
                            mCcCps.ClrAtSet();
                            mCcCps.ClrAtHit();
                            if (sLib::chase(&mScale.x, 0.0f, 0.1f)) {
                                setArrowFlag(ARROW_SHOULD_DESPAWN);
                            } else {
                                mScale.y = mScale.z = mScale.x;
                                mModel.setScale(mScale);
                            }
                        } else if (diff > 90.0f) {
                            setArrowFlag(ARROW_SHOULD_DESPAWN);
                        } else {
                            fn_8025DED0(mPosition.distance(field_0x6C8));
                        }
                    }
                } else {
                    f32 diff = mPosition.distance(field_0x6C8);
                    if (diff > field_0x6A4 || mAcceleration < 0.0f) {
                        if (isSlingshotProjectile()) {
                            mAcceleration = -5.0f;
                        } else {
                            mAcceleration = -2.0f;
                        }
                        mVelocity.y += mAcceleration;
                        if (isSlingshotProjectile()) {
                            dAcPy_c *link = dAcPy_c::GetLink2();
                            f32 xz = mVelocity.absXZ();
                            f32 t1 = link->fn_80208CE0();
                            if (xz > t1) {
                                f32 orig = xz;
                                sLib::chase(&xz, t1, link->fn_80208CF0());
                                xz /= orig;
                                mVelocity.x *= xz;
                                mVelocity.z *= xz;
                            }
                        }
                        if (field_0x68A != 0) {
                            field_0x68A--;
                        } else {
                            setArrowFlag(ARROW_SHOULD_DESPAWN);
                            return;
                        }
                        mRotation.x = mVelocity.atan2snY_XZ();
                        mAngle.x = -mRotation.x;
                        if (!isSlingshotProjectile()) {
                            field_0x6A0 = 90.0f / mVelocity.mag();
                        }
                    }
                    fn_8025DED0(diff);
                }
                break;
            }
        }
    }
}

void dAcArrow_c::finalizeState_Move() {}

void dAcArrow_c::initializeState_Bound() {
    if (isSlingshotProjectile()) {
        onSlingshotHit(mPosition, false);
    } else {
        mSpeed = 0.0f;
        fn_802601C0();
        f32 rnd = cM::rndFX(0x2000);
        f32 ang = mAngle.y + 0x8000;
        mAng ay = ang + rnd;
        mAng ax;
        if (checkArrowFlag(ARROW_0x400)) {
            ax = 0x2000 - cM::rndF(0x1000);
        } else {
            ax = -cM::rndF(0x1000);
        }
        mVelocity.fromXY(ax, ay, 15.0f + cM::rndF(15.0f));
        field_0x690 = 0x2C00 - cM::rndF(0x1000);
        setArrowFlag(ARROW_0x100);
        unsetArrowFlag(ARROW_0x200);
        updateMtx();
    }
}

void dAcArrow_c::executeState_Bound() {
    mVelocity.y -= 2.0f;
    mPosition += mVelocity;
    mRotation.x += field_0x690;

    mVec3_c next = mPosition + mVelocity * field_0x6A0;

    sArrowLinChk.Set(&mOldPosition, &next, this);
    if (dBgS::GetInstance()->LineCross(&sArrowLinChk)) {
        cM3dGPla pla;
        dBgS::GetInstance()->GetTriPla(sArrowLinChk, &pla);
        f32 halfMag = mVelocity.mag() * 0.5f;

        VECReflect(mVelocity, pla.mNormal, mVelocity);
        mVelocity *= halfMag;
        field_0x690.mVal >>= 1;
        if (dBgS_CheckBGroundPoly(sArrowLinChk)) {
            setArrowFlag(ARROW_0x1);
        }
    } else if ((checkArrowFlag(ARROW_0x1) && mVelocity.y <= 0.0f) || field_0x6C8.y - 5000.0f > mPosition.y) {
        setArrowFlag(ARROW_SHOULD_DESPAWN);
    }

    updateMtx();
}

void dAcArrow_c::finalizeState_Bound() {}

void dAcArrow_c::initializeState_BgStop() {
    fn_802601C0();
    cM3dGPla pla;
    dBgS::GetInstance()->GetTriPla(sArrowLinChk, &pla);
    mVec3_c vz;
    mWorldMtx.getBase(2, vz);
    if (pla.mNormal.dot(vz) > mAng(19115).cos()) {
        mVec3_c c;
        vecCross(c, pla.mNormal, vz);
        if (nw4r::math::VEC3LenSq(c) > 0.1f) {
            mMtx_c mtx;
            mtx.setAxisRotation(c, mAng::s2r_c(19115));
            MTXMultVecSR(mtx.m, pla.mNormal, vz);
            mRotation.y = vz.atan2sX_Z();
            mRotation.x = vz.atan2snY_XZ();
        }
    }

    unsetArrowFlag(ARROW_0x200);
    setArrowFlag(ARROW_0x100);
    mSpeed = 0.0f;
    mAngle.x = -mRotation.x;
    field_0x68A = 300;
    updateMtx();
    mPolyInfo.SetPolyInfo(sArrowLinChk);
    startBgHitSound(SE_AW_STICK, sArrowLinChk, nullptr);
    // TODO - constness
    dBgS::GetInstance()->ArrowStickCallBack(sArrowLinChk, this, const_cast<mVec3_c &>(sArrowLinChk.GetLinEnd()));
    initPickupCc();
    dLightEnv_c::GetPInstance()->setBPM8_Type6(&mPosition);
    const dAcObjBase_c *ac = dBgS::GetInstance()->GetActorPointer(sArrowLinChk);
    if (ac != nullptr) {
        // TODO -  constness
        fn_8025E720(const_cast<dAcObjBase_c *>(ac), mPosition);
        setArrowFlag(ARROW_0x8000);
    }
}

void dAcArrow_c::executeState_BgStop() {
    checkPickup();
    dAcObjBase_c *ref = this;
    if (checkArrowFlag(ARROW_0x8000)) {
        ref = fn_8025E960();
    }

    if (checkArrowFlag(ARROW_OLD) || ref == nullptr ||
        ((!ref->checkObjectProperty(OBJ_PROP_0x10000) || !checkArrowFlag(ARROW_0x8000)) &&
         !dBgS::GetInstance()->ChkPolySafe(mPolyInfo))) {
        setArrowFlag(ARROW_SHOULD_DESPAWN);
    } else if (!checkArrowFlag(ARROW_0x8000)) {
        bool b = false;
        s16 s1 = fn_8025E640();
        if (field_0x688 != 0) {
            b = true;
        }
        if (dBgS::GetInstance()->ChkMoveBG(mPolyInfo, true)) {
            dBgS::GetInstance()->MoveBgTransPos(mPolyInfo, true, &mPosition, &mAngle, &mRotation);
            b = true;
        }

        if (b != 0) {
            mRotation.x = s1 - mAngle.x;
            updateMtx();
        }
    }
}

void dAcArrow_c::finalizeState_BgStop() {}

void dAcArrow_c::initializeState_ActorStop() {
    fn_802601C0();
    unsetArrowFlag(ARROW_0x200);
    mSpeed = 0.0f;
    dAcObjBase_c *obj = fn_8025E960();
    if (obj != nullptr) {
        getSoundSource()->startObjHitSound(SE_AW_STICK, obj->getSoundSource(), mPosition);
    }
    initPickupCc();
}

void dAcArrow_c::executeState_ActorStop() {
    dAcObjBase_c *obj = fn_8025E960();
    checkPickup();
    if (checkArrowFlag(ARROW_OLD) || obj == nullptr) {
        setArrowFlag(ARROW_SHOULD_DESPAWN);
    }
    unsetArrowFlag(ARROW_0x10000);
}

void dAcArrow_c::finalizeState_ActorStop() {
    unsetArrowFlag(ARROW_0x10000);
}

void dAcArrow_c::onSlingshotHit(const mVec3_c &v, bool b) {
    if (b) {
        startSound(SE_PC_HIT_LAVA);
    } else {
        startSound(SE_PC_HIT);
    }
    setArrowFlag(ARROW_SHOULD_DESPAWN);
    setArrowFlag(ARROW_0x100);
    dJEffManager_c::spawnEffect(PARTICLE_RESOURCE_ID_MAPPING_538_, v, nullptr, nullptr, nullptr, nullptr, 0, 0);
}

void dAcArrow_c::fn_802600D0(bool b) {
    if (b) {
        mWorldMtx.getTranslation(mEffectTranslation);
        mEffectRotation.set(mAng(0), mRotation.y, mEffectRotationZ.mVal);
        if (mEffectId == PARTICLE_RESOURCE_ID_MAPPING_2_) {
            mEffectRotationZ += 0x888;
        }
        mMtx_c mtx;
        mtx.transS(mEffectTranslation);
        mtx.ZXYrotM(mEffectRotation);
        mEffects.holdEffect(mEffectId, mtx, nullptr, nullptr);
    } else {
        mEffects.setFading(5);
        unsetArrowFlag(ARROW_0x1000);
    }
    unsetArrowFlag(ARROW_0x100);
}

void dAcArrow_c::fn_802601C0() {
    if (field_0x683 != 0) {
        if (!(mCcCps.ChkAtHit() && mCcCps.GetAtActor()->mProfileName == fProfile::PUMPKIN)) {
            setArrowFlag(ARROW_0x4000);
        }
        field_0x683 = 0;
    }
}

bool dAcArrow_c::fn_80260250(dAcObjBase_c *o1, dAcObjBase_c *o2) {
    dAcObjBase_c *o = mRef2.get();
    if (o == nullptr || o1 == o) {
        o1->getLinkage().forceRemove(o1);
        o2->getLinkage().tryAttach(o2, this, &mRef2, dLinkage_c::CONNECTION_8, false);
        return true;
    } else {
        return false;
    }
}

int dAcArrow_c::actorExecute() {
    unsetArrowFlag(ARROW_0x4000);
    if (field_0x683 != 0) {
        field_0x683--;
        if (field_0x683 == 0) {
            setArrowFlag(ARROW_0x4000);
        }
    }

    if (checkArrowFlag(ARROW_SHOULD_DESPAWN)) {
        // TODO - works but maybe temps or inlines?
        if (mRef2.isLinked()) {
            mRef2.get()->getLinkage().forceRemove(mRef2.get());
        }
        fn_802601C0();
        if (mDespawnTimer != 0) {
            mDespawnTimer--;
        } else {
            deleteRequest();
            return SUCCEEDED;
        }
    } else {
        if (mCounterIdx == sCounter) {
            setArrowFlag(ARROW_OLD);
        }
        if (mHiddenTimer != 0) {
            mHiddenTimer--;
        }
        mStateMgr.executeState();
        if (mStateMgr.isState(StateID_Move)) {
            updateMtx();
        }
    }

    mPositionCopy2 = mPositionCopy3 = mPosition;
    if (checkArrowFlag(ARROW_0x1000)) {
        fn_802600D0(
            (mStateMgr.isState(StateID_Move) && !checkArrowFlag(ARROW_SHOULD_DESPAWN)) || checkArrowFlag(ARROW_0x100)
        );
    }
    updateRoomId();
    dAcObjBase_c *obj = mRef2.get();
    if (obj != nullptr) {
        static const mVec3_c v(0.0f, 0.0f, 45.0f);
        mVec3_c v2;
        MTXMultVec(mWorldMtx, v, v2);
        obj->mPosition = v2;
        obj->mPosition.y -= obj->getLinkage().getField_0x1C();
    }

    return SUCCEEDED;
}

int dAcArrow_c::draw() {
    if (mHiddenTimer != 0) {
        return SUCCEEDED;
    }
    if (checkArrowFlag(ARROW_SHOULD_DESPAWN)) {
        return SUCCEEDED;
    }
    if (mStateMgr.isState(StateID_Wait) && checkArrowFlag(ARROW_INITIAL_SPAWN)) {
        setInitialPosition();
        unsetArrowFlag(ARROW_INITIAL_SPAWN);
    }
    drawModelType1(&mModel);
    return SUCCEEDED;
}
