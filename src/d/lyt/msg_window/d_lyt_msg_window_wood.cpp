#include "d/lyt/msg_window/d_lyt_msg_window_wood.h"

#include "common.h"
#include "d/d_tag_processor.h"
#include "d/lyt/d2d.h"
#include "d/lyt/d_textbox.h"
#include "nw4r/lyt/lyt_group.h"
#include "nw4r/lyt/lyt_pane.h"
#include "s/s_State.hpp"

STATE_DEFINE(dLytMsgWindowWood_c, Invisible);
STATE_DEFINE(dLytMsgWindowWood_c, In);
STATE_DEFINE(dLytMsgWindowWood_c, Visible);
STATE_DEFINE(dLytMsgWindowWood_c, Out);

static const d2d::LytBrlanMapping brlanMap[] = {
    {  "kanbanWindow_00_in.brlan", "G_inOut_00"},
    {"kanbanWindow_00_loop.brlan",  "G_loop_00"},
    { "kanbanWindow_00_out.brlan", "G_inOut_00"},
};

#define KANBAN_ANIM_IN 0
#define KANBAN_ANIM_LOOP 1
#define KANBAN_ANIM_OUT 2

#define KANBAN_NUM_ANIMS 3

static const char *sPanes[] = {
    "N_aBtn_00",
    "N_inOut_00",
};

static const char *sTextBoxes[] = {
    "T_message_00",
    "T_messageS_00",
};

static const char *sGroupName = "G_ref_00";

bool dLytMsgWindowWood_c::build(void *unk, d2d::ResAccIf_c *resAcc, dTagProcessor_c *tagProcessor) {
    mLyt.setResAcc(resAcc);
    mLyt.build("kanbanWindow_00.brlyt", nullptr);
    mLyt.setPriority(0x8A);

    for (int i = 0; i < KANBAN_NUM_ANIMS; i++) {
        mAnm[i].init(brlanMap[i].mFile, resAcc, mLyt.getLayout(), brlanMap[i].mName);
        mAnm[i].bind(false);
        mAnm[i].setAnimEnable(false);
    }

    mAnm[KANBAN_ANIM_LOOP].setAnimEnable(true);
    mAnm[KANBAN_ANIM_OUT].setAnimEnable(true);
    mAnm[KANBAN_ANIM_OUT].setToEnd();
    mLyt.calc();
    mAnm[KANBAN_ANIM_OUT].setAnimEnable(false);

    for (int i = 0; i < 2; i++) {
        mpPanes[i] = mLyt.findPane(sPanes[i]);
    }

    for (int i = 0; i < 2; i++) {
        mpTextboxes[i] = mLyt.getTextBox(sTextBoxes[i]);
        mpTextboxes[i]->SetTextPositionH(0);
        mpTextboxes[i]->SetTextPositionV(0);
    }

    mpTextboxes[1]->set0x1F6(1);

    for (int i = 0; i < 1; i++) {
        mNodes[i].mpLytPane = &mCommon;
        mCommon.build(resAcc);
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

bool dLytMsgWindowWood_c::remove() {
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
    mCommon.remove();
    for (int i = 0; i < KANBAN_NUM_ANIMS; i++) {
        mAnm[i].remove();
    }
    return true;
}

void dLytMsgWindowWood_c::initializeState_Invisible() {
    mCommon.resetToInvisble();
}
void dLytMsgWindowWood_c::executeState_Invisible() {
    if (mShouldBeOpen) {
        mStateMgr.changeState(StateID_In);
    }
}
void dLytMsgWindowWood_c::finalizeState_Invisible() {}

void dLytMsgWindowWood_c::initializeState_In() {
    mAnm[KANBAN_ANIM_IN].setAnimEnable(true);
    mAnm[KANBAN_ANIM_IN].setFrame(0.0f);
}
void dLytMsgWindowWood_c::executeState_In() {
    if (mAnm[KANBAN_ANIM_IN].isEndReached()) {
        mAnm[KANBAN_ANIM_IN].setAnimEnable(false);
        mStateMgr.changeState(StateID_Visible);
    }
}
void dLytMsgWindowWood_c::finalizeState_In() {}

void dLytMsgWindowWood_c::initializeState_Visible() {}
void dLytMsgWindowWood_c::executeState_Visible() {
    if (!mShouldBeOpen) {
        mStateMgr.changeState(StateID_Out);
    }
}
void dLytMsgWindowWood_c::finalizeState_Visible() {}

void dLytMsgWindowWood_c::initializeState_Out() {
    mAnm[KANBAN_ANIM_OUT].setAnimEnable(true);
    mAnm[KANBAN_ANIM_OUT].setFrame(0.0f);
}
void dLytMsgWindowWood_c::executeState_Out() {
    if (mAnm[KANBAN_ANIM_OUT].isEndReached()) {
        mAnm[KANBAN_ANIM_OUT].setAnimEnable(false);
        mStateMgr.changeState(StateID_Invisible);
    }
}
void dLytMsgWindowWood_c::finalizeState_Out() {}

bool dLytMsgWindowWood_c::execute() {
    mStateMgr.executeState();
    for (int i = 0; i < KANBAN_NUM_ANIMS; i++) {
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

void dLytMsgWindowWood_c::draw() {
    mLyt.draw();
}

void dLytMsgWindowWood_c::open() {
    mShouldBeOpen = true;
}

bool dLytMsgWindowWood_c::isOpening() const {
    return !(*mStateMgr.getStateID() == StateID_In);
}

void dLytMsgWindowWood_c::close() {
    mShouldBeOpen = false;
}

bool dLytMsgWindowWood_c::isClosing() const {
    return !(*mStateMgr.getStateID() == StateID_Out);
}

bool dLytMsgWindowWood_c::setText(const wchar_t *text) {
    mpTextboxes[0]->SetTagProcessor(mpTagProcessor);
    mpTextboxes[1]->SetTagProcessor(mpTagProcessor);
    mpTextboxes[0]->fn_800AF540(text, mpTagProcessor, 0);
    mpTextboxes[1]->fn_800AF540(text, mpTagProcessor, 0);
    return true;
}
