#include "d/lyt/msg_window/d_lyt_msg_window_link.h"

#include "common.h"
#include "d/d_tag_processor.h"
#include "d/d_lyt_hio.h"
#include "d/lyt/d2d.h"
#include "d/lyt/d_textbox.h"
#include "d/lyt/msg_window/d_lyt_msg_window_common.h"
#include "nw4r/lyt/lyt_group.h"
#include "nw4r/lyt/lyt_pane.h"
#include "s/s_State.hpp"

STATE_DEFINE(dLytMsgWindowLink_c, Invisible);
STATE_DEFINE(dLytMsgWindowLink_c, In);
STATE_DEFINE(dLytMsgWindowLink_c, Wait);
STATE_DEFINE(dLytMsgWindowLink_c, Visible);
STATE_DEFINE(dLytMsgWindowLink_c, Out);

static const d2d::LytBrlanMapping brlanMap[] = {
    {  "linkWindow_00_in.brlan", "G_inOut_00"},
    { "linkWindow_00_out.brlan", "G_inOut_00"},
    {"linkWindow_00_position.brlan",  "G_position_00"},
    {"linkWindow_00_loop.brlan",  "G_loop_00"},
};

#define LINK_ANIM_IN 0
#define LINK_ANIM_OUT 1
#define LINK_ANIM_POSITION 2
#define LINK_ANIM_LOOP 3

#define LINK_NUM_ANIMS 4

static const char *sPanes[] = {
    "N_window_00", "N_message_00", "N_aBtn_00", "N_inOut_00", "N_position_00",
};

static const char *sTextBoxes[] = {
    "T_message_00",
    "T_messageS_00",
};

static const char *sGroupName = "G_ref_00";

