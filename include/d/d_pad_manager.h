#ifndef D_PAD_MANAGER_H
#define D_PAD_MANAGER_H

#include "common.h"
#include "d/lyt/d_lyt_system_window.h"
#include "egg/core/eggHeap.h"

class dPadManager_c {
public:
    dPadManager_c();
    virtual ~dPadManager_c();

    static bool create(EGG::Heap *pHeap);
    void init();
    bool execute();
    bool draw();

    static void buildWindow();

    static dPadManager_c *GetInstance() {
        return sInstance;
    }
    bool getField_0x25() const {
        return field_0x25;
    }

    void requestMplsCalibration();

private:
    typedef void (dPadManager_c::*ModeFunc)();

    void executeProc();
    static s32 get30SecondsInTicks();
    static s32 get3Point3SecondsInTicks();
    /* static  */ s32 get1Point5SecondsInTicks();

    enum Mode_e {
        Normal = 0,
        ReconnectPad = 1,
        AttachMplsAndNunchuk = 2,
        AttachNunchuk = 3,
        Calibrating = 4,
        LowBattery = 5,
        PointCenter = 6,
        CalibrationFail = 7,
    };

    enum PadStatus_e {
        PAD_DISCONNECTED = 0,
        PAD_NO_MPLS = 1,
        PAD_NO_NUNCHUK = 2,
        PAD_NEED_CALIBRATION = 3,
        PAD_CONFIGURING_MPLS = 4,
        PAD_NEED_POINT_CENTER = 5,
        PAD_NORMAL = 6,
    };

    void ModeRequestNormal();
    void ModeProc_Normal();
    
    void ModeRequestReconnectPad();
    void ModeProc_ReconnectPad();
    
    void ModeRequestAttachMplsAndNunchuk();
    void ModeProc_AttachMplsAndNunchuk();
    
    void ModeRequestAttachNunchuk();
    void ModeProc_AttachNunchuk();
    
    void ModeRequestCalibrating();
    void ModeProc_Calibrating();
    
    void ModeRequestLowBattery();
    void ModeProc_LowBattery();
    
    void ModeRequestPointCenter();
    void ModeProc_PointCenter();

    void ModeRequestCalibrationFail();
    void ModeProc_CalibrationFail();

    void ModeRequest(Mode_e mode);
    void ModeRequestNext(PadStatus_e status);

    PadStatus_e getPadStatus();
    bool isOutOfBattery();

    /* 0x04 */ dLytSystemWindowMain_c *mpWindow;
    /* 0x08 */ PadStatus_e mPadStatus;
    /* 0x0C */ Mode_e mMode;
    /* 0x10 */ s32 mStep;
    /* 0x14 */ s32 mCalibrationTimer;
    /* 0x18 */ s32 mStepTimer;
    /* 0x1C */ bool field_0x1C;
    /* 0x1D */ bool field_0x1D;
    /* 0x1E */ bool field_0x1E;
    /* 0x1F */ bool field_0x1F;
    /* 0x20 */ bool field_0x20;
    /* 0x21 */ bool mCalibrationFinished;
    /* 0x22 */ bool field_0x22;
    /* 0x23 */ bool field_0x23;
    /* 0x24 */ bool field_0x24;
    /* 0x25 */ bool field_0x25;

    static dPadManager_c *sInstance;
};

#endif
