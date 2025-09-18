#include "d/lyt/d_lyt_game_over.h"

#include "common.h"
#include "d/d_base.h"
#include "d/d_fader.h"
#include "d/d_message.h"
#include "d/d_sc_game.h"
#include "d/d_sc_title.h"
#include "d/d_stage.h"
#include "d/flag/sceneflag_manager.h"
#include "d/lyt/d2d.h"
#include "d/lyt/d_window.h"
#include "d/snd/d_snd_bgm_mgr.h"
#include "d/snd/d_snd_wzsound.h"
#include "f/f_base.h"
#include "nw4r/lyt/lyt_group.h"
#include "toBeSorted/arc_managers/layout_arc_manager.h"
#include "toBeSorted/file_manager.h"
#include "toBeSorted/minigame_mgr.h"
#include "toBeSorted/misc_actor.h"

STATE_DEFINE(dLytGameOver_c, Init);
STATE_DEFINE(dLytGameOver_c, In);
STATE_DEFINE(dLytGameOver_c, Wait);
STATE_DEFINE(dLytGameOver_c, Out);

static const d2d::LytBrlanMapping brlanMap2[] = {
    {"gameOver_02_gameOver.brlan",  "G_gameOver_00"},
    {    "gameOver_02_loop.brlan",      "G_loop_00"},
    { "gameOver_02_textOut.brlan", "G_textInOut_00"},
    {  "gameOver_02_textIn.brlan", "G_textInOut_00"},
    {    "gameOver_02_fade.brlan",      "G_fade_00"},
};

static const d2d::LytBrlanMapping brlanMap3[] = {
    {"gameOver_03_gameOver.brlan",  "G_gameOver_00"},
    {    "gameOver_03_loop.brlan",      "G_loop_00"},
    { "gameOver_03_textOut.brlan", "G_textInOut_00"},
    {  "gameOver_03_textIn.brlan", "G_textInOut_00"},
    {    "gameOver_03_fade.brlan",      "G_fade_00"},
};

static const d2d::LytBrlanMapping brlanMap1[] = {
    {"gameOver_01_gameOver.brlan",  "G_gameOver_00"},
    {    "gameOver_01_loop.brlan",      "G_loop_00"},
    { "gameOver_01_textOut.brlan", "G_textInOut_00"},
    {  "gameOver_01_textIn.brlan", "G_textInOut_00"},
    {    "gameOver_01_fade.brlan",      "G_fade_00"},
};

#define GAME_OVER_ANIM_GAME_OVER 0
#define GAME_OVER_ANIM_LOOP 1
#define GAME_OVER_ANIM_TEXT_OUT 2
#define GAME_OVER_ANIM_TEXT_IN 3
#define GAME_OVER_ANIM_TEXT_FADE 4

#define GAME_OVER_NUM_ANIMS 5

static const char *sGroupName = "G_ref_00";

dLytGameOver_c::dLytGameOver_c() : mStateMgr(*this) {
    init();
}

extern "C" u8 fn_80054F30();

