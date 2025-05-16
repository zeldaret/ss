#include "d/lyt/meter/d_lyt_meter_a_btn.h"

#include "common.h"
#include "d/lyt/d_lyt_do_button.h"
#include "d/lyt/d_lyt_unknowns.h"
#include "d/lyt/d_textbox.h"
#include "d/lyt/d_window.h"
#include "d/lyt/meter/d_lyt_meter.h"
#include "toBeSorted/event_manager.h"
#include "toBeSorted/misc_actor.h"

STATE_DEFINE(dLytMeterABtn_c, InvisibleWait);
STATE_DEFINE(dLytMeterABtn_c, In);
STATE_DEFINE(dLytMeterABtn_c, Out);
STATE_DEFINE(dLytMeterABtn_c, Wait);
STATE_DEFINE(dLytMeterABtn_c, On);
STATE_DEFINE(dLytMeterABtn_c, Active);
STATE_DEFINE(dLytMeterABtn_c, Off);

static const d2d::LytBrlanMapping brlanMap[] = {
    {"remoConBtn_06_input.brlan", "G_input_00"},
    { "remoConBtn_06_text.brlan",  "G_text_00"},
    { "remoConBtn_06_loop.brlan",  "G_loop_00"},
};

#define A_BTN_ANIM_INPUT 0
#define A_BTN_ANIM_TEXT 1
#define A_BTN_ANIM_LOOP 2

#define A_BTN_NUM_ANIMS 3

void dLytMeterABtn_c::initializeState_InvisibleWait() {}
void dLytMeterABtn_c::executeState_InvisibleWait() {
    if (field_0x1C8 == 0) {
        return;
    }

    if (field_0x1CA == 0) {
        return;
    }

    mStateMgr.changeState(StateID_In);
}
void dLytMeterABtn_c::finalizeState_InvisibleWait() {}

void dLytMeterABtn_c::initializeState_In() {
    mpContainerAnmGroup1->setToStart();
    mpContainerAnmGroup1->setAnimEnable(true);
    mAnm[A_BTN_ANIM_INPUT].setBackwardsOnce();
    mAnm[A_BTN_ANIM_INPUT].setToStart();
}
void dLytMeterABtn_c::executeState_In() {
    if (mpContainerAnmGroup1->isEndReached()) {
        mpContainerAnmGroup1->setAnimEnable(false);
        mStateMgr.changeState(StateID_Wait);
    }
    if (mpContainerAnmGroup1->isEnabled()) {
        mpContainerAnmGroup1->play();
    }
}
void dLytMeterABtn_c::finalizeState_In() {}

void dLytMeterABtn_c::initializeState_Out() {
    mpContainerAnmGroup2->setToStart();
    mpContainerAnmGroup2->setAnimEnable(true);
}
void dLytMeterABtn_c::executeState_Out() {
    if (mpContainerAnmGroup2->isEndReached()) {
        mpContainerAnmGroup2->setAnimEnable(false);
        mStateMgr.changeState(StateID_InvisibleWait);
    }
    if (mpContainerAnmGroup2->isEnabled()) {
        mpContainerAnmGroup2->play();
    }
}
void dLytMeterABtn_c::finalizeState_Out() {}

void dLytMeterABtn_c::initializeState_Wait() {}
void dLytMeterABtn_c::executeState_Wait() {
    if (field_0x1C9 == 0) {
        return;
    }

    if (field_0x1C0) {
        if (field_0x1BC != LytDoButtonRelated::ACT_IE_NONE) {
            mAnm[A_BTN_ANIM_INPUT].setBackwardsOnce();
            mAnm[A_BTN_ANIM_INPUT].setToStart();
            mAnm[A_BTN_ANIM_INPUT].setAnimEnable(true);
            mStateMgr.changeState(StateID_On);
            setMessage(field_0x1BC);
        }
        field_0x1B8 = field_0x1BC;
    } else {
        if (field_0x1BC != LytDoButtonRelated::ACT_IE_NONE) {
            setMessage(field_0x1BC);
        }
        field_0x1B8 = field_0x1BC;
    }
}
void dLytMeterABtn_c::finalizeState_Wait() {}

void dLytMeterABtn_c::initializeState_On() {}
void dLytMeterABtn_c::executeState_On() {
    if (mAnm[A_BTN_ANIM_INPUT].isStop2()) {
        mAnm[A_BTN_ANIM_INPUT].setAnimEnable(false);
        mStateMgr.changeState(StateID_Active);
    }
}
void dLytMeterABtn_c::finalizeState_On() {}

