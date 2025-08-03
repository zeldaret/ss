#include "d/lyt/d_lyt_common_title.h"

#include "common.h"
#include "d/d_message.h"
#include "d/lyt/d2d.h"
#include "d/lyt/d_lyt_control_game.h"
#include "d/lyt/d_textbox.h"
#include "d/lyt/d_window.h"
#include "toBeSorted/arc_managers/layout_arc_manager.h"

#include <cstring>

STATE_DEFINE(dLytCommonTitle_c, None);
STATE_DEFINE(dLytCommonTitle_c, In);
STATE_DEFINE(dLytCommonTitle_c, Wait);
STATE_DEFINE(dLytCommonTitle_c, Out);
STATE_DEFINE(dLytCommonTitle_c, Change);

static const d2d::LytBrlanMapping brlanMap[] = {
    {         "commonTitle_00_in.brlan",      "G_inOut_00"},
    {        "commonTitle_00_out.brlan",      "G_inOut_00"},
    {       "commonTitle_00_loop.brlan",       "G_loop_00"},
    {"commonTitle_00_titleChange.brlan", "G_titleInOut_00"},
    {     "commonTitle_00_textIn.brlan",  "G_textInOut_00"},
    {    "commonTitle_00_textOut.brlan",  "G_textInOut_00"},
    { "commonTitle_00_textChange.brlan",  "G_textInOut_00"},
    {      "commonTitle_00_input.brlan",      "G_input_00"},
};

#define COMMON_TITLE_ANIM_IN 0
#define COMMON_TITLE_ANIM_OUT 1
#define COMMON_TITLE_ANIM_LOOP 2
#define COMMON_TITLE_ANIM_TITLE_CHANGE 3
#define COMMON_TITLE_ANIM_TEXT_IN 4
#define COMMON_TITLE_ANIM_TEXT_OUT 5
#define COMMON_TITLE_ANIM_TEXT_CHANGE 6
#define COMMON_TITLE_ANIM_INPUT 7

#define COMMON_TITLE_NUM_ANIMS 8

dLytCommonTitle_c::dLytCommonTitle_c() : mStateMgr(*this) {}

bool dLytCommonTitle_c::build() {
    void *data = LayoutArcManager::GetInstance()->getLoadedData("DoButton");
    mResAcc.attach(data, "");
    mLyt.build("commonTitle_00.brlyt", &mResAcc);
    mLyt.setPriority(0x86);

    for (int i = 0; i < COMMON_TITLE_NUM_ANIMS; i++) {
        mAnm[i].init(brlanMap[i].mFile, &mResAcc, mLyt.getLayout(), brlanMap[i].mName);
    }

    mStateMgr.changeState(StateID_None);
    field_0x680 = 0;
    field_0x690 = true;
    return true;
}

bool dLytCommonTitle_c::remove() {
    mLyt.unbindAnims();
    for (int i = 0; i < COMMON_TITLE_NUM_ANIMS; i++) {
        mAnm[i].remove();
    }
    mResAcc.detach();
    return true;
}

bool dLytCommonTitle_c::execute() {
    mStateMgr.executeState();
    if (!(mStateMgr.getStateID()->isEqual(StateID_None))) {
        mAnm[COMMON_TITLE_ANIM_LOOP].play();
        if (field_0x690 == true) {
            playBackwards(mAnm[COMMON_TITLE_ANIM_INPUT]);
        } else {
            mAnm[COMMON_TITLE_ANIM_INPUT].play();
        }
    }
    mLyt.calc();
    return true;
}

bool dLytCommonTitle_c::draw() {
    if (mVisible == true) {
        mLyt.addToDrawList();
    }
    return true;
}

bool dLytCommonTitle_c::set(s32 arg, const char *title, const char *caption) {
    if (!(mStateMgr.getStateID()->isEqual(StateID_Wait)) && !(mStateMgr.getStateID()->isEqual(StateID_None))) {
        return false;
    }

    field_0x684 = arg;

    if (title != nullptr) {
        mTitle = title;
    } else {
        mTitle.empty();
    }

    if (caption != nullptr) {
        mCaption = caption;
    } else {
        mCaption.empty();
    }

    mChangeRequest = true;
    return true;
}

const char *dLytCommonTitle_c::setSubTitle(s32 msgIdx) {
    const char *ret = nullptr;
    mCaption.empty();
    if (msgIdx != 0 && msgIdx <= 2) {
        // "Gear", "Collection"
        mCaption.sprintf("SUB_TITLE_%02d", msgIdx);
        ret = mCaption;
    }
    return ret;
}

const char *dLytCommonTitle_c::setSysTitle(s32 msgIdx) {
    const char *ret = nullptr;
    mTitle.empty();
    if (msgIdx != 0 && msgIdx <= 2) {
        // "Quest Logs", "Your Name"
        mTitle.sprintf("SYS_TITLE_%02d", msgIdx);
        ret = mTitle;
    }
    return ret;
}

