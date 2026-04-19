#include "d/a/d_a_pumpkin.h"

SPECIAL_ACTOR_PROFILE(PUMPKIN, dAcPumpkin_c, fProfile::PUMPKIN, 0x134, 0, 3);

STATE_DEFINE(dAcPumpkin_c, Stick);
STATE_DEFINE(dAcPumpkin_c, Wait);
STATE_DEFINE(dAcPumpkin_c, CarryGrab);
STATE_DEFINE(dAcPumpkin_c, StickSword);
STATE_DEFINE(dAcPumpkin_c, Throw);
STATE_DEFINE(dAcPumpkin_c, Roll);
STATE_DEFINE(dAcPumpkin_c, CarryBeetle);
STATE_DEFINE(dAcPumpkin_c, CarryWhip);

void dAcPumpkin_c::initializeState_Stick() {

	pumpkinState = pumpkinState & 0xfffffffe;

}

void dAcPumpkin_c::executeState_Stick() {

    fn_19_31B0();

    bool temp = (mLinkage.mState == 1 && mLinkage.mType == 6);
    if (temp) {
        startSound(0x9f4);
        something->vt_0x18();
        return;
    }

    temp = (mLinkage.mState == 1 && mLinkage.mType == 5);
    if (temp) {
        startSound(0x9f4);
        something->vt_0x18();
        return;
    }

    temp = (mLinkage.mState == 1 && mLinkage.mType == 7);
    if (temp) {
        startSound(0x9f4);
        something->vt_0x18();
        return;
    }

    if (mLinkage.mState == 1) {
        startSound(0x9f4);
        field_a15 = 0;
        something->vt_0x18();
        return;
    }

    temp = false;
    if ((field_7ec & 1) && field_7d8->GetActor() != nullptr) {
        temp = true;
    }
    if (temp && field_a17 != 0) {
        fn_19_2C20();
    }



}


