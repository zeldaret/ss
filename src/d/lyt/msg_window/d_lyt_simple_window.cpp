#include "d/lyt/msg_window/d_lyt_simple_window.h"

#include "common.h"
#include "d/d_lyt_hio.h"
#include "d/d_message.h"
#include "d/d_tag_processor.h"
#include "d/d_textwindow_unk.h"
#include "d/lyt/d_lyt_control_game.h"
#include "d/lyt/msg_window/d_lyt_msg_window.h"
#include "d/lyt/msg_window/d_lyt_msg_window_common.h"
#include "m/m_vec.h"
#include "nw4r/lyt/lyt_types.h"
#include "nw4r/math/math_types.h"
#include "sized_string.h"
#include "toBeSorted/arc_managers/layout_arc_manager.h"
#include "toBeSorted/event_manager.h"

STATE_DEFINE(dLytSimpleWindow_c, Invisible);
STATE_DEFINE(dLytSimpleWindow_c, In);
STATE_DEFINE(dLytSimpleWindow_c, OutputText);
STATE_DEFINE(dLytSimpleWindow_c, Out);

dLytSimpleWindow_c *dLytSimpleWindow_c::sInstance;

bool dLytSimpleWindowSubtype::build(d2d::ResAccIf_c *, dTagProcessor_c *) {
    return true;
}

void dLytSimpleWindowSubtype::initializeIn(s32 pos) {
    // no-op
}

bool dLytSimpleWindowSubtype::setText(const wchar_t *text) {
    return true;
}

static const d2d::LytBrlanMapping brlanMapTalk[] = {
    {      "messageWindow_00_in.brlan",    "G_inOut_00"},
    {     "messageWindow_00_out.brlan",    "G_inOut_00"},
    {"messageWindow_00_position.brlan", "G_position_00"},
    {    "messageWindow_00_loop.brlan",     "G_loop_00"},
};

#define SIMPLE_WINDOW_TALK_ANIM_IN 0
#define SIMPLE_WINDOW_TALK_ANIM_OUT 1
#define SIMPLE_WINDOW_TALK_ANIM_POSITION 2
#define SIMPLE_WINDOW_TALK_ANIM_LOOP 3

#define SIMPLE_WINDOW_TALK_NUM_ANIMS 4

static const char *sTalkPaneNames[] = {
    "N_window_00", "N_message_00", "N_aBtn_00", "N_inOut_00", "N_position_00",
};

#define SIMPLE_WINDOW_TALK_PANE_WINDOW 0
#define SIMPLE_WINDOW_TALK_PANE_MESSAGE 1
#define SIMPLE_WINDOW_TALK_PANE_A_BTN 2
#define SIMPLE_WINDOW_TALK_PANE_IN_OUT 3
#define SIMPLE_WINDOW_TALK_PANE_POSITION 4

#define SIMPLE_WINDOW_TALK_NUM_PANES 5

static const char *sTalkTextboxNames[] = {
    "T_message_00",
    "T_messageS_00",
};

#define SIMPLE_WINDOW_TALK_NUM_TEXTBOXES 2

