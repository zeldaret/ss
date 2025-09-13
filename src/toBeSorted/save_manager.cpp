#include "toBeSorted/save_manager.h"

#include "common.h"
#include "d/d_cs_base.h"
#include "d/d_d2d.h"
#include "d/d_dvd_unk.h"
#include "d/d_heap.h"
#include "d/d_message.h"
#include "d/d_reset.h"
#include "d/d_s_boot.h"
#include "d/d_sc_game.h"
#include "d/d_sys.h"
#include "d/lyt/d_lyt_save_msg_window.h"
#include "d/lyt/d_lyt_system_window.h"
#include "d/snd/d_snd_small_effect_mgr.h"
#include "egg/core/eggHeap.h"
#include "m/m_pad.h"
#include "toBeSorted/arc_managers/layout_arc_manager.h"
#include "toBeSorted/file_manager.h"
#include "toBeSorted/nand_request_thread.h"
#include "toBeSorted/nand_result_tracker.h"

#include "rvl/NAND.h"
#include "rvl/TPL.h"

#include <cstring>

SaveMgr *SaveMgr::sInstance;

static const char sSaveFileName[] = "wiiking2.sav";
static const char sSkipDatFileName[] = "skip.dat";
static const char sBannerBinFileName[] = "banner.bin";
static const char sTmpBannerBinFileName[] = "/tmp/banner.bin";

#define FILE_PERMS (NAND_PERM_RUSR | NAND_PERM_WUSR | NAND_PERM_RGRP | NAND_PERM_WGRP)

const SaveMgr::StateFunc SaveMgr::sStateFuncs[STATE_MAX] = {
    &SaveMgr::executeCheckForSave, &SaveMgr::executeCheckForFreeSpace, &SaveMgr::executeCreateFiles,
    &SaveMgr::executeSaveBanner,   &SaveMgr::executeLoadSave,          &SaveMgr::executeWriteSave,
    &SaveMgr::executeWriteSkipDat, &SaveMgr::executeCopySave,          &SaveMgr::executeClearSelectedFile,
    &SaveMgr::executeSave,         &SaveMgr::executeDeleteAllData,     &SaveMgr::executeError,
    &SaveMgr::executeNandError,
};

SaveMgr::SaveMgr()
    : mCheckIsFileRequest(nullptr),
      mCheckRequest(nullptr),
      mCreateRequest(nullptr),
      mWriteRequest(nullptr),
      mSaveBannerRequest(nullptr),
      mLoadSaveRequest(nullptr),
      mDeleteRequest(nullptr) {
    sInstance = this;
    mpWindow = nullptr;
}
// Not used but this spawns the weak dtors
SaveMgr::~SaveMgr() {
    delete mpWindow;
}

void SaveMgr::create(EGG::Heap *heap) {
    new (heap, 4) SaveMgr();
    sInstance->init();
}

void SaveMgr::init() {
    mCurrentState = STATE_MAX;
    field_0x824 = 2;
    mCheckForFreeSpaceResult = 0;
    mCopyToFile = 0;
    mCopyFromFile = 0;
    field_0x836 = 0;
    field_0x837 = 0;
    field_0x838 = 0;
    field_0x839 = 0;
    field_0x83A = 0;
    field_0x83B = 0;
    field_0x83C = 0;
    field_0x83D = 0;
    field_0x83E = 0;
    mDelayTimer = 0;
    FileManager::GetInstance()->setField0xA840(1);
    if (dCsBase_c::GetInstance() != nullptr) {
        dCsBase_c::GetInstance()->setDrawDirectly(false);
    }
    if (mpWindow != nullptr) {
        mpWindow->reset();
    }
    field_0x83F = 0;
}

void SaveMgr::execute() {
    if (mCurrentState != STATE_MAX) {
        StateFunc f = sStateFuncs[mCurrentState];
        if (f != nullptr) {
            (this->*f)();
        }
    }
}

void SaveMgr::draw() {
    d2d::defaultSet();
    if (mpWindow != nullptr) {
        mpWindow->drawNow();
    }
}

void SaveMgr::createSaveMsgWindow() {
    if (mpWindow == nullptr) {
        mpWindow = new dLytSaveMsgWindow_c();
        mpWindow->build(false);
    }
}

bool SaveMgr::checkForSave() {
    if (mCurrentState != STATE_MAX) {
        return false;
    }
    field_0x836 = 0;
    initializeCheckForSave();
    return true;
}

bool SaveMgr::checkForFreeSpace() {
    if (mCurrentState != STATE_MAX) {
        return false;
    }
    field_0x836 = 0;
    initializeCheckForFreeSpace();
    return true;
}

bool SaveMgr::createFiles() {
    if (mCurrentState != STATE_MAX) {
        return false;
    }
    field_0x836 = 0;
    initializeCreateFiles();
    return true;
}

bool SaveMgr::loadSave() {
    if (mCurrentState != STATE_MAX) {
        return false;
    }
    field_0x836 = 0;
    initializeLoadSave();
    return true;
}

bool SaveMgr::save(bool entranceT1LoadFlag, bool full) {
    if (mCurrentState != STATE_MAX) {
        return false;
    }
    if (FileManager::GetInstance()->get_0xA84C()) {
        return false;
    }
    field_0x836 = 0;
    if (full == 1) {
        FileManager::GetInstance()->saveT1SaveInfo(entranceT1LoadFlag);
    } else {
        FileManager::GetInstance()->setEntranceLoadFlagT1(entranceT1LoadFlag);
    }
    FileManager::GetInstance()->setFileTimes();
    initializeSave();
    field_0x83E = 0;
    return true;
}

bool SaveMgr::saveAfterCredits() {
    if (mCurrentState != STATE_MAX) {
        return false;
    }
    if (FileManager::GetInstance()->get_0xA84C()) {
        return false;
    }
    field_0x836 = 0;
    dDvdUnk::FontUnk::GetInstance()->setNeedsPad(true);
    FileManager::GetInstance()->setFileTimes();
    FileManager::GetInstance()->saveAfterCredits();
    initializeSave();
    field_0x83E = 0;
    return true;
}

bool SaveMgr::writeSkipDat() {
    if (mCurrentState != STATE_MAX) {
        return false;
    }
    field_0x836 = 0;
    initializeWriteSkipDat();
    return true;
}

bool SaveMgr::copySave(u8 to, u8 from) {
    if (mCurrentState != STATE_MAX) {
        return false;
    }
    field_0x836 = 0;
    mCopyToFile = to;
    mCopyFromFile = from;
    initializeCopySave();
    field_0x83F = 0;
    return true;
}

bool SaveMgr::clearSelectedFile() {
    if (mCurrentState != STATE_MAX) {
        return false;
    }
    field_0x836 = 0;
    initializeClearSelectedFile();
    field_0x83F = 0;
    return true;
}

