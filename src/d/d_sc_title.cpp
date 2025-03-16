#include "d/d_sc_title.h"

#include "common.h"
#include "d/d_heap.h"
#include "d/d_sc_game.h"
#include "d/d_sys.h"
#include "f/f_base.h"
#include "f/f_profile_name.h"
#include "m/m_color.h"
#include "m/m_dvd.h"
#include "s/s_State.hpp"
#include "sized_string.h"
#include "toBeSorted/arc_managers/layout_arc_manager.h"
#include "toBeSorted/file_manager.h"
#include "toBeSorted/music_mgrs.h"
#include "toBeSorted/reload_color_fader.h"

SPECIAL_BASE_PROFILE(TITLE, dScTitle_c, fProfile::TITLE, 0, 0);

dScTitle_c *dScTitle_c::sInstance;
bool dScTitle_c::sPreventEmptyFilesTitle;
extern "C" {
u8 lbl_80575785;
u8 lbl_80575786;
u8 lbl_80575787;
}

static u16 sModulesToLoad[] = {
    // ??? not sure what these correspond to
    0x229, 0x2B0, 0x2B6, 0x2AF, 0x2B7,
};

static const char *sLayoutArcsToLoad[] = {
    "Title2D",
};

STATE_VIRTUAL_DEFINE(dScTitle_c, Stanby);
STATE_VIRTUAL_DEFINE(dScTitle_c, Action);

extern "C" void *lbl_80575688;
extern "C" void fn_801907F0(void *);
extern "C" void *lbl_805750D8;
extern "C" void fn_80052D50(void *);

dScTitle_c::dScTitle_c() {
    sInstance = this;
    mRelCtrl.set(sModulesToLoad, ARRAY_LENGTH(sModulesToLoad));
    mLayoutCtrl.set(sLayoutArcsToLoad, ARRAY_LENGTH(sLayoutArcsToLoad));
    fn_801907F0(lbl_80575688);
    if (checkAllSaveFilesEmpty() == true) {
        actuallyTriggerEntrance("F000", 0, 26, 55, 0, 0, 0, 15, -1);
    } else {
        actuallyTriggerEntrance("F000", 0, 28, 48, 0, 0, 0, 15, -1);
    }
    FileManager::GetInstance()->clearTempFileData();
    FileManager::GetInstance()->initFile(1);
    fn_80052D50(lbl_805750D8);
    lbl_80575787 = 0;
    field_0x2AC = 0;
}

dScTitle_c::~dScTitle_c() {
    sInstance = nullptr;
}

extern "C" u32 TITLE_SCREEN_CHANGE;
extern "C" void fn_80059450();
extern "C" void fn_80058C90(s32);
extern "C" void fn_80015E40();
extern "C" const char *getUsedLanguageString();

static const char *const sFileSelect = "FileSelect";
static const char *const sSkb = "SoftwareKeyboard";

int dScTitle_c::create() {
    int ret = dScGame_c::create();
    if (ret == SUCCEEDED && ReloadColorFader::GetInstance()->field_0x0C == 1) {
        if (field_0x2AC == 0) {
            ReloadColorFader::GetInstance()->fn_80067EF0(0);
            if (checkAllSaveFilesEmpty() != true) {
                TITLE_SCREEN_CHANGE = 0;
            }
            field_0x2AC = 1;
            ret = NOT_READY;
        } else {
            ReloadColorFader::GetInstance()->fn_80067EF0(1);
        }
    }

    if (ret == SUCCEEDED) {
        dSys::setFrameRate(2);
        dSys::setClearColor(mColor(0x00000000));
        fn_80059450();
        fn_80058C90(0);
        fn_80015E40();
        field_0x2AD = 0;
        LayoutArcManager::GetInstance()->loadLayoutArcFromDisk(sFileSelect, nullptr);
        LayoutArcManager::GetInstance()->loadLayoutArcFromDisk(sSkb, nullptr);
        mDvd_toMainRam_normal_c::create2(&mpSkbArc, "/SKB/sofkeybd.arc", 0, dHeap::work2Heap.heap);
        SizedString<128> fntPath;
        fntPath.sprintf("/US/Font/%s/%s", getUsedLanguageString(), "normal_02.brfnt");
        mDvd_toMainRam_normal_c::create2(&mpSkbFont, fntPath, 0, dHeap::work2Heap.heap);
        fn_8035E310(BGM_MGR);
    }

    return ret;
}

int dScTitle_c::execute() {
    mFader.calc();
    mStateMgr.executeState();
    return SUCCEEDED;
}

extern "C" void fn_80015E80();

int dScTitle_c::doDelete() {
    // TODO return codes
    int result;
    result = LayoutArcManager::GetInstance()->ensureLoaded2(sFileSelect);
    if (result != -2) {
        if (result != 0) {
            return NOT_READY;
        }
        LayoutArcManager::GetInstance()->decrement(sFileSelect);
    }

    result = LayoutArcManager::GetInstance()->ensureLoaded2(sSkb);
    if (result != -2) {
        if (result != 0) {
            return NOT_READY;
        }
        LayoutArcManager::GetInstance()->decrement(sSkb);
    }

    // TODO here's the other half of the ::destroy fakematch...
    if (mpSkbArc->mStatus != 0) {
        mDvd_command_c::destroy((mDvd_command_c **)&mpSkbArc);
    } else {
        return NOT_READY;
    }

    if (mpSkbFont->mStatus != 0) {
        mDvd_command_c::destroy((mDvd_command_c **)&mpSkbFont);
    } else {
        return NOT_READY;
    }

    dScGame_c::doDelete();
    fn_80015E80();
    return SUCCEEDED;
}

int dScTitle_c::draw() {
    dScGame_c::draw();
    return SUCCEEDED;
}

extern "C" u32 TITLE_SCREEN_CHANGE;
void dScTitle_c::loadTitleScreen(u32 params) {
    FileManager::GetInstance()->updateEmptyFiles();
    if (checkAllSaveFilesEmpty() == true && params != 1) {
        params = 2;
    }
    TITLE_SCREEN_CHANGE = params;
    setRootActor(fProfile::TITLE, params, 0, 0);
    if (checkAllSaveFilesEmpty() == true) {
        actuallyTriggerEntrance("F000", 0, 26, 55, 0, 0, 0, 15, -1);
    } else {
        actuallyTriggerEntrance("F000", 0, 28, 48, 0, 0, 0, 15, -1);
    }
    fn_80362150(ENEMY_SOUND_MGR, 30);
}

void dScTitle_c::setSomethingSkbRelated() {
    if (sInstance != nullptr) {
        sInstance->field_0x2AD = 1;
    }
}

void dScTitle_c::initializeState_Stanby() {
    dScGame_c::initializeState_Stanby();
}

void dScTitle_c::executeState_Stanby() {
    dScGame_c::executeState_Stanby();
}

void dScTitle_c::finalizeState_Stanby() {
    dScGame_c::finalizeState_Stanby();
}

void dScTitle_c::initializeState_Action() {
    dScGame_c::initializeState_Action();
}

void dScTitle_c::executeState_Action() {
    dScGame_c::executeState_Action();
}

void dScTitle_c::finalizeState_Action() {
    dScGame_c::finalizeState_Action();
}

bool dScTitle_c::checkAllSaveFilesEmpty() {
    FileManager *mgr = FileManager::GetInstance();
    bool allEmpty = true;
    for (int i = 0; i < 3; i++) {
        if (!mgr->isFileEmpty(i)) {
            allEmpty = false;
            break;
        }
    }
    if (!sPreventEmptyFilesTitle && allEmpty == true) {
        return true;
    }
    return false;
}
