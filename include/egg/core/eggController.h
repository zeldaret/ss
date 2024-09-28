#ifndef EGG_CONTROLLER_H
#define EGG_CONTROLLER_H

#include "egg/math/eggMath.h"
#include "egg/math/eggMatrix.h"
#include "egg/math/eggVector.h"
#include "egg/prim/eggBitFlag.h"
#include "egg/prim/eggBuffer.h"
#include "rvl/KPAD.h"
#include "rvl/PAD.h"
#include "rvl/WPAD.h"

#include <common.h>

namespace EGG {

class ControllerRumbleMgr;
class CoreController;

// To Be Filled out
enum eCoreDevType {};

typedef void (*ConnectCallback)(int args[]);
typedef class CoreController *(*ControllerFactory)();

class CoreStatus {
public:
    /* 0x00 */ u32 mHold;
    /* 0x04 */ u32 mTrig;
    /* 0x08 */ u32 mRelease;
    /* 0x0C */ Vector3f mAccel;
    /* 0x18 */ u8 field_0x01[0x20 - 0x18];
    /* 0x20 */ f32 mDpdRawX;
    /* 0x24 */ f32 mDpdRawY;
    /* 0x28 */ u8 field_0x02[0x48 - 0x28];
    /* 0x48 */ f32 mDpdDistance;
    /* 0x4C */ f32 float_0x4C;
    /* 0x50 */ f32 float_0x50;
    /* 0x54 */ f32 float_0x54;
    /* 0x58 */ f32 float_0x58;
    /* 0x5C */ u8 mDevType;
    /* 0x5D */ u8 field_0x5D;
    /* 0x5E */ s8 mDpdValid;
    /* 0x5F */ s8 unknown;
    /* 0x60 */ f32 mFSStickX;
    /* 0x64 */ f32 mFSStickY;
    /* 0x68 */ u8 field_0x03[0x88];

public:
    void init();
    u32 getFSStickButton() const;
    f32 getFSStickX() const { return mFSStickX; }
    f32 getFSStickY() const { return mFSStickY; }
    bool down(u32 mask) const { return (mask & mHold); }
    bool up(u32 mask) const { return (mask & mHold) != mask; }
    bool downTrigger(u32 mask) const { return (mask & mTrig); }
    bool upTrigger(u32 mask) const { return (mask & mRelease); }
    bool downAll(u32 mask) const { return (mask & mHold) == mask; }
    bool upAll(u32 mask) const { return (mask & mHold) == 0; }

    s32 getDevType() const { return mDevType; }
    bool isCore() const { return getDevType() == WPAD_DEV_CORE; }
    bool isFreestyle() const { return getDevType() == WPAD_DEV_FS; }

    u32 getHold() const { return mHold; }

    s8 getDpdValidFlag() const { return mDpdValid; };

    Vector2f getUnk() { return Vector2f(float_0x54, float_0x58); }

    const Vector3f &getAccel() const { return mAccel; }
};

class CoreController {
public:
    // vtable 0x0000
    /* vt 0x08 */ virtual void setPosParam(f32 f1, f32 f2) { KPADSetPosParam(mChannelID, f1, f2); }
    /* vt 0x0C */ virtual void setHoriParam(f32 f1, f32 f2) { KPADSetHoriParam(mChannelID, f1, f2); }
    /* vt 0x10 */ virtual void setDistParam(f32 f1, f32 f2) { KPADSetDistParam(mChannelID, f1, f2); }
    /* vt 0x14 */ virtual void setAccParam(f32 f1, f32 f2) { KPADSetAccParam(mChannelID, f1, f2); }
    /* vt 0x18 */ virtual bool down(u32 mask) const { return mCoreStatus[0].down(mask); }
    /* vt 0x1C */ virtual bool up(u32 mask) const { return mCoreStatus[0].up(mask); }
    /* vt 0x20 */ virtual bool downTrigger(u32 mask) const { return mCoreStatus[0].downTrigger(mask); }
    /* vt 0x24 */ virtual bool upTrigger(u32 mask) const { return mCoreStatus[0].upTrigger(mask); }
    /* vt 0x28 */ virtual bool downAll(u32 mask) const { return mCoreStatus[0].downAll(mask); }
    /* vt 0x2C */ virtual bool upAll(u32 mask) const { return mCoreStatus[0].upAll(mask); }
    /* vt 0x30 */ virtual void beginFrame(PADStatus *padStatus);
    /* vt 0x34 */ virtual void endFrame();

public:
    /* 0x004 */ s32 mChannelID;
    /* 0x008 */ u32 mButtonHeld;
    /* 0x00C */ u32 mButtonTriggered;
    /* 0x010 */ u32 mButtonReleased;
    /* 0x014 */ u32 field_0x14;
    /* 0x018 */ CoreStatus mCoreStatus[16];
    /* 0xf18 */ int mReadStatusIdx;
    /* 0xf1c */ TBitFlag<u8> mFlag;
    /* 0xf20 */ Vector3f mAccel;
    /* 0xf2c */ Vector2f mDpdPos;
    /* 0xf34 */ u32 mIdleTime;
    /* 0xf38 */ u32 mMotorPattern;
    /* 0xf3c */ bool mEnableMotor;
    /* 0xf40 */ s32 mMotorFrameDuration;
    /* 0xf44 */ u8 mMotorPatternLength;
    /* 0xf45 */ u8 mMotorPatternPos;
    /* 0xf48 */ ControllerRumbleMgr *mRumbleMgr;
    /* 0xf4c */ Matrix34f mPostureMatrix;
    /* 0xf7c */ Matrix34f mPostureMatrixPrev;
    /* 0xfac */ TBitFlag<u8> mAccelFlags; // May not be TBitFlag?
    /* 0xfb0 */ s32 mMaxAccelFrameTime;
    /* 0xfb4 */ s32 mAccelFrameTime[3];
    /* 0xfc0 */ f32 mMaxAccelDiff;
    /* 0xfc4 */ Vector3f mPrevAccel;

public:
    CoreController();
    void sceneReset();
    Vector2f getDpdRawPos();
    f32 getDpdDistance();
    s32 getDpdValidFlag(); // Name Assumed. BBA hints at variable
    void startMotor();
    void stopMotor();
    void createRumbleMgr(u8);
    void startPatternRumble(const char *, int, bool);
    void stopRumbleMgr();
    CoreStatus *getCoreStatus(int idx);
    void calc_posture_matrix(Matrix34f &mat, bool);
    f32 getFreeStickX() const;
    f32 getFreeStickY() const;