void SaveMgr::initializeCheckForSave() {
    beginState(STATE_CHECK_FOR_SAVE);
    mCheckIsFileRequest.checkIsFile(sSaveFileName);
    field_0x839 = 0;
    dDvdUnk::FontUnk::GetInstance()->setNeedsPad(false);
}

void SaveMgr::executeCheckForSave() {
    FileManager *fileManager = FileManager::GetInstance();
    NandResultTracker *t = NandResultTracker::GetInstance();
    switch (mStep) {
        case 0:
            if (mCheckIsFileRequest.isCompleted()) {
                if (t->isFailure(mCheckIsFileRequest.getResult()) && mCheckIsFileRequest.finish()) {
                    return;
                }
                if (mCheckIsFileRequest.getCheckResult()) {
                    field_0x839 |= 2;
                }
                if (mCheckIsFileRequest.finish()) {
                    mCheckIsFileRequest.checkIsFile(sSkipDatFileName);
                    mStep = 1;
                }
            }
            break;
        case 1:
            if (mCheckIsFileRequest.isCompleted()) {
                if (t->isFailure(mCheckIsFileRequest.getResult()) && mCheckIsFileRequest.finish()) {
                    return;
                }
                if (mCheckIsFileRequest.getCheckResult()) {
                    field_0x839 |= 4;
                }
                if (mCheckIsFileRequest.finish()) {
                    mCheckIsFileRequest.checkIsFile(sBannerBinFileName);
                    mStep = 2;
                }
            }
            break;
        case 2:
            if (mCheckIsFileRequest.isCompleted()) {
                if (t->isFailure(mCheckIsFileRequest.getResult()) && mCheckIsFileRequest.finish()) {
                    return;
                }
                if (mCheckIsFileRequest.getCheckResult()) {
                    field_0x839 |= 1;
                }
                if (mCheckIsFileRequest.finish()) {
                    mStep = 3;
                }
            }
            break;
        case 3:
            if (field_0x839 == (1 | 2) || field_0x839 == (1 | 2 | 4)) {
                fileManager->setField0xA841(1);
                if (field_0x839 == (1 | 2)) {
                    fileManager->setField0xA842(0);
                } else {
                    fileManager->setField0xA842(1);
                }
            } else {
                fileManager->setField0xA841(0);
                fileManager->setField0xA842(0);
                if (field_0x839) {
                    t->isFailure(NAND_RESULT_AUTHENTICATION);
                    initializeError();
                    return;
                }
            }
            endState();
            break;
    }
}

void SaveMgr::initializeCheckForFreeSpace() {
    beginState(STATE_CHECK_FOR_FREE_SPACE);
    dDvdUnk::FontUnk::GetInstance()->setNeedsPad(false);
    mCheckRequest.check(7, 3);
    mCheckForFreeSpaceResult = 0;
}

void SaveMgr::executeCheckForFreeSpace() {
    if (mCheckRequest.isCompleted()) {
        if (NandResultTracker::GetInstance()->isFailure(mCheckRequest.getResult()) && mCheckRequest.finish()) {
            return;
        }
        u32 result = mCheckRequest.getCheckResult();
        if ((result & (NAND_CHECK_TOO_MANY_APP_BLOCKS | NAND_CHECK_TOO_MANY_USER_BLOCKS)) != 0) {
            if (mCheckRequest.finish()) {
                mCheckForFreeSpaceResult = 1;
                initializeNandError();
            }
        } else if ((result & (NAND_CHECK_TOO_MANY_APP_FILES | NAND_CHECK_TOO_MANY_USER_FILES)) != 0) {
            if (mCheckRequest.finish()) {
                mCheckForFreeSpaceResult = 2;
                initializeNandError();
            }
        } else {
            if (mCheckRequest.finish()) {
                if (field_0x838 == 1 || field_0x83A == 1) {
                    initializeCreateFiles();
                } else {
                    endState();
                }
            }
        }
    }
}

void SaveMgr::initializeCreateFiles() {
    beginState(STATE_CREATE_FILES);
    field_0x838 = 1;
    FileManager::GetInstance()->setField0xA84D(1);
    dDvdUnk::FontUnk::GetInstance()->setNeedsPad(false);
}

void SaveMgr::executeCreateFiles() {
    FileManager *fileMgr = FileManager::GetInstance();
    switch (mStep) {
        case 0: {
            s32 numLines = 0;
            if (field_0x83A != 0) {
                if (field_0x837 == 1) {
                    // "Restoring save data. Please do not touch the POWER Button or RESET."
                    if (mpWindow->setProperties("SYS_INIT_06", 0, 1, field_0x83E)) {
                        numLines = 2;
                    }
                } else {
                    // "Creating save data again. Please do not touch the POWER Button or RESET."
                    if (mpWindow->setProperties("SYS_INIT_08", 0, 1, field_0x83E)) {
                        numLines = 2;
                    }
                }
            } else {
                // "Creating save data. Please do not touch the POWER Button or RESET."
                if (mpWindow->setProperties("SYS_INIT_02", 0, 1, field_0x83E)) {
                    numLines = 2;
                }
            }
            if (numLines != 0) {
                mDelayTimer = 0;
                mStep++;
                mpWindow->setNumLines(numLines);
            }
            break;
        }
        case 1: {
            if (mDelayTimer < 30) {
                mDelayTimer++;
            } else {
                mCreateRequest.create(sSaveFileName, FILE_PERMS, 0);
                mDelayTimer = 0;
                mStep++;
            }
            break;
        }
        case 2: {
            if (!mCreateRequest.isCompleted()) {
                return;
            }
            NANDResult status = mCreateRequest.getResult();
            if (!mCreateRequest.finish()) {
                return;
            }
            if (NandResultTracker::GetInstance()->isFailure(status)) {
                return;
            }
            fileMgr->setField0xA841(1);
            mStep++;
            break;
        }
        case 3: {
            mCreateRequest.create(sSkipDatFileName, FILE_PERMS, 0);
            mStep++;
            break;
        }
        case 4: {
            if (!mCreateRequest.isCompleted()) {
                return;
            }
            NANDResult status = mCreateRequest.getResult();
            if (!mCreateRequest.finish()) {
                return;
            }
            if (NandResultTracker::GetInstance()->isFailure(status)) {
                return;
            }
            if (dScGame_c::GetInstance() != nullptr && dScGame_c::GetInstance()->profile_name == fProfile::GAME) {
                fileMgr->copySelectedFileSkipData();
            }
            fileMgr->setField0xA842(1);
            initializeWriteSave();
            mStep = 1; // Skips something in other state!
            break;
        }
    }
    mpWindow->execute();
}