const char *dLytCommonTitle_c::setSysCaption(s32 msgIdx) {
    const char *ret = nullptr;
    mCaption.empty();
    if (msgIdx != 0 && msgIdx <= 5) {
        // "Select a file.", ...
        mCaption.sprintf("SYS_CAPTION_%02d", msgIdx);
        ret = mCaption;
    }
    return ret;
}

void dLytCommonTitle_c::gotoStateNone() {
    mStateMgr.changeState(StateID_None);
}

void dLytCommonTitle_c::initializeState_None() {
    mLyt.unbindAnims();
    mChangeRequest = false;
    field_0x68D = false;
    mVisible = false;
    field_0x684 = 0;
    mStep = 0;
    mTitle.empty();
    mCaption.empty();
    field_0x68F = 0;
    if (dLytControlGame_c::getInstance()->getField_0x15C67()) {
        mLyt.findPane("N_next_01")->SetVisible(false);
    } else {
        mLyt.findPane("N_next_01")->SetVisible(true);
    }
}
void dLytCommonTitle_c::executeState_None() {
    if (mChangeRequest == true) {
        mStateMgr.changeState(StateID_In);
    }
}
void dLytCommonTitle_c::finalizeState_None() {
    mChangeRequest = false;
    startAnim(COMMON_TITLE_ANIM_LOOP);
    startAnim(COMMON_TITLE_ANIM_INPUT);
}

void dLytCommonTitle_c::initializeState_In() {
    mStep = 0;
    startAnim(COMMON_TITLE_ANIM_IN);
    startAnim(COMMON_TITLE_ANIM_TITLE_CHANGE);
    if (field_0x680 == 0) {
        mLyt.setPriority(0x80);
        startAnim(COMMON_TITLE_ANIM_TEXT_IN);
    } else {
        startAnim(COMMON_TITLE_ANIM_TEXT_CHANGE);
        getAnm(COMMON_TITLE_ANIM_TEXT_CHANGE).setToEnd();
        startAnim(COMMON_TITLE_ANIM_TEXT_OUT);
        getAnm(COMMON_TITLE_ANIM_TEXT_OUT).setToEnd();
    }
    mVisible = true;
    applyText();
}
void dLytCommonTitle_c::executeState_In() {
    switch (mStep) {
        case 0: {
            d2d::AnmGroup_c &anm = mAnm[COMMON_TITLE_ANIM_IN];
            if (anm.isEndReached() == true) {
                if (field_0x680 == 0) {
                    mStep = 1;
                } else {
                    mStep = 10;
                    field_0x68D = true;
                }
            } else {
                anm.play();
            }
            break;
        }
        case 1: {
            d2d::AnmGroup_c &anm = mAnm[COMMON_TITLE_ANIM_TEXT_IN];
            if (anm.isEndReached() == true) {
                mStep = 10;
                field_0x68D = true;
            }
            anm.play();
            break;
        }
        case 10: {
            field_0x68D = false;
            mStateMgr.changeState(StateID_Wait);
            break;
        }
    }
}
void dLytCommonTitle_c::finalizeState_In() {
    stopAnim(COMMON_TITLE_ANIM_TITLE_CHANGE);
    stopAnim(COMMON_TITLE_ANIM_IN);
    if (field_0x680 == 0) {
        stopAnim(COMMON_TITLE_ANIM_TEXT_IN);
    } else {
        stopAnim(COMMON_TITLE_ANIM_TEXT_CHANGE);
    }
}

void dLytCommonTitle_c::initializeState_Wait() {
    mStep = 0;
    field_0x68F = false;
}
void dLytCommonTitle_c::executeState_Wait() {
    if (field_0x680 == 1) {
        if (std::strlen(mCaption) != 0) {
            field_0x68F = true;
            mAnm[COMMON_TITLE_ANIM_TEXT_OUT].setFrame(0.0f);
        } else {
            mAnm[COMMON_TITLE_ANIM_TEXT_OUT].play();
            if (field_0x68F == true && mAnm[COMMON_TITLE_ANIM_TEXT_OUT].isEndReached() == true) {
                applyCaption(0);
                field_0x68F = false;
            }
        }
    }

    if (mChangeRequest == true) {
        if (field_0x684 == 3) {
            mStateMgr.changeState(StateID_Out);
        } else if (field_0x680 == 0) {
            mStateMgr.changeState(StateID_Change);
        } else if (field_0x684 == 1) {
            mStateMgr.changeState(StateID_Change);
        } else {
            mChangeRequest = false;
            if (std::strlen(mCaption) != 0) {
                applyCaption(0);
            }
        }
    }
}
void dLytCommonTitle_c::finalizeState_Wait() {
    mChangeRequest = false;
}