void dLytMeterABtn_c::initializeState_Active() {}
void dLytMeterABtn_c::executeState_Active() {
    if (field_0x1BC != field_0x1B8 || !field_0x1C0) {
        if (field_0x1BC == LytDoButtonRelated::ACT_IE_NONE || !field_0x1C0) {
            mAnm[A_BTN_ANIM_INPUT].setForwardOnce();
            mAnm[A_BTN_ANIM_INPUT].setFrame(0.0f);
            mAnm[A_BTN_ANIM_INPUT].setAnimEnable(true);
            if (field_0x1B8 != field_0x1C4) {
                field_0x1BC = field_0x1C4;
            }
            mStateMgr.changeState(StateID_Off);
        } else {
            setMessage(field_0x1BC);
        }
        field_0x1B8 = field_0x1BC;
    } else if (field_0x1B8 == LytDoButtonRelated::ACT_IE_NONE) {
        mAnm[A_BTN_ANIM_INPUT].setForwardOnce();
        mAnm[A_BTN_ANIM_INPUT].setFrame(0.0f);
        mAnm[A_BTN_ANIM_INPUT].setAnimEnable(true);
        mStateMgr.changeState(StateID_Off);
    }
}
void dLytMeterABtn_c::finalizeState_Active() {}

void dLytMeterABtn_c::initializeState_Off() {}
void dLytMeterABtn_c::executeState_Off() {
    if (mAnm[A_BTN_ANIM_INPUT].isEndReached()) {
        mAnm[A_BTN_ANIM_INPUT].setAnimEnable(false);
        mStateMgr.changeState(StateID_Wait);
    }
}
void dLytMeterABtn_c::finalizeState_Off() {}

static const char *sPane = "N_all_00";

static const char *sTextBoxes[] = {
    "T_aBtn_00",
    "T_aBtnS_00",
};

static const char *sWindows[] = {"W_bgP_00"};

