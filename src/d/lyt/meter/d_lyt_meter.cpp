// clang-format off
#include "c/c_lib.h"
#include "common.h"
#include "d/a/d_a_player.h"
#include "d/d_message.h"
#include "d/d_sc_game.h"
#include "d/d_sc_title.h"
#include "d/d_stage_mgr.h"
#include "d/flag/storyflag_manager.h"
#include "d/lyt/d_lyt_area_caption.h"
#include "d/lyt/d_lyt_control_game.h"
#include "d/lyt/d_lyt_meter_configuration.h"
#include "d/lyt/d_lyt_unknowns.h"
#include "d/lyt/d_window.h"
#include "d/lyt/meter/d_lyt_meter.h"
#include "d/lyt/msg_window/d_lyt_msg_window.h"
#include "m/m_vec.h"
#include "nw4r/lyt/lyt_group.h"
#include "toBeSorted/arc_managers/layout_arc_manager.h"
#include "toBeSorted/event_manager.h"
#include "toBeSorted/small_sound_mgr.h"
// clang-format on

static dLytMeterConfiguration_c sConf;

STATE_DEFINE(dLytMeter1Button_c, Wait);
STATE_DEFINE(dLytMeter1Button_c, ToUse);
STATE_DEFINE(dLytMeter1Button_c, ToUnuse);
STATE_DEFINE(dLytMeter1Button_c, Unuse);

STATE_DEFINE(dLytMeter2Button_c, Wait);
STATE_DEFINE(dLytMeter2Button_c, ToUse);
STATE_DEFINE(dLytMeter2Button_c, ToUnuse);
STATE_DEFINE(dLytMeter2Button_c, Unuse);

STATE_DEFINE(dLytMeterParts_c, Invisible);
STATE_DEFINE(dLytMeterParts_c, In);
STATE_DEFINE(dLytMeterParts_c, Visible);
STATE_DEFINE(dLytMeterParts_c, Out);

void dLytMeter1Button_c::initializeState_Wait() {}
void dLytMeter1Button_c::executeState_Wait() {
    if (dLytMeterContainer_c::GetInstance()->checkAllFlags(0x80)) {
        return;
    }
    mStateMgr.changeState(StateID_ToUnuse);
}
void dLytMeter1Button_c::finalizeState_Wait() {}

void dLytMeter1Button_c::initializeState_ToUse() {
    mAnm[0].setBackwardsOnce();
    mAnm[0].setToStart();
    mAnm[0].setAnimEnable(true);
}
void dLytMeter1Button_c::executeState_ToUse() {
    if (mAnm[0].isStop2()) {
        mStateMgr.changeState(StateID_Wait);
    }

    if (mAnm[0].isEnabled()) {
        mAnm[0].play();
    }
}
void dLytMeter1Button_c::finalizeState_ToUse() {
    mAnm[0].setAnimEnable(false);
}

void dLytMeter1Button_c::initializeState_ToUnuse() {
    mAnm[0].setForwardOnce();
    mAnm[0].setToStart();
    mAnm[0].setAnimEnable(true);
}
void dLytMeter1Button_c::executeState_ToUnuse() {
    if (mAnm[0].isStop2()) {
        mStateMgr.changeState(StateID_Unuse);
    }

    if (mAnm[0].isEnabled()) {
        mAnm[0].play();
    }
}
void dLytMeter1Button_c::finalizeState_ToUnuse() {
    mAnm[0].setAnimEnable(false);
}

void dLytMeter1Button_c::initializeState_Unuse() {
    mUnuseDelay = 15;
}
void dLytMeter1Button_c::executeState_Unuse() {
    if (dLytMeterContainer_c::GetInstance()->checkAllFlags(0x80)) {
        if (!(mUnuseDelay > 0)) {
            mStateMgr.changeState(StateID_ToUse);
            return;
        }
        mUnuseDelay -= 1;
    } else {
        mUnuseDelay = 15;
    }
}
void dLytMeter1Button_c::finalizeState_Unuse() {}

static const d2d::LytBrlanMapping btn1BrlanMap[] = {
    {"remoConBtn_04_input.brlan", "G_input_00"},
    { "remoConBtn_04_call.brlan",  "G_call_00"},
    { "remoConBtn_04_loop.brlan",  "G_loop_00"},
};

#define BUTTON_1_ANIM_INPUT 0
#define BUTTON_1_ANIM_CALL 1
#define BUTTON_1_ANIM_LOOP 2

#define BUTTON_1_NUM_ANIMS 3

static char *sWindowName1[] = {"W_bgP_00"};

bool dLytMeter1Button_c::build(d2d::ResAccIf_c *resAcc) {
    mLyt.setResAcc(resAcc);
    mLyt.build("remoConBtn_04.brlyt", nullptr);

    for (int i = 0; i < BUTTON_1_NUM_ANIMS; i++) {
        mAnm[i].init(btn1BrlanMap[i].mFile, resAcc, mLyt.getLayout(), btn1BrlanMap[i].mName);
        mAnm[i].bind(false);
        mAnm[i].setAnimEnable(false);
    }

    mLyt.fn_800AB9A0(mLyt.getTextBox("T_sabBtnS_00"), 0);
    mLyt.fn_800AB9A0(mLyt.getTextBox("T_sabBtn_00"), 0);
    for (int i = 0; i < 1; i++) {
        mpWindow[i] = mLyt.getWindow(sWindowName1[i]);
        mpSizeBox[i] = mLyt.getSizeBoxInWindow(sWindowName1[i]);
        mpWindow[i]->UpdateSize(mpSizeBox[i], 32.0f);
    }
    mUnuseDelay = 15;
    mShouldCall = false;
    mHasInitedCall = false;
    mCallCount = false;
    mAnm[BUTTON_1_ANIM_CALL].setToEnd();
    mAnm[BUTTON_1_ANIM_CALL].setAnimEnable(true);
    mAnm[BUTTON_1_ANIM_LOOP].setAnimEnable(true);
    mLyt.calc();
    mAnm[BUTTON_1_ANIM_CALL].setAnimEnable(false);
    mStateMgr.changeState(StateID_Wait);
    return true;
}

bool dLytMeter1Button_c::remove() {
    for (int i = 0; i < BUTTON_1_NUM_ANIMS; i++) {
        mAnm[i].unbind();
        mAnm[i].remove();
    }
    return true;
}