bool dLytGameOver_c::build() {
    u8 lang = fn_80054F30();
    s32 variant;
    if (lang == 3) {
        variant = 1;
    } else if (lang == 4) {
        variant = 2;
    } else {
        variant = 0;
    }

    if (variant == 0) {
        void *data = LayoutArcManager::GetInstance()->getLoadedData("gameOver_01");
        mResAcc1.attach(data, "");
    } else if (variant == 1) {
        void *data = LayoutArcManager::GetInstance()->getLoadedData("gameOver_02");
        mResAcc1.attach(data, "");
    } else {
        void *data = LayoutArcManager::GetInstance()->getLoadedData("gameOver_03");
        mResAcc1.attach(data, "");
    }
    void *data2 = LayoutArcManager::GetInstance()->getLoadedData("System2D");
    mResAcc2.attach(data2, "");

    switch (variant) {
        case 1: {
            mLyt.setResAcc(&mResAcc1);
            mLyt.build("gameOver_02.brlyt", nullptr);
            for (int i = 0; i < GAME_OVER_NUM_ANIMS; i++) {
                mAnm[i].init(brlanMap2[i].mFile, &mResAcc1, mLyt.getLayout(), brlanMap2[i].mName);
            }
            break;
        }
        case 2: {
            mLyt.setResAcc(&mResAcc1);
            mLyt.build("gameOver_03.brlyt", nullptr);
            for (int i = 0; i < GAME_OVER_NUM_ANIMS; i++) {
                mAnm[i].init(brlanMap3[i].mFile, &mResAcc1, mLyt.getLayout(), brlanMap3[i].mName);
            }
            break;
        }
        default: {
            mLyt.setResAcc(&mResAcc1);
            mLyt.build("gameOver_01.brlyt", nullptr);
            for (int i = 0; i < GAME_OVER_NUM_ANIMS; i++) {
                mAnm[i].init(brlanMap1[i].mFile, &mResAcc1, mLyt.getLayout(), brlanMap1[i].mName);
            }
            break;
        }
    }

    mLyt.setPriority(0x8A);

    for (int i = 0; i < 1; i++) {
        mSubpanes[i].mpLytPane = &mSelectBtn;
        mSelectBtn.build(&mResAcc2);
        mSubpaneList.PushBack(&mSubpanes[i]);
    }

    if (mLyt.getLayout()->GetGroupContainer() != nullptr) {
        nw4r::lyt::Group *g = mLyt.getLayout()->GetGroupContainer()->FindGroupByName(sGroupName);
        if (g != nullptr) {
            d2d::dSubPane::linkMeters(g, &mSubpaneList);
        }
    }

    mSelectBtn.setField_0x9C4(1);
    mSelectBtn.setField_0x99C(1);
    mSelectBtn.setField_0x9A0(2);

    mStateMgr.changeState(StateID_Init);

    field_0x1338 = true;

    return true;
}

bool dLytGameOver_c::remove() {
    for (d2d::SubPaneList::Iterator it = mSubpaneList.GetBeginIter(); it != mSubpaneList.GetEndIter(); ++it) {
        d2d::dSubPane *subPane = it->mpLytPane;
        if (subPane != nullptr) {
            nw4r::lyt::Pane *parent = subPane->getPane()->GetParent();
            parent->RemoveChild(subPane->getPane());
        }
    }

    for (int i = 0; i < 1; i++) {
        mSubpanes[i].mpLytPane->remove();
    }

    mLyt.unbindAnims();

    for (int i = 0; i < GAME_OVER_NUM_ANIMS; i++) {
        mAnm[i].remove();
    }

    mResAcc2.detach();
    mResAcc1.detach();

    dBase_c::s_NextExecuteControlFlags &= ~0x200;
    dBase_c::s_DrawControlFlags &= ~0x200;

    return true;
}

bool dLytGameOver_c::execute() {
    mStateMgr.executeState();
    mLyt.calc();
    for (int i = 0; i < 1; i++) {
        mSubpanes[i].mpLytPane->execute();
    }
    return true;
}

bool dLytGameOver_c::draw() {
    if (mIsVisible != true) {
        return true;
    }
    mLyt.addToDrawList();
    return true;
}

bool dLytGameOver_c::requestIn(const char *label, s32 p1, s32 p2) {
    mLabel = label;
    mVariant = p1;
    mInType = p2;
    mInRequest = true;
    return true;
}

bool dLytGameOver_c::requestOut(s32 type) {
    if (!mStateMgr.getStateID()->isEqual(StateID_Wait)) {
        return false;
    }

    mOutType = type;
    mOutRequest = true;

    return true;
}

void dLytGameOver_c::initializeState_Init() {
    init();
}
void dLytGameOver_c::executeState_Init() {
    if (mInRequest == true) {
        mInRequest = false;
        mStateMgr.changeState(StateID_In);
    }
}
void dLytGameOver_c::finalizeState_Init() {}

