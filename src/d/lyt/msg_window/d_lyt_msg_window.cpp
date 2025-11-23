#include "d/lyt/msg_window/d_lyt_msg_window.h"

#include "common.h"
#include "d/a/obj/d_a_obj_base.h"
#include "d/d_lyt_hio.h"
#include "d/d_message.h"
#include "d/d_pad.h"
#include "d/d_stage_mgr.h"
#include "d/d_tag_processor.h"
#include "d/d_textwindow_unk.h"
#include "d/lyt/d_lyt_control_game.h"
#include "d/lyt/d_lyt_map.h"
#include "d/lyt/d_textbox.h"


// clang-format off
// vtable order
#include "d/lyt/meter/d_lyt_meter.h"
#include "d/lyt/msg_window/d_lyt_msg_window_common.h"
#include "d/lyt/msg_window/d_lyt_msg_window_talk.h"
#include "d/lyt/msg_window/d_lyt_msg_window_link.h"
#include "d/lyt/msg_window/d_lyt_msg_window_sword.h"
#include "d/lyt/msg_window/d_lyt_msg_window_wood.h"
#include "d/lyt/msg_window/d_lyt_msg_window_stone.h"
#include "d/lyt/msg_window/d_lyt_msg_window_get.h"
#include "d/lyt/msg_window/d_lyt_msg_window_demo.h"
#include "d/lyt/d_lyt_auto_explain.h"
#include "d/lyt/d_lyt_auto_caption.h"
// clang-format on

#include "d/d_gfx.h"
#include "d/snd/d_snd_fi_vocal_mgr.h"
#include "d/snd/d_snd_player_mgr.h"
#include "d/snd/d_snd_small_effect_mgr.h"
#include "f/f_base.h"
#include "m/m_fader_base.h"
#include "m/m_vec.h"
#include "s/s_State.hpp"
#include "toBeSorted/arc_managers/layout_arc_manager.h"
#include "toBeSorted/d_d3d.h"
#include "toBeSorted/event_manager.h"
#include "toBeSorted/fi_context.h"

#include <cstring>

class dLytMsgWindow_HIO_c {
public:
    dLytMsgWindow_HIO_c();
    virtual ~dLytMsgWindow_HIO_c();

private:
    /* 0x04 */ u32 field_0x04;
    /* 0x08 */ u8 field_0x08;
    /* 0x09 */ u8 field_0x09;
};

dLytMsgWindow_HIO_c sHio;

dLytMsgWindow_HIO_c::dLytMsgWindow_HIO_c() {
    field_0x04 = 8;
    field_0x08 = 0;
    field_0x09 = 0;
}
dLytMsgWindow_HIO_c::~dLytMsgWindow_HIO_c() {}

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

    if (FiContext::GetInstance() != nullptr) {
        FiContext::GetInstance()->resetSaveTimeRelated();
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
    mSpecialFiMenuValue = FiContext::KEN8_Nevermind;

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
    mMsgIdx = -1;

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

    mNumOptions = 0;
    field_0x820 = 0;

    field_0x81E = 0;

    field_0x828 = nullptr;

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
    mNumOptions = 0;
    mResAcc1.detach();
    mResAcc2.detach();
    return true;
}

