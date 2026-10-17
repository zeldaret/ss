#include "d/a/d_a_pumpkin.h"

//#include "c/c_lib.h"
#include "d/a/d_a_player.h"
#include "d/col/bg/d_bg_s_acch.h"
#include "d/col/cc/d_cc_s.h"
#include "d/snd/d_snd_wzsound.h"
#include "d/t/d_t_stream.h"
#include "egg/math/eggMatrix.h"
#include "egg/math/eggQuat.h"
#include "m/m3d/m_scnleaf.h"
#include "m/m3d/m_shadow.h"
#include "m/m3d/m_smdl.h"
#include "nw4r/math/math_arithmetic.h"
#include "toBeSorted/attention.h"
#include "toBeSorted/event_manager.h"
#include "d/d_pad.h"


SPECIAL_ACTOR_PROFILE(PUMPKIN, dAcPumpkin_c, fProfile::PUMPKIN, 0x134, 0, 3);

STATE_DEFINE(dAcPumpkin_c, Stick);
STATE_DEFINE(dAcPumpkin_c, Wait);
STATE_DEFINE(dAcPumpkin_c, CarryGrab);
STATE_DEFINE(dAcPumpkin_c, StickSword);
STATE_DEFINE(dAcPumpkin_c, Throw);
STATE_DEFINE(dAcPumpkin_c, Roll);
STATE_DEFINE(dAcPumpkin_c, CarryBeetle);
STATE_DEFINE(dAcPumpkin_c, CarryWhip);



void fn_67_26E0(u32 param1, u32 param2, mVec3_c position, u8 param4); // temp

void dAcPumpkin_c::initializeState_Stick() {
    mSph.mAt.mSrc.mSPrm = mSph.mAt.mSrc.mSPrm & 0xfffffffe;
}

void dAcPumpkin_c::executeState_Stick() {
    fn_19_31B0();


    if (mLinkage.checkConnection(dLinkage_c::CONNECTION_6)) {
        startSound(SE_Pumpkin_PULLOUT);
        mStateMgr.changeState(StateID_StickSword); // figure out which state this is later
        return;
    }

    // temp = tempLinkage.checkConnection(dLinkage_c::CONNECTION_5);
    if (mLinkage.checkConnection(dLinkage_c::CONNECTION_5)) {
        startSound(SE_Pumpkin_PULLOUT);
        mStateMgr.changeState(StateID_Stick); // figure out which state this is later
        return;
    }

    if (mLinkage.checkConnection(dLinkage_c::CONNECTION_7)) {
        startSound(SE_Pumpkin_PULLOUT);
        mStateMgr.changeState(StateID_Roll); // figure out which state this is later
        return;
    }

    if (mLinkage.getState() == 1) {
        startSound(SE_Pumpkin_PULLOUT);
        field_0xa15 = 0;
        mStateMgr.changeState(StateID_Throw); // figure out which state this is later
        return;
    }
    if (mSph.ChkTgHit() && field_0xa17 != 0) {
        fn_19_2C20();
    }
}

