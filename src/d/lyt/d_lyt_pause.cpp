#include "d/lyt/d_lyt_pause.h"

#include "common.h"
#include "d/d_base.h"
#include "d/d_cs_game.h"
#include "d/d_d2d.h"
#include "d/d_gfx.h"
#include "d/d_heap.h"
#include "d/d_pad.h"
#include "d/d_pad_manager.h"
#include "d/d_pad_nav.h"
#include "d/d_player.h"
#include "d/flag/storyflag_manager.h"
#include "d/lyt/d_lyt_common_arrow.h"
#include "d/lyt/d_lyt_control_game.h"
#include "d/lyt/d_lyt_pause_back.h"
#include "d/lyt/d_lyt_pause_disp_00.h"
#include "d/lyt/d_lyt_pause_disp_01.h"
#include "d/lyt/d_lyt_pause_info.h"
#include "d/lyt/d_lyt_pause_text.h"
#include "d/lyt/meter/d_lyt_meter.h"
#include "d/snd/d_snd_player_mgr.h"
#include "d/snd/d_snd_small_effect_mgr.h"
#include "d/snd/d_snd_wzsound.h"
#include "egg/gfx/eggTextureBuffer.h"
#include "m/m_pad.h"
#include "m/m_video.h"
#include "toBeSorted/arc_managers/layout_arc_manager.h"

STATE_DEFINE(dLytPauseMain_c, None);
STATE_DEFINE(dLytPauseMain_c, In);
STATE_DEFINE(dLytPauseMain_c, Wait);
STATE_DEFINE(dLytPauseMain_c, Change);
STATE_DEFINE(dLytPauseMain_c, Select);
STATE_DEFINE(dLytPauseMain_c, Ring);
STATE_DEFINE(dLytPauseMain_c, GetDemo);
STATE_DEFINE(dLytPauseMain_c, Out);

STATE_DEFINE(dLytPauseMgr_c, None);
STATE_DEFINE(dLytPauseMgr_c, In);
STATE_DEFINE(dLytPauseMgr_c, Wait);
STATE_DEFINE(dLytPauseMgr_c, Change);
STATE_DEFINE(dLytPauseMgr_c, Select);
STATE_DEFINE(dLytPauseMgr_c, Ring);
STATE_DEFINE(dLytPauseMgr_c, GetDemo);
STATE_DEFINE(dLytPauseMgr_c, Out);

f32 dLytPauseMgr_c::sDisp00ArrowRotation;
f32 dLytPauseMgr_c::sDisp00ArrowLength;
dLytPauseMgr_c *dLytPauseMgr_c::sInstance;

dLytPauseMain_c::dLytPauseMain_c()
    : mStateMgr(*this),
      mpPauseBack(nullptr),
      mpDisp00(nullptr),
      mpDisp01(nullptr),
      mpPauseInfo(nullptr),
      mpPauseText(nullptr),
      mpArrow(nullptr) {}

bool dLytPauseMain_c::build() {
    mpPauseBack = new (dHeap::layoutEx2Heap.heap) dLytPauseBack_c();
    mpPauseBack->build();
    mpPauseInfo = new (dHeap::layoutEx2Heap.heap) dLytPauseInfo_c();
    mpPauseInfo->build();
    mpPauseText = new (dHeap::layoutEx2Heap.heap) dLytPauseText_c();
    mpPauseText->build();
    mpArrow = new (dHeap::layoutEx2Heap.heap) dLytCommonArrow_c();
    mpArrow->build();
    mpDisp00 = new (dHeap::layoutEx2Heap.heap) dLytPauseDisp00_c();
    mpDisp00->build();
    mpDisp01 = new (dHeap::layoutEx2Heap.heap) dLytPauseDisp01_c();
    mpDisp01->build();
    mStateMgr.changeState(StateID_None);
    return true;
}

bool dLytPauseMain_c::remove() {
    if (mpArrow != nullptr) {
        mpArrow->remove();
        delete mpArrow;
        mpArrow = nullptr;
    }

    if (mpPauseText != nullptr) {
        mpPauseText->remove();
        delete mpPauseText;
        mpPauseText = nullptr;
    }

    if (mpPauseInfo != nullptr) {
        mpPauseInfo->remove();
        delete mpPauseInfo;
        mpPauseInfo = nullptr;
    }

    if (mpDisp00 != nullptr) {
        mpDisp00->remove();
        delete mpDisp00;
        mpDisp00 = nullptr;
    }

    if (mpDisp01 != nullptr) {
        mpDisp01->remove();
        delete mpDisp01;
        mpDisp01 = nullptr;
    }

    if (mpPauseBack != nullptr) {
        mpPauseBack->remove();
        delete mpPauseBack;
        mpPauseBack = nullptr;
    }

    dBase_c::s_NextExecuteControlFlags &= ~dBase_c::BASE_PROP_0x1;
    dBase_c::s_DrawControlFlags &= ~dBase_c::BASE_PROP_0x1;

    return true;
}

