#include "d/lyt/meter/d_lyt_meter_event_skip.h"

#include "d/d_sc_game.h"
#include "d/lyt/d2d.h"
#include "d/lyt/meter/d_lyt_meter.h"
#include "toBeSorted/event_manager.h"

STATE_DEFINE(dLytMeterEventSkip_c, Invisible);
STATE_DEFINE(dLytMeterEventSkip_c, In);
STATE_DEFINE(dLytMeterEventSkip_c, Visible);
STATE_DEFINE(dLytMeterEventSkip_c, Out);

static const d2d::LytBrlanMapping brlanMap[] = {
    { "skip_00_in.brlan", "G_inOut_00"},
    {"skip_00_out.brlan", "G_inOut_00"},
};

#define SKIP_ANIM_IN 0
#define SKIP_ANIM_OUT 1

#define SKIP_NUM_ANIMS 2

void dLytMeterEventSkip_c::initializeState_Invisible() {}
void dLytMeterEventSkip_c::executeState_Invisible() {
    if (shouldPromptForSkip() || dLytMeter_c::GetInstance()->getMeterField_0x13774()) {
        if (dLytMeter_c::GetInstance()->getMeterField_0x13774()) {
            for (int i = 0; i < 2; i++) {
                mLyt.fn_800AB9A0(mpTextBoxes[i], 1);
            }
        } else {
            for (int i = 0; i < 2; i++) {
                mLyt.fn_800AB9A0(mpTextBoxes[i], 0);
            }
        }

        mpWindow->UpdateSize(mpSizeBox, 32.0f);
        mVisible = true;
        mAnm[SKIP_ANIM_IN].setAnimEnable(true);
        mAnm[SKIP_ANIM_IN].setFrame(0.0f);
        mStateMgr.changeState(StateID_In);
    }
}
void dLytMeterEventSkip_c::finalizeState_Invisible() {}

void dLytMeterEventSkip_c::initializeState_In() {}
void dLytMeterEventSkip_c::executeState_In() {
    if (mAnm[SKIP_ANIM_IN].isStop2()) {
        mLyt.calc(); // ???
        mAnm[SKIP_ANIM_IN].setAnimEnable(false);
        mStateMgr.changeState(StateID_Visible);
    }
}
void dLytMeterEventSkip_c::finalizeState_In() {}

void dLytMeterEventSkip_c::initializeState_Visible() {}
void dLytMeterEventSkip_c::executeState_Visible() {
    if (!shouldPromptForSkip() && !dLytMeter_c::GetInstance()->getMeterField_0x13774()) {
        mAnm[SKIP_ANIM_OUT].setAnimEnable(true);
        mAnm[SKIP_ANIM_OUT].setFrame(0.0f);
        mStateMgr.changeState(StateID_Out);
    }
}
void dLytMeterEventSkip_c::finalizeState_Visible() {}

void dLytMeterEventSkip_c::initializeState_Out() {}
void dLytMeterEventSkip_c::executeState_Out() {
    if (mAnm[SKIP_ANIM_OUT].isStop2()) {
        mAnm[SKIP_ANIM_OUT].setAnimEnable(false);
        mVisible = false;
        mStateMgr.changeState(StateID_Invisible);
    }
}
void dLytMeterEventSkip_c::finalizeState_Out() {}

static const char *sTextBoxes[] = {
    "T_skipText_00",
    "T_skipTextS_00",
};

bool dLytMeterEventSkip_c::build(d2d::ResAccIf_c *resAcc) {
    mLyt.setResAcc(resAcc);
    mLyt.build("skip_00.brlyt", nullptr);

    for (int i = 0; i < SKIP_NUM_ANIMS; i++) {
        mAnm[i].init(brlanMap[i].mFile, resAcc, mLyt.getLayout(), brlanMap[i].mName);
        mAnm[i].bind(false);
        mAnm[i].setAnimEnable(false);
    }

    for (int i = 0; i < 2; i++) {
        mpTextBoxes[i] = mLyt.getTextBox(sTextBoxes[i]);
    }

    mpWindow = mLyt.getWindow("W_bgP_00");
    mpSizeBox = mLyt.getSizeBoxInWindow("W_bgP_00");
    mpWindow->UpdateSize(mpSizeBox, 32.0f);
    mLyt.setPriority(0x8A);
    mVisible = false;
    mStateMgr.changeState(StateID_Invisible);

    return true;
}

bool dLytMeterEventSkip_c::remove() {
    for (int i = 0; i < 2; i++) {
        mAnm[i].remove();
    }
    return true;
}

bool dLytMeterEventSkip_c::execute() {
    mStateMgr.executeState();
    if (mVisible) {
        for (int i = 0; i < 2; i++) {
            if (mAnm[i].isEnabled()) {
                mAnm[i].play();
            }
        }
        mLyt.calc();
    }
    return true;
}

bool dLytMeterEventSkip_c::draw() {
    if (mVisible) {
        mLyt.addToDrawList();
    }
    return true;
}

bool dLytMeterEventSkip_c::shouldPromptForSkip() const {
    return dScGame_c::currentSpawnInfo.stageName == "Demo" ? false : EventManager::canSkipCurrentEvent();
}
