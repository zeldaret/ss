#include "d/lyt/meter/d_lyt_meter_z_btn.h"

#include "common.h"
#include "d/d_pad.h"
#include "d/d_sc_game.h"
#include "d/flag/storyflag_manager.h"
#include "d/lyt/d_lyt_control_game.h"
#include "d/lyt/d_lyt_unknowns.h"
#include "d/lyt/meter/d_lyt_meter.h"
#include "toBeSorted/attention.h"
#include "toBeSorted/small_sound_mgr.h"

STATE_DEFINE(dLytMeterZBtn_c, Wait);
STATE_DEFINE(dLytMeterZBtn_c, On);
STATE_DEFINE(dLytMeterZBtn_c, Active);
STATE_DEFINE(dLytMeterZBtn_c, Off);

static const d2d::LytBrlanMapping brlanMap[] = {
    {"nunBtn_01_input.brlan", "G_input_00"},
    { "nunBtn_01_call.brlan",  "G_call_00"},
    { "nunBtn_01_loop.brlan",  "G_loop_00"},
};

#define Z_BTN_ANIM_INPUT 0
#define Z_BTN_ANIM_CALL 1
#define Z_BTN_ANIM_LOOP 2

#define Z_BTN_NUM_ANIMS 3

void dLytMeterZBtn_c::initializeState_Wait() {
    mOnDelay = 15;
}
void dLytMeterZBtn_c::executeState_Wait() {
    s32 timer = mOnDelay;
    if (field_0x1C4) {
        if (field_0x1B8) {
            bool bDoUpdate = true;
            if (field_0x1B4 != LytDoButtonRelated::DO_NONE) {
                if (timer <= 0) {
                    mAnm[Z_BTN_ANIM_INPUT].setBackwardsOnce();
                    mAnm[Z_BTN_ANIM_INPUT].setToStart();
                    mAnm[Z_BTN_ANIM_INPUT].setAnimEnable(true);
                    mStateMgr.changeState(StateID_On);
                    setMessage(field_0x1B4);
                } else {
                    mOnDelay = timer - 1;
                    bDoUpdate = false;
                }
            }
            if (bDoUpdate) {
                field_0x1B0 = field_0x1B4;
            }
        } else {
            if (field_0x1B4 != LytDoButtonRelated::DO_NONE) {
                setMessage(field_0x1B4);
            }
            field_0x1B0 = field_0x1B4;
        }
    }

    if (timer == mOnDelay) {
        mOnDelay = 15;
    }
}
void dLytMeterZBtn_c::finalizeState_Wait() {}

void dLytMeterZBtn_c::initializeState_On() {}
void dLytMeterZBtn_c::executeState_On() {
    if (mAnm[Z_BTN_ANIM_INPUT].isStop2()) {
        mAnm[Z_BTN_ANIM_INPUT].setAnimEnable(false);
        mStateMgr.changeState(StateID_Active);
    }
}
void dLytMeterZBtn_c::finalizeState_On() {}

void dLytMeterZBtn_c::initializeState_Active() {}
void dLytMeterZBtn_c::executeState_Active() {
    if (field_0x1B4 != field_0x1B0 || !field_0x1B8) {
        if (field_0x1B4 == LytDoButtonRelated::DO_NONE || !field_0x1B8) {
            mAnm[Z_BTN_ANIM_INPUT].setForwardOnce();
            mAnm[Z_BTN_ANIM_INPUT].setFrame(0.0f);
            mAnm[Z_BTN_ANIM_INPUT].setAnimEnable(true);
            if (field_0x1B0 != field_0x1BC) {
                field_0x1B4 = field_0x1BC;
            }
            mStateMgr.changeState(StateID_Off);
        } else {
            setMessage(field_0x1B4);
        }
        field_0x1B0 = field_0x1B4;
    } else if (field_0x1B0 == LytDoButtonRelated::DO_NONE) {
        mAnm[Z_BTN_ANIM_INPUT].setForwardOnce();
        mAnm[Z_BTN_ANIM_INPUT].setFrame(0.0f);
        mAnm[Z_BTN_ANIM_INPUT].setAnimEnable(true);
        mStateMgr.changeState(StateID_Off);
    }
}
void dLytMeterZBtn_c::finalizeState_Active() {}