bool dLytMeter1Button_c::execute() {
    dLytMeterContainer_c *container = dLytMeterContainer_c::GetInstance();
    if (mpOwnerPane == nullptr) {
        return true;
    }

    if (!mpOwnerPane->IsVisible()) {
        if (*mStateMgr.getStateID() == StateID_ToUnuse) {
            mAnm[BUTTON_1_ANIM_INPUT].setToEnd2();
        }
        return true;
    }

    mStateMgr.executeState();

    if (dLytControlGame_c::getInstance()->isStateNormal()) {
        if (StoryflagManager::sInstance->getCounterOrFlag(571)) {
            mShouldCall = true;
        }
    } else {
        mShouldCall = false;
    }

    if (mShouldCall && container->checkAllFlags(0x80)) {
        if (!mHasInitedCall) {
            mAnm[BUTTON_1_ANIM_CALL].setToEnd();
            mAnm[BUTTON_1_ANIM_CALL].setAnimEnable(true);
            mHasInitedCall = true;
        }

        if (mAnm[BUTTON_1_ANIM_CALL].isEnabled()) {
            mAnm[BUTTON_1_ANIM_CALL].play();
            if (container->checkAllFlags(0x80) && mCallCount < 3 && mAnm[BUTTON_1_ANIM_CALL].getFrame() == 1.0f) {
                SmallSoundManager::GetInstance()->playSound(SE_S_1_BUTTON_BLINK);
                mCallCount++;
            }
        }
    } else {
        if (mHasInitedCall == true) {
            mAnm[BUTTON_1_ANIM_CALL].setToEnd();
        } else if (mAnm[BUTTON_1_ANIM_CALL].isEnabled()) {
            mAnm[BUTTON_1_ANIM_CALL].setAnimEnable(false);
        }
    }

    if (!mShouldCall) {
        mHasInitedCall = mShouldCall;
    }

    mShouldCall = false;
    if (mAnm[BUTTON_1_ANIM_LOOP].isEnabled()) {
        mAnm[BUTTON_1_ANIM_LOOP].play();
    }

    return true;
}

bool dLytMeter1Button_c::shouldCall() const {
    if (dLytControlGame_c::getInstance()->isStateNormal() && StoryflagManager::sInstance->getCounterOrFlag(571)) {
        return true;
    }
    return false;
}

void dLytMeter2Button_c::initializeState_Wait() {}
void dLytMeter2Button_c::executeState_Wait() {
    if (dLytMeterContainer_c::GetInstance()->checkAllFlags(0x100)) {
        return;
    }
    mStateMgr.changeState(StateID_ToUnuse);
}
void dLytMeter2Button_c::finalizeState_Wait() {}

void dLytMeter2Button_c::initializeState_ToUse() {
    mAnm[0].setBackwardsOnce();
    mAnm[0].setToStart();
    mAnm[0].setAnimEnable(true);
}
void dLytMeter2Button_c::executeState_ToUse() {
    if (mAnm[0].isStop2()) {
        mStateMgr.changeState(StateID_Wait);
    }

    if (mAnm[0].isEnabled()) {
        mAnm[0].play();
    }
}
void dLytMeter2Button_c::finalizeState_ToUse() {
    mAnm[0].setAnimEnable(false);
}

void dLytMeter2Button_c::initializeState_ToUnuse() {
    mAnm[0].setForwardOnce();
    mAnm[0].setToStart();
    mAnm[0].setAnimEnable(true);
}
void dLytMeter2Button_c::executeState_ToUnuse() {
    if (mAnm[0].isStop2()) {
        mStateMgr.changeState(StateID_Unuse);
    }

    if (mAnm[0].isEnabled()) {
        mAnm[0].play();
    }
}
void dLytMeter2Button_c::finalizeState_ToUnuse() {
    mAnm[0].setAnimEnable(false);
}

void dLytMeter2Button_c::initializeState_Unuse() {
    mUnuseDelay = 15;
}
void dLytMeter2Button_c::executeState_Unuse() {
    if (dLytMeterContainer_c::GetInstance()->checkAllFlags(0x100)) {
        if (!(mUnuseDelay > 0)) {
            mStateMgr.changeState(StateID_ToUse);
            return;
        }
        mUnuseDelay -= 1;
    } else {
        mUnuseDelay = 15;
    }
}
void dLytMeter2Button_c::finalizeState_Unuse() {}

static const d2d::LytBrlanMapping btn2BrlanMap[] = {
    {"remoConBtn_05_input.brlan", "G_input_00"},
    { "remoConBtn_05_call.brlan",  "G_call_00"},
    { "remoConBtn_05_loop.brlan",  "G_loop_00"},
};

#define BUTTON_2_ANIM_INPUT 0
#define BUTTON_2_ANIM_CALL 1
#define BUTTON_2_ANIM_LOOP 2

#define BUTTON_2_NUM_ANIMS 3

static char *sWindowName2[] = {"W_bgP_00"};

bool dLytMeter2Button_c::build(d2d::ResAccIf_c *resAcc) {
    mLyt.setResAcc(resAcc);
    mLyt.build("remoConBtn_05.brlyt", nullptr);

    for (int i = 0; i < BUTTON_2_NUM_ANIMS; i++) {
        mAnm[i].init(btn2BrlanMap[i].mFile, resAcc, mLyt.getLayout(), btn2BrlanMap[i].mName);
        mAnm[i].bind(false);
        mAnm[i].setAnimEnable(false);
    }
    for (int i = 0; i < 1; i++) {
        mpWindow[i] = mLyt.getWindow(sWindowName2[i]);
        mpSizeBox[i] = mLyt.getSizeBoxInWindow(sWindowName2[i]);
        mpWindow[i]->UpdateSize(mpSizeBox[i], 32.0f);
    }
    mUnuseDelay = 15;
    mShouldCall = 0;
    mHasInitedCall = 0;
    mCallCount = 0;
    mAnm[BUTTON_2_ANIM_CALL].setToEnd();
    mAnm[BUTTON_2_ANIM_CALL].setAnimEnable(true);
    mAnm[BUTTON_2_ANIM_LOOP].setAnimEnable(true);
    mLyt.calc();
    mAnm[BUTTON_2_ANIM_CALL].setAnimEnable(false);
    mStateMgr.changeState(StateID_Wait);
    return true;
}

bool dLytMeter2Button_c::remove() {
    for (int i = 0; i < 3; i++) {
        mAnm[i].unbind();
        mAnm[i].remove();
    }
    return true;
}

