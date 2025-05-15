#include "d/lyt/meter/d_lyt_meter_cross_btn.h"

#include "common.h"
#include "d/a/d_a_player.h"
#include "d/d_message.h"
#include "d/flag/storyflag_manager.h"
#include "d/lyt/d2d.h"
#include "d/lyt/d_lyt_unknowns.h"
#include "d/lyt/meter/d_lyt_meter.h"
#include "toBeSorted/event_manager.h"
#include "toBeSorted/fi_context.h"
#include "toBeSorted/small_sound_mgr.h"

STATE_DEFINE(dLytMeterCrossBtnParts_c, Wait);
STATE_DEFINE(dLytMeterCrossBtnParts_c, On);
STATE_DEFINE(dLytMeterCrossBtnParts_c, Active);
STATE_DEFINE(dLytMeterCrossBtnParts_c, Off);

STATE_DEFINE(dLytMeterCrossBtn_c, Wait);
STATE_DEFINE(dLytMeterCrossBtn_c, ToUse);
STATE_DEFINE(dLytMeterCrossBtn_c, ToUnuse);
STATE_DEFINE(dLytMeterCrossBtn_c, Unuse);

void dLytMeterCrossBtnParts_c::initializeState_Wait() {
    mOnDelay = 15;
}
void dLytMeterCrossBtnParts_c::executeState_Wait() {
    if (dLytMeter_c::GetMain()->fn_800D5650()) {
        mOnDelay = 0;
    }

    s32 timer = mOnDelay;
    if (field_0x7C) {
        if (field_0x68) {
            bool bDoUpdate = true;
            if (field_0x64 != LytDoButtonRelated::ACT_IE_NONE || field_0x70 != field_0x74) {
                if (timer <= 0) {
                    mStateMgr.changeState(StateID_On);
                    setMessage(field_0x64);
                } else {
                    bDoUpdate = false;
                    mOnDelay -= 1;
                }
            }
            if (bDoUpdate) {
                field_0x60 = field_0x64;
                field_0x70 = field_0x74;
            }
        } else {
            if (field_0x64 != LytDoButtonRelated::ACT_IE_NONE || field_0x70 != field_0x74) {
                setMessage(field_0x64);
            }
            field_0x60 = field_0x64;
            field_0x70 = field_0x74;
        }
    }

    if (timer == mOnDelay) {
        mOnDelay = 15;
    }
}
void dLytMeterCrossBtnParts_c::finalizeState_Wait() {}

void dLytMeterCrossBtnParts_c::initializeState_On() {
    mpAnm[0]->setBackwardsOnce();
    mpAnm[0]->setToStart();
    mpAnm[0]->setAnimEnable(true);
}
void dLytMeterCrossBtnParts_c::executeState_On() {
    if (mpAnm[0]->isStop2()) {
        mpAnm[0]->setAnimEnable(false);
        mStateMgr.changeState(StateID_Active);
    }
}
void dLytMeterCrossBtnParts_c::finalizeState_On() {}

void dLytMeterCrossBtnParts_c::initializeState_Active() {}
void dLytMeterCrossBtnParts_c::executeState_Active() {
    if (field_0x64 != field_0x60 || !field_0x68 || field_0x70 != field_0x74) {
        if ((field_0x64 == LytDoButtonRelated::ACT_IE_NONE && field_0x74 == 6) || !field_0x68) {
            if (field_0x60 != field_0x6C) {
                field_0x64 = field_0x6C;
            }
            mStateMgr.changeState(StateID_Off);
        } else {
            setMessage(field_0x64);
        }
        field_0x60 = field_0x64;
        field_0x70 = field_0x74;
    } else if (field_0x60 == LytDoButtonRelated::ACT_IE_NONE && field_0x74 == 6) {
        mStateMgr.changeState(StateID_Off);
    }
}
void dLytMeterCrossBtnParts_c::finalizeState_Active() {}

void dLytMeterCrossBtnParts_c::initializeState_Off() {
    mpAnm[0]->setForwardOnce();
    mpAnm[0]->setToStart();
    mpAnm[0]->setAnimEnable(true);
}
void dLytMeterCrossBtnParts_c::executeState_Off() {
    if (mpAnm[0]->isStop2()) {
        mpAnm[0]->setAnimEnable(false);
        mStateMgr.changeState(StateID_Wait);
    }
}
void dLytMeterCrossBtnParts_c::finalizeState_Off() {}