void SaveMgr::initializeSaveBanner() {
    beginState(STATE_SAVE_BANNER);
    // "Zelda: Skyward Sword"
    const wchar_t *title = dMessage_c::getTextMessageByLabel("SYS_ZELDA_01", true, nullptr, 0);
    // Need to stash title as next call to getTextMessageByLabel will reuse internal buffer
    // Missed optimization: Could've passed mTextBuf directly
    for (s32 i = 0; i < 0x3FF; i++) {
        mTextBuf[i] = title[i];
    }
    // "A story of Link and Zelda."
    const wchar_t *subtitle = dMessage_c::getTextMessageByLabel("SYS_ZELDA_02", true, nullptr, 0);
    void *data = LayoutArcManager::GetInstance()->getData("saveBannerU", "tmp/saveBanner.tpl");
    mSaveBannerRequest.saveBanner(sTmpBannerBinFileName, (TPLPalette *)data, mTextBuf, subtitle, FILE_PERMS, 0);
    FileManager::GetInstance()->setField0xA84D(1);
}

void SaveMgr::executeSaveBanner() {
    dDvdUnk::FontUnk *fontUnk = dDvdUnk::FontUnk::GetInstance();
    switch (mStep) {
        case 0: {
            mDelayTimer++;
            if (!mSaveBannerRequest.isCompleted()) {
                return;
            }
            FileManager::GetInstance()->setField0xA84D(0);
            NANDResult result = mSaveBannerRequest.getResult();
            bool bFailed = mSaveBannerRequest.hasFailed();
            if (!mSaveBannerRequest.finish()) {
                return;
            }
            if (NandResultTracker::GetInstance()->isFailure(result)) {
                mStep = 20;
                return;
            }
            if (bFailed) {
                // retry
                mStep = 10;
                return;
            }
            mStep = 1;
            break;
        }
        case 1: {
            if (mDelayTimer < getFrameRate()) {
                mDelayTimer++;
            } else {
                if (mpWindow->checkIsWait()) {
                    mDelayTimer = 0;
                    mStep++;
                }
            }
            break;
        }
        case 2: {
            if (mpWindow->getWillFinishOut() == 1) {
                mStep++;
            }
            break;
        }
        case 3: {
            bool b = false;
            if (field_0x83A == 1) {
                if (field_0x837 != 0) {
                    // "Save data is restored."
                    if (mpWindow->setProperties("SYS_INIT_07", 0, 1, field_0x83E) == true) {
                        b = true;
                    }
                } else {
                    // "New save data has been created."
                    if (mpWindow->setProperties("SYS_INIT_09", 0, 1, field_0x83E) == true) {
                        b = true;
                    }
                }
            } else if (field_0x838 == 1) {
                // "Save data for The Legend of Zelda: Skyward Sword has been created."
                if (mpWindow->setProperties("SYS_INIT_03", 0, 1, field_0x83E) == true) {
                    b = true;
                    mpWindow->setNumLines(2);
                }
            } else if (field_0x83B == 1) {
                // "Copying complete."
                if (mpWindow->setProperties("SYS_COPY_02", 0, 1, field_0x83E) == true) {
                    b = true;
                }
            } else if (field_0x83C == 1) {
                // "Deleting complete."
                if (mpWindow->setProperties("SYS_DELETE_02", 0, 1, field_0x83E) == true) {
                    b = true;
                }
            } else {
                // "Saved."
                if (mpWindow->setProperties("SYS_SAVE_02", 0, 1, field_0x83E) == true) {
                    b = true;
                }
            }

            if (b == 1) {
                if (!dDvdUnk::FontUnk::GetInstance()->isDiskError()) {
                    dSndSmallEffectMgr_c::GetInstance()->playSound(SE_S_SAVE_FINISH);
                }
                mDelayTimer = 0;
                mStep++;
            }
            break;
        }

        case 4: {
            if (mDelayTimer < getFrameRate() + 30) {
                mDelayTimer++;
            } else {
                if (mpWindow->checkIsWait() == true) {
                    mDelayTimer = 0;
                    mStep++;
                }
            }
            break;
        }
        case 5: {
            if (mpWindow->getWillFinishOut() == 1) {
                if (fontUnk->isNandError() == 1) {
                    fontUnk->clearNandTrackerError();
                }
                field_0x838 = 0;
                field_0x83A = 0;
                field_0x837 = 1;
                endState();
            }
            break;
        }
        case 10: {
            // retry
            initializeSaveBanner();
            break;
        }
        case 20: {
            if (mpWindow->checkIsWait()) {
                mStep++;
            }
            break;
        }
        case 21: {
            initializeError();
            break;
        }
    }
    mpWindow->execute();
}

void SaveMgr::initializeLoadSave() {
    beginState(STATE_LOAD_SAVE);
    mLoadSaveRequest.loadSave(sSaveFileName, sizeof(SavedSaveFiles), 0, nullptr);
}

void SaveMgr::executeLoadSave() {
    FileManager *fileManager = FileManager::GetInstance();
    switch (mStep) {
        case 0: {
            if (!mLoadSaveRequest.isCompleted()) {
                return;
            }
            NANDResult result = mLoadSaveRequest.getResult();
            NandResultTracker *tracker = NandResultTracker::GetInstance();
            if (tracker->isFailure(result)) {
                mLoadSaveRequest.remove();
                if (mLoadSaveRequest.finish()) {
                    initializeError();
                }
                return;
            }
            if (mLoadSaveRequest.getBuf() != nullptr) {
                fileManager->saveSaveData(&mLoadSaveRequest, false);
            }
            mLoadSaveRequest.remove();
            if (!mLoadSaveRequest.finish()) {
                return;
            }
            fileManager->checkFileStatus();
            if (fileManager->isFileInvalid() == true) {
                tracker->isFailure(NAND_RESULT_AUTHENTICATION);
                initializeError();
            } else if (fileManager->getField_0xA842() != 1) {
                mStep = 10;
            } else {
                mStep = 1;
            }
            break;
        }
        case 1: {
            mLoadSaveRequest.loadSave(sSkipDatFileName, 0x80, 0, nullptr);
            mStep = 2;
            break;
        }
        case 2: {
            if (!mLoadSaveRequest.isCompleted()) {
                return;
            }
            NANDResult result = mLoadSaveRequest.getResult();
            NandResultTracker *tracker = NandResultTracker::GetInstance();
            if (tracker->isFailure(result)) {
                mLoadSaveRequest.remove();
                if (mLoadSaveRequest.finish()) {
                    mStep = 30;
                }
                return;
            }
            if (mLoadSaveRequest.getBuf() != nullptr) {
                fileManager->saveSaveData(&mLoadSaveRequest, true);
            }
            mLoadSaveRequest.remove();
            if (mLoadSaveRequest.finish()) {
                if (fileManager->checkSkipDataCRCs() == 1) {
                    mStep = 3;
                } else {
                    fileManager->updateEmptyFiles();
                    field_0x837 = 1;
                    endState();
                }
            }
            break;
        }
        case 3: {
            for (int i = 0; i < 3; i++) {
                fileManager->copyFileSkipData(i);
            }
            mStep++;
            break;
        }
        case 4: {
            fileManager->updateEmptyFiles();
            field_0x837 = 1;
            initializeWriteSkipDat();
            break;
        }
        case 10: {
            mCheckRequest.check(1, 1);
            mStep++;
            break;
        }
        case 11: {
            if (!mCheckRequest.isCompleted()) {
                return;
            }
            NANDResult result = mCheckRequest.getResult();
            if (!NandResultTracker::GetInstance()->isFailure(result) || !mCheckRequest.finish()) {
                u32 check = mCheckRequest.getCheckResult();
                if (mCheckRequest.finish()) {
                    if (check) {
                        fileManager->updateEmptyFiles();
                        field_0x837 = 1;
                        endState();
                    } else {
                        mStep += 1;
                    }
                }
            }
            break;
        }
        case 12: {
            fileManager->setField0xA84D(1);
            mCreateRequest.create(sSkipDatFileName, FILE_PERMS, 0);
            mStep += 1;
            break;
        }
        case 13: {
            if (mCreateRequest.isCompleted()) {
                NANDResult result = mCreateRequest.getResult();
                if (mCreateRequest.finish()) {
                    if (!NandResultTracker::GetInstance()->isFailure(result)) {
                        for (int i = 0; i < 3; i++) {
                            fileManager->copyFileSkipData(i);
                        }
                        fileManager->setField0xA842(1);
                        mStep = 4;
                    }
                }
            }
            break;
        }
        case 30: {
            fileManager->setField0xA84D(1);
            mDeleteRequest.doDelete(sSkipDatFileName);
            mStep += 1;
            break;
        }
        case 31: {
            if (!mDeleteRequest.isCompleted()) {
                return;
            }
            NANDResult result = mDeleteRequest.getResult();
            if (NandResultTracker::GetInstance()->isFailure(result)) {
                if (mDeleteRequest.finish()) {
                    initializeError();
                }

            } else {
                if (mDeleteRequest.finish()) {
                    mStep = 10;
                }
            }

            break;
        }
    }
}

