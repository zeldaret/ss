#include "d/lyt/meter/d_lyt_meter_ganbari_gauge.h"

#include "common.h"
#include "d/a/d_a_player.h"
#include "d/d_camera.h"
#include "d/d_sc_game.h"
#include "d/lyt/d_lyt_meter_hio.h"
#include "m/m_vec.h"
#include "toBeSorted/d_d3d.h"
#include "toBeSorted/event_manager.h"
#include "toBeSorted/file_manager.h"

STATE_DEFINE(dLytMeterGanbariGauge_c, InvisibleWait);
STATE_DEFINE(dLytMeterGanbariGauge_c, In);
STATE_DEFINE(dLytMeterGanbariGauge_c, Wait);
STATE_DEFINE(dLytMeterGanbariGauge_c, OutWait);
STATE_DEFINE(dLytMeterGanbariGauge_c, Out);
STATE_DEFINE(dLytMeterGanbariGauge_c, CameraOut);
STATE_DEFINE(dLytMeterGanbariGauge_c, Full);
STATE_DEFINE(dLytMeterGanbariGauge_c, FullGutsUse);
STATE_DEFINE(dLytMeterGanbariGauge_c, Normal);
STATE_DEFINE(dLytMeterGanbariGauge_c, Caution);
STATE_DEFINE(dLytMeterGanbariGauge_c, ToMin);
STATE_DEFINE(dLytMeterGanbariGauge_c, Recovery);
STATE_DEFINE(dLytMeterGanbariGauge_c, ToMax);

static const d2d::LytBrlanMapping brlanMap[] = {
    {           "guts_00_in.brlan",       "G_inOut_00"},
    {        "guts_00_angle.brlan",       "G_angle_00"},
    {"guts_00_ganbariUpDown.brlan",     "G_ganbari_00"},
    {"guts_00_ganbariUpDown.brlan",     "G_ganbari_01"},
    {   "guts_00_ganbariUse.brlan",     "G_ganbari_01"},
    {  "guts_00_ganbariLoop.brlan", "G_ganbariLoop_00"},
    {        "guts_00_drink.brlan",       "G_drink_00"},
    {        "guts_00_mLoop.brlan",       "G_mLoop_00"},
    {     "guts_00_mCaution.brlan",    "G_mCaution_00"},
    {      "guts_00_caution.brlan",     "G_caution_00"},
    {        "guts_00_toMin.brlan",      "G_minMax_00"},
    {    "guts_00_toMinLoop.brlan",      "G_toMinLoop"},
    {        "guts_00_toMax.brlan",      "G_minMax_00"},
    {        "guts_00_color.brlan",       "G_color_00"},
    {          "guts_00_out.brlan",       "G_inOut_00"},
    {    "guts_00_cameraOut.brlan",       "G_inOut_00"},
};

#define GANBARI_ANIM_IN 0
#define GANBARI_ANIM_ANGLE 1
#define GANBARI_ANIM_UPDOWN_0 2
#define GANBARI_ANIM_UPDOWN_1 3
#define GANBARI_ANIM_USE 4
#define GANBARI_ANIM_LOOP 5
#define GANBARI_ANIM_DRINK 6
#define GANBARI_ANIM_M_LOOP 7
#define GANBARI_ANIM_M_CAUTION 8
#define GANBARI_ANIM_CAUTION 9
#define GANBARI_ANIM_TO_MIN 10
#define GANBARI_ANIM_TO_MIN_LOOP 11
#define GANBARI_ANIM_TO_MAX 12
#define GANBARI_ANIM_COLOR 13
#define GANBARI_ANIM_OUT 14
#define GANBARI_ANIM_CAMERA_OUT 15

#define GANBARI_NUM_ANIMS 16

void dLytMeterGanbariGauge_c::initializeState_InvisibleWait() {
    mAnm[GANBARI_ANIM_IN].setToStart();
    mAnm[GANBARI_ANIM_IN].setAnimEnable(true);
    mLyt.calc();
    mAnm[GANBARI_ANIM_IN].setAnimEnable(false);
    mAnm[GANBARI_ANIM_OUT].setAnimEnable(false);
}
void dLytMeterGanbariGauge_c::executeState_InvisibleWait() {}
void dLytMeterGanbariGauge_c::finalizeState_InvisibleWait() {}

