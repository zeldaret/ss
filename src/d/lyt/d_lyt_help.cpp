#include "d/lyt/d_lyt_help.h"

#include "common.h"
#include "d/d_d2d.h"
#include "d/d_pad.h"
#include "d/lyt/meter/d_lyt_meter.h"
#include "d/snd/d_snd_player_mgr.h"
#include "m/m2d.h"
#include "sized_string.h"
#include "toBeSorted/arc_managers/layout_arc_manager.h"

STATE_DEFINE(dLytHelpMain_c, ModeNone);
STATE_DEFINE(dLytHelpMain_c, ModeIn);
STATE_DEFINE(dLytHelpMain_c, ModeWait);
STATE_DEFINE(dLytHelpMain_c, ModeOut);
STATE_DEFINE(dLytHelpMain_c, ModeEnd);

STATE_DEFINE(dLytHelp_c, None);
STATE_DEFINE(dLytHelp_c, In);
STATE_DEFINE(dLytHelp_c, Wait);
STATE_DEFINE(dLytHelp_c, Out);
STATE_DEFINE(dLytHelp_c, End);

static const d2d::LytBrlanMapping brlanMap[] = {
    {    "helpMode_02_in.brlan",     "G_inOut"},
    {   "helpMode_02_out.brlan",     "G_inOut"},
    {"helpMode_02_loopBg.brlan", "G_loopBg_00"},
};

#define HELP_MAIN_ANIM_IN 0
#define HELP_MAIN_ANIM_OUT 1
#define HELP_MAIN_ANIM_LOOP_BG 2

#define HELP_MAIN_NUM_ANIMS 3

static const char *sTextBoxNamesTitleS[] = {
    "T_orExTitleS_00",
    "T_orExTitleS_01",
    "T_orExTitleS_02",
};

static const char *sTextBoxNamesTitle[] = {
    "T_orExTitle_00",
    "T_orExTitle_01",
    "T_orExTitle_02",
};

static const char *sTextBoxNamesS[] = {
    "T_orExS_00",
    "T_orExS_01",
    "T_orExS_02",
};

static const char *sTextBoxNames[] = {
    "T_orEx_00",
    "T_orEx_01",
    "T_orEx_02",
};

#define HELP_ANIM_NUM_PANES 3

dLytHelp_c *dLytHelp_c::sInstance;

void dLytHelpMain_c::initializeState_ModeNone() {}
void dLytHelpMain_c::executeState_ModeNone() {
    mIsIdle = true;
}
void dLytHelpMain_c::finalizeState_ModeNone() {}

void dLytHelpMain_c::initializeState_ModeIn() {}
void dLytHelpMain_c::executeState_ModeIn() {
    if (mAnm[HELP_MAIN_ANIM_IN].isEndReached()) {
        mAnm[HELP_MAIN_ANIM_IN].setAnimEnable(false);
        mIsIdle = true;
    }
}
void dLytHelpMain_c::finalizeState_ModeIn() {}

void dLytHelpMain_c::initializeState_ModeWait() {}
void dLytHelpMain_c::executeState_ModeWait() {
    mIsIdle = true;
}
void dLytHelpMain_c::finalizeState_ModeWait() {}

void dLytHelpMain_c::initializeState_ModeOut() {}
void dLytHelpMain_c::executeState_ModeOut() {
    if (field_0x1CC == 0) {
        if (mAnm[HELP_MAIN_ANIM_OUT].isEndReached()) {
            mAnm[HELP_MAIN_ANIM_LOOP_BG].setAnimEnable(false);
            mAnm[HELP_MAIN_ANIM_OUT].setAnimEnable(false);
            field_0x1CC = 1;
        }
    } else {
        mIsIdle = true;
    }
}
void dLytHelpMain_c::finalizeState_ModeOut() {}

void dLytHelpMain_c::initializeState_ModeEnd() {}
void dLytHelpMain_c::executeState_ModeEnd() {
    mIsIdle = true;
}
void dLytHelpMain_c::finalizeState_ModeEnd() {}

dLytHelpMain_c::dLytHelpMain_c() : mStateMgr(*this) {}