void SaveMgr::initializeWriteSave() {
    beginState(STATE_WRITE);
    mDelayTimer = 0;
    dDvdUnk::FontUnk::GetInstance()->setNeedsPad(false);
    FileManager::GetInstance()->setField0xA84D(1);
    field_0x83D = 1;
}

void SaveMgr::executeWriteSave() {
    FileManager *fileManager = FileManager::GetInstance();
    switch (mStep) {
        case 0: {
            bool anyAction = false;
            if (field_0x83B == 1) {
                // "Copying... Please do not touch the POWER Button or RESET."
                if (mpWindow->setProperties("SYS_COPY_01", 0, 1, field_0x83E)) {
                    anyAction = true;
                }
            } else if (field_0x83C == 1) {
                // "Deleting... Please do not touch the POWER Button or RESET."
                if (mpWindow->setProperties("SYS_DELETE_01", 0, 1, field_0x83E)) {
                    anyAction = true;
                }
            } else {
                // "Saving in progress. Please do not\\ntouch the POWER Button or RESET."
                if (mpWindow->setProperties("SYS_SAVE_01", 0, 1, field_0x83E)) {
                    anyAction = true;
                }
            }

            if (anyAction == true) {
                mStep = 1;
                mDelayTimer = 0;
                mpWindow->setNumLines(2);
            }
            break;
        }
        case 1: {
            if (mDelayTimer >= 30) {
                fileManager->loadSaveData(&mWriteRequest, sSaveFileName, false);
                mDelayTimer = 0;
                mStep++;
            } else {
                mDelayTimer++;
            }
            break;
        }
        case 2: {
            mDelayTimer++;
            if (!mWriteRequest.isCompleted()) {
                return;
            }
            NANDResult result = mWriteRequest.getResult();
            if (NandResultTracker::GetInstance()->isFailure(result)) {
                if (mWriteRequest.finish()) {
                    mStep = 10;
                }
                return;
            }
            if (mWriteRequest.failedWrite() == 1) {
                if (mWriteRequest.finish()) {
                    mStep = 5;
                }
                return;
            }
            if (!mWriteRequest.finish()) {
                return;
            }
            if (fileManager->getField_0xA842() == 1) {
                mStep = 3;
            } else {
                mStep = 40;
            }
            break;
        }
        case 3: {
            fileManager->loadSaveData(&mWriteRequest, sSkipDatFileName, true);
            mStep = 4;
            break;
        }
        case 4: {
            mDelayTimer++;
            if (!mWriteRequest.isCompleted()) {
                return;
            }
            NANDResult result = mWriteRequest.getResult();
            if (NandResultTracker::GetInstance()->isFailure(result)) {
                if (mWriteRequest.finish()) {
                    mStep = 30;
                }
                return;
            }
            if (mWriteRequest.failedWrite() == 1) {
                if (mWriteRequest.finish()) {
                    mStep = 6;
                }
                return;
            }
            if (!mWriteRequest.finish()) {
                return;
            }
            mStep = 40;
            break;
        }
        case 5: {
            fileManager->loadSaveData(&mWriteRequest, sSaveFileName, false);
            mStep = 2;
            mDelayTimer = 0;
            break;
        }
        case 6: {
            fileManager->loadSaveData(&mWriteRequest, sSkipDatFileName, true);
            mStep = 4;
            mDelayTimer = 0;
            break;
        }
        case 10: {
            if (mpWindow->checkIsWait()) {
                initializeError();
            }
            break;
        }
        case 20: {
            if (mpWindow->checkIsWait()) {
                mStep++;
            }
            break;
        }
        case 21: {
            if (mpWindow->getWillFinishOut() == true) {
                fileManager->checkFileStatus();
                if (fileManager->isFileInvalid() == 1) {
                    NandResultTracker::GetInstance()->isFailure(NAND_RESULT_AUTHENTICATION);
                    initializeError();
                    return;
                }
                initializeSaveBanner();
            }
            break;
        }
        case 30: {
            mDelayTimer++;
            mDeleteRequest.doDelete(sSkipDatFileName);
            mStep++;
            break;
        }
        case 31: {
            mDelayTimer++;
            if (!mDeleteRequest.isCompleted()) {
                return;
            }
            NANDResult result = mDeleteRequest.getResult();
            if (NandResultTracker::GetInstance()->isFailure(result)) {
                if (!mDeleteRequest.finish()) {
                    return;
                }
                mpWindow->checkIsWait();
                initializeError();
                return;
            }
            if (!mDeleteRequest.finish()) {
                return;
            }
            fileManager->setField0xA842(0);
            mStep++;
            break;
        }
        case 32: {
            mDelayTimer++;
            mCheckRequest.check(1, 1);
            mStep++;
            break;
        }
        case 33: {
            mDelayTimer++;
            if (!mCheckRequest.isCompleted()) {
                return;
            }
            NANDResult result = mCheckRequest.getResult();
            if (NandResultTracker::GetInstance()->isFailure(result) && mCheckRequest.finish()) {
                mpWindow->checkIsWait();
                initializeError();
            } else {
                u32 result = mCheckRequest.getCheckResult();
                if (!mCheckRequest.finish()) {
                    return;
                }
                if (result) {
                    mStep = 40;
                } else {
                    mStep++;
                }
            }
            break;
        }
        case 34: {
            mDelayTimer++;
            mCreateRequest.create(sSkipDatFileName, FILE_PERMS, 0);
            mStep++;
            break;
        }
        case 35: {
            mDelayTimer++;
            if (!mCreateRequest.isCompleted()) {
                return;
            }
            NANDResult result = mCreateRequest.getResult();
            if (!mCreateRequest.finish()) {
                return;
            }
            if (NandResultTracker::GetInstance()->isFailure(result)) {
                mpWindow->checkIsWait();
                initializeError();
                return;
            }
            for (int i = 0; i < 3; i++) {
                fileManager->copyFileSkipData(i);
            }
            mStep = 3;
            fileManager->setField0xA842(1);
            break;
        }
        case 40: {
            fileManager->checkFileStatus();
            if (fileManager->isFileInvalid() == true) {
                NandResultTracker::GetInstance()->isFailure(NAND_RESULT_UNKNOWN);
                mpWindow->checkIsWait();
                initializeError();
                return;
            }
            u32 oldTimer = mDelayTimer;
            initializeSaveBanner();
            mDelayTimer = oldTimer;
            break;
        }
    }
    mpWindow->execute();
}