void dLytMeterGanbariGauge_c::initializeState_In() {
    if (dAcPy_c::GetLink()->getRidingActorType() == dAcPy_c::RIDING_BOAT) {
        field_0x554 = 3.0f;
    } else if (swimmingRelated()) {
        field_0x554 = 1.0f;
    } else {
        field_0x554 = 0.0f;
    }
    mAnm[GANBARI_ANIM_IN].setAnimEnable(true);
    mAnm[GANBARI_ANIM_IN].setFrame(0.0f);
}
#pragma pool_data off
void dLytMeterGanbariGauge_c::executeState_In() {
    if (mAnm[GANBARI_ANIM_IN].getFrame() == 1.0f) {
        realizeAnimState();
        if (*mStateMgrWheel.getStateID() == StateID_Caution || *mStateMgrWheel.getStateID() == StateID_ToMin ||
            *mStateMgrWheel.getStateID() == StateID_Recovery || *mStateMgrWheel.getStateID() == StateID_ToMax) {
            if (mStaminaPercent > 0.95f) {
                mStateMgrWheel.changeState(StateID_Normal);
            } else {
                mStateMgrWheel.changeState(*mStateMgrWheel.getStateID());
            }
        }
    }

    if (mAnm[GANBARI_ANIM_IN].isEndReached()) {
        mAnm[GANBARI_ANIM_IN].setAnimEnable(false);
        mStateMgrMain.changeState(StateID_Wait);
    }
}
#pragma pool_data on
void dLytMeterGanbariGauge_c::finalizeState_In() {
    mAnm[GANBARI_ANIM_IN].setAnimEnable(false);
}

void dLytMeterGanbariGauge_c::initializeState_Wait() {}
void dLytMeterGanbariGauge_c::executeState_Wait() {}
void dLytMeterGanbariGauge_c::finalizeState_Wait() {}

void dLytMeterGanbariGauge_c::initializeState_OutWait() {
    mOutWaitTimer = 0x1E;
}
void dLytMeterGanbariGauge_c::executeState_OutWait() {
    if (--mOutWaitTimer <= 0) {
        mStateMgrMain.changeState(StateID_Out);
    } else if (mStaminaPercent < 0.9999f) {
        mStateMgrMain.changeState(StateID_Wait);
    }
}
void dLytMeterGanbariGauge_c::finalizeState_OutWait() {}

void dLytMeterGanbariGauge_c::initializeState_Out() {
    mAnm[GANBARI_ANIM_IN].setAnimEnable(false);
    mAnm[GANBARI_ANIM_OUT].setAnimEnable(true);
    mAnm[GANBARI_ANIM_OUT].setFrame(0.0f);
}
void dLytMeterGanbariGauge_c::executeState_Out() {
    if (mAnm[GANBARI_ANIM_OUT].isEndReached()) {
        mLyt.calc();
        mStateMgrMain.changeState(StateID_InvisibleWait);
    }
}
void dLytMeterGanbariGauge_c::finalizeState_Out() {
    mAnm[GANBARI_ANIM_OUT].setAnimEnable(false);
}

void dLytMeterGanbariGauge_c::initializeState_CameraOut() {
    mAnm[GANBARI_ANIM_CAMERA_OUT].setAnimEnable(true);
    mAnm[GANBARI_ANIM_CAMERA_OUT].setFrame(0.0f);
}
void dLytMeterGanbariGauge_c::executeState_CameraOut() {
    if (mAnm[GANBARI_ANIM_CAMERA_OUT].isEndReached()) {
        mLyt.calc();
        mStateMgrMain.changeState(StateID_InvisibleWait);
    }
}
void dLytMeterGanbariGauge_c::finalizeState_CameraOut() {
    mAnm[GANBARI_ANIM_CAMERA_OUT].setAnimEnable(false);
}

void dLytMeterGanbariGauge_c::initializeState_Full() {}
void dLytMeterGanbariGauge_c::executeState_Full() {
    if (mStaminaPercent < 0.9999f) {
        mStateMgrWheel.changeState(StateID_Normal);
    } else if (dAcPy_c::GetLink()->checkSwordAndMoreStates(0x200 | 0x40 | 0x20) || field_0x558 != 0) {
        mStateMgrWheel.changeState(StateID_FullGutsUse);
    }
}
void dLytMeterGanbariGauge_c::finalizeState_Full() {}