bool dLytSimpleWindowTalk_c::build(d2d::ResAccIf_c *resAcc, dTagProcessor_c *tagProcessor) {
    mpResAcc = resAcc;
    mLyt.setResAcc(resAcc);
    mLyt.build("messageWindow_00.brlyt", nullptr);
    mLyt.setPriority(0x8A);

    for (int i = 0; i < SIMPLE_WINDOW_TALK_NUM_ANIMS; i++) {
        mAnm[i].init(brlanMapTalk[i].mFile, mpResAcc, mLyt.getLayout(), brlanMapTalk[i].mName);
        mAnm[i].bind(false);
        mAnm[i].setAnimEnable(false);
    }

    mAnm[SIMPLE_WINDOW_TALK_ANIM_LOOP].setAnimEnable(true);

    mAnm[SIMPLE_WINDOW_TALK_ANIM_OUT].setAnimEnable(true);
    mAnm[SIMPLE_WINDOW_TALK_ANIM_OUT].setToEnd();
    mLyt.calc();
    mAnm[SIMPLE_WINDOW_TALK_ANIM_OUT].setAnimEnable(false);

    for (int i = 0; i < SIMPLE_WINDOW_TALK_NUM_PANES; i++) {
        mpPanes[i] = mLyt.findPane(sTalkPaneNames[i]);
    }

    for (int i = 0; i < SIMPLE_WINDOW_TALK_NUM_TEXTBOXES; i++) {
        mpTextboxes[i] = mLyt.getTextBox(sTalkTextboxNames[i]);
        mpTextboxes[i]->SetTextPositionH(0);
        mpTextboxes[i]->SetTextPositionV(0);
    }

    mpTextboxes[1]->setIsShadow(1);
    mpTagProcessor = tagProcessor;

    mLightText.build(mpResAcc, dLytTextLight::NORMAL);
    mBlurRelated.build();

    mpPanes[SIMPLE_WINDOW_TALK_PANE_IN_OUT]->SetVisible(true);
    mLyt.calc();
    mpPanes[SIMPLE_WINDOW_TALK_PANE_IN_OUT]->SetVisible(false);

    nw4r::math::MTX34 transform = mpPanes[SIMPLE_WINDOW_TALK_PANE_A_BTN]->GetGlobalMtx();
    mVec2_c _unused;
    _unused.x = transform._03;
    _unused.y = transform._13;

    mOrigPosition.copyFrom(mpPanes[SIMPLE_WINDOW_TALK_PANE_POSITION]->GetTranslate());
    mPosition.copyFrom(mpPanes[SIMPLE_WINDOW_TALK_PANE_POSITION]->GetTranslate());
    mOffset.x = mOffset.y = 0.0f;

    return true;
}

bool dLytSimpleWindowTalk_c::remove() {
    mLightText.remove();
    for (int i = 0; i < SIMPLE_WINDOW_TALK_NUM_ANIMS; i++) {
        mAnm[i].remove();
    }
    return true;
}

bool dLytSimpleWindowTalk_c::execute() {
    for (int i = 0; i < SIMPLE_WINDOW_TALK_NUM_ANIMS; i++) {
        if (mAnm[i].isEnabled()) {
            mAnm[i].play();
        }
    }
    mLyt.calc();
    return true;
}

bool dLytSimpleWindowTalk_c::draw() {
    bool bVis[3];
    for (int i = 0; i < 3; ++i) {
        bVis[i] = mpPanes[i]->IsVisible();
    };

    mpPanes[0]->SetVisible(bVis[0]);
    mpPanes[1]->SetVisible(false);
    mpPanes[2]->SetVisible(false);

    mLyt.draw();
    mBlurRelated.drawTextBlur(&mLightText, mOffset, mpPanes[3]->GetAlpha());

    mpPanes[0]->SetVisible(false);
    mpPanes[1]->SetVisible(bVis[1]);
    mpPanes[2]->SetVisible(bVis[2]);

    mLyt.draw();

    for (int i = 0; i < 3; ++i) {
        mpPanes[i]->SetVisible(bVis[i]);
    };
    return true;
}

void dLytSimpleWindowTalk_c::initializeIn(s32 pos) {
    mAnm[SIMPLE_WINDOW_TALK_ANIM_IN].setAnimEnable(true);
    mAnm[SIMPLE_WINDOW_TALK_ANIM_IN].setFrame(0.0f);

    mAnm[SIMPLE_WINDOW_TALK_ANIM_POSITION].setFrame(pos);
    mAnm[SIMPLE_WINDOW_TALK_ANIM_POSITION].setAnimEnable(true);
    mLyt.calc();
    mAnm[SIMPLE_WINDOW_TALK_ANIM_POSITION].setAnimEnable(false);

    mPosition = mOrigPosition;
    mPosition.y = mpPanes[SIMPLE_WINDOW_TALK_PANE_POSITION]->GetTranslate().y;
    mpPanes[SIMPLE_WINDOW_TALK_PANE_POSITION]->SetTranslate(mPosition);
}

bool dLytSimpleWindowTalk_c::isIn() {
    if (mAnm[SIMPLE_WINDOW_TALK_ANIM_IN].isEndReached()) {
        mAnm[SIMPLE_WINDOW_TALK_ANIM_IN].setAnimEnable(false);
        return true;
    }
    return false;
}

void dLytSimpleWindowTalk_c::initializeOutputText() {
    nw4r::lyt::Size size = getTextBox()->GetSize();
    nw4r::lyt::Size fontSize = getTextBox()->GetFontSize();
    nw4r::math::MTX34 transform = getTextBox()->GetGlobalMtx();

    mOffset.x = transform._03 - size.width / 2;
    mOffset.y = transform._13 + size.height / 2;
}

