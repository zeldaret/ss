#include "d/d_dvd_unk.h"

#include "common.h"
#include "d/d_cs_base.h"
#include "d/d_cursor_hit_check.h"
#include "d/d_dvd_drive_error.h"
#include "d/d_gfx.h"
#include "d/d_hbm.h"
#include "d/d_pad.h"
#include "d/d_pad_manager.h"
#include "d/d_s_boot.h"
#include "d/d_video.h"
#include "d/lyt/d_lyt_system_window.h"
#include "d/snd/d_snd_player_mgr.h"
#include "egg/core/eggHeap.h"
#include "egg/gfx/eggTextureBuffer.h"
#include "rvl/NAND/nand.h"
#include "toBeSorted/nand_result_tracker.h"

// included asset
#include "d/dol_backup_error_font.inc"

namespace dDvdUnk {

FontUnk *FontUnk::sInstance;

void FontUnk::create(EGG::Heap *heap) {
    sInstance = new (heap, 0x04) FontUnk();
    sInstance->init();
    dDvdDriveError_c::create(heap);
    NandResultTracker::create(heap);
    dPadManager_c::create(heap);
    sInstance->field_0x2C = 0;
}

void FontUnk::execute() {
    if (dHbm::Manage_c::GetInstance()->getState() == dHbm::Manage_c::HBM_MANAGE_ACTIVE) {
        return;
    }

    dDvdDriveError_c *driveError = dDvdDriveError_c::GetInstance();
    dPadManager_c *padManager = dPadManager_c::GetInstance();
    NandResultTracker *nandTracker = NandResultTracker::GetInstance();

    // Errors in decreasing importance
    switch (mErrorType) {
        case ERROR_DISK: driveError->execute(); break;
        case ERROR_PAD:
            driveError->execute();
            padManager->execute();
            break;
        case ERROR_NAND:
            driveError->execute();
            padManager->execute();
            nandTracker->execute();
            break;
        default:
            driveError->execute();
            nandTracker->execute();
            padManager->execute();
            break;
    }
    if (checkFlag(FLAG_ERROR_REQUEST)) {
        offFlag(FLAG_ERROR_REQUEST);
        EGG::TextureBuffer *buf = dGfx_c::GetInstance()->getTextureBuffer();
        if (buf != nullptr) {
            buf->capture(0, 0, false, -1);
        }
        onFlag(FLAG_ERROR_STATE);
    }
}

void FontUnk::drawNonDriveError() {
    switch (mErrorType) {
        case ERROR_PAD:  dPadManager_c::GetInstance()->draw(); break;
        case ERROR_NAND: NandResultTracker::GetInstance()->draw(); break;
        default:         break;
    }
}

void FontUnk::drawDriveError() {
    if (mIsDiskError == true) {
        dDvdDriveError_c::GetInstance()->draw();
    }
}

void FontUnk::init() {
    mIsDiskError = false;
    mIsNandError = false;
    mIsPadError = false;
    mFlags = 0;
    mDiskErrorCode = 0;
    mNandErrorCode = 0;
    field_0x2B = false;
    mErrorType = ERROR_NONE;
    mFont.SetResource(DOL_BACKUP_FONT);
}

void dDvdUnk::FontUnk::fn_800529B0() {
    mIsPadError = false;
    field_0x2B = false;
    if (dCsBase_c::GetInstance() == nullptr) {
        return;
    }
    dCsBase_c::GetInstance()->setVisible(false);
    dCsBase_c::GetInstance()->setDrawDirectly(false);
    dCsBase_c::GetInstance()->setCalibrationPointCenterEnabled(false);
}

void dDvdUnk::FontUnk::clearNandError() {
    clearNandTrackerError();
    mIsNandError = false;
    mNandErrorCode = 0;
}

void dDvdUnk::FontUnk::fn_80052A20() {
    if (dScBoot_c::GetInstance() == nullptr && !dPadManager_c::GetInstance()->isCalibrationFinished()) {
        dPad::ex_c::setCalibrateMpls();
    }

    if (checkFlag(FLAG_ERROR_STATE)) {
        dGfx_c::GetInstance()->releaseTextureBuffer();
        dSndPlayerMgr_c::GetInstance()->leaveCaution();
    }

    if (dLytSystemWindow_c::GetInstance() != nullptr && dLytSystemWindow_c::GetInstance()->fn_80152F80()) {
        dLytSystemWindow_c::GetInstance()->fn_80152F10();
    }

    mIsDiskError = false;
    mIsNandError = false;
    mIsPadError = false;
    mFlags = false;
    mDiskErrorCode = 0;
    mNandErrorCode = 0;
    field_0x2B = false;
    mErrorType = ERROR_NONE;

    dPadManager_c::GetInstance()->init();
    NandResultTracker::GetInstance()->init();
}

void FontUnk::preExecute() {
    if (dHbm::Manage_c::GetInstance()->getState() != dHbm::Manage_c::HBM_MANAGE_ACTIVE) {
        dDvdDriveError_c *drive = dDvdDriveError_c::GetInstance();
        NandResultTracker *nand = NandResultTracker::GetInstance();
        dPadManager_c *pad = dPadManager_c::GetInstance();
        bool prevError = isAnyError();
        mIsDiskError = drive->isError();
        mDiskErrorCode = drive->getStatus();
        mIsNandError = nand->isError();
        mNandErrorCode = nand->getErrorCategory();

        if (padErrorsAllowed() == true) {
            mIsPadError = pad->isError();
        }

        // Errors in decreasing importance
        if (mIsDiskError == true) {
            mErrorType = ERROR_DISK;
        } else if (mIsPadError == true) {
            mErrorType = ERROR_PAD;
        } else if (mIsNandError == true) {
            mErrorType = ERROR_NAND;
        } else {
            mErrorType = ERROR_NONE;
        }

        if (prevError && !isAnyError()) {
            dGfx_c::GetInstance()->releaseTextureBuffer();
            offFlag(FLAG_ERROR_STATE);
            dSndPlayerMgr_c::GetInstance()->leaveCaution();
        }

        if (field_0x2C == true && !prevError && isAnyError()) {
            dSndPlayerMgr_c::GetInstance()->enterCaution();
        }
    }
}

bool FontUnk::isAnyError() const {
    return mIsDiskError | mIsNandError | mIsPadError;
}

void FontUnk::onError() {
    if (checkFlag(FLAG_ERROR_STATE)) {
        return;
    }
    onFlag(FLAG_ERROR_REQUEST);
    dVideo::enableDimming(1);
}

void FontUnk::clearNandTrackerError() {
    NandResultTracker::GetInstance()->isFailure(NAND_RESULT_OK);
}

void FontUnk::fn_80052CC0() {
    setNeedsPad(true);
    field_0x2B = true;
}

void FontUnk::setNeedsPad(bool b) {
    if (dPadManager_c::GetInstance() != nullptr) {
        dPadManager_c::GetInstance()->setField_0x1F(b);
    }
}

bool FontUnk::padErrorsAllowed() {
    if (field_0x2B != true) {
        return false;
    }

    if (dPadManager_c::GetInstance() != nullptr) {
        return dPadManager_c::GetInstance()->getField_0x1F();
    }

    return false;
}

void FontUnk::fn_80052D50() {
    if (dPadManager_c::GetInstance()->isError() == true) {
        if (checkFlag(FLAG_ERROR_STATE)) {
            dGfx_c::GetInstance()->releaseTextureBuffer();
            mFlags = 0;
            dSndPlayerMgr_c::GetInstance()->leaveCaution();
        }
        dPadManager_c::GetInstance()->init();
    }
    setNeedsPad(false);
    dVideo::enableDimming(0);
}

void FontUnk::fn_80052DD0() {
    setNeedsPad(true);
    dVideo::enableDimming(1);
}

} // namespace dDvdUnk
