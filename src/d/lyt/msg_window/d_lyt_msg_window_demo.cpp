#include "d/lyt/msg_window/d_lyt_msg_window_demo.h"

#include "common.h"
#include "d/d_tag_processor.h"
#include "d/lyt/d2d.h"
#include "d/lyt/d_textbox.h"
#include "d/lyt/msg_window/d_lyt_msg_window.h"
#include "d/lyt/msg_window/d_lyt_msg_window_common.h"
#include "nw4r/lyt/lyt_pane.h"
#include "nw4r/lyt/lyt_types.h"
#include "s/s_State.hpp"
#include "toBeSorted/arc_managers/layout_arc_manager.h"

STATE_DEFINE(dLytMsgWindowDemo_c, Invisible);
STATE_DEFINE(dLytMsgWindowDemo_c, In);
STATE_DEFINE(dLytMsgWindowDemo_c, Visible);
STATE_DEFINE(dLytMsgWindowDemo_c, Out);

static const d2d::LytBrlanMapping brlanMap[] = {
    { "demoWait_00_in.brlan", "G_inOut_00"},
    {"demoWait_00_out.brlan", "G_inOut_00"},
};

#define DEMO_ANIM_IN 0
#define DEMO_ANIM_OUT 1

#define DEMO_NUM_ANIMS 2

static const char *sTextBoxes[] = {
    "T_message_00",
    "T_messageS_00",
};

bool dLytMsgWindowDemo_c::build(d2d::ResAccIf_c *resAcc1, d2d::ResAccIf_c *resAcc, dTagProcessor_c *tagProcessor) {
    mResAcc.attach(LayoutArcManager::GetInstance()->getLoadedData("DemoWait"), "");
    mLyt.setResAcc(&mResAcc);
    mLyt.build("demoWait_00.brlyt", nullptr);
    mLyt.setPriority(0x8A);

    for (int i = 0; i < DEMO_NUM_ANIMS; i++) {
        mAnm[i].init(brlanMap[i].mFile, &mResAcc, mLyt.getLayout(), brlanMap[i].mName);
        mAnm[i].bind(false);
        mAnm[i].setAnimEnable(false);
    }

    mAnm[DEMO_ANIM_OUT].setAnimEnable(true);
    mAnm[DEMO_ANIM_OUT].setToEnd();
    mLyt.calc();
    mAnm[DEMO_ANIM_OUT].setAnimEnable(false);

    mpPane = mLyt.findPane("N_inOut_00");

    for (int i = 0; i < 2; i++) {
        mpTextboxes[i] = mLyt.getTextBox(sTextBoxes[i]);
    }

    mpTextboxes[1]->set0x1F6(1);

    mpTagProcessor = tagProcessor;

    mOffsets[0].x = mOffsets[0].y = 0.0f;
    mOffsets[1].x = mOffsets[1].y = 0.0f;

    mText.build(&mResAcc, dLytTextLight::DEMO);
    mBlurRelated.build();

    mStateMgr.changeState(StateID_Invisible);
    return true;
}

bool dLytMsgWindowDemo_c::remove() {
    mText.remove();
    for (int i = 0; i < DEMO_NUM_ANIMS; i++) {
        mAnm[i].remove();
    }
    mResAcc.detach();
    return true;
}

void dLytMsgWindowDemo_c::initializeState_Invisible() {
    nw4r::lyt::Size size = getTextBox()->GetSize();
    nw4r::lyt::Size fontSize = getTextBox()->GetFontSize();
    nw4r::math::MTX34 transform = getTextBox()->GetGlobalMtx();

    mOffsets[0].x = transform._03 - size.width / 2.0f;
    mOffsets[0].y = (transform._13 - size.height / 2.0f) + fontSize.height + fontSize.height * 0.5f;
    mOffsets[1].x = mOffsets[0].x;
    mOffsets[1].y =
        (transform._13 - size.height / 2.0f) + fontSize.height + getTextBox()->GetLineSpace() + fontSize.height;
}
void dLytMsgWindowDemo_c::executeState_Invisible() {
    if (mShouldBeOpen) {
        mStateMgr.changeState(StateID_In);
    }
}
void dLytMsgWindowDemo_c::finalizeState_Invisible() {}