void dLytSimpleWindowTalk_c::initializeOut() {
    mAnm[SIMPLE_WINDOW_TALK_ANIM_OUT].setAnimEnable(true);
    mAnm[SIMPLE_WINDOW_TALK_ANIM_OUT].setFrame(0.0f);
}

bool dLytSimpleWindowTalk_c::isOut() {
    if (mAnm[SIMPLE_WINDOW_TALK_ANIM_OUT].isEndReached()) {
        mAnm[SIMPLE_WINDOW_TALK_ANIM_OUT].setAnimEnable(false);
        return true;
    }
    return false;
}

bool dLytSimpleWindowTalk_c::setText(const wchar_t *text) {
    mpTextboxes[0]->SetTagProcessor(mpTagProcessor);
    mpTextboxes[1]->SetTagProcessor(mpTagProcessor);
    mpTextboxes[0]->setTextWithTextProcessor(text, mpTagProcessor, nullptr);
    mpTextboxes[1]->setTextWithTextProcessor(text, mpTagProcessor, nullptr);
    return true;
}

static const d2d::LytBrlanMapping brlanMapSword[] = {
    {      "messageWindow_01_in.brlan",    "G_inOut_00"},
    {     "messageWindow_01_out.brlan",    "G_inOut_00"},
    {    "messageWindow_01_loop.brlan",     "G_loop_00"},
    {   "messageWindow_01_color.brlan",    "G_color_00"},
    {"messageWindow_01_position.brlan", "G_position_00"},
};

#define SIMPLE_WINDOW_SWORD_ANIM_IN 0
#define SIMPLE_WINDOW_SWORD_ANIM_OUT 1
#define SIMPLE_WINDOW_SWORD_ANIM_LOOP 2
#define SIMPLE_WINDOW_SWORD_ANIM_COLOR 3
#define SIMPLE_WINDOW_SWORD_ANIM_POSITION 4

#define SIMPLE_WINDOW_SWORD_NUM_ANIMS 5

static const char *sSwordPaneNames[] = {
    "N_aBtn_00",
    "N_inOut_00",
    "N_position_00",
};

#define SIMPLE_WINDOW_SWORD_PANE_A_BTN 0
#define SIMPLE_WINDOW_SWORD_PANE_IN_OUT 1
#define SIMPLE_WINDOW_SWORD_PANE_POSITION 2

#define SIMPLE_WINDOW_SWORD_NUM_PANES 3

static const char *sSwordTextboxNames[] = {
    "T_message_00",
    "T_messageS_00",
};

#define SIMPLE_WINDOW_SWORD_NUM_TEXTBOXES 2

bool dLytSimpleWindowSword_c::build(d2d::ResAccIf_c *resAcc, dTagProcessor_c *tagProcessor) {
    mpResAcc = resAcc;
    mLyt.setResAcc(resAcc);
    mLyt.build("messageWindow_01.brlyt", nullptr);
    mLyt.setPriority(0x8A);

    for (int i = 0; i < SIMPLE_WINDOW_SWORD_NUM_ANIMS; i++) {
        mAnm[i].init(brlanMapSword[i].mFile, mpResAcc, mLyt.getLayout(), brlanMapSword[i].mName);
        mAnm[i].bind(false);
        mAnm[i].setAnimEnable(false);
    }

    mAnm[SIMPLE_WINDOW_SWORD_ANIM_LOOP].setAnimEnable(true);

    mAnm[SIMPLE_WINDOW_SWORD_ANIM_OUT].setAnimEnable(true);
    mAnm[SIMPLE_WINDOW_SWORD_ANIM_OUT].setToEnd();
    mLyt.calc();
    mAnm[SIMPLE_WINDOW_SWORD_ANIM_OUT].setAnimEnable(false);

    for (int i = 0; i < SIMPLE_WINDOW_SWORD_NUM_PANES; i++) {
        mpPanes[i] = mLyt.findPane(sSwordPaneNames[i]);
    }

    for (int i = 0; i < SIMPLE_WINDOW_SWORD_NUM_TEXTBOXES; i++) {
        mpTextboxes[i] = mLyt.getTextBox(sSwordTextboxNames[i]);
        mpTextboxes[i]->SetTextPositionH(0);
        mpTextboxes[i]->SetTextPositionV(0);
    }

    mpTextboxes[1]->setIsShadow(1);
    mpTagProcessor = tagProcessor;

    for (int i = 0; i < TextWindowUnk::BUF_SIZE; i++) {
        mCharData[i].reset();
        mTextBuf[i] = L'\0';
    }

    mAnm[SIMPLE_WINDOW_SWORD_ANIM_COLOR].setAnimEnable(true);
    mAnm[SIMPLE_WINDOW_SWORD_ANIM_COLOR].setFrame(mSwordType);
    mpPanes[SIMPLE_WINDOW_SWORD_PANE_IN_OUT]->SetVisible(true);
    mLyt.calc();
    mpPanes[SIMPLE_WINDOW_SWORD_PANE_IN_OUT]->SetVisible(false);
    mAnm[SIMPLE_WINDOW_SWORD_ANIM_COLOR].setAnimEnable(false);

    nw4r::math::MTX34 transform = mpPanes[SIMPLE_WINDOW_SWORD_PANE_A_BTN]->GetGlobalMtx();
    mVec2_c _unused;
    _unused.x = transform._03;
    _unused.y = transform._13;

    mOrigPosition.copyFrom(mpPanes[SIMPLE_WINDOW_SWORD_PANE_POSITION]->GetTranslate());
    mPosition.copyFrom(mpPanes[SIMPLE_WINDOW_SWORD_PANE_POSITION]->GetTranslate());
    mOffset.x = mOffset.y = 0.0f;

    field_0x5AC8 = 0;

    return true;
}

