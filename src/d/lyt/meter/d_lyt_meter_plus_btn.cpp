#include "d/lyt/meter/d_lyt_meter_plus_btn.h"

#include "common.h"
#include "d/d_stage_mgr.h"
#include "d/flag/dungeonflag_manager.h"
#include "d/flag/sceneflag_manager.h"
#include "d/flag/storyflag_manager.h"
#include "d/lyt/d_lyt_control_game.h"
#include "d/lyt/d_lyt_unknowns.h"
#include "d/lyt/d_window.h"
#include "d/lyt/meter/d_lyt_meter.h"
#include "d/t/d_t_map_mark.h"
#include "f/f_base.h"
#include "f/f_manager.h"
#include "f/f_profile_name.h"
#include "toBeSorted/small_sound_mgr.h"

STATE_DEFINE(dLytMeterPlusBtn_c, Wait);
STATE_DEFINE(dLytMeterPlusBtn_c, ToUse);
STATE_DEFINE(dLytMeterPlusBtn_c, ToUnuse);
STATE_DEFINE(dLytMeterPlusBtn_c, Unuse);

static const d2d::LytBrlanMapping brlanMap[] = {
    {"remoConBtn_02_input.brlan", "G_input_00"},
    { "remoConBtn_02_call.brlan",  "G_call_00"},
    { "remoConBtn_02_loop.brlan",  "G_loop_00"},
};

#define PLUS_BTN_ANIM_INPUT 0
#define PLUS_BTN_ANIM_CALL 1
#define PLUS_BTN_ANIM_LOOP 2

#define PLUS_BTN_NUM_ANIMS 3

void dLytMeterPlusBtn_c::initializeState_Wait() {}
void dLytMeterPlusBtn_c::executeState_Wait() {
    if (!dLytMeter_c::GetInstance()->checkAllFlags(METER_BTN_PLUS)) {
        mStateMgr.changeState(StateID_ToUnuse);
    }
}
void dLytMeterPlusBtn_c::finalizeState_Wait() {}

void dLytMeterPlusBtn_c::initializeState_ToUse() {
    mAnm[PLUS_BTN_ANIM_INPUT].setBackwardsOnce();
    mAnm[PLUS_BTN_ANIM_INPUT].setToStart();
    mAnm[PLUS_BTN_ANIM_INPUT].setAnimEnable(true);
}
void dLytMeterPlusBtn_c::executeState_ToUse() {
    if (mAnm[PLUS_BTN_ANIM_INPUT].isStop2()) {
        mStateMgr.changeState(StateID_Wait);
    }
    if (mAnm[PLUS_BTN_ANIM_INPUT].isEnabled()) {
        mAnm[PLUS_BTN_ANIM_INPUT].play();
    }
}
void dLytMeterPlusBtn_c::finalizeState_ToUse() {
    mAnm[PLUS_BTN_ANIM_INPUT].setAnimEnable(false);
}

void dLytMeterPlusBtn_c::initializeState_ToUnuse() {
    mAnm[PLUS_BTN_ANIM_INPUT].setForwardOnce();
    mAnm[PLUS_BTN_ANIM_INPUT].setToStart();
    mAnm[PLUS_BTN_ANIM_INPUT].setAnimEnable(true);
}
void dLytMeterPlusBtn_c::executeState_ToUnuse() {
    if (mAnm[PLUS_BTN_ANIM_INPUT].isStop2()) {
        mStateMgr.changeState(StateID_Unuse);
    }
    if (mAnm[PLUS_BTN_ANIM_INPUT].isEnabled()) {
        mAnm[PLUS_BTN_ANIM_INPUT].play();
    }
}
void dLytMeterPlusBtn_c::finalizeState_ToUnuse() {
    mAnm[PLUS_BTN_ANIM_INPUT].setAnimEnable(false);
}

void dLytMeterPlusBtn_c::initializeState_Unuse() {}
void dLytMeterPlusBtn_c::executeState_Unuse() {
    if (dLytMeter_c::GetInstance()->checkAllFlags(METER_BTN_PLUS)) {
        if (field_0x1BC <= 0) {
            mStateMgr.changeState(StateID_ToUse);
        } else {
            field_0x1BC--;
        }
    } else {
        field_0x1BC = 15;
    }
}
void dLytMeterPlusBtn_c::finalizeState_Unuse() {}

static const char *sPane[] = {"N_all_00"};