bool dLytMeter2Button_c::execute() {
    dLytMeterContainer_c *container = dLytMeterContainer_c::GetInstance();
    if (container->getMeterField_0x13774()) {
        StoryflagManager::sInstance->unsetFlag(832);
    }
    if (mpOwnerPane == nullptr) {
        return true;
    }

    if (!mpOwnerPane->IsVisible()) {
        if (*mStateMgr.getStateID() == StateID_ToUnuse) {
            mAnm[BUTTON_1_ANIM_INPUT].setToEnd2();
        }
        return true;
    }

    mStateMgr.executeState();

    if (dLytControlGame_c::getInstance()->isStateNormal()) {
        if (StoryflagManager::sInstance->getCounterOrFlag(832)) {
            mShouldCall = true;
        }
    } else {
        mShouldCall = false;
    }

    if (mShouldCall && container->checkAllFlags(0x100)) {
        if (!mHasInitedCall) {
            mAnm[BUTTON_1_ANIM_CALL].setToEnd();
            mAnm[BUTTON_1_ANIM_CALL].setAnimEnable(true);
            mHasInitedCall = true;
        }

        if (mAnm[BUTTON_1_ANIM_CALL].isEnabled()) {
            mAnm[BUTTON_1_ANIM_CALL].play();
            if (container->checkAllFlags(0x100) && mCallCount < 3 && mAnm[BUTTON_1_ANIM_CALL].getFrame() == 1.0f) {
                SmallSoundManager::GetInstance()->playSound(SE_S_2_BUTTON_BLINK);
                mCallCount++;
            }
        }
    } else {
        if (mHasInitedCall == true) {
            mAnm[BUTTON_1_ANIM_CALL].setToEnd();
        } else if (mAnm[BUTTON_1_ANIM_CALL].isEnabled()) {
            mAnm[BUTTON_1_ANIM_CALL].setAnimEnable(false);
        }
    }

    if (!mShouldCall) {
        mHasInitedCall = mShouldCall;
    }

    mShouldCall = false;
    if (mAnm[BUTTON_1_ANIM_LOOP].isEnabled()) {
        mAnm[BUTTON_1_ANIM_LOOP].play();
    }

    return true;
}

bool dLytMeter2Button_c::shouldCall() const {
    if (dLytControlGame_c::getInstance()->isStateNormal() && StoryflagManager::sInstance->getCounterOrFlag(832)) {
        return true;
    }
    return false;
}

void dLytMeterParts_c::initializeState_Invisible() {}
void dLytMeterParts_c::executeState_Invisible() {
    if (mShouldBeVisible) {
        if (mIndex == METER_SHIELD) {
            mpAnm1->setForwardOnce();
            if (dMessage_c::getInstance()->getField_0x2FC() != 0) {
                mpAnm1->setToEnd();
                dLytMeterContainer_c::setRupyField_0x8A9(1);
            } else {
                mpAnm1->setFrame(0.0f);
                dLytMeterContainer_c::setRupyField_0x8A9(2);
            }
            mpAnm1->setAnimEnable(true);
        } else {
            mpAnm1->setForwardOnce();
            mpAnm1->setFrame(0.0f);
            mpAnm1->setAnimEnable(true);
        }
        mStateMgr.changeState(StateID_In);
    }
}
void dLytMeterParts_c::finalizeState_Invisible() {}

void dLytMeterParts_c::initializeState_In() {}
void dLytMeterParts_c::executeState_In() {
    if (mpAnm1->isEndReached()) {
        mpAnm1->setAnimEnable(false);
        mStateMgr.changeState(StateID_Visible);
    }

    if (mpAnm1->isEnabled()) {
        mpAnm1->play();
    }
}
void dLytMeterParts_c::finalizeState_In() {}

void dLytMeterParts_c::initializeState_Visible() {}
void dLytMeterParts_c::executeState_Visible() {
    if (!mShouldBeVisible) {
        if (mIndex == METER_SHIELD) {
            mpAnm2->setForwardOnce();
            if (dLytMeterContainer_c::getRupyField_0x8AC()) {
                mpAnm2->setToEnd();
                if (dLytMeterContainer_c::getHeartField_0x78C() == 0) {
                    dLytMeterContainer_c::setRupyField_0x8AA(1);
                } else {
                    dLytMeterContainer_c::setRupyField_0x8AA(3);
                }
                dLytMeterContainer_c::setRupyField_0x8AC(0);
            } else {
                mpAnm2->setFrame(0.0f);
                if (dLytMeterContainer_c::getHeartField_0x78C() == 0) {
                    dLytMeterContainer_c::setRupyField_0x8AA(2);
                } else {
                    dLytMeterContainer_c::setRupyField_0x8AA(4);
                }
            }
            mpAnm2->setAnimEnable(true);
        } else {
            mpAnm2->setForwardOnce();
            mpAnm2->setFrame(0.0f);
            mpAnm2->setAnimEnable(true);
        }
        mStateMgr.changeState(StateID_Out);
    }
}
void dLytMeterParts_c::finalizeState_Visible() {}

void dLytMeterParts_c::initializeState_Out() {}
void dLytMeterParts_c::executeState_Out() {
    dLytMeterContainer_c *container = dLytMeterContainer_c::GetInstance();
    if (mpAnm2->isEndReached()) {
        if (mIndex == METER_SHIELD && dMessage_c::getInstance()->getField_0x2FC() == -2) {
            dMessage_c::getInstance()->setField_0x2FC(0);
        }
        mpAnm2->setAnimEnable(false);
        mStateMgr.changeState(StateID_Invisible);
    }

    if (mpAnm2->isEnabled()) {
        mpAnm2->play();
    }

    switch (mIndex) {
        case 1:  container->clearFlags(0x40); break;
        case 4:  container->clearFlags(0x80); break;
        case 3:  container->clearFlags(0x1 | 0x2 | 0x4 | 0x8); break;
        case 5:  container->clearFlags(0x100); break;
        case 6:  container->clearFlags(0x200); break;
        case 8:  container->clearFlags(0x400); break;
        case 9:  container->clearFlags(0x1000); break;
        case 10: container->clearFlags(0x200); break;
    }
}
void dLytMeterParts_c::finalizeState_Out() {}

void dLytMeterParts_c::build(s32 index) {
    mIndex = index;
    mShouldBeVisible = false;
    mStateMgr.changeState(StateID_Invisible);
}

void dLytMeterParts_c::execute() {
    mStateMgr.executeState();
}

