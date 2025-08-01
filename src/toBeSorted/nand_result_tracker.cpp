#include "toBeSorted/nand_result_tracker.h"

#include "common.h"
#include "d/d_dvd_unk.h"
#include "d/d_gfx.h"
#include "d/lyt/d_lyt_system_window.h"
#include "egg/core/eggHeap.h"

NandResultTracker *NandResultTracker::sInstance;

void NandResultTracker::create(EGG::Heap *heap) {
    sInstance = new (heap) NandResultTracker();
    sInstance->init();
}

NandResultTracker *NandResultTracker::GetInstance() {
    return sInstance;
}

bool NandResultTracker::isFailure(NANDResult status) {
    bool ret = true;

    switch (status) {
        case NAND_RESULT_OK:
        case NAND_RESULT_EXISTS:
        case NAND_RESULT_NOEXISTS: {
            mErrorCategory = ERR_CAT_NONE;
            ret = false;
            break;
        }
        case NAND_RESULT_ECC_CRIT:
        case NAND_RESULT_AUTHENTICATION: {
            mErrorCategory = ERR_CAT_SAVE_MGR;
            break;
        }
        default: {
            mErrorCategory = ERR_CAT_FATAL;
            break;
        }
    }

    return ret;
}

void NandResultTracker::draw() {
    dGfx_c::doDrawCapTexture();
}

void NandResultTracker::execute() {
    static const ModeFunc procs[] = {
        &NandResultTracker::ModeProc_Normal,
        &NandResultTracker::ModeProc_Error,
    };
    ModeFunc f = procs[mMode];
    if (f != nullptr) {
        (this->*f)();
    }
}

void NandResultTracker::init() {
    mErrorCategory = ERR_CAT_NONE;
    mIsError = false;
    ModeRequestNormal();
}

void NandResultTracker::ModeRequestNormal() {
    ModeRequest(Normal);
}

void NandResultTracker::ModeProc_Normal() {
    if (mErrorCategory != ERR_CAT_NONE && mErrorCategory != ERR_CAT_SAVE_MGR) {
        mIsError = true;
        dDvdUnk::FontUnk::GetInstance()->onError();
    } else {
        mIsError = false;
    }

    if (mErrorCategory != ERR_CAT_NONE && mErrorCategory != ERR_CAT_SAVE_MGR) {
        ModeRequestError();
    }
}

void NandResultTracker::ModeRequestError() {
    ModeRequest(Error);
}

void NandResultTracker::ModeProc_Error() {
    // Note: Only index 3 (SYS_NAND_12) is reachable apparently
#pragma push
#pragma readonly_strings on
    static const char *sMessages[] = {
        // "The Wii system memory has been damaged. Refer to the Wii Operations Manual for details."
        "SYS_NAND_08",
        "SYS_NAND_08",
        // "Could not access Wii system memory. Refer to the Wii Operations Manual for details."
        "SYS_NAND_11",
        // "An error occurred while accessing Wii system memory. Refer to the Wii Operations Manual for details."
        "SYS_NAND_12",
        "SYS_NAND_12",
    };
#pragma pop
    dLytSystemWindow_c *window = dLytSystemWindow_c::GetInstance();
    s32 error = GetInstance()->getErrorCategory();

    switch (mStep) {
        case 0:
            if (window->setProperties(sMessages[error], false, nullptr) == true) {
                mStep = 1;
            }
            break;
        default: break;
    }
}

void NandResultTracker::ModeRequest(Mode_e mode) {
    mMode = mode;
    mStep = 0;
}