void dAcPumpkin_c::finalizeState_Stick() {
    mViewClipIdx = -1;
}
void dAcPumpkin_c::initializeState_Wait() {
    field_0x908 = 0; // use set/unsetActorProperty for this
    mActorProperties = mActorProperties | 1;
    fn_19_2FC0(10);
    mSph.mAt.mSrc.mSPrm = mSph.mAt.mSrc.mSPrm & 0xfffffffe;
}
void dAcPumpkin_c::executeState_Wait() {
    u32 returnVal = 0;
    if (mLinkage.checkConnection(dLinkage_c::CONNECTION_6)) {
        mStateMgr.changeState(StateID_StickSword);
    }
    else if (mLinkage.checkConnection(dLinkage_c::CONNECTION_5)) {
        mStateMgr.changeState(StateID_Stick);
    }
    else if (mLinkage.checkConnection(dLinkage_c::CONNECTION_1)) {
        // startSound(0x9f4);
        int temp2 = 0;
        if (mObjAcch.mFlags & 0x800 && mPosition.y > mObjAcch.GetWtrGroundH()) {
            temp2 = 1;
        }
        field_0xa15 = temp2;
        mStateMgr.changeState(StateID_Roll);
        //return;
    }

    else if (mLinkage.checkConnection(dLinkage_c::CONNECTION_7)) {
        mStateMgr.changeState(StateID_Wait);
    }

    else if (mSph.ChkTgHit()) {
        if(field_0xa17 != 0) {
            fn_19_2C20();
        }
        //return;
    }
    // bool temp;
    // temp = mObjAcch.mFlags & 0x800 && mPosition.y < mObjAcch.mWtr.mGroundH;
    //u32 returnVal = 0;
    else if (mObjAcch.ChkWaterHit() && mObjAcch.GetWtrGroundH() > mPosition.y) {//weird if stuff
        returnVal = 1;
        if (mObjAcch.ChkGroundLanding() && mPosition.y + 50 < mObjAcch.GetWtrGroundH()) {
            fn_19_2C20();
            return;
        }
        mVec3_c zero = mVec3_c::Zero;
        if (dTgStream_c::getForce(mPosition, zero) == 0) {
            return;
        }
        // if (angleToRotate(mVec3_c::Zero)) {
        mAngle.y = cM::atan2s(mPosition.x, mPosition.z);
        sLib::addCalc(
            &(mSpeed), EGG::Math<float>::sqrt(mPosition.x * mPosition.x + mPosition.z * mPosition.z), 0.02, 1.0, 0.05
        );
        //}
        // mSpeed = mSpeed * 0.93;
        // u32 temp5 = field0x9b8 << 0x10;
        // s32 temp11 = temp5 / 0x32 + temp5 >> 0x1f;
        // float temp15 = nw4r::math::SinFIdx(temp11 - (temp11 >> 0x1f) * 1.0f * 0.00390625);
        // 264-288: integer divide field by 10, store as s16 on stack
        s32 angleIdx = (field_0x9b8 / 50);

        // 28c-294: load as float, scale by constant
        // f32 scaled = 1.0f * angleIdx;

        // 298: compute sine in index units
        f32 sinResult = nw4r::math::SinFIdx(angleIdx * 0.00390625f);
        // float temp15 = nw4r::math::SinFIdx(tempf1);
        mSpeed *= 0.93f; // fix this, completely wrong, just temp
        mVelocity.y = (mSpeed * 0.8 + (mObjAcch.mWtr.mGroundH - mPosition.y) * fabs(mObjAcch.mWtr.mGroundH)) /
                      (sinResult * 3.0 + 20.0);
        if (mVelocity.y > 20.0) {
            mVelocity.y = 20.0;
        }
        if (mVelocity.y < -20.0) {
            mVelocity.y = -20.0;
        }
        if (field_0x9db == 0) {
            //field_0x90C = cM::rndInt(0x200) + 0x800;
            field_0x912 = cM::rndInt(0x100) + 0xa00;
            startSound(0x9a0);
            mSph.mAt.mSrc.mSPrm = (mSph.mAt.mSrc.mSPrm & 0xfffffffe);
        }
        fn_19_31B0();
        field_0x908 = 0;
        sLib::addCalcAngle((short *)&(mRotation.x), 0, 5, 0x100);
        sLib::addCalcAngle((short *)&(mRotation.z), 0, 5, 0x100);

    } else {
        if (mObjAcch.ChkGndHit()) {
            field_0x912 = 0;
            if (field_0x908 < 150.0f) {
                dJEffManager_c::spawnGroundEffect(mPosition, mPolyAttr0, mPolyAttr1, field_0x1B4, 1, 1.0f, field_0x1B0);
                fn_19_2C20();
            } else {
                fn_19_31B0();
            }
        }

        mSpeed *= 0.93f;
        if (mObjAcch.mFlags & 0x80) {
            startSound(SE_Pumpkin_PUT);
            if (!field_0xA16 && fabsf(field_0x908) > 10.0f) {
                dJEffManager_c::spawnGroundEffect(mPosition, mPolyAttr0, mPolyAttr1, field_0x1B4, 1, 1.0f, field_0x1B0);
            }
            field_0xA16 = 0;
            mSph.mAt.mSrc.mSPrm = (mSph.mAt.mSrc.mSPrm & 0xfffffffe);
        }
        field_0x908 = 15.0f;
    }
    // if(field_0x908 >= -150.0) {
    //     startSound(SE_Pumpkin_PUT);
    //         dJEffManager_c::spawnGroundEffect(mPosition, mPolyAttr0, mPolyAttr1, field_0x1B4, 1, 1.0f, field_0x1B0);

    // }
    fn_19_2FD0();
    field_0x9d0 = returnVal;
}
void dAcPumpkin_c::finalizeState_Wait() {}
void dAcPumpkin_c::initializeState_CarryGrab() {
    fn_19_30C0();
    fn_19_2FC0(4);
    mVelocity = mVec3_c::Zero;
    mSpeed = 0;
    mSph.mCo.mSrc.mSPrm = mSph.mCo.mSrc.mSPrm | 0x400;
    mSph.mAt.mSrc.mSPrm = mSph.mAt.mSrc.mSPrm & 0xfffffffe;
    unsetActorProperty(AC_PROP_0x1);
    field_0x99c = 0;
    field_0xa14 = 0;
    field_0xA16 = 0;
}