void dLytCommonTitle_c::initializeState_Out() {
    mStep = 0;
    startAnim(COMMON_TITLE_ANIM_OUT);
}
void dLytCommonTitle_c::executeState_Out() {
    d2d::AnmGroup_c &anm = mAnm[COMMON_TITLE_ANIM_OUT];
    switch (mStep) {
        case 0: {
            if (anm.isEndReached() == true) {
                mStep = 1;
                field_0x68D = true;
            }
            break;
        }
        case 1: {
            mStateMgr.changeState(StateID_None);
            return;
        }
    }
    anm.play();
}
void dLytCommonTitle_c::finalizeState_Out() {}

void dLytCommonTitle_c::initializeState_Change() {
    applyTextChange(0);
    if (field_0x684 == 1) {
        startAnim(COMMON_TITLE_ANIM_TITLE_CHANGE);
    }

    if (field_0x680 == 0) {
        mStep = 0;
        startAnim(COMMON_TITLE_ANIM_TEXT_CHANGE);
    } else {
        mStep = 10;
    }
}
void dLytCommonTitle_c::executeState_Change() {
    switch (mStep) {
        case 0: {
            d2d::AnmGroup_c &anm = mAnm[COMMON_TITLE_ANIM_TEXT_CHANGE];
            if (anm.isEndReached() == true) {
                mStep++;
            }
            anm.play();
            if (field_0x684 == 1) {
                getAnm(COMMON_TITLE_ANIM_TITLE_CHANGE).play();
            }
            break;
        }
        case 1: {
            applyTextChange(1);
            getAnm(COMMON_TITLE_ANIM_TEXT_CHANGE).setFrame(0.0f);
            if (field_0x684 == 1) {
                getAnm(COMMON_TITLE_ANIM_TITLE_CHANGE).setFrame(0.0f);
            }
            field_0x68D = true;
            mStep++;
            // fall-through
        }
        case 2: {
            mStateMgr.changeState(StateID_Wait);
            break;
        }
        case 10: {
            d2d::AnmGroup_c &anm = mAnm[COMMON_TITLE_ANIM_TITLE_CHANGE];
            if (anm.isEndReached() == true) {
                mStep++;
            }
            anm.play();
            break;
        }
        case 11: {
            applyTextChange(1);
            mAnm[COMMON_TITLE_ANIM_TITLE_CHANGE].setFrame(0.0f);
            mAnm[COMMON_TITLE_ANIM_TEXT_OUT].setToEnd();
            mStep = 2;
            field_0x68D = true;
            break;
        }
    }
}
void dLytCommonTitle_c::finalizeState_Change() {
    stopAnim(COMMON_TITLE_ANIM_TEXT_CHANGE);
    if (field_0x684 == 1) {
        stopAnim(COMMON_TITLE_ANIM_TITLE_CHANGE);
    }
}

void dLytCommonTitle_c::startAnim(int idx) {
    d2d::AnmGroup_c &anm = mAnm[idx];
    anm.bind(false);
    anm.setFrame(0.0f);
}

void dLytCommonTitle_c::stopAnim(int idx) {
    d2d::AnmGroup_c &anm = mAnm[idx];
    anm.unbind();
}

void dLytCommonTitle_c::playBackwards(d2d::AnmGroup_c &anm) {
    f32 frame = anm.getFrame();
    if (frame) {
        frame -= 1.0f;
        if (frame <= 0.0f) {
            frame = 0.0f;
        }
        anm.setFrame(frame);
    }
}

void dLytCommonTitle_c::applyText() {
    if (field_0x684 == 2) {
        applyCaption(0);
    } else {
        applyTitle(0);
        applyCaption(0);
    }
}

void dLytCommonTitle_c::applyTextChange(s32 arg) {
    switch (arg) {
        case 0: {
            if (field_0x684 == 2) {
                applyCaption(1);
            } else {
                applyTitle(1);
                applyCaption(1);
            }
            break;
        }
        case 1: {
            if (field_0x684 == 2) {
                applyCaption(2);
            } else {
                applyTitle(2);
                applyCaption(2);
            }
            break;
        }
    }
}