void dLytMsgWindow_c::initializeState_Invisible() {}
void dLytMsgWindow_c::executeState_Invisible() {
    if (!EventManager::isInEvent() && dLytMeter_c::GetInstance()->getMeterField_0x13773()) {
        dLytMeter_c::GetInstance()->setMeterField_0x13773(false);
    }
    if (dMessage_c::getInstance()->getField_0x329()) {
        mStateMgr.changeState(StateID_MapOpen);
    } else if (dMessage_c::getInstance()->getField_0x32A()) {
        mStateMgr.changeState(StateID_WaitKeyMapClose);
    } else if (field_0x80C) {
        field_0x80C = 0;
        if (dMessage_c::isValidTextLabel(mName)) {
            mNameCopy = mName;
            dMessage_c::getTextMessageByLabel(mNameCopy, mpTagProcessor, true, 0, 0);

            if (field_0x1220 == 0) {
                if (field_0x80D != 0) {
                    if (mpTagProcessor->getMsgWindowSubtype() == MSG_WINDOW_22) {
                        mpAutoCaption = new dLytAutoCaption_c();
                        mpAutoCaption->build(&mResAcc1, &mResAcc2, mpTagProcessor);
                        mpCurrentSubtype = mpAutoCaption;
                    } else {
                        mpAutoExplain = new dLytAutoExplain_c();
                        mpAutoExplain->build(&mResAcc1, &mResAcc2, mpTagProcessor);
                        mpCurrentSubtype = mpAutoExplain;
                    }

                    mpCurrentSubtype->setPriority(0x8A);
                    dTextBox_c *box = mpCurrentSubtype->getTextBox();
                    mpMsgWindowUnk->fn_800B2130(mNameCopy, box, nullptr, true);
                    mpMsgWindowUnk->textAdvancingRelated(true, true);
                    mShowAutoMessage = false;
                    setTextToDisplay(mpMsgWindowUnk->getProcessedText());
                    mStateMgr.changeState(StateID_ExplainIn);
                } else {
                    createSubMsgManager(mpTagProcessor->getMsgWindowSubtype());
                    field_0x828 = nullptr;
                    mSpecialFiMenuValue = FiContext::KEN8_Nevermind;
                    if (mpTagProcessor->getMsgWindowSubtype() == MSG_WINDOW_WOOD) {
                        mpCurrentSubtype = mpWindowWood;
                        dTextBox_c *box = mpCurrentSubtype->getTextBox();
                        mpMsgWindowUnk->fn_800B2130(mNameCopy, box, nullptr, true);
                        mpMsgWindowUnk->textAdvancingRelated(true, true);
                        setTextToDisplay(mpMsgWindowUnk->getProcessedText());
                    } else if (mpTagProcessor->getMsgWindowSubtype() == MSG_WINDOW_STONE) {
                        mpCurrentSubtype = mpWindowStone;
                        dTextBox_c *box = mpCurrentSubtype->getTextBox();
                        mpMsgWindowUnk->fn_800B2130(mNameCopy, box, nullptr, true);
                        mpMsgWindowUnk->textAdvancingRelated(true, true);
                        setTextToDisplay(mpMsgWindowUnk->getProcessedText());
                    } else if (mpTagProcessor->getMsgWindowSubtype() >= MSG_WINDOW_SWORD_FI &&
                               mpTagProcessor->getMsgWindowSubtype() < MSG_WINDOW_SWORD_FI + 3) {
                        mpCurrentSubtype = mpWindowSword;
                        field_0x828 = mpWindowSword->getCharData();
                    } else if (mpTagProcessor->getMsgWindowSubtype() == MSG_WINDOW_LINK) {
                        mpCurrentSubtype = mpWindowLink;
                        field_0x828 = mpWindowLink->getCharData();
                    } else if (mpTagProcessor->getMsgWindowSubtype() == MSG_WINDOW_GET) {
                        mpCurrentSubtype = mpWindowGet;
                        field_0x828 = mpWindowGet->getCharData();
                    } else if (mpTagProcessor->getMsgWindowSubtype() == MSG_WINDOW_DEMO) {
                        mpCurrentSubtype = mpWindowDemo;
                        field_0x828 = mpWindowDemo->getCharData();
                        dTextBox_c *box = mpCurrentSubtype->getTextBox();
                        mpMsgWindowUnk->fn_800B2130(mNameCopy, box, field_0x828, true);
                        mpMsgWindowUnk->textAdvancingRelated(true, true);
                        setTextToDisplay(mpMsgWindowUnk->getProcessedText());
                        mpCurrentSubtype->setPriority(0x8A);
                        mStateMgr.changeState(StateID_DemoIn);
                        return;
                    } else {
                        mpCurrentSubtype = mpWindowTalk;
                        field_0x828 = mpWindowTalk->getCharData();
                    }
                    mpCurrentSubtype->setPriority(0x8A);
                    dTextBox_c *box = mpCurrentSubtype->getTextBox();
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

void dLytMsgWindow_c::initializeState_In() {
    dAcObjBase_c *obj = EventManager::fn_800A08F0(fBase_c::ACTOR);
    u32 param = 0;
    if (mpTagProcessor->getField_0x90D() == 1) {
        param = 0;
    } else if (mpTagProcessor->getField_0x90D() == 2) {
        param = 2;
    } else if (mpTagProcessor->getField_0x90D() == 3) {
        param = 1;
    } else if (obj != nullptr && d3d::fn_80016A90(obj->mPositionCopy2)) {
        d3d::fn_80016960(field_0x768, obj->mPositionCopy2);
        mVec3_c v = field_0x768;
        v.z = 0.f;
        field_0x768 = v;
        f32 h, b;
        h = dGfx_c::getCurrentScreenHeight();
        b = +dGfx_c::getCurrentScreenBottom();

        if (v.y < b + (h / 3.f)) {
            param = 2;
        }
    }
    mpCurrentSubtype->open(obj, param);
}
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
void dLytMsgWindow_c::executeState_OutputText() {
    u16 oldValue = mpMsgWindowUnk->getField_0x147A();
    field_0x812 = 0;
    if (mpTagProcessor->getField_0x830() != -1) {
        field_0x811 = 1;
    } else {
        field_0x811 = 0;
    }

    if (field_0x815 == 0 && field_0x816 == 0) {
        mpMsgWindowUnk->textAdvancingRelated(false, true);
        if (field_0x811 == 0 && dPad::getDownA() && field_0x815 == 0 && field_0x816 == 0) {
            mpMsgWindowUnk->textAdvancingRelated(false, true);
        }
    }

    if (mpTagProcessor->getMsgWindowSubtype() >= MSG_WINDOW_SWORD_FI &&
        mpTagProcessor->getMsgWindowSubtype() < MSG_WINDOW_SWORD_FI + 3) {
        dSndFiVocalMgr_c::GetInstance()->executeOutputText(
            mpMsgWindowUnk->getField_0x147A(), mpMsgWindowUnk->getField_0x147C()
        );
    } else if (mpTagProcessor->getMsgWindowSubtype() <= MSG_WINDOW_1 && oldValue != mpMsgWindowUnk->getField_0x147A()) {
        u16 a = mpMsgWindowUnk->getField_0x147C();
        f32 b = (dTagProcessor_c::fn_800B8040(0, 0) * 100.0f);
        dSndSmallEffectMgr_c::GetInstance()->playButtonPressSoundWhenAdvancingTextBoxes(a / b);
    }

    if (setTextToDisplay(mpMsgWindowUnk->getProcessedText())) {
        if (field_0x81C != 0) {
            if (field_0x811 != 0) {
                mStateMgr.changeState(StateID_WaitKeyMsgEnd0);
            } else if (mpTagProcessor->getField_0x90E() != 0) {
                mStateMgr.changeState(StateID_WaitKeyMsgEnd0);
            } else if (mpCurrentSubtype->startConfirm()) {
                field_0x817 = 1;
                mStateMgr.changeState(StateID_WaitKeyMsgEnd0);
            }
        } else if (field_0x81D != 0) {
            if (field_0x811 != 0) {
                mStateMgr.changeState(StateID_WaitKeyChangePage0);
            } else if (mpCurrentSubtype->startConfirm()) {
                field_0x817 = 1;
                mStateMgr.changeState(StateID_WaitKeyChangePage0);
            }
        }
    }

    field_0x81C = mpMsgWindowUnk->checkEndReached();
    field_0x81D = mpMsgWindowUnk->checkLastLineReached();
}
void dLytMsgWindow_c::finalizeState_OutputText() {}

void dLytMsgWindow_c::initializeState_WaitKeyChangePage0() {
    if (field_0x811 != 0) {
        field_0x812 = 1;
    } else if (field_0x814 == 0) {
        dSndPlayerMgr_c::GetInstance()->enterMsgWait();
    }
}
void dLytMsgWindow_c::executeState_WaitKeyChangePage0() {
    bool allowChange = false;
    if (field_0x811 != 0) {
        if (mpTagProcessor->getField_0x830() <= 0) {
            allowChange = true;
        }
    } else if (field_0x814 != 0) {
        if (field_0x813 != 0) {
            field_0x813 = 0;
            allowChange = true;
        }
    } else if (dPad::getDownTrigA() || fn_8011A5D0()) {
        dSndPlayerMgr_c::GetInstance()->leaveMsgWait();
        allowChange = true;
    }

    if (!allowChange) {
        return;
    }

    if (field_0x811 == 0) {
        mStateMgr.changeState(StateID_WaitKeyChangePage1);
    } else {
        mpMsgWindowUnk->fn_800B2AA0();
        if (mpTagProcessor->getMsgWindowSubtype() >= MSG_WINDOW_WOOD &&
            mpTagProcessor->getMsgWindowSubtype() < MSG_WINDOW_WOOD + 2) {
            mpMsgWindowUnk->textAdvancingRelated(true, true);
        }
        setTextToDisplay(mpMsgWindowUnk->getProcessedText());
        field_0x818 = 1;
        mStateMgr.changeState(StateID_OutputText);
    }
}
void dLytMsgWindow_c::finalizeState_WaitKeyChangePage0() {}

void dLytMsgWindow_c::initializeState_WaitKeyChangePage1() {}
void dLytMsgWindow_c::executeState_WaitKeyChangePage1() {
    if (mpCurrentSubtype->startDecide(false)) {
        field_0x817 = 0;
        mpMsgWindowUnk->fn_800B2AA0();
        if (mpTagProcessor->getMsgWindowSubtype() >= MSG_WINDOW_WOOD &&
            mpTagProcessor->getMsgWindowSubtype() < MSG_WINDOW_WOOD + 2) {
            mpMsgWindowUnk->textAdvancingRelated(true, true);
        }
        setTextToDisplay(mpMsgWindowUnk->getProcessedText());
        field_0x818 = 1;
        mStateMgr.changeState(StateID_OutputText);
    }
}
void dLytMsgWindow_c::finalizeState_WaitKeyChangePage1() {}

void dLytMsgWindow_c::initializeState_WaitKeyMsgEnd0() {
    if (field_0x811 != 0) {
        field_0x812 = 1;
    } else if (mpTagProcessor->getField_0x90E() == 0 && field_0x814 == 0) {
        dSndPlayerMgr_c::GetInstance()->enterMsgWait();
    }
}
void dLytMsgWindow_c::executeState_WaitKeyMsgEnd0() {
    bool allowChange = false;
    if (field_0x811 != 0) {
        if (mpTagProcessor->getField_0x830() <= 0) {
            allowChange = true;
        }
    } else if (mpTagProcessor->getField_0x90E() != 0) {
        allowChange = true;
    } else if (field_0x814 != 0) {
        if (field_0x813 != 0) {
            field_0x813 = 0;
            allowChange = true;
        }
    } else if (dPad::getDownTrigA() || fn_8011A5D0()) {
        dSndPlayerMgr_c::GetInstance()->leaveMsgWait();
        allowChange = true;
    }

    if (!allowChange) {
        return;
    }

    if (field_0x811 == 0 && mpTagProcessor->getField_0x90E() == 0) {
        mStateMgr.changeState(StateID_WaitKeyMsgEnd1);
    } else if (field_0x810 != 0) {
        if (dMessage_c::isValidTextLabel(mName)) {
            mNameCopy = mName;
            dTextBox_c *box = mpCurrentSubtype->getTextBox();
            mpMsgWindowUnk->fn_800B2130(mNameCopy, box, field_0x828, true);
            setTextToDisplay(mpMsgWindowUnk->getProcessedText());
            field_0x818 = 1;
            mStateMgr.changeState(StateID_OutputText);
        }
        field_0x810 = 0;
    } else {
        dSndPlayerMgr_c::GetInstance()->unsetMsgActor();
        if (mpTagProcessor->getField_0x90E() != 0) {
            mStateMgr.changeState(StateID_WaitKeySelectQuestion);
        } else {
            mStateMgr.changeState(StateID_Out);
        }
    }
}
void dLytMsgWindow_c::finalizeState_WaitKeyMsgEnd0() {}

void dLytMsgWindow_c::initializeState_WaitKeyMsgEnd1() {}
void dLytMsgWindow_c::executeState_WaitKeyMsgEnd1() {
    if (mpCurrentSubtype->startDecide(false)) {
        if (field_0x810 != 0) {
            field_0x817 = 0;
            if (dMessage_c::isValidTextLabel(mName)) {
                mNameCopy = mName;
                mpMsgWindowUnk->fn_800B2130(mNameCopy, mpCurrentSubtype->getTextBox(), field_0x828, true);
                setTextToDisplay(mpMsgWindowUnk->getProcessedText());
                field_0x818 = 1;
                mStateMgr.changeState(StateID_OutputText);
            }
            field_0x810 = 0;
        } else {
            dSndPlayerMgr_c::GetInstance()->unsetMsgActor();
            if (mpTagProcessor->getField_0x90E() != 0) {
                mStateMgr.changeState(StateID_WaitKeySelectQuestion);
            } else if (field_0x817 == 0) {
                mStateMgr.changeState(StateID_Out);
            } else {
                field_0x817 = 0;
                mStateMgr.changeState(StateID_WaitKeyMsgEnd2);
            }
        }
    }
}
void dLytMsgWindow_c::finalizeState_WaitKeyMsgEnd1() {}

void dLytMsgWindow_c::initializeState_WaitKeyMsgEnd2() {}
void dLytMsgWindow_c::executeState_WaitKeyMsgEnd2() {
    if (mpCurrentSubtype->isDoneDecide()) {
        mStateMgr.changeState(StateID_Out);
    }
}
void dLytMsgWindow_c::finalizeState_WaitKeyMsgEnd2() {}

static wchar_t *sBufs[4];

void dLytMsgWindow_c::initializeState_WaitKeySelectQuestion() {
    // TODO regswaps
    mNumOptions = mpTagProcessor->getField_0x90E();

    mSelectBtn.setField_0x9B8(mpTagProcessor->getField_0x82C());
    s32 tmp = mpTagProcessor->getCancelBtnIdx();
    mSelectBtn.setCancelBtnIdx(tmp);
    mSelectBtn.setField_0x990(tmp);
    mSelectBtn.setTagProcessor(mpTagProcessor);

    for (s32 i = 0; i < 4; i++) {
        sBufs[i] = mpTagProcessor->getOptionString(i);
    }

    mSelectBtn.requestIn(mNumOptions, true);

    for (s32 i = 0, option = 0; i < mNumOptions; option++, i++) {
        for (s32 j = 0; j < 2; j++) {
            mSelectBtn.getSelectTextBox(option, j)->setTextWithGlobalTextProcessor(sBufs[i]);
        }
    }
}
void dLytMsgWindow_c::executeState_WaitKeySelectQuestion() {
    s32 selection = mSelectBtn.getDecidedBtnIdx();
    if (selection >= 0) {
        if (mSelectBtn.isStateWait()) {
            bool doFiThing = false;
            mpTagProcessor->setField_0x90E(0);
            mpTagProcessor->setField_0x82C(-1);
            mpTagProcessor->setCancelBtnIdx(-1);
            mSelectBtn.setTagProcessor(nullptr);
            mNumOptions = 0;
            mTextOptionSelection = selection;
            field_0x1220 = 0;
            if (FiContext::GetInstance() != nullptr) {
                doFiThing = FiContext::GetInstance()->getIsInFiMainMenu();
            }

            if (doFiThing) {
                switch ((int)mTextOptionSelection) {
                    case 0: mSpecialFiMenuValue = FiContext::getGlobalFiInfo0(0); break;
                    case 1: mSpecialFiMenuValue = FiContext::getGlobalFiInfo0(1); break;
                    case 2: mSpecialFiMenuValue = FiContext::getGlobalFiInfo0(2); break;
                    case 3: mSpecialFiMenuValue = FiContext::KEN8_Nevermind; break;
                }
            }

            if (field_0x1220 != 0) {
                mStateMgr.changeState(StateID_Invisible);
            } else {
                mStateMgr.changeState(StateID_Out);
            }
        }
    }
}
void dLytMsgWindow_c::finalizeState_WaitKeySelectQuestion() {}

void dLytMsgWindow_c::initializeState_MapOpen() {}
void dLytMsgWindow_c::executeState_MapOpen() {
    if ((dLytMap_c::GetInstance() != nullptr && dLytMap_c::GetInstance()->isOpen()) ||
        !dLytControlGame_c::getInstance()->isInSomeMapState()) {
        dMessage_c::getInstance()->setField_0x329(0);
        mStateMgr.changeState(StateID_Invisible);
    }
}
void dLytMsgWindow_c::finalizeState_MapOpen() {}

void dLytMsgWindow_c::initializeState_WaitKeyMapClose() {}
void dLytMsgWindow_c::executeState_WaitKeyMapClose() {
    if ((dPad::getDownTrigB() || dPad::getDownTrigPlus()) && !dLytControlGame_c::getInstance()->isNotInStateMap()) {
        dLytControlGame_c::getInstance()->setMapEventDone(true);
        mStateMgr.changeState(StateID_MapClose);
    }
}
void dLytMsgWindow_c::finalizeState_WaitKeyMapClose() {}

void dLytMsgWindow_c::initializeState_MapClose() {}
void dLytMsgWindow_c::executeState_MapClose() {
    if (dLytMeter_c::GetInstance()->getBasicPosition() == dLytMeterMain_c::POSITION_NORMAL) {
        dMessage_c::getInstance()->setField_0x32A(false);
        dMessage_c::getInstance()->clearLightPillarRelatedArgs();
        mStateMgr.changeState(StateID_Invisible);
    }
}
void dLytMsgWindow_c::finalizeState_MapClose() {}

void dLytMsgWindow_c::initializeState_Out() {
    field_0x768 = field_0x75C;
    mpCurrentSubtype->close();
}
void dLytMsgWindow_c::executeState_Out() {
    if (mpCurrentSubtype->isDoneClosing() && (field_0x817 == 0 || mpCurrentSubtype->isDoneDecide())) {
        removeSubMsgManagers();
        mStateMgr.changeState(StateID_Invisible);
    }
}
void dLytMsgWindow_c::finalizeState_Out() {}

void dLytMsgWindow_c::initializeState_ExplainIn() {
    mpCurrentSubtype->open(nullptr, false);
}
void dLytMsgWindow_c::executeState_ExplainIn() {
    if (mpCurrentSubtype->isDoneOpening()) {
        mpMsgWindowUnk->textAdvancingRelated(true, true);
        if (setTextToDisplay(mpMsgWindowUnk->getProcessedText())) {
            mStateMgr.changeState(StateID_ExplainVisible);
        }
    }
}
void dLytMsgWindow_c::finalizeState_ExplainIn() {}

void dLytMsgWindow_c::initializeState_ExplainVisible() {}
void dLytMsgWindow_c::executeState_ExplainVisible() {
    if ((mShowAutoMessage != 0 || (mpAutoExplain != nullptr && mpAutoExplain->checkSomethingMeter())) ||
        ((mpAutoCaption != nullptr && mpAutoCaption->checkSomethingMeter()))) {
        mStateMgr.changeState(StateID_ExplainOut);
    }
}
void dLytMsgWindow_c::finalizeState_ExplainVisible() {}

void dLytMsgWindow_c::initializeState_ExplainOut() {
    mpCurrentSubtype->close();
}
void dLytMsgWindow_c::executeState_ExplainOut() {
    if (mpCurrentSubtype->isDoneClosing()) {
        field_0x80D = 0;
        mShowAutoMessage = 0;
        removeSubMsgManagers();
        mStateMgr.changeState(StateID_Invisible);
    }
}
void dLytMsgWindow_c::finalizeState_ExplainOut() {}

void dLytMsgWindow_c::initializeState_DemoIn() {
    mpCurrentSubtype->open(nullptr, 0);
}
void dLytMsgWindow_c::executeState_DemoIn() {
    if (mpCurrentSubtype->isDoneOpening()) {
        mStateMgr.changeState(StateID_DemoVisible);
    }
}
void dLytMsgWindow_c::finalizeState_DemoIn() {}

void dLytMsgWindow_c::initializeState_DemoVisible() {
    field_0x81C = 0;
    field_0x812 = 1;
}
void dLytMsgWindow_c::executeState_DemoVisible() {
    if (mpTagProcessor->getField_0x830() <= 0) {
        mStateMgr.changeState(StateID_DemoOut);
    }
    field_0x81C = mpMsgWindowUnk->checkEndReached();
}
void dLytMsgWindow_c::finalizeState_DemoVisible() {}

void dLytMsgWindow_c::initializeState_DemoOut() {
    mpCurrentSubtype->close();
}
void dLytMsgWindow_c::executeState_DemoOut() {
    if (mpCurrentSubtype->isDoneClosing()) {
        if (field_0x81C != 0) {
            field_0x81C = 0;
            removeSubMsgManagers();
            mStateMgr.changeState(StateID_Invisible);
        } else {
            mpMsgWindowUnk->fn_800B2AA0();
            mpMsgWindowUnk->textAdvancingRelated(true, true);
            setTextToDisplay(mpMsgWindowUnk->getProcessedText());
            mStateMgr.changeState(StateID_DemoIn);
        }
    }
}
void dLytMsgWindow_c::finalizeState_DemoOut() {}

bool dLytMsgWindow_c::execute() {
    field_0x818 = 0;
    u8 old_field_0x816 = field_0x816;
    if (*mStateMgr.getStateID() == StateID_DemoIn || *mStateMgr.getStateID() == StateID_DemoVisible ||
        *mStateMgr.getStateID() == StateID_DemoOut) {
        dStageMgr_c *mgr = dStageMgr_c::GetInstance();
        // Bleh
        bool b =
            !(mgr->getFader()->isNotStatus(static_cast<mFaderBase_c::EStatus>(-1)) ||
              mgr->getFader()->isNotStatus(mFaderBase_c::FADED_OUT));
        if (!b) {
            field_0x819 = 1;
            field_0x81B = 1;
        }
    }

    if (field_0x81B != 0) {
        if (isVisible()) {
            removeSubMsgManagers();
            mStateMgr.changeState(StateID_Invisible);
        }
        mpMsgWindowUnk->reset();
        mSelectBtn.remove();
        field_0x81B = 0;
        // TODO boss caption
    }
    // TODO ActorEventFlowManagerRelated::checkEventFinished

    mStateMgr.executeState();

    if (isVisible()) {
        mSelectBtn.execute();
        if (mpCurrentSubtype != nullptr) {
            mpCurrentSubtype->execute();
        }
    }

    if (field_0x812 != 0) {
        if (mpTagProcessor->getField_0x830() != -1) {
            mpTagProcessor->tick0x830();
        } else {
            field_0x811 = 0;
            field_0x812 = 0;
        }
    }

    mpTagProcessor->execute();
    if (field_0x816 == 1 && old_field_0x816 == 1) {
        field_0x816 = 0;
    }
    return true;
}

bool dLytMsgWindow_c::draw() {
    if (isVisible() || field_0x1220 != 0) {
        if (mpCurrentSubtype != nullptr) {
            // TODO tag processor
            mpCurrentSubtype->addToDrawList();
        }
        mSelectBtn.draw();
    }

    return true;
}

bool dLytMsgWindow_c::setCurrentLabelName(const char *name, bool storeFile) {
    // TODO
    mName = name;
    return !std::strcmp(name, "FR_SIREN_04");
}

bool dLytMsgWindow_c::isVisible() const {
    return !(*mStateMgr.getStateID() == StateID_Invisible);
}

bool dLytMsgWindow_c::setTextToDisplay(const wchar_t *text) {
    if (mpCurrentSubtype != nullptr) {
        return mpCurrentSubtype->setText(text);
    }
    return false;
}

bool dLytMsgWindow_c::isOutputtingText() const {
    return *mStateMgr.getStateID() == StateID_OutputText;
}

void dLytMsgWindow_c::setCurrentEntrypointName(const char *name) {
    mCurrentEntrypointName = name;
}

void dLytMsgWindow_c::setCurrentFlowFilename(const char *name) {
    mCurrentFlowFileName = name;
}

void dLytMsgWindow_c::createSubMsgManager(u8 type) {
    switch (type) {
        case MSG_WINDOW_WOOD:
            mpWindowWood = new dLytMsgWindowWood_c();
            mpWindowWood->build(&mResAcc1, &mResAcc2, mpTagProcessor);
            break;
        case MSG_WINDOW_STONE:
            mpWindowStone = new dLytMsgWindowStone_c();
            mpWindowStone->build(&mResAcc1, &mResAcc2, mpTagProcessor);
            break;
        case MSG_WINDOW_SWORD_FI:
            mpWindowSword = new dLytMsgWindowSword_c();
            mpWindowSword->setSwordType(dLytTextSword::FI);
            mpWindowSword->build(&mResAcc1, &mResAcc2, mpTagProcessor);
            break;
        case MSG_WINDOW_SWORD_GHIRAHIM:
            mpWindowSword = new dLytMsgWindowSword_c();
            mpWindowSword->setSwordType(dLytTextSword::GHIRAHIM);
            mpWindowSword->build(&mResAcc1, &mResAcc2, mpTagProcessor);
            break;
        case MSG_WINDOW_SWORD_LASTBOSS:
            mpWindowSword = new dLytMsgWindowSword_c();
            mpWindowSword->setSwordType(dLytTextSword::LASTBOSS);
            mpWindowSword->build(&mResAcc1, &mResAcc2, mpTagProcessor);
            break;
        case MSG_WINDOW_LINK:
            mpWindowLink = new dLytMsgWindowLink_c();
            mpWindowLink->build(&mResAcc1, &mResAcc2, mpTagProcessor);
            break;
        case MSG_WINDOW_GET:
            mpWindowGet = new dLytMsgWindowGet_c();
            mpWindowGet->build(&mResAcc1, &mResAcc2, mpTagProcessor);
            break;
        case MSG_WINDOW_DEMO:
            mpWindowDemo = new dLytMsgWindowDemo_c();
            mpWindowDemo->build(&mResAcc1, &mResAcc2, mpTagProcessor);
            break;
        default:
            mpWindowTalk = new dLytMsgWindowTalk_c();
            mpWindowTalk->build(&mResAcc1, &mResAcc2, mpTagProcessor);
            break;
    }
}

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

    dLyt_HIO_c::getInstance()->setStr2("");
    dLyt_HIO_c::getInstance()->setStr1("");
    dLyt_HIO_c::getInstance()->setStr3("");
}
