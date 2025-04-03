#include "d/lyt/d_lyt_auto_caption.h"

#include "common.h"
#include "d/d_tag_processor.h"
#include "d/lyt/d2d.h"
#include "d/lyt/d_textbox.h"
#include "s/s_State.hpp"

STATE_DEFINE(dLytAutoCaption_c, Invisible);
STATE_DEFINE(dLytAutoCaption_c, In);
STATE_DEFINE(dLytAutoCaption_c, OutputText);
STATE_DEFINE(dLytAutoCaption_c, Out);

static const d2d::LytBrlanMapping brlanMap[] = {
    {      "autoCaption_00_in.brlan",    "G_inOut_00"},
    {     "autoCaption_00_out.brlan",    "G_inOut_00"},
    {    "autoCaption_00_loop.brlan",     "G_loop_00"},
    {"autoCaption_00_textLine.brlan", "G_textLine_00"},
};

#define AUTO_CAPTION_ANIM_IN 0
#define AUTO_CAPTION_ANIM_OUT 1
#define AUTO_CAPTION_ANIM_LOOP 2
#define AUTO_CAPTION_ANIM_TEXTLINE 3

#define AUTO_CAPTION_NUM_ANIMS 4

static const char *sPanes[] = {
    "N_inOutAll_00",
};

static const char *sTextBoxes[] = {
    "T_textBox_00",
    "T_textBoxS_00",
};

static const char *sGroupName = "G_ref_00";

void dLytAutoCaption_c::initializeState_Invisible() {}
void dLytAutoCaption_c::executeState_Invisible() {
    if (mShouldBeOpen) {
        mStateMgr.changeState(StateID_In);
    }
}
void dLytAutoCaption_c::finalizeState_Invisible() {}

void dLytAutoCaption_c::initializeState_In() {
    s32 numTextLines = mpTextboxes[0]->calcTextLines(mpTextboxes[0]->GetString(), mpTagProcessor);
    f32 lineWidth = mpTextboxes[0]->GetLineWidth(nullptr);
    f32 frame = 0.0f;
    if (lineWidth <= 226.0f) {
        if (numTextLines > 1) {
            frame = 1.0f;
        }
    } else if (numTextLines > 1) {
        frame = 3.0f;
    } else {
        frame = 2.0f;
    }

    mAnm[AUTO_CAPTION_ANIM_TEXTLINE].setAnimEnable(true);
    mAnm[AUTO_CAPTION_ANIM_TEXTLINE].setFrame(frame);
    mLyt.calc();
    mAnm[AUTO_CAPTION_ANIM_TEXTLINE].setAnimEnable(false);

    mAnm[AUTO_CAPTION_ANIM_IN].setAnimEnable(true);
    mAnm[AUTO_CAPTION_ANIM_IN].setFrame(0.0f);
}
void dLytAutoCaption_c::executeState_In() {
    if (mAnm[AUTO_CAPTION_ANIM_IN].isEndReached()) {
        mAnm[AUTO_CAPTION_ANIM_IN].setAnimEnable(false);
        mStateMgr.changeState(StateID_OutputText);
    }
}
void dLytAutoCaption_c::finalizeState_In() {}

void dLytAutoCaption_c::initializeState_OutputText() {}
void dLytAutoCaption_c::executeState_OutputText() {
    if (!mShouldBeOpen) {
        mStateMgr.changeState(StateID_Out);
    }
}
void dLytAutoCaption_c::finalizeState_OutputText() {}

void dLytAutoCaption_c::initializeState_Out() {
    mAnm[AUTO_CAPTION_ANIM_OUT].setAnimEnable(true);
    mAnm[AUTO_CAPTION_ANIM_OUT].setFrame(0.0f);
}
void dLytAutoCaption_c::executeState_Out() {
    if (mAnm[AUTO_CAPTION_ANIM_OUT].isEndReached()) {
        mAnm[AUTO_CAPTION_ANIM_OUT].setAnimEnable(false);
        mStateMgr.changeState(StateID_Invisible);
    }
}
void dLytAutoCaption_c::finalizeState_Out() {}

bool dLytAutoCaption_c::build(d2d::ResAccIf_c *resAcc1, d2d::ResAccIf_c *resAcc, dTagProcessor_c *tagProcessor) {
    mLyt.setResAcc(resAcc1);
    mLyt.build("autoCaption_00.brlyt", nullptr);
    mLyt.setPriority(0x8A);

    for (int i = 0; i < AUTO_CAPTION_NUM_ANIMS; i++) {
        mAnm[i].init(brlanMap[i].mFile, resAcc1, mLyt.getLayout(), brlanMap[i].mName);
        mAnm[i].bind(false);
        mAnm[i].setAnimEnable(false);
    }

    mpPane = mLyt.findPane(sPanes[0]);

    for (int i = 0; i < 2; i++) {
        mpTextboxes[i] = mLyt.getTextBox(sTextBoxes[i]);
    }

    mpTagProcessor = tagProcessor;
    mShouldBeOpen = false;

    mAnm[AUTO_CAPTION_ANIM_LOOP].setAnimEnable(true);
    mAnm[AUTO_CAPTION_ANIM_OUT].setAnimEnable(true);
    mAnm[AUTO_CAPTION_ANIM_OUT].setToEnd();
    mLyt.calc();
    mAnm[AUTO_CAPTION_ANIM_OUT].setAnimEnable(false);

    mStateMgr.changeState(StateID_Invisible);
    return true;
}

bool dLytAutoCaption_c::remove() {
    for (int i = 0; i < AUTO_CAPTION_NUM_ANIMS; i++) {
        mAnm[i].remove();
    }
    return true;
}

bool dLytAutoCaption_c::execute() {
    mStateMgr.executeState();
    for (int i = 0; i < AUTO_CAPTION_NUM_ANIMS; i++) {
        if (mAnm[i].isEnabled()) {
            mAnm[i].play();
        }
    }
    mLyt.calc();
    return true;
}

void dLytAutoCaption_c::draw() {
    mLyt.draw();
}

void dLytAutoCaption_c::open() {
    mShouldBeOpen = true;
}

bool dLytAutoCaption_c::isDoneOpening() const {
    return !(*mStateMgr.getStateID() == StateID_In);
}

void dLytAutoCaption_c::close() {
    mShouldBeOpen = false;
}

bool dLytAutoCaption_c::isDoneClosing() const {
    return !(*mStateMgr.getStateID() == StateID_Out);
}

bool dLytAutoCaption_c::setText(const wchar_t *text) {
    mpTextboxes[0]->SetTagProcessor(mpTagProcessor);
    mpTextboxes[1]->SetTagProcessor(mpTagProcessor);
    mpTextboxes[0]->setTextWithGlobalTextProcessor(text);
    mpTextboxes[1]->setTextWithGlobalTextProcessor(text);
    return true;
}

bool dLytAutoCaption_c::checkSomethingMeter() {
    // TODO - too many dependencies on dLytMeter
    return false;
}
