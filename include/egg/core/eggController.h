#ifndef EGG_CONTROLLER_H
#define EGG_CONTROLLER_H

#include "common.h"
#include "egg/core/eggSingleton.h"
#include "egg/math/eggMatrix.h"
#include "egg/math/eggVector.h"
#include "egg/prim/eggBitFlag.h"
#include "egg/prim/eggBuffer.h"
#include "rvl/KPAD/KPAD.h"

#include "rvl/KPAD.h" // IWYU pragma: exports
#include "rvl/PAD.h"  // IWYU pragma: exports
#include "rvl/WPAD.h" // IWYU pragma: exports

namespace EGG {

class ControllerRumbleMgr;
class CoreController;

enum eCoreDevType {
    cDEV_CORE = WPAD_DEV_CORE,
    cDEV_FREESTYLE = WPAD_DEV_FREESTYLE,
    cDEV_CLASSIC = WPAD_DEV_CLASSIC,

    cDEV_MPLS = WPAD_DEV_MOTION_PLUS,
    cDEV_MPLS_PT_FS = WPAD_DEV_MPLS_PT_FS,

    cDEV_FUTURE = WPAD_DEV_FUTURE,
    cDEV_NOT_SUPPORTED = WPAD_DEV_NOT_SUPPORTED,
    cDEV_NOT_FOUND = WPAD_DEV_NOT_FOUND,
    cDEV_NULL = WPAD_DEV_NULL,
    cDEV_UNKNOWN = WPAD_DEV_UNKNOWN,
};

enum eCoreButton {
    cCORE_BUTTON_UP = 1 << 3,
    cCORE_BUTTON_DOWN = 1 << 2,
    cCORE_BUTTON_LEFT = 1 << 0,
    cCORE_BUTTON_RIGHT = 1 << 1,

    cCORE_BUTTON_PLUS = 1 << 4,
    cCORE_BUTTON_MINUS = 1 << 12,

    cCORE_BUTTON_1 = 1 << 9,
    cCORE_BUTTON_2 = 1 << 8,

    cCORE_BUTTON_A = 1 << 11,
    cCORE_BUTTON_B = 1 << 10,

    cCORE_BUTTON_HOME = 1 << 15,

    cCORE_FSSTICK_UP = 1 << 16,
    cCORE_FSSTICK_DOWN = 1 << 17,
    cCORE_FSSTICK_LEFT = 1 << 18,
    cCORE_FSSTICK_RIGHT = 1 << 19,

    cCORE_FSSTICK_BUTTONS = cCORE_FSSTICK_UP | cCORE_FSSTICK_DOWN | cCORE_FSSTICK_LEFT | cCORE_FSSTICK_RIGHT
};

typedef CoreController *(*CoreControllerFactory)();

struct CoreControllerConnectArg {
    s32 chan;   // at 0x0
    s32 result; // at 0x4
};

typedef void (*CoreControllerConnectCallback)(const CoreControllerConnectArg &rArg);

struct CoreControllerExtensionArg {
    eCoreDevType newDevType; // at 0x0
    eCoreDevType oldDevType; // at 0x4
    s32 chan;                // at 0x8
};

class CoreControllerExtensionCallback {
public:
    virtual void onConnect(const CoreControllerExtensionArg &rArg); // at 0x8
};

/******************************************************************************
 *
 * CoreStatus
 *
 ******************************************************************************/
class CoreStatus : public KPADStatus {
    friend class CoreController;

public:
    CoreStatus() {}

public:
    void init();

    u32 getHold() const {
        return hold;
    }
    u32 getRelease() const {
        return release;
    }
    u32 getTrigger() const {
        return trig;
    }

    bool down(u32 buttons) const {
        return buttons & hold;
    }
    bool up(u32 buttons) const {
        return (buttons & hold) != buttons;
    }

    bool downAll(u32 buttons) const {
        return (buttons & hold) == buttons;
    }
    bool upAll(u32 buttons) const {
        return (buttons & hold) == 0;
    }

    bool downTrigger(u32 buttons) const {
        return buttons & trig;
    }
    bool upTrigger(u32 buttons) const {
        return buttons & release;
    }

    const Vector3f &getAccel() const {
        return *reinterpret_cast<const Vector3f *>(&acc);
    }

    Vector3f getAccelBad() const {
        return Vector3f(acc.x, acc.y, acc.z);
    }
    Vector2f getAccelVertical() {
        return Vector2f(acc_vertical.x, acc_vertical.y);
    }

    eCoreDevType getDevType() const {
        return static_cast<eCoreDevType>(dev_type);
    }

    bool isCore() const {
        return getDevType() == cDEV_CORE || isFreestyle();
    }
    bool isFreestyle() const {
        return getDevType() == cDEV_FREESTYLE;
    }

    s8 getDPDValidFlag() const {
        return dpd_valid_fg;
    }

