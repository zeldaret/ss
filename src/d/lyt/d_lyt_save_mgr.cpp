#include "d/lyt/d_lyt_save_mgr.h"

#include "common.h"
#include "d/a/d_a_player.h"
#include "d/d_base.h"
#include "d/d_player.h"
#include "d/d_sc_game.h"
#include "d/d_sc_title.h"
#include "d/d_stage.h"
#include "d/flag/sceneflag_manager.h"
#include "d/flag/storyflag_manager.h"
#include "d/lyt/d_lyt_save_msg_window.h"
#include "toBeSorted/file_manager.h"
#include "toBeSorted/save_manager.h"

SPECIAL_BASE_PROFILE(LYT_SAVE_MGR, dLytSaveMgr_c, fProfile::LYT_SAVE_MGR, 0x2AD, 1);

STATE_DEFINE(dLytSaveMgr_c, Init);
STATE_DEFINE(dLytSaveMgr_c, FadeOut);
STATE_DEFINE(dLytSaveMgr_c, Select);
STATE_DEFINE(dLytSaveMgr_c, FadeIn);

dLytSaveMgr_c *dLytSaveMgr_c::sInstance;

dLytSaveMgr_c::dLytSaveMgr_c() : mStateMgr(*this) {
    sInstance = this;
    init();
}

dLytSaveMgr_c::~dLytSaveMgr_c() {
    sInstance = nullptr;
}

int dLytSaveMgr_c::create() {
    mWindow.build(true);
    mStateMgr.changeState(StateID_Init);
    return SUCCEEDED;
}

int dLytSaveMgr_c::doDelete() {
    mWindow.remove();
    return SUCCEEDED;
}

int dLytSaveMgr_c::execute() {
    mStateMgr.executeState();
    return SUCCEEDED;
}

int dLytSaveMgr_c::draw() {
    mWindow.draw();
    return SUCCEEDED;
}

bool dLytSaveMgr_c::setPrompt(s32 prompt, bool setTempflag2) {
    mSavePrompt = prompt;
    mShouldSetTempflag2 = setTempflag2;
    mFadeOutRequested = true;
    return true;
}

bool dLytSaveMgr_c::setSaveobjPrompt(s32 prompt, u8 saveobjId, bool setTempflag2) {
    mSavePrompt = prompt;
    mSaveobjId = saveobjId;
    mShouldSetTempflag2 = setTempflag2;
    mFadeOutRequested = true;
    return true;
}

bool dLytSaveMgr_c::unused_weakfnorder() {
    return mStateMgr.getStateID()->isEqual(StateID_Init);
}

bool dLytSaveMgr_c::isBusy() const {
    return !mStateMgr.getStateID()->isEqual(StateID_Init);
}

void dLytSaveMgr_c::initializeState_Init() {
    mStep = 0;
    mFadeOutRequested = false;
    mIsFinishedFadingIn = false;
}
void dLytSaveMgr_c::executeState_Init() {
    if (mFadeOutRequested == true) {
        mStateMgr.changeState(StateID_FadeOut);
    }
}
void dLytSaveMgr_c::finalizeState_Init() {}

void dLytSaveMgr_c::initializeState_FadeOut() {
    if (mSavePrompt == PROMPT_HERO_MODE) {
        StoryflagManager::sInstance->setFlag(STORYFLAG_490);
        if (StoryflagManager::sInstance->getFlag(STORYFLAG_HERO_MODE)) {
            // Skip hero mode prompt if we were in hero mode
            mSavePrompt = PROMPT_HERO_MODE_CONFIRM;
        }
    }
}
void dLytSaveMgr_c::executeState_FadeOut() {
    static const char *sFadeOutTexts[] = {
        "SYS_SAVE_03", // "[1]Save[2]Save and quit[3-]Cancel"
        "SYS_SAVE_05", // "[1]Save[2]Save and quit[3]To the sky[4-]Cancel"
        "SYS_SAVE_07", // "[1]Save[2]Save and quit[3]Go outside[4-]Cancel"
        "SYS_SAVE_09", // "Do you want to save your progress? [1]Save[2]Save and quit[3-]Cancel"
        "SYS_SAVE_16", // "You can play again in Hero Mode using this save data. Do you want to continue? [1]Yes[2-]No"
        "SYS_SAVE_14", // "Your progress up to this point will be deleted. Are you sure you want to play Hero Mode?
                       // [1]Bring it on[2-]No, thanks"
    };

    static const s32 sFadeOutModes[] = {
        dLytSaveMsgWindow_c::MODE_3_BUTTONS, dLytSaveMsgWindow_c::MODE_4_BUTTONS, dLytSaveMsgWindow_c::MODE_4_BUTTONS,
        dLytSaveMsgWindow_c::MODE_3_BUTTONS, dLytSaveMsgWindow_c::MODE_2_BUTTONS, dLytSaveMsgWindow_c::MODE_2_BUTTONS,
    };

    if (mWindow.setProperties(sFadeOutTexts[mSavePrompt], sFadeOutModes[mSavePrompt], 0, 1)) {
        mWindow.setField0x10DC(1);
        mWindow.setField0x10B8(0);
        mStateMgr.changeState(StateID_Select);

        if (mSavePrompt < PROMPT_SAVEOBJ_NUM) {
            mWindow.setSaveObjId(mSaveobjId);
        }
    }

    if (mSavePrompt == PROMPT_SAVE_NORMAL) {
        dBase_c::s_NextExecuteControlFlags |= 1;
        dBase_c::s_DrawControlFlags |= 1;
    }
}
void dLytSaveMgr_c::finalizeState_FadeOut() {}

