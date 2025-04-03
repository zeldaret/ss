#include "d/lyt/msg_window/d_lyt_msg_window.h"

#include "common.h"
#include "d/d_message.h"
#include "d/d_tag_processor.h"
#include "d/d_textunk.h"
#include "d/d_textwindow_unk.h"
#include "d/lyt/d_textbox.h"
// clang-format off
// vtable order
#include "d/lyt/msg_window/d_lyt_msg_window_wood.h"
#include "d/lyt/msg_window/d_lyt_msg_window_stone.h"
#include "d/lyt/d_lyt_auto_explain.h"
#include "d/lyt/d_lyt_auto_caption.h"
// clang-format on
#include "s/s_State.hpp"
#include "toBeSorted/arc_managers/layout_arc_manager.h"
#include "toBeSorted/global_fi_context.h"

STATE_DEFINE(dLytMsgWindow_c, Invisible);
STATE_DEFINE(dLytMsgWindow_c, In);
STATE_DEFINE(dLytMsgWindow_c, OutputText);
STATE_DEFINE(dLytMsgWindow_c, WaitKeyChangePage0);
STATE_DEFINE(dLytMsgWindow_c, WaitKeyChangePage1);
STATE_DEFINE(dLytMsgWindow_c, WaitKeyMsgEnd0);
STATE_DEFINE(dLytMsgWindow_c, WaitKeyMsgEnd1);
STATE_DEFINE(dLytMsgWindow_c, WaitKeyMsgEnd2);
STATE_DEFINE(dLytMsgWindow_c, WaitKeySelectQuestion);
STATE_DEFINE(dLytMsgWindow_c, MapOpen);
STATE_DEFINE(dLytMsgWindow_c, WaitKeyMapClose);
STATE_DEFINE(dLytMsgWindow_c, MapClose);
STATE_DEFINE(dLytMsgWindow_c, Out);
STATE_DEFINE(dLytMsgWindow_c, ExplainIn);
STATE_DEFINE(dLytMsgWindow_c, ExplainVisible);
STATE_DEFINE(dLytMsgWindow_c, ExplainOut);
STATE_DEFINE(dLytMsgWindow_c, DemoIn);
STATE_DEFINE(dLytMsgWindow_c, DemoVisible);
STATE_DEFINE(dLytMsgWindow_c, DemoOut);

bool dLytMsgWindow_c::build() {
    mResAcc1.attach(LayoutArcManager::GetInstance()->getLoadedData("DoButton"), "");
    mResAcc2.attach(LayoutArcManager::GetInstance()->getLoadedData("System2D"), "");
    mSelectBtn.build(&mResAcc2);

    mpTagProcessor = new dTagProcessor_c();
    mpMsgWindowUnk = new TextWindowUnk(mpTagProcessor);

    if (GLOBAL_FI_CONTEXT != nullptr) {
        fn_8016C9F0(GLOBAL_FI_CONTEXT);
    }

    mpWindowTalk = nullptr;
    mpWindowLink = nullptr;
    mpWindowSword = nullptr;
    mpWindowWood = nullptr;
    mpWindowStone = nullptr;
    mpWindowGet = nullptr;
    mpWindowDemo = nullptr;
    mpAutoExplain = nullptr;
    mpAutoCaption = nullptr;
    mpCurrentSubtype = nullptr;

    mTextOptionSelection = 0;
    mSpecialFiMenuValue = 7;

    field_0x1220 = 0;

    field_0x80C = 0;
    field_0x80D = 0;
    field_0x80E = 0;
    mShowAutoMessage = 0;
    field_0x810 = 0;

    field_0x774 = 0;
    field_0x778 = 0;
    field_0x77C = 0;
    field_0x780 = 0;
    field_0x784 = -1;

    mAlsoEntryPointToTrigger = 0xFFFF;
    mEntryPointToTrigger = 0xFFFF;

    mName = "";

    field_0x811 = 0;
    field_0x812 = 0;
    field_0x813 = 0;
    field_0x814 = 0;
    field_0x815 = 0;
    field_0x816 = 0;
    field_0x817 = 0;
    field_0x818 = 0;
    field_0x819 = 0;
    field_0x81B = 0;
    field_0x81A = 0;
    field_0x81C = 0;
    field_0x81D = 0;

    field_0x824 = nullptr;
    field_0x820 = 0;

    field_0x81E = 0;

    field_0x828 = 0;

    mStateMgr.changeState(StateID_Invisible);

    return true;
}