bool dLytMeter_c::isNotSilentRealmOrLoftwing() {
    if (isSilentRealm() || dAcPy_c::GetLink()->getRidingActorType() == dAcPy_c::RIDING_LOFTWING) {
        return false;
    }
    return true;
}

bool dLytMeter_c::fn_800C9F70() {
    if ((fn_800D56B0() && !field_0x13775) || fn_800D5650() || fn_800D5680()) {
        return true;
    }
    return false;
}

bool dLytMeter_c::fn_800C9FE0() {
    if (fn_800D56B0() || !field_0x13750 || fn_800D5680()) {
        return true;
    }
    return false;
}

bool dLytMeter_c::fn_800CA040() {
    if (field_0x13774 || fn_800D5650() || fn_800D5680()) {
        return true;
    }
    return false;
}

void floats() {
    32.0f;
    1.0f;
    0.0f;
    int x = 0;
    f32 f = x;
}

void floats2() {
    5.0f;
    3.0f;
}

void dLytMeter_c::setUiMode(u16 value) const {
    StoryflagManager::sInstance->setFlagOrCounterToValue(840, value);
}

u8 dLytMeter_c::getUiMode() {
    return StoryflagManager::sInstance->getFlag(840);
}

#pragma dont_inline on
dLytMeter_c::dLytMeter_c() {}
#pragma dont_inline reset

extern "C" void fn_800D97E0(int i);
extern "C" void fn_800D9800(int i);

static const d2d::LytBrlanMapping meterBrlanMap[] = {
    {          "basicPosition_00_in.brlan", "G_remoConBtn_00"},
    {          "basicPosition_00_in.brlan", "G_remoConBtn_01"},
    {          "basicPosition_00_in.brlan", "G_remoConBtn_02"},
    {          "basicPosition_00_in.brlan", "G_remoConBtn_03"},
    {          "basicPosition_00_in.brlan", "G_remoConBtn_04"},
    {          "basicPosition_00_in.brlan", "G_remoConBtn_05"},
    {          "basicPosition_00_in.brlan", "G_remoConBtn_06"},
    {          "basicPosition_00_in.brlan",  "G_remoConBg_00"},
    {          "basicPosition_00_in.brlan",     "G_nunBtn_03"},
    {          "basicPosition_00_in.brlan",     "G_nunBtn_01"},
    {          "basicPosition_00_in.brlan",     "G_nunBtn_02"},
    {          "basicPosition_00_in.brlan",      "G_nunBg_00"},
    {          "basicPosition_00_in.brlan",     "G_shield_00"},
    {          "basicPosition_00_in.brlan",       "G_rupy_00"},
    {          "basicPosition_00_in.brlan",      "G_heart_00"},
    {          "basicPosition_00_in.brlan",    "G_ganbari_00"},

    {         "basicPosition_00_out.brlan", "G_remoConBtn_00"},
    {         "basicPosition_00_out.brlan", "G_remoConBtn_01"},
    {         "basicPosition_00_out.brlan", "G_remoConBtn_02"},
    {         "basicPosition_00_out.brlan", "G_remoConBtn_03"},
    {         "basicPosition_00_out.brlan", "G_remoConBtn_04"},
    {         "basicPosition_00_out.brlan", "G_remoConBtn_05"},
    {         "basicPosition_00_out.brlan", "G_remoConBtn_06"},
    {         "basicPosition_00_out.brlan",  "G_remoConBg_00"},
    {         "basicPosition_00_out.brlan",     "G_nunBtn_03"},
    {         "basicPosition_00_out.brlan",     "G_nunBtn_01"},
    {         "basicPosition_00_out.brlan",     "G_nunBtn_02"},
    {         "basicPosition_00_out.brlan",      "G_nunBg_00"},
    {         "basicPosition_00_out.brlan",     "G_shield_00"},
    {         "basicPosition_00_out.brlan",       "G_rupy_00"},
    {         "basicPosition_00_out.brlan",      "G_heart_00"},
    {         "basicPosition_00_out.brlan",    "G_ganbari_00"},

    {    "basicPosition_00_position.brlan",   "G_position_00"},
    {"basicPosition_00_rupyPosition.brlan",   "G_rupyPosi_00"},
};

#define METER_ANIM_POSITION_IN_OFFSET 0
#define METER_ANIM_POSITION_OUT_OFFSET 16
#define METER_ANIM_POSITION 32
#define METER_ANIM_RUPY_POSITION 33

#define METER_NUM_ANIMS 34

#define METER_NUM_PANES 16

static const char *sGroupName = "G_ref_00";

