#include "d/lyt/msg_window/d_lyt_msg_window_get.h"

#include "common.h"
#include "d/d_tag_processor.h"
#include "d/lyt/d2d.h"
#include "d/lyt/d_textbox.h"
#include "d/lyt/msg_window/d_lyt_msg_window_common.h"
#include "nw4r/lyt/lyt_group.h"
#include "nw4r/lyt/lyt_pane.h"
#include "s/s_State.hpp"

STATE_DEFINE(dLytMsgWindowGet_c, Invisible);
STATE_DEFINE(dLytMsgWindowGet_c, In);
STATE_DEFINE(dLytMsgWindowGet_c, Visible);
STATE_DEFINE(dLytMsgWindowGet_c, Out);

static const d2d::LytBrlanMapping brlanMap[] = {
    {  "getWindow_00_in.brlan", "G_inOut_00"},
    { "getWindow_00_out.brlan", "G_inOut_00"},
    {"getWindow_00_loop.brlan",  "G_loop_00"},
};

#define GET_ANIM_IN 0
#define GET_ANIM_OUT 1
#define GET_ANIM_LOOP 2

#define GET_NUM_ANIMS 3

static const char *sPanes[] = {
    "N_window_00", "P_light_03", "P_light_02", "N_message_00", "N_aBtn_00", "N_inOut_00",
};

static const char *sTextBoxes[] = {
    "T_message_00",
    "T_messageS_00",
};

static const char *sGroupName = "G_ref_00";

bool dLytMsgWindowGet_c::build(d2d::ResAccIf_c *resAcc1, d2d::ResAccIf_c *resAcc, dTagProcessor_c *tagProcessor) {
    mLyt.setResAcc(resAcc);
    mLyt.build("getWindow_00.brlyt", nullptr);
    mLyt.setPriority(0x8A);

    for (int i = 0; i < GET_NUM_ANIMS; i++) {
        mAnm[i].init(brlanMap[i].mFile, resAcc, mLyt.getLayout(), brlanMap[i].mName);
        mAnm[i].bind(false);
        mAnm[i].setAnimEnable(false);
    }

    mAnm[GET_ANIM_LOOP].setAnimEnable(true);
    mAnm[GET_ANIM_OUT].setAnimEnable(true);
    mAnm[GET_ANIM_OUT].setToEnd();
    mLyt.calc();
    mAnm[GET_ANIM_OUT].setAnimEnable(false);

    for (int i = 0; i < 6; i++) {
        mpPanes[i] = mLyt.findPane(sPanes[i]);
    }

    for (int i = 0; i < 2; i++) {
        mpTextboxes[i] = mLyt.getTextBox(sTextBoxes[i]);
        mpTextboxes[i]->SetTextPositionH(0);
        mpTextboxes[i]->SetTextPositionV(0);
    }

    mpTextboxes[1]->set0x1F6(1);

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

    mText.build(resAcc, dLytTextLight::NORMAL);
    mBlurRelated.build();

    mpPanes[5]->SetVisible(true);
    mLyt.calc();
    mpPanes[5]->SetVisible(false);
    mpTagProcessor = tagProcessor;
    mOffset.x = mOffset.y = 0.0f;
    mStateMgr.changeState(StateID_Invisible);
    return true;
}

bool dLytMsgWindowGet_c::remove() {
    mText.remove();

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
    for (int i = 0; i < GET_NUM_ANIMS; i++) {
        mAnm[i].remove();
    }
    return true;
}

void dLytMsgWindowGet_c::initializeState_Invisible() {
    mBtn.resetToInvisble();
}
void dLytMsgWindowGet_c::executeState_Invisible() {
    if (mShouldBeOpen) {
        mStateMgr.changeState(StateID_In);
    }
}
void dLytMsgWindowGet_c::finalizeState_Invisible() {}

void dLytMsgWindowGet_c::initializeState_In() {
    mAnm[GET_ANIM_IN].setAnimEnable(true);
    mAnm[GET_ANIM_IN].setFrame(0.0f);
}
void dLytMsgWindowGet_c::executeState_In() {
    if (mAnm[GET_ANIM_IN].isEndReached()) {
        mAnm[GET_ANIM_IN].setAnimEnable(false);
        mStateMgr.changeState(StateID_Visible);
    }
}
void dLytMsgWindowGet_c::finalizeState_In() {}