bool dLytPauseMain_c::execute() {
    mStateMgr.executeState();
    mpPauseBack->execute();
    mpDisp00->execute();
    mpDisp01->execute();
    mpPauseInfo->execute();
    mpPauseText->execute();
    mpArrow->execute();
    return true;
}

bool dLytPauseMain_c::draw() {
    mpPauseBack->draw();
    if (mpDisp00 != nullptr) {
        mpDisp00->draw();
    }
    if (mpDisp01 != nullptr) {
        mpDisp01->draw();
    }
    if (!dLytControlGame_c::getInstance()->isPauseDemo()) {
        mpArrow->draw();
    }
    mpPauseText->draw();
    mpPauseInfo->draw();
    return true;
}

void dLytPauseMain_c::drawDirectly() {
    if (dLytControlGame_c::getInstance()->getCurrentPauseDisp() == dLytPauseMgr_c::DISP_00_INVENTORY) {
        mpDisp00->drawDirectly();
    } else {
        mpDisp01->drawDirectly();
    }
}

void dLytPauseMain_c::loadBgTextureUnused() {
    // no-op
}

void dLytPauseMain_c::loadBgTexture() {
    if (mpPauseBack != nullptr) {
        mpPauseBack->loadBgTexture();
    }
}

void dLytPauseMain_c::requestIn() {
    mInRequest = true;
}

void dLytPauseMain_c::requestOut() {
    mOutRequest = true;
}

void dLytPauseMain_c::requestChange() {
    mChangeRequest = true;
}

void dLytPauseMain_c::requestSelect() {
    mSelectRequest = true;
}

void dLytPauseMain_c::requestRingToggle() {
    mRingToggleRequest = true;
}

bool dLytPauseMain_c::hasSelection() const {
    if (dLytControlGame_c::getInstance()->getCurrentPauseDisp() != dLytPauseMgr_c::DISP_00_INVENTORY) {
        return mpDisp01->hasSelection();
    } else {
        return mpDisp00->hasSelection();
    }
}

void dLytPauseMain_c::updateTitle() {
    if (mpPauseInfo != nullptr) {
        mpPauseInfo->updateTitle();
    }
}

void dLytPauseMain_c::initializeState_None() {
    mIsChangingState = false;
    mInRequest = false;
    mOutRequest = false;
    mChangeRequest = false;
    mSelectRequest = false;
    mRingToggleRequest = false;
    mPartStateChangeFlags = 0;
    mStep = 0;
    mTimer = 0;
}
void dLytPauseMain_c::executeState_None() {
    if (mInRequest == true) {
        mInRequest = false;
        mpPauseBack->requestIn();
        if (dLytControlGame_c::getInstance()->getCurrentPauseDisp() == dLytPauseMgr_c::DISP_00_INVENTORY) {
            mpDisp00->requestIn(false);
        } else {
            mpDisp01->requestIn(false);
        }
        mpPauseInfo->requestIn();
        mStateMgr.changeState(StateID_In);
    }
}
void dLytPauseMain_c::finalizeState_None() {}

void dLytPauseMain_c::initializeState_In() {
    mPartStateChangeFlags = 0;
    mTimer = 0;
}
void dLytPauseMain_c::executeState_In() {
    if (mTimer == 8) {
        mpArrow->requestIn();
    }
    mTimer++;

    if (mpPauseBack->isChangingState() == true) {
        mPartStateChangeFlags |= 0x1;
    }

    if (dLytControlGame_c::getInstance()->getCurrentPauseDisp() == dLytPauseMgr_c::DISP_00_INVENTORY) {
        if (mpDisp00->isChangingState() == true) {
            mPartStateChangeFlags |= 0x2;
        }
    } else {
        if (mpDisp01->isChangingState() == true) {
            mPartStateChangeFlags |= 0x2;
        }
    }

    if (mpPauseInfo->isChangingState() == true) {
        mPartStateChangeFlags |= 0x4;
    }

    if (mPartStateChangeFlags == (0x1 | 0x2 | 0x4)) {
        if (dLytControlGame_c::getInstance()->isPauseDemo()) {
            mStateMgr.changeState(StateID_GetDemo);
        } else {
            mStateMgr.changeState(StateID_Wait);
        }
    }
}
void dLytPauseMain_c::finalizeState_In() {
    dBase_c::s_DrawControlFlags |= dBase_c::BASE_PROP_0x1;
}