bool dLytMeter_c::build(d2d::ResAccIf_c *resAcc) {
    mLyt.setResAcc(resAcc);
    mLyt.build("basicPosition_00.brlyt", nullptr);
    mLyt.setPriority(0x8A);

    for (int i = 0; i < METER_NUM_ANIMS; i++) {
        mAnmGroups[i].init(meterBrlanMap[i].mFile, resAcc, mLyt.getLayout(), meterBrlanMap[i].mName);
        mAnmGroups[i].bind(false);
        mAnmGroups[i].setAnimEnable(false);
    }

    // in G_remoConBtn_06, out G_remoConBtn_06
    mABtn.setContainerGroups(&mAnmGroups[6], &mAnmGroups[22]);
    // out G_shield_00
    mShield.setContainerGroup(&mAnmGroups[28]);

    if (dScTitle_c::sInstance == nullptr) {
        mp1Button = new dLytMeter1Button_c();
        mp2Button = new dLytMeter2Button_c();
    } else {
        mp1Button = nullptr;
        mp2Button = nullptr;
    }

    mNodes[METER_GANBARI].mpLytPane = &mGanbariGauge;
    mNodes[METER_RUPY].mpLytPane = &mRupy;
    mNodes[METER_ITEM_SELECT].mpLytPane = &mItemSelect;
    mNodes[METER_CROSS_BTN].mpLytPane = &mCrossBtn;
    mNodes[METER_PLUS_BTN].mpLytPane = &mPlusBtn;
    mNodes[METER_MINUS_BTN].mpLytPane = &mMinusBtn;
    mNodes[METER_A_BTN].mpLytPane = &mABtn;
    mNodes[METER_DOWSING].mpLytPane = &mDowsing;
    mNodes[METER_Z_BTN].mpLytPane = &mZBtn;
    mNodes[METER_NUN_STK].mpLytPane = &mNunStk;
    mNodes[METER_REMOCON_BG].mpLytPane = &mRemoCon;
    mNodes[METER_NUN_BG].mpLytPane = &mNunBg;
    mNodes[METER_HEART].mpLytPane = &mHeart;
    mNodes[METER_SHIELD].mpLytPane = &mShield;
    mNodes[METER_1_BTN].mpLytPane = mp1Button;
    mNodes[METER_2_BTN].mpLytPane = mp2Button;

    mShieldPositions[0].copyFrom(mLyt.findPane("N_shield_00")->GetTranslate());
    mShieldPositions[1].copyFrom(mLyt.findPane("N_shield_01")->GetTranslate());
    mShieldPosIndex = 0;
    mOldShieldPosIndex = 0;
    mShieldPosInterpFrame = 0;
    mShieldPos = mShieldPositions[mShieldPosIndex];

    mLyt.findPane("N_rupyAll_00")->SetVisible(true);
    mAnmGroups[METER_ANIM_RUPY_POSITION].setAnimEnable(true);

    // Advance through some anim's keyframes and store positions in mRupyPositions
    for (int i = 0; i < 7; i++) {
        mAnmGroups[METER_ANIM_RUPY_POSITION].setFrame(i);
        mLyt.calc();
        mRupyPositions[i].copyFrom(mLyt.findPane("N_rupy_00")->GetTranslate());
    }

    mAnmGroups[METER_ANIM_RUPY_POSITION].setAnimEnable(false);
    mLyt.findPane("N_rupyAll_00")->SetVisible(false);

    mRupyPosIndex = 0;
    mOldRupyPosIndex = 0;
    mRupyPosInterpFrame = 0;
    mRupyPos = mRupyPositions[mRupyPosIndex];
    mPos3.x = mPos3.y = mPos3.z = 0.0f;
    field_0x137C0 = 0x3C;

    for (int i = 0; i < METER_NUM_PANES; i++) {
        if (mNodes[i].mpLytPane != nullptr) {
            mNodes[i].mpLytPane->build(resAcc);
            mMeters.PushBack(&mNodes[i]);
        }
    }

    if (isSilentRealm()) {
        void *sirenData = LayoutArcManager::GetInstance()->getLoadedData("Siren");
        mResAcc.attach(sirenData, "");
        mpTimer = new dLytMeterTimer_c();
        mpTimerPart1 = new LytMeterTimerPart1_c();
        mpTimerPart2 = new LytMeterTimerPart2_c();

        mpTimerPart1->build(&mResAcc);
        mpTimerPart2->build(&mResAcc);
        mpTimer->build();
    } else {
        mpTimer = nullptr;
        mpTimerPart1 = nullptr;
        mpTimerPart2 = nullptr;
    }

    if (dScTitle_c::sInstance == nullptr && dStageMgr_c::GetInstance()->isAreaTypeSky()) {
        mpSkyGauge = new dLytSkyGauge_c();
        mpSkyGauge->build(resAcc);

        mpBirdGauge = new dLytBirdGauge_c();
        mpBirdGauge->build(resAcc);
    } else {
        mpSkyGauge = nullptr;
        mpBirdGauge = nullptr;
    }

    if (dScGame_c::currentSpawnInfo.stageName == "F401") {
        mpBossGauge = new dLytBossGauge_c();
        mpBossGauge->build();
    } else {
        mpBossGauge = nullptr;
    }

    if (dScGame_c::isCurrentStage("F200") || dScGame_c::isCurrentStage("F210") || dScGame_c::isCurrentStage("F211")) {
        mpKakeraKey = new dLytMeterKakeraKey_c();
        mpKakeraKey->build(resAcc);
    } else {
        mpKakeraKey = nullptr;
    }

    if (dStageMgr_c::GetInstance()->isAreaTypeDungeon() && !(dScGame_c::currentSpawnInfo.stageName == "F100_1") &&
        !(dScGame_c::currentSpawnInfo.stageName == "F103_1")) {
        mpBossKey = new dLytMeterBossKey_c();
        mpBossKey->build(resAcc);
    } else {
        mpBossKey = nullptr;
    }

    if (dStageMgr_c::GetInstance()->isAreaTypeDungeon() && (!(dScGame_c::currentSpawnInfo.stageName == "F100_1") &&
                                                            !(dScGame_c::currentSpawnInfo.stageName == "F103_1")) ||
        dScGame_c::currentSpawnInfo.stageName == "F302" || dScGame_c::currentSpawnInfo.stageName == "F303") {
        mpSmallKey = new dLytMeterSmallKey_c();
        mpSmallKey->build(resAcc);
    } else {
        mpSmallKey = nullptr;
    }

    if (dScTitle_c::sInstance != nullptr) {
        mpDrink = nullptr;
    } else {
        mpDrink = new dLytMeterDrink_c();
        mpDrink->build(resAcc);
    }

    mItemSelect.setOwnerPane(mLyt.findPane("N_remoConBtn_00"));
    mCrossBtn.setOwnerPane(mLyt.findPane("N_remoConBtn_03"));
    mPlusBtn.setOwnerPane(mLyt.findPane("N_remoConBtn_02"));
    mMinusBtn.setOwnerPane(mLyt.findPane("N_remoConBtn_01"));
    mDowsing.setOwnerPane(mLyt.findPane("N_nunBtn_03"));
    mABtn.setOwnerPane(mLyt.findPane("N_remoConBtn_06"));
    mZBtn.setOwnerPane(mLyt.findPane("N_nunBtn_01"));
    mNunStk.setOwnerPane(mLyt.findPane("N_nunBtn_02"));
    mNunBg.setOwnerPane(mLyt.findPane("N_nunBg_00"));
    if (mp1Button != nullptr) {
        mp1Button->setOwnerPane(mLyt.findPane("N_remoConBtn_04"));
    }
    if (mp2Button != nullptr) {
        mp2Button->setOwnerPane(mLyt.findPane("N_remoConBtn_05"));
    }

    mGanbariGauge.setOwnerPane(mLyt.findPane("N_gutsAlpha"));
    mShield.setOwnerPane(mLyt.findPane("N_shieldAll_00"));

    if (mLyt.getLayout()->GetGroupContainer() != nullptr) {
        nw4r::lyt::Group *g = mLyt.getLayout()->GetGroupContainer()->FindGroupByName(sGroupName);
        if (g != nullptr) {
            d2d::dSubPane::linkMeters(g, &mMeters);
        }
    }

    for (int i = 0; i < METER_NUM_PANES; i++) {
        if (mNodes[i].mpLytPane != nullptr) {
            mNodes[i].mpPane->SetVisible(false);
        }
    }

    mNodes[METER_GANBARI].mpPane->SetVisible(true);
    mNodes[METER_HEART].mpPane->SetVisible(true);
    mNodes[METER_RUPY].mpPane->SetVisible(true);
    mNodes[METER_ITEM_SELECT].mpPane->SetVisible(true);
    mNodes[METER_CROSS_BTN].mpPane->SetVisible(true);
    mNodes[METER_PLUS_BTN].mpPane->SetVisible(true);
    mNodes[METER_MINUS_BTN].mpPane->SetVisible(true);
    mNodes[METER_A_BTN].mpPane->SetVisible(true);
    mNodes[METER_DOWSING].mpPane->SetVisible(true);
    mNodes[METER_Z_BTN].mpPane->SetVisible(true);
    mNodes[METER_NUN_STK].mpPane->SetVisible(true);
    mNodes[METER_REMOCON_BG].mpPane->SetVisible(true);
    mNodes[METER_NUN_BG].mpPane->SetVisible(true);
    mNodes[METER_SHIELD].mpPane->SetVisible(true);

    if (mNodes[METER_1_BTN].mpPane != nullptr) {
        mNodes[METER_1_BTN].mpPane->SetVisible(true);
    }

    if (mNodes[METER_2_BTN].mpPane != nullptr) {
        mNodes[METER_2_BTN].mpPane->SetVisible(true);
    }

    mNodes[METER_SHIELD].mpPane->SetTranslate(mShieldPos);
    mNodes[METER_RUPY].mpPane->SetTranslate(mRupyPos);

    for (int i = 0; i < METER_NUM_PANES; i++) {
        mAnmGroups[i + METER_ANIM_POSITION_IN_OFFSET].setAnimEnable(true);
        mAnmGroups[i + METER_ANIM_POSITION_IN_OFFSET].setToStart();
    }
    s32 zero = 0;

    field_0x1374C = 0;
    field_0x13748 = 0;
    field_0x13750 = 4;
    field_0x13770 = 3;
    field_0x13774 = 0;
    field_0x13775 = 0;
    field_0x13773 = 0;
    field_0x13776 = 0;
    field_0x13777 = 0;
    field_0x13780 = 1;
    field_0x13781 = 0;
    field_0x13778 = 0;
    field_0x13779 = 0;
    field_0x1377A = 0;
    field_0x1377B = 0;
    field_0x1377C = 0;
    field_0x1377D = 0;
    field_0x1377E = 0;
    field_0x1377F = 0;
    field_0x13754 = 0;

    mAnmGroups[METER_ANIM_POSITION].setFrame(zero);
    mAnmGroups[METER_ANIM_POSITION].setAnimEnable(true);

    mLyt.calc();

    for (int i = 0; i < METER_NUM_PANES; i++) {
        mAnmGroups[i + METER_ANIM_POSITION_IN_OFFSET].setAnimEnable(false);
    }

    mAnmGroups[METER_ANIM_POSITION].setAnimEnable(false);
    field_0x13771 = 0;

    for (int i = 0; i < METER_NUM_PANES; i++) {
        mParts[i].setAnmGroups(
            &mAnmGroups[i + METER_ANIM_POSITION_IN_OFFSET], &mAnmGroups[i + METER_ANIM_POSITION_OUT_OFFSET]
        );
        mParts[i].build(i);
        field_0x13782[i] = 0;
        field_0x13792[i] = 1;
        field_0x137A2[i] = 0;
    }

    field_0x137B2 = 1;

    return true;
}