void dLytSaveMgr_c::initializeState_Select() {
    mStep = 0;
    if (mSavePrompt == PROMPT_HERO_MODE) {
        mStep = 200;
    } else if (mSavePrompt == PROMPT_HERO_MODE_CONFIRM) {
        mStep = 300;
        StoryflagManager::sInstance->setFlag(STORYFLAG_HEROS_TUNIC_FI);
    }
}
void dLytSaveMgr_c::executeState_Select() {
    static const char *sConfirmTexts[] = {
        "SYS_SAVE_04", // "Are you sure you want to quit? [1]Yes[2-]No"
        "SYS_SAVE_06", // "Do you want to return to the sky? [1]Yes[2-]No"
        "SYS_SAVE_08", // "Leave the area? [1]Leave[2-]Stay"
        "SYS_SAVE_04", // "Are you sure you want to quit? [1]Yes[2-]No"
        "SYS_SAVE_17", // "Your progress up to this point will be deleted. Are you sure you want to play Hero Mode?
                       // [1]Bring it on[2-]No, thanks"
        "SYS_SAVE_15", // "Are you sure you want to return to the title screen without saving? [1]Don't save[2-]Save"
    };

    mWindow.execute();
    switch (mStep) {
        case 0: {
            if (mWindow.getField0x10C8() == 0 || mWindow.getField0x10C8() == 1) {
                SaveMgr::GetInstance()->setField_0x83D(true);
            }
            s32 confirmedButton = mWindow.getField0x10C4();
            if (confirmedButton >= 0) {
                mWindow.checkIsWait();
                mStep = 1;
                if (confirmedButton == 0 || confirmedButton == 1) {
                    mWindow.setField0x10BC(1);
                } else if (mSavePrompt == PROMPT_SAVEOBJ_TOSKY || mSavePrompt == PROMPT_SAVEOBJ_DUNGEON) {
                    if (confirmedButton == 2) {
                        mWindow.setField0x10BC(1);
                    } else {
                        mWindow.setField0x10BC(0);
                    }
                } else {
                    mWindow.setField0x10BC(0);
                }
            }
            break;
        }
        case 1: {
            if (mWindow.getWillFinishOut() == true) {
                s32 confirmedButton = mWindow.getField0x10C4();
                if (confirmedButton == 0) {
                    mChosenAction = ACTION_0;
                    mStep = 20;
                    if (mShouldSetTempflag2 == true) {
                        mShouldSetTempflag2 = false;
                        if (FileManager::GetInstance()->get_0xA84C() == 0) {
                            SceneflagManager::sInstance->setTempflag_i(dStage_c::GetInstance()->getCurrRoomId(), 0x2);
                        }
                    }
                } else if (confirmedButton == 1) {
                    mChosenAction = ACTION_3;
                    mStep = 20;
                } else if (mSavePrompt == PROMPT_SAVEOBJ_NORMAL || mSavePrompt == PROMPT_SAVE_NORMAL) {
                    mChosenAction = ACTION_0;
                    mStep = 100;
                } else if (mSavePrompt == PROMPT_SAVEOBJ_TOSKY) {
                    if (confirmedButton == 2) {
                        mStep = 10;
                        mChosenAction = ACTION_1;
                    } else {
                        mChosenAction = ACTION_0;
                        mStep = 100;
                    }
                } else {
                    if (confirmedButton == 2) {
                        mChosenAction = ACTION_1;
                        mStep = 10;
                    } else {
                        mChosenAction = ACTION_0;
                        mStep = 100;
                    }
                }

                if (mStep == 20 &&
                    (FileManager::GetInstance()->get_0xA84C() != 0 || !FileManager::GetInstance()->checkRegionCode())) {
                    mStep = 30;
                    SaveMgr::GetInstance()->setField_0x83D(false);
                }
            }
            break;
        }
        case 10: {
            if (mWindow.setProperties(sConfirmTexts[mSavePrompt], dLytSaveMsgWindow_c::MODE_2_BUTTONS, 0, 1)) {
                mWindow.setField0x10B8(1);
                mStep = 11;
                break;
            }
            // fall-through
        }
        case 11: {
            if (mWindow.getField0x10C4() >= 0) {
                mWindow.checkIsWait();
                mWindow.setField0x10BC(0);
                mStep = 12;
            }
            break;
        }
        case 12: {
            if (mWindow.getWillFinishOut() == true) {
                if (mWindow.getField0x10C4() == 0) {
                    FileManager::GetInstance()->setPlayerInfoFileA();
                } else {
                    mChosenAction = ACTION_0;
                }
                mStep = 100;
            }

            break;
        }
        case 20: {
            bool b = false;
            if (mSavePrompt == PROMPT_SAVE_NORMAL) {
                b = true;
            }
            if (mSavePrompt != PROMPT_SAVE_NORMAL && mSavePrompt != PROMPT_HERO_MODE) {
                dAcPy_c *link = getLinkPtr();
                if (link != nullptr && !dScGame_c::isCurrentStage("F202")) {
                    // not Boko Base
                    FileManager::GetInstance()->setT3Info(link->mPosition, link->mAngle);
                    StoryflagManager::sInstance->setFlag(STORYFLAG_RESPAWN_AT_STATUE);
                }
            }
            FileManager::GetInstance()->fn_8000CBD0(b);
            mStep = 29;
            break;
        }
        case 29: {
            bool t1Flag = false;
            if (mSavePrompt == PROMPT_SAVE_NORMAL) {
                t1Flag = true;
            }
            if (SaveMgr::GetInstance()->save(t1Flag, true) == true) {
                mStep = 21;
            }
            break;
        }
        case 21: {
            if (SaveMgr::GetInstance()->isIdle() == true) {
                if (mChosenAction == ACTION_3) {
                    mStep = 22;
                } else {
                    mStep = 40;
                }
            }
            break;
        }
        case 22: {
            if (mWindow.setProperties(
                    sConfirmTexts[PROMPT_SAVEOBJ_NORMAL], dLytSaveMsgWindow_c::MODE_2_BUTTONS, 0, 1
                )) {
                mWindow.setField0x10B8(1);
                mStep = 23;
            }
            break;
        }
        case 23: {
            s32 confirmedButton = mWindow.getField0x10C4();
            if (confirmedButton >= 0) {
                if (confirmedButton != 0) {
                    mWindow.checkIsWait();
                    mStep = 24;
                    mWindow.setField0x10BC(0);
                } else {
                    mStep = 25;
                }
            }
            break;
        }
        case 24: {
            if (mWindow.getWillFinishOut() == true) {
                mChosenAction = ACTION_0;
                mStep = 100;
            }
            break;
        }
        case 25: {
            dScTitle_c::loadTitleScreen(1);
            mStep = 1000;
            break;
        }
        case 30: {
            // "It is not possible to save. Continue your adventure anyway? [1]Continue[2]Quit"
            if (mWindow.setProperties("SYS_SAVE_11", dLytSaveMsgWindow_c::MODE_2_BUTTONS, 0, 1)) {
                mWindow.setField0x10B8(1);
                mWindow.setNumLines(2);
                mStep++;
            }
            break;
        }
        case 31: {
            s32 confirmedButton = mWindow.getField0x10C4();
            if (confirmedButton >= 0) {
                mWindow.checkIsWait();
                if (confirmedButton != 0) {
                    mWindow.setField0x10BC(1);
                } else {
                    mWindow.setField0x10BC(0);
                }
                mStep++;
            }
            break;
        }
        case 32: {
            if (mWindow.getWillFinishOut() == true) {
                if (mWindow.getField0x10C4()) {
                    mStep = 50;
                } else {
                    mChosenAction = ACTION_0;
                    mStep = 100;
                }
            }
            break;
        }
        case 40:
            if (mWindow.checkIsWait()) {
                mWindow.setField0x10BC(2);
                mStep++;
            }
            break;
        case 41:
            if (mWindow.getWillFinishOut() == true) {
                mStep = 100;
            }
            break;
        case 50:
            // "Are you sure you want to quit? [1]Quit[2-]Cancel"
            if (mWindow.setProperties("SYS_GAMEOVER_03", dLytSaveMsgWindow_c::MODE_2_BUTTONS, 0, 1)) {
                mWindow.setField0x10B8(1);
                mStep++;
            }
            break;
        case 51:
            if (mWindow.getField0x10C4() >= 0) {
                mWindow.checkIsWait();
                mWindow.setField0x10BC(0);
                mStep++;
            }
            break;
        case 52:
            if (mWindow.getWillFinishOut() == true) {
                if (mWindow.getField0x10C4() == 0) {
                    dScTitle_c::loadTitleScreen(0);
                    mStep = 1000;
                } else {
                    mChosenAction = ACTION_0;
                    mStep = 100;
                }
            }
            break;
        case 60:
            if (mWindow.setProperties(sConfirmTexts[mSavePrompt], dLytSaveMsgWindow_c::MODE_3_BUTTONS, 0, 1)) {
                mWindow.setField0x10B8(1);
                mStep = 61;
            }
            break;
        case 61:
            if (mWindow.getField0x10C4() >= 0) {
                mWindow.checkIsWait();
                mWindow.setField0x10BC(0);
                mStep = 62;
            }
            break;
        case 62:
            if (mWindow.getWillFinishOut() == true) {
                s32 confirmedButton = mWindow.getField0x10C4();
                if (confirmedButton == 0) {
                    mChosenAction = ACTION_1;
                    FileManager::GetInstance()->setPlayerInfoFileA();
                } else if (confirmedButton == 1) {
                    mChosenAction = ACTION_2;
                    FileManager::GetInstance()->setPlayerInfoFileA();
                } else {
                    mChosenAction = ACTION_0;
                }
                mStep = 100;
            }
            break;
        case 100: {
            mStateMgr.changeState(StateID_FadeIn);
            break;
        }
        case 200:
            if (mWindow.getField0x10C4() >= 0) {
                mWindow.checkIsWait();
                mWindow.setField0x10BC(1);
                mStep++;
            }
            break;
        case 201:
            if (mWindow.getWillFinishOut() == true) {
                if (mWindow.getField0x10C4() != 0) {
                    mStep = 230;
                } else {
                    mStep = 210;
                }
            }
            break;
        case 210:
            // "Your progress up to this point will be deleted. Are you sure you want to play Hero Mode? [1]Bring it on[2-]No, thanks"
            if (mWindow.setProperties("SYS_SAVE_17", dLytSaveMsgWindow_c::MODE_2_BUTTONS, 0, 1)) {
                mWindow.setField0x10B8(1);
                mStep++;
            }
            break;
        case 211:
            if (mWindow.getField0x10C8() == 0) {
                SaveMgr::GetInstance()->setField_0x83D(true);
            }
            if (mWindow.getField0x10C4() >= 0) {
                mWindow.checkIsWait();
                mWindow.setField0x10BC(1);
                mStep++;
            }
            break;
        case 212:
            if (mWindow.getWillFinishOut() == true) {
                if (mWindow.getField0x10C4() != 0) {
                    mWindow.setField0x10BC(1);
                    mStep = 230;
                } else {
                    mWindow.setField0x10BC(1);
                    mStep++;
                }
            }
            break;
        case 213:
            FileManager::GetInstance()->fn_8000CBD0(true);
            if (SaveMgr::GetInstance()->saveAfterCredits() == true) {
                mStep++;
            }
            break;
        case 214:
            if (SaveMgr::GetInstance()->isIdle() == true) {
                mStep = 40;
            }
            break;
        case 230:
            // "Are you sure you want to return to the title screen without saving? [1]Don't save[2-]Save"
            if (mWindow.setProperties("SYS_SAVE_15", dLytSaveMsgWindow_c::MODE_2_BUTTONS, 0, 1)) {
                mWindow.setField0x10B8(1);
                mStep++;
            }
            break;
        case 231: {
            s32 tmp = mWindow.getField0x10C4();
            if (tmp >= 0) {
                mWindow.checkIsWait();
                if (tmp != 0) {
                    mWindow.setField0x10BC(1);
                } else {
                    mWindow.setField0x10BC(0);
                }
                mStep++;
            }
            break;
        }
        case 232:
            if (mWindow.getWillFinishOut() == true) {
                if (mWindow.getField0x10C4() != 0) {
                    mStep++;
                } else {
                    mStep = 100;
                }
            }
            break;
        case 233:
            // "You can play again in Hero Mode using this save data. Do you want to continue? [1]Yes[2-]No"
            if (mWindow.setProperties("SYS_SAVE_16", dLytSaveMsgWindow_c::MODE_2_BUTTONS, 0, 1)) {
                mWindow.setField0x10B8(1);
                mStep = 200;
            }
            break;
        case 300:
            if (mWindow.getField0x10C8() == 0) {
                SaveMgr::GetInstance()->setField_0x83D(true);
            }
            if (mWindow.getField0x10C4() >= 0) {
                mWindow.checkIsWait();
                mWindow.setField0x10BC(1);
                mStep++;
            }
            break;
        case 301:
            if (mWindow.getWillFinishOut() == true) {
                if (mWindow.getField0x10C4() != 0) {
                    mStep = 320;
                } else if (FileManager::GetInstance()->get_0xA84C()) {
                    mStep = 330;
                    SaveMgr::GetInstance()->setField_0x83D(false);
                } else {
                    mStep = 310;
                }
            }
            break;
        case 310:
            FileManager::GetInstance()->fn_8000CBD0(true);
            if (SaveMgr::GetInstance()->saveAfterCredits() == true) {
                mStep++;
            }
            break;
        case 311:
            if (SaveMgr::GetInstance()->isIdle() == true) {
                mStep = 40;
            }
            break;
        case 320:
            if (mWindow.setProperties(sConfirmTexts[mSavePrompt], dLytSaveMsgWindow_c::MODE_2_BUTTONS, 0, 1)) {
                mWindow.setField0x10B8(1);
                mStep++;
            }
            break;
        case 321: {
            s32 tmp = mWindow.getField0x10C4();
            if (tmp >= 0) {
                mWindow.checkIsWait();
                if (tmp != 0) {
                    mWindow.setField0x10BC(1);
                } else {
                    mWindow.setField0x10BC(0);
                }
                mStep++;
            }
            break;
        }
        case 322:
            if (mWindow.getWillFinishOut() == true) {
                if (mWindow.getField0x10C4() != 0) {
                    mStep++;
                } else {
                    mStep = 100;
                }
            }
            break;
        case 323:
            // "You have conquered Hero Mode! Congratulations! Do you want to create a save just before the final
            // battle? [1]Save[2-]Don't save"
            if (mWindow.setProperties("SYS_SAVE_14", dLytSaveMsgWindow_c::MODE_2_BUTTONS, 0, 1)) {
                mWindow.setField0x10B8(1);
                mStep = 300;
            }
            break;
        case 330:
            // "It is not possible to save. Continue your adventure anyway? [1]Continue[2]Quit"
            if (mWindow.setProperties("SYS_SAVE_11", dLytSaveMsgWindow_c::MODE_2_BUTTONS, 0, 1)) {
                mWindow.setField0x10B8(1);
                mWindow.setNumLines(2);
                mStep++;
            }
            break;
        case 331:
            if (mWindow.getField0x10C4() >= 0) {
                mWindow.checkIsWait();
                mWindow.setField0x10BC(0);
                mStep++;
            }
            break;
        case 332:
            if (mWindow.getWillFinishOut() == true) {
                mStep = 100;
            }
            break;
    }
}
void dLytSaveMgr_c::finalizeState_Select() {}

void dLytSaveMgr_c::initializeState_FadeIn() {
    mStep = 0;
    if (mSavePrompt == PROMPT_SAVE_NORMAL) {
        dBase_c::s_NextExecuteControlFlags &= ~1;
        dBase_c::s_DrawControlFlags &= ~1;
    }
}
void dLytSaveMgr_c::executeState_FadeIn() {
    switch (mStep) {
        case 0: {
            mIsFinishedFadingIn = true;
            mStep = 1;
            break;
        }
        case 1: {
            mStateMgr.changeState(StateID_Init);
            break;
        }
    }
}
void dLytSaveMgr_c::finalizeState_FadeIn() {}

void dLytSaveMgr_c::init() {
    mStep = 0;
    mChosenAction = ACTION_MAX;
    mSavePrompt = PROMPT_MAX;
    mFadeOutRequested = false;
    mIsFinishedFadingIn = false;
    mSaveobjId = 0;
}