void dLytPauseMain_c::initializeState_Wait() {
    mStep = 0;
    mPartStateChangeFlags = 0;
    mIsChangingState = true;
}
void dLytPauseMain_c::executeState_Wait() {
    mIsChangingState = false;
    if (mOutRequest == true) {
        mOutRequest = false;
        mpPauseBack->requestOut();
        if (dLytControlGame_c::getInstance()->getCurrentPauseDisp() == dLytPauseMgr_c::DISP_00_INVENTORY) {
            mpDisp00->requestOut(false);
        } else {
            mpDisp01->requestOut(false);
        }
        mpPauseInfo->requestOut();
        mStateMgr.changeState(StateID_Out);
    } else if (mSelectRequest == true) {
        mStateMgr.changeState(StateID_Select);
    } else if (mChangeRequest == true) {
        mChangeRequest = false;
        mStateMgr.changeState(StateID_Change);
    } else {
        if (mRingToggleRequest == true) {
            mRingToggleRequest = false;
            mStateMgr.changeState(StateID_Ring);
        }
        mpArrow->unk();
    }
}
void dLytPauseMain_c::finalizeState_Wait() {}

void dLytPauseMain_c::initializeState_Change() {
    mStep = 0;
    if (dLytControlGame_c::getInstance()->getCurrentPauseDisp() == dLytPauseMgr_c::DISP_00_INVENTORY) {
        mpDisp01->requestIn(true);
        mpDisp00->requestOut(true);
    } else {
        mpDisp00->requestIn(true);
        mpDisp01->requestOut(true);
    }
}
void dLytPauseMain_c::executeState_Change() {
    if (mStep != 0) {
        return;
    }

    bool change = false;
    if (dLytControlGame_c::getInstance()->getCurrentPauseDisp() == dLytPauseMgr_c::DISP_00_INVENTORY) {
        if (mpDisp01->isChangingState()) {
            change = true;
        }
    } else {
        if (mpDisp00->isChangingState()) {
            change = true;
        }
    }

    if (change == true) {
        mStateMgr.changeState(StateID_Wait);
    }
}
void dLytPauseMain_c::finalizeState_Change() {}

void dLytPauseMain_c::initializeState_Select() {
    mStep = 0;
    mSelectRequest = false;
    if (dLytPauseMgr_c::GetInstance()->getField_0x0838() == true) {
        mStep = 10;
        mpDisp00->requestSelect();
        dSndSmallEffectMgr_c::GetInstance()->playSound(SE_S_MENU_P1_SELECT_TOGGLE);
    } else if (dLytPauseMgr_c::GetInstance()->getField_0x083C() == true) {
        if (mpDisp00->requestSelectGuide()) {
            mStep = 100;
        } else {
            mStep = 200;
        }
    } else if (dLytPauseMgr_c::GetInstance()->getField_0x083D() == true) {
        if (mpDisp00->requestSelectMpls()) {
            mStep = 300;
        } else {
            mStep = 200;
        }
    } else {
        mpArrow->setBackwards(false);
        mpPauseInfo->setInputInOut(false);
        if (dLytControlGame_c::getInstance()->getCurrentPauseDisp() == dLytPauseMgr_c::DISP_00_INVENTORY) {
            mpDisp00->requestSelect();
            dSndSmallEffectMgr_c::GetInstance()->playSound(SE_S_MENU_P1_SELECT_ITEM);
        } else {
            mpDisp01->requestSelect();
        }
        mpPauseText->requestIn();
    }
}