bool dLytMsgWindow_c::remove() {
    delete mpMsgWindowUnk;
    mpMsgWindowUnk = nullptr;
    delete mpTagProcessor;
    mpTagProcessor = nullptr;
    mSelectBtn.remove();
    removeSubMsgManagers();
    field_0x824 = 0;
    mResAcc1.detach();
    mResAcc2.detach();
    return true;
}

void dLytMsgWindow_c::initializeState_Invisible() {}
void dLytMsgWindow_c::executeState_Invisible() {
    if (dMessage_c::getInstance()->getField_0x329()) {
        mStateMgr.changeState(StateID_MapOpen);
    } else if (dMessage_c::getInstance()->getField_0x32A()) {
        mStateMgr.changeState(StateID_WaitKeyMapClose);
    } else if (field_0x80C) {
        field_0x80C = 0;
        if (isValidTextLabel(mName)) {
            mNameCopy = mName;
            dMessage_c::loadTextByLabel(mNameCopy, mpTagProcessor, true, 0, 0);

            if (field_0x1220 == 0) {
                if (field_0x80D != 0) {
                    if (mpTagProcessor->getMsgWindowSubtype() == 0x16) {
                        mpAutoCaption = new dLytAutoCaption_c();
                        mpAutoCaption->build(&mResAcc1, &mResAcc2, mpTagProcessor);
                        mpCurrentSubtype = mpAutoCaption;
                    } else {
                        mpAutoExplain = new dLytAutoExplain_c();
                        mpAutoExplain->build(&mResAcc1, &mResAcc2, mpTagProcessor);
                        mpCurrentSubtype = mpAutoExplain;
                    }

                    mpCurrentSubtype->setPriority(0x8A);
                    dTextBox_c *box = mpCurrentSubtype->vt_0x30();
                    mpMsgWindowUnk->fn_800B2130(mNameCopy, box, nullptr, true);
                    mpMsgWindowUnk->textAdvancingRelated(true, true);
                    mShowAutoMessage = false;
                    setTextToDisplay(mpMsgWindowUnk->getProcessedText());
                    mStateMgr.changeState(StateID_ExplainIn);
                } else {
                    createSubMsgManager(mpTagProcessor->getMsgWindowSubtype());
                    field_0x828 = 0;
                    mSpecialFiMenuValue = 7;
                    if (mpTagProcessor->getMsgWindowSubtype() == 6) {
                        mpCurrentSubtype = mpWindowWood;
                        dTextBox_c *box = mpCurrentSubtype->vt_0x30();
                        mpMsgWindowUnk->fn_800B2130(mNameCopy, box, nullptr, true);
                        mpMsgWindowUnk->textAdvancingRelated(true, true);
                        setTextToDisplay(mpMsgWindowUnk->getProcessedText());
                    } else if (mpTagProcessor->getMsgWindowSubtype() == 7) {
                        mpCurrentSubtype = mpWindowStone;
                        dTextBox_c *box = mpCurrentSubtype->vt_0x30();
                        mpMsgWindowUnk->fn_800B2130(mNameCopy, box, nullptr, true);
                        mpMsgWindowUnk->textAdvancingRelated(true, true);
                        setTextToDisplay(mpMsgWindowUnk->getProcessedText());
                    } else if (mpTagProcessor->getMsgWindowSubtype() >= 2 &&
                               mpTagProcessor->getMsgWindowSubtype() < 5) {
                        mpCurrentSubtype = mpWindowSword;
                        // TODO
                    } else if (mpTagProcessor->getMsgWindowSubtype() == 9) {
                        mpCurrentSubtype = mpWindowLink;
                        // TODO
                    } else if (mpTagProcessor->getMsgWindowSubtype() == 5) {
                        mpCurrentSubtype = mpWindowGet;
                        // TODO
                    } else if (mpTagProcessor->getMsgWindowSubtype() == 30) {
                        mpCurrentSubtype = mpWindowDemo;
                        dTextBox_c *box = mpCurrentSubtype->vt_0x30();
                        mpMsgWindowUnk->fn_800B2130(mNameCopy, box, field_0x828, true);
                        mpMsgWindowUnk->textAdvancingRelated(true, true);
                        setTextToDisplay(mpMsgWindowUnk->getProcessedText());
                        mStateMgr.changeState(StateID_DemoIn);
                        return;
                    } else {
                        mpCurrentSubtype = mpWindowTalk;
                        // TODO
                    }
                    mpCurrentSubtype->setPriority(0x8A);
                    dTextBox_c *box = mpCurrentSubtype->vt_0x30();
                    mpMsgWindowUnk->fn_800B2130(mNameCopy, box, field_0x828, true);
                    setTextToDisplay(mpMsgWindowUnk->getProcessedText());
                    mStateMgr.changeState(StateID_In);
                }
            } else {
                mStateMgr.changeState(StateID_OutputText);
            }
        }
    }
}
void dLytMsgWindow_c::finalizeState_Invisible() {}

