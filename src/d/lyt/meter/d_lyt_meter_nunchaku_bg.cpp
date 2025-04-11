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

    dLytMeterContainer_c::GetMeter()->getNunStk()->getBgPane()->SetVisible(false);

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

    bool hasZ = LytDoButtonRelated::getHas(LytDoButtonRelated::DO_BUTTON_Z);

    mStateMgr.executeState();
    for (int i = 0; i < NUN_BG_NUM_ANIMS; i++) {
        if (i != 1 && mAnm[i].isEnabled()) {
            mAnm[i].play();
        }
    }

    return true;
}

// TODO
static const s32 sActIds[] = {
    0x1,  0x2,  0x3,  0x4,  0x5,  0x6,  0x7,  0x8,  0x9,  0xA,  0xB,  0xC,  0xD,  0xE,  0xF,  0x10, 0x11, 0x12, 0x13,
    0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1A, 0x1B, 0x1C, 0x1D, 0x1E, 0x1F, 0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26,
    0x27, 0x28, 0x29, 0x2A, 0x2B, 0x2C, 0x2D, 0x2E, 0x2F, 0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39,
    0x3A, 0x3B, 0x3C, 0x3D, 0x3E, 0x3F, 0x40, 0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x47, 0x48, 0x49, 0x4A, 0x4B, 0x4C,
    0x4D, 0x4E, 0x4F, 0x50, 0x51, 0x52, 0x53, 0x54, 0x55, 0x56, 0x57, 0x58, 0x59, 0x5A, 0x5B, 0x5C, 0x5D, -1,   0x1,
    0x2,  0x3,  0x4,  0x5,  0x6,  0x7,  0x8,  0x9,  0xA,  0xB,  0xC,  0xD,  0xE,  0xF,  0x10, 0x11, 0x12, 0x13, 0x14,
    0x15, 0x16, 0x17, 0x18, 0x19, 0x1A, 0x1B, 0x1C, 0x1D, 0x1E, 0x1F, 0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27,
    0x28, 0x29, 0x2A, 0x2B, 0x2C, 0x2D, 0x2E, 0x2F, 0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x0,  0x0,  0x0,  0x0,  0x0
};

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