void dLytMeterCrossBtnParts_c::init() {
    field_0x6C = LytDoButtonRelated::ACT_IE_NONE;
    field_0x60 = LytDoButtonRelated::ACT_IE_NONE;
    field_0x64 = LytDoButtonRelated::ACT_IE_NONE;
    field_0x7C = false;
    field_0x70 = 6;
    field_0x68 = 1;
    mOnDelay = 15;
    mpOwnerPane = 0;
    setMessage(LytDoButtonRelated::ACT_IE_NONE);
    mStateMgr.changeState(StateID_Wait);
}

extern "C" bool checkIsInSkykeepPuzzle();
void dLytMeterCrossBtnParts_c::execute(bool bIsVisible) {
    if (mpOwnerPane == nullptr) {
        return;
    }

    if (!mpOwnerPane->IsVisible()) {
        if (!(*mStateMgr.getStateID() == StateID_On)) {
            return;
        }
        field_0x70 = 6;
        mpAnm[0]->setForwardOnce();
        mpAnm[0]->setToEnd2();
        mpAnm[0]->setAnimEnable(true);
        mpLyt->getLayout()->Animate(0);
        mpLyt->calc();
        mpAnm[0]->setAnimEnable(false);
        mStateMgr.changeState(StateID_Wait);
        return;
    }

    if (dLytMeter_c::GetInstance()->getMeterField_0x13750() == 0) {
        if (mIndex == 0) {
            LytDoButtonRelated::fn_8010EC10(LytDoButtonRelated::ACT_IE_NONE, true);
        } else if (mIndex == 1) {
            LytDoButtonRelated::fn_8010ED50(LytDoButtonRelated::ACT_IE_NONE, true);
        } else if (mIndex == 2) {
            LytDoButtonRelated::set(LytDoButtonRelated::DO_BUTTON_CROSS_L, LytDoButtonRelated::ACT_IE_NONE);
        } else if (mIndex == 3) {
            LytDoButtonRelated::set(LytDoButtonRelated::DO_BUTTON_CROSS_R, LytDoButtonRelated::ACT_IE_NONE);
        }

        if (field_0x64 != LytDoButtonRelated::ACT_IE_NONE) {
            field_0x64 = LytDoButtonRelated::ACT_IE_NONE;
            field_0x60 = LytDoButtonRelated::ACT_IE_NONE;
        }
        setMessage(field_0x64);
    }

    if (!bIsVisible && *mStateMgr.getStateID() == StateID_Off) {
        mpAnm[0]->setToEnd();
        mpLyt->calc();
    }

    if ((EventManager::isInEvent() && dMessage_c::getInstance()->getField_0x32C() == 12 && !checkIsInSkykeepPuzzle() &&
         !dLytMeter_c::GetInstance()->fn_800D5670()) ||
        ((dLytMeter_c::getItemSelect0x75A2() && (mIndex != 1 || field_0x74 != 0)) || !bIsVisible)) {
        field_0x7C = 0;

        if (mIndex == 0) {
            LytDoButtonRelated::fn_8010EC10(LytDoButtonRelated::ACT_IE_NONE, true);
        } else if (mIndex == 1) {
            LytDoButtonRelated::fn_8010ED50(LytDoButtonRelated::ACT_IE_NONE, true);
        } else if (mIndex == 2) {
            LytDoButtonRelated::set(LytDoButtonRelated::DO_BUTTON_CROSS_L, LytDoButtonRelated::ACT_IE_NONE);
        } else if (mIndex == 3) {
            LytDoButtonRelated::set(LytDoButtonRelated::DO_BUTTON_CROSS_R, LytDoButtonRelated::ACT_IE_NONE);
        }
    } else {
        field_0x7C = 1;
    }

    if (mIndex == 0) {
        field_0x64 = LytDoButtonRelated::get(LytDoButtonRelated::DO_BUTTON_CROSS_T);
        field_0x68 = LytDoButtonRelated::getHas(LytDoButtonRelated::DO_BUTTON_CROSS_T);
    } else if (mIndex == 1) {
        field_0x64 = LytDoButtonRelated::get(LytDoButtonRelated::DO_BUTTON_CROSS_D);
        field_0x68 = LytDoButtonRelated::getHas(LytDoButtonRelated::DO_BUTTON_CROSS_D);
    } else if (mIndex == 2) {
        field_0x64 = LytDoButtonRelated::get(LytDoButtonRelated::DO_BUTTON_CROSS_L);
        field_0x68 = LytDoButtonRelated::getHas(LytDoButtonRelated::DO_BUTTON_CROSS_L);
    } else if (mIndex == 3) {
        field_0x64 = LytDoButtonRelated::get(LytDoButtonRelated::DO_BUTTON_CROSS_R);
        field_0x68 = LytDoButtonRelated::getHas(LytDoButtonRelated::DO_BUTTON_CROSS_R);
    }

    mStateMgr.executeState();

    for (int i = 0; i < 2; i++) {
        if (mpAnm[i]->isEnabled()) {
            mpAnm[i]->play();
        }
    }
}

