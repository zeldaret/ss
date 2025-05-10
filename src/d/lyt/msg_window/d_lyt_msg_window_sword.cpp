#include "d/lyt/msg_window/d_lyt_msg_window_sword.h"

#include "common.h"
#include "d/d_tag_processor.h"
#include "d/d_textunk.h"
#include "d/lyt/d2d.h"
#include "d/lyt/d_textbox.h"
#include "m/m_vec.h"
#include "nw4r/lyt/lyt_group.h"
#include "nw4r/lyt/lyt_pane.h"
#include "nw4r/lyt/lyt_types.h"
#include "nw4r/math/math_types.h"
#include "s/s_State.hpp"
#include "toBeSorted/lyt_related_floats.h"

STATE_DEFINE(dLytMsgWindowSword_c, Invisible);
STATE_DEFINE(dLytMsgWindowSword_c, In);
STATE_DEFINE(dLytMsgWindowSword_c, Wait);
STATE_DEFINE(dLytMsgWindowSword_c, Visible);
STATE_DEFINE(dLytMsgWindowSword_c, Out);

static const d2d::LytBrlanMapping brlanMap[] = {
    {      "messageWindow_01_in.brlan",    "G_inOut_00"},
    {     "messageWindow_01_out.brlan",    "G_inOut_00"},
    {    "messageWindow_01_loop.brlan",     "G_loop_00"},
    {   "messageWindow_01_color.brlan",    "G_color_00"},
    {"messageWindow_01_position.brlan", "G_position_00"},
};

#define SWORD_ANIM_IN 0
#define SWORD_ANIM_OUT 1
#define SWORD_ANIM_LOOP 2
#define SWORD_ANIM_COLOR 3
#define SWORD_ANIM_POSITION 4

#define SWORD_NUM_ANIMS 5

#define TRANSLATE_ORIGINAL 0
#define TRANSLATE_TO 1
#define TRANSLATE_FROM 2

static const char *sPanes[] = {
    "N_aBtn_00",
    "N_inOut_00",
    "N_position_00",
};

static const char *sTextBoxes[] = {
    "T_message_00",
    "T_messageS_00",
};

static const char *sGroupName = "G_ref_00";