void SaveMgr::initializeWriteSkipDat() {
    beginState(STATE_WRITE_SKIP_DAT);
    if (FileManager::GetInstance()->get_0xA84C() || FileManager::GetInstance()->getField_0xA842() != 1) {
        mStep = 10;
    } else {
        FileManager::GetInstance()->setField0xA84D(1);
        FileManager::GetInstance()->copySelectedFileSkipData();
        FileManager::GetInstance()->loadSaveData(&mWriteRequest, sSkipDatFileName, true);
    }
}

void SaveMgr::executeWriteSkipDat() {
    FileManager *fileManager = FileManager::GetInstance();
    switch (mStep) {
        case 0: {
            if (mWriteRequest.isCompleted()) {
                FileManager::GetInstance()->setField0xA84D(0);
                NANDResult result = mWriteRequest.getResult();
                if (NandResultTracker::GetInstance()->isFailure(result)) {
                    if (mWriteRequest.finish()) {
                        mStep = 30;
                    }
                } else {
                    if (mWriteRequest.failedWrite() == true) {
                        if (mWriteRequest.finish()) {
                            mStep = 1;
                        }
                    } else {
                        if (mWriteRequest.finish()) {
                            endState();
                        }
                    }
                }
            }
            break;
        }
        case 1: {
            fileManager->loadSaveData(&mWriteRequest, sSkipDatFileName, true);
            FileManager::GetInstance()->setField0xA84D(1);
            mStep = 0;
            mDelayTimer = 0;
            break;
        }
        case 10: {
            fileManager->setField0xA84D(0);
            endState();
            break;
        }
        case 30: {
            fileManager->setField0xA84D(1);
            mDeleteRequest.doDelete(sSkipDatFileName);
            mStep++;
            break;
        }
        case 31: {
            if (!mDeleteRequest.isCompleted()) {
                return;
            }
            NANDResult result = mDeleteRequest.getResult();
            if (NandResultTracker::GetInstance()->isFailure(result)) {
                if (mDeleteRequest.finish()) {
                    initializeError();
                }
            } else {
                if (mDeleteRequest.finish()) {
                    fileManager->setField0xA842(0);
                    mStep++;
                }
            }
            break;
        }
        case 32: {
            mCheckRequest.check(1, 1);
            mStep++;
            break;
        }
        case 33: {
            if (mCheckRequest.isCompleted()) {
                NANDResult result = mCheckRequest.getResult();
                if (NandResultTracker::GetInstance()->isFailure(result) && mCheckRequest.finish()) {
                    initializeError();
                } else {
                    u32 result = mCheckRequest.getCheckResult();
                    if (mCheckRequest.finish()) {
                        if (result) {
                            mStep = 10;
                        } else {
                            mStep++;
                        }
                    }
                }
            }
            break;
        }
        case 34: {
            mDelayTimer++;
            mCreateRequest.create(sSkipDatFileName, FILE_PERMS, 0);
            mStep++;
            break;
        }
        case 35: {
            if (!mCreateRequest.isCompleted()) {
                return;
            }
            NANDResult result = mCreateRequest.getResult();
            if (!mCreateRequest.finish()) {
                return;
            }
            if (NandResultTracker::GetInstance()->isFailure(result)) {
                initializeError();
                return;
            }
            for (int i = 0; i < 3; i++) {
                fileManager->copyFileSkipData(i);
            }
            fileManager->setField0xA842(1);
            FileManager::GetInstance()->loadSaveData(&mWriteRequest, sSkipDatFileName, true);
            mStep = 0;
            break;
        }
    }
}

void SaveMgr::initializeCopySave() {
    beginState(STATE_COPY_SAVE);
    FileManager::GetInstance()->setField0xA840(0);
    field_0x83B = 1;
}

void SaveMgr::executeCopySave() {
    FileManager *fileManager = FileManager::GetInstance();
    fileManager->copyFile(mCopyFromFile, mCopyToFile);
    fileManager->setSelectedFileNum(mCopyToFile);
    initializeWriteSave();
    FileManager::GetInstance()->setField0xA840(1);
}

void SaveMgr::initializeClearSelectedFile() {
    beginState(STATE_CLEAR_SELECTED_FILE);
    FileManager::GetInstance()->setField0xA840(0);
    field_0x83C = 1;
}

void SaveMgr::executeClearSelectedFile() {
    FileManager::GetInstance()->clearFileA();
    FileManager::GetInstance()->saveFileAToSelectedFile();
    initializeWriteSave();
    FileManager::GetInstance()->setField0xA840(1);
}

void SaveMgr::initializeSave() {
    beginState(STATE_SAVE);
    FileManager::GetInstance()->setField0xA840(0);
    dDvdUnk::FontUnk::GetInstance()->setNeedsPad(false);
    FileManager::GetInstance()->setField_0xA843(1);
}

void SaveMgr::executeSave() {
    FileManager *fileManager = FileManager::GetInstance();
    switch (mStep) {
        case 0: {
            if (fileManager->get_0xA84C() != 1 && fileManager->getField_0xA843()) {
                if (fileManager->getField_0xA841() == 1) {
                    FileManager::GetInstance()->unsetFileANewFile();
                    FileManager::GetInstance()->copyFileAToSelectedFile();
                    initializeWriteSave();
                    FileManager::GetInstance()->setField0xA840(1);
                } else {
                    FileManager::GetInstance()->copyFileAToSelectedFile();
                    FileManager::GetInstance()->setField0xA840(1);
                    initializeCheckForFreeSpace();
                    field_0x838 = 1;
                }
            }
            break;
        }
    }
}