void dLytPauseMain_c::executeState_Select() {
    switch (mStep) {
        case 0: {
            if (mpPauseText->isChangingState()) {
                mStep = 1;
            }
            break;
        }
        case 1: {
            if (dPad::getDownTrigA() || dPad::getDownTrigB()) {
                mStep = 2;
                mpPauseText->requestOut();
            }
            break;
        }
        case 2: {
            if (mpPauseText->isChangingState()) {
                if (dLytControlGame_c::getInstance()->getCurrentPauseDisp() == dLytPauseMgr_c::DISP_00_INVENTORY) {
                    mpDisp00->requestUnselect();
                } else {
                    mpDisp01->requestUnselect();
                }
                mStateMgr.changeState(StateID_Wait);
            }
            break;
        }
        case 10: {
            if (mpDisp00->isChangingState() == true) {
                mStateMgr.changeState(StateID_Wait);
            }
            break;
        }
        case 100: {
            if (mpDisp00->isChangingState() == true) {
                mStateMgr.changeState(StateID_Wait);
                u8 uiMode = dLytMeter_c::GetMain()->getUiMode() + 1;
                if (uiMode >= 3) {
                    uiMode = 0;
                }
                dLytMeter_c::GetMain()->setUiMode(uiMode);
            }
            break;
        }
        case 200: {
            mStateMgr.changeState(StateID_Wait);
            break;
        }
        case 300: {
            if (mpDisp00->isChangingState() == true) {
                mStep++;
                dPadManager_c::GetInstance()->requestMplsCalibration();
            }
            break;
        }
        case 301: {
            mStateMgr.changeState(StateID_Wait);
            break;
        }
    }
}
void dLytPauseMain_c::finalizeState_Select() {
    mpArrow->setBackwards(true);
    mpPauseInfo->setInputInOut(true);
}

void dLytPauseMain_c::initializeState_Ring() {
    if (dLytControlGame_c::getInstance()->getCurrentPauseDisp() == dLytPauseMgr_c::DISP_00_INVENTORY) {
        mpDisp00->requestRingToggle();
    }
}
void dLytPauseMain_c::executeState_Ring() {
    if (mSelectRequest == true) {
        mStateMgr.changeState(StateID_Select);
    } else if (mRingToggleRequest == true) {
        mRingToggleRequest = false;
        if (dLytControlGame_c::getInstance()->getCurrentPauseDisp() == dLytPauseMgr_c::DISP_00_INVENTORY) {
            mpDisp00->requestRingToggle();
        }
        mStateMgr.changeState(StateID_Wait);
    }
}
void dLytPauseMain_c::finalizeState_Ring() {}

void dLytPauseMain_c::initializeState_GetDemo() {
    mIsChangingState = true;
}
void dLytPauseMain_c::executeState_GetDemo() {
    mIsChangingState = false;
    if (dLytControlGame_c::getInstance()->getPauseDemoDisp()) {
        if (mpDisp01->isChangingState() == true) {
            mStateMgr.changeState(StateID_Wait);
        }
    } else {
        if (mpDisp00->isChangingState() == true) {
            mStateMgr.changeState(StateID_Wait);
        }
    }
}
void dLytPauseMain_c::finalizeState_GetDemo() {}

void dLytPauseMain_c::initializeState_Out() {
    mPartStateChangeFlags = 0;
    mStep = 0;
    dBase_c::s_DrawControlFlags &= ~dBase_c::BASE_PROP_0x1;
    mpArrow->requestOut();
}
void dLytPauseMain_c::executeState_Out() {
    switch (mStep) {
        case 0: {
            if (mpPauseBack->isChangingState() == true) {
                mPartStateChangeFlags |= 0x1;
            }

            if (dLytControlGame_c::getInstance()->getCurrentPauseDisp() == dLytPauseMgr_c::DISP_00_INVENTORY) {
                if (mpDisp00->isChangingState() == true) {
                    mPartStateChangeFlags |= 0x2;
                }
            } else {
                if (mpDisp01->isChangingState() == true) {
                    mPartStateChangeFlags |= 0x2;
                }
            }

            if (mpPauseInfo->isChangingState() == true) {
                mPartStateChangeFlags |= 0x4;
            }
            if (mPartStateChangeFlags == (0x1 | 0x2 | 0x4)) {
                mIsChangingState = true;
                mStep = 1;
            }
            break;
        }
        case 1: {
            mStateMgr.changeState(StateID_None);
            break;
        }
    }
}
void dLytPauseMain_c::finalizeState_Out() {}

SPECIAL_BASE_PROFILE(LYT_PAUSE, dLytPauseMgr_c, fProfile::LYT_PAUSE, 0x2B7, 1);

dLytPauseMgr_c::dLytPauseMgr_c() : mStateMgr(*this), mpTexture0x0810(nullptr), mpBgTexture(nullptr) {
    sInstance = this;
    field_0x083A = false;
}

dLytPauseMgr_c::~dLytPauseMgr_c() {
    sInstance = nullptr;
}

int dLytPauseMgr_c::create() {
    bool ok = allocateLytWork1Heap("pause", true);
    if (!ok) {
        return FAILED;
    }

    mStateMgr.changeState(StateID_None);
    return SUCCEEDED;
}