void dLytMsgWindow_c::initializeState_In() {}
void dLytMsgWindow_c::executeState_In() {
    if (mpCurrentSubtype->isDoneOpening()) {
        mStateMgr.changeState(StateID_OutputText);
    }
}
void dLytMsgWindow_c::finalizeState_In() {}

void dLytMsgWindow_c::initializeState_OutputText() {
    field_0x81C = 0;
    field_0x81D = 0;
    field_0x820 = 0;
    field_0x81E = 0;
}
void dLytMsgWindow_c::executeState_OutputText() {}
void dLytMsgWindow_c::finalizeState_OutputText() {}

void dLytMsgWindow_c::initializeState_WaitKeyChangePage0() {}
void dLytMsgWindow_c::executeState_WaitKeyChangePage0() {}
void dLytMsgWindow_c::finalizeState_WaitKeyChangePage0() {}

void dLytMsgWindow_c::initializeState_WaitKeyChangePage1() {}
void dLytMsgWindow_c::executeState_WaitKeyChangePage1() {}
void dLytMsgWindow_c::finalizeState_WaitKeyChangePage1() {}

void dLytMsgWindow_c::initializeState_WaitKeyMsgEnd0() {}
void dLytMsgWindow_c::executeState_WaitKeyMsgEnd0() {}
void dLytMsgWindow_c::finalizeState_WaitKeyMsgEnd0() {}

void dLytMsgWindow_c::initializeState_WaitKeyMsgEnd1() {}
void dLytMsgWindow_c::executeState_WaitKeyMsgEnd1() {}
void dLytMsgWindow_c::finalizeState_WaitKeyMsgEnd1() {}

void dLytMsgWindow_c::initializeState_WaitKeyMsgEnd2() {}
void dLytMsgWindow_c::executeState_WaitKeyMsgEnd2() {}
void dLytMsgWindow_c::finalizeState_WaitKeyMsgEnd2() {}

void dLytMsgWindow_c::initializeState_WaitKeySelectQuestion() {}
void dLytMsgWindow_c::executeState_WaitKeySelectQuestion() {}
void dLytMsgWindow_c::finalizeState_WaitKeySelectQuestion() {}

void dLytMsgWindow_c::initializeState_MapOpen() {}
void dLytMsgWindow_c::executeState_MapOpen() {}
void dLytMsgWindow_c::finalizeState_MapOpen() {}

void dLytMsgWindow_c::initializeState_WaitKeyMapClose() {}
void dLytMsgWindow_c::executeState_WaitKeyMapClose() {}
void dLytMsgWindow_c::finalizeState_WaitKeyMapClose() {}

void dLytMsgWindow_c::initializeState_MapClose() {}
void dLytMsgWindow_c::executeState_MapClose() {}
void dLytMsgWindow_c::finalizeState_MapClose() {}

void dLytMsgWindow_c::initializeState_Out() {}
void dLytMsgWindow_c::executeState_Out() {}
void dLytMsgWindow_c::finalizeState_Out() {}

void dLytMsgWindow_c::initializeState_ExplainIn() {}
void dLytMsgWindow_c::executeState_ExplainIn() {}
void dLytMsgWindow_c::finalizeState_ExplainIn() {}