void SaveMgr::initializeDeleteAllData() {
    beginState(STATE_DELETE_ALL_DATA);
    field_0x83A = 1;
}

void SaveMgr::executeDeleteAllData() {
    NandResultTracker *tracker = NandResultTracker::GetInstance();
    switch (mStep) {
        case 0: {
            FileManager::GetInstance()->setField0xA84D(1);
            mDeleteRequest.doDelete(sBannerBinFileName);
            if (field_0x837 != 1 || FileManager::GetInstance()->isFileInvalid() == 1) {
                FileManager::GetInstance()->initBlankSaveFiles();
            }
            mStep = 1;
            break;
        }
        case 1: {
            if (!mDeleteRequest.isCompleted()) {
                return;
            }
            NANDResult result = mDeleteRequest.getResult();
            if (tracker->isFailure(result)) {
                if (mDeleteRequest.finish()) {
                    initializeError();
                }
            } else {
                if (mDeleteRequest.finish()) {
                    mStep = 2;
                }
            }
            break;
        }
        case 2: {
            mDeleteRequest.doDelete(sSaveFileName);
            mStep = 3;
            break;
        }
        case 3: {
            if (!mDeleteRequest.isCompleted()) {
                return;
            }
            NANDResult result = mDeleteRequest.getResult();
            if (tracker->isFailure(result)) {
                if (mDeleteRequest.finish()) {
                    initializeError();
                }
            } else {
                if (mDeleteRequest.finish()) {
                    mStep = 4;
                }
            }
            break;
        }
        case 4: {
            mDeleteRequest.doDelete(sSkipDatFileName);
            mStep = 5;
            break;
        }
        case 5: {
            if (!mDeleteRequest.isCompleted()) {
                return;
            }
            NANDResult result = mDeleteRequest.getResult();
            if (tracker->isFailure(result)) {
                if (mDeleteRequest.finish()) {
                    initializeError();
                }
            } else {
                if (mDeleteRequest.finish()) {
                    initializeCheckForFreeSpace();
                }
            }
            break;
        }
    }
}

void SaveMgr::initializeError() {
    FileManager::GetInstance()->setField0xA84D(0);
    field_0x83D = 0;
    beginState(STATE_ERROR);
    dDvdUnk::FontUnk::GetInstance()->setNeedsPad(true);
    field_0x83F = 0;
}

void SaveMgr::executeError() {
    dLytSystemWindow_c *systemWindow = dLytSystemWindow_c::GetInstance();
    FileManager *fileManager = FileManager::GetInstance();
    switch (mStep) {
        case 0: {
            if (dDvdUnk::FontUnk::GetInstance()->getNandError() == NandResultTracker::ERR_CAT_SAVE_MGR) {
                if (field_0x837 == true) {
                    // "The save data is corrupted. Save data will be restored."
                    if (systemWindow->setProperties("SYS_INIT_05", false, nullptr) == true) {
                        systemWindow->showMaybe();
                        mStep = 20;
                    }
                } else if (field_0x838 == true) {
                    // "Save data could not be created. Try again?[1]Yes[2]No"
                    if (systemWindow->setProperties("SYS_INIT_04", false, nullptr) == true) {
                        systemWindow->showMaybe(4);
                        mStep = 1;
                        dCsBase_c::GetInstance()->setDrawDirectly(true);
                    }
                } else {
                    // "The file cannot be used because nthe data is corrupted. All data saved up to this point will be
                    // lost. Create new save data?[1]Yes[2]No"
                    if (systemWindow->setProperties("SYS_NAND_07", false, nullptr) == true) {
                        systemWindow->showMaybe(4);
                        mStep = 1;
                        dCsBase_c::GetInstance()->setDrawDirectly(true);
                    }
                }
            }
            break;
        }
        case 1: {
            if (systemWindow->getField_0xE10() != 0 && mPad::g_currentCore->downTrigger(/* A */ 0x800) &&
                systemWindow->getField_0xDE0() != 2) {
                if (systemWindow->getField_0xDE0() == 0) {
                    if (systemWindow->fn_80152F60() == true) {
                        mStep = 2;
                    }
                } else {
                    if (systemWindow->fn_80152F50() == true) {
                        mStep = 2;
                    }
                }
            }
            break;
        }
        case 2: {
            if (systemWindow->fn_80152F70() == true) {
                if (systemWindow->getField_0xDE0() == false) {
                    mStep = 3;
                } else {
                    initializeDeleteAllData();
                    dCsBase_c::GetInstance()->setDrawDirectly(false);
                }
            }
            break;
        }
        case 3: {
            if (dScBoot_c::GetInstance() != nullptr) {
                // "You will not be able to save. Do you want to begin your game anyway? [1]Begin[2-]Quit"
                if (systemWindow->setProperties("SYS_SAVE_10", false, nullptr) == true) {
                    systemWindow->showMaybe(4);
                    mpWindow->setNumLines(2);
                    mStep = 4;
                }
            } else {
                // "It is not possible to save. Continue your adventure anyway? [1]Continue[2]Quit"
                if (systemWindow->setProperties("SYS_SAVE_11", false, nullptr) == true) {
                    systemWindow->showMaybe(4);
                    mpWindow->setNumLines(2);
                    mStep = 4;
                }
            }
            break;
        }
        case 4: {
            if (mPad::g_currentCore->downTrigger(/* A */ 0x800) && systemWindow->getField_0xDE0() != 2) {
                if (systemWindow->getField_0xDE0() == 0) {
                    if (systemWindow->fn_80152F60() == true) {
                        mStep = 5;
                    }
                } else {
                    if (systemWindow->fn_80152F50()) {
                        mStep = 10;
                    }
                }
            }
            break;
        }
        case 5: {
            if (systemWindow->fn_80152F70() == true) {
                mStep = 6;
            }
            break;
        }
        case 6: {
            if (field_0x837 == true) {
                // "The save data is corrupted. Save data will be restored."
                if (systemWindow->setProperties("SYS_INIT_05", false, nullptr) == true) {
                    systemWindow->showMaybe(4);
                    mStep = 1;
                }
            } else {
                // "The file cannot be used because nthe data is corrupted. All data saved up to this point will be
                // lost. Create new save data?[1]Yes[2]No"
                if (systemWindow->setProperties("SYS_NAND_07", false, nullptr) == true) {
                    systemWindow->showMaybe(4);
                    mStep = 1;
                }
            }
            break;
        }
        case 10: {
            if (systemWindow->fn_80152F70() == true) {
                dDvdUnk::FontUnk::GetInstance()->clearNandTrackerError();
                if (field_0x837 != true) {
                    fileManager->setField0xA84C(1);
                }
                dCsBase_c::GetInstance()->setDrawDirectly(false);
                endState();
            }
            break;
        }
        case 20: {
            if (systemWindow->fn_80152F70() == true) {
                mStep++;
            }
            break;
        }
        case 21: {
            if (systemWindow->getField_0xDFC() == true && mPad::g_currentCore->downTrigger(/* A */ 0x800) &&
                systemWindow->fn_80152F50() == true) {
                mStep++;
            }
            break;
        }
        case 22: {
            if (systemWindow->fn_80152F70() == true) {
                initializeDeleteAllData();
            }
        }
    }
    if (mpWindow != nullptr) {
        mpWindow->execute();
    }
}