int dLytPauseMgr_c::doDelete() {
    removeLyt();
    mMain.remove();
    mResAcc2.detach();
    mResAcc1.detach();
    if (mpTexture0x0810 != nullptr) {
        mpTexture0x0810->free();
        mpTexture0x0810 = nullptr;
    }
    if (mpBgTexture != nullptr) {
        mpBgTexture->free();
        mpBgTexture = nullptr;
    }
    return SUCCEEDED;
}

int dLytPauseMgr_c::execute() {
    mStateMgr.executeState();
    if (*mStateMgr.getStateID() != StateID_None) {
        mMain.execute();
    }
    return SUCCEEDED;
}

int dLytPauseMgr_c::draw() {
    if (*mStateMgr.getStateID() != StateID_None) {
        mMain.draw();
    }
    return SUCCEEDED;
}

void dLytPauseMgr_c::initializeState_None() {
    mInRequest = 0;
    setSelection(SELECT_NONE, 0, false);
    mPrevSelectionType = SELECT_NONE;
    mCurrentSelectionId = 0;
    mPrevSelectionId = 0;
    field_0x083C = false;
    field_0x083D = false;
    mIsActive = false;
    field_0x0837 = false;
    field_0x0838 = false;
    mIsNavLeft = false;
    field_0x0832 = false;
    field_0x083E = false;
    field_0x083F = false;
    field_0x0840 = false;
    mCurrentDisp00Tab = TAB_ITEM;
    mCurrentDisp00HeldRing = TAB_MAX;
    mCurrentSelectionTab = 0; // "-1 + 1"
    mCurrentSelectionIsRestricted = false;
    mTimer = 0;
    field_0x0835 = 0;
}
void dLytPauseMgr_c::executeState_None() {
    if (mInRequest == true) {
        mInRequest = false;
        field_0x083A = true;
        mMain.requestIn();
        mStateMgr.changeState(StateID_In);
        dBase_c::s_NextExecuteControlFlags |= BASE_PROP_0x1;
        dLytMeter_c::GetInstance()->setMeterMode(dLytMeterMain_c::MODE_PAUSE);
        dLytControlGame_c *lytControl = dLytControlGame_c::getInstance();
        if (lytControl->isPauseDemo()) {
            mSavedPauseDisp = lytControl->getCurrentPauseDisp();
            lytControl->setCurrentPauseDisp(lytControl->getPauseDemoDisp());
        } else {
            StoryflagManager::sInstance->unsetFlag(STORYFLAG_1_BTN_NOTICE);
        }
    }
}
void dLytPauseMgr_c::finalizeState_None() {}

void dLytPauseMgr_c::initializeState_In() {}
void dLytPauseMgr_c::executeState_In() {
    if (mMain.isChangingState() == true) {
        if (dLytControlGame_c::getInstance()->isPauseDemo()) {
            mStateMgr.changeState(StateID_GetDemo);
        } else {
            mStateMgr.changeState(StateID_Wait);
            dCsGame_c::GetInstance()->pointerDefSet();
        }
    }
}
void dLytPauseMgr_c::finalizeState_In() {}