void dLytMeterZBtn_c::initializeState_Off() {}
void dLytMeterZBtn_c::executeState_Off() {
    if (mAnm[Z_BTN_ANIM_INPUT].isEndReached()) {
        mAnm[Z_BTN_ANIM_INPUT].setAnimEnable(false);
        mStateMgr.changeState(StateID_Wait);
    }
}
void dLytMeterZBtn_c::finalizeState_Off() {}

static const char *sPane = "N_collectAll_00";

static const char *sTextBoxes[] = {
    "T_zBtn_00",
    "T_zBtnS_00",
};

static const char *sWindows[] = {"W_bgP_00"};

bool dLytMeterZBtn_c::build(d2d::ResAccIf_c *resAcc) {
    mLyt.setResAcc(resAcc);
    mLyt.build("nunBtn_01.brlyt", nullptr);

    for (int i = 0; i < Z_BTN_NUM_ANIMS; i++) {
        mAnm[i].init(brlanMap[i].mFile, resAcc, mLyt.getLayout(), brlanMap[i].mName);
        mAnm[i].bind(false);
        mAnm[i].setAnimEnable(false);
    }

    mpPane = mLyt.findPane(sPane);

    for (int i = 0; i < 2; i++) {
        mpTextBoxes[i] = mLyt.getTextBox(sTextBoxes[i]);
    }

    for (int i = 0; i < 1; i++) {
        mpWindow[i] = mLyt.getWindow(sWindows[i]);
        mpSizeBox[i] = mLyt.getSizeBoxInWindow(sWindows[i]);
    }

    field_0x1BC = 0x5E;
    field_0x1B0 = 0x5E;
    field_0x1B4 = 0x5E;
    field_0x1C4 = false;
    field_0x1B8 = true;
    mpOwnerPane = nullptr;

    setMessage(0x5E);
    field_0x1C5 = 0;
    field_0x1C6 = 0;
    mOnDelay = 15;

    mAnm[Z_BTN_ANIM_INPUT].setToEnd();
    mAnm[Z_BTN_ANIM_INPUT].setAnimEnable(true);
    mAnm[Z_BTN_ANIM_CALL].setToEnd();
    mAnm[Z_BTN_ANIM_CALL].setAnimEnable(true);
    mAnm[Z_BTN_ANIM_LOOP].setAnimEnable(true);
    mLyt.calc();
    mAnm[Z_BTN_ANIM_INPUT].setAnimEnable(false);
    mAnm[Z_BTN_ANIM_CALL].setAnimEnable(false);
    mStateMgr.changeState(StateID_Wait);
    return true;
}

bool dLytMeterZBtn_c::remove() {
    for (int i = 0; i < Z_BTN_NUM_ANIMS; i++) {
        mAnm[i].unbind();
        mAnm[i].remove();
    }
    return true;
}