bool dLytHelpMain_c::build(d2d::ResAccIf_c *resAcc) {
    mLyt.setResAcc(resAcc);
    mLyt.build("helpMode_02.brlyt", nullptr);
    mLyt.setPriority(0x8A);

    for (int i = 0; i < HELP_MAIN_NUM_ANIMS; i++) {
        mAnm[i].init(brlanMap[i].mFile, resAcc, mLyt.getLayout(), brlanMap[i].mName);
        mAnm[i].bind(false);
        mAnm[i].setAnimEnable(false);
    }

    for (int i = 0; i < HELP_ANIM_NUM_PANES; i++) {
        mpTextBoxesTitleS[i] = mLyt.getTextBox(sTextBoxNamesTitleS[i]);
        mpTextBoxesTitle[i] = mLyt.getTextBox(sTextBoxNamesTitle[i]);
        mpTextBoxesS[i] = mLyt.getTextBox(sTextBoxNamesS[i]);
        mpTextBoxes[i] = mLyt.getTextBox(sTextBoxNames[i]);
    }

    mAnm[HELP_MAIN_ANIM_IN].setAnimEnable(true);
    mAnm[HELP_MAIN_ANIM_IN].setFrame(0.0f);
    mAnm[HELP_MAIN_ANIM_LOOP_BG].setAnimEnable(true);
    mAnm[HELP_MAIN_ANIM_LOOP_BG].setFrame(0.0f);

    mLyt.calc();

    mAnm[HELP_MAIN_ANIM_IN].setAnimEnable(false);
    mAnm[HELP_MAIN_ANIM_OUT].setAnimEnable(false);

    modeRequestNone();

    field_0x1C8 = 0;
    field_0x1CC = 0;
    return true;
}

bool dLytHelpMain_c::remove() {
    mLyt.unbindAnims();
    for (int i = 0; i < HELP_MAIN_NUM_ANIMS; i++) {
        mAnm[i].remove();
    }
    return true;
}

void dLytHelpMain_c::modeRequestIn(s32 textIndex) {
    mTextIndex = textIndex;
    setText(textIndex);

    mAnm[HELP_MAIN_ANIM_IN].setAnimEnable(true);
    mAnm[HELP_MAIN_ANIM_IN].setFrame(0.0f);
    mLyt.calc();
    mStateMgr.changeState(StateID_ModeIn);
    mIsIdle = false;
}

void dLytHelpMain_c::modeRequestWait() {
    mAnm[HELP_MAIN_ANIM_IN].setAnimEnable(false);
    mStateMgr.changeState(StateID_ModeWait);
    mIsIdle = false;
}

void dLytHelpMain_c::modeRequestOut() {
    mAnm[HELP_MAIN_ANIM_OUT].setAnimEnable(true);
    mAnm[HELP_MAIN_ANIM_OUT].setFrame(0.0f);
    mLyt.calc();
    field_0x1CC = 0;
    mStateMgr.changeState(StateID_ModeOut);
    mIsIdle = false;
}

void dLytHelpMain_c::modeRequestEnd() {
    mStateMgr.changeState(StateID_ModeEnd);
    mIsIdle = false;
}

void dLytHelpMain_c::modeRequestNone() {
    mStateMgr.changeState(StateID_ModeNone);
    mIsIdle = false;
}

static const char *sHelpTextKeys[] = {
    "NORMAL",    "NORMAL_Z",    "SWORD",         "SWORD_LV1", "SWORD_LV1", "DOWSING",     "SCOOP_LV2",
    "SWIM_LV0",  "SWIM_LV1",    "PACHINKO",      "PACHINKO",  "BEATLE",    "BEATLE_HOOK", "BEATLE_HOOK",
    "BOMB_BAG",  "VACUUM",      "BUTTERFLY_NET", "BOW",       "BOW",       "WHIP",        "HOOK",
    "PICK_UP",   "HARP",        "SHIELD",        "DIVING",    "DIVING",    "PARASHAWL",   "BIRD",
    "BIRD",      "BOTTLE",      "LOOK",          "LOOK",      "CLIMB",     "TIGHTROPE",   "HANG_TIGHTROPE",
    "SWING",     "SWING",       "SWING",         "CRAWLING",  "NORMAL",    "BOSSKEY",     "TROLLEY",
    "BOAT",      "BALANCEBALL", "SLIDER",        "DRAWING",   "PANEL",     "SIT",         "CATAPULT",
    "LADDER",    "BIGSWORD",    "GISIKI",        "PUMPKIN",   "SIREN",     "MINI_DIVING", "MINI_MINE",
    "MINI_TAKE", "MINI_INSECT", "MINI_CLAY",
};

// TODO - fakematch
#pragma push
#pragma ppc_iro_level 2