void dLytPauseMgr_c::initializeState_Wait() {
    dPadNav::setNavEnabled(true, false);
    if (sInstance->getField_0x083E()) {
        sInstance->setField_0x083E(false);
        dPadNav::hidePointer();
    }

    if (field_0x083F == true) {
        field_0x083F = false;
        dPadNav::hidePointer();
    }
    field_0x0840 = false;
    field_0x0835 = 0;
}
void dLytPauseMgr_c::executeState_Wait() {
    field_0x083B = false;
    mPrevSelectionId = mCurrentSelectionId;
    mPrevSelectionType = mCurrentSelectionType;

    if (dPad::getDownTrigDown()) {
        dPad::ex_c::getInstance()->centerCursor(mPad::getCurrentCoreID(), true);
    }

    dCsGame_c::GetInstance()->pointerDefSet();

    if (dPad::getDownTrig1() ||
        (dLytControlGame_c::getInstance()->getCurrentPauseDisp() != dLytPauseMgr_c::DISP_00_INVENTORY &&
         dPad::getDownTrigB())) {
        dSndPlayerMgr_c::GetInstance()->leaveMenu();
        mMain.requestOut();
        (void)getLinkPtr(); // yes
        mStateMgr.changeState(StateID_Out);
        return;
    }

    if (dLytControlGame_c::getInstance()->getCurrentPauseDisp() == dLytPauseMgr_c::DISP_00_INVENTORY) {
        bool b = false;
        if (!StoryflagManager::sInstance->getFlag(STORYFLAG_B_WHEEL_UNLOCKED)) {
            if (dPad::getDownTrigB()) {
                b = true;
            }
        } else {
            if (dPad::getUpTrigB() && field_0x0835 < 10) {
                b = true;
            }
        }

        if (b) {
            dSndPlayerMgr_c::GetInstance()->leaveMenu();
            mMain.requestOut();
            (void)getLinkPtr(); // yes
            mStateMgr.changeState(StateID_Out);
            return;
        }
    }

    if (mMain.hasSelection() && dPad::getDownTrigA()) {
        if (field_0x0837 && !dPadNav::isPointerVisible()) {
            field_0x083F = true;
        }
        (void)checkSelectRing();
        mStateMgr.changeState(StateID_Select);
        return;
    }

    if (checkChangeDisp()) {
        mStateMgr.changeState(StateID_Change);
        return;
    }

    if ((dPad::getDownTrigB() || dPad::getDownTrigMinus() || dPad::getDownTrigC()) && checkSelectRing()) {
        field_0x0837 = true;
        mStateMgr.changeState(StateID_Select);
        return;
    }

    if (checkRing()) {
        if (mTimer >= 10) {
            mTimer = 0;
            dCsGame_c::GetInstance()->noneSet();
            mStateMgr.changeState(StateID_Ring);
        } else {
            mTimer++;
        }

    } else {
        mTimer = 0;
    }
    if (dPad::getDownB()) {
        field_0x0835++;
    } else {
        field_0x0835 = 0;
    }
}
void dLytPauseMgr_c::finalizeState_Wait() {
    dPadNav::setNavEnabled(false, false);
}

void dLytPauseMgr_c::initializeState_Change() {
    mMain.requestChange();
    mMain.updateTitle();
    if (mIsNavLeft) {
        dSndSmallEffectMgr_c::GetInstance()->playSound(SE_S_MENU_TURN_PAGE_RIGHT);
    } else {
        dSndSmallEffectMgr_c::GetInstance()->playSound(SE_S_MENU_TURN_PAGE_LEFT);
    }
}
void dLytPauseMgr_c::executeState_Change() {
    if (!sInstance->getField_0x083E()) {
        dCsGame_c::GetInstance()->pointerDefSet();
    }

    if (mMain.isChangingState() == true) {
        changeSavedDisp();
        mStateMgr.changeState(StateID_Wait);
    }
}
void dLytPauseMgr_c::finalizeState_Change() {}

void dLytPauseMgr_c::initializeState_Select() {
    mMain.requestSelect();
    field_0x0838 = field_0x0837;
}
void dLytPauseMgr_c::executeState_Select() {
    if (mMain.isChangingState() == true) {
        if (field_0x0838) {
            saveTabFlag();
            field_0x0838 = false;
        }
        mStateMgr.changeState(StateID_Wait);
    } else if (field_0x0838) {
        s32 tab = mCurrentDisp00Tab;
        bool buttonHeld = false;
        if (dPad::getDownB() && tab == TAB_ITEM) {
            buttonHeld = true;
        }
        if (dPad::getDownMinus() && tab == TAB_POUCH) {
            buttonHeld = true;
        }
        if (dPad::getDownC() && tab == TAB_DOWSING) {
            buttonHeld = true;
        }

        if (buttonHeld) {
            mTimer++;
        } else {
            mTimer = 0;
        }
    }
}
void dLytPauseMgr_c::finalizeState_Select() {}

void dLytPauseMgr_c::initializeState_Ring() {
    s32 controlGameTab = dLytControlGame_c::getInstance()->getPauseDisp00Tab();
    if (controlGameTab == TAB_ITEM) {
        mCurrentDisp00HeldRing = TAB_ITEM;
    } else if (controlGameTab == TAB_POUCH) {
        mCurrentDisp00HeldRing = TAB_POUCH;
    } else {
        mCurrentDisp00HeldRing = TAB_DOWSING;
    }
    mMain.requestRingToggle();
}
void dLytPauseMgr_c::executeState_Ring() {
    field_0x083B = false;
    mPrevSelectionId = mCurrentSelectionId;
    mPrevSelectionType = mCurrentSelectionType;
    bool buttonHeld = false;
    if (mCurrentDisp00HeldRing == TAB_ITEM) {
        if (!dPad::getDownB()) {
            buttonHeld = true;
        }
    } else if (mCurrentDisp00HeldRing == TAB_POUCH) {
        if (!dPad::getDownMinus()) {
            buttonHeld = true;
        }
    } else {
        if (!dPad::getDownC()) {
            buttonHeld = true;
        }
    }

    if (buttonHeld == true) {
        if (mMain.hasSelection()) {
            mStateMgr.changeState(StateID_Select);
        } else {
            mMain.requestRingToggle();
            mStateMgr.changeState(StateID_Wait);
        }
    }
}
void dLytPauseMgr_c::finalizeState_Ring() {
    mTimer = 0;
}