void dLytMsgWindow_c::initializeState_ExplainVisible() {}
void dLytMsgWindow_c::executeState_ExplainVisible() {}
void dLytMsgWindow_c::finalizeState_ExplainVisible() {}

void dLytMsgWindow_c::initializeState_ExplainOut() {}
void dLytMsgWindow_c::executeState_ExplainOut() {}
void dLytMsgWindow_c::finalizeState_ExplainOut() {}

void dLytMsgWindow_c::initializeState_DemoIn() {}
void dLytMsgWindow_c::executeState_DemoIn() {}
void dLytMsgWindow_c::finalizeState_DemoIn() {}

void dLytMsgWindow_c::initializeState_DemoVisible() {}
void dLytMsgWindow_c::executeState_DemoVisible() {}
void dLytMsgWindow_c::finalizeState_DemoVisible() {}

void dLytMsgWindow_c::initializeState_DemoOut() {}
void dLytMsgWindow_c::executeState_DemoOut() {}
void dLytMsgWindow_c::finalizeState_DemoOut() {}

#pragma dont_inline on
void dLytMsgWindow_c::createSubMsgManager(u8 type) {
    switch (type) {
        case 6:
            mpWindowWood = new dLytMsgWindowWood_c();
            mpWindowWood->build(&mResAcc1, &mResAcc2, mpTagProcessor);
            break;
        case 7:
            mpWindowStone = new dLytMsgWindowStone_c();
            mpWindowStone->build(&mResAcc1, &mResAcc2, mpTagProcessor);
            break;
        case 2:
            // TODO Sword
            break;
        case 3:
            // TODO Sword
            break;
        case 4:
            // TODO Sword
            break;
        case 5:
            // TODO Get
            mpWindowGet = new dLytMsgWindowWood_c();
            mpWindowGet->build(&mResAcc1, &mResAcc2, mpTagProcessor);
            break;
        case 9:
            // TODO Link
            mpWindowLink = new dLytMsgWindowWood_c();
            mpWindowLink->build(&mResAcc1, &mResAcc2, mpTagProcessor);
            break;
        case 30:
            // TODO Demo
            mpWindowDemo = new dLytMsgWindowWood_c();
            mpWindowDemo->build(&mResAcc1, &mResAcc2, mpTagProcessor);
            break;
        default:
            // TODO talk
            mpWindowDemo = new dLytMsgWindowWood_c();
            mpWindowDemo->build(&mResAcc1, &mResAcc2, mpTagProcessor);
            break;
    }
}

#pragma dont_inline off

void dLytMsgWindow_c::removeSubMsgManagers() {
    if (mpWindowTalk != nullptr) {
        mpWindowTalk->remove();
        delete mpWindowTalk;
        mpWindowTalk = nullptr;
    }

    if (mpWindowLink != nullptr) {
        mpWindowLink->remove();
        delete mpWindowLink;
        mpWindowLink = nullptr;
    }

    if (mpWindowSword != nullptr) {
        mpWindowSword->remove();
        delete mpWindowSword;
        mpWindowSword = nullptr;
    }

    if (mpWindowWood != nullptr) {
        mpWindowWood->remove();
        delete mpWindowWood;
        mpWindowWood = nullptr;
    }

    if (mpWindowStone != nullptr) {
        mpWindowStone->remove();
        delete mpWindowStone;
        mpWindowStone = nullptr;
    }

    if (mpWindowGet != nullptr) {
        mpWindowGet->remove();
        delete mpWindowGet;
        mpWindowGet = nullptr;
    }

    if (mpWindowDemo != nullptr) {
        mpWindowDemo->remove();
        delete mpWindowDemo;
        mpWindowDemo = nullptr;
    }

    if (mpAutoExplain != nullptr) {
        mpAutoExplain->remove();
        delete mpAutoExplain;
        mpAutoExplain = nullptr;
    }

    if (mpAutoCaption != nullptr) {
        mpAutoCaption->remove();
        delete mpAutoCaption;
        mpAutoCaption = nullptr;
    }

    mpCurrentSubtype = nullptr;
    field_0x1220 = 0;

    UnkTextThing::getInstance()->reset1("");
    UnkTextThing::getInstance()->reset0("");
    UnkTextThing::getInstance()->reset2("");
}