void dLytHelpMain_c::setText(s32 textIndex) {
    if (textIndex >= 59) {
        return;
    }

    SizedString<64> label;

    if (mTextIndex != textIndex) {
        mTextIndex = textIndex;
    }

    for (int i = 0; i < 3; i++) {
        s32 textSuffix = i + 1;
        bool bUseCaption2 = false;
        bool bUseTitle2 = false;
        switch (mTextIndex) {
            case 4: {
                // SWORD_LV1 dupe
                if (i == 2) {
                    bUseCaption2 = true;
                }
                break;
            }
            case 36: {
                // SWING dupe 1
                if (i == 0) {
                    bUseCaption2 = true;
                } else if (i == 2) {
                    textSuffix = 4;
                }
                break;
            }
            case 37: {
                // SWING dupe 2
                if (i == 2) {
                    textSuffix = 4;
                }
                break;
            }
            case 13: {
                // BEATLE_HOOK dupe
                if (i == 1) {
                    textSuffix = 3;
                    bUseCaption2 = true;
                } else if (i == 2) {
                    textSuffix = 4;
                }
                break;
            }
            case 10: {
                // PACHINKO dupe
                if (i == 1) {
                    bUseCaption2 = true;
                }
                break;
            }
            case 39: {
                // NORMAL dupe
                if (i >= 1) {
                    bUseCaption2 = true;
                }
                break;
            }

            case 25: {
                // DIVING dupe
                if (i == 2) {
                    bUseCaption2 = true;
                }
                break;
            }
            case 28: {
                // BIRD dupe
                if (i == 1) {
                    bUseCaption2 = true;
                    bUseTitle2 = true;
                }
                break;
            }
            case 31: {
                // LOOK dupe
                if (i == 2) {
                    textSuffix = 4;
                }
                break;
            }
            case 18: {
                // BOW dupe
                bUseCaption2 = true;
                bUseTitle2 = true;
                break;
            }
        }

        if (!bUseTitle2) {
            label.sprintf("HLP_%s_%02d", sHelpTextKeys[textIndex], textSuffix);
        } else {
            label.sprintf("HLP_%s_%02d_2", sHelpTextKeys[textIndex], textSuffix);
        }
        mpTextBoxesTitleS[i]->setMessageWithGlobalTextProcessor2(label, nullptr);
        mpTextBoxesTitle[i]->setMessageWithGlobalTextProcessor2(label, nullptr);

        if (!bUseCaption2) {
            label.sprintf("CPT_%s_%02d", sHelpTextKeys[textIndex], textSuffix);
        } else {
            label.sprintf("CPT_%s_%02d_2", sHelpTextKeys[textIndex], textSuffix);
        }
        mpTextBoxesS[i]->setMessageWithGlobalTextProcessor2(label, nullptr);
        mpTextBoxes[i]->setMessageWithGlobalTextProcessor2(label, nullptr);
    }
}

#pragma pop

bool dLytHelpMain_c::execute() {
    mStateMgr.executeState();

    for (int i = 0; i < HELP_MAIN_NUM_ANIMS; i++) {
        if (mAnm[i].isEnabled()) {
            mAnm[i].play();
        }
    }
    mLyt.calc();
    return true;
}

bool dLytHelpMain_c::draw() {
    mLyt.addToDrawList();
    return true;
}

void dLytHelp_c::initializeState_None() {}
void dLytHelp_c::executeState_None() {
    if (field_0x3C0 == true) {
        dSndPlayerMgr_c::GetInstance()->enterHelp();
        mpMain->modeRequestIn(mTextIndex);
        dLytMeter_c::GetInstance()->setHelpOpen(true);
        mStateMgr.changeState(StateID_In);
        field_0x3C0 = false;
    }
}
void dLytHelp_c::finalizeState_None() {}

void dLytHelp_c::initializeState_In() {}
void dLytHelp_c::executeState_In() {
    if (mpMain->isIdle()) {
        mIsWaiting = true;
        mpMain->modeRequestWait();
        mStateMgr.changeState(StateID_Wait);
        field_0x3C8 = 0;
    }
}
void dLytHelp_c::finalizeState_In() {}

void dLytHelp_c::initializeState_Wait() {}
void dLytHelp_c::executeState_Wait() {
    if (dPad::getDownTrig2() || dPad::getDownTrigMinus() || dPad::getDownTrigB() || mOutRequest) {
        mpMain->modeRequestOut();
        dSndPlayerMgr_c::GetInstance()->leaveHelp();
        dLytMeter_c::GetInstance()->setHelpOpen(false);
        mStateMgr.changeState(StateID_Out);
    }
}
void dLytHelp_c::finalizeState_Wait() {}