bool dLytSimpleWindowSword_c::remove() {
    for (int i = 0; i < SIMPLE_WINDOW_SWORD_NUM_ANIMS; i++) {
        mAnm[i].remove();
    }
    return true;
}

bool dLytSimpleWindowSword_c::execute() {
    for (int i = 0; i < SIMPLE_WINDOW_SWORD_NUM_ANIMS; i++) {
        if (mAnm[i].isEnabled()) {
            mAnm[i].play();
        }
    }
    mLyt.calc();
    field_0x5AC8 = 0;
    return true;
}

bool dLytSimpleWindowSword_c::draw() {
    mLyt.draw();
    return true;
}

void dLytSimpleWindowSword_c::initializeIn(s32 pos) {
    mAnm[SIMPLE_WINDOW_SWORD_ANIM_IN].setAnimEnable(true);
    mAnm[SIMPLE_WINDOW_SWORD_ANIM_IN].setFrame(0.0f);

    mAnm[SIMPLE_WINDOW_SWORD_ANIM_POSITION].setFrame(pos);
    mAnm[SIMPLE_WINDOW_SWORD_ANIM_POSITION].setAnimEnable(true);
    mLyt.calc();
    mAnm[SIMPLE_WINDOW_SWORD_ANIM_POSITION].setAnimEnable(false);

    mPosition = mOrigPosition;
    mPosition.y = mpPanes[SIMPLE_WINDOW_SWORD_PANE_POSITION]->GetTranslate().y;
    mpPanes[SIMPLE_WINDOW_SWORD_PANE_POSITION]->SetTranslate(mPosition);
}

bool dLytSimpleWindowSword_c::isIn() {
    if (mAnm[SIMPLE_WINDOW_SWORD_ANIM_IN].isEndReached()) {
        mAnm[SIMPLE_WINDOW_SWORD_ANIM_IN].setAnimEnable(false);
        return true;
    }
    return false;
}

void dLytSimpleWindowSword_c::initializeOutputText() {
    nw4r::lyt::Size size = getTextBox()->GetSize();
    nw4r::lyt::Size fontSize = getTextBox()->GetFontSize();
    nw4r::math::MTX34 transform = getTextBox()->GetGlobalMtx();
    mOffset.x = transform._03 - size.width / 2;
    mOffset.y = transform._13 + size.height / 2;
}

void dLytSimpleWindowSword_c::initializeOut() {
    mAnm[SIMPLE_WINDOW_SWORD_ANIM_OUT].setAnimEnable(true);
    mAnm[SIMPLE_WINDOW_SWORD_ANIM_OUT].setFrame(0.0f);
}