void dLytMeterGanbariGauge_c::initializeState_FullGutsUse() {}
void dLytMeterGanbariGauge_c::executeState_FullGutsUse() {
    if (mStaminaPercent < 0.9999f) {
        mStateMgrWheel.changeState(StateID_Normal);
    } else if (!dAcPy_c::GetLink()->checkSwordAndMoreStates(0x200 | 0x40 | 0x20) && field_0x558 == 0) {
        mStateMgrWheel.changeState(StateID_Full);
    }
}
void dLytMeterGanbariGauge_c::finalizeState_FullGutsUse() {}

void dLytMeterGanbariGauge_c::initializeState_Normal() {}
void dLytMeterGanbariGauge_c::executeState_Normal() {
    if (isStaminaLow(mStaminaPercent)) {
        mAnm[GANBARI_ANIM_CAUTION].setRate(1.0f);
        mStateMgrWheel.changeState(StateID_Caution);
    } else if (isStaminaCritical(mStaminaPercent)) {
        mAnm[GANBARI_ANIM_CAUTION].setRate(3.0f);
        mStateMgrWheel.changeState(StateID_Caution);
    } else if (mStaminaPercent >= 0.9999f) {
        mStateMgrWheel.changeState(StateID_ToMax);
    }
}
void dLytMeterGanbariGauge_c::finalizeState_Normal() {}

void dLytMeterGanbariGauge_c::initializeState_Caution() {
    mAnm[GANBARI_ANIM_CAUTION].setAnimEnable(true);
    mAnm[GANBARI_ANIM_CAUTION].setFrame(0.0f);
}
void dLytMeterGanbariGauge_c::executeState_Caution() {
    if (mStaminaPercent <= 0.0001f) {
        mStateMgrWheel.changeState(StateID_ToMin);
    } else if (isStaminaCritical(mStaminaPercent)) {
        mAnm[GANBARI_ANIM_CAUTION].setRate(3.0f);
    } else if (isStaminaLow(mStaminaPercent)) {
        mAnm[GANBARI_ANIM_CAUTION].setRate(1.0f);
    } else {
        mStateMgrWheel.changeState(StateID_Normal);
    }
}
void dLytMeterGanbariGauge_c::finalizeState_Caution() {
    mAnm[GANBARI_ANIM_CAUTION].setFrame(0.0f);
    mLyt.calc();
    mAnm[GANBARI_ANIM_CAUTION].setAnimEnable(false);
}

void dLytMeterGanbariGauge_c::initializeState_ToMin() {
    mAnm[GANBARI_ANIM_UPDOWN_1].setAnimEnable(true);
    mAnm[GANBARI_ANIM_UPDOWN_1].setFrame(0.0f);
    mLyt.calc();
    mAnm[GANBARI_ANIM_UPDOWN_1].setAnimEnable(false);

    mAnm[GANBARI_ANIM_TO_MIN].setAnimEnable(true);
    mAnm[GANBARI_ANIM_TO_MIN].setToStart();
}
void dLytMeterGanbariGauge_c::executeState_ToMin() {
    if (mStaminaPercent >= 0.9999f) {
        mAnm[GANBARI_ANIM_TO_MIN].setToStart();
        mLyt.calc();
        mAnm[GANBARI_ANIM_TO_MIN].setAnimEnable(false);
        mStateMgrWheel.changeState(StateID_ToMax);
    } else if (mAnm[GANBARI_ANIM_TO_MIN].isEndReached()) {
        mStateMgrWheel.changeState(StateID_Recovery);
    }
}
void dLytMeterGanbariGauge_c::finalizeState_ToMin() {}