void SaveMgr::initializeNandError() {
    FileManager::GetInstance()->setField0xA84D(0);
    field_0x83D = 0;
    dDvdUnk::FontUnk::GetInstance()->setNeedsPad(true);
    beginState(STATE_NAND_ERROR);
}

void SaveMgr::executeNandError() {
    FileManager *fileManager = FileManager::GetInstance();
    dLytSystemWindow_c *systemWindow = dLytSystemWindow_c::GetInstance();
    switch (mStep) {
        case 0: {
            bool anyAction = false;
            if (mCheckForFreeSpaceResult == 1) {
                // "There is not enough available space in Wii system memory. Create 1 block of free space either by
                // moving data to an SD Card or deleting data in the Wii console's Data Management screen."
                if (systemWindow->setProperties("SYS_NAND_02", false, nullptr) == true) {
                    anyAction = true;
                }
            } else {
                // "There is not enough available space in Wii system memory. Either move data to an SD Card or delete
                // data in the Wii console's Data Management screen."
                if (systemWindow->setProperties("SYS_NAND_03", false, nullptr) == true) {
                    anyAction = true;
                }
            }
            if (anyAction) {
                systemWindow->showMaybe();
                mStep = 1;
            }
            break;
        }
        case 1: {
            if (systemWindow->fn_80152F70() == 1) {
                mStep = 2;
            }
            break;
        }
        case 2: {
            if (systemWindow->getField_0xDFC() == 1 && mPad::g_currentCore->downTrigger(/* A */ 0x800) &&
                systemWindow->fn_80152F60() == 1) {
                mStep = 3;
            }
            break;
        }
        case 3: {
            if (systemWindow->fn_80152F70() == 1) {
                mStep = 4;
            }
            break;
        }
        case 4: {
            // "Do you want to go to the Wii console's Data Management screen to edit data? [1]Yes[2]No"
            if (systemWindow->setProperties("SYS_NAND_04", false, nullptr) == true) {
                systemWindow->showMaybe(4);
                dCsBase_c::GetInstance()->setDrawDirectly(true);
                mStep = 5;
            }
            break;
        }
        case 5: {
            if (systemWindow->fn_80152F70() == 1) {
                mStep = 6;
            }
            break;
        }
        case 6: {
            if (systemWindow->getField_0xE10() != 0) {
                if (!dReset::Manage_c::GetInstance()->FadeOutCalc() &&
                    !dReset::Manage_c::GetInstance()->IsFaderBlank() && mPad::getCore()->downTrigger(mPad::BUTTON_A) &&
                    systemWindow->getField_0xDE0() != 2 && systemWindow->fn_80152F60() == true) {
                    mStep = 7;
                }
            }
            break;
        }
        case 7: {
            if (systemWindow->fn_80152F70() == 1) {
                mStep = 8;
            }
            break;
        }
        case 8: {
            if (systemWindow->getField_0xDE0() == 0) {
                mStep = 9;
            } else {
                dCsBase_c::GetInstance()->setDrawDirectly(false);
                dReset::Manage_c::GetInstance()->SetInteriorReturnDataManager();
                endState();
                field_0x836 = 0;
            }
            break;
        }
        case 9: {
            // "You will not be able to save. Do you want to begin your game anyway? [1]Begin[2-]Quit"
            if (systemWindow->setProperties("SYS_SAVE_10", false, nullptr) == true) {
                systemWindow->showMaybe(4);
                // Why save mgs window and not system window?
                mpWindow->setNumLines(2);
                mStep = 10;
            }
            break;
        }
        case 10: {
            if (systemWindow->fn_80152F70() == 1) {
                mStep = 11;
            }
            break;
        }
        case 11: {
            if (systemWindow->getField_0xE10() != 0 && mPad::g_currentCore->downTrigger(/* A */ 0x800) &&
                systemWindow->getField_0xDE0() != 2) {
                if (systemWindow->getField_0xDE0() == 0) {
                    if (systemWindow->fn_80152F60() == true) {
                        mStep = 12;
                    }
                } else {
                    if (systemWindow->fn_80152F50()) {
                        mStep = 13;
                    }
                }
            }
            break;
        }
        case 12: {
            if (systemWindow->fn_80152F70() == 1) {
                mStep = 0;
            }
            break;
        }
        case 13: {
            if (systemWindow->fn_80152F70() == 1) {
                dCsBase_c::GetInstance()->setDrawDirectly(false);
                dDvdUnk::FontUnk::GetInstance()->clearNandTrackerError();
                fileManager->setField0xA84C(1);
                endState();
            }
            break;
        }
    }
}

void SaveMgr::beginState(SaveMgrState_e state) {
    mCurrentState = state;
    field_0x824 = 2;
    mStep = 0;
    mDelayTimer = 0;
}

void SaveMgr::endState() {
    field_0x836 = 1;
    FileManager::GetInstance()->setField0xA840(1);
    field_0x83D = 0;
    dDvdUnk::FontUnk::GetInstance()->setNeedsPad(true);
    field_0x83B = 0;
    field_0x83C = 0;
    mCurrentState = STATE_MAX;
    field_0x83E = 0;
    field_0x83F = 0;
}

s32 SaveMgr::getFrameRate() {
    return 60 / dSys_c::getFrameRate();
}

NandRequestCheckIsFile::NandRequestCheckIsFile(const char *path) : mIsFile(0) {
    mPath = path;
}

bool NandRequestCheckIsFile::execute() {
    u8 type = 0;
    mStatus = NANDGetType(mPath, &type);
    if (mStatus == NAND_RESULT_OK && type == NAND_FILE_TYPE_FILE) {
        mIsFile = 1;
    } else {
        mIsFile = 0;
    }
    return true;
}

bool NandRequestCheckIsFileHolder::checkIsFile(const char *path) {
    if (mpRequest != nullptr) {
        return false;
    }
    NandRequestCheckIsFile *req = new NandRequestCheckIsFile(path);
    if (req == nullptr) {
        return false;
    }
    NandRequestThread::GetInstance()->enqueueRequest(req);
    mpRequest = req;
    return true;
}

bool NandRequestCheckIsFileHolder::getCheckResult() const {
    return static_cast<NandRequestCheckIsFile *>(mpRequest)->mIsFile;
}