void dLytGameOver_c::initializeState_In() {
    mStep = 0;
    mIsVisible = true;
    u32 anim = GAME_OVER_ANIM_TEXT_IN;
    if (mInType == 0) {
        anim = GAME_OVER_ANIM_GAME_OVER;
    }
    setAnm(anim);
    applyText();
    field_0x1338 = 0;
}
void dLytGameOver_c::executeState_In() {
    u32 anim = GAME_OVER_ANIM_TEXT_IN;
    if (mInType == 0) {
        anim = GAME_OVER_ANIM_GAME_OVER;
    }

    d2d::AnmGroup_c &anm = mAnm[anim];
    if (anm.isEndReached() == true) {
        stopAnm(anim);
        setAnm(GAME_OVER_ANIM_LOOP);
        mStateMgr.changeState(StateID_Wait);
        setupButtons();
    }
    anm.play();
}
void dLytGameOver_c::finalizeState_In() {
    mSelectBtn.getPane()->SetVisible(true);
}

void dLytGameOver_c::initializeState_Wait() {
    mStep = 0;
}
void dLytGameOver_c::executeState_Wait() {
    d2d::AnmGroup_c &anm = mAnm[GAME_OVER_ANIM_LOOP];
    anm.play();
    switch (mStep) {
        case 0: {
            if (mSelectBtn.getField_0x9B4() >= 0) {
                mSelectedOption = mSelectBtn.getField_0x9B4();
                mStep += 1;
                mIsChangingState = true;
            }
            break;
        }
        case 1: {
            if (mStep == 1 && mOutRequest == true) {
                mIsChangingState = false;
                mOutRequest = false;
                mStateMgr.changeState(StateID_Out);
            }
            break;
        }
    }
}
void dLytGameOver_c::finalizeState_Wait() {}

void dLytGameOver_c::initializeState_Out() {
    mStep = 0;
    if (mOutType == 0) {
        setAnm(GAME_OVER_ANIM_TEXT_OUT);
    } else {
        setAnm(GAME_OVER_ANIM_TEXT_FADE);
    }
}
void dLytGameOver_c::executeState_Out() {
    u32 anim = GAME_OVER_ANIM_TEXT_OUT;
    if (mOutType != 0) {
        anim = GAME_OVER_ANIM_TEXT_FADE;
    }
    d2d::AnmGroup_c &anm = mAnm[anim];
    switch (mStep) {
        case 0: {
            if (anm.isEndReached() == true) {
                stopAnm(GAME_OVER_ANIM_TEXT_OUT);
                stopAnm(GAME_OVER_ANIM_LOOP);
                mIsVisible = false;
                mIsChangingState = true;
                mSelectBtn.getPane()->SetVisible(false);
                mStep = 1;
            }
            break;
        }
        case 1: {
            mStateMgr.changeState(StateID_Init);
            break;
        }
    }
    anm.play();
}
void dLytGameOver_c::finalizeState_Out() {}

void dLytGameOver_c::init() {
    mIsVisible = false;
    mIsChangingState = false;
    mInRequest = false;
    mOutRequest = false;
    mSelectedOption = 0;
}

void dLytGameOver_c::setAnm(int idx) {
    d2d::AnmGroup_c &anm = mAnm[idx];
    anm.bind(false);
    anm.setFrame(0.0f);
}

void dLytGameOver_c::stopAnm(int idx) {
    d2d::AnmGroup_c &anm = mAnm[idx];
    anm.unbind();
}

void dLytGameOver_c::applyText() {
    const wchar_t *text = dMessage_c::getTextMessageByLabel(mLabel, true, nullptr, 0);
    mLyt.getTextBox("T_select_00")->setTextWithGlobalTextProcessor(text);
    mLyt.getTextBox("T_selectS_00")->setTextWithGlobalTextProcessor(text);

    dWindow_c *window = mLyt.getWindow("W_bgP_00");
    window->UpdateSize(mLyt.getSizeBoxInWindow("W_bgP_00"), 32.0f);
}