void dLytMeterGanbariGauge_c::initializeState_Recovery() {
    mAnm[GANBARI_ANIM_TO_MIN].setAnimEnable(true);
    mAnm[GANBARI_ANIM_TO_MIN].setToEnd2();
    mLyt.calc();
    mAnm[GANBARI_ANIM_TO_MIN].setAnimEnable(false);

    mAnm[GANBARI_ANIM_TO_MIN_LOOP].setAnimEnable(true);
    mAnm[GANBARI_ANIM_TO_MIN_LOOP].setFrame(0.0f);

    if (dAcPy_c::GetLink()->getRidingActorType() == dAcPy_c::RIDING_BOAT) {
        field_0x554 = 4.0f;
    } else if (swimmingRelated()) {
        field_0x554 = 1.0f;
    } else {
        field_0x554 = 2.0f;
    }
}
void dLytMeterGanbariGauge_c::executeState_Recovery() {
    if (swimmingRelated()) {
        if (mStaminaPercent >= 0.0001f) {
            mAnm[GANBARI_ANIM_TO_MIN_LOOP].setFrame(0.0f);
            mAnm[GANBARI_ANIM_TO_MAX].setAnimEnable(true);
            mAnm[GANBARI_ANIM_TO_MAX].setToEnd2();
            mAnm[GANBARI_ANIM_UPDOWN_1].setAnimEnable(true);
            mAnm[GANBARI_ANIM_UPDOWN_1].setFrame(0.0f);
            mLyt.calc();
            mAnm[GANBARI_ANIM_TO_MIN_LOOP].setAnimEnable(false);
            mAnm[GANBARI_ANIM_TO_MAX].setAnimEnable(false);
            mAnm[GANBARI_ANIM_UPDOWN_1].setAnimEnable(false);
            mStateMgrWheel.changeState(StateID_Normal);
        }
    } else if (mStaminaPercent >= 0.9999f) {
        mAnm[GANBARI_ANIM_TO_MIN_LOOP].setFrame(0.0f);
        mLyt.calc();
        mAnm[GANBARI_ANIM_TO_MIN_LOOP].setAnimEnable(false);
        mStateMgrWheel.changeState(StateID_ToMax);
    }
}
void dLytMeterGanbariGauge_c::finalizeState_Recovery() {}

void dLytMeterGanbariGauge_c::initializeState_ToMax() {
    mAnm[GANBARI_ANIM_TO_MIN].setAnimEnable(true);
    mAnm[GANBARI_ANIM_TO_MIN].setToStart();
    mLyt.calc();
    mAnm[GANBARI_ANIM_TO_MIN].setAnimEnable(false);
    mAnm[GANBARI_ANIM_TO_MAX].setAnimEnable(true);
    mAnm[GANBARI_ANIM_TO_MAX].setToStart();
    if (swimmingRelated()) {
        field_0x554 = 1.0f;
    }
}
void dLytMeterGanbariGauge_c::executeState_ToMax() {
    if (mAnm[GANBARI_ANIM_TO_MAX].isStop2()) {
        realizeAnimState();
        if (dAcPy_c::GetLink()->checkSwordAndMoreStates(0x200 | 0x40 | 0x20)) {
            mStateMgrWheel.changeState(StateID_FullGutsUse);
        } else {
            mStateMgrWheel.changeState(StateID_Full);
        }
    }
}
void dLytMeterGanbariGauge_c::finalizeState_ToMax() {}

bool dLytMeterGanbariGauge_c::isStaminaLow(f32 arg) {
    if (swimmingRelated()) {
        return arg <= 0.125f;
    } else {
        return arg <= 0.3f;
    }
}

bool dLytMeterGanbariGauge_c::isStaminaCritical(f32 arg) {
    if (swimmingRelated()) {
        return false;
    } else {
        return arg <= 0.1f;
    }
}

bool dLytMeterGanbariGauge_c::build(d2d::ResAccIf_c *resAcc) {
    mLyt.setResAcc(resAcc);
    mLyt.build("guts_00.brlyt", nullptr);

    for (int i = 0; i < GANBARI_NUM_ANIMS; i++) {
        mAnm[i].init(brlanMap[i].mFile, resAcc, mLyt.getLayout(), brlanMap[i].mName);
        mAnm[i].bind(false);
        mAnm[i].setAnimEnable(true);
    }

    mAnm[GANBARI_ANIM_USE].setAnimEnable(false);
    mAnm[GANBARI_ANIM_LOOP].setAnimEnable(false);
    mAnm[GANBARI_ANIM_M_LOOP].setAnimEnable(false);
    mAnm[GANBARI_ANIM_M_CAUTION].setAnimEnable(false);
    mAnm[GANBARI_ANIM_OUT].setAnimEnable(false);
    mAnm[GANBARI_ANIM_CAMERA_OUT].setAnimEnable(false);

    mStaminaPercent = 1.0f;
    field_0x51C = 1.0f;
    field_0x54C = 1.0f;
    field_0x550 = 1.0f;

    field_0x548 = dLytMeter_HIO_c::sInstance->field_0x1C8;

    field_0x558 = 0;
    field_0x559 = 0;
    field_0x55B = 0;
    mpOwnerPane = nullptr;
    field_0x55A = 0;

    field_0x554 = 0.0f;

    mOutWaitTimer = 0x1E;
    field_0x540 = 0;
    field_0x534 = 0;

    if (dLytMeter_HIO_c::sInstance->field_0x13C != 0) {
        field_0x534 = 0;
    } else {
        field_0x534 = 1;
    }

    if (field_0x534 == 0) {
        mAnm[GANBARI_ANIM_IN].setToStart();
    } else {
        mAnm[GANBARI_ANIM_IN].setToEnd2();
    }

    mAnm[GANBARI_ANIM_IN].setAnimEnable(true);
    realizeAnimState();
    mAnm[GANBARI_ANIM_IN].setAnimEnable(false);

    field_0x524.copyFrom(mLyt.getLayout()->GetRootPane()->GetTranslate());
    field_0x530 = 0;
    field_0x544 = 0;

    if (field_0x534 == 0) {
        mStateMgrMain.changeState(StateID_InvisibleWait);
    } else {
        mStateMgrMain.changeState(StateID_Wait);
    }

    mStateMgrWheel.changeState(StateID_Full);

    return true;
}