    const Vector3f &getAccel() const { return mCoreStatus[0].getAccel(); }

    // Guess ?
    bool isStable(u8 p1) const { return (mAccelFlags.getDirect() & p1) == p1; }
};

class CoreControllerMgr {
public:
    struct T__Disposer : Disposer {
        /* vt 0x08 */ virtual ~T__Disposer();
        static T__Disposer *sStaticDisposer;
    };
    /* 0x0000 */ T__Disposer mDisposer;

    struct UnkField0x10A0 {
        virtual void v_08(UNKTYPE *);
    };

public:
    // 0x0010 vtable
    /* vt 0x08 */ virtual void beginFrame();
    /* vt 0x0C */ virtual void endFrame();

public:
    /* 0x0014 */ TBuffer<CoreController *> mControllers;
    /* 0x0020 */ u8 field_0x20[0x10a0 - 0x0020];
    /* 0x10A0 */ UnkField0x10A0 *field_0x10A0;
    /* 0x10A4 */ TBuffer<eCoreDevType> mDevTypes;
    /* 0x10b0 */ u8 field_0x10B0[0x10e0 - 0x10b0];

public:
    static CoreControllerMgr *createInstance();
    static void deleteInstance();
    CoreController *getNthController(int);

    static void *allocThunk(size_t size);
    static int deleteThunk(void *ptr);

    static void connectCallback(s32, s32);
    CoreControllerMgr();

public:
    static CoreControllerMgr *sInstance;
    static CoreController *(*sCoreControllerFactory)();
    static ConnectCallback sConnectCallback;
    static bool sUseBuiltinWpadAllocator;
    static s32 sWPADWorkSize;
};

class NullController : public CoreController {
public:
    NullController() { unk[92] = 0xfe; }
    // idk this has NO effect on anything
    u8 unk[0x80674c00 - 0x80674b10];
};

class ControllerRumbleUnit {
public:
    // 0x00 vtable
    inline ControllerRumbleUnit() : mFlag(0) { init(); }
    /* vt 0x08 */ virtual ~ControllerRumbleUnit() {}

public:
    /* 0x04 */ const char *mPattern;
    /* 0x08 */ const char *mPatternPos;
    /* 0x0C */ s32 mTimer;     // guess
    /* 0x10 */ f32 mRampUp;    // guess
    /* 0x14 */ f32 mIntensity; // guess
    /* 0x18 */ TBitFlag<u8> mFlag;
    /* 0x1C */ nw4r::ut::Node mNode;
    /* 0x24 */ u32 field_0x24; // could just be part of the node (List)
public:
    void init();
    void startPattern(const char *patter, int);
    f32 calc();
};

class ControllerRumbleMgr {
public:
    // 0x00 vtable
    /* vt 0x08 */ virtual ~ControllerRumbleMgr() {}

public:
    /* 0x04 */ nw4r::ut::List mActiveUnitList;
    /* 0x10 */ nw4r::ut::List mInactiveUnitList;
    /* 0x1C */ CoreController *mController;

public:
    ControllerRumbleMgr();
    void createUnit(u8 numUnits, CoreController *controller);
    void stop();
    void calc();
    void startPattern(const char *pattern, int, bool);
    ControllerRumbleUnit *getUnitFromList(bool bGrabActive);
};

} // namespace EGG

#endif
