#include "d/d_reset.h"

#include "common.h"
#include "d/d_cs_base.h"
#include "d/d_dvd_unk.h"
#include "d/d_hbm.h"
#include "d/d_rumble.h"
#include "d/d_s_boot.h"
#include "d/d_sc_title.h"
#include "d/d_title_manager.h"
#include "d/lyt/d_lyt_system_window.h"
#include "d/snd/d_snd_player_mgr.h"
#include "egg/core/eggColorFader.h"
#include "egg/core/eggController.h"
#include "egg/core/eggFader.h"
#include "egg/core/eggHeap.h"
#include "m/m_color.h"
#include "m/m_pad.h"
#include "m/m_video.h"
#include "rvl/GX/GXDraw.h"
#include "rvl/GX/GXMisc.h"
#include "rvl/OS/OSCache.h"
#include "rvl/OS/OSInterrupt.h"
#include "rvl/OS/OSReset.h"
#include "rvl/OS/OSStateTM.h"
#include "rvl/OS/OSThread.h"
#include "rvl/VI/vi.h"
#include "toBeSorted/file_manager.h"
#include "toBeSorted/nand_result_tracker.h"
#include "toBeSorted/save_manager.h"

#include "rvl/OS.h"

namespace dReset {

mColor Manage_c::c_faderColor(0, 0, 0, 0xFF);
Manage_c *Manage_c::s_pInstance;

void Manage_c::CreateInstance(EGG::Heap *pHeap) {
    s_pInstance = new (pHeap) Manage_c(pHeap);
}

Manage_c *Manage_c::GetInstance() {
    return s_pInstance;
}

Manage_c::Manage_c(EGG::Heap *pHeap) {
    mpHeap = pHeap;
    mpFader = nullptr;
    mMode = Normal;
    field_0x10 = 0;
    mModePrev = Normal;
    mExec = ExecNone;
    field_0x1C = 0;
    mHbmReset = false;
    mHbmReturnMenu = false;
    mRestart = false;
    mShutdown = false;
    mInteriorReturnDataManager = false;
    field_0x58 = 0;
    field_0x5C = 0;
    field_0x60 = 0;
    mBootComplete = 0;
    mSoftReset = 0;
    field_0x66 = 0;
    field_0x67 = 0;
    Configure();
}

void Manage_c::Configure() {
    mpFader = new (mpHeap) EGG::ColorFader(
        0.f, 0.f, mVideo::m_video->pRenderMode->fbWidth, mVideo::m_video->pRenderMode->efbHeight, c_faderColor,
        EGG::ColorFader::STATUS_PREPARE_OUT
    );
    SetResetCallback();
    SetPowerCallback();
}

void Manage_c::Reset() {
    mMode = Normal;
    field_0x10 = 0;
    mExec = ExecNone;
    field_0x1C = 0;
    SetResetCallback();
    SetPowerCallback();
    mHbmReset = false;
    mHbmReturnMenu = false;
    mRestart = false;
    mShutdown = false;
    mInteriorReturnDataManager = false;
    field_0x58 = 0;
    field_0x5C = 0;
    field_0x60 = 0;
    mSoftReset = 0;
    field_0x67 = 0;
}

void Manage_c::Calculate() {
    ModeProc();
    if (!SaveMgr::GetInstance()->getField_0x83D() && !SaveMgr::GetInstance()->getField_0x83F()) {
        if (FileManager::GetInstance()->getField_0xA84D() != 1) {
            mpFader->calc();
        }
    }
}

void Manage_c::Draw() {
    mpFader->draw();
}

void Manage_c::BootComplete(bool complete) {
    mBootComplete = complete;
}

bool Manage_c::CanExecSoftReset() const {
    return mMode == SoftReset && mpFader->getStatus() == EGG::Fader::STATUS_PREPARE_IN && field_0x60 == 0 &&
           dHbm::Manage_c::GetInstance()->getState() != dHbm::Manage_c::HBM_MANAGE_ACTIVE &&
           dSndPlayerMgr_c::GetInstance()->fn_8035E2E0() && FileManager::GetInstance()->getField_0xA84D() != 1;
}

void Manage_c::SetSoftResetFinish() {
    if (mMode == SoftReset) {
        if (field_0x60 == 1) {
            field_0x60 = 2;
        }
    }
}

void Manage_c::SetSoftResetScene() {
    if (CanExecSoftReset() && dScBoot_c::GetInstance() == nullptr) {
        dScTitle_c::loadTitleScreen(0);
        field_0x60 = 1;
    }
}

void Manage_c::PostDeleteScene() {}

bool Manage_c::IsFaderBlank() const {
    return mpFader->getStatus() == EGG::Fader::STATUS_PREPARE_IN;
}

void Manage_c::SetHbmReturnMenu() {
    mHbmReturnMenu = true;
}

void Manage_c::SetHbmReset() {
    mHbmReset = true;
}

void Manage_c::SetDelayReset() {
    field_0x58 = 2;
}

void Manage_c::SetDelayPowerOff() {
    field_0x5C = 2;
}

void Manage_c::SetInteriorReturnDataManager() {
    mInteriorReturnDataManager = true;
}

void Manage_c::fn_80067FF0() {
    MotorCancelOn();
    CleanUpGX();
    CleanUpLC();
}

void Manage_c::ExecProcPre() {
    CleanUpGX();
    CleanUpLC();
}

void Manage_c::fn_80068070() {
    ExecProcMain();
}

void Manage_c::ExecProcMain() {
    if (mExec == Restart) {
        OSRestart(0);
    } else if (mExec == Reboot) {
        OSRebootSystem();
    } else if (mExec == ReturnToMenu) {
        OSReturnToMenu();
    } else if (mExec == ReturnToDataManager) {
        OSReturnToDataManager();
    } else if (mExec == Shutdown) {
        OSShutdownSystem();
    }
}

void Manage_c::DiskCheckModeRequest(bool) {
    if (mBootComplete) {
        SetDiskCheckCallback();
        while (true) {
            if (field_0x1C == 2) {
                ModeRequestSafetyWait(ReturnToMenu);
                return;
            } else if (field_0x1C == 1) {
                ModeRequestSoftReset();
                return;
            } else {
                VIWaitForRetrace();
            }
        }
    } else {
        SetDiskCheckCallback();
        while (true) {
            if (field_0x1C == 2) {
                ModeRequestSafetyWait(ReturnToMenu);
                return;
            } else if (field_0x1C != 1) {
                VIWaitForRetrace();
            } else {
                if (dScBoot_c::GetInstance()) {
                    ModeRequestSoftReset();
                    dHbm::Manage_c::GetInstance()->fn_80197560(1);
                    field_0x1C = 0;
                }
                return;
            }
        }
    }
}

void Manage_c::DebugPrintMode(const char *) {}

void Manage_c::ModeProc() {
    bool fatalErr = IsFatalError();
    bool diskErr = IsDiskError();
    if (fatalErr) {
        if (mMode != FatalError) {
            ModeRequest(FatalError);
        }
    } else if (diskErr) {
        if (mMode != DiskWait && mMode != SafetyWait) {
            ModeRequest(DiskWait);
        }
    } else if (mMode == Normal &&
               dHbm::Manage_c::GetInstance()->getState() == dHbm::Manage_c::HBM_MANAGE_ACTIVE) {
        ModeRequest(HbmWait);
    }
    static void (Manage_c::*const procs[6])() = {
        &Manage_c::ModeProc_Normal,   &Manage_c::ModeProc_SoftReset,  &Manage_c::ModeProc_HbmWait,
        &Manage_c::ModeProc_DiskWait, &Manage_c::ModeProc_FatalError, &Manage_c::ModeProc_SafetyWait,
    };

    (this->*procs[mMode])();

    if (mRestart) {
        SetResetCallback();
    }
    if (mShutdown) {
        SetPowerCallback();
    }
    mHbmReset = false;
    mHbmReturnMenu = false;
    mRestart = false;
    mShutdown = false;
    mInteriorReturnDataManager = false;
}

void Manage_c::ModeRequest(Manage_c::Mode_e mode) {
    mModePrev = mMode;
    mMode = mode;
    static void (Manage_c::*const procs[6])() = {
        &Manage_c::ModeInit_Normal,   &Manage_c::ModeInit_SoftReset,  &Manage_c::ModeInit_HbmWait,
        &Manage_c::ModeInit_DiskWait, &Manage_c::ModeInit_FatalError, &Manage_c::ModeInit_SafetyWait,
    };
    (this->*procs[mMode])();
}

void Manage_c::ModeRequestSafetyWait(Manage_c::Exec_e exec) {
    ModeRequest(SafetyWait);
    mExec = exec;
    MotorCancelOn();
}

void Manage_c::ModeRequestSoftReset() {
    mSoftReset = 1;
    ModeRequest(SoftReset);
    MotorCancelOn();
}

void Manage_c::ModeInit_Normal() {
    DebugPrintMode("Normal");
    mExec = ExecNone;
}

void Manage_c::ModeProc_Normal() {
    Exec_e exec;
    if (mShutdown) {
        ModeRequestSafetyWait(Shutdown);
    } else if (mInteriorReturnDataManager) {
        ModeRequestSafetyWait(ReturnToDataManager);
    } else if (mRestart) {
        DiskCheckModeRequest(false);
    }
}

extern "C" void fn_803267B0(s32, s32);
void Manage_c::ModeInit_SoftReset() {
    DebugPrintMode("SoftReset");
    if (mSoftReset) {
        mSoftReset = false;
        if (dScBoot_c::GetInstance()) {
            mpFader->setFrame(30);
            mpFader->fadeOut();
            field_0x66 = true;
            dSndPlayerMgr_c::GetInstance()->fn_8035E250(30);
        } else {
            field_0x60 = 0;
            if (dTitleMgr_c::GetInstance()) {
                if (dTitleMgr_c::GetInstance()->getField_0xBC() == 0) {
                    dSndPlayerMgr_c::GetInstance()->fn_8035E250(30);
                }
            } else {
                dSndPlayerMgr_c::GetInstance()->fn_8035E250(30);
            }
            fn_803267B0(0, 60);
            mpFader->setFrame(30);
            mpFader->fadeOut();
            field_0x66 = true;
            field_0x67 = false;
        }
    }
}

void Manage_c::ModeProc_SoftReset() {
    if (dScBoot_c::GetInstance()) {
        if (mpFader->getStatus() == EGG::ColorFader::STATUS_PREPARE_IN) {
            if (dScBoot_c::GetInstance()->getField_0x5E4()) {
                dScBoot_c::GetInstance()->setField_0x5E4(false);
                mpFader->setStatus(EGG::ColorFader::STATUS_PREPARE_OUT);
                ModeRequest(Normal);
                dSndPlayerMgr_c::GetInstance()->fn_8035E310();
            } else {
                if (field_0x66) {
                    dScBoot_c::GetInstance()->setField_0x5E3(true);

                    dDvdUnk::FontUnk::GetInstance()->fn_800529B0();
                    dSndPlayerMgr_c::GetInstance()->fn_8035E6E0();

                    if (dDvdUnk::FontUnk::GetInstance()->isNandError() ||
                        dDvdUnk::FontUnk::GetInstance()->getNandError() != NandResultTracker::ERR_CAT_NONE ||
                        SaveMgr::GetInstance()->getCheckForFreeSpaceResult()) {
                        dScBoot_c::GetInstance()->setField_0x5E3(false);
                        dScBoot_c::GetInstance()->setField_0x5E2(true);
                        dDvdUnk::FontUnk::GetInstance()->clearNandError();
                    }
                    dDvdUnk::FontUnk::GetInstance()->fn_80052A20();
                }
                field_0x66 = false;
            }
        }
    } else if (dTitleMgr_c::GetInstance() == nullptr || dTitleMgr_c::GetInstance()->getField_0xBC() == 0) {
        if (field_0x66 && mpFader->getStatus() == EGG::ColorFader::STATUS_PREPARE_IN) {
            dDvdUnk::FontUnk::GetInstance()->fn_800529B0();
            dDvdUnk::FontUnk::GetInstance()->fn_80052A20();

            dCsBase_c::GetInstance()->setVisible(false);
            dCsBase_c::GetInstance()->setDrawDirectly(false);
            dCsBase_c::GetInstance()->setCalibrationPointCenterEnabled(false);

            field_0x66 = false;
        }
        if (mpFader->getStatus() == EGG::Fader::STATUS_PREPARE_IN && field_0x60 == 2) {
            if (field_0x67 == 0) {
                if (SaveMgr::GetInstance()->isInState()) {
                    SaveMgr::GetInstance()->init();
                }
                if (dLytSystemWindow_c::GetInstance()->fn_80152F80()) {
                    dLytSystemWindow_c::GetInstance()->fn_80152F10();
                }
                field_0x67++;
            } else {
                mpFader->setStatus(EGG::Fader::STATUS_PREPARE_OUT);
                VISetBlack(FALSE);
                MotorCancelOff();
                Reset();
            }
        }
    }
}

void Manage_c::ModeInit_HbmWait() {
    DebugPrintMode("HbmWait");
}

void Manage_c::ModeProc_HbmWait() {
    bool doFade = false;
    if (field_0x5C == 0 && field_0x58 == 0) {
        if (mHbmReturnMenu) {
            ModeRequestSafetyWait(ReturnToMenu);
        } else if (mHbmReset) {
            DiskCheckModeRequest(true);
            doFade = true;
        } else {
            if (dHbm::Manage_c::GetInstance()->getState() != dHbm::Manage_c::HBM_MANAGE_ACTIVE) {
                if (mShutdown) {
                    ModeRequestSafetyWait(Shutdown);
                } else if (mRestart) {
                    DiskCheckModeRequest(false);
                } else {
                    ModeRequest(Normal);
                }
            } else {
                if (mShutdown) {
                    if (dHbm::Manage_c::GetInstance()->fn_801973C0(2)) {
                        field_0x5C = 1;
                    }
                } else if (mRestart) {
                    if (dHbm::Manage_c::GetInstance()->fn_801973C0(1)) {
                        field_0x58 = 1;
                    }
                }
            }
        }
    } else if (field_0x5C == 2) {
        field_0x5C = 0;
        ModeRequestSafetyWait(Shutdown);
        doFade = true;
    } else if (field_0x58 == 2) {
        field_0x58 = 0;
        DiskCheckModeRequest(false);
        doFade = true;
    }
    if (doFade) {
        mpFader->setStatus(EGG::Fader::STATUS_PREPARE_IN);
    }
}

void Manage_c::ModeInit_DiskWait() {
    if (dScBoot_c::GetInstance()) {
        mFaderStatusPrev = mpFader->getStatus();
        mpFader->setStatus(EGG::Fader::STATUS_PREPARE_OUT);
    } else {
        if (mModePrev == SoftReset) {
            mpFader->setStatus(EGG::Fader::STATUS_PREPARE_OUT);
        }
    }
    DebugPrintMode("DiskWait");
}

void Manage_c::ModeProc_DiskWait() {
    if (IsDiskError()) {
        if (mShutdown) {
            ModeRequestSafetyWait(Shutdown);
        } else if (mRestart) {
            ModeRequestSafetyWait(ReturnToMenu);
        }
    } else {
        if (dScBoot_c::GetInstance()) {
            if (mFaderStatusPrev == EGG::Fader::STATUS_PREPARE_IN) {
                mpFader->setStatus(EGG::Fader::STATUS_PREPARE_IN);
            } else if (mFaderStatusPrev == EGG::Fader::STATUS_PREPARE_OUT) {
                mpFader->setStatus(EGG::Fader::STATUS_PREPARE_OUT);
            } else if (mFaderStatusPrev == EGG::Fader::STATUS_FADE_IN) {
                mpFader->setStatus(EGG::Fader::STATUS_PREPARE_OUT);
            } else {
                mpFader->setStatus(EGG::Fader::STATUS_PREPARE_IN);
            }
        } else {
            if (mModePrev == SoftReset) {
                mpFader->setStatus(EGG::Fader::STATUS_PREPARE_IN);
            }
        }
        ModeRequest(mModePrev);
    }
}

void Manage_c::ModeInit_FatalError() {
    DebugPrintMode("FatalError");
}

void Manage_c::ModeProc_FatalError() {}

void Manage_c::ModeInit_SafetyWait() {
    DebugPrintMode("SafetyWait");
    dSndPlayerMgr_c::GetInstance()->fn_8035E1B0(30);
    mpFader->setFrame(30);
    mpFader->fadeOut();
}

void Manage_c::ModeProc_SafetyWait() {
    if (mpFader->getStatus() == EGG::Fader::STATUS_PREPARE_IN) {
        bool b = dSndPlayerMgr_c::GetInstance()->fn_8035E220() && !FileManager::GetInstance()->getField_0xA84D();
        if (b) {
            ExecProcPre();
            fn_80068070();
        }
    }
    if (SaveMgr::GetInstance()->getField_0x83D() || SaveMgr::GetInstance()->getField_0x83F() ||
        FileManager::GetInstance()->getField_0xA84D() == 1) {
        if (FileManager::GetInstance()->getField_0xA84D() != 1) {
            mpFader->calc();
        }
    }
}

void Manage_c::SetResetCallback() {
    OSSetResetCallback(ResetCallback);
}

void Manage_c::SetPowerCallback() {
    OSSetPowerCallback(PowerCallback);
}

void Manage_c::SetDiskCheckCallback() {
    field_0x1C = 0;
    DVDCheckDiskAsync(&mDvdCmd, DiskCheckCallback);
}

void Manage_c::DiskCheckCallback(s32 result, struct DVDFileInfo *info) {
    if (result == 0) {
        GetInstance()->field_0x1C = 2;
    } else {
        GetInstance()->field_0x1C = 1;
    }
};

void Manage_c::ResetCallback() {
    GetInstance()->mRestart = true;
};

void Manage_c::PowerCallback() {
    GetInstance()->mShutdown = true;
};

void Manage_c::CleanUpGX() {
    VISetBlack(TRUE);
    VIFlush();
    VIWaitForRetrace();
    VIWaitForRetrace();
    OSThread *thread = GXGetCurrentGXThread();
    BOOL status = OSDisableInterrupts();

    if (thread != OSGetCurrentThread()) {
        OSCancelThread(thread);
        GXSetCurrentGXThread();
    }
    GXFlush();
    GXAbortFrame();
    GXDrawDone();
    OSRestoreInterrupts(status);

    GXSetDrawDoneCallback(0);
    VIWaitForRetrace();
}

void Manage_c::CleanUpLC() {
    LCDisable();
}

void Manage_c::MotorCancelOn() {
    for (int i = 0; i < 4; i++) {
        // TODO: Fakematch
        mPad::getCore(--++i)->stopRumbleMgr();
    }
    dRumble_c::stop(-1);
}

void Manage_c::MotorCancelOff() {}

bool Manage_c::IsFatalError() {
    return false;
}

bool Manage_c::IsDiskError() {
    return dDvdUnk::FontUnk::GetInstance()->isDiskError();
}

bool Manage_c::FadeOutCalc() {
    if (mpFader->getStatus() == EGG::Fader::STATUS_PREPARE_IN ||
        mpFader->getStatus() == EGG::Fader::STATUS_PREPARE_OUT) {
        return false;
    } else {
        return true;
    }
}

void Manage_c::FadeOutRequest(bool fade) {
    if (!fade) {
        dDvdUnk::FontUnk::GetInstance()->fn_800529B0();
        dDvdUnk::FontUnk::GetInstance()->fn_80052A20();

        dCsBase_c::GetInstance()->setVisible(false);
        dCsBase_c::GetInstance()->setDrawDirectly(false);
        dCsBase_c::GetInstance()->setCalibrationPointCenterEnabled(false);

        field_0x66 = 0;

        if (SaveMgr::GetInstance()->isInState()) {
            SaveMgr::GetInstance()->init();
        }

        if (dLytSystemWindow_c::GetInstance()->fn_80152F80()) {
            dLytSystemWindow_c::GetInstance()->fn_80152F10();
        }
    } else {
        mpFader->setStatus(EGG::Fader::STATUS_PREPARE_OUT);
        VISetBlack(FALSE);
        MotorCancelOff();
        Reset();
    }
}

} // namespace dReset