bool dLytSimpleWindowSword_c::isOut() {
    if (mAnm[SIMPLE_WINDOW_SWORD_ANIM_OUT].isEndReached()) {
        mAnm[SIMPLE_WINDOW_SWORD_ANIM_OUT].setAnimEnable(false);
        return true;
    }
    return false;
}

bool dLytSimpleWindowSword_c::setText(const wchar_t *text) {
    int foundIdx = TextWindowUnk::BUF_SIZE;
    for (int i = 0; i < TextWindowUnk::BUF_SIZE; i++) {
        if (mCharData[i].displayTimerMaybe >= 0 && foundIdx > mCharData[i].field_0x0C) {
            foundIdx = mCharData[i].field_0x0C;
        }
    }

    for (int i = 0; i < TextWindowUnk::BUF_SIZE; i++) {
        if (i < foundIdx) {
            mTextBuf[i] = text[i];
        } else {
            mTextBuf[i] = L' ';
        }
    }

    mpTextboxes[0]->SetTagProcessor(mpTagProcessor);
    mpTextboxes[1]->SetTagProcessor(mpTagProcessor);
    mpTextboxes[0]->setTextWithTextProcessor(text, mpTagProcessor, nullptr);
    mpTextboxes[1]->setTextWithTextProcessor(text, mpTagProcessor, nullptr);
    return true;
}

static const d2d::LytBrlanMapping brlanMapLink[] = {
    {      "linkWindow_00_in.brlan",    "G_inOut_00"},
    {     "linkWindow_00_out.brlan",    "G_inOut_00"},
    {"linkWindow_00_position.brlan", "G_position_00"},
    {    "linkWindow_00_loop.brlan",     "G_loop_00"},
};

#define SIMPLE_WINDOW_LINK_ANIM_IN 0
#define SIMPLE_WINDOW_LINK_ANIM_OUT 1
#define SIMPLE_WINDOW_LINK_ANIM_POSITION 2
#define SIMPLE_WINDOW_LINK_ANIM_LOOP 3

#define SIMPLE_WINDOW_LINK_NUM_ANIMS 4

static const char *sLinkPaneNames[] = {
    "N_window_00", "N_message_00", "N_aBtn_00", "N_inOut_00", "N_position_00",
};

#define SIMPLE_WINDOW_LINK_PANE_WINDOW 0
#define SIMPLE_WINDOW_LINK_PANE_MESSAGE 1
#define SIMPLE_WINDOW_LINK_PANE_A_BTN 2
#define SIMPLE_WINDOW_LINK_PANE_IN_OUT 3
#define SIMPLE_WINDOW_LINK_PANE_POSITION 4

#define SIMPLE_WINDOW_LINK_NUM_PANES 5

static const char *sLinkTextboxNames[] = {
    "T_message_00",
    "T_messageS_00",
};

#define SIMPLE_WINDOW_LINK_NUM_TEXTBOXES 2

bool dLytSimpleWindowLink_c::build(d2d::ResAccIf_c *resAcc, dTagProcessor_c *tagProcessor) {
    mpResAcc = resAcc;
    mLyt.setResAcc(resAcc);
    mLyt.build("linkWindow_00.brlyt", nullptr);
    mLyt.setPriority(0x8A);

    for (int i = 0; i < SIMPLE_WINDOW_LINK_NUM_ANIMS; i++) {
        mAnm[i].init(brlanMapLink[i].mFile, mpResAcc, mLyt.getLayout(), brlanMapLink[i].mName);
        mAnm[i].bind(false);
        mAnm[i].setAnimEnable(false);
    }

    mAnm[SIMPLE_WINDOW_LINK_ANIM_LOOP].setAnimEnable(true);

    mAnm[SIMPLE_WINDOW_LINK_ANIM_OUT].setAnimEnable(true);
    mAnm[SIMPLE_WINDOW_LINK_ANIM_OUT].setToEnd();
    mLyt.calc();
    mAnm[SIMPLE_WINDOW_LINK_ANIM_OUT].setAnimEnable(false);

    for (int i = 0; i < SIMPLE_WINDOW_LINK_NUM_PANES; i++) {
        mpPanes[i] = mLyt.findPane(sLinkPaneNames[i]);
    }

    for (int i = 0; i < SIMPLE_WINDOW_LINK_NUM_TEXTBOXES; i++) {
        mpTextboxes[i] = mLyt.getTextBox(sLinkTextboxNames[i]);
        mpTextboxes[i]->SetTextPositionH(0);
        mpTextboxes[i]->SetTextPositionV(0);
    }

    mpTextboxes[1]->setIsShadow(1);
    mpTagProcessor = tagProcessor;

    mLightText.build(mpResAcc, dLytTextLight::NORMAL);
    mBlurRelated.build();

    mpPanes[SIMPLE_WINDOW_LINK_PANE_IN_OUT]->SetVisible(true);
    mLyt.calc();
    mpPanes[SIMPLE_WINDOW_LINK_PANE_IN_OUT]->SetVisible(false);

    nw4r::math::MTX34 transform = mpPanes[SIMPLE_WINDOW_LINK_PANE_A_BTN]->GetGlobalMtx();
    mVec2_c _unused;
    _unused.x = transform._03;
    _unused.y = transform._13;

    mOrigPosition.copyFrom(mpPanes[SIMPLE_WINDOW_LINK_PANE_POSITION]->GetTranslate());
    mPosition.copyFrom(mpPanes[SIMPLE_WINDOW_LINK_PANE_POSITION]->GetTranslate());
    mOffset.x = mOffset.y = 0.0f;

    return true;
}