bool dLytMeterGanbariGauge_c::remove() {
    for (int i = 0; i < GANBARI_NUM_ANIMS; i++) {
        mAnm[i].remove();
    }
    return true;
}

bool dLytMeterGanbariGauge_c::execute() {
    if (mpOwnerPane == nullptr) {
        return true;
    }

    if (!mpOwnerPane->IsVisible()) {
        if (*mStateMgrMain.getStateID() != StateID_InvisibleWait) {
            mStateMgrMain.changeState(StateID_InvisibleWait);
        }
        return true;
    }

    if (fn_80104710(false) || (isCrawling() && !fn_801047B0())) {
        field_0x534 = 1;
    } else if (dLytMeter_HIO_c::GetInstance()->field_0x13C) {
        field_0x534 = 0;
    } else {
        field_0x534 = 1;
    }

    bool b12 = false;
    field_0x558 = 0;
    f32 drinkFrame = 0.0f;
    if (displayAirInsteadOfStamina()) {
        if (FileManager::GetInstance()->hasAirPotionPlus() || FileManager::GetInstance()->hasAirPotionNormal()) {
            b12 = true;
            if ((FileManager::GetInstance()->hasAirPotionNormal() &&
                 FileManager::GetInstance()->getAirPotionTimer() < 600) ||
                (FileManager::GetInstance()->hasAirPotionPlus() &&
                 FileManager::GetInstance()->getAirPotionPlusTimer() < 600)) {
                field_0x558 = 1;
            }
            if (FileManager::GetInstance()->hasAirPotionPlus()) {
                drinkFrame = 3.0f;
            } else if (FileManager::GetInstance()->hasAirPotionNormal()) {
                drinkFrame = 1.0f;
            }
        }
    } else {
        if (dAcPy_c::GetLink()->isAffectedByStaminaPotionPlus() || dAcPy_c::GetLink()->isAffectedByStaminaPotion()) {
            b12 = true;
            if ((dAcPy_c::GetLink()->isAffectedByStaminaPotion() &&
                 FileManager::GetInstance()->getStaminaPotionTimer() < 600) ||
                (dAcPy_c::GetLink()->isAffectedByStaminaPotionPlus() &&
                 FileManager::GetInstance()->getStaminaPotionPlusTimer() < 600)) {
                field_0x558 = 1;
            }
            if (dAcPy_c::GetLink()->isAffectedByStaminaPotionPlus()) {
                drinkFrame = 2.0f;
            } else if (dAcPy_c::GetLink()->isAffectedByStaminaPotion()) {
                drinkFrame = 1.0f;
            }
        }
    }

    mVec3_c diff = dAcPy_c::GetLink()->mPositionCopy3 - dScGame_c::getCamera(0)->getPositionMaybe();
    f32 len = diff.mag();
    mVec3_c v;
    d3d::worldToScreen(v, dAcPy_c::GetLink()->mPositionCopy3);
    if (!EventManager::isInEvent()) {
        field_0x524 = v;
    }

    s32 angle = 0;
    mVec3_c v1(field_0x524);
    mVec2_c v2(0.0f, 0.0f);
    if (fn_80104710(true)) {
        field_0x540 = 5;
    } else if (field_0x540 > 0 && *mStateMgrMain.getStateID() == StateID_Wait) {
        mStateMgrMain.changeState(StateID_CameraOut);
    }

    f32 scale;
    if (fn_80104710(false) || (isCrawling() && !fn_801047B0())) {
        if (dAcPy_c::GetLink()->getRidingActorType() == dAcPy_c::RIDING_BOAT) {
            angle = 4;
            v1.x = 232.0f;
            v1.y = 145.0f;
            scale = 1.12f;
        } else if (displayAirInsteadOfStamina()) {
            angle = 2;
            scale = 1.45f;
            v1.x = 232.0f;
            v1.y = 145.0f;
        } else {
            angle = 2;
            scale = 1.3f;
            v1.x = 232.0f;
            v1.y = 145.0f;
        }
    } else if (fn_801047B0()) {
        v2.set(-32.0f, -40.0f);
        scale = 0.8f;
    } else if (field_0x55B) {
        v2.x = -44.0f;
        v2.y = -105.0f;
        v2.y += 46.0f;
        scale = 1.0f;
    } else if (dAcPy_c::GetLink()->getRidingActorType() == dAcPy_c::RIDING_BOAT) {
        angle = 3;
        v2.set(-95.0f, -50.0f);
        scale = 0.9f;
    } else if (field_0x559 && swimmingRelated()) {
        angle = 1;
        v2.set(-65.0f, 0.0f);
        scale = 1.0f;
    } else if (field_0x534 == 0) {
        v2.set(-44.0f, -105.0f);
        scale = 1.0f;
    } else {
        v1.x = 216.0f;
        v1.y = -60.0f;
        scale = 1.4f;
    }

    v1.x = v1.x + v2.x;
    v1.y = v1.y + v2.y;
    v1.z = 0.0f;
    mLyt.getLayout()->GetRootPane()->SetTranslate(v1);
    mVec2_c scaleV(scale, scale);
    mLyt.getLayout()->GetRootPane()->SetScale(scaleV);

    if (!field_0x534 && len <= dLytMeter_HIO_c::GetInstance()->field_0x138 && !fn_80104710(false) && !isCrawling()) {
        field_0x530 = 1;
    } else {
        field_0x530 = 0;
    }

    for (int i = 0; i < GANBARI_NUM_ANIMS; i++) {
        if (i != GANBARI_ANIM_ANGLE && i != GANBARI_ANIM_UPDOWN_0 && i != GANBARI_ANIM_UPDOWN_1 &&
            i != GANBARI_ANIM_USE && i != GANBARI_ANIM_COLOR && i != GANBARI_ANIM_DRINK) {
            if (mAnm[i].isEnabled()) {
                mAnm[i].play();
            }
        }
    }

    mAnm[GANBARI_ANIM_UPDOWN_0].setFrame(mStaminaPercent * 100.0f);
    mAnm[GANBARI_ANIM_ANGLE].setFrame(angle);

    mStateMgrWheel.executeState();

    if (field_0x544 != 0) {
        if (mStaminaPercent < field_0x550 &&
            mStaminaPercent < field_0x51C - dLytMeter_HIO_c::GetInstance()->getField_0x1CC()) {
            field_0x548 = dLytMeter_HIO_c::GetInstance()->getField_0x1C8();
            field_0x54C = field_0x51C;
            field_0x550 = mStaminaPercent;
            mAnm[GANBARI_ANIM_UPDOWN_1].setFrame(field_0x54C * 100.0f);
            mAnm[GANBARI_ANIM_UPDOWN_1].setAnimEnable(true);
        } else {
            if (field_0x548 > 0) {
                field_0x548--;
            }
            if (field_0x548 == 0) {
                field_0x54C -= dLytMeter_HIO_c::GetInstance()->getField_0x1D0();
                if (field_0x54C < mStaminaPercent) {
                    field_0x544 = 0;
                    mAnm[GANBARI_ANIM_UPDOWN_1].setAnimEnable(true);
                    mAnm[GANBARI_ANIM_UPDOWN_1].setFrame(0.0f);
                    mLyt.calc();
                    mAnm[GANBARI_ANIM_UPDOWN_1].setAnimEnable(false);
                } else {
                    f32 frame = field_0x54C * 100.0f;
                    mAnm[GANBARI_ANIM_UPDOWN_1].setAnimEnable(true);
                    mAnm[GANBARI_ANIM_UPDOWN_1].setFrame(frame);
                }
            }
        }
    } else {
        if (mStaminaPercent < field_0x51C - dLytMeter_HIO_c::GetInstance()->getField_0x1C4()) {
            field_0x544 = 1;
            field_0x54C = field_0x51C;
            field_0x550 = mStaminaPercent;
            field_0x548 = dLytMeter_HIO_c::GetInstance()->getField_0x1C8();
            mAnm[GANBARI_ANIM_UPDOWN_1].setFrame(field_0x54C * 100.0f);
            mAnm[GANBARI_ANIM_UPDOWN_1].setAnimEnable(true);
        } else {
            mAnm[GANBARI_ANIM_UPDOWN_1].setFrame(mStaminaPercent * 100.0f);
            mAnm[GANBARI_ANIM_UPDOWN_1].setAnimEnable(true);
        }
    }

    if (b12) {
        if (!mAnm[GANBARI_ANIM_M_LOOP].isEnabled()) {
            mAnm[GANBARI_ANIM_M_LOOP].setFrame(0.0f);
            mAnm[GANBARI_ANIM_M_LOOP].setAnimEnable(true);
        }
    } else if (mAnm[GANBARI_ANIM_M_LOOP].getFrame() < 1.0f) {
        mAnm[GANBARI_ANIM_M_LOOP].setFrame(0.0f);
        mAnm[GANBARI_ANIM_M_LOOP].setAnimEnable(false);
    }

    if (b12 && field_0x558) {
        if (!mAnm[GANBARI_ANIM_M_CAUTION].isEnabled()) {
            mAnm[GANBARI_ANIM_M_CAUTION].setFrame(0.0f);
            mAnm[GANBARI_ANIM_M_CAUTION].setAnimEnable(true);
        }
    } else if (mAnm[GANBARI_ANIM_M_CAUTION].getFrame() < 1.0f) {
        mAnm[GANBARI_ANIM_M_CAUTION].setFrame(0.0f);
        mAnm[GANBARI_ANIM_M_CAUTION].setAnimEnable(false);
    }

    if (!b12 && mStaminaPercent < field_0x51C) {
        if (!mAnm[GANBARI_ANIM_LOOP].isEnabled()) {
            mAnm[GANBARI_ANIM_LOOP].setFrame(0.0f);
            mAnm[GANBARI_ANIM_LOOP].setAnimEnable(true);
        }
    } else if (mAnm[GANBARI_ANIM_LOOP].getFrame() < 1.0f) {
        mAnm[GANBARI_ANIM_LOOP].setFrame(0.0f);
        mAnm[GANBARI_ANIM_LOOP].setAnimEnable(false);
    }

    mAnm[GANBARI_ANIM_COLOR].setFrame(field_0x554);
    mAnm[GANBARI_ANIM_COLOR].setAnimEnable(true);

    mAnm[GANBARI_ANIM_DRINK].setFrame(drinkFrame);
    mAnm[GANBARI_ANIM_DRINK].setAnimEnable(true);

    if (!EventManager::isInEvent() && !dAcPy_c::GetLink()->hasvt_0x1C0() &&
        !dAcPy_c::GetLink()->checkActionFlagsCont(0x10) &&
        (d3d::isOnScreen(dAcPy_c::GetLink()->mPositionCopy3) || fn_80104710(false) || isCrawling()) &&
        *mStateMgrWheel.getStateID() != StateID_Full && *mStateMgrMain.getStateID() == StateID_InvisibleWait &&
        !field_0x530) {
        mStateMgrMain.changeState(StateID_In);
    } else if (!EventManager::isInEvent() && *mStateMgrWheel.getStateID() == StateID_Full &&
               *mStateMgrWheel.getOldStateID() != StateID_Full && *mStateMgrMain.getStateID() == StateID_Wait) {
        mStateMgrMain.changeState(StateID_OutWait);
    } else if ((EventManager::isInEvent() || dAcPy_c::GetLink()->hasvt_0x1C0() ||
                dAcPy_c::GetLink()->checkActionFlagsCont(0x10) ||
                (!d3d::isOnScreen(dAcPy_c::GetLink()->mPositionCopy3) && !fn_80104710(false) && !isCrawling())) &&
               *mStateMgrMain.getStateID() == StateID_Wait) {
        mStateMgrMain.changeState(StateID_CameraOut);
    } else if (field_0x530 && *mStateMgrMain.getStateID() == StateID_Wait) {
        mStateMgrMain.changeState(StateID_CameraOut);
    }

    if (field_0x540 > 0) {
        field_0x540--;
    }

    if (field_0x55A != swimmingRelated() && *mStateMgrMain.getStateID() == StateID_Wait) {
        mStateMgrMain.changeState(StateID_Out);
        field_0x55A = swimmingRelated();
    }

    mStateMgrMain.executeState();
    field_0x51C = mStaminaPercent;

    return true;
}