    f32 getFSStickX() const {
        return ex_status.fs.stick.x;
    }
    f32 getFSStickY() const {
        return ex_status.fs.stick.y;
    }

    u32 getFSStickButton();
};
/******************************************************************************
 *
 * CoreController
 *
 ******************************************************************************/
class CoreController {
    friend class CoreControllerMgr;

public:
    CoreController();
    // vtable 0x0000
    /* vt 0x08 */ virtual void setPosParam(f32 f1, f32 f2) {
        KPADSetPosParam(mChannelID, f1, f2);
    }
    /* vt 0x0C */ virtual void setHoriParam(f32 f1, f32 f2) {
        KPADSetHoriParam(mChannelID, f1, f2);
    }
    /* vt 0x10 */ virtual void setDistParam(f32 f1, f32 f2) {
        KPADSetDistParam(mChannelID, f1, f2);
    }
    /* vt 0x14 */ virtual void setAccParam(f32 f1, f32 f2) {
        KPADSetAccParam(mChannelID, f1, f2);
    }
    /* vt 0x18 */ virtual bool down(u32 buttons) const {
        return mCoreStatus[0].down(buttons);
    }
    /* vt 0x1C */ virtual bool up(u32 buttons) const {
        return mCoreStatus[0].up(buttons);
    }
    /* vt 0x20 */ virtual bool downTrigger(u32 buttons) const {
        return mCoreStatus[0].downTrigger(buttons);
    }
    /* vt 0x24 */ virtual bool upTrigger(u32 buttons) const {
        return mCoreStatus[0].upTrigger(buttons);
    }
    /* vt 0x28 */ virtual bool downAll(u32 buttons) const {
        return mCoreStatus[0].downAll(buttons);
    }
    /* vt 0x2C */ virtual bool upAll(u32 buttons) const {
        return mCoreStatus[0].upAll(buttons);
    }
    /* vt 0x30 */ virtual void beginFrame(PADStatus *padStatus);
    /* vt 0x34 */ virtual void endFrame();

    Vector2f getDpdRawPos();
    f32 getDpdDistance();
    s32 getDpdValidFlag();
    f32 getFreeStickX() const;
    f32 getFreeStickY() const;

    s32 getChannelID() const {
        return mChannelID;
    }

    const Vector3f &getAccel() const {
        return mCoreStatus[0].getAccel();
    }
    Vector3f getAccelBad() const {
        return mCoreStatus[0].getAccelBad();
    }

    eCoreDevType getDevType() const {
        return mCoreStatus[0].getDevType();
    }

    bool isCore() const {
        return mCoreStatus[0].isCore();
    }
    bool isFreestyle() const {
        return mCoreStatus[0].isFreestyle();
    }
    u32 getHold() const {
        return mCoreStatus[0].hold;
    }

    s32 getReadLength() const {
        return mKPADReadLength;
    }

    bool isStable(u8 bits) const {
        return (mStableFlags & bits) == bits;
    }

    void setStableFrame(int frame) {
        mStableFrame = frame;
    }
    void setStableMag(f32 mag) {
        mStableMag = mag;
    }
    void sceneReset();

    void startMotor();
    void stopMotor();

    void createRumbleMgr(u8 overlap_num);
    void startPatternRumble(const char *pPattern, int frame, bool force);
    void startPowerFrameRumble(f32 power, int frame, bool force);
    void stopRumbleMgr();

    bool isConnected() const {
        return mFlag.onBit(0);
    }

    CoreStatus *getCoreStatus(int idx);
    CoreStatus *getCoreStatus() {
        return mCoreStatus;
    }

private:
    enum StableAxis {
        STABLE_AXIS_X,
        STABLE_AXIS_Y,
        STABLE_AXIS_Z,

        STABLE_AXIS_MAX
    };

    enum StableFlag {
        STABLE_FLAG_X = 1 << STABLE_AXIS_X,
        STABLE_FLAG_Y = 1 << STABLE_AXIS_Y,
        STABLE_FLAG_Z = 1 << STABLE_AXIS_Z,

        STABLE_FLAG_XYZ = STABLE_FLAG_X | STABLE_FLAG_Y | STABLE_FLAG_Z
    };

private:
    void calc_posture_matrix(Matrix34f &rPostureMtx, bool checkStable);

private:
    /* 0x004 */ s32 mChannelID;

    /* 0x008 */ u32 mFSStickHold;
    /* 0x00C */ u32 mFSStickTrig;
    /* 0x010 */ u32 mFSStickRelease;
    /* 0x014 */ u32 field_0x14;

    /* 0x018 */ CoreStatus mCoreStatus[16];
    /* 0xf18 */ int mKPADReadLength;

    /* 0xf1c */ TBitFlag<u8> mFlag;
    /* 0xf20 */ Vector3f mAccelPrev;
    /* 0xf2c */ Vector2f mDpdPosPrev;
    /* 0xf34 */ u32 mIdleTime;