bool dLytSimpleWindowLink_c::remove() {
    mLightText.remove();
    for (int i = 0; i < SIMPLE_WINDOW_LINK_NUM_ANIMS; i++) {
        mAnm[i].remove();
    }
    return true;
}

bool dLytSimpleWindowLink_c::execute() {
    for (int i = 0; i < SIMPLE_WINDOW_LINK_NUM_ANIMS; i++) {
        if (mAnm[i].isEnabled()) {
            mAnm[i].play();
        }
    }
    mLyt.calc();
    return true;
}

bool dLytSimpleWindowLink_c::draw() {
    bool bVis[3];
    for (int i = 0; i < 3; ++i) {
        bVis[i] = mpPanes[i]->IsVisible();
    };

    mpPanes[0]->SetVisible(bVis[0]);
    mpPanes[1]->SetVisible(false);
    mpPanes[2]->SetVisible(false);

    mLyt.draw();
    mBlurRelated.drawTextBlur(&mLightText, mOffset, mpPanes[3]->GetAlpha());

    mpPanes[0]->SetVisible(false);
    mpPanes[1]->SetVisible(bVis[1]);
    mpPanes[2]->SetVisible(bVis[2]);

    mLyt.draw();

    for (int i = 0; i < 3; ++i) {
        mpPanes[i]->SetVisible(bVis[i]);
    };
    return true;
}

void dLytSimpleWindowLink_c::initializeIn(s32 pos) {
    mAnm[SIMPLE_WINDOW_LINK_ANIM_IN].setAnimEnable(true);
    mAnm[SIMPLE_WINDOW_LINK_ANIM_IN].setFrame(0.0f);

    mAnm[SIMPLE_WINDOW_LINK_ANIM_POSITION].setFrame(pos);
    mAnm[SIMPLE_WINDOW_LINK_ANIM_POSITION].setAnimEnable(true);
    mLyt.calc();
    mAnm[SIMPLE_WINDOW_LINK_ANIM_POSITION].setAnimEnable(false);

    mPosition = mOrigPosition;
    mPosition.y = mpPanes[SIMPLE_WINDOW_LINK_PANE_POSITION]->GetTranslate().y;
    mpPanes[SIMPLE_WINDOW_LINK_PANE_POSITION]->SetTranslate(mPosition);
}

bool dLytSimpleWindowLink_c::isIn() {
    if (mAnm[SIMPLE_WINDOW_LINK_ANIM_IN].isEndReached()) {
        mAnm[SIMPLE_WINDOW_LINK_ANIM_IN].setAnimEnable(false);
        return true;
    }
    return false;
}

void dLytSimpleWindowLink_c::initializeOutputText() {
    nw4r::lyt::Size size = getTextBox()->GetSize();
    nw4r::lyt::Size fontSize = getTextBox()->GetFontSize();
    nw4r::math::MTX34 transform = getTextBox()->GetGlobalMtx();

    mOffset.x = transform._03 - size.width / 2;
    mOffset.y = transform._13 + size.height / 2;
}

void dLytSimpleWindowLink_c::initializeOut() {
    mAnm[SIMPLE_WINDOW_LINK_ANIM_OUT].setAnimEnable(true);
    mAnm[SIMPLE_WINDOW_LINK_ANIM_OUT].setFrame(0.0f);
}