void dLytGameOver_c::setupButtons() {
    s32 numOptions = 2;
    if (mVariant == 1) {
        numOptions = 3;
    } else if (mVariant == 2) {
        numOptions = 4;
    }

    for (int i = 0; i < numOptions; i++) {
        const wchar_t *buf = dMessage_c::getGlobalTagProcessor()->getBuf(i);
        mSelectBtn.getSelectTextBox(i, 0)->setTextWithGlobalTextProcessor(buf);
        mSelectBtn.getSelectTextBox(i, 1)->setTextWithGlobalTextProcessor(buf);
    }
    mSelectBtn.setTagProcessor(dMessage_c::getGlobalTagProcessor());
    u32 tmp = dMessage_c::getGlobalTagProcessor()->getField_0x828();
    mSelectBtn.setField_0x9BC(tmp);
    mSelectBtn.setField_0x990(tmp);
    mSelectBtn.fn_8011E5D0(numOptions, true);
}

SPECIAL_BASE_PROFILE(LYT_GAMEOVER_MGR, dLytGameOverMgr_c, fProfile::LYT_GAMEOVER_MGR, 0x02AC, 0x0201);

STATE_DEFINE(dLytGameOverMgr_c, Init);
STATE_DEFINE(dLytGameOverMgr_c, FadeOut);
STATE_DEFINE(dLytGameOverMgr_c, Select);

dLytGameOverMgr_c::dLytGameOverMgr_c() : mStateMgr(*this) {
    sInstance = this;
    init();
}

dLytGameOverMgr_c::~dLytGameOverMgr_c() {
    sInstance = nullptr;
}

int dLytGameOverMgr_c::create() {
    mMain.build();
    mStateMgr.changeState(StateID_Init);
    return SUCCEEDED;
}

int dLytGameOverMgr_c::doDelete() {
    mMain.remove();
    return SUCCEEDED;
}

int dLytGameOverMgr_c::execute() {
    mStateMgr.executeState();
    return SUCCEEDED;
}

int dLytGameOverMgr_c::draw() {
    mMain.draw();
    return SUCCEEDED;
}

void dLytGameOverMgr_c::initializeState_Init() {}
void dLytGameOverMgr_c::executeState_Init() {
    mStateMgr.changeState(StateID_FadeOut);
}
void dLytGameOverMgr_c::finalizeState_Init() {}

void dLytGameOverMgr_c::initializeState_FadeOut() {
    dStage_c::GetInstance()->fadeOut(dFader_c::FADER_BLACK, 40);
}
void dLytGameOverMgr_c::executeState_FadeOut() {
    if (dStage_c::GetInstance()->isFadedOut() == true) {
        // "Do you want to continue adventuring? [1]Continue[2]Quit"
        if (mMain.requestIn("SYS_GAMEOVER_01", 0, 0)) {
            mMain.setField_0x1338(true);
            mStateMgr.changeState(StateID_Select);
            dSndBgmMgr_c::GetInstance()->playFanSound(FAN_GAMEOVER);
        }
    }
}
void dLytGameOverMgr_c::finalizeState_FadeOut() {}