#include "d/lyt/meter/d_lyt_meter_action_table.inc"

void dLytMeterCrossBtnParts_c::setMessage(s32 id) {
    char buf[11];
    for (int i = 0; i < 11; i++) {
        buf[i] = 0;
    }

    // If only there was an easier way to write this
    if (field_0x74 == 0 || field_0x74 == 1) {
        mpWindow->SetVisible(false);
        mpAnm[1]->setFrame(0.0f);
        mpAnm[1]->setAnimEnable(true);
        mpTextBoxes[0]->SetVisible(false);
        mpTextBoxes[1]->SetVisible(false);
        mpLyt->getLayout()->Animate(0);
        mpLyt->calc();
        mpAnm[1]->setAnimEnable(false);
    } else if (field_0x74 == 2) {
        mpWindow->SetVisible(false);
        mpAnm[1]->setFrame(2.0f);
        mpAnm[1]->setAnimEnable(true);
        mpTextBoxes[0]->SetVisible(false);
        mpTextBoxes[1]->SetVisible(false);
        mpLyt->getLayout()->Animate(0);
        mpLyt->calc();
        mpAnm[1]->setAnimEnable(false);
    } else if (field_0x74 == 3) {
        mpWindow->SetVisible(false);
        mpAnm[1]->setFrame(3.0f);
        mpAnm[1]->setAnimEnable(true);
        mpTextBoxes[0]->SetVisible(false);
        mpTextBoxes[1]->SetVisible(false);
        mpLyt->getLayout()->Animate(0);
        mpLyt->calc();
        mpAnm[1]->setAnimEnable(false);
    } else if (field_0x74 == 4) {
        mpWindow->SetVisible(false);
        mpAnm[1]->setFrame(4.0f);
        mpAnm[1]->setAnimEnable(true);
        mpTextBoxes[0]->SetVisible(false);
        mpTextBoxes[1]->SetVisible(false);
        mpLyt->getLayout()->Animate(0);
        mpLyt->calc();
        mpAnm[1]->setAnimEnable(false);
    } else if (field_0x74 == 5) {
        mpWindow->SetVisible(false);
        mpAnm[1]->setFrame(5.0f);
        mpAnm[1]->setAnimEnable(true);
        mpTextBoxes[0]->SetVisible(false);
        mpTextBoxes[1]->SetVisible(false);
        mpLyt->getLayout()->Animate(0);
        mpLyt->calc();
        mpAnm[1]->setAnimEnable(false);
    } else if (id < LytDoButtonRelated::ACT_IE_NONE) {
        if (id < LytDoButtonRelated::ACT_IE_SEPARATOR) {
            sprintf(buf, "ACT_INFO_%03d", sActIds[id]);
        } else {
            s32 id2 = id - LytDoButtonRelated::ACT_IE_SEPARATOR - 1;
            sprintf(buf, "ACT_ETC_%03d", sActIds[id2]);
        }

        for (int i = 0; i < 2; i++) {
            mpTextBoxes[i]->SetVisible(true);
            mpTextBoxes[i]->setMessageWithGlobalTextProcessor2(buf, nullptr);
        }
        mpWindow->SetVisible(true);
        mpWindow->UpdateSize(mpSizeBox, 32.0f);
        field_0x6C = id;
        mpAnm[1]->setFrame(1.0f);
        mpAnm[1]->setAnimEnable(true);
        mpLyt->getLayout()->Animate(0);
        mpLyt->calc();
        mpAnm[1]->setAnimEnable(false);
    } else {
        mpWindow->SetVisible(false);
        mpAnm[1]->setFrame(1.0f);
        mpAnm[1]->setAnimEnable(true);
        mpLyt->getLayout()->Animate(0);
        mpLyt->calc();
        mpAnm[1]->setAnimEnable(false);
    }
}