void dAcPumpkin_c::finalizeState_Stick() {

	mViewClipIdx = -1;

}
void dAcPumpkin_c::initializeState_Wait() {

    field_0x908 = 0;
    mActorProperties = mActorProperties | 1;
    fn_19_2FC0(10);
    pumpkinState = pumpkinState & 0xfffffffe;

}
void dAcPumpkin_c::executeState_Wait() {

    bool temp = (mLinkage.mState == 1 && mLinkage.mType == 6);
    if (temp) {
        //startSound(0x9f4);
        something->vt_0x18();
        return;
    }
    temp = (mLinkage.mState == 1 && mLinkage.mType == 5);
    if (temp) {
        //startSound(0x9f4);
        something->vt_0x18();
        return;
    }
    temp = (mLinkage.mState == 1 && mLinkage.mType == 1);
    if (temp) {
        //startSound(0x9f4);
        int temp2 = 0;
        if (field_0x40c && mPosition.y < field_0x744) {
            temp2 = 1;
        }
        field_a15 = temp2;
        something->vt_0x18();
        return;
    }
    temp = (mLinkage.mState == 1 && mLinkage.mType == 7);
    if (temp) {
        //startSound(0x9f4);
        something->vt_0x18();
        return;
    }
    if ((field_7ec & 1) && field_7d8->GetActor() != nullptr) {
        temp = true;
    }
    if (temp && field_a17 != 0) {
        fn_19_2C20();
        return;
    }
    temp = field_0x40c & 0x800 && mPosition.y < field_0x744;
    if (temp) {
        if (field_0x40c & 0x80 && mPosition.y + 50 < field_0x744) {
            fn_19_2C20();
        }
        return;

        /*if (angleToRotate(mVec3_c::Zero)) {

        }*/
        mSpeed = mSpeed * 0.93;
        u32 temp5 = field0x9b8 << 0x10;
        s32 temp11 = temp5 / 0x32 + temp5 >> 0x1f;
        float temp15 = nw4r::math::SinFIdx(temp11 - (temp11 >> 0x1f) * 1.0f * 0.00390625);
        mVelocity.y = (mSpeed * 0.8 + (field_0x744 - mPosition.y) * fabs(field_0x744)) / (temp15 * 3.0 + 20.0);
        if (mVelocity.y > 20.0) {
            mVelocity.y = 20.0;
        }
        if (mVelocity.y < -20.0) {
            mVelocity.y = -20.0;
        }
        if (field_0x9db == 0) {
            field_0x908 = cM::rndInt(0x200) + 0x800;
            field_0x90C = cM::rndInt(0x100) + 0xa00;
            startSound(0x9a0);
            mRotation.x = (mRotation.x & 0xfffffffe) >> 0x10;
            mRotation.y = (mRotation.y & 0xfffffffe);
        }
        fn_19_31B0();
        mVelocity.y = 0;
        sLib::addCalcAngle((short*) & (mRotation.x), 0, 5, 0x100);
        sLib::addCalcAngle((short*) &(mRotation.z), 0, 5, 0x100);
    }
}
void dAcPumpkin_c::finalizeState_Wait() {}
void dAcPumpkin_c::initializeState_CarryGrab() {

    fn_19_30C0();
    fn_19_2FC0(4);
    mVelocity = mVec3_c::Zero;
    mSpeed = 0;
    field_0x870 = field_0x870 | 0x400;
    pumpkinState = pumpkinState & 0xfffffffe;
    unsetActorProperty(AC_PROP_0x1);
    field_99c = 0;
    field_a14 = 0;
    field_a16 = 0;
}
void dAcPumpkin_c::executeState_CarryGrab() {}
void dAcPumpkin_c::finalizeState_CarryGrab() {

    field_0x870 = field_0x870 & 0xfffffbff;

}
void dAcPumpkin_c::initializeState_StickSword() {

    fn_19_30C0();
    mVelocity = mVec3_c::Zero;
    field_0x870 = field_0x870 | 0x400;
    pumpkinState = pumpkinState & 0xfffffffe;
    field0x914[0] = 0;
    field0x914[1] = 0;
    field0x914[2] = 0;
    field0x914[3] = 0;
    field0x914[4] = 0;
    field0x914[5] = 0;
    field0x914[6] = 0;
    field0x914[7] = 0;
    field0x914[8] = 0;
    field0x914[9] = 0;
    field0x914[10] = 0;
    field0x914[11] = 0;
    field0x914[12] = 0;
    field0x914[13] = 0;
    field0x914[14] = 0;
    field0x914[15] = 0;
    field0x914[16] = 0;
    field0x914[17] = 0;
    field0x914[18] = 0;
    field0x914[19] = 0;
    field0x914[20] = 0;
    field0x914[21] = 0;
}
void dAcPumpkin_c::executeState_StickSword() {}
void dAcPumpkin_c::finalizeState_StickSword() {

    field_0x870 = field_0x870 & 0xfffffbff;

}
void dAcPumpkin_c::initializeState_Throw() {

    mActorProperties = mActorProperties & 0xfffffffe;

}
void dAcPumpkin_c::executeState_Throw() {}
void dAcPumpkin_c::finalizeState_Throw() {}
void dAcPumpkin_c::initializeState_Roll() {

    field_0x908 = 0;
    pumpkinState = pumpkinState & 0xfffffffe;

}
void dAcPumpkin_c::executeState_Roll() {}
void dAcPumpkin_c::finalizeState_Roll() {}
void dAcPumpkin_c::initializeState_CarryBeetle() {

    fn_19_30C0();

    mVelocity.x = mVec3_c::Zero.x;
    mVelocity.z = mVec3_c::Zero.z;
    mVelocity.y = mVec3_c::Zero.y;
    mSpeed = 0;
    field_0x870 = field_0x870 | 0x400;
    pumpkinState = pumpkinState & 0xfffffffe;
    mActorProperties = mActorProperties & 0xfffffffe;

}
void dAcPumpkin_c::executeState_CarryBeetle() {

    if (field_0x40c & 0x80 || field_0x3cc->ChkWallHit()) {
        dJEffManager_c::spawnGroundEffect(mPosition, mPolyAttr0, mPolyAttr1, field_0x1B4, 0, 1.0f, field_0x1B0);
    }

}
void dAcPumpkin_c::finalizeState_CarryBeetle() {}
void dAcPumpkin_c::initializeState_CarryWhip() {}
void dAcPumpkin_c::executeState_CarryWhip() {}
void dAcPumpkin_c::finalizeState_CarryWhip() {}