void dLytPauseMgr_c::initializeState_GetDemo() {}
void dLytPauseMgr_c::executeState_GetDemo() {
    if (mMain.isChangingState() == true) {
        dSndPlayerMgr_c::GetInstance()->leaveMenu();
        mMain.requestOut();
        (void)getLinkPtr();
        mStateMgr.changeState(StateID_Out);
    }
}
void dLytPauseMgr_c::finalizeState_GetDemo() {}

void dLytPauseMgr_c::initializeState_Out() {
    dCsGame_c::GetInstance()->offNextCursor();
}
void dLytPauseMgr_c::executeState_Out() {
    if (mMain.isChangingState() == true) {
        dBase_c::s_NextExecuteControlFlags &= ~dBase_c::BASE_PROP_0x1;
        dBase_c::s_DrawControlFlags &= ~dBase_c::BASE_PROP_0x1;
        mStateMgr.changeState(StateID_None);
    }
}
void dLytPauseMgr_c::finalizeState_Out() {
    if (dLytControlGame_c::getInstance()->isPauseDemo()) {
        dLytControlGame_c::getInstance()->setCurrentPauseDisp(mSavedPauseDisp);
    }
}

void dLytPauseMgr_c::inRequest() {
    mInRequest = true;
    mIsActive = true;
}

bool dLytPauseMgr_c::isStateWait() const {
    return mStateMgr.getStateID()->isEqual(StateID_Wait);
}

void dLytPauseMgr_c::preDrawStage() {
    if (field_0x0839 == true) {
        field_0x0839 = false;
        d2d::defaultSet();
        if (mpTexture0x0810 != nullptr) {
            mpTexture0x0810->free();
            mpTexture0x0810 = nullptr;
        }
        mMain.drawDirectly();
        mpTexture0x0810 = EGG::TextureBuffer::alloc(
            mVideo::m_video->pRenderMode->fbWidth / 2, dGfx_c::getEFBHeight() / 2, GX_TF_RGBA8
        );
        mpTexture0x0810->capture(0, dGfx_c::getLetterboxAmount(), true, -1);
        mMain.loadBgTextureUnused();
    }
}

void dLytPauseMgr_c::postDrawStage() {
    if (field_0x083A == true) {
        field_0x083A = false;
        d2d::defaultSet();
        if (dLytControlGame_c::getInstance() != nullptr) {
            dLytControlGame_c::getInstance()->fn_802D04F0();
        }
        mpBgTexture = EGG::TextureBuffer::alloc(
            mVideo::m_video->pRenderMode->fbWidth / 2, dGfx_c::getEFBHeight() / 2, GX_TF_RGBA8
        );
        mpBgTexture->capture(0, dGfx_c::getLetterboxAmount(), true, -1);
        mMain.loadBgTexture();
    }
}

nw4r::lyt::Bounding *dLytPauseMgr_c::getArrowBounding(int idx) const {
    return mMain.getArrow()->getArrowBounding(idx);
}

void dLytPauseMgr_c::setSelectedArrowBounding(int idx) {
    mMain.getArrow()->setField_0x6B8(idx);
}

void dLytPauseMgr_c::setSelection(dLytPauseMgr_c::SelectionType_e type, u16 id, bool restricted) {
    if (type != mPrevSelectionType || id != getCurrentSelectionId()) {
        field_0x083B = true;
    } else {
        field_0x083B = false;
    }
    mCurrentSelectionType = type;
    mCurrentSelectionId = id;
    mCurrentSelectionIsRestricted = restricted;
}