static const d2d::LytBrlanMapping brlanMap[] = {
    {  "remoConBtn_03_input.brlan", "G_inputT_00"},
    {  "remoConBtn_03_input.brlan", "G_inputD_00"},
    {  "remoConBtn_03_input.brlan", "G_inputL_00"},
    {  "remoConBtn_03_input.brlan", "G_inputR_00"},
    {"remoConBtn_03_pattern.brlan",      "N_T_00"},
    {"remoConBtn_03_pattern.brlan",      "N_D_00"},
    {"remoConBtn_03_pattern.brlan",      "N_L_00"},
    {"remoConBtn_03_pattern.brlan",      "N_R_00"},
    {   "remoConBtn_03_call.brlan",   "G_call_00"},
    {   "remoConBtn_03_call.brlan",   "G_call_01"},
    {   "remoConBtn_03_call.brlan",   "G_call_02"},
    {   "remoConBtn_03_loop.brlan",   "G_loop_00"},
};

#define CROSS_BTN_ANIM_INPUT_T 0
#define CROSS_BTN_ANIM_INPUT_OFFSET 0
#define CROSS_BTN_ANIM_PATTERN_OFFSET 4
#define CROSS_BTN_ANIM_CALL_0 8
#define CROSS_BTN_ANIM_CALL_1 9
#define CROSS_BTN_ANIM_CALL_2 10
#define CROSS_BTN_ANIM_LOOP 11

#define CROSS_BTN_NUM_ANIMS 12

void dLytMeterCrossBtn_c::initializeState_Wait() {}
void dLytMeterCrossBtn_c::executeState_Wait() {}
void dLytMeterCrossBtn_c::finalizeState_Wait() {}

void dLytMeterCrossBtn_c::initializeState_ToUse() {
    mAnm[CROSS_BTN_ANIM_INPUT_T].setBackwardsOnce();
    mAnm[CROSS_BTN_ANIM_INPUT_T].setToStart();
    mAnm[CROSS_BTN_ANIM_INPUT_T].setAnimEnable(true);
}
void dLytMeterCrossBtn_c::executeState_ToUse() {
    if (mAnm[CROSS_BTN_ANIM_INPUT_T].isStop2()) {
        mStateMgr.changeState(StateID_Wait);
    }

    if (mAnm[CROSS_BTN_ANIM_INPUT_T].isEnabled()) {
        mAnm[CROSS_BTN_ANIM_INPUT_T].play();
    }
}
void dLytMeterCrossBtn_c::finalizeState_ToUse() {
    mAnm[CROSS_BTN_ANIM_INPUT_T].setAnimEnable(false);
}

void dLytMeterCrossBtn_c::initializeState_ToUnuse() {
    mAnm[CROSS_BTN_ANIM_INPUT_T].setForwardOnce();
    mAnm[CROSS_BTN_ANIM_INPUT_T].setToStart();
    mAnm[CROSS_BTN_ANIM_INPUT_T].setAnimEnable(true);
}
void dLytMeterCrossBtn_c::executeState_ToUnuse() {
    if (mAnm[CROSS_BTN_ANIM_INPUT_T].isStop2()) {
        mStateMgr.changeState(StateID_Unuse);
    }

    if (mAnm[CROSS_BTN_ANIM_INPUT_T].isEnabled()) {
        mAnm[CROSS_BTN_ANIM_INPUT_T].play();
    }
}
void dLytMeterCrossBtn_c::finalizeState_ToUnuse() {
    mAnm[CROSS_BTN_ANIM_INPUT_T].setAnimEnable(false);
}

void dLytMeterCrossBtn_c::initializeState_Unuse() {}
void dLytMeterCrossBtn_c::executeState_Unuse() {
    if (dLytMeter_c::GetInstance()->checkAllFlags(METER_BTN_CROSS_UP)) {
        mStateMgr.changeState(StateID_ToUse);
    }
}
void dLytMeterCrossBtn_c::finalizeState_Unuse() {}

static const char *sPane = "N_all_00";

static const char *sTextBoxes[] = {
    "T_textT_00", "T_textD_00", "T_textL_00", "T_textR_00", "T_textTS_00", "T_textDS_00", "T_textLS_00", "T_textRS_00",
};

static const char *sWindows[] = {"W_bgP_01", "W_bgP_00", "W_bgP_02", "W_bgP_03"};

