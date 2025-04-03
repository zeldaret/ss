#include "d/lyt/msg_window/d_lyt_msg_window_stone.h"

#include "common.h"
#include "d/d_tag_processor.h"
#include "d/lyt/d2d.h"
#include "d/lyt/d_textbox.h"
#include "nw4r/lyt/lyt_group.h"
#include "nw4r/lyt/lyt_pane.h"
#include "s/s_State.hpp"

STATE_DEFINE(dLytMsgWindowStone_c, Invisible);
STATE_DEFINE(dLytMsgWindowStone_c, In);
STATE_DEFINE(dLytMsgWindowStone_c, Visible);
STATE_DEFINE(dLytMsgWindowStone_c, Out);

static const d2d::LytBrlanMapping brlanMap[] = {
    {  "sekibanWindow_00_in.brlan", "G_inOut_00"},
    {"sekibanWindow_00_loop.brlan",  "G_loop_00"},
    { "sekibanWindow_00_out.brlan", "G_inOut_00"},
};

#define SEKIBAN_ANIM_IN 0
#define SEKIBAN_ANIM_LOOP 1
#define SEKIBAN_ANIM_OUT 2

#define SEKIBAN_NUM_ANIMS 3

static const char *sPanes[] = {
    "N_aBtn_00",
    "N_inOut_00",
};

static const char *sTextBoxes[] = {
    "T_message_00",
    "T_messageS_00",
    "T_messageS_01",
};

static const char *sGroupName = "G_ref_00";

bool dLytMsgWindowStone_c::build(d2d::ResAccIf_c *resAcc1, d2d::ResAccIf_c *resAcc, dTagProcessor_c *tagProcessor) {
    mLyt.setResAcc(resAcc);
    mLyt.build("sekibanWindow_00.brlyt", nullptr);
    mLyt.setPriority(0x8A);

    for (int i = 0; i < SEKIBAN_NUM_ANIMS; i++) {
        mAnm[i].init(brlanMap[i].mFile, resAcc, mLyt.getLayout(), brlanMap[i].mName);
        mAnm[i].bind(false);
        mAnm[i].setAnimEnable(false);
    }

    mAnm[SEKIBAN_ANIM_LOOP].setAnimEnable(true);
    mAnm[SEKIBAN_ANIM_OUT].setAnimEnable(true);
    mAnm[SEKIBAN_ANIM_OUT].setToEnd();
    mLyt.calc();
    mAnm[SEKIBAN_ANIM_OUT].setAnimEnable(false);

    for (int i = 0; i < 2; i++) {
        mpPanes[i] = mLyt.findPane(sPanes[i]);
    }

    for (int i = 0; i < 3; i++) {
        mpTextboxes[i] = mLyt.getTextBox(sTextBoxes[i]);
        mpTextboxes[i]->SetTextPositionH(0);
        mpTextboxes[i]->SetTextPositionV(0);
    }

    mpTextboxes[1]->set0x1F6(1);
    mpTextboxes[2]->set0x1F6(1);

    for (int i = 0; i < 1; i++) {
        mNodes[i].mpLytPane = &mBtn;
        mBtn.build(resAcc);
        mSubpaneList.PushBack(&mNodes[i]);
    }

    if (mLyt.getLayout()->GetGroupContainer() != nullptr) {
        nw4r::lyt::Group *g = mLyt.getLayout()->GetGroupContainer()->FindGroupByName(sGroupName);
        if (g != nullptr) {
            d2d::dSubPane::linkMeters(g, &mSubpaneList);
        }
    }

    mpPanes[1]->SetVisible(true);
    mLyt.calc();
    mpPanes[1]->SetVisible(false);
    mpTagProcessor = tagProcessor;
    mStateMgr.changeState(StateID_Invisible);
    return true;
}