void dLytMsgWindowDemo_c::initializeState_In() {
    mAnm[DEMO_ANIM_IN].setForwardOnce();
    mAnm[DEMO_ANIM_IN].setAnimEnable(true);
    mAnm[DEMO_ANIM_IN].setToStart();
}
void dLytMsgWindowDemo_c::executeState_In() {
    if (mAnm[DEMO_ANIM_IN].isStop2()) {
        mAnm[DEMO_ANIM_IN].setAnimEnable(false);
        mStateMgr.changeState(StateID_Visible);
    }
}
void dLytMsgWindowDemo_c::finalizeState_In() {}

void dLytMsgWindowDemo_c::initializeState_Visible() {}
void dLytMsgWindowDemo_c::executeState_Visible() {
    if (!mShouldBeOpen) {
        mStateMgr.changeState(StateID_Out);
    }
}
void dLytMsgWindowDemo_c::finalizeState_Visible() {}

void dLytMsgWindowDemo_c::initializeState_Out() {
    mAnm[DEMO_ANIM_OUT].setForwardOnce();
    mAnm[DEMO_ANIM_OUT].setAnimEnable(true);
    mAnm[DEMO_ANIM_OUT].setToStart();
}
void dLytMsgWindowDemo_c::executeState_Out() {
    if (mAnm[DEMO_ANIM_OUT].isStop2()) {
        mAnm[DEMO_ANIM_OUT].setAnimEnable(false);
        mStateMgr.changeState(StateID_Invisible);
    }
}
void dLytMsgWindowDemo_c::finalizeState_Out() {}

bool dLytMsgWindowDemo_c::execute() {
    mStateMgr.executeState();
    for (int i = 0; i < DEMO_NUM_ANIMS; i++) {
        if (mAnm[i].isEnabled()) {
            mAnm[i].play();
        }
    }
    mLyt.calc();
    return true;
}

void dLytMsgWindowDemo_c::draw() {
    SizedString<64> label = getTextLabel();
    // TODO figure out what these correspond to
    if (!(label == "Demo01_01:07") && !(label == "Demo01_01:15") && !(label == "Demo01_01:16") &&
        !(label == "Demo01_01:17") && !(label == "Demo01_01:18")) {
        // TODO order of evaluation
        s32 i = 0;
        if (mpTagProcessor->getNumLinesMaybe() > 1) {
            i = 1;
        }
        mBlurRelated.drawTextBlur(&mText, mOffsets[i], mpPane->GetAlpha());
    }
    mLyt.draw();
}

SizedString<64> dLytMsgWindowDemo_c::getTextLabel() {
    return dLytMsgWindow_c::getInstance()->getLabel();
}

void dLytMsgWindowDemo_c::open(dAcObjBase_c *obj, u32 param) {
    mShouldBeOpen = true;
}

bool dLytMsgWindowDemo_c::isDoneOpening() const {
    return !(*mStateMgr.getStateID() == StateID_In);
}

void dLytMsgWindowDemo_c::close() {
    mShouldBeOpen = false;
}

bool dLytMsgWindowDemo_c::isDoneClosing() const {
    return !(*mStateMgr.getStateID() == StateID_Out);
}

bool dLytMsgWindowDemo_c::setText(const wchar_t *text) {
    mpTextboxes[0]->SetTagProcessor(mpTagProcessor);
    mpTextboxes[1]->SetTagProcessor(mpTagProcessor);
    mpTextboxes[0]->setTextWithTextProcessor(text, mpTagProcessor, 0);
    mpTextboxes[1]->setTextWithTextProcessor(text, mpTagProcessor, 0);
    return true;
}