bool dLytMeter_c::remove() {
    for (int i = 0; i < METER_NUM_ANIMS; i++) {
        mAnmGroups[i].remove();
    }

    for (d2d::SubPaneList::Iterator it = mMeters.GetBeginIter(); it != mMeters.GetEndIter(); ++it) {
        d2d::dSubPane *m = it->mpLytPane;
        if (m != nullptr) {
            m->getPane()->GetParent()->RemoveChild(m->getPane());
        }
    }

    for (int i = 0; i < METER_NUM_PANES; i++) {
        if (mNodes[i].mpLytPane != nullptr) {
            mNodes[i].mpLytPane->remove();
        }
    }

    if (mp1Button != nullptr) {
        delete mp1Button;
        mp1Button = nullptr;
    }

    if (mp2Button != nullptr) {
        delete mp2Button;
        mp2Button = nullptr;
    }

    if (mpTimerPart1 != nullptr) {
        mpTimerPart1->remove();
        delete mpTimerPart1;
        mpTimerPart1 = nullptr;
    }

    if (mpTimerPart2 != nullptr) {
        mpTimerPart2->remove();
        delete mpTimerPart2;
        mpTimerPart2 = nullptr;
    }

    if (mpTimer != nullptr) {
        mpTimer->remove();
        delete mpTimer;
        mpTimer = nullptr;
        // WHY IS THIS HERE???
        mResAcc.detach();
    }

    if (mpSkyGauge != nullptr) {
        mpSkyGauge->remove();
        delete mpSkyGauge;
        mpSkyGauge = nullptr;
    }

    if (mpBirdGauge != nullptr) {
        mpBirdGauge->remove();
        delete mpBirdGauge;
        mpBirdGauge = nullptr;
    }

    if (mpBossGauge != nullptr) {
        mpBossGauge->remove();
        delete mpBossGauge;
        mpBossGauge = nullptr;
    }

    if (mpKakeraKey != nullptr) {
        mpKakeraKey->remove();
        delete mpKakeraKey;
        mpKakeraKey = nullptr;
    }

    if (mpBossKey != nullptr) {
        mpBossKey->remove();
        delete mpBossKey;
        mpBossKey = nullptr;
    }

    if (mpSmallKey != nullptr) {
        mpSmallKey->remove();
        delete mpSmallKey;
        mpSmallKey = nullptr;
    }

    if (mpDrink != nullptr) {
        mpDrink->remove();
        delete mpDrink;
        mpDrink = nullptr;
    }

    return true;
}

