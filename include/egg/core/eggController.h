#ifndef EGG_CONTROLLER_H
#define EGG_CONTROLLER_H

#include <common.h>

#include "egg/math/eggMath.h"
#include "egg/math/eggMatrix.h"
#include "egg/math/eggVector.h"
#include "egg/prim/eggBitFlag.h"
#include "egg/prim/eggBuffer.h"
#include "rvl/PAD.h"

namespace EGG {

class ControllerRumbleMgr;
class CoreController;

// To Be Filled out
enum eCoreDevType {};

typedef void (*ConnectCallback)(int args[]);
typedef class CoreController *(*ControllerFactory)();

class CoreStatus {
public:
    /* 0x00 */ u8 field_0x00[0x0C];
    /* 0x0C */ f32 accel[3];
    /* 0x18 */ u8 field_0x01[0x20 - 0x18];
    /* 0x20 */ f32 dpdRawX;
    /* 0x24 */ f32 dpdRawY;
    /* 0x28 */ u8 field_0x02[0x54 - 0x28];
    /* 0x54 */ f32 float_0x54;
    /* 0x58 */ f32 float_0x58;
    /* 0x5C */ u8 field_0x5C[0x5E - 0x5C];
    /* 0x5E */ s8 dpdDistance;
    /* 0x5F */ s8 unknown;
    /* 0x60 */ f32 fsStickButton;
    /* 0x60 */ f32 fsStickButton2;
    /* 0x64 */ u8 field_0x03[0x88];

public:
    /* 80498f90 */ void init();
    /* 80498fa0 */ u32 getFSStickButton() const;

    inline Vector2f getUnk() {
        return Vector2f(float_0x54, float_0x58);
    }
};

class CoreController {
public:
    // vtable 0x000 | 8056ec50
    // TODO all of these have inline implementations and are scattered
    // across the binary
    /* vt 0x08 | 80064920 */ virtual void setPosParam(f32, f32) {
        // TODO
    }
    /* vt 0x0C | 8049a940 */ virtual void setHoriParam(f32, f32) {
        // TODO
    }
    /* vt 0x10 | 8049a930 */ virtual void setDistParam(f32, f32) {
        // TODO
    }
    /* vt 0x14 | 8049a920 */ virtual void setAccParam(f32, f32) {
        // TODO
    }
    /* vt 0x18 | 80059820 */ virtual bool isPressed(u32 mask) {
        // TODO
        return false;
    }
    /* vt 0x1C | 80059a60 */ virtual bool isAnyPressed(u32 mask) {
        // TODO
        return false;
    }
    /* vt 0x20 | 80014e30 */ virtual bool isTriggered(u32 mask) {
        // TODO
        return false;
    }
    /* vt 0x24 | 800599e0 */ virtual bool isReleased() {
        // TODO
        return false;
    }
    /* vt 0x28 | 80059840 */ virtual bool isAllPressed() {
        // TODO
        return false;
    }
    /* vt 0x2C | 80059a80 */ virtual bool isNotPressed() {
        // TODO
        return false;
    }
    // We know the above are inline because if a class has any non-inline virtual functions,
    // then the TU that contains an implementation of said function gets the vtable,
    // and we know that eggController.cpp contains the vtable and the functions below
    /* vt 0x30 | 80499660 */ virtual void beginFrame(void *padStatus); // Really needs to be PADStatus
    /* vt 0x34 | 80499a60 */ virtual void endFrame();

public:
    /* 0x004 */ s32 mChannelID;
    /* 0x008 */ u32 mButtonHeld; // these fields may need a slight amount of work
    /* 0x00C */ u32 mButtonTriggered;
    /* 0x010 */ u32 mButtonReleased;
    /* 0x014 */ u32 field_0x14;
    /* 0x018 */ CoreStatus coreStatus[16];
    /* 0xf18 */ int mKPADResult;
    /* 0xf1c */ TBitFlag<u8> mFlag;
    /* 0xf20 */ Vector3f mAccel;
    /* 0xf2c */ Vector2f mDpdPos;
    /* 0xf34 */ u32 field_0xf28;
    /* 0xf38 */ u32 mMotorPattern;
    /* 0xf3c */ bool mEnableMotor;
    /* 0xf40 */ s32 mMotorFrameDuration;
    /* 0xf44 */ u8 mMotorPatternLength;
    /* 0xf45 */ u8 mMotorPatternPos;
    /* 0xf48 */ ControllerRumbleMgr *mRumbleMgr;
    /* 0xf4c */ Matrix34f mPostureMatrix;
    /* 0xf7c */ Matrix34f mPostureMatrixPrev;
    /* 0xfac */ TBitFlag<u8> mAccelFlags;
    /* 0xfb0 */ s32 mMaxAccelFrameTime;
    /* 0xfb4 */ s32 mAccelFrameTimeX;
    /* 0xfb8 */ s32 mAccelFrameTimeY;
    /* 0xfbc */ s32 mAccelFrameTimeZ;
    /* 0xfc0 */ f32 mMaxAccelDiff;
    /* 0xfc4 */ Vector3f mPrevAccel;

public:
    /* 80499050 */ CoreController();
    /* 804990b0 */ void sceneReset();
    /* 80499170 */ Vector2f getDpdRawPos() const;
    /* 80499190 */ s32 getDpdDistance() const;
    /* 804991a0 */ void startMotor();
    /* 804991b0 */ void stopMotor();
    /* 804991c0 */ void createRumbleMgr(u8);
    /* 80499220 */ void startPatternRumble(const char *, int, bool);
    /* 80499240 */ void stopRumbleMgr();
    /* 80499260 */ CoreStatus *getCoreStatus(s32 idx);
    /* 80499270 */ void calc_posture_matrix(Matrix34f &mat, bool);
    /* 80499ac0 */ f32 getFreeStickX() const;
    /* 80499ae0 */ f32 getFreeStickY() const;
};

class CoreControllerMgr {
public:
    struct T__Disposer : Disposer {
        /* vt 0x08 | 80499b00 */ virtual ~T__Disposer();
        /* 805767ac */ static T__Disposer *sStaticDisposer;
    };
    // Disposer Vtable: 8056ec40
    /* 0x0000 */ T__Disposer mDisposer; // for the static T__Disposer
public:
    // 0x0010 vtable | 8056ebf8
    /* vt 0x08 | 8049a130 */ virtual void beginFrame();
    /* vt 0x0C | 8049a1e0 */ virtual void endFrame();

public:
    /* 0x0014 */ TBuffer<CoreController *> mControllers;
    /* 0x0020 */ u8 field_0x20[0x10a0 - 0x0020];
    /* 0x10A0 */ u32 field_0x10A0;
    /* 0x10A4 */ TBuffer<eCoreDevType> mDevTypes;
    /* 0x10b0 */ u8 field_0x10B0[0x10e0 - 0x10b0];

public:
    /* 80499b80 */ static CoreControllerMgr *createInstance();
    /* 80499bd0 */ static void deleteInstance();
    /* 80499be0 */ CoreController *getNthController(int);

