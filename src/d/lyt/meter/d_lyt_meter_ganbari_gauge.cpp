#include "d/lyt/meter/d_lyt_meter_ganbari_gauge.h"

#include "common.h"
#include "d/a/d_a_player.h"
#include "d/lyt/d_lyt_meter_configuration.h"

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
    if (dAcPy_c::GetLink()->getRidingActorType() == 3) {
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
            if (field_0x518 > 0.95f) {
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
    } else if (field_0x518 < 0.9999f) {
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
    if (field_0x518 < 0.9999f) {
        mStateMgrWheel.changeState(StateID_Normal);
    } else if (dAcPy_c::GetLink()->checkSwordAndMoreStates(0x200 | 0x40 | 0x20) || field_0x558 != 0) {
        mStateMgrWheel.changeState(StateID_FullGutsUse);
    }
}
void dLytMeterGanbariGauge_c::finalizeState_Full() {}

void dLytMeterGanbariGauge_c::initializeState_FullGutsUse() {}
void dLytMeterGanbariGauge_c::executeState_FullGutsUse() {
    if (field_0x518 < 0.9999f) {
        mStateMgrWheel.changeState(StateID_Normal);
    } else if (!dAcPy_c::GetLink()->checkSwordAndMoreStates(0x200 | 0x40 | 0x20) && field_0x558 == 0) {
        mStateMgrWheel.changeState(StateID_Full);
    }
}
void dLytMeterGanbariGauge_c::finalizeState_FullGutsUse() {}

void dLytMeterGanbariGauge_c::initializeState_Normal() {}
void dLytMeterGanbariGauge_c::executeState_Normal() {
    if (fn_801034B0(field_0x518)) {
        mAnm[GANBARI_ANIM_CAUTION].setRate(1.0f);
        mStateMgrWheel.changeState(StateID_Caution);
    } else if (fn_80103520(field_0x518)) {
        mAnm[GANBARI_ANIM_CAUTION].setRate(3.0f);
        mStateMgrWheel.changeState(StateID_Caution);
    } else if (field_0x518 >= 0.9999f) {
        mStateMgrWheel.changeState(StateID_ToMax);
    }
}
void dLytMeterGanbariGauge_c::finalizeState_Normal() {}

void dLytMeterGanbariGauge_c::initializeState_Caution() {
    mAnm[GANBARI_ANIM_CAUTION].setAnimEnable(true);
    mAnm[GANBARI_ANIM_CAUTION].setFrame(0.0f);
}
void dLytMeterGanbariGauge_c::executeState_Caution() {
    if (field_0x518 <= 0.0001f) {
        mStateMgrWheel.changeState(StateID_ToMin);
    } else if (fn_80103520(field_0x518)) {
        mAnm[GANBARI_ANIM_CAUTION].setRate(3.0f);
    } else if (fn_801034B0(field_0x518)) {
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
    if (field_0x518 >= 0.9999f) {
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

    if (dAcPy_c::GetLink()->getRidingActorType() == 3) {
        field_0x554 = 4.0f;
    } else if (swimmingRelated()) {
        field_0x554 = 1.0f;
    } else {
        field_0x554 = 2.0f;
    }
}
void dLytMeterGanbariGauge_c::executeState_Recovery() {
    if (swimmingRelated()) {
        if (field_0x518 >= 0.0001f) {
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
    } else if (field_0x518 >= 0.9999f) {
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

    field_0x518 = 1.0f;
    field_0x51C = 1.0f;
    field_0x54C = 1.0f;
    field_0x550 = 1.0f;

    field_0x548 = dLytMeterConfiguration_c::sInstance->field_0x1C8;

    field_0x558 = 0;
    field_0x559 = 0;
    field_0x55B = 0;
    mpPane = nullptr;
    field_0x55A = 0;

    field_0x554 = 0.0f;

    mOutWaitTimer = 0x1E;
    field_0x540 = 0;
    field_0x534 = 0;

    if (dLytMeterConfiguration_c::sInstance->field_0x13C != 0) {
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
    if (mpPane == nullptr) {
        return true;
    }

    if (!mpPane->IsVisible()) {
        if (!(*mStateMgrMain.getStateID() != StateID_InvisibleWait)) {
            return true;
        }
        mStateMgrMain.changeState(StateID_InvisibleWait);
        return true;
    }

    // TODO I really don't like the look of the rest of this function. Depends on a bunch
    // of player, camera and filemanager stuff with a ton of inlines...

    return true;
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
        if (dAcPy_c::GetLink()->getRidingActorType() == 3) {
            field_0x554 = 3.0f;
        } else if (swimmingRelated()) {
            field_0x554 = 1.0f;
        } else {
            field_0x554 = 0.0f;
        }
    }
}
