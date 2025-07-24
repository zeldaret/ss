#ifndef D_PAD_H
#define D_PAD_H

#include "common.h"
#include "m/m_angle.h"
#include "m/m_mtx.h"
#include "m/m_vec.h"
#include "rvl/KPAD/KPAD.h"
#include "rvl/WPAD/WPAD.h"

namespace dPad {
class ex_c {
    // Complete Made up name
    struct acc_c {
        void init();
        f32 getEntryField_0xB40(s32 chan);
        void fn_800576D0(s32 chan);
        void fn_800578E0(s32 chan);
        void fn_80057AC0(s32 chan, bool);

        f32 fn_80057F00(s32 chan);
        f32 fn_80057F30(s32 chan);
        f32 fn_80057F60(s32 chan);

        bool fn_80057F90(s32 chan, bool);
        bool fn_800580C0(s32 chan, bool);

        bool fn_800581F0(s32 chan, bool);
        bool fn_80058320(s32 chan, bool);

        bool fn_80058450(s32 chan, bool);

        void fn_80058540(s32 chan, bool);

        void fn_80058990(u32 mask, bool);

        bool fn_800589F0();
        f32 fn_80058A00();
        void fn_80058AE0(s32 chan, bool);

        /* 0x0000 */ mVec3_c field_0x000[120];
        /* 0x05A0 */ mVec3_c field_0x5A0[120];
        /* 0x0B40 */ f32 field_0xB40[120];
        /* 0x0D20 */ mVec3_c field_0xD20[39];
        /* 0x0EF4 */ mVec3_c field_0xEF4[33];
        /* 0x1080 */ mVec3_c field_0x1080;
        /* 0x108C */ mVec3_c field_0x108C;
        /* 0x1098 */ mMtx_c field_0x1098;
        /* 0x10C8 */ u32 field_0x10C8;
        /* 0x10CC */ u32 field_0x10CC;
        /* 0x10D0 */ u32 field_0x10D0;
        /* 0x10D4 */ f32 field_0x10D4;
        /* 0x10D8 */ u32 field_0x10D8;
    };

    struct mpls_c {
        mpls_c() : mX(mVec3_c::Ex), mY(mVec3_c::Ey), mZ(mVec3_c::Ez) {}
        mMtx_c getMtx() const;
        /* 0x00 */ mVec3_c mX;
        /* 0x0C */ mVec3_c mY;
        /* 0x18 */ mVec3_c mZ;
    };

public:
    ex_c();
    void fn_80055EF0(s32 chan);
    void fn_800562B0(s32 chan, mVec3_c &mpls);
    void fn_80056330(s32 chan);
    void fn_80056580(s32 chan, const mVec2_c &);
    void centerCursor(s32 chan, bool);
    void fn_80056790(s32 chan);
    void setField_0x70(mAng ang);
    void setField_0x70();
    void fn_80056AF0(s32 chan);

    bool checkWPADProbeStable();
    void fn_80056B90(s32 chan);
    void fn_80056CE0(s32 chan);

    void startMplsCalibration(s32 chan);
    void workMplsCalibration(s32 chan);
    f32 getCalibrationWork();
    void stopMplsCalibration(s32 chan);

    void centerCursor(s32 chan);

    void gotoStateWaitForConnect(s32 chan);
    void executeStateWaitForConnect(s32 chan);
    void gotoStatePostConnect(s32 chan);
    void executeStatePostConnect(s32 chan);
    void gotoStateEnableMpls(s32 chan); // (EnableMPLS)
    void executeStateEnableMpls(s32 chan);
    void gotoStateDisconnectWpad(s32 chan); // (Disconnect)
    void executeStateDisconnectWpad(s32 chan);
    void gotoStateMplsConfigError(s32 chan);
    void executeStateMplsConfigError(s32 chan);
    void gotoStateMplsConfigured(s32 chan);
    void executeStateMplsConfigured(s32 chan);
    void fn_800572A0(s32 chan); // State Handling

    /**
     * Is the Wiimote missing MotionPlus? This means the Wiimote neither has
     * a builtin Mpls nor a passthrough Mpls attached.
     */
    static bool isMissingMpls();
    /**
     * Is the Wiimote known to not have an attached Nunchuk?
     */
    static bool isMissingNunchuk();
    static void fn_80058C90(s32 chan);