bool dLytSimpleWindowLink_c::isOut() {
    if (mAnm[SIMPLE_WINDOW_LINK_ANIM_OUT].isEndReached()) {
        mAnm[SIMPLE_WINDOW_LINK_ANIM_OUT].setAnimEnable(false);
        return true;
    }
    return false;
}

bool dLytSimpleWindowLink_c::setText(const wchar_t *text) {
    mpTextboxes[0]->SetTagProcessor(mpTagProcessor);
    mpTextboxes[1]->SetTagProcessor(mpTagProcessor);
    mpTextboxes[0]->setTextWithTextProcessor(text, mpTagProcessor, nullptr);
    mpTextboxes[1]->setTextWithTextProcessor(text, mpTagProcessor, nullptr);
    return true;
}

bool dLytSimpleWindow_c::build() {
    void *data = LayoutArcManager::GetInstance()->getLoadedData("System2D");
    mResAcc.attach(data, "");
    mpTagProcessor = new dTagProcessor_c();
    mpUnk = new TextWindowUnk(mpTagProcessor);

    mInRequest = false;
    field_0x3D4 = 0;
    field_0x3D8 = 0;
    field_0x3DC = 0;
    field_0x3E0 = 0;
    field_0x3E8 = -1;
    mLabel = "";
    mTimer = 0;
    mWaitForEvent = false;
    mpCurrentWindow = nullptr;
    mpWindowTalk = nullptr;
    mpWindowSword = nullptr;
    mpWindowLink = nullptr;

    mStateMgr.changeState(StateID_Invisible);

    return true;
}

bool dLytSimpleWindow_c::remove() {
    if (mpWindowTalk != nullptr) {
        delete mpWindowTalk;
        mpWindowTalk = nullptr;
    }
    if (mpWindowSword != nullptr) {
        delete mpWindowSword;
        mpWindowSword = nullptr;
    }
    if (mpWindowLink != nullptr) {
        delete mpWindowLink;
        mpWindowLink = nullptr;
    }

    mpCurrentWindow = nullptr;
    delete mpUnk;
    mpUnk = nullptr;
    delete mpTagProcessor;
    mpTagProcessor = nullptr;

    mResAcc.detach();
    return true;
}

void dLytSimpleWindow_c::initializeState_Invisible() {}
void dLytSimpleWindow_c::executeState_Invisible() {
    if (!mInRequest) {
        return;
    }
    mInRequest = false;
    if (!dMessage_c::isValidTextLabel(mLabel)) {
        return;
    }

    const wchar_t *message = dMessage_c::getTextMessageByLabel(mLabel, mpTagProcessor, true, nullptr, 0);
    if (mpTagProcessor->getMsgWindowSubtype() == dLytMsgWindow_c::MSG_WINDOW_SWORD_FI ||
        mpTagProcessor->getMsgWindowSubtype() == dLytMsgWindow_c::MSG_WINDOW_SWORD_GHIRAHIM) {
        mpWindowSword = new dLytSimpleWindowSword_c();

        if (mpTagProcessor->getMsgWindowSubtype() == dLytMsgWindow_c::MSG_WINDOW_SWORD_GHIRAHIM) {
            mpWindowSword->setSwordType(dLytTextSword::GHIRAHIM);
        } else {
            mpWindowSword->setSwordType(dLytTextSword::FI);
        }

        mpWindowSword->build(&mResAcc, mpTagProcessor);
        mpCurrentWindow = mpWindowSword;
    } else if (mpTagProcessor->getMsgWindowSubtype() == dLytMsgWindow_c::MSG_WINDOW_LINK) {
        mpWindowLink = new dLytSimpleWindowLink_c();
        mpWindowLink->build(&mResAcc, mpTagProcessor);
        mpCurrentWindow = mpWindowLink;
    } else {
        mpWindowTalk = new dLytSimpleWindowTalk_c();
        mpWindowTalk->build(&mResAcc, mpTagProcessor);
        mpCurrentWindow = mpWindowTalk;
    }

    field_0x42A = mLabel;

    mpUnk->fn_800B2130(field_0x42A, mpCurrentWindow->getTextBox(), mpCurrentWindow->getCharData(), true);
    mpCurrentWindow->setText(mpUnk->getProcessedText());
    mStateMgr.changeState(StateID_In);
}
void dLytSimpleWindow_c::finalizeState_Invisible() {}