static const char *sTextBoxes[] = {
    "T_mapBtn_02",
    "T_mapBtnS_00",
};

static const char *sWindows[] = {"W_bgP_00"};

bool dLytMeterPlusBtn_c::build(d2d::ResAccIf_c *resAcc) {
    mLyt.setResAcc(resAcc);
    mLyt.build("remoConBtn_02.brlyt", nullptr);

    for (int i = 0; i < PLUS_BTN_NUM_ANIMS; i++) {
        mAnm[i].init(brlanMap[i].mFile, resAcc, mLyt.getLayout(), brlanMap[i].mName);
        mAnm[i].bind(false);
        mAnm[i].setAnimEnable(false);
    }

    for (int i = 0; i < 1; i++) {
        mpPane[i] = mLyt.findPane(sPane[i]);
    }

    for (int i = 0; i < 2; i++) {
        mpTextBoxes[i] = mLyt.getTextBox(sTextBoxes[i]);
    }

    for (int i = 0; i < 1; i++) {
        mpWindow[i] = mLyt.getWindow(sWindows[i]);
        mpSizeBox[i] = mLyt.getSizeBoxInWindow(sWindows[i]);
        mpWindow[i]->UpdateSize(mpSizeBox[i], 32.0f);
    }

    field_0x1B0 = LytDoButtonRelated::DO_NONE;
    field_0x1B4 = LytDoButtonRelated::DO_NONE;

    setMessage(LytDoButtonRelated::DO_NONE);

    field_0x1C0 = 0;
    field_0x1C1 = StoryflagManager::sInstance->getCounterOrFlag(212);

    mCallCount = 0;

    mpOwnerPane = nullptr;

    mShouldCall = false;
    field_0x1C3 = false;
    field_0x1BC = 15;

    mAnm[PLUS_BTN_ANIM_LOOP].setAnimEnable(true);
    mAnm[PLUS_BTN_ANIM_INPUT].setAnimEnable(true);
    mAnm[PLUS_BTN_ANIM_INPUT].setFrame(0.0f);
    mLyt.calc();
    mAnm[PLUS_BTN_ANIM_INPUT].setAnimEnable(false);
    mStateMgr.changeState(StateID_Wait);
    return true;
}

bool dLytMeterPlusBtn_c::remove() {
    for (int i = 0; i < PLUS_BTN_NUM_ANIMS; i++) {
        mAnm[i].unbind();
        mAnm[i].remove();
    }
    return true;
}

bool dLytMeterPlusBtn_c::execute() {
    if (mpOwnerPane == nullptr) {
        return true;
    }

    if (!mpOwnerPane->IsVisible()) {
        if (*mStateMgr.getStateID() == StateID_Wait && dStageMgr_c::GetInstance()->isInLastBoss()) {
            mAnm[PLUS_BTN_ANIM_INPUT].setAnimEnable(true);
            mAnm[PLUS_BTN_ANIM_INPUT].setToEnd();
            mLyt.calc();
            mAnm[PLUS_BTN_ANIM_INPUT].setAnimEnable(false);
            mStateMgr.changeState(StateID_Unuse);
        }
        return true;
    }

    // Better double check if the map is open!
    if (hasSpecificMapMark() && DungeonflagManager::sInstance->getCounterOrFlag(2, 8) && !isMapOpen() &&
        dLytMeter_c::GetInstance()->checkAllFlags(METER_BTN_PLUS) && !isMapOpen()) {
        mShouldCall = true;
    } else {
        if (dLytControlGame_c::getInstance()->isStateNormal()) {
            if (StoryflagManager::sInstance->getCounterOrFlag(567)) {
                mShouldCall = true;
            } else {
                mShouldCall = false;
            }
        } else {
            mShouldCall = false;
        }
    }

    if (mShouldCall && dLytMeter_c::GetInstance()->checkAllFlags(METER_BTN_PLUS)) {
        if (!field_0x1C3) {
            mAnm[PLUS_BTN_ANIM_CALL].setToEnd();
            mAnm[PLUS_BTN_ANIM_CALL].setAnimEnable(true);
            field_0x1C3 = true;
        }
        if (mAnm[PLUS_BTN_ANIM_CALL].isEnabled()) {
            mAnm[PLUS_BTN_ANIM_CALL].play();
            if (dLytMeter_c::GetInstance()->checkAllFlags(METER_BTN_PLUS) &&
                mAnm[PLUS_BTN_ANIM_CALL].getFrame() == 1.0f && mCallCount < 3) {
                SmallSoundManager::GetInstance()->playSound(SE_S_PLUS_BUTTON_BLINK);
                mCallCount++;
            }
        }
    } else if (field_0x1C3 == true) {
        mAnm[PLUS_BTN_ANIM_CALL].setToEnd();
    } else if (mAnm[PLUS_BTN_ANIM_CALL].isEnabled()) {
        mAnm[PLUS_BTN_ANIM_CALL].setAnimEnable(false);
    }

    if (!mShouldCall) {
        field_0x1C3 = mShouldCall;
    }
    mShouldCall = false;

    field_0x1B0 = LytDoButtonRelated::get(LytDoButtonRelated::DO_BUTTON_PLUS);
    if (field_0x1B0 != field_0x1B4) {
        setMessage(field_0x1B0);
    }

    mStateMgr.executeState();

    LytDoButtonRelated::set(LytDoButtonRelated::DO_BUTTON_PLUS, LytDoButtonRelated::DO_NONE);

    if (mAnm[PLUS_BTN_ANIM_LOOP].isEnabled()) {
        mAnm[PLUS_BTN_ANIM_LOOP].play();
    }

    return true;
}