bool dLytMsgWindowStone_c::remove() {
    for (d2d::SubPaneList::Iterator it = mSubpaneList.GetBeginIter(); it != mSubpaneList.GetEndIter(); ++it) {
        d2d::dSubPane *subPane = it->mpLytPane;
        if (subPane != nullptr) {
            nw4r::lyt::Pane *parent = subPane->getPane()->GetParent();
            parent->RemoveChild(subPane->getPane());
        }
    }

    for (int i = 0; i < 1; i++) {
        mNodes[i].mpLytPane->remove();
    }
    mBtn.remove();
    for (int i = 0; i < SEKIBAN_NUM_ANIMS; i++) {
        mAnm[i].remove();
    }
    return true;
}

void dLytMsgWindowStone_c::initializeState_Invisible() {
    mBtn.resetToInvisble();
}
void dLytMsgWindowStone_c::executeState_Invisible() {
    if (mShouldBeOpen) {
        mStateMgr.changeState(StateID_In);
    }
}
void dLytMsgWindowStone_c::finalizeState_Invisible() {}

void dLytMsgWindowStone_c::initializeState_In() {
    mAnm[SEKIBAN_ANIM_IN].setAnimEnable(true);
    mAnm[SEKIBAN_ANIM_IN].setFrame(0.0f);
}
void dLytMsgWindowStone_c::executeState_In() {
    if (mAnm[SEKIBAN_ANIM_IN].isEndReached()) {
        mAnm[SEKIBAN_ANIM_IN].setAnimEnable(false);
        mStateMgr.changeState(StateID_Visible);
    }
}
void dLytMsgWindowStone_c::finalizeState_In() {}

void dLytMsgWindowStone_c::initializeState_Visible() {}
void dLytMsgWindowStone_c::executeState_Visible() {
    if (!mShouldBeOpen) {
        mStateMgr.changeState(StateID_Out);
    }
}
void dLytMsgWindowStone_c::finalizeState_Visible() {}

void dLytMsgWindowStone_c::initializeState_Out() {
    mAnm[SEKIBAN_ANIM_OUT].setAnimEnable(true);
    mAnm[SEKIBAN_ANIM_OUT].setFrame(0.0f);
}
void dLytMsgWindowStone_c::executeState_Out() {
    if (mAnm[SEKIBAN_ANIM_OUT].isEndReached()) {
        mAnm[SEKIBAN_ANIM_OUT].setAnimEnable(false);
        mStateMgr.changeState(StateID_Invisible);
    }
}
void dLytMsgWindowStone_c::finalizeState_Out() {}

bool dLytMsgWindowStone_c::execute() {
    mStateMgr.executeState();
    for (int i = 0; i < SEKIBAN_NUM_ANIMS; i++) {
        if (mAnm[i].isEnabled()) {
            mAnm[i].play();
        }
    }
    mLyt.calc();
    for (int i = 0; i < 1; i++) {
        mNodes[i].mpLytPane->execute();
    }
    return true;
}

void dLytMsgWindowStone_c::draw() {
    mLyt.draw();
}

void dLytMsgWindowStone_c::open(dAcObjBase_c *obj, u32 param) {
    mShouldBeOpen = true;
}

bool dLytMsgWindowStone_c::isDoneOpening() const {
    return !(*mStateMgr.getStateID() == StateID_In);
}

void dLytMsgWindowStone_c::close() {
    mShouldBeOpen = false;
}

bool dLytMsgWindowStone_c::isDoneClosing() const {
    return !(*mStateMgr.getStateID() == StateID_Out);
}

bool dLytMsgWindowStone_c::setText(const wchar_t *text) {
    mpTextboxes[0]->SetTagProcessor(mpTagProcessor);
    mpTextboxes[1]->SetTagProcessor(mpTagProcessor);
    mpTextboxes[2]->SetTagProcessor(mpTagProcessor);
    mpTextboxes[0]->setTextWithTextProcessor(text, mpTagProcessor, 0);
    mpTextboxes[1]->setTextWithTextProcessor(text, mpTagProcessor, 0);
    mpTextboxes[2]->setTextWithTextProcessor(text, mpTagProcessor, 0);
    return true;
}