void dLytMsgWindowGet_c::initializeState_Visible() {
    nw4r::lyt::Size size = getTextBox()->GetSize();
    nw4r::lyt::Size fontSize = getTextBox()->GetFontSize();
    nw4r::math::MTX34 transform = getTextBox()->GetGlobalMtx();

    mOffset.x = transform._03 - size.width / 2.0f;
    mOffset.y = transform._13 + size.height / 2.0f;
}
void dLytMsgWindowGet_c::executeState_Visible() {
    if (!mShouldBeOpen) {
        mStateMgr.changeState(StateID_Out);
    }
}
void dLytMsgWindowGet_c::finalizeState_Visible() {}

void dLytMsgWindowGet_c::initializeState_Out() {
    mAnm[GET_ANIM_OUT].setAnimEnable(true);
    mAnm[GET_ANIM_OUT].setFrame(0.0f);
}
void dLytMsgWindowGet_c::executeState_Out() {
    if (mAnm[GET_ANIM_OUT].isEndReached()) {
        mAnm[GET_ANIM_OUT].setAnimEnable(false);
        mStateMgr.changeState(StateID_Invisible);
    }
}
void dLytMsgWindowGet_c::finalizeState_Out() {}

bool dLytMsgWindowGet_c::execute() {
    mStateMgr.executeState();
    for (int i = 0; i < GET_NUM_ANIMS; i++) {
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

void dLytMsgWindowGet_c::draw() {
    // NONMATCHING
    // The instruction patterns here are the same that EGG::StateGX::Scoped
    // generates, but there's no RAII to be found here. What's the missing
    // link that makes the compiler generate these?
    // When you fix this, fix other instances of this problem
    // by searching for 91657b77
    bool bVis[5] = {
        mpPanes[0]->IsVisible(),
        mpPanes[1]->IsVisible(),
        mpPanes[2]->IsVisible(),
        mpPanes[3]->IsVisible(),
        mpPanes[4]->IsVisible(),
    };

    mpPanes[0]->SetVisible(bVis[0]);
    mpPanes[1]->SetVisible(bVis[1]);
    mpPanes[2]->SetVisible(bVis[2]);
    mpPanes[3]->SetVisible(false);
    mpPanes[4]->SetVisible(false);

    mLyt.draw();
    mBlurRelated.drawTextBlur(&mText, mOffset, mpPanes[5]->GetAlpha());

    mpPanes[0]->SetVisible(false);
    mpPanes[1]->SetVisible(false);
    mpPanes[2]->SetVisible(false);
    mpPanes[3]->SetVisible(bVis[3]);
    mpPanes[4]->SetVisible(bVis[4]);

    mLyt.draw();

    mpPanes[0]->SetVisible(bVis[0]);
    mpPanes[1]->SetVisible(bVis[1]);
    mpPanes[2]->SetVisible(bVis[2]);
    mpPanes[3]->SetVisible(bVis[3]);
    mpPanes[4]->SetVisible(bVis[4]);
}

void dLytMsgWindowGet_c::open(dAcObjBase_c *obj, u32 param) {
    mShouldBeOpen = true;
}

bool dLytMsgWindowGet_c::isDoneOpening() const {
    return !(*mStateMgr.getStateID() == StateID_In);
}

void dLytMsgWindowGet_c::close() {
    mShouldBeOpen = false;
}

bool dLytMsgWindowGet_c::isDoneClosing() const {
    return !(*mStateMgr.getStateID() == StateID_Out);
}

bool dLytMsgWindowGet_c::setText(const wchar_t *text) {
    mpTextboxes[0]->SetTagProcessor(mpTagProcessor);
    mpTextboxes[1]->SetTagProcessor(mpTagProcessor);
    mpTextboxes[0]->setTextWithTextProcessor(text, mpTagProcessor, 0);
    mpTextboxes[1]->setTextWithTextProcessor(text, mpTagProcessor, 0);
    return true;
}