    /* 0xf38 */ u32 mSimpleRumblePattern;
    /* 0xf3c */ bool mEnableSimpleRumble;
    /* 0xf40 */ s32 mSimpleRumbleFrame;
    /* 0xf44 */ u8 mSimpleRumbleSize;
    /* 0xf45 */ u8 mSimpleRumbleIndex;
    /* 0xf48 */ ControllerRumbleMgr *mRumbleMgr;

    /* 0xf4c */ Matrix34f mPostureMatrix;
    /* 0xf7c */ Matrix34f mPostureMatrixPrev;

    /* 0xfac */ u8 mStableFlags; // May not be TBitFlag?
    /* 0xfb0 */ s32 mStableFrame;
    /* 0xfb4 */ s32 mStableTimer[3];
    /* 0xfc0 */ f32 mStableMag;
    /* 0xfc4 */ Vector3f mStableAccel;
};

/******************************************************************************
 *
 * CoreControllerMgr
 *
 ******************************************************************************/
class CoreControllerMgr {
    EGG_SINGLETON_DECL(CoreControllerMgr);

    struct UnkField0x10A0 {
        virtual void v_08(UNKTYPE *);
    };

public:
    // 0x0010 vtable
    /* vt 0x08 */ virtual void beginFrame();
    /* vt 0x0C */ virtual void endFrame();

    CoreController *getNthController(int index);

    eCoreDevType getDevType(const int i) const {
        return mDevTypes(i);
    }

    static void setConnectCallback(CoreControllerConnectCallback cb) {
        sConnectCallback = cb;
    }

private:
    CoreControllerMgr();

    static void *allocThunk(size_t size);
    static int deleteThunk(void *ptr);
    static void connectCallback(s32, s32);

private:
    /* 0x0014 */ TBuffer<CoreController *> mControllers;
    /* 0x0020 */ u8 field_0x20[0x10a0 - 0x0020];
    /* 0x10A0 */ UnkField0x10A0 *field_0x10A0;
    /* 0x10A4 */ TBuffer<eCoreDevType> mDevTypes;
    /* 0x10b0 */ u8 field_0x10B0[0x10e0 - 0x10b0];

    static CoreControllerFactory sCoreControllerFactory;
    static CoreControllerConnectCallback sConnectCallback;
    static bool sWPADRegisterAllocatorOFF;
    static s32 sWPADWorkSize;
};

/******************************************************************************
 *
 * NullController
 *
 ******************************************************************************/
class NullController : public CoreController {
public:
    NullController() {
        unk[92] = 0xfe;
    }

private:
    // idk this has NO effect on anything
    u8 unk[0x80674c00 - 0x80674b10];
};

/******************************************************************************
 *
 * ControllerRumbleUnit
 *
 ******************************************************************************/
class ControllerRumbleUnit {
    friend class ControllerRumbleMgr;

public:
    // 0x00 vtable
    ControllerRumbleUnit() : mFlags(0) {
        init();
    }
    /* vt 0x08 */ virtual ~ControllerRumbleUnit() {}

    void startPattern(const char *pPattern, int frame);

    f32 calc();

private:
    enum {
        BIT_USE_PATTERN,
        BIT_USE_POWER,

        BIT_POWER_LOOP,
        BIT_ENABLED,
        BIT_PATTERN_LOOP,
        BIT_PATTERN_TIMER,
    };

private:
    void init();

    void setFlags(u8 bit) {
        mFlags.setBit(bit);
    }
    void clrFlags(u8 bit) {
        mFlags.resetBit(bit);
    }
    bool tstFlags(u8 bit) {
        return mFlags.onBit(bit);
    }

private:
    /* 0x04 */ const char *mRumblePattern;
    /* 0x08 */ const char *mRumblePatternPtr;
    /* 0x0C */ s32 mRumbleFrame;
    /* 0x10 */ f32 mRumblePower;
    /* 0x14 */ f32 mRumbleValue;
    /* 0x18 */ TBitFlag<u8> mFlags;
    /* 0x1C */ nw4r::ut::Node mNode;
    /* 0x24 */ u32 field_0x24;
};

/******************************************************************************
 *
 * ControllerRumbleMgr
 *
 ******************************************************************************/
class ControllerRumbleMgr {
public:
    ControllerRumbleMgr();
    virtual ~ControllerRumbleMgr() {} // at 0x8

    void createUnit(u8 overlap_num, CoreController *pController);

    void stop();
    void calc();

    void startPattern(const char *pattern, int, bool);

private:
    ControllerRumbleUnit *getUnitFromList(bool bGrabActive);

private:
    /* 0x04 */ nw4r::ut::List mActiveUnitList;
    /* 0x10 */ nw4r::ut::List mStoppedUnitList;
    /* 0x1C */ CoreController *mController;
};

} // namespace EGG

#endif