bool dLytMsgWindowSword_c::build(d2d::ResAccIf_c *resAcc1, d2d::ResAccIf_c *resAcc, dTagProcessor_c *tagProcessor) {
    mLyt.setResAcc(resAcc);
    mLyt.build("messageWindow_01.brlyt", nullptr);
    mLyt.setPriority(0x8A);

    for (int i = 0; i < SWORD_NUM_ANIMS; i++) {
        mAnm[i].init(brlanMap[i].mFile, resAcc, mLyt.getLayout(), brlanMap[i].mName);
        mAnm[i].bind(false);
        mAnm[i].setAnimEnable(false);
    }

    mAnm[SWORD_ANIM_LOOP].setAnimEnable(true);
    mAnm[SWORD_ANIM_OUT].setAnimEnable(true);
    mAnm[SWORD_ANIM_OUT].setToEnd();
    mLyt.calc();
    mAnm[SWORD_ANIM_OUT].setAnimEnable(false);

    for (int i = 0; i < 3; i++) {
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

    mSwordText.build(resAcc, mSwordType);

    for (int i = 0; i < 1023; i++) {
        mCharacterData[i].reset();
        mTextBuf[i] = L'\0';
    }

    mAnm[SWORD_ANIM_COLOR].setAnimEnable(true);
    mAnm[SWORD_ANIM_COLOR].setFrame(mSwordType);

    mpPanes[1]->SetVisible(true);
    mLyt.calc();
    mpPanes[1]->SetVisible(false);

    mAnm[SWORD_ANIM_COLOR].setAnimEnable(false);

    mpTagProcessor = tagProcessor;

    mTranslationOrig.copyFrom(mpPanes[2]->GetTranslate());
    mTranslationTo.copyFrom(mpPanes[2]->GetTranslate());
    mTranslationFrom.copyFrom(mpPanes[2]->GetTranslate());

    mParam = 0;
    mOffset.x = mOffset.y = 0.0f;
    mWaitDelay = 0;

    mHasDrawnThisTick = false;

    mStateMgr.changeState(StateID_Invisible);
    return true;
}

bool dLytMsgWindowSword_c::remove() {
    mSwordText.remove();

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
    for (int i = 0; i < SWORD_NUM_ANIMS; i++) {
        mAnm[i].remove();
    }
    return true;
}

void dLytMsgWindowSword_c::initializeState_Invisible() {
    mBtn.resetToInvisble();
}
void dLytMsgWindowSword_c::executeState_Invisible() {
    if (mShouldBeOpen) {
        mStateMgr.changeState(StateID_In);
    }
}
void dLytMsgWindowSword_c::finalizeState_Invisible() {}

void dLytMsgWindowSword_c::initializeState_In() {
    mAnm[SWORD_ANIM_POSITION].setFrame(mParam);
    mAnm[SWORD_ANIM_POSITION].setAnimEnable(true);
    mLyt.calc();
    mAnm[SWORD_ANIM_POSITION].setAnimEnable(false);

    mTranslationTo = mTranslationOrig;
    mTranslationTo.y = mpPanes[2]->GetTranslate().y;
    mTranslationFrom = mTranslationTo;

    mAnm[SWORD_ANIM_IN].setAnimEnable(true);
    mAnm[SWORD_ANIM_IN].setFrame(0.0f);

    mpPanes[2]->SetTranslate(mTranslationFrom);
}
void dLytMsgWindowSword_c::executeState_In() {
    if (mTranslationFrom != mTranslationTo) {
        mVec3_c v;
        if (mAnm[SWORD_ANIM_IN].isEndReached()) {
            v = mTranslationTo;
        } else {
            f32 ratio = mAnm[SWORD_ANIM_IN].getRatio();
            if (ratio > 1.0f) {
                ratio = 1.0f;
            }
            v = mTranslationFrom + (mTranslationTo - mTranslationFrom) * ratio;
        }
        mpPanes[2]->SetTranslate(v);
    }

    if (mAnm[SWORD_ANIM_IN].isEndReached()) {
        mAnm[SWORD_ANIM_IN].setAnimEnable(false);
        mStateMgr.changeState(StateID_Wait);
    }
}
void dLytMsgWindowSword_c::finalizeState_In() {}

void dLytMsgWindowSword_c::initializeState_Wait() {
    mWaitDelay = UnkTextThing::getInstance()->getMsgWindowWaitDelay();
}
void dLytMsgWindowSword_c::executeState_Wait() {
    if (--mWaitDelay > 0) {
        return;
    }
    mStateMgr.changeState(StateID_Visible);
}
void dLytMsgWindowSword_c::finalizeState_Wait() {}

void dLytMsgWindowSword_c::initializeState_Visible() {
    nw4r::lyt::Size size = getTextBox()->GetSize();
    nw4r::lyt::Size fontSize = getTextBox()->GetFontSize(); // unused
    nw4r::math::MTX34 transform = getTextBox()->GetGlobalMtx();

    mOffset.x = transform._03 - size.width / 2;
    mOffset.y = transform._13 + size.height / 2;

    getTextBox()->GetTextDrawRect(); // unused
}
void dLytMsgWindowSword_c::executeState_Visible() {
    if (!mShouldBeOpen) {
        mStateMgr.changeState(StateID_Out);
    }
}
void dLytMsgWindowSword_c::finalizeState_Visible() {}

void dLytMsgWindowSword_c::initializeState_Out() {
    mTranslationFrom = mTranslationTo;
    mAnm[SWORD_ANIM_OUT].setAnimEnable(true);
    mAnm[SWORD_ANIM_OUT].setFrame(0.0f);
}
void dLytMsgWindowSword_c::executeState_Out() {
    if (mTranslationFrom != mTranslationTo) {
        mVec3_c v;
        if (mAnm[SWORD_ANIM_OUT].isEndReached()) {
            v = mTranslationFrom;
        } else {
            v.copyFrom(mpPanes[2]->GetTranslate());
            mVec3_c tmp = (mTranslationTo - v) / 2.0f;
            v += tmp;
        }
        mpPanes[2]->SetTranslate(v);
    }

    if (mAnm[SWORD_ANIM_OUT].isEndReached()) {
        mAnm[SWORD_ANIM_OUT].setAnimEnable(false);
        mStateMgr.changeState(StateID_Invisible);
    }
}
void dLytMsgWindowSword_c::finalizeState_Out() {}

bool dLytMsgWindowSword_c::execute() {
    mStateMgr.executeState();

    for (int i = 0; i < SWORD_NUM_ANIMS; i++) {
        if (mAnm[i].isEnabled()) {
            mAnm[i].play();
        }
    }

    mLyt.calc();
    mNodes[0].mpLytPane->execute();
    mHasDrawnThisTick = false;
    return true;
}

void dLytMsgWindowSword_c::draw() {
    mLyt.draw();

    for (int i = 0; i < 1023; i++) {
        if (mCharacterData[i].displayTimerMaybe >= 0) {
            if (!mHasDrawnThisTick) {
                mCharacterData[i].displayTimerMaybe += 1;
            }
            mVec3_c v(
                (mOffset.x + mCharacterData[i].posX) / get_805751A4(), mOffset.y + mCharacterData[i].posY, 0.0f
            );

            wchar_t str[2];
            str[0] = mCharacterData[i].character;
            str[1] = 0;

            mSwordText.draw(str, mCharacterData[i].displayTimerMaybe, v, mCharacterData[i].field_0x08);
            if (mCharacterData[i].displayTimerMaybe >= mSwordText.getAnimDuration()) {
                mCharacterData[i].displayTimerMaybe = -1;
            }
        }
    }

    if (!mHasDrawnThisTick) {
        mHasDrawnThisTick = true;
    }
}

void dLytMsgWindowSword_c::open(dAcObjBase_c *obj, u32 param) {
    mpActor = obj;
    mParam = param;
    mShouldBeOpen = true;
}

bool dLytMsgWindowSword_c::isDoneOpening() const {
    if (*mStateMgr.getStateID() == StateID_In || *mStateMgr.getStateID() == StateID_Wait) {
        return false;
    }
    return true;
}

void dLytMsgWindowSword_c::close() {
    mShouldBeOpen = false;
}

bool dLytMsgWindowSword_c::isDoneClosing() const {
    return !(*mStateMgr.getStateID() == StateID_Out);
}

bool dLytMsgWindowSword_c::setText(const wchar_t *text) {
    int foundIdx = 1023;
    for (int i = 0; i < 1023; i++) {
        if (mCharacterData[i].displayTimerMaybe >= 0 && foundIdx > mCharacterData[i].field_0x0C) {
            foundIdx = mCharacterData[i].field_0x0C;
        }
    }

    // How is text bounds-checked here????
    for (int i = 0; i < 1023; i++) {
        if (i < foundIdx) {
            mTextBuf[i] = text[i];
        } else {
            mTextBuf[i] = L' ';
        }
    }

    mpTextboxes[0]->SetTagProcessor(mpTagProcessor);
    mpTextboxes[1]->SetTagProcessor(mpTagProcessor);
    mpTextboxes[0]->setTextWithTextProcessor(mTextBuf, mpTagProcessor, 0);
    mpTextboxes[1]->setTextWithTextProcessor(mTextBuf, mpTagProcessor, 0);
    return foundIdx == 1023;
}
