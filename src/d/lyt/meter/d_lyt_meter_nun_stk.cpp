#include "d/lyt/meter/d_lyt_meter_nun_stk.h"

#include "d/lyt/d_lyt_unknowns.h"
#include "d/lyt/d_textbox.h"
#include "d/lyt/d_window.h"

STATE_DEFINE(dLytMeterNunStk_c, Wait);
STATE_DEFINE(dLytMeterNunStk_c, On);
STATE_DEFINE(dLytMeterNunStk_c, Active);
STATE_DEFINE(dLytMeterNunStk_c, Off);

static const d2d::LytBrlanMapping brlanMap[] = {
    {"nunBtn_02_input.brlan", "G_input_00"},
    { "nunBtn_02_loop.brlan",  "G_loop_00"},
};

#define NUN_STK_ANIM_INPUT 0
#define NUN_STK_ANIM_LOOP 1

#define NUN_STK_NUM_ANIMS 2

void dLytMeterNunStk_c::initializeState_Wait() {
    mOnDelay = 15;
}
void dLytMeterNunStk_c::executeState_Wait() {
    s32 timer = mOnDelay;
    if (field_0x184) {
        if (field_0x178) {
            bool bDoUpdate = true;
            if (field_0x174 != LytDoButtonRelated::ACT_IE_NONE) {
                if (timer <= 0) {
                    mAnm[NUN_STK_ANIM_INPUT].setBackwardsOnce();
                    mAnm[NUN_STK_ANIM_INPUT].setToStart();
                    mAnm[NUN_STK_ANIM_INPUT].setAnimEnable(true);
                    mStateMgr.changeState(StateID_On);
                    setMessage(field_0x174);
                } else {
                    mOnDelay = timer - 1;
                    bDoUpdate = false;
                }
            }
            if (bDoUpdate) {
                field_0x170 = field_0x174;
            }
        } else {
            if (field_0x174 != LytDoButtonRelated::ACT_IE_NONE) {
                setMessage(field_0x174);
            }
            field_0x170 = field_0x174;
        }
    }

    if (timer == mOnDelay) {
        mOnDelay = 15;
    }
}
void dLytMeterNunStk_c::finalizeState_Wait() {}

void dLytMeterNunStk_c::initializeState_On() {}
void dLytMeterNunStk_c::executeState_On() {
    if (mAnm[NUN_STK_ANIM_INPUT].isStop2()) {
        mAnm[NUN_STK_ANIM_INPUT].setAnimEnable(false);
        mStateMgr.changeState(StateID_Active);
    }
}
void dLytMeterNunStk_c::finalizeState_On() {}

void dLytMeterNunStk_c::initializeState_Active() {}
void dLytMeterNunStk_c::executeState_Active() {
    if (field_0x174 != field_0x170 || !field_0x178) {
        if (field_0x174 == LytDoButtonRelated::ACT_IE_NONE || !field_0x178) {
            mAnm[NUN_STK_ANIM_INPUT].setForwardOnce();
            mAnm[NUN_STK_ANIM_INPUT].setFrame(0.0f);
            mAnm[NUN_STK_ANIM_INPUT].setAnimEnable(true);
            if (field_0x170 != mDisplayedAction) {
                field_0x174 = mDisplayedAction;
            }
            mStateMgr.changeState(StateID_Off);
        } else {
            setMessage(field_0x174);
        }
        field_0x170 = field_0x174;
    } else if (field_0x170 == LytDoButtonRelated::ACT_IE_NONE) {
        mAnm[NUN_STK_ANIM_INPUT].setForwardOnce();
        mAnm[NUN_STK_ANIM_INPUT].setFrame(0.0f);
        mAnm[NUN_STK_ANIM_INPUT].setAnimEnable(true);
        mStateMgr.changeState(StateID_Off);
    }
}
void dLytMeterNunStk_c::finalizeState_Active() {}

void dLytMeterNunStk_c::initializeState_Off() {}
void dLytMeterNunStk_c::executeState_Off() {
    if (mAnm[NUN_STK_ANIM_INPUT].isEndReached()) {
        mAnm[NUN_STK_ANIM_INPUT].setAnimEnable(false);
        mStateMgr.changeState(StateID_Wait);
    }
}
void dLytMeterNunStk_c::finalizeState_Off() {}

static const char *sPane = "N_all_00";