void dLytGameOverMgr_c::initializeState_Select() {
    mStep = 0;
    mTimer = 0;
}
void dLytGameOverMgr_c::executeState_Select() {
    // TODO - regswaps https://decomp.me/scratch/IPWjo
    // FileManager::GetInstance() causes even more regswaps
    FileManager *fileManager = FileManager::sInstance;

    // I'm not sure if all these were declared here - it'd make
    // sense for a switch statement...
    const char *stage;
    u8 entrance;
    u16 room;
    u8 layer;
    bool isSpecialMinigame;

    switch (mStep) {
        case 0: {
            if (mTimer < 20) {
                mTimer++;
                if (mTimer == 20) {
                    dBase_c::s_NextExecuteControlFlags |= 1;
                    dBase_c::s_DrawControlFlags |= 1;
                }
            }
            if (mMain.isChangingState() == true) {
                if (mMain.getSelectedOption() == 0) {
                    mStep = 20;
                } else {
                    mStep = 1;
                }
            }
            break;
        }
        case 1:
        case 10: {
            if (mMain.requestOut(0) == true) {
                mStep++;
            }
            break;
        }
        case 2:
        case 11: {
            if (mMain.isChangingState() == true) {
                mStep++;
            }
            break;
        }
        case 3: {
            // "Are you sure you want to quit? [1]Quit[2-]Cancel"
            if (mMain.requestIn("SYS_GAMEOVER_03", 0, 1)) {
                mStep++;
            }
            break;
        }
        case 4: {
            if (mMain.isChangingState() == true) {
                if (mMain.getSelectedOption() == 0) {
                    mStep++;
                } else {
                    mStep = 10;
                }
            }
            break;
        }
        case 5:
        case 20: {
            if (mMain.requestOut(1) == true) {
                mStep++;
            }
            break;
        }
        case 6: {
            dScTitle_c::loadTitleScreen(1);
            mStep = 100;
            // break;
        }
        case 12: {
            // "Do you want to continue adventuring? [1]Continue[2]Quit"
            if (mMain.requestIn("SYS_GAMEOVER_01", 0, 1)) {
                mStep = 0;
            }
            break;
        }
        case 21: {
            if (mMain.isChangingState() == true) {
                dScTitle_c::setLbl_80575787(true);
                dScGame_c::sPreventClearingSomeFlag = true;
                // Maybe an inline? Would have to use the inverse condition and then negate...
                isSpecialMinigame = false;
                if (MinigameManager::isInMinigameState(MinigameManager::TRIAL_TIME_ATTACK) ||
                    MinigameManager::isInMinigameState(MinigameManager::BOSS_RUSH)) {
                    isSpecialMinigame = true;
                }

                if (mParams != 0 && !isSpecialMinigame) {
                    dScGame_c::setReloaderType(1);
                    dScGame_c::sCopyFileBToCurrentAfterRespawn = true;
                    entrance = fileManager->getEntranceT1_FileB();
                    room = fileManager->getLoadRoomT1_FileB();
                    layer = fileManager->getForcedLayerT1_FileB();
                    // TODO document this logic
                    if (dScGame_c::isCurrentStage("F403") && layer == 2) {
                        layer = 7;
                    }
                    stage = fileManager->getAreaT1_FileB();
                    // TODO - this is weird, why is dScGame_c::sInstance being loaded first?
                    // inlines? But we're using the fileManager from above for this call...
                    // Probably the reason for the regswaps...
                    dScGame_c::GetInstance()->triggerEntrance(
                        stage, room, layer, entrance, fileManager->getNightT1_FileB()
                    );
                } else {
                    dScGame_c::setReloaderType(3);
                    entrance = fileManager->getEntranceT3();
                    room = fileManager->getLoadRoomT3();
                    layer = fileManager->getForcedLayerT3();
                    // TODO - document this logic
                    if (dScGame_c::isCurrentStage("B400") && !isSpecialMinigame) {
                        dScGame_c::setReloaderType(4);
                        entrance = 8;
                        room = 1;
                        layer = 3;
                    }
                    stage = fileManager->getAreaT3();
                    // TODO - same as above
                    dScGame_c::GetInstance()->triggerEntrance(stage, room, layer, entrance, fileManager->getNightT3());
                }

                // TODO - document this logic
                if (dScGame_c::isCurrentStage("F401")) {
                    if (fileManager->getCurrentHealth() < 24) {
                        fileManager->setCurrentHealth(24);
                    }
                } else {
                    fileManager->setCurrentHealth(24);
                }

                // TODO - document this logic
                if (dScGame_c::isCurrentStage("F302")) {
                    // "Minecart Escort related"
                    SceneflagManager::sInstance->unsetTempflag_i(0x3F, 0);
                }
                restoreSkykeepPuzzleFromGlobal();
                mStep = 100;
            }
            break;
        }
        case 100: {
            dBase_c::s_NextExecuteControlFlags &= ~0x1;
            dBase_c::s_DrawControlFlags &= ~0x1;

            dBase_c::s_NextExecuteControlFlags |= 0x200;
            dBase_c::s_DrawControlFlags |= 0x200;

            mStep++;
            break;
        }
    }

    mMain.execute();
}
void dLytGameOverMgr_c::finalizeState_Select() {}

void dLytGameOverMgr_c::init() {
    mStep = 0;
    mTimer = 0;
}
