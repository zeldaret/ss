// clang-format off
#include "d/lyt/d2d.h"
#include "d/d_cursor_hit_check.h"
#include "c/c_lib.h"
#include "common.h"
#include "d/a/d_a_bird.h"
#include "d/a/d_a_item.h"
#include "d/a/d_a_player.h"
#include "d/d_message.h"
#include "d/d_sc_game.h"
#include "d/d_sc_title.h"
#include "d/d_stage.h"
#include "d/d_stage_mgr.h"
#include "d/flag/storyflag_manager.h"
#include "d/flag/dungeonflag_manager.h"
#include "d/flag/sceneflag_manager.h"
#include "d/lyt/d_lyt_area_caption.h"
#include "d/lyt/d_lyt_control_game.h"
#include "d/lyt/d_lyt_do_button.h"
#include "d/lyt/d_lyt_map.h"
#include "d/lyt/d_lyt_meter_hio.h"
#include "d/lyt/d_lyt_save_mgr.h"
#include "d/lyt/d_lyt_unknowns.h"
#include "d/lyt/d_window.h"
#include "d/lyt/meter/d_lyt_meter.h"
#include "d/lyt/meter/d_lyt_meter_key.h"
#include "d/lyt/meter/d_lyt_meter_drink.h"
#include "d/lyt/meter/d_lyt_meter_timer.h"
#include "d/lyt/d_lyt_bird_gauge.h"
#include "d/lyt/d_lyt_boss_gauge.h"
#include "d/lyt/d_lyt_sky_gauge.h"
#include "d/lyt/msg_window/d_lyt_msg_window.h"
#include "d/lyt/msg_window/d_lyt_simple_window.h"
#include "d/snd/d_snd_small_effect_mgr.h"
#include "f/f_manager.h"
#include "f/f_profile_name.h"
#include "m/m_vec.h"
#include "nw4r/lyt/lyt_group.h"
#include "sized_string.h"
#include "toBeSorted/arc_managers/layout_arc_manager.h"
#include "toBeSorted/event_manager.h"
#include "toBeSorted/minigame_mgr.h"
#include "toBeSorted/misc_actor.h"
// clang-format on