static const char *sTextBoxes[] = {
    "T_conStick_00",
    "T_conStickS_00",
};

static const char *sWindows[] = {"W_bgP_00"};

bool dLytMeterNunStk_c::build(d2d::ResAccIf_c *resAcc) {
    mLyt.setResAcc(resAcc);
    mLyt.build("nunBtn_02.brlyt", nullptr);

    for (int i = 0; i < NUN_STK_NUM_ANIMS; i++) {
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

    mDisplayedAction = LytDoButtonRelated::ACT_IE_NONE;
    field_0x170 = LytDoButtonRelated::ACT_IE_NONE;
    field_0x174 = LytDoButtonRelated::ACT_IE_NONE;
    field_0x184 = true;
    field_0x178 = true;

    field_0x185 = false;
    mpOwnerPane = nullptr;

    setMessage(field_0x170);

    mAnm[NUN_STK_ANIM_INPUT].setToEnd();
    mAnm[NUN_STK_ANIM_INPUT].setAnimEnable(true);

    mAnm[NUN_STK_ANIM_LOOP].setAnimEnable(true);
    mLyt.calc();
    mAnm[NUN_STK_ANIM_INPUT].setAnimEnable(false);

    mStateMgr.changeState(StateID_Wait);
    return true;
}

bool dLytMeterNunStk_c::remove() {
    for (int i = 0; i < NUN_STK_NUM_ANIMS; i++) {
        mAnm[i].unbind();
        mAnm[i].remove();
    }
    return true;
}

bool dLytMeterNunStk_c::execute() {
    if (mpOwnerPane == nullptr) {
        return true;
    }

    if (!mpOwnerPane->IsVisible()) {
        if (*mStateMgr.getStateID() == StateID_Active) {
            mAnm[NUN_STK_ANIM_INPUT].setAnimEnable(true);
        }
        if (mAnm[NUN_STK_ANIM_INPUT].isEnabled()) {
            mAnm[NUN_STK_ANIM_INPUT].setForwardOnce();
            mAnm[NUN_STK_ANIM_INPUT].setToEnd2();
            mLyt.calc();
            mAnm[NUN_STK_ANIM_INPUT].setAnimEnable(false);
            mStateMgr.changeState(StateID_Wait);
        }
        LytDoButtonRelated::set(LytDoButtonRelated::DO_BUTTON_NUN_STK, LytDoButtonRelated::ACT_IE_NONE);
        field_0x174 = LytDoButtonRelated::get(LytDoButtonRelated::DO_BUTTON_NUN_STK);
        setMessage(field_0x174);
        return true;
    }

    field_0x185 = false;

    field_0x174 = LytDoButtonRelated::get(LytDoButtonRelated::DO_BUTTON_NUN_STK);
    field_0x178 = LytDoButtonRelated::getHas(LytDoButtonRelated::DO_BUTTON_NUN_STK);

    mStateMgr.executeState();
    for (int i = 0; i < NUN_STK_NUM_ANIMS; i++) {
        if (mAnm[i].isEnabled()) {
            mAnm[i].play();
        }
    }

    return true;
}

#include "d/lyt/meter/d_lyt_meter_action_table.inc"

void dLytMeterNunStk_c::setMessage(s32 id) {
    char buf[11];
    for (int i = 0; i < 11; i++) {
        buf[i] = 0;
    }

    if (id < LytDoButtonRelated::ACT_IE_NONE) {
        if (id < LytDoButtonRelated::ACT_IE_SEPARATOR) {
            sprintf(buf, "ACT_INFO_%03d", sActIds[id]);
        } else {
            s32 id2 = id - LytDoButtonRelated::ACT_IE_SEPARATOR - 1;
            sprintf(buf, "ACT_ETC_%03d", sActIds[id2]);
        }

        for (int i = 0; i < 2; i++) {
            mpTextBoxes[i]->setMessageWithGlobalTextProcessor2(buf, nullptr);
        }

        mpWindow[0]->SetVisible(true);
        mpWindow[0]->UpdateSize(mpSizeBox[0], 32.0f);
        mDisplayedAction = id;
    } else {
        wchar_t b2[2];
        b2[0] = b2[1] = 0;
        for (int i = 0; i < 2; i++) {
            mpTextBoxes[i]->setTextWithGlobalTextProcessor(b2);
        }
        mpWindow[0]->SetVisible(false);
    }
}