    static bool isLowBattery();
    static bool isOutOfBattery();

    static void resetInfo();
    static void clearInfo(s32 chan);
    static s32 getInfo(s32 chan);
    static s32 getBatteryLevel();
    static s32 getBatteryLevel(s32 chan);
    static void setInfo(s32 chan, const WPADInfo *pInfo);

    static f32 fn_80058F50();
    static bool fn_80058F60();
    static f32 getCurrentCalibrationWork();
    static void fn_80058FF0();
    static void fn_80059000();
    static bool needMplsCalibration();
    static void setCalibrateMpls();
    static bool fn_800590B0();
    static bool fn_800590E0();
    static void fn_800590F0();
    static bool fn_80059100();
    static bool fn_80059110(s32 chan);
    static void fn_80059210();
    static void fn_80059220();
    static void fn_80059230();
    static void fn_80059240();

    static void on_0x54(s32 chan);
    static void on_0x55(s32 chan);
    static void on_0x56(s32 chan);
    static void on_0x57(s32 chan);

    static void fn_800592D0(s32 chan);

    void fn_80059300(s32 chan);
    static bool fn_80059330(s32 chan);
    static bool fn_80059350(s32 chan);
    static bool fn_80059370(s32 chan);
    static bool fn_80059390(s32 chan);
    void getUnifiedWpadStatus(s32 chan);
    void calcFSStickDirMask();

    static void setNoSleep();
    static void setAutoSleepTime();

    void updateStatus(s32 chan);

    static ex_c *getInstance() {
        return m_current_ex;
    }

    enum ExState_e {
        EX_STATE_WAITING_FOR_CONNECT = 0,
        EX_STATE_POST_CONNECT = 1,
        EX_STATE_ENABLE_MPLS = 2,
        EX_STATE_DISCONECT_WPAD = 3,
        EX_STATE_MPLS_CONFIG_ERROR = 4,
        EX_STATE_MPLS_CONFIGURED = 5,
    };

public:
    /* 0x0000 */ mVec2_c mDpdPos;
    /* 0x0008 */ mVec2_c mDpdPosScreen;
    /* 0x0010 */ mVec2_c mFSStick;
    /* 0x0018 */ f32 mFSStickDistance;
    /* 0x001C */ mAng mFSStickAngle;
    /* 0x0020 */ WPADResult mWPADProbeResult;
    /* 0x0024 */ WPADResult mWPADProbeResultStable;
    /* 0x0028 */ s32 mWPADProbeStableTimer;
    /* 0x002C */ WPADDeviceType mWPADDeviceType;
    /* 0x0030 */ u32 mWPADDeviceTypeStable;
    /* 0x0034 */ s32 mWPADDeviceTypeStableTimer;
    /* 0x0038 */ s32 mConnectedStableTimer;
    /* 0x003C */ s32 field_0x3C;
    /* 0x0040 */ f32 mCalibrationWork;
    /* 0x0044 */ bool field_0x44;
    /* 0x0045 */ bool field_0x45;
    /* 0x0046 */ bool field_0x46;
    /* 0x0047 */ bool mIsCalibrating;
    /* 0x0048 */ s32 field_0x48;
    /* 0x004C */ bool mDidConnect;
    /* 0x004D */ bool mDidDisconnect;
    /* 0x004E */ bool mNeedMplsCalibration;
    /* 0x004F */ bool field_0x4F;
    /* 0x0050 */ bool field_0x50;
    /* 0x0051 */ u8 field_0x51;
    /* 0x0052 */ bool mMplsEnabled;
    /* 0x0053 */ bool field_0x53;
    /* 0x0054 */ bool field_0x54;
    /* 0x0055 */ bool field_0x55;
    /* 0x0056 */ bool field_0x56;
    /* 0x0057 */ bool field_0x57;
    /* 0x0058 */ bool field_0x58;
    /* 0x0059 */ bool field_0x59;
    /* 0x005A */ bool field_0x5A;
    /* 0x005B */ bool field_0x5B;
    /* 0x005C */ mVec2_c field_0x5C;
    /* 0x0064 */ mVec2_c field_0x64;
    /* 0x006C */ s32 mIsWPADDeviceTypeMplsStableTimer;
    /* 0x0070 */ mAng field_0x70;
    /* 0x0074 */ mVec3_c field_0x74;
    /* 0x0080 */ mVec3_c field_0x80;
    /* 0x008C */ mVec3_c field_0x8C;
    /* 0x0098 */ acc_c mMotion;
    /* 0x1174 */ acc_c mFSMotion;
    /* 0x2250 */ mVec3_c mMPLSVelocity;
    /* 0x225C */ mpls_c mMPLS;
    /* 0x2280 */ s32 mState;
    /* 0x2284 */ s32 mOutOfHbmStableTimer;
    /* 0x2288 */ s32 field_0x2288;
    /* 0x228C */ KPADUnifiedWpadStatus mStatus;
    /* 0x22CE */ bool field_0x22CE;
    /* 0x22CF */ bool field_0x22CF;
    /* 0x22D0 */ u8 field_0x22D0;
    /* 0x22D1 */ bool mFSStickMaskChanged;
    /* 0x22D4 */ u32 mFSStickMask;
    /* 0x22D8 */ u8 field_0x22D8;