bool dLytMeter_c::execute() {
    dLytMeterContainer_c *container = dLytMeterContainer_c::GetInstance();
    if (field_0x13775 && !EventManager::isInEvent()) {
        field_0x13775 = 0;
    }

    if (isSilentRealm()) {
        container->clearFlags(0x40);
        container->clearFlags(0xF);
    }

    if (dStageMgr_c::GetInstance()->isInLastBoss()) {
        container->clearFlags(0x20);
    }

    if (field_0x13750 != 0 && field_0x13748 == 1) {
        fn_800D5290();
    }

    fn_800D57B0();

    s32 heartsHeight;
    s32 rupeePos;
    s32 kakeraKeyPos;
    s32 bossKeyPos;
    s32 smallKeyPos;

    if (mHeart.getCurrentHealthCapacity() / 4 <= 10) {
        heartsHeight = 0;
        if (mShield.getGaugePercentMaybe() > 0.0f &&
            (mLyt.findPane("N_shieldAll_00")->IsVisible() || isNotSilentRealmOrLoftwing())) {
            rupeePos = 0;
            kakeraKeyPos = 1;
            bossKeyPos = 1;
            smallKeyPos = 1;
        } else {
            rupeePos = 2;
            kakeraKeyPos = 0;
            bossKeyPos = 0;
            smallKeyPos = 0;
        }
    } else {
        heartsHeight = 1;
        if (mShield.getGaugePercentMaybe() > 0.0f &&
            (mLyt.findPane("N_shieldAll_00")->IsVisible() || isNotSilentRealmOrLoftwing())) {
            rupeePos = 1;
            kakeraKeyPos = 1;
            bossKeyPos = 3;
            smallKeyPos = 3;
        } else {
            rupeePos = 4;
            kakeraKeyPos = 0;
            bossKeyPos = 2;
            smallKeyPos = 2;
        }
    }

    if (dMessage_c::getInstance()->getField_0x2FC() != 0) {
        rupeePos = 3;
        if (dLytMsgWindow_c::getInstance() != nullptr && (dLytMsgWindow_c::getInstance()->fn_800D7B40() == 0x9E02 ||
                                                          dLytMsgWindow_c::getInstance()->fn_800D7B40() == 0x768C)) {
            s32 off = -(mHeart.getCurrentHealthCapacity() / 4 <= 10);
            rupeePos = 6 + off;
        }
    }

    if (mShieldPosIndex != heartsHeight) {
        mOldShieldPosIndex = mShieldPosIndex;
        mShieldPosIndex = heartsHeight;
        mShieldPosInterpFrame = 0;
    }

    if (mRupyPosIndex != rupeePos) {
        mOldRupyPosIndex = mRupyPosIndex;
        mRupyPosIndex = rupeePos;
        mRupyPosInterpFrame = 0;
    }

    if (mpKakeraKey != nullptr) {
        mpKakeraKey->setPosition(kakeraKeyPos);
    }

    if (mpBossKey != nullptr) {
        mpBossKey->setPosition(bossKeyPos);
    }

    if (mpSmallKey != nullptr) {
        mpSmallKey->setPosition(smallKeyPos);
    }

    if (mShieldPos != mShieldPositions[mShieldPosIndex]) {
        if (mShieldPosInterpFrame < 5) {
            mVec3_c diff = mShieldPositions[mShieldPosIndex] - mShieldPositions[mOldShieldPosIndex];
            mShieldPos =
                diff * cLib::easeOut(++mShieldPosInterpFrame / 5.0f, 3.0f) + mShieldPositions[mOldShieldPosIndex];
        } else {
            mShieldPosInterpFrame = 0;
            mShieldPos = mShieldPositions[mShieldPosIndex];
        }
        mNodes[METER_SHIELD].mpPane->SetTranslate(mShieldPos);
    }

    if (field_0x13748 != field_0x1374C) {
        field_0x1374C = field_0x13748;
        mAnmGroups[METER_ANIM_POSITION].setFrame(field_0x13748);
        mAnmGroups[METER_ANIM_POSITION].setAnimEnable(true);
    }

    for (int i = 0; i < METER_NUM_PANES; i++) {
        bool visible = true;
        if (!field_0x137A2[i]) {
            bool b = field_0x13782[i] && field_0x13792[i];
            if (!b) {
                visible = false;
            }
        }
        mParts[i].setShouldBeVisible(visible);
        mParts[i].execute();
    }

    if (mRupyPos != mRupyPositions[mRupyPosIndex]) {
        mShield.setfield_0x318(2);
        if (mRupy.getField_0x8A9() == 1) {
            mRupyPosInterpFrame = 5;
        }
        if (mRupyPosInterpFrame < 5) {
            mVec3_c diff = mRupyPositions[mRupyPosIndex] - mRupyPositions[mOldRupyPosIndex];
            mRupyPos = diff * cLib::easeOut(++mRupyPosInterpFrame / 5.0f, 3.0f) + mRupyPositions[mOldRupyPosIndex];
        } else {
            mRupyPosInterpFrame = 0;
            mRupyPos = mRupyPositions[mRupyPosIndex];
        }
        mNodes[METER_RUPY].mpPane->SetTranslate(mRupyPos);
    }

    for (int i = 0; i < METER_NUM_PANES; i++) {
        if (mNodes[i].mpLytPane != nullptr) {
            mNodes[i].mpLytPane->execute();
        }
    }

    mLyt.calc();
    mItemSelect.fn_800EF580();

    if (mAnmGroups[METER_ANIM_POSITION].isEnabled()) {
        mAnmGroups[METER_ANIM_POSITION].setAnimEnable(false);
    }

    container->resetFlags();
    LytDoButtonRelated::set(LytDoButtonRelated::DO_BUTTON_A, LytDoButtonRelated::DO_NONE);
    LytDoButtonRelated::set(LytDoButtonRelated::DO_BUTTON_B, LytDoButtonRelated::DO_NONE);

    LytDoButtonRelated::fn_8010EC10(LytDoButtonRelated::DO_NONE, true);
    LytDoButtonRelated::fn_8010ED50(LytDoButtonRelated::DO_NONE, true);

    LytDoButtonRelated::set(LytDoButtonRelated::DO_BUTTON_CROSS_L, LytDoButtonRelated::DO_NONE);
    LytDoButtonRelated::set(LytDoButtonRelated::DO_BUTTON_CROSS_R, LytDoButtonRelated::DO_NONE);

    LytDoButtonRelated::reset(LytDoButtonRelated::DO_BUTTON_C, LytDoButtonRelated::DO_NONE);
    LytDoButtonRelated::set(LytDoButtonRelated::DO_BUTTON_Z, LytDoButtonRelated::DO_NONE);
    LytDoButtonRelated::reset(LytDoButtonRelated::DO_BUTTON_NUN_STK, LytDoButtonRelated::DO_NONE);
    LytDoButtonRelated::set(LytDoButtonRelated::DO_BUTTON_NUN_BG, LytDoButtonRelated::DO_NONE);

    if (mpTimer != nullptr) {
        mpTimer->execute();
    }
    if (mpTimerPart1 != nullptr) {
        mpTimerPart1->execute();
    }
    if (mpTimerPart2 != nullptr) {
        mpTimerPart2->execute();
    }
    if (mpSkyGauge != nullptr) {
        mpSkyGauge->execute();
    }
    if (mpBirdGauge != nullptr) {
        mpBirdGauge->execute();
    }
    if (mpBossGauge != nullptr) {
        mpBossGauge->execute();
    }
    if (mpKakeraKey != nullptr) {
        mpKakeraKey->execute();
    }
    if (mpBossKey != nullptr) {
        mpBossKey->execute();
    }
    if (mpSmallKey != nullptr) {
        mpSmallKey->execute();
    }
    if (mpDrink != nullptr) {
        mpDrink->execute();
    }

    return true;
}