static volatile u8 lbl_19_data_70[] = { // temp
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x0c, 0x00, 0x00, 0x00, 0x00, 0x20, 0x00, 0x00, 0x01, 0x20
};

void dAcPumpkin_c::executeState_CarryGrab() {

    int iVar6;
    const dAcPy_c *LINK = dAcPy_c::GetLink();

    fn_19_2FD0();

    u32 tempBool = mSph.mTg.MskRPrm(1) && mSph.mTg.GetActor();
    if(tempBool && field_0xa17) {
        fn_19_2C20();
    }

    iVar6 = dBgS_WtrChk::CheckPos(&mPosition, 1, 500.0, -500.0);
    if (field_0xa15 == 0 && iVar6 && field_0xa14 == 0) {
        if (mPosition.y < dBgS_WtrChk::GetWaterHeight()) {
            startSound(SE_O_FALL_WATER_M);
            field_0xa14 = 1;
        }
    } else if (dBgS_WtrChk::GetWaterHeight() > mPosition.y) {
        field_0xa15 = 0;
    }

    if (LINK != nullptr && LINK->checkFlags0x350(0x400000)) {
        field_0xA16 = 1;
    }

    s32 state  = mLinkage.getState();
    if(state == 1) {
        return;
    }
    field_0x99c = 0;
    field_0x99e = lbl_19_data_70[0]; // TODO fix
    switch (state) {
    case 2:
        field_0x9bc = 0;
        mSph.mAt.mSrc.mSPrm = mSph.mAt.mSrc.mSPrm | 0x1;
        mStateMgr.changeState(StateID_Throw);
        break;
    case 3:
        if (LINK != nullptr) {
            field_0x9a0 = mRotation.y;
        }
        field_0x99c = 1;
        field_0x9a8 = 1.2f;
        field_0x9a4 = 1.2f;
        mStateMgr.changeState(StateID_Roll); 
        break;
        
    default:
        if (iVar6 != 0) {
            field_0x9d0 = 1;
        }
        mStateMgr.changeState(StateID_Wait); 
        break;
    }

    return;
}
void dAcPumpkin_c::finalizeState_CarryGrab() {
    mSph.mCo.mSrc.mSPrm = mSph.mCo.mSrc.mSPrm & 0xfffffbff;
}
void dAcPumpkin_c::initializeState_StickSword() {
    fn_19_30C0();
    
    mVelocity = mVec3_c::Zero;
    //mSph.mCo.mSrc.mSPrm |=  0x400;
    mSph.mCo.OnSPrm(0x400);
    //mSph.mAt.mSrc.mSPrm &=  0xfffffffe;
    mSph.mAt.OffSPrm(1);
    field_0x964 = 0;
    field_0x914[0] = 0;
    field_0x914[1] = 0;
    field_0x914[2] = 0;
    field_0x914[3] = 0;
    field_0x914[4] = 0;
    field_0x914[5] = 0;
    field_0x914[6] = 0;
    field_0x914[7] = 0;
    field_0x914[8] = 0;
    field_0x914[9] = 0;
    field_0x914[10] = 0;
    field_0x914[11] = 0;
    field_0x914[12] = 0;
    field_0x914[13] = 0;
    field_0x914[14] = 0;
    field_0x914[15] = 0;
    field_0x914[16] = 0;
    field_0x914[17] = 0;
    field_0x914[18] = 0;
    field_0x914[19] = 0;
    fn_19_2FC0(1);
    field_0x99c = 0;
    //field_0x914[20] = 0;
    //field_0x914[21] = 0;
    
}
void dAcPumpkin_c::executeState_StickSword() {
    fn_19_2FD0();
    
    mVec3_c tempVec = dPad::ex_c::m_current_ex->mMPLSVelocity;
    field_0x964++;
    if(field_0x964 == 0x14) {
        field_0x964 = 0;
    }
    f32 tempFloat = 0;
    if(tempVec.squareMagXY() <= 0.0f) {
        tempFloat = nw4r::math::FrSqrt(tempVec.squareMagXY()) * tempVec.squareMagXY();
    }
    field_0x914[0] = tempFloat;
    if(mLinkage.getState() != 1) {
        if(mSpeed > 1.0f) {
            mSph.mAt.mSrc.mSPrm = mSph.mAt.mSrc.mSPrm | 1;
        }
        f32 tempFloat2 = 0;
        u32 counter = 2;
        while(counter != 0) {
            u32 counter2 = 0;
            while(counter2 < 10) {
                if(tempFloat2 < field_0x914[counter2]) {
                    tempFloat2 = field_0x914[counter2];
                }
                counter2++;
            }
            counter--;
        }
        mAcceleration = 0xC0000000;
        mVelocity.y = tempFloat2 / 5.0f * 20.0f;
        mSpeed = tempFloat2 / 5.0f * 60.0f;
        if(dAcPy_c::GetLink() == nullptr  || dAcPy_c::GetLink()->isAttackingDown()) {
            field_0x9bc = 0;
        }
        else {
            field_0x9bc = 1;
        }
        mStateMgr.changeState(StateID_Throw); 
    }
}
void dAcPumpkin_c::finalizeState_StickSword() {
    mSph.mCo.mSrc.mSPrm = mSph.mCo.mSrc.mSPrm & 0xfffffbff;
}
void dAcPumpkin_c::initializeState_Throw() {
    mActorProperties = mActorProperties & 0xfffffffe;
}
void dAcPumpkin_c::executeState_Throw() {
    if ((mSph.mTg.mRPrm & 1 && mSph.mTg.GetActor()) || field_0xa17 == 0) {
        if ((mSph.mTg.mRPrm & 1 && mSph.mTg.GetActor()) || field_0xa17 == 0) {
            if ((mSph.mTg.mRPrm & 1 && mSph.mTg.GetActor()) || field_0xa17 == 0) {
                fn_19_2C20();
                if((mObjAcch.mFlags & 0x80) || mObjAcch.ChkWallHit(nullptr)) {
                    dJEffManager_c::spawnGroundEffect(mPosition, mPolyAttr0, mPolyAttr1, field_0x1B4, 0, 1.0f, field_0x1B0);
                    fn_19_2C20();
                    return;
                }
                if (mObjAcch.mFlags & 0x800 && mPosition.y > mObjAcch.GetWtrGroundH()) {
                    field_0x9d0 = 0;
                    mStateMgr.changeState(StateID_Wait); 
                }
                
                if(field_0x9bc == 0) {
                    f32 vel_mag = VEC3Len(mVelocity);
                    f32 f1 = mAng(vel_mag * (field_0x9a4 + 200.f)).radian();
                    f32 f2 = mAng(vel_mag * 182.0f * 0.2f).radian();
                    f32 f0 = mAng(mAngle.y - mRotation.y).radian();//spinangle
                    
                    f32 hSpeed = mVelocity.squareMagXZ();

                    mQuat_c q0, q1, q2, q3;//
                    q1.setAxisRotation(mVec3_c::Ey, f0);
                    q0.setAxisRotation(mVec3_c::Ey, -f0);   
                    q3.setAxisRotation(mVec3_c::Ex, hSpeed * f1);
                    //q2.setAxisRotation(mVec3_c::Ey, f2);
                    f32 fVar14 = mPosition.squareMagXZ();
                    // uStack_a4 = 0x80000000;
                    // uVar11 = (uint)(short)(*(short *)(param_1 + 0x13e) - *(short *)(param_1 + 0xba));
                    // uStack_ac = uVar11 ^ 0x80000000;
                    // iVar13 = (int)(((fVar14 * ((float)((double)CONCAT44(local_b0, 0x80000000) -
                    //                                     4503601774854144.0) * 0.01 + 0.4) +
                    //                 ABS(fVar1) *
                    //                 ((float)((double)CONCAT44(local_a8, 0x80000000) - 4503601774854144.0) *
                    //                 0.01 + 0.3)) * 65535.0) / 194.77875);
                    //local_a0 = (longlong)iVar13;
                    //fVar1 = (float)((double)(CONCAT44(local_b0, uVar11) ^ 0x80000000) - 4503601774854144.0) *
                    //        9.58738e-05;
                    //EGG::Quatf::setAxisRotation(fVar1, &q0, &mVec3_c::Ey);
                    //EGG::Quatf::setAxisRotation(-fVar1, &q1, &mVec3_c::Ey);
                    //uVar11 = (uint)(short)iVar13;
                    //uStack_a4 = uVar11 ^ 0x80000000;
                    //EGG::Quatf::setAxisRotation
                    //            ((float)((double)(CONCAT44(local_a8, uVar11) ^ 0x80000000) - 4503601774854144.0)
                    //            * 9.58738e-05, &q3, &mVec3_c::Ex);
                    /*f32 fVar3 = (q0.v.z * q3.v.x - q0.v.x * q3.v.z) +
                            q3.v.y * q0.w + q0.v.y * q3.w;
                    f32 fVar5 = (q0.v.y * q3.v.z - q0.v.z * q3.v.y) +
                            q3.v.x * q0.w + q0.v.x * q3.w;
                    f32 fVar4 = (q0.v.x * q3.v.y - q0.v.y * q3.v.x) +
                            q3.v.z * q0.w + q0.v.z * q3.w;
                    f32 fVar6 = q0.w * q3.w -
                            (q0.v.z * q3.v.z +
                            q0.v.x * q3.v.x + q0.v.y * q3.v.y);
                    f32 fVar1 = field_0x9c0.v.z;
                    fVar14 = field_0x9c0.v.y;
                    f32 fVar7 = (fVar3 * q1.v.z - fVar4 * q1.v.y) + q1.v.x * fVar6 +
                            fVar5 * q1.w;
                    f32 fVar8 = (fVar4 * q1.v.x - fVar5 * q1.v.z) + q1.v.y * fVar6 +
                            fVar3 * q1.w;
                    f32 fVar2 = field_0x9c0.v.x;
                    f32 fVar9 = (fVar5 * q1.v.y - fVar3 * q1.v.x) + q1.v.z * fVar6 +
                            fVar4 * q1.w;
                    fVar4 = fVar6 * q1.w -
                            (fVar4 * q1.v.z + fVar5 * q1.v.x + fVar3 * q1.v.y);
                    fVar3 = field_0x9c0.w;
                    field_0x9c0.v.x =
                        (fVar8 * fVar1 - fVar9 * fVar14) + fVar2 * fVar4 + fVar7 * fVar3;
                    field_0x9c0.v.y =
                        (fVar9 * fVar2 - fVar7 * fVar1) + fVar14 * fVar4 + fVar8 * fVar3;
                    field_0x9c0.v.z =
                        (fVar7 * fVar14 - fVar8 * fVar2) + fVar1 * fVar4 + fVar9 * fVar3;
                    field_0x9c0.w =
                        fVar4 * fVar3 - (fVar9 * fVar1 + fVar7 * fVar2 + fVar8 * fVar14);*/
                    field_0x9c0 = field_0x9c0 * q0 * q3 * q1 ;
                    return;
                }
                if (field_0x9bc != 1) {
                    return;
                }
                mRotation.y = mRotation.y + 0xc00;
                sLib::addCalcAngle(mRotation.x.ref(), 0x1000, 5, 0x100);
                sLib::addCalcAngle(mRotation.z.ref(), 0, 5, 0x100);
                return;
                
            }
        }
    }
    //fn_19_2C20();
}
void dAcPumpkin_c::finalizeState_Throw() {}
void dAcPumpkin_c::initializeState_Roll() {
    field_0x908 = 0;
    mSph.mAt.mSrc.mSPrm = mSph.mAt.mSrc.mSPrm & 0xfffffffe;
}
void dAcPumpkin_c::executeState_Roll() {
    fn_19_31B0();
    if (mLinkage.checkConnection(dLinkage_c::CONNECTION_6)) {
        mStateMgr.changeState(StateID_Roll);
        return;
    }
    if(mLinkage.checkState(dLinkage_c::STATE_ACTIVE)) {
        mStateMgr.changeState(StateID_Wait);
        return;
    }
    if (mSph.ChkTgHit()) {
                // Handle hit type 0x20
        if(field_0xa17 != 0) {    
            fn_19_2C20(); 
        }
    }
    if ((mObjAcch.mFlags & 0x80) || mObjAcch.ChkWallHit(nullptr)) {
        if (mObjAcch.mFlags & 0x80) {
            if(field_0x908 > -150.0f) {
                dJEffManager_c::spawnGroundEffect(mPosition, mPolyAttr0, mPolyAttr1, field_0x1B4, 0, 1.0f, field_0x1B0);
                fn_19_2C20();
                
            }
            else {
                AttentionManager::GetInstance()->addPickUpTarget(*this, 120.0f);
                AttentionManager::GetInstance()->addUnk3Target(*this, 1, 500.0f, -200.0f, 200.0f);
            }
            field_0x908 = 0;
            mSpeed *= 0.98f;
            if(mSpeed < 1.0f) {
                mSpeed = 0;
            }
        }
        if(field_0x99c == 0) {
            
            field_0x9b4 += field_0x9b4 * 0.05f;
        }
        return;
    }

}
void dAcPumpkin_c::finalizeState_Roll() {}
void dAcPumpkin_c::initializeState_CarryBeetle() {
    fn_19_30C0();
    mSpeed = 0;
    mVelocity = mVec3_c::Zero;
    mSph.mCo.mSrc.mSPrm = mSph.mCo.mSrc.mSPrm | 0x400;
    mSph.mAt.mSrc.mSPrm = mSph.mAt.mSrc.mSPrm & 0xfffffffe;
    mActorProperties = mActorProperties & 0xfffffffe;
}
void dAcPumpkin_c::executeState_CarryBeetle() {
    if ((mObjAcch.mFlags & 0x80) || mObjAcch.ChkWallHit(nullptr)) {
        if (mObjAcch.mFlags & 0x80) {
            dJEffManager_c::spawnGroundEffect(mPosition, mPolyAttr0, mPolyAttr1, field_0x1B4, 0, 1.0f, field_0x1B0);
        }
        fn_19_2C20();
        return;
    }

    if (mLinkage.checkConnection(dLinkage_c::CONNECTION_1)) {
        field_0xa15 = 0;
        mStateMgr.changeState(StateID_Stick); // figure out which state this is later
        return;
    }
    if (mLinkage.getState() == 1) {
        mStateMgr.changeState(StateID_Roll); // figure out which state this is later
        mSph.mAt.mSrc.mSPrm = mSph.mAt.mSrc.mSPrm | 0x1;
    }
}
void dAcPumpkin_c::finalizeState_CarryBeetle() {
    mSph.mCo.mSrc.mSPrm = mSph.mCo.mSrc.mSPrm & 0xfffffbff;
}
void dAcPumpkin_c::initializeState_CarryWhip() {
    fn_19_30C0();
    mSpeed = 0;
    mVelocity = mVec3_c::Zero;
    mSph.mAt.mSrc.mSPrm = mSph.mAt.mSrc.mSPrm & 0xfffffffe;
    mActorProperties = mActorProperties & 0xfffffffe;
}
void dAcPumpkin_c::executeState_CarryWhip() { // TODO adjust symbol because of blr at end issue
    if (mLinkage.checkConnection(dLinkage_c::CONNECTION_1)) {
        mStateMgr.changeState(StateID_Stick); // figure out which state this is later
    } else {
        if (mLinkage.getState() == dLinkage_c::STATE_ACTIVE) {
            return;
        }
        mStateMgr.changeState(StateID_Roll); // figure out which state this is later
    }
}
void dAcPumpkin_c::finalizeState_CarryWhip() {}

