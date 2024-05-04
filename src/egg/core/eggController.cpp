#include "egg/core/eggController.h"
#include "MSL_C/string.h"
#include "rvl/VI.h"

extern EGG::CoreControllerMgr *lbl_805767AC;

EGG::NullController null_controller;

// TODO vtable order in this TU is still messed up
// TODO NullController implementation

namespace EGG {

CoreControllerMgr *CoreControllerMgr::sInstance;
CoreControllerMgr::T__Disposer *CoreControllerMgr::T__Disposer::sStaticDisposer;
void *CoreControllerFactory; // TODO
ConnectCallback CoreControllerMgr::sConnectCallback;
// This controls whether EggController registers an allocator within the WPAD driver
bool CoreControllerMgr::sUseBuiltinWpadAllocator;
void *TODO_Allocator; // TODO

/* 0x80498F90 */ void CoreStatus::init() {
    memset(this, 0, sizeof(CoreStatus));
}

/* 0x80498FA0 */ u32 CoreStatus::getFSStickButton() const {
    f32 button = this->fsStickButton;
    u32 result = 0;
    // TODO what are these flags and why is this code so weird?

    if (!(-0.25f < button) || !(button < 0.25f)) {
        if (button <= -0.5f) {
            result = 0;
            result |= 0x40000;
        } else if (button >= 0.5f) {
            result = 0;
            result |= 0x80000;
        }
    }

    button = this->fsStickButton2;
    if (!(-0.25f < button) || !(button < 0.25f)) {
        if (button <= -0.5f) {
            result &= ~0x10000;
            result |= 0x20000;
        } else if (button >= 0.5f) {
            result &= ~0x20000;
            result |= 0x10000;
        }
    }

    return result;
}

/* 0x80499050 */ CoreController::CoreController() : mDpdPos(), mAccel(), mAccelFlags(nullptr) {
    this->mRumbleMgr = nullptr;
    this->mButtonHeld = 0;
    this->mButtonTriggered = 0;
    this->mButtonReleased = 0;
    this->sceneReset();
    this->mFlag.makeAllZero();
}

/* 0x804990B0 */ void CoreController::sceneReset() {
    this->mAccel.set(0.0, 0.0, 0.0);
    this->mDpdPos.x = 0.0;
    this->mDpdPos.y = 0.0;
    this->field_0xf28 = 0;
    this->mPostureMatrixPrev.makeIdentity();
    this->mPostureMatrix.makeIdentity();
    this->mMaxAccelFrameTime = 10;
    this->mMaxAccelDiff = 0.13;
    this->mPrevAccel.set(0.0, 0.0, 0.0);
    this->mAccelFlags.makeAllZero();
    this->mAccelFrameTimeZ = 0;
    this->mAccelFrameTimeY = 0;
    this->mAccelFrameTimeX = 0;
    this->mMotorPattern = 0;
    this->mMotorFrameDuration = 0;
    this->mEnableMotor = false;
    this->mMotorPatternLength = 0x20;
    this->mMotorPatternPos = 0x1f;
    this->stopRumbleMgr();
}

/* 0x80499170 */ Vector2f CoreController::getDpdRawPos() const {
    return Vector2f(this->coreStatus[0].dpdRawX, this->coreStatus[0].dpdRawY);
}

/* 0x80499190 */ s32 CoreController::getDpdDistance() const {
    return this->coreStatus[0].dpdDistance;
}

// TODO
extern "C" void fn_803DB1E0(s32 channel, bool arg);

/* 0x804991A0 */ void CoreController::startMotor() {
    fn_803DB1E0(mChannelID, true);
}

/* 0x804991B0 */ void CoreController::stopMotor() {
    fn_803DB1E0(mChannelID, false);
}

/* 0x804991C0 */ void CoreController::createRumbleMgr(u8 numUnits) {
    this->mRumbleMgr = new ControllerRumbleMgr();
    this->mRumbleMgr->createUnit(numUnits, this);
}

/* 0x80499220 */ void CoreController::startPatternRumble(const char *pattern, int duration, bool bGrabActive) {
    if (this->mRumbleMgr) {
        this->mRumbleMgr->startPattern(pattern, duration, bGrabActive);
    }
}

/* 0x80499240 */ void CoreController::stopRumbleMgr() {
    if (this->mRumbleMgr) {
        this->mRumbleMgr->stop();
    }
}

/* 0x80499260 */ CoreStatus *CoreController::getCoreStatus(s32 idx) {
    return &this->coreStatus[idx];
}

/* 0x80499270 */ void CoreController::calc_posture_matrix(Matrix34f &mat, bool someBool) {
    if (!someBool || this->mAccelFlags.onBit(7)) {
        Vector3f vec = Vector3f(-mat(2, 3), -mat(2, 2), -mat(2, 1));
        Vector3f vec3 = vec;
        vec.normalise();
        // TODO lots of inlined math
    }
}

/* 0x80499660 */ void CoreController::beginFrame(void *padStatus) {}

/* 0x80499A60 */ void CoreController::endFrame() {
    this->mAccel(0) = this->coreStatus[0].accel[0];
    this->mAccel(1) = this->coreStatus[0].accel[1];
    this->mAccel(2) = this->coreStatus[0].accel[2];
    this->mDpdPos = this->getDpdRawPos();
}

/* 0x80499AC0 */ f32 CoreController::getFreeStickX() const {
    if (this->coreStatus[0].field_0x00[0x5C] == 0) {
        return 0.0;
    }
    return ((f32 *)(&this->coreStatus[0].field_0x00))[0x18];
}

/* 0x80499AE0 */ f32 CoreController::getFreeStickY() const {
    if (this->coreStatus[0].field_0x00[0x5C] == 0) {
        return 0.0;
    }
    return ((f32 *)(&this->coreStatus[0].field_0x00))[0x19];
}

/* 0x80499B00 */ CoreControllerMgr::T__Disposer::~T__Disposer() {
    if (this == CoreControllerMgr::T__Disposer::sStaticDisposer) {
        deleteInstance();
    }
}

/* 0x80499B80 */ CoreControllerMgr *CoreControllerMgr::createInstance() {
    if (CoreControllerMgr::sInstance == nullptr) {
        CoreControllerMgr *mgr = new CoreControllerMgr();
        CoreControllerMgr::sInstance = mgr;
        lbl_805767AC = mgr;
    }
    return CoreControllerMgr::sInstance;
}

/* 0x80499BD0 */ void CoreControllerMgr::deleteInstance() {
    CoreControllerMgr::sInstance = nullptr;
    lbl_805767AC = nullptr;
}

/* 0x80499BE0 */ EGG::CoreController *CoreControllerMgr::getNthController(s32 n) {
    return this->mControllers(n);
}

/* 0x80499C70 */ void *CoreControllerMgr::allocThunk(size_t size) {
    return nullptr; // TODO
}
/* 0x80499C90 */ void CoreControllerMgr::deleteThunk(void *ptr) {
    // TODO
}

/* 0x80499CD0 */ void CoreControllerMgr::connectCallback(int a1, int a2) {
    int args[] = {a1, a2};
    if (sConnectCallback != nullptr) {
        (sConnectCallback)(args);
    }
}

/* 0x80499D10 */ CoreControllerMgr::CoreControllerMgr() {
    const int idxes[] = {0, 1, 2, 3};
    if (sUseBuiltinWpadAllocator == false) {
        // TODO create heap, register allocator thunks
    }
    // init KPAD
    beginFrame();
    endFrame();
    VIWaitForRetrace();

    // TODO there's likely an allocate call here but it shifts the
    // inline buffer methods up in the TU :(
    // mControllers.allocate(4, 0);
    // TODO moar
}

/* 0x8049A130 */ void CoreControllerMgr::beginFrame() {
    for (int i = 0; i < mControllers.getSize(); ++i) {
        mControllers(i)->beginFrame(NULL);
    }
}

/* 0x8049A1E0 */ void CoreControllerMgr::endFrame() {
    for (int i = 0; i < mControllers.mSize; i++) {
        mControllers(i)->endFrame();
        // TODO WPADprobe
        // Update device type after probe?
    }
}

/* 0x8049A3B0 */ void ControllerRumbleUnit::init() {
    this->mPattern = nullptr;
    this->mPatternPos = nullptr;
    this->mTimer = 0;
    this->mIntensity = 0.0;
    this->mRampUp = 0.0;
    this->mFlag.makeAllZero();
}

/* 0x8049A3E0 */ void ControllerRumbleUnit::startPattern(const char *pattern, int duration) {
    this->mPattern = pattern;
    this->mPatternPos = pattern;

    this->mFlag.value &= 0xef;
    this->mFlag.value &= 0xdf;

    if (duration < 0) {
        this->mFlag.set(0x10);
    } else if (duration > 0) {
        this->mFlag.set(0x30);
    }

    this->mTimer = duration;
    this->mFlag.value = ((this->mFlag.value | 0x01) & 0xfd) | 0x8;
}

/* 0x8049A440 */ f32 ControllerRumbleUnit::calc() {
    f32 result = 0.0f;
    if (this->mFlag.onBit(3)) {
        if (this->mFlag.onBit(0)) {
            char x = *++this->mPatternPos;
            if (x == '\0') {
                if (this->mFlag.onBit(4)) {
                    this->mPatternPos = this->mPattern;
                } else {
                    this->mFlag.reset(~0xf7);
                }
            } else if (x == '*') {
                result = 1.0f;
            }

            if (this->mFlag.offBit(5)) {
                return result;
            }

            if (--this->mTimer > 0) {
                return result;
            }
            this->mFlag.reset(~0xf7);
            return result;
        } else {
            f32 intensity = this->mIntensity + this->mRampUp;
            this->mIntensity = intensity;
            if (intensity >= 1.0f) {
                result = 1.0f;
                this->mIntensity = 0.0f;
            }

            // infinite flag?
            if (this->mFlag.onBit(2)) {
                return result;
            }

            if (--this->mTimer > 0) {
                return result;
            }
        }
        this->mFlag.reset(~0xf7);
        return result;
    } else {
        return -1.0f;
    }
}

/* 0x8049A530 */ ControllerRumbleMgr::ControllerRumbleMgr() {
    this->mController = nullptr;
    // TODO offsetof macro
    List_Init(&this->mActiveUnitList, 0x1c);
    List_Init(&this->mInactiveUnitList, 0x1c);
}

/* 0x8049A590 */ void ControllerRumbleMgr::createUnit(u8 numUnits, CoreController *ctrl) {
    for (u8 created = 0; created < numUnits; created++) {
        ControllerRumbleUnit *unit = new ControllerRumbleUnit();
        List_Append(&this->mInactiveUnitList, unit);
    }
    this->mController = ctrl;
}

/* 0x8049A620 */ void ControllerRumbleMgr::stop() {
    this->mController->stopMotor();
    while (List_GetSize(&this->mActiveUnitList) != 0) {
        ControllerRumbleUnit *unit = static_cast<ControllerRumbleUnit *>(List_GetNext(&this->mActiveUnitList, nullptr));
        List_Remove(&this->mActiveUnitList, unit);
        List_Append(&this->mInactiveUnitList, unit);
    }
}

/* 0x8049A690 */ void ControllerRumbleMgr::calc() {
    if (List_GetSize(&this->mActiveUnitList) != 0) {
        void *object = List_GetFirst(&this->mActiveUnitList);
        f32 acc = 0.0f;

        while (object != nullptr) {
            ControllerRumbleUnit *unit = static_cast<ControllerRumbleUnit *>(object);
            f32 x = unit->calc();
            void *nextObject = List_GetNext(&this->mActiveUnitList, object);
            if (x < 0.0f) {
                List_Remove(&this->mActiveUnitList, object);
                List_Append(&this->mInactiveUnitList, object);
            } else {
                acc += x;
            }
            object = nextObject;
        }

        if (acc >= 1.0f) {
            this->mController->startMotor();
        } else {
            this->mController->stopMotor();
        }
    }
}

/* 0x8049A7A0 */ void ControllerRumbleMgr::startPattern(const char *pattern, int duration, bool bGrabActive) {
    EGG::ControllerRumbleUnit *unit = getUnitFromList(bGrabActive);
    if (unit != nullptr) {
        unit->startPattern(pattern, duration);
    }
}

/* 0x8049A7F0 */ ControllerRumbleUnit *ControllerRumbleMgr::getUnitFromList(bool bGrabActive) {
    void *first = List_GetFirst(&this->mInactiveUnitList);
    if (first != nullptr) {
        List_Remove(&this->mInactiveUnitList, first);
        List_Append(&this->mActiveUnitList, first);
    } else if (bGrabActive && (first = List_GetFirst(&this->mActiveUnitList), first != nullptr)) {
        List_Remove(&this->mActiveUnitList, first);
        List_Append(&this->mActiveUnitList, first);
    }

    return static_cast<ControllerRumbleUnit *>(first);
}

} // namespace EGG