void dLytSimpleWindow_c::initializeState_In() {
    s32 pos;
    if (mpTagProcessor->getField_0x90D() == 1) {
        pos = 0;
    } else if (mpTagProcessor->getField_0x90D() == 3) {
        pos = 1;
    } else {
        pos = 2;
    }

    mpCurrentWindow->initializeIn(pos);
    dLyt_HIO_c::getInstance()->setStr2("");
}
void dLytSimpleWindow_c::executeState_In() {
    if (mpCurrentWindow->isIn()) {
        mpUnk->textAdvancingRelated(true, true);
        mpCurrentWindow->setText(mpUnk->getProcessedText());
        mStateMgr.changeState(StateID_OutputText);
    }
}
void dLytSimpleWindow_c::finalizeState_In() {}

void dLytSimpleWindow_c::initializeState_OutputText() {
    mpCurrentWindow->initializeOutputText();
}
void dLytSimpleWindow_c::executeState_OutputText() {
    if (mTimer > 0) {
        mTimer--;
    }

    if (isInEvent() || !dLytControlGame_c::getInstance()->isStateNormal() || dLytMeter_c::getItemSelect0x75A2() ||
        dLytMeter_c::getMinusBtnFn800F7600() || dLytMeter_c::getDowsingFn800FE4B0()) {
        mTimer = 0;
        mWaitForEvent = false;
    }

    if (!mWaitForEvent && mTimer <= 0) {
        mStateMgr.changeState(StateID_Out);
    }
}
void dLytSimpleWindow_c::finalizeState_OutputText() {}

void dLytSimpleWindow_c::initializeState_Out() {
    mpCurrentWindow->initializeOut();
    dLyt_HIO_c::getInstance()->setStr2("");
    dLyt_HIO_c::getInstance()->setStr1("");
    dLyt_HIO_c::getInstance()->setStr3("");
}
void dLytSimpleWindow_c::executeState_Out() {
    if (mpCurrentWindow->isOut()) {
        mLabel = "";
        if (mpWindowTalk != nullptr) {
            delete mpWindowTalk;
            mpWindowTalk = nullptr;
        }
        if (mpWindowSword != nullptr) {
            delete mpWindowSword;
            mpWindowSword = nullptr;
        }
        if (mpWindowLink != nullptr) {
            delete mpWindowLink;
            mpWindowLink = nullptr;
        }

        mpCurrentWindow = nullptr;
        mStateMgr.changeState(StateID_Invisible);
    }
}
void dLytSimpleWindow_c::finalizeState_Out() {}

bool dLytSimpleWindow_c::execute() {
    mStateMgr.executeState();
    if (isVisible() && mpCurrentWindow != nullptr) {
        mpCurrentWindow->execute();
    }
    return true;
}

void dLytSimpleWindow_c::draw() {
    if (isVisible() && mpCurrentWindow != nullptr) {
        mpTagProcessor->resetSomeThings();
        mpCurrentWindow->draw();
    }
}

bool dLytSimpleWindow_c::set(const char *label, s32 timer, bool global) {
    if (!isVisible()) {
        if (global) {
            s32 idx = dMessage_c::getMsbtIndexForLabel(label);
            if (idx < 0) {
                return false;
            }
            dMessage_c::getInstance()->setCurrentTextFileNumber(idx);
        }
        mInRequest = true;
        if (timer >= 0) {
            mTimer = timer;
            mWaitForEvent = false;
        } else {
            // This is a bit weird, the timer will still be running
            // here and it'll be checked even when the event has happened
            // Probably not a problem in practice though
            mWaitForEvent = true;
        }
        mLabel = label;
        return true;
    } else {
        return false;
    }
}

bool dLytSimpleWindow_c::isVisible() {
    return !(*mStateMgr.getStateID() == StateID_Invisible);
}

bool dLytSimpleWindow_c::isOutputText() {
    return (*mStateMgr.getStateID() == StateID_OutputText);
}

bool dLytSimpleWindow_c::isInEvent() {
    if (EventManager::isInEvent()) {
        if (EventManager::getCurrentEventName() != nullptr &&
            strequals(EventManager::getCurrentEventName(), "DefaultStart")) {
            return false;
        }
        return true;
    }
    return false;
}