void dAcPumpkin_c::fn_19_220() {
    // if(m3d::mShadowCircle_c::~mShadowCircle_c()) {
    //    delete(this);
    // }
}
void dAcPumpkin_c::fn_19_2C20() { // destroy related to pumpkin game?

    // fn_80022BE0(dLightEnv_c::GetPInstance(), mPosition);
    // getLinkage().forceRemove(this);
    mVec3_c tempVec;
    fn_19_3130(&tempVec, this); // TODO fix
    // mVec3_c tempVec;
    dEmitterBase_c *fx_thing = dJEffManager_c::spawnEffect(
        PARTICLE_RESOURCE_ID_MAPPING_512_, tempVec, nullptr, nullptr, nullptr, nullptr, 0, 0
    );

    fx_thing = dJEffManager_c::spawnEffect(
        PARTICLE_RESOURCE_ID_MAPPING_491_, mPosition, nullptr, nullptr, nullptr, nullptr, 0, 0
    );
    if (fx_thing) {
        fx_thing->bindShpEmitter(8, true);
        fn_19_30C0();
        startSound(SE_Pumpkin_CRASH);
    }

    s32 tempParams = mParams & 0xf;
    bool temp = tempParams != 1 && tempParams != 2;
    if (temp) {
        u32 tempfield = field_0x9dc;
        if (tempfield) {
            u32 hit = 0;
            if ((mSph.mTg.mRPrm & 1) && mSph.mTg.GetActor() != nullptr) {
                // Handle hit type 0x20

                if (mSph.ChkTgAtHitType(0x2000)) {
                    hit = 1;
                }
                u32 tempParam = getFromParams(0, 0xF) == 2;
                fn_67_26E0(/*field_0x9dc*/ tempfield, hit, mPosition, tempParam); // pumpking shooting function
            }
        }
    }

    getLinkage().forceRemove(this);

    if (EventManager::sInstance->isInEvent()) {
        if (EventManager::sInstance->isInEvent()) {
            if (EventManager::sInstance->fn_800A0B80()) {
                deleteRequest();
                itemDroppingAndGivingRelated(0, 1);
                return;
            }
        }
    }

    killNoItemDrop();
}
void dAcPumpkin_c::fn_19_31B0() { // TODO fix float values
    AttentionManager *tempInstance = AttentionManager::GetInstance();
    if (tempInstance == nullptr) {
        return;
    }
    tempInstance->addPickUpTarget(*this, 1.5f);
    tempInstance->addUnk3Target(*this, 1, 1.6f, 1.2f, 1.3f);
}
void dAcPumpkin_c::fn_19_2FC0(u8 temp) {
    field_0x998 = temp;
}
void dAcPumpkin_c::fn_19_30C0() {
    if (field_0x9b6) {
        mVec3_c tempVec;
        fn_19_3130(&tempVec, this); // TODO fix?
        dJEffManager_c::spawnEffect(
            PARTICLE_RESOURCE_ID_MAPPING_513_, tempVec, nullptr, nullptr, nullptr, nullptr, 0, 0
        );
    }
    field_0x9b6 = 0;
}
void dAcPumpkin_c::fn_19_2FD0() {
    if (field_0x998) {
        // field_0x9c0.slerpTo()
        // f32 tempf1 = 1.f / field_0x998;
        field_0x968.slerpTo(mMtx_c::Identity, field_0x968, 1.f / (f32)field_0x998);
        EGG::Quatf tempQuat;
        tempQuat.set(1.0f, 0.0f, 0.0f, 0.0f);
        field_0x9c0.slerpTo(tempQuat, 1.0f / field_0x998, field_0x9c0);
        // field_0x968.slerpTo(mMtx_c::Identity, field_0x968, 5.2f);
        field_0x998 = field_0x998 - 1;
    }
}
void dAcPumpkin_c::fn_19_4a0() {}
void dAcPumpkin_c::fn_19_500(
    u32 *unused1, u32 *unused2, u32 *tempPtr
) { // have to figure out what ptr is being passed in here to name it properly
    if ((*(tempPtr + 0x1c) & 0x400000) != 0) {
        return;
    }
    if ((*(tempPtr + 0x1c) & 0x800) != 0) {
        return;
    }
    field_0xa15 = 1;
    return;
}
int dAcPumpkin_c::fn_19_520(u32 *param1, u32 *param2, u32 *param3) {
    fn_19_500(param2, param1, param3); // passing random point for now
    return 1;
}
bool dAcPumpkin_c::fn_19_550() {
    nw4r::g3d::ResFile *oarcResFile = (nw4r::g3d::ResFile *)dAcObjBase_c::getOarcResFile("Pumpkin");
    nw4r::g3d::ResMdl tempResMdl = oarcResFile->GetResMdl("Pumpkin");
    if (field_0x330.create(tempResMdl, &mAllocator, 0x120, 1, (u32 *)0)) {
        nw4r::g3d::ResMdl tempResMdl2 = oarcResFile->GetResMdl("Turu");
        if (field_0x34C.create(tempResMdl2, &mAllocator, 0x120, 1, (u32 *)0)) {
            return true;
        }
    }
    return false;
}
int dAcPumpkin_c::fn_19_940() {
    return SUCCEEDED;
}
int dAcPumpkin_c::fn_19_950() {
    if ((mParams & 0xf) != 3) {
        if (mObjAcch.mGroundHeight < -100000.0) {
            mActorProperties = mActorProperties & 0xfffffffe;
        }
        if (mPosition.y < mPositionCopy.y - 10000.0) {
            fBase_c::deleteRequest();
        }
        field_0x9b8 = field_0x9b8 + 1;
        //(**(code **)(*(int *)&param_1->field_0x8cc + 0x10))(&param_1->field_0x8cc);
        if (fn_19_2E10()) {
            calcVelocity(mPosition, mSpeed);
            // mPosition.y = mPosition.y + mVelocity.y + mStts.mCcMove.y;
            // mPosition.x = mPosition.x + mVelocity.x + mStts.mCcMove.x;
            // mPosition.z = mPosition.z + mVelocity.z + mStts.mCcMove.z;
            mPosition += mVelocity; // + mStts.mCcMove;
            mPosition += mStts.GetCcMove();
            field_0x908 += (mPosition.y - mOldPosition.y);
        }

        if (fn_19_2F60()) {
            getLinkage().bushTpFunc(mObjAcch);
        }

        // if ((mParams & 0xf) != 1  && (mParams & 0xf) != 2) {

        if ((mParams & 0xf) == 2) {
            field_0x9E0.holdEffect(
                /*(u16)PARTICLE_RESOURCE_ID_MAPPING_0_[0x274]*/ (u16)0, mPosition, (mAng3_c *)0x0, &mScale,
                (GXColor *)0x0, (GXColor *)0x0
            );
        }
        mVec3_c local_18 = mPosition;
        mPosition.y = mPosition.y + mScale.y * 31.0;
        mSph.SetC(local_18);
        dCcS::GetInstance()->Set(&mSph);
        fn_19_2920();
        mPositionCopy2.x = mPosition.x;
        mPositionCopy2.z = mPosition.z;
        mPositionCopy2.y = mPosition.y + 31.0;
        mPositionCopy3.x = mPosition.x;
        mPositionCopy3.y = mPosition.y + 31.0;
        mPositionCopy3.z = mPosition.z;
        field_0xa18.execute((double)mObjAcch.mWtr.mGroundH, (double)mObjAcch.mGroundHeight);
    }
    //}
    return 1;
}
void dAcPumpkin_c::fn_19_B80() {}
void dAcPumpkin_c::fn_19_2920() {//actor execute?
    sLib::addCalcAngle(&field_0x90C.mVal, 0, 0x3c, 0x100, 0x10);
    field_0x90E = field_0x90E + field_0x912;
    f32 tempf5 = 0.0f;
    if (mStateMgr.isState(StateID_CarryWhip) != 0) { // TODO figure out which state this is later
        
        mVec3_c tempVec = mVec3_c::Ey;
        mAng temp4 = 0;
        PSMTXMultVec(field_0x968, tempVec, tempVec);
        bool tempBool = fabsf(tempVec.absXZ()) <= FLT_EPSILON;
        if (tempBool) {
            tempf5 = tempVec.absXZ();
            //temp4 = temp4.fromVec(tempVec);
            temp4 = temp4.atan2s(tempVec.y, tempf5);
            //temp4 = temp4.degree();
            //tempVec.atan2sY_XZ();
        }
        tempf5 = nw4r::math::SinFIdx(temp4 /** 0.00390625f*/);
        tempf5 = nw4r::math::CosFIdx((float)(0x4000 - temp4) * 1.0f * 0.00390625f);
    }
    getLinkage().fn_800511E0(this);
    Mtx tempMtx;
    PSMTXTrans(tempMtx, 0, 15.0, 0);
    PSMTXConcat(mWorldMtx, tempMtx, mWorldMtx);
    mWorldMtx.YrotM(field_0x90E);
    mWorldMtx.XrotM(field_0x90C);
    //mAng3_c tempAng = -field_0x90E;
    mWorldMtx.YrotM(-field_0x90E);
    Mtx tempMtx2;
    PSMTXTrans(tempMtx, 0, 15.0, 0);
    PSMTXConcat(mWorldMtx, tempMtx, mWorldMtx);
    mWorldMtx.YrotM(-field_0x90E);
    PSMTXTrans(tempMtx, 0, 15.0, 0);
    PSMTXConcat(mWorldMtx, tempMtx, mWorldMtx);
    PSMTXConcat(mWorldMtx, tempMtx, mWorldMtx);
    mWorldMtx.YrotM(field_0x90E);
    mWorldMtx.YrotM(field_0x90C);
    mMtx_c tempMtx3;
    tempMtx3.fromQuat(field_0x9c0);
    // EGG::Matrix34f::fromQuat()
    PSMTXTrans(tempMtx, 0, 15.0, 0);
    PSMTXConcat(mWorldMtx, tempMtx, mWorldMtx);
    PSMTXConcat(mWorldMtx, tempMtx, mWorldMtx);
    PSMTXTrans(tempMtx, 0, 15.0, 0);
    PSMTXConcat(mWorldMtx, tempMtx, mWorldMtx);
    field_0x330.setLocalMtx(mWorldMtx);
    // m3d::scnLeaf_c::setLocalMtx
    field_0x34C.setLocalMtx(mWorldMtx);
    // m3d::scnLeaf_c::setLocalMtx
}
bool dAcPumpkin_c::fn_19_2E10() {
    /*if (mStateMgr.isState(StateID_Stick) == 0) {                 // TODO figure out which state this is later
        if (mStateMgr.isState(StateID_Roll) == 0) {              // TODO figure out which state this is later
            if (mStateMgr.isState(StateID_CarryGrab) == 0) {     // TODO figure out which state this is later
                if (mStateMgr.isState(StateID_CarryWhip) == 0) { // TODO figure out which state this is later
                    return true;
                }
            }
        }
    }
    return false;*/
    return mStateMgr.isState(StateID_Stick) || mStateMgr.isState(StateID_Roll) || mStateMgr.isState(StateID_CarryGrab) || mStateMgr.isState(StateID_CarryWhip);
    //return true;
}
bool dAcPumpkin_c::fn_19_2F60() {
    if (mStateMgr.isState(StateID_Stick) == 0) { // TODO figure out which state this is later
        return true;
    }
    return false;
}
void dAcPumpkin_c::fn_19_3130(mVec3_c *Vec3ptr, dAcPumpkin_c *pumpkinPtr) {
    *Vec3ptr = mVec3_c::Ey;
    *Vec3ptr = *Vec3ptr * 62.0f;
    *Vec3ptr *= 0.5f;
    MTXMultVec(pumpkinPtr->mWorldMtx, *Vec3ptr, *Vec3ptr);
}
void dAcPumpkin_c::fn_19_31b0() {}
void dAcPumpkin_c::fn_19_2900() {
    return;
}