    static void *allocThunk(size_t size);
    static int deleteThunk(void *ptr);

    /* 80499cd0 */ static void connectCallback(s32, s32);
    /* 80499d10 */ CoreControllerMgr();

public:
    /* 805767a8 */ static CoreControllerMgr *sInstance;
    /* 805767b0 */ static CoreController *(*sCoreControllerFactory)();
    /* 805767b4 */ static ConnectCallback sConnectCallback;
    /* 805767b8 */ static bool sUseBuiltinWpadAllocator;
    // /* 805767bc */ static sAllocator; // defined in cpp file
};

class NullController : public CoreController {
public:
    NullController() {
        unk[92] = 0xfe;
    }
    // idk this has NO effect on anything
    u8 unk[0x80674c00 - 0x80674b10];
};

class ControllerRumbleUnit {
public:
    // 0x00 vtable | 8056ebb4
    inline ControllerRumbleUnit() : mFlag(0) {
        init();
    }
    /* vt 0x08 | 8049a8e0 */ virtual ~ControllerRumbleUnit() {}

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
    /* 8049a3b0 */ void init();
    /* 8049a3e0 */ void startPattern(const char *patter, int);
    /* 8049a440 */ f32 calc();
};

class ControllerRumbleMgr {
public:
    // 0x00 vtable | 8056eba8
    /* vt 0x08 | 8049a8a0 */ virtual ~ControllerRumbleMgr() {}

public:
    /* 0x04 */ nw4r::ut::List mActiveUnitList;
    /* 0x10 */ nw4r::ut::List mInactiveUnitList;
    /* 0x1C */ CoreController *mController;

public:
    /* 8049a530 */ ControllerRumbleMgr();
    /* 8049a590 */ void createUnit(u8 numUnits, CoreController *controller);
    /* 8049a620 */ void stop();
    /* 8049a690 */ void calc();
    /* 8049a7a0 */ void startPattern(const char *pattern, int, bool);
    /* 8049a7f0 */ ControllerRumbleUnit *getUnitFromList(bool bGrabActive);
};

} // namespace EGG

#endif