static dLytMeter_HIO_c sHio;
dLytMeter_c *dLytMeter_c::sInstance;

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
    if (dLytMeter_c::GetInstance()->checkAllFlags(METER_BTN_1)) {
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
    if (dLytMeter_c::GetInstance()->checkAllFlags(METER_BTN_1)) {
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

    // "Gear"
    mLyt.loadTextVariant(mLyt.getTextBox("T_sabBtnS_00"), 0);
    mLyt.loadTextVariant(mLyt.getTextBox("T_sabBtn_00"), 0);
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
    dLytMeter_c *meter = dLytMeter_c::GetInstance();
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
        if (StoryflagManager::sInstance->getCounterOrFlag(STORYFLAG_1_BTN_NOTICE)) {
            mShouldCall = true;
        }
    } else {
        mShouldCall = false;
    }

    if (mShouldCall && meter->checkAllFlags(METER_BTN_1)) {
        if (!mHasInitedCall) {
            mAnm[BUTTON_1_ANIM_CALL].setToEnd();
            mAnm[BUTTON_1_ANIM_CALL].setAnimEnable(true);
            mHasInitedCall = true;
        }

        if (mAnm[BUTTON_1_ANIM_CALL].isEnabled()) {
            mAnm[BUTTON_1_ANIM_CALL].play();
            if (meter->checkAllFlags(METER_BTN_1) && mCallCount < 3 && mAnm[BUTTON_1_ANIM_CALL].getFrame() == 1.0f) {
                dSndSmallEffectMgr_c::GetInstance()->playSound(SE_S_1_BUTTON_BLINK);
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
    if (dLytControlGame_c::getInstance()->isStateNormal() &&
        StoryflagManager::sInstance->getCounterOrFlag(STORYFLAG_1_BTN_NOTICE)) {
        return true;
    }
    return false;
}

void dLytMeter2Button_c::initializeState_Wait() {}
void dLytMeter2Button_c::executeState_Wait() {
    if (dLytMeter_c::GetInstance()->checkAllFlags(METER_BTN_2)) {
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
    if (dLytMeter_c::GetInstance()->checkAllFlags(METER_BTN_2)) {
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
    dLytMeter_c *meter = dLytMeter_c::GetInstance();
    if (meter->isHelpOpen()) {
        StoryflagManager::sInstance->unsetFlag(STORYFLAG_2_BTN_NOTICE);
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
        if (StoryflagManager::sInstance->getCounterOrFlag(STORYFLAG_2_BTN_NOTICE)) {
            mShouldCall = true;
        }
    } else {
        mShouldCall = false;
    }

    if (mShouldCall && meter->checkAllFlags(METER_BTN_2)) {
        if (!mHasInitedCall) {
            mAnm[BUTTON_1_ANIM_CALL].setToEnd();
            mAnm[BUTTON_1_ANIM_CALL].setAnimEnable(true);
            mHasInitedCall = true;
        }

        if (mAnm[BUTTON_1_ANIM_CALL].isEnabled()) {
            mAnm[BUTTON_1_ANIM_CALL].play();
            if (meter->checkAllFlags(METER_BTN_2) && mCallCount < 3 && mAnm[BUTTON_1_ANIM_CALL].getFrame() == 1.0f) {
                dSndSmallEffectMgr_c::GetInstance()->playSound(SE_S_2_BUTTON_BLINK);
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
    if (dLytControlGame_c::getInstance()->isStateNormal() &&
        StoryflagManager::sInstance->getCounterOrFlag(STORYFLAG_2_BTN_NOTICE)) {
        return true;
    }
    return false;
}

enum MeterPart_e {
    METER_GANBARI = 0,
    METER_RUPY = 1,
    METER_ITEM_SELECT = 2,
    METER_CROSS_BTN = 3,
    METER_PLUS_BTN = 4,
    METER_MINUS_BTN = 5,
    METER_A_BTN = 6,
    METER_DOWSING = 7,
    METER_Z_BTN = 8,
    METER_NUN_STK = 9,
    METER_REMOCON_BG = 10,
    METER_NUN_BG = 11,
    METER_HEART = 12,
    METER_SHIELD = 13,
    METER_1_BTN = 14,
    METER_2_BTN = 15,
};

#define METER_ANIM_ITEM_SELECT 0
#define METER_ANIM_MINUS_BTN 1
#define METER_ANIM_PLUS_BTN 2
#define METER_ANIM_CROSS_BTN 3
#define METER_ANIM_1_BTN 4
#define METER_ANIM_2_BTN 5
#define METER_ANIM_A_BTN 6
#define METER_ANIM_REMOCON_BG 7
#define METER_ANIM_DOWSING 8
#define METER_ANIM_Z_BTN 9
#define METER_ANIM_NUN_STK 10
#define METER_ANIM_NUN_BG 11
#define METER_ANIM_RUPY 12
#define METER_ANIM_SHIELD 13
#define METER_ANIM_HEART 14
#define METER_ANIM_GANBARI_GAUGE 15

void dLytMeterParts_c::initializeState_Invisible() {}
void dLytMeterParts_c::executeState_Invisible() {
    if (mShouldBeVisible) {
        if (mIndex == METER_SHIELD) {
            mpAnmIn->setForwardOnce();
            if (dMessage_c::getInstance()->getField_0x2FC() != 0) {
                mpAnmIn->setToEnd();
                dLytMeter_c::setRupyField_0x8A9(1);
            } else {
                mpAnmIn->setFrame(0.0f);
                dLytMeter_c::setRupyField_0x8A9(2);
            }
            mpAnmIn->setAnimEnable(true);
        } else {
            mpAnmIn->setForwardOnce();
            mpAnmIn->setFrame(0.0f);
            mpAnmIn->setAnimEnable(true);
        }
        mStateMgr.changeState(StateID_In);
    }
}
void dLytMeterParts_c::finalizeState_Invisible() {}

void dLytMeterParts_c::initializeState_In() {}
void dLytMeterParts_c::executeState_In() {
    if (mpAnmIn->isEndReached()) {
        mpAnmIn->setAnimEnable(false);
        mStateMgr.changeState(StateID_Visible);
    }

    if (mpAnmIn->isEnabled()) {
        mpAnmIn->play();
    }
}
void dLytMeterParts_c::finalizeState_In() {}

void dLytMeterParts_c::initializeState_Visible() {}
void dLytMeterParts_c::executeState_Visible() {
    if (!mShouldBeVisible) {
        if (mIndex == METER_SHIELD) {
            mpAnmOut->setForwardOnce();
            if (dLytMeter_c::getRupyField_0x8AC()) {
                mpAnmOut->setToEnd();
                if (dLytMeter_c::getHeartField_0x78C() == 0) {
                    dLytMeter_c::setRupyField_0x8AA(1);
                } else {
                    dLytMeter_c::setRupyField_0x8AA(3);
                }
                dLytMeter_c::setRupyField_0x8AC(0);
            } else {
                mpAnmOut->setFrame(0.0f);
                if (dLytMeter_c::getHeartField_0x78C() == 0) {
                    dLytMeter_c::setRupyField_0x8AA(2);
                } else {
                    dLytMeter_c::setRupyField_0x8AA(4);
                }
            }
            mpAnmOut->setAnimEnable(true);
        } else {
            mpAnmOut->setForwardOnce();
            mpAnmOut->setFrame(0.0f);
            mpAnmOut->setAnimEnable(true);
        }
        mStateMgr.changeState(StateID_Out);
    }
}
void dLytMeterParts_c::finalizeState_Visible() {}

void dLytMeterParts_c::initializeState_Out() {}
void dLytMeterParts_c::executeState_Out() {
    dLytMeter_c *meter = dLytMeter_c::GetInstance();
    if (mpAnmOut->isEndReached()) {
        if (mIndex == METER_SHIELD && dMessage_c::getInstance()->getField_0x2FC() == -2) {
            dMessage_c::getInstance()->setField_0x2FC(0);
        }
        mpAnmOut->setAnimEnable(false);
        mStateMgr.changeState(StateID_Invisible);
    }

    if (mpAnmOut->isEnabled()) {
        mpAnmOut->play();
    }

    switch (mIndex) {
        case METER_ANIM_MINUS_BTN: meter->clearFlags(METER_BTN_MINUS); break;
        case METER_ANIM_1_BTN:     meter->clearFlags(METER_BTN_1); break;
        case METER_ANIM_CROSS_BTN: meter->clearFlags(METER_BTN_CROSS); break;
        case METER_ANIM_2_BTN:     meter->clearFlags(METER_BTN_2); break;
        case METER_ANIM_A_BTN:     meter->clearFlags(METER_BTN_NUN_STK); break;
        case METER_ANIM_DOWSING:   meter->clearFlags(METER_BTN_C); break;
        case METER_ANIM_Z_BTN:     meter->clearFlags(METER_BTN_0x1000); break;
        case METER_ANIM_NUN_STK:   meter->clearFlags(METER_BTN_NUN_STK); break;
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

bool dLytMeterMain_c::isNotSilentRealmOrLoftwing() {
    if (isSilentRealm() || dAcPy_c::GetLink()->getRidingActorType() == dAcPy_c::RIDING_LOFTWING) {
        return false;
    }
    return true;
}

bool dLytMeterMain_c::fn_800C9F70() {
    if ((fn_800D56B0() && !field_0x13775) || isInModeMap() || isInModePause()) {
        return true;
    }
    return false;
}

bool dLytMeterMain_c::fn_800C9FE0() {
    if (fn_800D56B0() || mMode == MODE_MAP_INIT || isInModePause()) {
        return true;
    }
    return false;
}

bool dLytMeterMain_c::isPopupOpen() {
    if (mHelpOpen || isInModeMap() || isInModePause()) {
        return true;
    }
    return false;
}

void dLytMeterMain_c::setUiMode(u16 value) const {
    StoryflagManager::sInstance->setFlagOrCounterToValue(STORYFLAG_UI_MODE, value);
}

u8 dLytMeterMain_c::getUiMode() {
    return StoryflagManager::sInstance->getFlag(STORYFLAG_UI_MODE);
}

dLytMeterMain_c::dLytMeterMain_c() {}

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

bool dLytMeterMain_c::build(d2d::ResAccIf_c *resAcc) {
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

    mSavedBasicPosition = 0;
    mBasicPosition = POSITION_NORMAL;
    mMode = MODE_NONE;
    field_0x13770 = 3;
    mHelpOpen = false;
    field_0x13775 = 0;
    field_0x13773 = 0;
    mTimerVisible = false;
    mBirdGaugeVisible = false;
    field_0x13780 = 1;
    field_0x13781 = 0;
    mSkyGaugeVisible = false;
    mBossGaugeVisible = false;
    mKakeraKeyVisible = false;
    mBossKeyVisible = false;
    mSmallKeyVisible = false;
    mDrinkVisible = false;
    mIsInSwordDrawEvent = false;
    field_0x1377F = false;
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
        mPanesVisible[i] = false;
        mPanesNotHiddenByAreaCaption[i] = true;
        mPanesForceShown[i] = false;
    }

    field_0x137B2 = 1;

    return true;
}

bool dLytMeterMain_c::remove() {
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

void dLytMeterMain_c::executeMap() {
    dLytMeter_c *meter = dLytMeter_c::GetInstance();

    if (dLytMap_c::GetInstance() != nullptr && !dLytMap_c::GetInstance()->isMapEventEq2Or4Or5Or6()) {
        meter->setFlags(METER_BTN_PLUS);
    }

    LytDoButtonRelated::set(LytDoButtonRelated::DO_BUTTON_PLUS, LytDoButtonRelated::ACT_IE_ETC_BACK_2);
    meter->clearFlags(METER_BTN_MINUS | METER_BTN_1 | METER_BTN_2);
    if (LytDoButtonRelated::get(LytDoButtonRelated::DO_BUTTON_C) == LytDoButtonRelated::ACT_IE_NONE) {
        meter->clearFlags(METER_BTN_C);
    }
}

bool dLytMeterMain_c::isInMapEvent() {
    if (mBasicPosition == POSITION_MAP && dMessage_c::getInstance()->getInMapEvent()) {
        return true;
    }
    return false;
}

bool dLytMeterMain_c::fn_800D5380(u8 arg) {
    if (dAcPy_c::GetLink() != nullptr &&
        (dAcPy_c::GetLink()->checkActionFlagsCont(0x400 | 0x100 | 0x80 | 0x40 | 0x10 | 0x4 | 0x2 | 0x1) &&
         (dAcPy_c::GetLink()->checkActionFlagsCont(0x40) || (arg && mItemSelect.getField_0x5794() == 1)))) {
        return true;
    }
    return false;
}

bool dLytMeterMain_c::isDoingSkyKeepPuzzle() {
    if (checkIsInSkykeepPuzzle() && !mHelpOpen) {
        return true;
    }
    return false;
}

bool dLytMeterMain_c::fn_800D5420() {
    if (!isInModeMap()) {
        if (!dStageMgr_c::GetInstance()->isFaderSettled() || !dScGame_c::GetInstance()->isFaderSettled() ||
            !dStageMgr_c::GetInstance()->fn_80199250() || !dStage_c::GetInstance()->fn_801B3EE0()) {
            return true;
        }
    }
    return false;
}

bool dLytMeterMain_c::isSilentRealm() {
    if (dScGame_c::currentSpawnInfo.getTrial() == SpawnInfo::TRIAL && !dScGame_c::isCurrentStage("D003_8")) {
        return true;
    }
    return false;
}

bool dLytMeterMain_c::fn_800D5590() {
    if (dLytMeter_c::getfn_800D97A0() && !dLytMeter_c::GetInstance()->fn_800D56F0()) {
        return true;
    }

    if (dLytMeter_c::getField_0x13B63()) {
        return true;
    }
    return field_0x13773;
}

void dLytMeterMain_c::fn_800D5630() {
    if (mpBossGauge != nullptr) {
        mpBossGauge->fn_80158940();
    }
}

bool dLytMeterMain_c::isInModeMap() {
    if (mMode == MODE_MAP_INIT || mMode == MODE_MAP) {
        return true;
    }
    return false;
}

bool dLytMeterMain_c::isInModePause() {
    if (mMode == MODE_PAUSE_INIT || mMode == MODE_PAUSE) {
        return true;
    }
    return false;
}

bool dLytMeterMain_c::fn_800D56B0() {
    if (dLytSaveMgr_c::GetInstance() != nullptr && dLytSaveMgr_c::GetInstance()->isBusy()) {
        return true;
    }

    if (isInMapEvent()) {
        return true;
    }

    if ((EventManager::isInEvent() && !mIsInSwordDrawEvent && dAcPy_c::GetLink()->getCurrentAction() != 0x8C &&
         dMessage_c::getInstance()->getMapEvent() == dLytMapMain_c::MAP_EVENT_MAX) ||
        mHelpOpen) {
        return true;
    }

    if (dLytMap_c::GetInstance() != nullptr && !dLytMap_c::GetInstance()->isVisibleNoIntro()) {
        return true;
    }

    return false;
}

void dLytMeterMain_c::checkPaneVisibility() {
    bool oldTimerVisible = mTimerVisible;
    bool oldBirdGaugeVisible = mBirdGaugeVisible;
    bool oldSkyGaugeVisible = mSkyGaugeVisible;
    bool oldBossGaugeVisible = mBossGaugeVisible;
    bool oldKakeraKeyVisible = mKakeraKeyVisible;
    bool oldBossKeyVisible = mBossKeyVisible;
    bool oldSmallKeyVisible = mSmallKeyVisible;
    bool oldDrinkVisible = mDrinkVisible;

    dBird_c *bird = nullptr;
    if (dAcPy_c::GetLink()->getRidingActorType() == dAcPy_c::RIDING_LOFTWING) {
        bird = static_cast<dBird_c *>(fManager_c::searchBaseByProfName(fProfile::BIRD, nullptr));
    }

    // Set all panes to visible, then go through
    // them one by one to find reasons they should
    // be hidden.

    mTimerVisible = true;
    mBirdGaugeVisible = true;
    mSkyGaugeVisible = true;
    mBossGaugeVisible = true;
    mKakeraKeyVisible = true;
    mBossKeyVisible = true;
    mSmallKeyVisible = true;
    mDrinkVisible = true;
    mIsInSwordDrawEvent = false;

    if (EventManager::getCurrentEventName() != nullptr) {
        const char *name = EventManager::getCurrentEventName();
        if (strequals(name, "SwordDraw") || strequals(name, "SwordDrawDoorNew")) {
            mIsInSwordDrawEvent = true;
            if (dLytDobutton_c::getNextActionToShow() != dLytDobutton_c::ACT_DO_DRAW) {
                dLytDobutton_c::setActionTextStuff(dLytDobutton_c::ICON_NONE, dLytDobutton_c::ACT_DO_INVALID, true);
            }
            if (LytDoButtonRelated::get(LytDoButtonRelated::DO_BUTTON_A) != LytDoButtonRelated::ACT_IE_INFO_DRAW) {
                LytDoButtonRelated::set(LytDoButtonRelated::DO_BUTTON_A, LytDoButtonRelated::ACT_IE_NONE);
            }
            LytDoButtonRelated::set(LytDoButtonRelated::DO_BUTTON_C, LytDoButtonRelated::ACT_IE_NONE);
            LytDoButtonRelated::set(LytDoButtonRelated::DO_BUTTON_Z, LytDoButtonRelated::ACT_IE_NONE);
        }
    }

    for (int i = 0; i < METER_NUM_PANES; i++) {
        mPanesVisible[i] = true;
    }

    if (dAcPy_c::GetLink2()->canDowseProbably() && !isInModeMap() && !isInModePause()) {
        if (!field_0x1377F) {
            field_0x1377F = true;
        }
    } else {
        if (field_0x1377F) {
            field_0x1377F = false;
        }
    }

    if ((!StoryflagManager::sInstance->getCounterOrFlag(STORYFLAG_B_WHEEL_UNLOCKED) &&
         ((LytDoButtonRelated::get(LytDoButtonRelated::DO_BUTTON_B) == LytDoButtonRelated::ACT_IE_NONE &&
           mItemSelect.getField_0x5794() != 2 &&
           (!EventManager::isInEvent() || !EventManager::isCurrentEvent("ItemGetGorgeous")))))

        || (dStageMgr_c::GetInstance()->isAreaTypeHouse() &&
            LytDoButtonRelated::get(LytDoButtonRelated::DO_BUTTON_B) == LytDoButtonRelated::ACT_IE_NONE &&
            !MinigameManager::isInMinigameState(MinigameManager::HOUSE_CLEANING) && !mItemSelect.fn_800F02F0())

        || (isSilentRealm() &&
            LytDoButtonRelated::get(LytDoButtonRelated::DO_BUTTON_B) == LytDoButtonRelated::ACT_IE_NONE &&
            !mItemSelect.fn_800F02F0())

        || ((dAcPy_c::GetLink()->checkActionFlagsCont(0x400000) || fn_800D5420() ||
             dAcPy_c::GetLink()->checkActionFlags(dAcPy_c::FLG0_CRAWLING) || fn_800D5380(0) ||
             MinigameManager::isInMinigameState(MinigameManager::FUN_FUN_ISLAND) ||
             MinigameManager::isInMinigameState(MinigameManager::THRILL_DIGGER) ||
             MinigameManager::isInMinigameState(MinigameManager::BAMBOO_CUTTING)))

        || (MinigameManager::isInMinigameState(MinigameManager::TRIAL_TIME_ATTACK) &&
            LytDoButtonRelated::get(LytDoButtonRelated::DO_BUTTON_B) == LytDoButtonRelated::ACT_IE_NONE) ||
        (dLytMeter_c::getField_0x13B66() || (fn_800D56B0() && !mItemSelect.fn_800F02F0() && !isDoingSkyKeepPuzzle()) ||
         isInModeMap() || isInModePause())) {
        mPanesVisible[METER_ANIM_ITEM_SELECT] = false;
    }

    if ((fn_800D56B0() && !isDoingSkyKeepPuzzle() && !mMinusBtn.fn_800F75E0())

        || (dLytMeter_c::getField_0x13B66() || fn_800D5420() || mMode == MODE_MAP_INIT || isInModePause())) {
        mPanesVisible[METER_ANIM_MINUS_BTN] = false;
    }

    if ((fn_800D56B0() && !isDoingSkyKeepPuzzle())

        || (dLytMeter_c::getField_0x13B66() || fn_800D5420() || mMode == MODE_MAP_INIT || isInModePause())

        || (getUiMode() > 1 && !mPlusBtn.getField_0x1C0() && !mPlusBtn.isCalling())) {
        mPanesVisible[METER_ANIM_PLUS_BTN] = false;
    }

    if ((fn_800D56B0() && !isDoingSkyKeepPuzzle())

        || (dLytMeter_c::getField_0x13B66() || fn_800D5420() || mMode == MODE_MAP_INIT || isInModePause())

        || (getUiMode() > 1 && !mCrossBtn.fn_800FA730())) {
        mPanesVisible[METER_ANIM_CROSS_BTN] = false;
    }

    if ((fn_800D56B0() && !isDoingSkyKeepPuzzle())

        || (dLytMeter_c::getField_0x13B66() || fn_800D5420() || mMode == MODE_MAP_INIT || isInModePause())

        || (getUiMode() != 0 && !mp1Button->shouldCall())) {
        mPanesVisible[METER_ANIM_1_BTN] = false;
    }

    if ((fn_800D56B0() && !isDoingSkyKeepPuzzle())

        || (dLytMeter_c::getField_0x13B66() || fn_800D5420() || mMode == MODE_MAP_INIT || isInModePause())

        || (getUiMode() != 0 && !mp2Button->shouldCall())) {
        mPanesVisible[METER_ANIM_2_BTN] = false;
    }

    if ((fn_800D56B0() && !isDoingSkyKeepPuzzle())

        || (dLytMeter_c::getField_0x13B66() || fn_800D5420() || mMode == MODE_MAP_INIT || isInModePause())

        || (getUiMode() > 1)) {
        mPanesVisible[METER_ANIM_A_BTN] = false;
    }

    if ((fn_800D56B0() && !isDoingSkyKeepPuzzle())

        || (dLytMeter_c::getField_0x13B66() || fn_800D5420() || mMode == MODE_MAP_INIT || isInModePause())

        || (getUiMode() != 0)) {
        mPanesVisible[METER_ANIM_REMOCON_BG] = false;
    }

    if ((fn_800D56B0() && !isDoingSkyKeepPuzzle() && !mDowsing.fn_800FE490())

        || (dLytMeter_c::getField_0x13B66() || fn_800D5420() || mMode == MODE_MAP_INIT || isInModePause())

        || (getUiMode() != 0 && !fn_800D5380(true) && !mDowsing.shouldCall() && !mDowsing.fn_800FE490())) {
        mPanesVisible[METER_ANIM_DOWSING] = false;
    }

    if ((fn_800D56B0() && !isDoingSkyKeepPuzzle())

        || (dLytMeter_c::getField_0x13B66() || fn_800D5420() || mMode == MODE_MAP_INIT || isInModePause())

        || (getUiMode() != 0 && !fn_800D5380(true) && !mZBtn.isCalling())) {
        mPanesVisible[METER_ANIM_Z_BTN] = false;
    }

    if ((fn_800D56B0() && !isDoingSkyKeepPuzzle())

        || (dLytMeter_c::getField_0x13B66() || fn_800D5420() || mMode == MODE_MAP_INIT || isInModePause())

        || (getUiMode() != 0)) {
        mPanesVisible[METER_ANIM_NUN_STK] = false;
    }

    if ((fn_800D56B0() && !isDoingSkyKeepPuzzle())

        || (dLytMeter_c::getField_0x13B66() || fn_800D5420() || mMode == MODE_MAP_INIT || isInModePause())

        || (getUiMode() != 0)) {
        mPanesVisible[METER_ANIM_NUN_BG] = false;
    }

    if ((!isNotSilentRealmOrLoftwing() || (mShield.getCurrentDurability() == 0.0f && mShield.getField_0x31D()) ||
         dAcPy_c::GetLink()->getCurrentAction() == 0x8C || fn_800D5380(false) || field_0x13770 != 3 ||
         MinigameManager::isInMinigameState(MinigameManager::FUN_FUN_ISLAND) ||
         MinigameManager::isInMinigameState(MinigameManager::THRILL_DIGGER) ||
         MinigameManager::isInMinigameState(MinigameManager::BAMBOO_CUTTING) ||
         MinigameManager::isInMinigameState(MinigameManager::TRIAL_TIME_ATTACK) ||
         MinigameManager::isInMinigameState(MinigameManager::PUMPKIN_ARCHERY) ||
         MinigameManager::isInMinigameState(MinigameManager::SPIRAL_CHARGE_TUTORIAL) ||
         MinigameManager::isInMinigameState(MinigameManager::ROLLERCOASTER) || fn_800D56B0() || mIsInSwordDrawEvent)

        || (dLytMeter_c::getField_0x13B66() || fn_800D5420() ||
            (dLytSimpleWindow_c::getInstance() != nullptr && dLytSimpleWindow_c::getInstance()->isOutputText()) ||
            isInModeMap() || isInModePause())) {
        mPanesVisible[METER_ANIM_RUPY] = false;
    }

    if ((isSilentRealm() || fn_800D5380(false) || field_0x13770 != 3 ||
         dAcPy_c::GetLink()->getCurrentAction() == 0x8C ||

         MinigameManager::isInMinigameState(MinigameManager::FUN_FUN_ISLAND) ||
         MinigameManager::isInMinigameState(MinigameManager::THRILL_DIGGER) ||
         MinigameManager::isInMinigameState(MinigameManager::BAMBOO_CUTTING) ||
         MinigameManager::isInMinigameState(MinigameManager::TRIAL_TIME_ATTACK) ||
         MinigameManager::isInMinigameState(MinigameManager::PUMPKIN_ARCHERY) ||
         MinigameManager::isInMinigameState(MinigameManager::INSECT_CAPTURE) ||
         MinigameManager::isInMinigameState(MinigameManager::SPIRAL_CHARGE_TUTORIAL) ||
         MinigameManager::isInMinigameState(MinigameManager::ROLLERCOASTER))

        ||
        (dMessage_c::getInstance()->getField_0x2FC() && mHeart.getField_0x78C() && !dAcPy_c::LINK->isSittingOrUnk0xAE())

        || (fn_800D56B0() &&
            (dMessage_c::getInstance()->getField_0x2FC() == 0 || dMessage_c::getInstance()->getField_0x2FC() == -2)) ||
        mIsInSwordDrawEvent

        || (dLytMeter_c::getField_0x13B66() || fn_800D5420() ||
            (dLytSimpleWindow_c::getInstance() != nullptr && dLytSimpleWindow_c::getInstance()->isOutputText()) ||
            isInModeMap() || isInModePause())) {
        mPanesVisible[METER_ANIM_SHIELD] = false;
    }

    if (dMessage_c::getInstance()->getField_0x2FC() != 0) {
        mRupy.setSize(0);
    } else {
        mRupy.setSize(1);
    }

    if ((isSilentRealm() || field_0x13770 != 3 || dAcPy_c::GetLink()->getCurrentAction() == 0x8C ||

         MinigameManager::isInMinigameState(MinigameManager::FUN_FUN_ISLAND) ||
         MinigameManager::isInMinigameState(MinigameManager::BAMBOO_CUTTING) ||
         MinigameManager::isInMinigameState(MinigameManager::TRIAL_TIME_ATTACK) ||
         MinigameManager::isInMinigameState(MinigameManager::PUMPKIN_ARCHERY) ||
         MinigameManager::isInMinigameState(MinigameManager::SPIRAL_CHARGE_TUTORIAL) ||
         MinigameManager::isInMinigameState(MinigameManager::ROLLERCOASTER))

        || (fn_800D56B0() && mHeart.getField_0x78C() == 0)

        || (dScGame_c::currentSpawnInfo.stageName == "F406" && dScGame_c::currentSpawnInfo.layer == 13)

        || (mIsInSwordDrawEvent)

        || (dLytMeter_c::getField_0x13B66() || fn_800D5420() ||
            (dLytSimpleWindow_c::getInstance() != nullptr && dLytSimpleWindow_c::getInstance()->isOutputText()) ||
            isInModeMap() || isInModePause())) {
        mPanesVisible[METER_ANIM_HEART] = false;
    }

    if (!isSilentRealm() || (fn_800D56B0() && !fn_800D5590())

        || (dLytMeter_c::getField_0x13B66() || fn_800D5420() || mMode == MODE_MAP_INIT || isInModePause())) {
        mTimerVisible = false;
    }

    if (mpTimer != nullptr && mTimerVisible != oldTimerVisible) {
        if (mTimerVisible) {
            mpTimer->startIn2();
        } else {
            mpTimer->startOut2();
        }
    }

    if ((dAcPy_c::GetLink()->hasvt_0x1C0() || dAcPy_c::GetLink()->checkActionFlagsCont(0x10) ||
         (fn_800D56B0() && !mHelpOpen) || mGanbariGauge.fn_80104760() ||

         MinigameManager::isInMinigameState(MinigameManager::FUN_FUN_ISLAND) ||
         MinigameManager::isInMinigameState(MinigameManager::THRILL_DIGGER) ||
         MinigameManager::isInMinigameState(MinigameManager::PUMPKIN_ARCHERY) ||
         MinigameManager::isInMinigameState(MinigameManager::SPIRAL_CHARGE_TUTORIAL) ||
         MinigameManager::isInMinigameState(MinigameManager::ROLLERCOASTER))

        || (dLytMeter_c::getField_0x13B66() || fn_800D5420() || isInModeMap() || isInModePause())) {
        mPanesVisible[METER_ANIM_GANBARI_GAUGE] = false;
    }

    if ((dAcPy_c::GetLink()->getRidingActorType() != dAcPy_c::RIDING_LOFTWING || fn_800D56B0()) ||
        (dLytMeter_c::getField_0x13B66() || fn_800D5420() || isInModeMap() || isInModePause())) {
        mSkyGaugeVisible = false;
    } else if (bird != nullptr && mpSkyGauge != nullptr) {
        mpSkyGauge->setHeight(dAcPy_c::GetLink()->vt_0x260());
    }

    if (mpSkyGauge != nullptr && mSkyGaugeVisible != oldSkyGaugeVisible) {
        if (mSkyGaugeVisible) {
            mpSkyGauge->setWantsIn();
        } else {
            mpSkyGauge->setWantsOut();
        }
    }

    if (dAcPy_c::GetLink()->getRidingActorType() != dAcPy_c::RIDING_LOFTWING || !field_0x13780 ||

        (dLytDobutton_c::getAction() != dLytDobutton_c::ACT_DO_INVALID || fn_800D56B0() ||
         dLytMeter_c::getField_0x13B66() || fn_800D5420() || isInModeMap() || isInModePause())) {
        mBirdGaugeVisible = false;
    }

    if (mpBirdGauge != nullptr) {
        if (bird != nullptr) {
            mpBirdGauge->setNumDashes(bird->getNumDashes());
        }
        if (mBirdGaugeVisible != oldBirdGaugeVisible) {
            if (mBirdGaugeVisible) {
                if (mpBirdGauge->getField_0x693()) {
                    mpBirdGauge->setField_0x690(1);
                } else {
                    mBirdGaugeVisible = false;
                }
            } else {
                if (mpBirdGauge->getField_0x692()) {
                    mpBirdGauge->hide();
                } else {
                    mBirdGaugeVisible = true;
                }
            }
        }
    }

    if (!field_0x13781 || fn_800D56B0() ||

        (dLytMeter_c::getField_0x13B66() || fn_800D5420() || isInModeMap() || isInModePause())) {
        mBossGaugeVisible = false;
    }

    if (mBossGaugeVisible != oldBossGaugeVisible) {
        if (mBossGaugeVisible) {
            if (mpBossGauge != nullptr) {
                s32 mode = 0;
                if (dScGame_c::currentSpawnInfo.layer == 3) {
                    mode = 1;
                } else if (dScGame_c::currentSpawnInfo.layer == 4) {
                    mode = 2;
                }
                mpBossGauge->setMode(mode);
            }
        } else {
            if (mpBossGauge != nullptr) {
                mpBossGauge->setField_0x545(true);
            }
        }
    }

    if (mpKakeraKey != nullptr) {
        if (dAcItem_c::getKeyPieceCount() == 0 || dLytAreaCaption_c::getVisible() || fn_800D5380(false) ||
            SceneflagManager::sInstance->checkSceneflagGlobal(4, 0x21) || MinigameManager::isInAnyMinigame()

            || (dLytMeter_c::getField_0x13B66() || fn_800D56B0() || mIsInSwordDrawEvent || fn_800D5420() ||
                isInModeMap() || isInModePause())) {
            mKakeraKeyVisible = false;
        }
        if (mKakeraKeyVisible != oldKakeraKeyVisible) {
            if (mKakeraKeyVisible) {
                mpKakeraKey->setShouldBeVisible(true);
            } else {
                mpKakeraKey->setShouldBeVisible(false);
            }
        }
    }

    if (mpBossKey != nullptr) {
        if (DungeonflagManager::sInstance->getCounterOrFlag(12, 8) == 0

            || DungeonflagManager::sInstance->getCounterOrFlag(16, 8) != 0 ||
            dAcPy_c::GetLink()->getCurrentAction() == 0x8C || dLytAreaCaption_c::getVisible() || fn_800D5380(false) ||
            MinigameManager::isInAnyMinigame() || fn_800D56B0() || mIsInSwordDrawEvent

            || (dLytMeter_c::getField_0x13B66() || fn_800D5420() || isInModeMap() || isInModePause())) {
            mBossKeyVisible = false;
        }

        if (mBossKeyVisible != oldBossKeyVisible) {
            if (mBossKeyVisible) {
                mpBossKey->setShouldBeVisible(true);
            } else {
                mpBossKey->setShouldBeVisible(false);
            }
        }
    }

    if (mpSmallKey != nullptr) {
        if (dAcItem_c::getSmallKeyCount() == 0 || dLytAreaCaption_c::getVisible() || fn_800D5380(false) ||
            MinigameManager::isInAnyMinigame() || fn_800D56B0() || mIsInSwordDrawEvent

            || (dLytMeter_c::getField_0x13B66() || fn_800D5420() || isInModeMap() || isInModePause())) {
            mSmallKeyVisible = false;
        }

        if (mSmallKeyVisible != oldSmallKeyVisible) {
            if (mSmallKeyVisible) {
                mpSmallKey->setShouldBeVisible(true);
            } else {
                mpSmallKey->setShouldBeVisible(false);
            }
        }
    }

    if (mpDrink != nullptr) {
        if (!field_0x137B2 || dLytAreaCaption_c::getVisible() || fn_800D5380(false) ||
            MinigameManager::isInAnyMinigame() || mIsInSwordDrawEvent

            || fn_800D56B0() || dLytMeter_c::getField_0x13B66() || fn_800D5420() || isInModeMap() || isInModePause()) {
            mDrinkVisible = false;
        }

        if (mDrinkVisible != oldDrinkVisible) {
            if (mDrinkVisible) {
                mpDrink->setShouldBeVisible(true);
            } else {
                mpDrink->setShouldBeVisible(false);
            }
        }

        mVec3_c pos = mRupy.getLastVisibleDigitPosition();
        mpDrink->setField_0x6E0(pos);
    }
}

bool dLytMeterMain_c::execute() {
    dLytMeter_c *meter = dLytMeter_c::GetInstance();
    if (field_0x13775 && !EventManager::isInEvent()) {
        field_0x13775 = 0;
    }

    if (isSilentRealm()) {
        meter->clearFlags(METER_BTN_MINUS);
        meter->clearFlags(METER_BTN_CROSS);
    }

    if (dStageMgr_c::GetInstance()->isInLastBoss()) {
        meter->clearFlags(METER_BTN_PLUS);
    }

    if (mMode != MODE_MAP_INIT && mBasicPosition == POSITION_MAP) {
        executeMap();
    }

    checkPaneVisibility();

    s32 heartsHeight;
    s32 rupeePos;
    s32 kakeraKeyPos;
    s32 bossKeyPos;
    s32 smallKeyPos;

    if (mHeart.getCurrentHealthCapacity() / 4 <= 10) {
        heartsHeight = 0;
        if (mShield.getCurrentDurability() > 0.0f &&
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
        if (mShield.getCurrentDurability() > 0.0f &&
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
        if (dLytMsgWindow_c::getInstance() != nullptr && (dLytMsgWindow_c::getInstance()->fn_800D7B40() == 40450 ||
                                                          dLytMsgWindow_c::getInstance()->fn_800D7B40() == 30348)) {
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

    if (mBasicPosition != mSavedBasicPosition) {
        mSavedBasicPosition = mBasicPosition;
        mAnmGroups[METER_ANIM_POSITION].setFrame(mBasicPosition);
        mAnmGroups[METER_ANIM_POSITION].setAnimEnable(true);
    }

    for (int i = 0; i < METER_NUM_PANES; i++) {
        bool visible = true;
        if (!mPanesForceShown[i]) {
            bool b = mPanesVisible[i] && mPanesNotHiddenByAreaCaption[i];
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

    meter->resetFlags();
    LytDoButtonRelated::set(LytDoButtonRelated::DO_BUTTON_A, LytDoButtonRelated::ACT_IE_NONE);
    LytDoButtonRelated::set(LytDoButtonRelated::DO_BUTTON_B, LytDoButtonRelated::ACT_IE_NONE);

    LytDoButtonRelated::setCrossTop(LytDoButtonRelated::ACT_IE_NONE, true);
    LytDoButtonRelated::setCrossDown(LytDoButtonRelated::ACT_IE_NONE, true);

    LytDoButtonRelated::set(LytDoButtonRelated::DO_BUTTON_CROSS_L, LytDoButtonRelated::ACT_IE_NONE);
    LytDoButtonRelated::set(LytDoButtonRelated::DO_BUTTON_CROSS_R, LytDoButtonRelated::ACT_IE_NONE);

    LytDoButtonRelated::reset(LytDoButtonRelated::DO_BUTTON_C, LytDoButtonRelated::ACT_IE_NONE);
    LytDoButtonRelated::set(LytDoButtonRelated::DO_BUTTON_Z, LytDoButtonRelated::ACT_IE_NONE);
    LytDoButtonRelated::reset(LytDoButtonRelated::DO_BUTTON_NUN_STK, LytDoButtonRelated::ACT_IE_NONE);
    LytDoButtonRelated::set(LytDoButtonRelated::DO_BUTTON_NUN_BG, LytDoButtonRelated::ACT_IE_NONE);

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

bool dLytMeterMain_c::draw() {
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

dLytMeter_c::dLytMeter_c() {
    sInstance = this;
}

bool dLytMeter_c::build() {
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
    mMain.build(&mResAcc);
    if (dScTitle_c::sInstance != nullptr) {
        mpDoButton = nullptr;
        mpDoButtonRelated = nullptr;
        mpTargetBird = nullptr;
    } else {
        mpDoButton = new dLytDobutton_c();
        mpDoButtonRelated = new LytDoButtonRelated();
        mpTargetBird = new dLytTargetBird_c();
    }

    if (mpDoButton != nullptr) {
        mpDoButton->build(&mResAcc);
    }
    if (mpDoButtonRelated != nullptr) {
        mpDoButtonRelated->build(&mResAcc);
    }
    if (mpTargetBird != nullptr) {
        mpTargetBird->build(&mResAcc);
    }

    fn_800D97E0(0xb);
    setVisible(true);
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

bool dLytMeter_c::remove() {
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
    mMain.remove();
    if (mpEventSkip != nullptr) {
        mpEventSkip->remove();
        delete mpEventSkip;
        mpEventSkip = nullptr;
    }
    if (mpTargetBird != nullptr) {
        mpTargetBird->remove();
        delete mpTargetBird;
        mpTargetBird = nullptr;
    }
    dLytAreaCaption_c::remove();
    mResAcc.detach();
    return true;
}

bool dLytMeter_c::execute() {
    mMain.mMinusBtn.setField_0x4E80(field_0x13B58);
    mMain.mDowsing.setField_0x54E0(field_0x13B5C);
    if (mpEventSkip != nullptr) {
        mpEventSkip->execute();
    }

    dLytAreaCaption_c::update();
    mMain.execute();
    if (mpDoButton != nullptr) {
        mpDoButton->execute();
    }

    if (mpTargetBird != nullptr) {
        mpTargetBird->execute();
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
        dLytMeter_c::setField_0x13B63(0);
        field_0x13B64 = 0;
        field_0x13B65 = 0;
    }

    if (dLytMeter_c::getField_0x13B63() && EventManager::isInEvent()) {
        field_0x13B65 = 1;
    }

    return true;
}

bool dLytMeter_c::draw() {
    if (mVisible) {
        mMain.draw();
        if (mMain.mMode != dLytMeterMain_c::MODE_PAUSE) {
            if (mpEventSkip != nullptr) {
                mpEventSkip->draw();
            }
            dLytAreaCaption_c::draw();
            if (mpDoButton != nullptr) {
                mpDoButton->draw();
            }
            if (mpTargetBird != nullptr) {
                mpTargetBird->draw();
            }
        }
    }

    return true;
}

bool dLytMeter_c::fn_800D5670() {
    return mMain.isInMapEvent();
}

void dLytMeter_c::setAreaCaptionOverrideVisibility(bool visible) {
    for (int i = 0; i < METER_NUM_PANES; i++) {
        mMain.mPanesNotHiddenByAreaCaption[i] = visible;
    }
    mMain.field_0x137B2 = visible;
}

bool dLytMeter_c::fn_800D56F0() {
    if (mMain.mpTimer != nullptr) {
        return mMain.mpTimer->getField_0x54();
    }

    return false;
}

void dLytMeter_c::fn_800D9710() {
    field_0x13B5C = 8;
}

void dLytMeter_c::setStaminaWheelPercentInternal(f32 percent) {
    mMain.mGanbariGauge.setStaminaPercent(percent);
}

void dLytMeter_c::fn_800D9730(u8 val) {
    mMain.mItemSelect.fn_800EF6B0(val);
    mMain.mGanbariGauge.setField_0x539(val != 0);
}

void dLytMeter_c::fn_800D9780(bool val) {
    if (!field_0x13B61) {
        field_0x13B60 = val;
    }
}

bool dLytMeter_c::fn_800D97A0() {
    if (field_0x13B61) {
        return false;
    }
    return field_0x13B60;
}

void dLytMeter_c::setStaminaWheelPercent(f32 percent) {
    if (sInstance != nullptr) {
        sInstance->setStaminaWheelPercentInternal(percent);
    }
}

void dLytMeter_c::fn_800D97E0(u8 arg) {
    if (sInstance != nullptr) {
        sInstance->fn_800D9730(arg);
    }
}

void dLytMeter_c::setVisible(bool b) {
    if (sInstance != nullptr) {
        sInstance->mVisible = b;
    }
}
