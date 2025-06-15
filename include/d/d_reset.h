#ifndef RELOAD_COLOR_FADER_H
#define RELOAD_COLOR_FADER_H

#include "common.h"
#include "egg/core/eggFader.h"
#include "m/m_color.h"
namespace dReset {

class Manage_c {
public:
    enum Mode_c {
        // TODO
    };
    enum Exec_c {
        // TODO
    };

    static Manage_c *GetInstance();

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
    // fn_80067FF0
    void ExecProcPre();
    // fn_80068070
    void ExecProcMain();
    void DiskCheckModeRequest();
    void DebugPrintMode(const char *);
    void ModeProc();
    void ModeRequest(Mode_c);
    void ModeRequestSafetyWait(Exec_c);
    void ModeRequestSoftReset();

    void ModeInit_Normal();
    void ModeProc_Normal();

    void ModtInit_SoftReset();
    void ModtProc_SoftReset();

    void ModtInit_HbmWait();
    void ModtProc_HbmWait();

    void ModtInit_DiscWait();
    void ModtProc_DiscWait();

    void ModeInit_FatalError();
    void ModeProc_FatalError();

    void ModeInit_SafetyWait();
    void ModeProc_SafetyWait();

    void SetResetCallback();
    void SetPowerCallback();
    // fn_80068C70
    // fn_80068C90
    static void ResetCallback();
    static void PowerCallback();
    void CleanUpGX();
    void CleanUpLC();
    void MotorCancelOn();
    void MotorCancelOff();
    bool IsFatalError();
    bool IsDiskError();
    EGG::Fader::EStatus FadeOutCalc();
    void FadeOutRequest(bool fadeout);

    bool is1Or5() const {
        return field_0x0C == 1 || field_0x0C == 5;
    }

    s32 get0x0C() const {
        return field_0x0C;
    }

private:
    static Manage_c *s_pInstance;
    static mColor c_faderColor;

    u8 field_0x00[0x0C - 0x00];
    s32 field_0x0C;
};

} // namespace dReset

#endif