bool dLytMeter_c::draw() {
    mLyt.addToDrawList();
    if (mpTimerPart1 != nullptr) {
        mpTimerPart1->getLyt()->addToDrawList();
    }
    if (mpTimerPart2 != nullptr) {
        mpTimerPart2->getLyt()->addToDrawList();
    }
    if (mpSkyGauge != nullptr) {
        mpSkyGauge->getLyt()->addToDrawList();
    }
    if (mpBirdGauge != nullptr) {
        mpBirdGauge->getLyt()->addToDrawList();
    }

    if (mpBossGauge != nullptr) {
        mpBossGauge->draw();
    }

    if (mpKakeraKey != nullptr) {
        mpKakeraKey->draw();
    }

    if (mpBossKey != nullptr) {
        mpBossKey->draw();
    }

    if (mpSmallKey != nullptr) {
        mpSmallKey->draw();
    }

    if (mpDrink != nullptr) {
        mpDrink->draw();
    }

    return true;
}

dLytMeterContainer_c::dLytMeterContainer_c() {
    sInstance = this;
}

bool dLytMeterContainer_c::build() {
    void *data = LayoutArcManager::GetInstance()->getLoadedData("DoButton");
    mResAcc.attach(data, "");
    if (dScTitle_c::sInstance != nullptr) {
        mpEventSkip = nullptr;
    } else {
        mpEventSkip = new dLytMeterEventSkip_c();
    }

    if (mpEventSkip != nullptr) {
        mpEventSkip->build(&mResAcc);
    }

    dLytAreaCaption_c::create();
    mMeter.build(&mResAcc);
    if (dScTitle_c::sInstance != nullptr) {
        mpDoButton = nullptr;
        mpDoButtonRelated = nullptr;
        mpBirdRelated = nullptr;
    } else {
        mpDoButton = new dLytDobutton_c();
        mpDoButtonRelated = new LytDoButtonRelated();
        mpBirdRelated = new LytBirdButtonRelated();
    }

    if (mpDoButton != nullptr) {
        mpDoButton->init(&mResAcc);
    }
    if (mpDoButtonRelated != nullptr) {
        mpDoButtonRelated->build(&mResAcc);
    }
    if (mpBirdRelated != nullptr) {
        mpBirdRelated->build(&mResAcc);
    }

    fn_800D97E0(0xb);
    fn_800D9800(1);
    dMessage_c::getInstance()->init();
    mFlags = 0xFFFFFFFF;
    field_0x13B54 = 0xFFFFFFFF;
    field_0x13B60 = 0;
    field_0x13B61 = 0;
    field_0x13B62 = 0;
    field_0x13B63 = 0;
    field_0x13B64 = 0;
    field_0x13B65 = 0;
    field_0x13B66 = 0;
    return true;
}

bool dLytMeterContainer_c::remove() {
    if (mpDoButton != nullptr) {
        mpDoButton->remove();
        delete mpDoButton;
        mpDoButton = nullptr;
    }
    if (mpDoButtonRelated != nullptr) {
        mpDoButtonRelated->remove();
        delete mpDoButtonRelated;
        mpDoButtonRelated = nullptr;
    }
    mMeter.remove();
    if (mpEventSkip != nullptr) {
        mpEventSkip->remove();
        delete mpEventSkip;
        mpEventSkip = nullptr;
    }
    if (mpBirdRelated != nullptr) {
        mpBirdRelated->remove();
        delete mpBirdRelated;
        mpBirdRelated = nullptr;
    }
    dLytAreaCaption_c::remove();
    mResAcc.detach();
    return true;
}

bool dLytMeterContainer_c::execute() {
    mMeter.mMinusBtn.setField_0x4E80(field_0x13B58);
    mMeter.mDowsing.setField_0x54E0(field_0x13B5C);
    if (mpEventSkip != nullptr) {
        mpEventSkip->execute();
    }

    dLytAreaCaption_c::update();
    mMeter.execute();
    if (mpDoButton != nullptr) {
        mpDoButton->execute();
    }

    if (mpBirdRelated != nullptr) {
        mpBirdRelated->execute();
    }

    if (field_0x13B61 || (!EventManager::isInEvent() && field_0x13B62)) {
        field_0x13B60 = 0;
        field_0x13B61 = 0;
        field_0x13B62 = 0;
    }

    if (fn_800D97A0() && EventManager::isInEvent()) {
        field_0x13B62 = 1;
    }

    if (field_0x13B64 || (!EventManager::isInEvent() && field_0x13B65)) {
        dLytMeterContainer_c::setField_0x13B63(0);
        field_0x13B64 = 0;
        field_0x13B65 = 0;
    }

    if (dLytMeterContainer_c::getField_0x13B63() && EventManager::isInEvent()) {
        field_0x13B65 = 1;
    }

    return true;
}

bool dLytMeterContainer_c::draw() {
    if (mVisible) {
        mMeter.draw();
        if (mMeter.field_0x13750 != 3) {
            if (mpEventSkip != nullptr) {
                mpEventSkip->draw();
            }
            dLytAreaCaption_c::draw();
            if (mpDoButton != nullptr) {
                mpDoButton->draw();
            }
            if (mpBirdRelated != nullptr) {
                mpBirdRelated->draw();
            }
        }
    }

    return true;
}

void dLytMeterContainer_c::setStaminaWheelPercentInternal(f32 percent) {
    mMeter.mGanbariGauge.setStaminaPercent(percent);
}

void dLytMeterContainer_c::setStaminaWheelPercent(f32 percent) {
    if (sInstance != nullptr) {
        sInstance->setStaminaWheelPercentInternal(percent);
    }
}