bool dLytMeterGanbariGauge_c::fn_80104710(bool arg) const {
    if (dAcPy_c::GetLink() != nullptr &&
        dAcPy_c::GetLink()->checkActionFlagsCont(0x400 | 0x100 | 0x80 | 0x40 | 0x10 | 0x4 | 0x2 | 0x1) &&
        dAcPy_c::GetLink()->checkActionFlagsCont(0x40)) {
        return true;
    }
    if (!arg && field_0x540 > 0) {
        return true;
    }
    return false;
}

bool dLytMeterGanbariGauge_c::fn_80104760() const {
    const dAcPy_c *link = dAcPy_c::GetLink();
    if (link != nullptr &&
        (link->checkActionFlagsCont(0x2) || link->checkActionFlagsCont(0x80) || link->checkActionFlagsCont(0x4) ||
         link->checkActionFlagsCont(0x10) || link->checkActionFlagsCont(0x100))) {
        return true;
    }
    return false;
}

bool dLytMeterGanbariGauge_c::fn_801047B0() const {
    if (fn_80081FE0(dScGame_c::getCamera(0)->getField_0xD98(), "mogu") ||
        fn_80081FE0(dScGame_c::getCamera(0)->getField_0xD98(), "mogu2")) {
        return true;
    }
    return false;
}