bool dLytMsgWindowLink_c::build(d2d::ResAccIf_c *resAcc1, d2d::ResAccIf_c *resAcc, dTagProcessor_c *tagProcessor) {
    mLyt.setResAcc(resAcc);
    mLyt.build("linkWindow_00.brlyt", nullptr);
    mLyt.setPriority(0x8A);

    for (int i = 0; i < LINK_NUM_ANIMS; i++) {
        mAnm[i].init(brlanMap[i].mFile, resAcc, mLyt.getLayout(), brlanMap[i].mName);
        mAnm[i].bind(false);
        mAnm[i].setAnimEnable(false);
    }

    mAnm[LINK_ANIM_LOOP].setAnimEnable(true);
    mAnm[LINK_ANIM_OUT].setAnimEnable(true);
    mAnm[LINK_ANIM_OUT].setToEnd();
    mLyt.calc();
    mAnm[LINK_ANIM_OUT].setAnimEnable(false);

    for (int i = 0; i < 5; i++) {
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

    mpPanes[3]->SetVisible(true);
    mLyt.calc();
    mpPanes[3]->SetVisible(false);
    mpTagProcessor = tagProcessor;

    mTranslationOrig.copyFrom(mpPanes[4]->GetTranslate());
    mTranslationTo.copyFrom(mpPanes[4]->GetTranslate());
    mTranslationFrom.copyFrom(mpPanes[4]->GetTranslate());

    mParam = 0;

    mOffset.x = mOffset.y = 0.0f;
    
    mWaitDelay = 0;

    mStateMgr.changeState(StateID_Invisible);
    return true;
}

bool dLytMsgWindowLink_c::remove() {
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
    for (int i = 0; i < LINK_NUM_ANIMS; i++) {
        mAnm[i].remove();
    }
    return true;
}

void dLytMsgWindowLink_c::initializeState_Invisible() {
    mBtn.resetToInvisble();
}
void dLytMsgWindowLink_c::executeState_Invisible() {
    if (mShouldBeOpen) {
        mStateMgr.changeState(StateID_In);
    }
}
void dLytMsgWindowLink_c::finalizeState_Invisible() {}

void dLytMsgWindowLink_c::initializeState_In() {
    mAnm[LINK_ANIM_POSITION].setFrame(mParam);
    mAnm[LINK_ANIM_POSITION].setAnimEnable(true);
    mLyt.calc();
    mAnm[LINK_ANIM_POSITION].setAnimEnable(false);

    mTranslationTo = mTranslationOrig;
    mTranslationTo.y = mpPanes[4]->GetTranslate().y;
    mTranslationFrom = mTranslationTo;

    mAnm[LINK_ANIM_IN].setAnimEnable(true);
    mAnm[LINK_ANIM_IN].setFrame(0.0f);

    mpPanes[4]->SetTranslate(mTranslationFrom);
}
void dLytMsgWindowLink_c::executeState_In() {
    if (mTranslationFrom != mTranslationTo) {
        mVec3_c v;
        if (mAnm[LINK_ANIM_IN].isEndReached()) {
            v = mTranslationTo;
        } else {
            v.copyFrom(mpPanes[4]->GetTranslate());
            mVec3_c tmp = (mTranslationTo - v) / 2.0f;
            v += tmp;
        }
        mpPanes[4]->SetTranslate(v);
    }

    if (mAnm[LINK_ANIM_IN].isEndReached()) {
        mAnm[LINK_ANIM_IN].setAnimEnable(false);
        mStateMgr.changeState(StateID_Wait);
    }
}
void dLytMsgWindowLink_c::finalizeState_In() {}

void dLytMsgWindowLink_c::initializeState_Wait() {
    mWaitDelay = dLyt_HIO_c::getInstance()->getMsgWindowWaitDelay();
}
void dLytMsgWindowLink_c::executeState_Wait() {
    if (--mWaitDelay > 0) {
        return;
    }
    mStateMgr.changeState(StateID_Visible);
}
void dLytMsgWindowLink_c::finalizeState_Wait() {}

void dLytMsgWindowLink_c::initializeState_Visible() {
    nw4r::lyt::Size size = getTextBox()->GetSize();
    nw4r::lyt::Size fontSize = getTextBox()->GetFontSize();
    nw4r::math::MTX34 transform = getTextBox()->GetGlobalMtx();

    mOffset.x = transform._03 - size.width / 2;
    mOffset.y = transform._13 + size.height / 2;
}
void dLytMsgWindowLink_c::executeState_Visible() {
    if (!mShouldBeOpen) {
        mStateMgr.changeState(StateID_Out);
    }
}
void dLytMsgWindowLink_c::finalizeState_Visible() {}

void dLytMsgWindowLink_c::initializeState_Out() {
    mTranslationFrom = mTranslationTo;
    mAnm[LINK_ANIM_OUT].setAnimEnable(true);
    mAnm[LINK_ANIM_OUT].setFrame(0.0f);
}
void dLytMsgWindowLink_c::executeState_Out() {
    if (mTranslationFrom != mTranslationTo) {
        mVec3_c v;
        if (mAnm[LINK_ANIM_OUT].isEndReached()) {
            v = mTranslationFrom;
        } else {
            f32 ratio = mAnm[LINK_ANIM_OUT].getRatio();
            if (ratio > 1.0f) {
                ratio = 1.0f;
            }
            v = mTranslationTo + (mTranslationFrom - mTranslationTo) * ratio;
        }
        mpPanes[4]->SetTranslate(v);
    }

    if (mAnm[LINK_ANIM_OUT].isEndReached()) {
        mAnm[LINK_ANIM_OUT].setAnimEnable(false);
        mStateMgr.changeState(StateID_Invisible);
    }
}
void dLytMsgWindowLink_c::finalizeState_Out() {}

bool dLytMsgWindowLink_c::execute() {
    mStateMgr.executeState();
    for (int i = 0; i < LINK_NUM_ANIMS; i++) {
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

void dLytMsgWindowLink_c::draw() {
    // NONMATCHING
    // TODO - SetVisible clrlwi
    // The instruction patterns here are the same that EGG::StateGX::Scoped
    // generates, but there's no RAII to be found here. What's the missing
    // link that makes the compiler generate these?
    // When you fix this, fix other instances of this problem
    // by searching for 91657b77
    bool bVis[3] = {
        mpPanes[0]->IsVisible(),
        mpPanes[1]->IsVisible(),
        mpPanes[2]->IsVisible(),
    };

    mpPanes[0]->SetVisible(bVis[0]);
    mpPanes[1]->SetVisible(false);
    mpPanes[2]->SetVisible(false);

    mLyt.draw();
    if (mWaitDelay <= 0) {
        mBlurRelated.drawTextBlur(&mText, mOffset, mpPanes[3]->GetAlpha());
    }

    mpPanes[0]->SetVisible(false);
    mpPanes[1]->SetVisible(bVis[1]);
    mpPanes[2]->SetVisible(bVis[2]);

    mLyt.draw();

    mpPanes[0]->SetVisible(bVis[0]);
    mpPanes[1]->SetVisible(bVis[1]);
    mpPanes[2]->SetVisible(bVis[2]);
}

void dLytMsgWindowLink_c::open(dAcObjBase_c *obj, u32 param) {
    mpActor = obj;
    mParam = param;
    mShouldBeOpen = true;
}

// Curiously enough, this file is almost an exact copy of d_lyt_msg_window_talk,
// but the behavior of these two functions (isDoneOpening/isDoneClosing) is swapped
bool dLytMsgWindowLink_c::isDoneOpening() const {
    return !(*mStateMgr.getStateID() == StateID_In);
}

void dLytMsgWindowLink_c::close() {
    mShouldBeOpen = false;
}

bool dLytMsgWindowLink_c::isDoneClosing() const {
    if (*mStateMgr.getStateID() == StateID_Out || *mStateMgr.getStateID() == StateID_Wait) {
        return false;
    }
    return true;
}

bool dLytMsgWindowLink_c::setText(const wchar_t *text) {
    mpTextboxes[0]->SetTagProcessor(mpTagProcessor);
    mpTextboxes[1]->SetTagProcessor(mpTagProcessor);
    mpTextboxes[0]->setTextWithTextProcessor(text, mpTagProcessor, 0);
    mpTextboxes[1]->setTextWithTextProcessor(text, mpTagProcessor, 0);
    return true;
}