NandRequestSaveBanner::NandRequestSaveBanner(
    const char *filePath, TPLPalette *palette, const wchar_t *title, const wchar_t *subtitle, u8 perm, u8 attr
)
    : NandRequestCreate(filePath, perm, attr),
      mpPalette(palette),
      mpTitle(title),
      mpSubTitle(subtitle),
      mFailed(false) {}

bool NandRequestSaveBanner::execute() {
    if (!NandRequestCreate::execute()) {
        return true;
    }

    char homeDir[64];
    NANDFileInfo info;
    mStatus = NANDOpen(mFilePath, &info, NAND_ACCESS_WRITE);
    if (mStatus != NAND_RESULT_OK) {
        return mStatus;
    }

    if (writeBannerTpl(&info) != 0x72A0) {
        NANDResult res = NANDClose(&info);
        if (res != NAND_RESULT_OK) {
            mStatus = res;
        }
        mFailed = true;
        return false;
    }

    mStatus = NANDClose(&info);
    mStatus = NANDGetHomeDir(homeDir);
    if (mStatus != NAND_RESULT_OK) {
        return mStatus;
    }

    mStatus = NANDMove(mFilePath, homeDir);
    return true;
}

#define NUM_BANNER_ICONS 1

u32 NandRequestSaveBanner::writeBannerTpl(NANDFileInfo *info) {
    static NANDBanner sBanner;
    NANDInitBanner(&sBanner, 0, getTitle(), getSubTitle());
    TPLPalette *palette = getPalette();
    TPLDescriptor *tpl = TPLGet(palette, 0);
    std::memcpy(&sBanner.bannerTexture, tpl->texHeader->data, NAND_BANNER_TEXTURE_SIZE);
    for (s32 i = 0; i < NUM_BANNER_ICONS;) {
        TPLDescriptor *tpl = TPLGet(palette, i + 1);
        std::memcpy(sBanner.iconTexture[0], tpl->texHeader->data, NAND_BANNER_ICON_TEXTURE_SIZE);
        // ah yes
        s32 idx = i++;
        NAND_SET_ICON_SPEED(&sBanner, idx, 2);
    }
    NAND_SET_ICON_SPEED(&sBanner, NUM_BANNER_ICONS, 0);
    // How is this calculated?
    return NANDWrite(info, &sBanner, 0x72A0);
}

bool NandRequestSaveBannerHolder::saveBanner(
    const char *filePath, TPLPalette *palette, const wchar_t *title, const wchar_t *subtitle, u8 perm, u8 attr
) {
    if (mpRequest != nullptr) {
        return false;
    }
    NandRequestSaveBanner *req = new NandRequestSaveBanner(filePath, palette, title, subtitle, perm, attr);
    if (req == nullptr) {
        return false;
    }
    NandRequestThread::GetInstance()->enqueueRequest(req);
    mpRequest = req;
    return true;
}

bool NandRequestSaveBannerHolder::hasFailed() const {
    return static_cast<NandRequestSaveBanner *>(mpRequest)->mFailed;
}

NandRequestLoadSaveFile::NandRequestLoadSaveFile(const char *path, s32 readSize, s32 mountDirection, EGG::Heap *heap)
    : mpHeap(heap), mpBuf(nullptr), mBufLen(0), mMountDirection(mountDirection), mReadSize(readSize), mUnused(false) {
    mPath = path;
    if (mpHeap == nullptr) {
        mpHeap = dHeap::work2Heap.heap;
    }
}

bool NandRequestLoadSaveFile::execute() {
    NANDFileInfo info;
    mStatus = NANDOpen(mPath, &info, NAND_ACCESS_READ);
    if (mStatus != NAND_RESULT_OK) {
        return true;
    }

    u32 length;
    mStatus = NANDGetLength(&info, &length);
    if (mStatus != NAND_RESULT_OK) {
        mStatus = NAND_RESULT_AUTHENTICATION; // ??
        NANDResult res = NANDClose(&info);
        if (res != NAND_RESULT_OK) {
            mStatus = res;
        }
        return true;
    }

    if (length != mReadSize) {
        mStatus = NAND_RESULT_AUTHENTICATION; // ??
        NANDResult res = NANDClose(&info);
        if (res != NAND_RESULT_OK) {
            mStatus = res;
        }
        return true;
    }
    s32 align = mMountDirection == 0 ? 0x20 : -0x20;
    mpBuf = mpHeap->alloc(length, align);
    if (mpBuf == nullptr) {
        mStatus = NAND_RESULT_AUTHENTICATION; // ??
        NANDResult res = NANDClose(&info);
        if (res != NAND_RESULT_OK) {
            mStatus = res;
        }
        return false;
    }

    mBufLen = length;
    mStatus = NANDRead(&info, mpBuf, mBufLen);
    NANDResult res = NANDClose(&info);
    if (mStatus != mBufLen && mStatus >= NAND_RESULT_OK) {
        mStatus = NAND_RESULT_AUTHENTICATION; // ??
    } else if (mStatus != NAND_RESULT_ECC_CRIT && mStatus != NAND_RESULT_AUTHENTICATION) {
        mStatus = res;
    }
    return true;
}

void NandRequestLoadSaveFile::remove() {
    if (mpBuf != nullptr) {
        mpHeap->free(mpBuf);
        mpBuf = nullptr;
        mBufLen = 0;
    }
}

bool NandRequestLoadSaveFileHolder::loadSave(const char *path, s32 readSize, s32 mountDirection, EGG::Heap *heap) {
    if (mpRequest != nullptr) {
        return false;
    }
    NandRequestLoadSaveFile *req = new NandRequestLoadSaveFile(path, readSize, mountDirection, heap);
    if (req == nullptr) {
        return false;
    }
    NandRequestThread::GetInstance()->enqueueRequest(req);
    mpRequest = req;
    return true;
}

void *NandRequestLoadSaveFileHolder::getBuf() const {
    return static_cast<NandRequestLoadSaveFile *>(mpRequest)->mpBuf;
}

u32 NandRequestLoadSaveFileHolder::getLen() const {
    return static_cast<NandRequestLoadSaveFile *>(mpRequest)->mBufLen;
}

void NandRequestLoadSaveFileHolder::remove() {
    static_cast<NandRequestLoadSaveFile *>(mpRequest)->remove();
}

NandRequestDelete::NandRequestDelete(const char *path) {
    mPath = path;
}

bool NandRequestDelete::execute() {
    mStatus = NANDDelete(mPath);
    return true;
}

bool NandRequestDeleteHolder::doDelete(const char *path) {
    if (mpRequest != nullptr) {
        return false;
    }
    NandRequestDelete *req = new NandRequestDelete(path);
    if (req == nullptr) {
        return false;
    }
    NandRequestThread::GetInstance()->enqueueRequest(req);
    mpRequest = req;
    return true;
}