bool dLytMeterGanbariGauge_c::isCrawling() const {
    return dAcPy_c::GetLink()->checkActionFlags(dAcPy_c::FLG0_CRAWLING);
}

bool dLytMeterGanbariGauge_c::swimmingRelated() {
    if (!dAcPy_c::GetLink()->checkFlags0x340(0x100) || dAcPy_c::GetLink()->getCurrentAction() == 0x57 ||
        dAcPy_c::GetLink()->checkActionFlags(dAcPy_c::FLG0_IN_WATER)) {
        return true;
    }
    return false;
}

bool dLytMeterGanbariGauge_c::displayAirInsteadOfStamina() {
    if (field_0x554 == 1.0f) {
        return true;
    }
    if (field_0x554 == 2.0f && swimmingRelated()) {
        return true;
    }
    return false;
}

void dLytMeterGanbariGauge_c::realizeAnimState() {
    mAnm[GANBARI_ANIM_TO_MIN].setToStart();
    mAnm[GANBARI_ANIM_TO_MIN].setAnimEnable(true);

    mAnm[GANBARI_ANIM_TO_MIN_LOOP].setFrame(0.0f);
    mAnm[GANBARI_ANIM_TO_MIN_LOOP].setAnimEnable(true);

    mAnm[GANBARI_ANIM_TO_MAX].setToEnd2();
    mAnm[GANBARI_ANIM_TO_MAX].setAnimEnable(true);

    mAnm[GANBARI_ANIM_CAUTION].setFrame(0.0f);
    mAnm[GANBARI_ANIM_CAUTION].setAnimEnable(true);

    // okay
    mAnm[GANBARI_ANIM_USE].setAnimEnable(false);
    mAnm[GANBARI_ANIM_USE].setAnimEnable(true);

    mAnm[GANBARI_ANIM_UPDOWN_1].setAnimEnable(true);
    mAnm[GANBARI_ANIM_UPDOWN_1].setFrame(0.0f);

    mLyt.calc();

    mAnm[GANBARI_ANIM_TO_MIN].setAnimEnable(false);
    mAnm[GANBARI_ANIM_TO_MIN_LOOP].setAnimEnable(false);
    mAnm[GANBARI_ANIM_TO_MAX].setAnimEnable(false);
    mAnm[GANBARI_ANIM_CAUTION].setAnimEnable(false);
    mAnm[GANBARI_ANIM_USE].setAnimEnable(false);
    mAnm[GANBARI_ANIM_UPDOWN_1].setAnimEnable(false);

    if (dAcPy_c::GetLink() != nullptr) {
        if (dAcPy_c::GetLink()->getRidingActorType() == dAcPy_c::RIDING_BOAT) {
            field_0x554 = 3.0f;
        } else if (swimmingRelated()) {
            field_0x554 = 1.0f;
        } else {
            field_0x554 = 0.0f;
        }
    }
}