bool dLytMeterABtn_c::build(d2d::ResAccIf_c *resAcc) {
    mLyt.setResAcc(resAcc);
    mLyt.build("remoConBtn_06.brlyt", nullptr);

    for (int i = 0; i < A_BTN_NUM_ANIMS; i++) {
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

    field_0x1C4 = LytDoButtonRelated::ACT_IE_ETC_DASH;
    field_0x1B8 = LytDoButtonRelated::ACT_IE_ETC_DASH;
    field_0x1BC = LytDoButtonRelated::ACT_IE_ETC_DASH;
    field_0x1C8 = 0;
    field_0x1CA = 0;
    field_0x1C9 = 0;
    field_0x1C0 = true;
    mOldTextState = false;
    mpOwnerPane = nullptr;
    mAnm[A_BTN_ANIM_TEXT].setAnimEnable(true);
    setMessage(field_0x1B8);
    mAnm[A_BTN_ANIM_TEXT].setFrame(0.0f);
    mAnm[A_BTN_ANIM_INPUT].setToEnd();
    mAnm[A_BTN_ANIM_INPUT].setAnimEnable(true);
    mAnm[A_BTN_ANIM_LOOP].setAnimEnable(true);
    mLyt.calc();
    mAnm[A_BTN_ANIM_INPUT].setAnimEnable(false);
    mStateMgr.changeState(StateID_Wait);
    return true;
}

bool dLytMeterABtn_c::remove() {
    for (int i = 0; i < A_BTN_NUM_ANIMS; i++) {
        mAnm[i].unbind();
        mAnm[i].remove();
    }
    return true;
}

bool dLytMeterABtn_c::execute() {
    if (mpOwnerPane == nullptr) {
        return true;
    }

    if (!mpOwnerPane->IsVisible()) {
        if (*mStateMgr.getStateID() == StateID_Off) {
            mAnm[A_BTN_ANIM_INPUT].setToEnd2();
        }

        LytDoButtonRelated::set(LytDoButtonRelated::DO_BUTTON_A, LytDoButtonRelated::ACT_IE_NONE);

        return true;
    }

    if (dLytMeter_c::GetInstance()->getMeterField_0x13750() == 0) {
        LytDoButtonRelated::set(LytDoButtonRelated::DO_BUTTON_A, LytDoButtonRelated::ACT_IE_NONE);

        if (field_0x1BC != LytDoButtonRelated::ACT_IE_NONE) {
            field_0x1BC = LytDoButtonRelated::ACT_IE_NONE;
            field_0x1B8 = LytDoButtonRelated::ACT_IE_NONE;
        }
        setMessage(field_0x1BC);
    }

    bool state = false;

    if (EventManager::isInEvent() && !dLytMeter_c::GetInstance()->fn_800D5670() &&
        dLytMeter_c::GetMain()->getField_0x1377E() == 0 && !checkIsInSkykeepPuzzle() &&
        LytDoButtonRelated::get(LytDoButtonRelated::DO_BUTTON_A) == LytDoButtonRelated::ACT_IE_NONE) {
        field_0x1C9 = 0;

        LytDoButtonRelated::set(LytDoButtonRelated::DO_BUTTON_A, LytDoButtonRelated::ACT_IE_NONE);
    } else {
        field_0x1C9 = 1;
    }

    field_0x1CA = 1;

    // TODO
    switch (dLytDobutton_c::getField0x480()) {
        case 0x0:
        case 0x7:
        case 0xB:
        case 0x15:
        case 0x1A:
        case 0x1B:
        case 0x1C:
        case 0x1D:
        case 0x1F:
        case 0x21:
        case 0x22:
        case 0x23:
        case 0x27:
        case 0x28: {
            LytDoButtonRelated::Act_IE_e action = LytDoButtonRelated::convertDoButton(dLytDobutton_c::getField0x47C());
            if (action >= 0) {
                state = true;
                LytDoButtonRelated::set(LytDoButtonRelated::DO_BUTTON_A, action);
            }
            field_0x1CA = 0;
            break;
        }
    }

    field_0x1BC = LytDoButtonRelated::get(LytDoButtonRelated::DO_BUTTON_A);
    field_0x1C0 = LytDoButtonRelated::getHas(LytDoButtonRelated::DO_BUTTON_A);

    if (field_0x1BC == 0x76) {
        state = true;
    } else if (field_0x1BC == 0x57) {
        state = false;
    }

    mStateMgr.executeState();

    if (mOldTextState != state) {
        if (*mStateMgr.getStateID() != StateID_Off) {
            if (state) {
                mAnm[A_BTN_ANIM_TEXT].setFrame(1.0f);
            } else {
                mAnm[A_BTN_ANIM_TEXT].setFrame(0.0f);
            }
            mOldTextState = state;
        }
    }

    for (int i = 0; i < A_BTN_NUM_ANIMS; i++) {
        if (i != 1 && mAnm[i].isEnabled()) {
            mAnm[i].play();
        }
    }

    return true;
}

#include "d/lyt/meter/d_lyt_meter_action_table.inc"

void dLytMeterABtn_c::setMessage(s32 id) {
    char buf[11];
    for (int i = 0; i < 11; i++) {
        buf[i] = 0;
    }

    if (id < LytDoButtonRelated::ACT_IE_NONE) {
        if (id < LytDoButtonRelated::ACT_IE_SEPARATOR) {
            sprintf(buf, "ACT_INFO_%03d", sActIds[id]);
        } else if (id == LytDoButtonRelated::ACT_IE_SEPARATOR) {
            wchar_t b2[2];
            b2[0] = b2[1] = 0;
            for (int i = 0; i < 2; i++) {
                mpTextBoxes[i]->setTextWithGlobalTextProcessor(b2);
            }
        } else {
            s32 id2 = id - LytDoButtonRelated::ACT_IE_SEPARATOR - 1;
            sprintf(buf, "ACT_ETC_%03d", sActIds[id2]);
        }

        if (id != LytDoButtonRelated::ACT_IE_SEPARATOR) {
            for (int i = 0; i < 2; i++) {
                mpTextBoxes[i]->SetVisible(true);
                mpTextBoxes[i]->setMessageWithGlobalTextProcessor2(buf, nullptr);
            }
            mpWindow[0]->SetVisible(true);
            mpWindow[0]->UpdateSize(mpSizeBox[0], 32.0f);
        } else {
            mpTextBoxes[0]->SetVisible(false);
            mpTextBoxes[1]->SetVisible(false);
            mpWindow[0]->SetVisible(false);
        }
        field_0x1C4 = id;
    } else {
        mpTextBoxes[0]->SetVisible(false);
        mpTextBoxes[1]->SetVisible(false);
        mpWindow[0]->SetVisible(false);
    }
}