void dLytCommonTitle_c::applyTitle(s32 arg) {
    dTextBox_c *mainTitle0 = mLyt.getTextBox("T_mainTitle_00");
    dTextBox_c *mainTitleS0 = mLyt.getTextBox("T_mainTitleS_00");
    dTextBox_c *mainTitle1 = mLyt.getTextBox("T_mainTitle_01");
    dTextBox_c *mainTitleS1 = mLyt.getTextBox("T_mainTitleS_01");

    switch (arg) {
        case 0: {
            if (std::strlen(mTitle) != 0) {
                const wchar_t *text = dMessage_c::getTextMessageByLabel(mTitle, true, nullptr, 0);
                mainTitle0->setTextWithGlobalTextProcessor(text);
                mainTitleS0->setTextWithGlobalTextProcessor(text);
                mainTitle1->setTextWithGlobalTextProcessor(text);
                mainTitleS1->setTextWithGlobalTextProcessor(text);
            } else {
                mainTitle0->setTextWithGlobalTextProcessor(L"");
                mainTitleS0->setTextWithGlobalTextProcessor(L"");
                mainTitle1->setTextWithGlobalTextProcessor(L"");
                mainTitleS1->setTextWithGlobalTextProcessor(L"");
            }
            // @bug missing break, ends up doing duplicate work
            // break;
        }
        case 1: {
            if (std::strlen(mTitle) != 0) {
                const wchar_t *text = dMessage_c::getTextMessageByLabel(mTitle, true, nullptr, 0);
                mainTitle0->setTextWithGlobalTextProcessor(text);
                mainTitleS0->setTextWithGlobalTextProcessor(text);
            } else {
                mainTitle0->setTextWithGlobalTextProcessor(L"");
                mainTitleS0->setTextWithGlobalTextProcessor(L"");
            }
            break;
        }
        case 2: {
            if (std::strlen(mTitle) != 0) {
                const wchar_t *text = dMessage_c::getTextMessageByLabel(mTitle, true, nullptr, 0);
                mainTitle1->setTextWithGlobalTextProcessor(text);
                mainTitleS1->setTextWithGlobalTextProcessor(text);
            } else {
                mainTitle1->setTextWithGlobalTextProcessor(L"");
                mainTitleS1->setTextWithGlobalTextProcessor(L"");
            }
            break;
        }
    }
}

void dLytCommonTitle_c::applyCaption(s32 arg) {
    dTextBox_c *mainTitle0 = mLyt.getTextBox("T_changeText_00");
    dTextBox_c *mainTitleS0 = mLyt.getTextBox("T_changeTextS_00");
    dTextBox_c *mainTitle1 = mLyt.getTextBox("T_changeText_01");
    dTextBox_c *mainTitleS1 = mLyt.getTextBox("T_changeTextS_01");
    dWindow_c *window0 = mLyt.getWindow("W_bgP_00");
    dWindow_c *window1 = mLyt.getWindow("W_bgP_01");

    switch (arg) {
        case 0: {
            if (std::strlen(mCaption) != 0) {
                const wchar_t *text = dMessage_c::getTextMessageByLabel(mCaption, true, nullptr, 0);
                mainTitle0->setTextWithGlobalTextProcessor(text);
                mainTitleS0->setTextWithGlobalTextProcessor(text);
                mainTitle1->setTextWithGlobalTextProcessor(text);
                mainTitleS1->setTextWithGlobalTextProcessor(text);
                if (field_0x680 == 0) {
                    window0->SetVisible(false);
                    window1->SetVisible(true);
                } else {
                    window0->SetVisible(true);
                    window1->SetVisible(false);
                }
            } else {
                mainTitle0->setTextWithGlobalTextProcessor(L"");
                mainTitleS0->setTextWithGlobalTextProcessor(L"");
                mainTitle1->setTextWithGlobalTextProcessor(L"");
                mainTitleS1->setTextWithGlobalTextProcessor(L"");
                window0->SetVisible(false);
                window1->SetVisible(false);
            }

            break;
        }
        case 1: {
            if (std::strlen(mCaption) != 0) {
                const wchar_t *text = dMessage_c::getTextMessageByLabel(mCaption, true, nullptr, 0);
                mainTitle1->setTextWithGlobalTextProcessor(text);
                mainTitleS1->setTextWithGlobalTextProcessor(text);
                window0->SetVisible(true);
            } else {
                mainTitle1->setTextWithGlobalTextProcessor(L"");
                mainTitleS1->setTextWithGlobalTextProcessor(L"");
                window0->SetVisible(false);
            }
            break;
        }
        case 2: {
            if (std::strlen(mCaption) != 0) {
                window1->SetVisible(true);
                const wchar_t *text = dMessage_c::getTextMessageByLabel(mCaption, true, nullptr, 0);
                mainTitle0->setTextWithGlobalTextProcessor(text);
                mainTitleS0->setTextWithGlobalTextProcessor(text);
            } else {
                window1->SetVisible(false);
                mainTitle0->setTextWithGlobalTextProcessor(L"");
                mainTitleS0->setTextWithGlobalTextProcessor(L"");
            }
            break;
        }
    }

    window0->UpdateSize(mLyt.getSizeBoxInWindow("W_bgP_00"), 32.0f);
    window1->UpdateSize(mLyt.getSizeBoxInWindow("W_bgP_01"), 32.0f);
}
