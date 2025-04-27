#include "d/lyt/meter/d_lyt_meter_nunchaku_bg.h"

#include "common.h"
#include "d/lyt/d_lyt_unknowns.h"
#include "d/lyt/meter/d_lyt_meter.h"
#include "toBeSorted/event_manager.h"

STATE_DEFINE(dLytMeterNunchakuBg_c, Wait);
STATE_DEFINE(dLytMeterNunchakuBg_c, On);
STATE_DEFINE(dLytMeterNunchakuBg_c, Active);
STATE_DEFINE(dLytMeterNunchakuBg_c, Off);

static const d2d::LytBrlanMapping brlanMap[] = {
    { "nunBg_00_input.brlan",      "G_input_00"},
    {"nunBg_00_arrowV.brlan", "G_arrowAlpha_00"},
    {  "nunBg_00_loop.brlan",       "G_loop_00"},
};

#define NUN_BG_ANIM_INPUT 0
#define NUN_BG_ANIM_ARROWV 1
#define NUN_BG_ANIM_LOOP 2

#define NUN_BG_NUM_ANIMS 3

void dLytMeterNunchakuBg_c::initializeState_Wait() {
    mOnDelay = 15;
}
void dLytMeterNunchakuBg_c::executeState_Wait() {
    s32 timer = mOnDelay;
    if (field_0x1C4) {
        if (field_0x1B8) {
            bool bDoUpdate = true;
            if (field_0x1B4 != LytDoButtonRelated::DO_NONE) {
                if (timer <= 0) {
                    mAnm[NUN_BG_ANIM_INPUT].setBackwardsOnce();
                    mAnm[NUN_BG_ANIM_INPUT].setToStart();
                    mAnm[NUN_BG_ANIM_INPUT].setAnimEnable(true);
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
void dLytMeterNunchakuBg_c::finalizeState_Wait() {}

void dLytMeterNunchakuBg_c::initializeState_On() {}
void dLytMeterNunchakuBg_c::executeState_On() {
    if (mAnm[NUN_BG_ANIM_INPUT].isStop2()) {
        mAnm[NUN_BG_ANIM_INPUT].setAnimEnable(false);
        mStateMgr.changeState(StateID_Active);
    }
}
void dLytMeterNunchakuBg_c::finalizeState_On() {}

void dLytMeterNunchakuBg_c::initializeState_Active() {}
void dLytMeterNunchakuBg_c::executeState_Active() {
    if (field_0x1B4 != field_0x1B0 || !field_0x1B8) {
        if (field_0x1B4 == LytDoButtonRelated::DO_NONE || !field_0x1B8) {
            mAnm[NUN_BG_ANIM_INPUT].setForwardOnce();
            mAnm[NUN_BG_ANIM_INPUT].setFrame(0.0f);
            mAnm[NUN_BG_ANIM_INPUT].setAnimEnable(true);
            if (field_0x1B0 != field_0x1BC) {
                field_0x1B4 = field_0x1BC;
            }
            mStateMgr.changeState(StateID_Off);
        } else {
            setMessage(field_0x1B4);
        }
        field_0x1B0 = field_0x1B4;
    } else if (field_0x1B0 == LytDoButtonRelated::DO_NONE) {
        mAnm[NUN_BG_ANIM_INPUT].setForwardOnce();
        mAnm[NUN_BG_ANIM_INPUT].setFrame(0.0f);
        mAnm[NUN_BG_ANIM_INPUT].setAnimEnable(true);
        mStateMgr.changeState(StateID_Off);
    }
}
void dLytMeterNunchakuBg_c::finalizeState_Active() {}

void dLytMeterNunchakuBg_c::initializeState_Off() {}
void dLytMeterNunchakuBg_c::executeState_Off() {
    if (mAnm[NUN_BG_ANIM_INPUT].isEndReached()) {
        mAnm[NUN_BG_ANIM_INPUT].setAnimEnable(false);
        mStateMgr.changeState(StateID_Wait);
    }
}
void dLytMeterNunchakuBg_c::finalizeState_Off() {}

static const char *sPane = "N_nunBg_00";

static const char *sTextBoxes[] = {
    "T_nunchaku_00",
    "T_nunchakuS_00",
};

static const char *sWindows[] = {"W_bgP_04"};

bool dLytMeterNunchakuBg_c::build(d2d::ResAccIf_c *resAcc) {
    mLyt.setResAcc(resAcc);
    mLyt.build("nunBg_00.brlyt", nullptr);

    for (int i = 0; i < NUN_BG_NUM_ANIMS; i++) {
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

    field_0x1BC = 0x60;
    field_0x1B0 = 0x60;
    field_0x1B4 = 0x60;
    field_0x1C4 = false;
    field_0x1B8 = true;

    field_0x1C5 = 0;
    field_0x1C6 = 0;
    mpOwnerPane = nullptr;
    mOnDelay = 15;

    dLytMeter_c::GetMain()->getNunStk()->getBgPane()->SetVisible(false);

    setMessage(field_0x1B0);

    mAnm[NUN_BG_ANIM_INPUT].setToEnd();
    mAnm[NUN_BG_ANIM_INPUT].setAnimEnable(true);

    mAnm[NUN_BG_ANIM_LOOP].setAnimEnable(true);
    mLyt.calc();
    mAnm[NUN_BG_ANIM_INPUT].setAnimEnable(false);
    mStateMgr.changeState(StateID_Wait);
    return true;
}

bool dLytMeterNunchakuBg_c::remove() {
    for (int i = 0; i < NUN_BG_NUM_ANIMS; i++) {
        mAnm[i].unbind();
        mAnm[i].remove();
    }
    return true;
}

bool dLytMeterNunchakuBg_c::execute() {
    if (mpOwnerPane == nullptr) {
        return true;
    }

    if (!mpOwnerPane->IsVisible()) {
        if (*mStateMgr.getStateID() == StateID_Active) {
            mAnm[NUN_BG_ANIM_INPUT].setAnimEnable(true);
        }
        if (mAnm[NUN_BG_ANIM_INPUT].isEnabled()) {
            mAnm[NUN_BG_ANIM_INPUT].setForwardOnce();
            mAnm[NUN_BG_ANIM_INPUT].setToEnd2();
            mLyt.calc();
            mAnm[NUN_BG_ANIM_INPUT].setAnimEnable(false);
            mStateMgr.changeState(StateID_Wait);
        }
        LytDoButtonRelated::set(LytDoButtonRelated::DO_BUTTON_NUN_BG, LytDoButtonRelated::DO_NONE);
        field_0x1B4 = LytDoButtonRelated::get(LytDoButtonRelated::DO_BUTTON_NUN_STK);
        setMessage(field_0x1B4);
        return true;
    }

    if (EventManager::isInEvent()) {
        field_0x1C4 = false;
        LytDoButtonRelated::set(LytDoButtonRelated::DO_BUTTON_NUN_BG, LytDoButtonRelated::DO_NONE);
    } else {
        field_0x1C4 = true;
    }

    s32 d = dLytDobutton_c::getFn0x8010E5E0();
    if (d == 6 || d == 9) {
        LytDoButtonRelated::Action_e a = LytDoButtonRelated::convertDoButton(dLytDobutton_c::getFn0x8010E5D0());
        if (a >= 0) {
            LytDoButtonRelated::set(LytDoButtonRelated::DO_BUTTON_NUN_BG, a);
        }
    }

    field_0x1B4 = LytDoButtonRelated::get(LytDoButtonRelated::DO_BUTTON_NUN_BG);
    field_0x1B8 = LytDoButtonRelated::getHas(LytDoButtonRelated::DO_BUTTON_NUN_BG);

    bool hasNunStk = LytDoButtonRelated::getHas(LytDoButtonRelated::DO_BUTTON_NUN_STK);
    u8 val = dLytMeter_c::getDowsing0x550A();
    if (hasNunStk != field_0x1C5 || val != field_0x1C6) {
        field_0x1C5 = hasNunStk;
        field_0x1C6 = val;

        if (hasNunStk) {
            mAnm[NUN_BG_ANIM_ARROWV].setFrame(1.0f);
            mAnm[NUN_BG_ANIM_ARROWV].setAnimEnable(true);
            dLytMeter_c::GetMain()->getNunStk()->getBgPane()->SetVisible(true);
        } else {
            if (val) {
                mAnm[NUN_BG_ANIM_ARROWV].setFrame(1.0f);
                mAnm[NUN_BG_ANIM_ARROWV].setAnimEnable(true);
            } else {
                mAnm[NUN_BG_ANIM_ARROWV].setFrame(0.0f);
                mAnm[NUN_BG_ANIM_ARROWV].setAnimEnable(true);
                setMessage(field_0x1B4);
            }
            dLytMeter_c::GetMain()->getNunStk()->getBgPane()->SetVisible(false);
        }
    }

    mStateMgr.executeState();
    for (int i = 0; i < NUN_BG_NUM_ANIMS; i++) {
        if (i != 1 && mAnm[i].isEnabled()) {
            mAnm[i].play();
        }
    }

    return true;
}

#include "d/lyt/meter/d_lyt_meter_action_table.inc"

void dLytMeterNunchakuBg_c::setMessage(s32 id) {
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
            mpTextBoxes[i]->setMessageWithGlobalTextProcessor2(buf, nullptr);
        }
        mpWindow[0]->SetVisible(true);
        mpWindow[0]->UpdateSize(mpSizeBox[0], 32.0f);
        field_0x1BC = id;
    } else {
        wchar_t b2[2];
        b2[0] = b2[1] = 0;
        for (int i = 0; i < 2; i++) {
            mpTextBoxes[i]->setTextWithGlobalTextProcessor(b2);
        }
        mpWindow[0]->SetVisible(false);
    }
}