bool dLytPauseMgr_c::checkSelectRing() {
    dLytControlGame_c *lytControl = dLytControlGame_c::getInstance();
    if (lytControl == nullptr) {
        mCurrentDisp00Tab = TAB_ITEM;
        return false;
    }

    if (lytControl->getCurrentPauseDisp() != DISP_00_INVENTORY) {
        return false;
    }

    s32 tab = lytControl->getPauseDisp00Tab();

    if (dPad::getDownTrigB()) {
        mCurrentDisp00Tab = TAB_ITEM;
        if (tab != TAB_ITEM && StoryflagManager::sInstance->getFlag(STORYFLAG_B_WHEEL_UNLOCKED)) {
            return true;
        } else {
            return false;
        }
    } else if (dPad::getDownTrigMinus()) {
        mCurrentDisp00Tab = TAB_POUCH;
        if (tab != TAB_POUCH && StoryflagManager::sInstance->getFlag(STORYFLAG_POUCH_UNLOCKED)) {
            return true;
        } else {
            return false;
        }
    } else if (dPad::getDownTrigC()) {
        mCurrentDisp00Tab = TAB_DOWSING;
        if (tab != TAB_DOWSING && StoryflagManager::sInstance->getFlag(STORYFLAG_DOWSING_UNLOCKED)) {
            return true;
        } else {
            return false;
        }
    }

    mCurrentDisp00Tab = mCurrentSelectionTab - 1;
    return true;
}

bool dLytPauseMgr_c::build() {
    void *data;
    data = LayoutArcManager::GetInstance()->getLoadedData("MenuPause");
    mResAcc1.attach(data, "");
    data = LayoutArcManager::GetInstance()->getLoadedData("DoButton");
    mResAcc2.attach(data, "");
    mMain.build();
    return true;
}

bool dLytPauseMgr_c::checkChangeDisp() {
    bool nav = false;
    if (dPadNav::isMplsNavGesture() == true) {
        if (dPadNav::isMplsNavLeftGesture()) {
            mIsNavLeft = true;
        } else {
            mIsNavLeft = false;
        }
        nav = true;
    }

    if (dPad::getDownTrigLeft() | dPad::getDownTrigRight()) {
        if (dPad::getDownTrigLeft()) {
            mIsNavLeft = true;
        } else {
            mIsNavLeft = false;
        }
        nav = true;
    }
    s32 arrowPane = mMain.getArrow()->getField_0x6B8();
    if (arrowPane != dLytCommonArrow_c::ARROW_NONE && dPad::getDownTrigA() && mMain.getArrow()->fn_80168760()) {
        if (arrowPane == dLytCommonArrow_c::ARROW_LEFT) {
            mIsNavLeft = true;
        } else {
            mIsNavLeft = false;
        }

        if (!dPadNav::isPointerVisible()) {
            field_0x083E = true;
            field_0x0832 = mIsNavLeft;
        } else {
            field_0x0840 = true;
        }
        nav = true;
    }
    return nav;
}

void dLytPauseMgr_c::changeSavedDisp() {
    dLytControlGame_c *lytControl = dLytControlGame_c::getInstance();
    if (lytControl == nullptr) {
        return;
    }
    s32 disp = lytControl->getCurrentPauseDisp() + 1;
    if (disp >= DISP_MAX) {
        disp = 0;
    }
    lytControl->setCurrentPauseDisp(disp);
    if (disp != 0) {
        StoryflagManager::sInstance->setFlag(STORYFLAG_ON_COLLECTION_SCREEN);
    } else {
        StoryflagManager::sInstance->unsetFlag(STORYFLAG_ON_COLLECTION_SCREEN);
    }
}

void dLytPauseMgr_c::saveTabFlag() {
    dLytControlGame_c::getInstance()->setPauseDisp00Tab(mCurrentDisp00Tab);
    StoryflagManager::sInstance->setFlagOrCounterToValue(STORYFLAG_SAVED_DISP00_TAB, mCurrentDisp00Tab);
}

bool dLytPauseMgr_c::checkRing() {
    dLytControlGame_c *lytControl = dLytControlGame_c::getInstance();
    if (lytControl->getCurrentPauseDisp() != DISP_00_INVENTORY) {
        return false;
    }
    if (!StoryflagManager::sInstance->getFlag(STORYFLAG_POUCH_UNLOCKED)) {
        return false;
    }

    s32 tab = lytControl->getPauseDisp00Tab();

    if (dPad::getDownB() && tab == TAB_ITEM) {
        return true;
    } else if (dPad::getDownMinus() && tab == TAB_POUCH) {
        return true;
    } else if (dPad::getDownC() && tab == TAB_DOWSING) {
        return true;
    }
    return false;
}

void dLytPauseMgr_c::setSelectHand(f32 rotation, f32 length) {
    sDisp00ArrowRotation = rotation;
    sDisp00ArrowLength = length;
}