void dLytHelp_c::initializeState_Out() {}
void dLytHelp_c::executeState_Out() {
    if (mpMain->isIdle()) {
        mTextIndex = -1;
        mpMain->modeRequestEnd();
        mStateMgr.changeState(StateID_End);
    }
}
void dLytHelp_c::finalizeState_Out() {}

void dLytHelp_c::initializeState_End() {}
void dLytHelp_c::executeState_End() {}
void dLytHelp_c::finalizeState_End() {}

bool dLytHelp_c::setText(s32 textIndex) {
    if (*mStateMgr.getStateID() == StateID_None) {
        field_0x3C0 = true;
        mOutRequest = false;
        if (textIndex <= ARRAY_LENGTH(sHelpTextKeys) - 1) {
            mTextIndex = textIndex;
        } else {
            mTextIndex = 0;
        }
        return true;
    } else {
        return false;
    }
}

void dLytHelp_c::changeText(s32 textIndex) {
    bool instantSwitch = false;
    if (*mStateMgr.getStateID() != StateID_None || mTextIndex >= 0) {
        switch (mTextIndex) {
            case 0:
                /* NORMAL */
                if (textIndex == 1) {
                    mTextIndex = 1;
                    instantSwitch = true;
                } else if (textIndex == 39) {
                    mTextIndex = 39;
                    instantSwitch = true;
                }
                break;
            case 1:
                /* NORMAL_Z */
                if (textIndex == 0) {
                    mTextIndex = 0;
                    instantSwitch = true;
                }
                break;
            case 2:
            case 3:
            case 4:
                /* SWORD / _LV1 */
                if (textIndex == 23) {
                    mTextIndex = 23;
                    instantSwitch = true;
                }
                break;
            case 11:
                /* BEATLE */
                if (textIndex == 12) {
                    mTextIndex = 12;
                    instantSwitch = true;
                } else if (textIndex == 13) {
                    mTextIndex = 13;
                    instantSwitch = true;
                }
                break;
            case 12:
            case 13:
                /* BEATLE / _HOOK */
                if (textIndex == 11) {
                    mTextIndex = 11;
                    instantSwitch = true;
                }
                break;
            case 23:
                /* SHIELD */
                if (textIndex == 2) {
                    mTextIndex = 2;
                    instantSwitch = true;
                } else if (textIndex == 3) {
                    mTextIndex = 3;
                    instantSwitch = true;
                } else if (textIndex == 4) {
                    mTextIndex = 4;
                    instantSwitch = true;
                }
                break;
            case 27:
                if (textIndex == 28) {
                    mTextIndex = 28;
                    instantSwitch = true;
                }
                break;
            case 33:
                if (textIndex == 34) {
                    mTextIndex = 34;
                    instantSwitch = true;
                }
                break;
            case 34:
                if (textIndex == 33) {
                    mTextIndex = 33;
                    instantSwitch = true;
                }
                break;
            case 35:
                if (textIndex == 37) {
                    mTextIndex = 37;
                    instantSwitch = true;
                }
                break;
            case 37:
                if (textIndex == 35) {
                    mTextIndex = 35;
                    instantSwitch = true;
                }
                break;
            case 39:
                if (textIndex == 0) {
                    mTextIndex = 0;
                    instantSwitch = true;
                }
                break;
        }
    }

    if (instantSwitch) {
        mpMain->setText(mTextIndex);
    } else if (mTextIndex != textIndex) {
        mTextIndex = textIndex;
        mOutRequest = true;
    }
}

bool dLytHelp_c::build() {
    d2d::setLytAllocator();
    void *data = LayoutArcManager::GetInstance()->getLoadedData("MenuHelp");
    mResAcc.attach(data, "");
    mpMain = new dLytHelpMain_c();
    mpMain->build(&mResAcc);
    mStateMgr.changeState(StateID_None);
    field_0x3C0 = false;
    mOutRequest = false;
    mIsWaiting = false;
    mTextIndex = -1;
    field_0x3C8 = 0;
    m2d::setLytAllocator();
    return true;
}

bool dLytHelp_c::remove() {
    d2d::setLytAllocator();
    mpMain->remove();
    delete mpMain;
    mpMain = nullptr;
    mResAcc.detach();
    m2d::setLytAllocator();
    return true;
}

bool dLytHelp_c::execute() {
    if (*mStateMgr.getStateID() != StateID_None) {
        mpMain->execute();
    }
    mStateMgr.executeState();
    return true;
}

void dLytHelp_c::draw() {
    if (*mStateMgr.getStateID() != StateID_None) {
        mpMain->draw();
    }
}
