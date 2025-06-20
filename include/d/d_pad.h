#ifndef D_PAD_H
#define D_PAD_H

#include "common.h"
#include "egg/core/eggController.h"
#include "m/m_mtx.h"
#include "m/m_vec.h"
#include "rvl/KPAD/KPAD.h"
#include "rvl/WPAD/WPAD.h"

namespace dPad {
class ex_c {
    // Complete Made up name
    struct motion_c {
        void init();

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

public:
    void fn_80056790(s32 chan);
    void fn_80056AF0(s32 chan);
    void fn_80056B90(s32 chan);
    void fn_80056CE0(s32 chan);
    void fn_80056DA0(s32 chan); // Deals with Mpls Calibration Start
    void fn_80056DF0(s32 chan); // Deals with Mpls Calibration Work
    f32 fn_80056E50();
    void fn_80056E60(s32 chan); // Deals with Mpls Calibration Stop
    void resetState(s32 chan);

    void fn_800572A0(s32 chan);

    static bool isLowBattery();
    static bool isOutOfBattery();

    static void resetInfo();
    static void clearInfo(s32 chan);
    static s32 getInfo(s32 chan);
    static s32 getBatteryLevel();
    static s32 getBatteryLevel(s32 chan);
    static void setInfo(s32 chan, const WPADInfo *pInfo);

    static void on_0x54(s32 chan);
    static void on_0x55(s32 chan);
    static void on_0x56(s32 chan);
    static void on_0x57(s32 chan);

    static void fn_800592D0(s32 chan);

    void fn_80059300(s32 chan);
    void getUnifiedWpadStatus(s32 chan);
    void fn_800593D0();

    void updateStatus(s32 chan);

    static ex_c *getInstance() {
        return m_current_ex;
    }

public:
    /* 0x0000 */ mVec2_c mDpdPos;
    /* 0x0008 */ mVec2_c field_0x8;
    /* 0x0010 */ mVec2_c mFSStick;
    /* 0x0018 */ f32 mFSStickDistance;
    /* 0x001C */ mAng mFSStickAngle;
    /* 0x0020 */ WPADResult mWPADProbeResult;
    /* 0x0024 */ WPADResult mWPADProbeResultStable;
    /* 0x0028 */ s32 mWPADProbeStableTimer;
    /* 0x002C */ WPADDeviceType mWPADDeviceType;
    /* 0x0030 */ u32 mWPADDeviceTypeStable;
    /* 0x0034 */ s32 mWPADDeviceTypeStableTimer;
    /* 0x0038 */ s32 field_0x38;
    /* 0x003C */ s32 field_0x3C;
    /* 0x0040 */ f32 field_0x40;
    /* 0x0044 */ bool field_0x44;
    /* 0x0045 */ bool field_0x45;
    /* 0x0046 */ bool field_0x46;
    /* 0x0047 */ bool field_0x47;
    /* 0x0048 */ s32 field_0x48;
    /* 0x004C */ bool mSpeakerSetup;
    /* 0x004D */ bool mSpeakerShutdown;
    /* 0x004E */ bool mIncorrectDeviceType;
    /* 0x004F */ bool field_0x4F;
    /* 0x0050 */ bool field_0x50;
    /* 0x0051 */ bool field_0x51;
    /* 0x0052 */ bool mMplsEnabled;
    /* 0x0053 */ bool field_0x53;
    /* 0x0054 */ bool field_0x54[4];
    /* 0x0058 */ bool field_0x58[4];
    /* 0x005C */ mVec2_c field_0x5C;
    /* 0x0064 */ mVec2_c field_0x64;
    /* 0x006C */ s32 field_0x6C;
    /* 0x0070 */ mAng field_0x70;
    /* 0x0074 */ mVec3_c field_0x74;
    /* 0x0080 */ mVec3_c field_0x80;
    /* 0x008C */ mVec3_c field_0x8C;
    /* 0x0098 */ motion_c mMotion;
    /* 0x1174 */ motion_c mFSMotion;
    /* 0x2250 */ mVec3_c mMPLSSpeed;
    /* 0x225C */ mVec3_c mMPLSBasisX;
    /* 0x2268 */ mVec3_c mMPLSBasisY;
    /* 0x2274 */ mVec3_c mMPLSBasisZ;
    /* 0x2280 */ s32 mState;
    /* 0x2284 */ s32 field_0x2284;
    /* 0x2288 */ s32 field_0x2288;
    /* 0x228C */ KPADUnifiedWpadStatus mStatus;
    /* 0x22C4 */ u8 _0x22C4[0x22CE - 0x22C4];
    /* 0x22CE */ bool field_0x22CE;
    /* 0x22CF */ bool field_0x22CF;
    /* 0x22D0 */ u8 field_0x22D0;
    /* 0x22D1 */ u8 field_0x22D1;
    /* 0x22D4 */ s32 field_0x22D4;
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

void setNoSleep();
void setAutoSleepTime();

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
mVec2_c &getDpdStableMaybe();
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