    static WPADInfo m_info[2][4];
    static bool m_connected[4];
    static ex_c m_ex[4];
    static ex_c *m_current_ex;
};

void initMpls(s32 chan);
void create();
void setMpls(bool enable, s32 chan);
void beginPad_BR();
void endPad_BR();

void setConnectCallback();
void enableMplsDirRevise(s32 chan);
void disableMplsDirRevise(s32 chan);

// the next 4 funcs do nothing
void fn_80059620();
void fn_80059630();
void fn_80059640();
void fn_80059650();

bool getDownTrig(u32 btns);
bool getDownTrigA();
bool getDownTrigB();
bool getDownTrigC();
bool getDownTrigZ();
bool getDownTrigUp();
bool getDownTrigDown();
bool getDownTrigLeft();
bool getDownTrigRight();
bool getDownTrig1();
bool getDownTrig2();
bool getDownTrigMinus();
bool getDownTrigPlus();

bool getDown(u32 btns);
bool getDownAll(u32 btns);
bool getDownA();
bool getDownB();
bool getDownC();
bool getDownZ();
bool getDownUp();
bool getDownDown();
bool getDownLeft();
bool getDownRight();
bool getDown1();
bool getDown2();
bool getDownMinus();
bool getDownPlus();

bool getUpTrig(u32 btns);
bool getUpTrigA();
bool getUpTrigB();
bool getUpTrigC();
bool getUpTrigZ();
bool getUpTrigUp();
bool getUpTrigDown();
bool getUpTrigLeft();
bool getUpTrigRight();
bool getUpTrig1();
bool getUpTrig2();
bool getUpTrigMinus();
bool getUpTrigPlus();

bool getUp(u32 btns);
bool getUpAll(u32 btns);
bool getUpA();
bool getUpB();
bool getUpC();
bool getUpZ();
bool getUpUp();
bool getUpDown();
bool getUpLeft();
bool getUpRight();
bool getUp1();
bool getUp2();
bool getUpMinus();
bool getUpPlus();

mVec2_c &getDpdPos();
mVec2_c &getDpdPosScreen();
mVec2_c &getFSStick();
f32 getFSStickX();
f32 getFSStickY();
f32 getFSStickDistance();
mAng getFSStickAngle();
mVec3_c getAcc();

/** returns m_current_ex->field_0x1160 >> 0 & 1 */
u32 fn_80059B750();

/** returns m_current_ex->field_0x1160 >> 1 & 1 */
u32 fn_80059B760();

/** returns m_current_ex->field_0x1160 >> 2 & 1 */
u32 fn_80059B70();

/** returns m_current_ex->field_0x1160 >> 3 & 1 */
u32 fn_80059B80();

/** returns m_current_ex->field_0x1160 >> 5 & 1 */
u32 fn_80059B90();

mVec3_c getFSAcc();

/** returns m_current_ex->field_0x223C >> 4 & 1 */
u32 fn_80059CC0();

/** returns m_current_ex->field_0x223C >> 5 & 1 */
u32 fn_80059CD0();

}; // namespace dPad

#endif