bool dLytMeterPlusBtn_c::isCalling() const {
    if (dLytControlGame_c::getInstance()->isStateNormal() && StoryflagManager::sInstance->getCounterOrFlag(567)) {
        return true;
    }
    return false;
}

void dLytMeterPlusBtn_c::setCall(bool shouldCall) {
    field_0x1C0 = shouldCall;
    if (shouldCall) {
        StoryflagManager::sInstance->setFlag(212);
    }
}

bool dLytMeterPlusBtn_c::hasSpecificMapMark() {
    dTgMapMark_c *mark = nullptr;
    while ((mark = static_cast<dTgMapMark_c *>(fManager_c::searchBaseByGroupType(fBase_c::STAGE, mark))) != nullptr) {
        if (mark->checkActorProperty(0x100) || mark->profile_name != fProfile::T_MAP_MARK ||
            mark->rotz_shift0_0xf == 1) {
            continue;
        }

        switch (mark->shift0_0x3) {
            case 0: {
                mark->field_0x10D = mark->shift4_0x8 < 0xFF &&
                                    SceneflagManager::sInstance->checkBoolFlag(mark->roomid, mark->shift4_0x8);
                break;
            }
            case 1: {
                mark->field_0x10D = mark->shift4_0x8 < 0xFF &&
                                    !SceneflagManager::sInstance->checkBoolFlag(mark->roomid, mark->shift4_0x8);
                break;
            }
            case 3: {
                mark->field_0x10D = true;
                break;
            }
        }

        switch (mark->shift2_0x3) {
            case 0: {
                mark->field_0x10C = StoryflagManager::sInstance->getCounterOrFlag(mark->mTriggerStoryFlag) != 0;
                break;
            }
            case 1: {
                mark->field_0x10C = StoryflagManager::sInstance->getCounterOrFlag(mark->mTriggerStoryFlag) == 0;
                break;
            }
            case 3: {
                mark->field_0x10C = true;
                break;
            }
        }

        mark->field_0x10E = mark->field_0x10C && mark->field_0x10D;

        if (mark->shift23_0x8 < 0xFF && SceneflagManager::sInstance->checkBoolFlag(mark->roomid, mark->shift23_0x8)) {
            mark->field_0x10E = !mark->field_0x10E;
            if (mark->rotz_shift12_0x3 != 0) {
                mark->field_0x10E = false;
            }
        }

        if (mark->field_0x10E && mark->rotz_shift10_0x3 != 0) {
            return true;
        }
    }

    return false;
}

bool dLytMeterPlusBtn_c::isMapOpen() const {
    if (dLytControlGame_c::getInstance() != nullptr) {
        return dLytControlGame_c::getInstance()->isInSomeMapState();
    }
    return false;
}

#include "d/lyt/meter/d_lyt_meter_action_table.inc"

void dLytMeterPlusBtn_c::setMessage(s32 id) {
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
    } else {
        mpTextBoxes[0]->SetVisible(false);
        mpTextBoxes[1]->SetVisible(false);
        mpWindow[0]->SetVisible(false);
    }
    field_0x1B4 = id;
}
