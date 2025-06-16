#ifndef RELOAD_COLOR_FADER_H
#define RELOAD_COLOR_FADER_H

#include "common.h"
#include "egg/core/eggColorFader.h"
#include "egg/core/eggFader.h"
#include "egg/core/eggHeap.h"
#include "m/m_color.h"
namespace dReset {

class Manage_c {
public:
    enum Mode_e {
        Normal = 0,
        SoftReset = 1,
        HbmWait = 2,
        DiskWait = 3,
        FatalError = 4,
        SafetyWait = 5,
    };
    enum Exec_e {
        Restart = 0,
        Reboot = 1,
        ReturnToMenu = 2,
        ReturnToDataManager = 3,
        Shutdown = 4,
        ExecNone = 5,
    };

    static void CreateInstance(EGG::Heap *);
    static Manage_c *GetInstance();
    Manage_c(EGG::Heap *);

    void Configure();
    void Reset();
    void Calculate();
    void Draw();
    void BootComplete(bool complete);
    bool CanExecSoftReset() const;
    void SetSoftResetFinish();
    void SetSoftResetScene();
    void PostDeleteScene();
    bool IsFaderBlank() const;
    void SetHbmReturnMenu();
    void SetHbmReset();
    void SetDelayReset();
    void SetDelayPowerOff();
    void SetInteriorReturnDataManager();
    void fn_80067FF0();
    void ExecProcPre();
    void fn_80068070();
    void ExecProcMain();
    void DiskCheckModeRequest(bool);
    void DebugPrintMode(const char *);
    void ModeProc();
    void ModeRequest(Mode_e);
    void ModeRequestSafetyWait(Exec_e);
    void ModeRequestSoftReset();

    void ModeInit_Normal();
    void ModeProc_Normal();

    void ModeInit_SoftReset();
    void ModeProc_SoftReset();

    void ModeInit_HbmWait();
    void ModeProc_HbmWait();

    void ModeInit_DiskWait();
    void ModeProc_DiskWait();

    void ModeInit_FatalError();
    void ModeProc_FatalError();

    void ModeInit_SafetyWait();
    void ModeProc_SafetyWait();

    void SetResetCallback();
    void SetPowerCallback();
    void SetDiskCheckCallback();
    static void DiskCheckCallback(s32 b, struct DVDFileInfo *info);
    static void ResetCallback();
    static void PowerCallback();
    void CleanUpGX();
    void CleanUpLC();
    void MotorCancelOn();
    void MotorCancelOff();
    static bool IsFatalError();
    static bool IsDiskError();
    bool FadeOutCalc();
    void FadeOutRequest(bool fadeout);

    bool isSoftResetOrSafetyWait() const {
        return mMode == SoftReset || mMode == SafetyWait;
    }

    bool isNormal() const {
        return mMode == Normal;
    }
    bool isSoftReset() const {
        return mMode == SoftReset;
    }
    bool isHbmWait() const {
        return mMode == HbmWait;
    }
    bool isDiskWait() const {
        return mMode == DiskWait;
    }
    bool isFatalError() const {
        return mMode == FatalError;
    }
    bool isSafetyWait() const {
        return mMode == SafetyWait;
    }

private:
    /* 0x00 */ EGG::Heap *mpHeap;
    /* 0x04 */ EGG::ColorFader *mpFader;
    /* 0x08 */ EGG::ColorFader::EStatus mFaderStatusPrev;
    /* 0x0C */ Mode_e mMode;
    /* 0x10 */ UNKWORD field_0x10;
    /* 0x14 */ Mode_e mModePrev;
    /* 0x18 */ Exec_e mExec;
    /* 0x1C */ UNKWORD field_0x1C;
    /* 0x20 */ DVDCommandBlock mDvdCmd;
    /* 0x50 */ bool mHbmReset;
    /* 0x51 */ bool mHbmReturnMenu;
    /* 0x52 */ bool mRestart;
    /* 0x53 */ bool mShutdown;
    /* 0x54 */ bool mInteriorReturnDataManager;
    /* 0x58 */ UNKWORD field_0x58;
    /* 0x5C */ UNKWORD field_0x5C;
    /* 0x60 */ UNKWORD field_0x60;
    /* 0x64 */ u8 mBootComplete;
    /* 0x65 */ u8 mSoftReset;
    /* 0x66 */ u8 field_0x66;
    /* 0x67 */ u8 field_0x67;

    static Manage_c *s_pInstance;
    static mColor c_faderColor;
};

} // namespace dReset

#endif