bool dLytMeterZBtn_c::execute() {
    if (mpOwnerPane == nullptr) {
        return true;
    }

    if (!mpOwnerPane->IsVisible()) {
        if (*mStateMgr.getStateID() == StateID_Off) {
            mAnm[Z_BTN_ANIM_INPUT].setToEnd2();
        } else if (*mStateMgr.getStateID() == StateID_Active) {
            mAnm[Z_BTN_ANIM_INPUT].setForwardOnce();
            mAnm[Z_BTN_ANIM_INPUT].setToEnd2();
            mAnm[Z_BTN_ANIM_INPUT].setAnimEnable(true);
            mStateMgr.changeState(StateID_Off);
        }
        LytDoButtonRelated::set(LytDoButtonRelated::DO_BUTTON_Z, LytDoButtonRelated::DO_NONE);
        return true;
    }
    field_0x1C4 = true;
    if ((!fn_80081FE0(dScGame_c::getCamera(0)->getField_0xD98(), "mogu") || AttentionManager::GetInstance()->checkLink2()) &&
        LytDoButtonRelated::get(LytDoButtonRelated::DO_BUTTON_Z) == 0x67) {
        field_0x1C4 = false;
        LytDoButtonRelated::set(LytDoButtonRelated::DO_BUTTON_Z, LytDoButtonRelated::DO_NONE);
    }
    field_0x1B4 = LytDoButtonRelated::get(LytDoButtonRelated::DO_BUTTON_Z);
    field_0x1B8 = LytDoButtonRelated::getHas(LytDoButtonRelated::DO_BUTTON_Z) &&
                  dLytMeter_c::GetInstance()->checkAllFlags(0x800);
    mStateMgr.executeState();

    for (int i = 0; i < Z_BTN_NUM_ANIMS; i++) {
        if (i != 1 && mAnm[i].isEnabled()) {
            mAnm[i].play();
        }
    }

    if (dLytControlGame_c::getInstance()->isStateNormal()) {
        if (StoryflagManager::sInstance->getCounterOrFlag(565)) {
            if (dPad::checkButtonZPressed()) {
                field_0x1C5 = 0;
                StoryflagManager::sInstance->unsetFlag(565);
            } else {
                field_0x1C5 = 1;
            }
        }
    } else {
        field_0x1C5 = 0;
    }

    if (field_0x1C5 != 0 && dLytMeter_c::GetInstance()->checkAllFlags(0x800)) {
        if (field_0x1C6 == 0) {
            mAnm[Z_BTN_ANIM_CALL].setToEnd();
            mAnm[Z_BTN_ANIM_CALL].setAnimEnable(true);
            field_0x1C6 = 1;
        }
        if (mAnm[Z_BTN_ANIM_CALL].isEnabled()) {
            mAnm[Z_BTN_ANIM_CALL].play();
            if (dLytMeter_c::GetInstance()->checkAllFlags(0x800) && mAnm[Z_BTN_ANIM_CALL].getFrame() == 1.0f) {
                SmallSoundManager::GetInstance()->playSound(SE_S_Z_BUTTON_BLINK);
            }
        }
    } else if (field_0x1C6 == 1) {
        mAnm[Z_BTN_ANIM_CALL].setToEnd();
    } else if (mAnm[Z_BTN_ANIM_CALL].isEnabled()) {
        mAnm[Z_BTN_ANIM_CALL].setAnimEnable(false);
    }

    if (field_0x1C5 == 0) {
        field_0x1C6 = field_0x1C5;
    }
    field_0x1C5 = 0;

    return true;
}

#include "d/lyt/meter/d_lyt_meter_action_table.inc"

void dLytMeterZBtn_c::setMessage(s32 id) {
    char buf[11];
    for (int i = 0; i < 11; i++) {
        buf[i] = 0;
    }

    if (id < LytDoButtonRelated::DO_NONE) {
        if (id < 0x5D) {
            sprintf(buf, "ACT_INFO_%03d", sActIds[id]);
        } else {
            s32 id2 = id - 0x5E;
            sprintf(buf, "ACT_ETC_%03d", sActIds[id2]);
        }

        for (int i = 0; i < 2; i++) {
            mpTextBoxes[i]->SetVisible(true);
            mpTextBoxes[i]->setMessageWithGlobalTextProcessor2(buf, nullptr);
        }
        mpWindow[0]->SetVisible(true);
        mpWindow[0]->UpdateSize(mpSizeBox[0], 32.0f);
        field_0x1BC = id;
    } else {
        mpTextBoxes[0]->SetVisible(false);
        mpTextBoxes[1]->SetVisible(false);
        mpWindow[0]->SetVisible(false);
    }
}

bool dLytMeterZBtn_c::isCalling() const {
    if (dLytControlGame_c::getInstance()->isStateNormal() && StoryflagManager::sInstance->getCounterOrFlag(565)) {
        return true;
    }

    return false;
}
