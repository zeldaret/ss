#include "d/lyt/d_lyt_auto_explain.h"

#include "common.h"
#include "d/d_tag_processor.h"
#include "d/lyt/d2d.h"
#include "d/lyt/d_lyt_control_game.h"
#include "d/lyt/d_textbox.h"
#include "d/lyt/meter/d_lyt_meter.h"
#include "s/s_State.hpp"
#include "toBeSorted/event_manager.h"

STATE_DEFINE(dLytAutoExplain_c, Invisible);
STATE_DEFINE(dLytAutoExplain_c, In);
STATE_DEFINE(dLytAutoExplain_c, OutputText);
STATE_DEFINE(dLytAutoExplain_c, Out);

static const d2d::LytBrlanMapping brlanMap[] = {
    {  "autoMessage_00_in.brlan", "G_inOut_00"},
    { "autoMessage_00_out.brlan", "G_inOut_00"},
    {"autoMessage_00_line.brlan",  "G_line_00"},
};

#define AUTO_EXPLAIN_ANIM_IN 0
#define AUTO_EXPLAIN_ANIM_OUT 1
#define AUTO_EXPLAIN_ANIM_TEXTLINE 2

#define AUTO_EXPLAIN_NUM_ANIMS 3

static const char *sPanes[] = {
    "N_inOutAll_00",
};

static const char *sTextBoxes[] = {
    "T_autoMsg_00",
    "T_autoMsgS_00",
};

static const char *sGroupName = "G_ref_00";

void dLytAutoExplain_c::initializeState_Invisible() {}
void dLytAutoExplain_c::executeState_Invisible() {
    if (mShouldBeOpen) {
        mStateMgr.changeState(StateID_In);
    }
}
void dLytAutoExplain_c::finalizeState_Invisible() {}

extern "C" u8 fn_80054F30();

void dLytAutoExplain_c::initializeState_In() {
    int numLines = mpTagProcessor->getNumLinesMaybe() - 1;
    if (fn_80054F30() != 0 && numLines == 3) {
        numLines = 2;
    }

    mAnm[AUTO_EXPLAIN_ANIM_TEXTLINE].setAnimEnable(true);
    mAnm[AUTO_EXPLAIN_ANIM_TEXTLINE].setFrame(numLines);
    mLyt.calc();
    mAnm[AUTO_EXPLAIN_ANIM_TEXTLINE].setAnimEnable(false);

    mAnm[AUTO_EXPLAIN_ANIM_IN].setAnimEnable(true);
    mAnm[AUTO_EXPLAIN_ANIM_IN].setFrame(0.0f);
}
void dLytAutoExplain_c::executeState_In() {
    if (mAnm[AUTO_EXPLAIN_ANIM_IN].isEndReached()) {
        mAnm[AUTO_EXPLAIN_ANIM_IN].setAnimEnable(false);
        mStateMgr.changeState(StateID_OutputText);
    }
}
void dLytAutoExplain_c::finalizeState_In() {}

void dLytAutoExplain_c::initializeState_OutputText() {}
void dLytAutoExplain_c::executeState_OutputText() {
    if (!mShouldBeOpen) {
        mStateMgr.changeState(StateID_Out);
    }
}
void dLytAutoExplain_c::finalizeState_OutputText() {}

void dLytAutoExplain_c::initializeState_Out() {
    mAnm[AUTO_EXPLAIN_ANIM_OUT].setAnimEnable(true);
    mAnm[AUTO_EXPLAIN_ANIM_OUT].setFrame(0.0f);
}
void dLytAutoExplain_c::executeState_Out() {
    if (mAnm[AUTO_EXPLAIN_ANIM_OUT].isEndReached()) {
        mAnm[AUTO_EXPLAIN_ANIM_OUT].setAnimEnable(false);
        mStateMgr.changeState(StateID_Invisible);
    }
}
void dLytAutoExplain_c::finalizeState_Out() {}

bool dLytAutoExplain_c::build(d2d::ResAccIf_c *resAcc1, d2d::ResAccIf_c *resAcc, dTagProcessor_c *tagProcessor) {
    mLyt.setResAcc(resAcc1);
    mLyt.build("autoMessage_00.brlyt", nullptr);
    mLyt.setPriority(0x8A);

    for (int i = 0; i < AUTO_EXPLAIN_NUM_ANIMS; i++) {
        mAnm[i].init(brlanMap[i].mFile, resAcc1, mLyt.getLayout(), brlanMap[i].mName);
        mAnm[i].bind(false);
        mAnm[i].setAnimEnable(false);
    }

    mpPane = mLyt.findPane(sPanes[0]);

    for (int i = 0; i < 2; i++) {
        mpTextboxes[i] = mLyt.getTextBox(sTextBoxes[i]);
        mpTextboxes[i]->SetTextPositionH(0);
        // mpTextboxes[i]->SetTextPositionV(0);
    }

    mpTagProcessor = tagProcessor;
    mShouldBeOpen = false;

    mAnm[AUTO_EXPLAIN_ANIM_OUT].setAnimEnable(true);
    mAnm[AUTO_EXPLAIN_ANIM_OUT].setToEnd();
    mLyt.calc();
    mAnm[AUTO_EXPLAIN_ANIM_OUT].setAnimEnable(false);

    mStateMgr.changeState(StateID_Invisible);
    return true;
}

bool dLytAutoExplain_c::remove() {
    for (int i = 0; i < AUTO_EXPLAIN_NUM_ANIMS; i++) {
        mAnm[i].remove();
    }
    return true;
}

bool dLytAutoExplain_c::execute() {
    mStateMgr.executeState();
    for (int i = 0; i < AUTO_EXPLAIN_NUM_ANIMS; i++) {
        if (mAnm[i].isEnabled()) {
            mAnm[i].play();
        }
    }
    mLyt.calc();
    return true;
}

void dLytAutoExplain_c::draw() {
    mLyt.draw();
}

void dLytAutoExplain_c::open(dAcObjBase_c *obj, u32 param) {
    mShouldBeOpen = true;
}

bool dLytAutoExplain_c::isDoneOpening() const {
    return !(*mStateMgr.getStateID() == StateID_In);
}

void dLytAutoExplain_c::close() {
    mShouldBeOpen = false;
}

bool dLytAutoExplain_c::isDoneClosing() const {
    return !(*mStateMgr.getStateID() == StateID_Out);
}

bool dLytAutoExplain_c::setText(const wchar_t *text) {
    mpTextboxes[0]->SetTagProcessor(mpTagProcessor);
    mpTextboxes[1]->SetTagProcessor(mpTagProcessor);
    mpTextboxes[0]->setTextWithTextProcessor(text, mpTagProcessor, 0);
    mpTextboxes[1]->setTextWithTextProcessor(text, mpTagProcessor, 0);
    return true;
}

bool dLytAutoExplain_c::checkSomethingMeter() {
    if (EventManager::isInEvent() || !dLytControlGame_c::getInstance()->isStateNormal() ||
        dLytMeterContainer_c::getItemSelect0x75A2() || dLytMeterContainer_c::getMinusBtnFn800F7600() ||
        dLytMeterContainer_c::getDowsingFn800FE4B0()) {
        return true;
    }

    return false;
}