bool dLytMeterCrossBtn_c::build(d2d::ResAccIf_c *resAcc) {
    mLyt.setResAcc(resAcc);
    mLyt.build("remoConBtn_03.brlyt", nullptr);

    for (int i = 0; i < CROSS_BTN_NUM_ANIMS; i++) {
        mAnm[i].init(brlanMap[i].mFile, resAcc, mLyt.getLayout(), brlanMap[i].mName);
        mAnm[i].bind(false);
        mAnm[i].setAnimEnable(false);
    }

    mpPane = mLyt.findPane(sPane);

    for (int i = 0; i < 8; i++) {
        mpTextboxes[i] = mLyt.getTextBox(sTextBoxes[i]);
    }

    for (int i = 0; i < 4; i++) {
        mpWindows[i] = mLyt.getWindow(sWindows[i]);
        mpSizeBoxes[i] = mLyt.getSizeBoxInWindow(sWindows[i]);
    }

    for (int i = 0; i < 4; i++) {
        mParts[i].mIndex = i;
        mParts[i].mpLyt = &mLyt;
        mParts[i].mpAnm[0] = &mAnm[i + CROSS_BTN_ANIM_INPUT_OFFSET];
        mParts[i].mpAnm[1] = &mAnm[i + CROSS_BTN_ANIM_PATTERN_OFFSET];
        mParts[i].mpTextBoxes[0] = mpTextboxes[i];
        mParts[i].mpTextBoxes[1] = mpTextboxes[i + 4];
        mParts[i].mpWindow = mpWindows[i];
        mParts[i].mpSizeBox = mpSizeBoxes[i];
        mParts[i].field_0x74 = 6;
        mParts[i].init();
    }

    field_0x634 = 0;
    field_0x635 = 0;
    field_0x636 = 0;

    mSavedFiFlow = 0xFFFF;
    mCallCount0 = 0;
    mCallCount1 = 0;
    field_0x620 = 6;
    field_0x624 = 6;

    field_0x637 = 0;
    field_0x638 = 0;
    field_0x639 = 0;
    field_0x63A = 0;
    field_0x63B = 0;
    field_0x63C = 0;

    mpOwnerPane = nullptr;

    for (int i = 0; i < 4; i++) {
        mAnm[i + CROSS_BTN_ANIM_INPUT_OFFSET].setToEnd();
        mAnm[i + CROSS_BTN_ANIM_INPUT_OFFSET].setAnimEnable(true);
    }

    mAnm[CROSS_BTN_ANIM_CALL_0].setToEnd();
    mAnm[CROSS_BTN_ANIM_CALL_0].setAnimEnable(true);
    mAnm[CROSS_BTN_ANIM_CALL_1].setToEnd();
    mAnm[CROSS_BTN_ANIM_CALL_1].setAnimEnable(true);
    mAnm[CROSS_BTN_ANIM_CALL_2].setToEnd();
    mAnm[CROSS_BTN_ANIM_CALL_2].setAnimEnable(true);

    mLyt.calc();

    for (int i = 0; i < 4; i++) {
        mAnm[i + CROSS_BTN_ANIM_INPUT_OFFSET].setAnimEnable(false);
    }

    mAnm[CROSS_BTN_ANIM_CALL_0].setAnimEnable(false);
    mAnm[CROSS_BTN_ANIM_CALL_1].setAnimEnable(false);
    mAnm[CROSS_BTN_ANIM_CALL_2].setAnimEnable(false);
    mAnm[CROSS_BTN_ANIM_LOOP].setAnimEnable(true);

    mStateMgr.changeState(StateID_Wait);

    return true;
}

bool dLytMeterCrossBtn_c::remove() {
    for (int i = 0; i < CROSS_BTN_NUM_ANIMS; i++) {
        mAnm[i].unbind();
        mAnm[i].remove();
    }
    return true;
}

bool dLytMeterCrossBtn_c::execute() {
    u16 newFiFlow = FiContext::prepareFiHelpIndex();
    if (newFiFlow != mSavedFiFlow) {
        mSavedFiFlow = newFiFlow;
        mCallCount0 = 0;
    }

    mParts[0].field_0x74 = field_0x624;
    mParts[1].field_0x74 = field_0x620;

    if (mpOwnerPane != nullptr) {
        for (int i = 0; i < 4; i++) {
            mParts[i].execute(mpOwnerPane->IsVisible());
        }
    }

    mStateMgr.executeState();

    if (StoryflagManager::sInstance->getCounterOrFlag(43)) {
        field_0x63B = 1;
    }

    if (field_0x637 != 0) {
        if (field_0x620 == 0) {
            f32 frame = dAcPy_c::GetLink2()->getAnmMatClrFrame();
            if (frame == 0.0f) {
                mAnm[CROSS_BTN_ANIM_CALL_0].setToEnd();
                if (!mAnm[CROSS_BTN_ANIM_CALL_0].isEnabled()) {
                    mAnm[CROSS_BTN_ANIM_CALL_0].setAnimEnable(true);
                }
            }

            if (mAnm[CROSS_BTN_ANIM_CALL_0].isEnabled()) {
                mAnm[CROSS_BTN_ANIM_CALL_0].play();
                if (mParts[1].isActive() && mCallCount0 < 3 && mAnm[CROSS_BTN_ANIM_CALL_0].getFrame() == 1.0f) {
                    SmallSoundManager::GetInstance()->playSound(SE_S_SG_CALL);
                    mCallCount0++;
                }
            }
        }
    } else if (field_0x638 == 1) {
        mAnm[CROSS_BTN_ANIM_CALL_0].setToEnd();
    } else if (mAnm[CROSS_BTN_ANIM_CALL_0].isEnabled()) {
        mAnm[CROSS_BTN_ANIM_CALL_0].setAnimEnable(false);
    }

    if (field_0x639 != 0) {
        if (field_0x624 == 3) {
            if (field_0x63A == 0) {
                mAnm[CROSS_BTN_ANIM_CALL_1].setToEnd();
                mAnm[CROSS_BTN_ANIM_CALL_1].setAnimEnable(true);
            }
            if (mAnm[CROSS_BTN_ANIM_CALL_1].isEnabled()) {
                mAnm[CROSS_BTN_ANIM_CALL_1].play();
                if (mParts[0].isActive() && mCallCount1 < 3 && mAnm[CROSS_BTN_ANIM_CALL_1].getFrame() == 1.0f) {
                    SmallSoundManager::GetInstance()->playSound(SE_S_CATAPULT_READY);
                    mCallCount1++;
                }
            }
        }
    } else if (field_0x63A == 1) {
        mAnm[CROSS_BTN_ANIM_CALL_1].setToEnd();
    } else if (mAnm[CROSS_BTN_ANIM_CALL_1].isEnabled()) {
        mAnm[CROSS_BTN_ANIM_CALL_1].setAnimEnable(false);
    }

    if (field_0x63B != 0) {
        if (field_0x620 == 2) {
            if (field_0x63C == 0) {
                mAnm[CROSS_BTN_ANIM_CALL_2].setToEnd();
                mAnm[CROSS_BTN_ANIM_CALL_2].setRate(2.0f);
                mAnm[CROSS_BTN_ANIM_CALL_2].setAnimEnable(true);
            }
            if (mAnm[CROSS_BTN_ANIM_CALL_2].isEnabled()) {
                mAnm[CROSS_BTN_ANIM_CALL_2].play();
                if (mParts[1].isActive() && mAnm[CROSS_BTN_ANIM_CALL_2].getFrame() == 1.0f) {
                    SmallSoundManager::GetInstance()->playSound(SE_S_BIRD_CALL);
                }
            }
        }
    } else if (field_0x63C == 1) {
        mAnm[CROSS_BTN_ANIM_CALL_2].setToEnd();
    } else if (mAnm[CROSS_BTN_ANIM_CALL_2].isEnabled()) {
        mAnm[CROSS_BTN_ANIM_CALL_2].setAnimEnable(false);
    }

    if (mAnm[CROSS_BTN_ANIM_LOOP].isEnabled()) {
        mAnm[CROSS_BTN_ANIM_LOOP].play();
    }

    field_0x620 = 6;
    field_0x624 = 6;
    // Might be arrays
    field_0x638 = field_0x637;
    field_0x637 = 0;
    field_0x63A = field_0x639;
    field_0x639 = 0;
    field_0x63C = field_0x63B;
    field_0x63B = 0;

    return true;
}

void dLytMeterCrossBtn_c::setOwnerPane(nw4r::lyt::Pane *pane) {
    mpOwnerPane = pane;
    for (int i = 0; i < 4; i++) {
        mParts[i].mpOwnerPane = pane;
    }
}

bool dLytMeterCrossBtn_c::fn_800FA730() const {
    if (field_0x620 == 2) {
        return true;
    }

    return field_0x637 != 0 || field_0x639 != 0 || field_0x63B != 0;
}
