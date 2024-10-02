
#include "egg/core/eggController.h"

#include "MSL_C/string.h"
#include "egg/core/eggAllocator.h"
#include "egg/core/eggExpHeap.h"
#include "egg/core/eggSystem.h"
#include "egg/math/eggMath.h"
#include "rvl/VI.h"

EGG::NullController null_controller;
namespace EGG {

CoreControllerMgr *CoreControllerMgr::sInstance;
CoreControllerMgr::T__Disposer *CoreControllerMgr::T__Disposer::sStaticDisposer;
ControllerFactory CoreControllerMgr::sCoreControllerFactory;
ConnectCallback CoreControllerMgr::sConnectCallback;
// This controls whether EggController registers an allocator within the WPAD driver
bool CoreControllerMgr::sUseBuiltinWpadAllocator;
s32 CoreControllerMgr::sWPADWorkSize = 0x32000;

void CoreStatus::init() {
    memset(this, 0, sizeof(CoreStatus));
}

u32 CoreStatus::getFSStickButton() const {
    f32 stick = getFSStickX();
    u32 result = 0;

    if (!(-0.25f < stick) || !(stick < 0.25f)) {
        if (stick <= -0.5f) {
            result = 0;
            result |= 0x40000;
        } else if (stick >= 0.5f) {
            result = 0;
            result |= 0x80000;
        }
    }

    stick = getFSStickY();
    if (!(-0.25f < stick) || !(stick < 0.25f)) {
        if (stick <= -0.5f) {
            result &= ~0x10000;
            result |= 0x20000;
        } else if (stick >= 0.5f) {
            result &= ~0x20000;
            result |= 0x10000;
        }
    }

    return result;
}

CoreController::CoreController() : mDpdPos(), mAccel(), mAccelFlags(), mFlag(0) {
    mRumbleMgr = nullptr;
    mButtonHeld = 0;
    mButtonTriggered = 0;
    mButtonReleased = 0;
    sceneReset();
    mFlag.makeAllZero();
}

void CoreController::sceneReset() {
    mAccel.set(0.0, 0.0, 0.0);
    mDpdPos.x = 0.0;
    mDpdPos.y = 0.0;
    mIdleTime = 0;
    mPostureMatrixPrev.makeIdentity();
    mPostureMatrix.makeIdentity();
    mMaxAccelFrameTime = 10;
    mMaxAccelDiff = 0.13;
    mPrevAccel.set(0.0, 0.0, 0.0);
    mAccelFlags.makeAllZero();
    mAccelFrameTime[2] = 0;
    mAccelFrameTime[1] = 0;
    mAccelFrameTime[0] = 0;
    mMotorPattern = 0;
    mMotorFrameDuration = 0;
    mEnableMotor = false;
    mMotorPatternLength = 0x20;
    mMotorPatternPos = 0x1f;
    stopRumbleMgr();
}

Vector2f CoreController::getDpdRawPos() {
    return Vector2f(mCoreStatus[0].mDpdRawX, mCoreStatus[0].mDpdRawY);
}

f32 CoreController::getDpdDistance() {
    return mCoreStatus[0].mDpdDistance;
}

s32 CoreController::getDpdValidFlag() {
    return mCoreStatus[0].getDpdValidFlag();
}

void CoreController::startMotor() {
    WPADControlMotor(mChannelID, WPAD_MOTOR_RUMBLE);
}

void CoreController::stopMotor() {
    WPADControlMotor(mChannelID, WPAD_MOTOR_STOP);
}

void CoreController::createRumbleMgr(u8 numUnits) {
    mRumbleMgr = new ControllerRumbleMgr();
    mRumbleMgr->createUnit(numUnits, this);
}

void CoreController::startPatternRumble(const char *pattern, int duration, bool bGrabActive) {
    if (mRumbleMgr) {
        mRumbleMgr->startPattern(pattern, duration, bGrabActive);
    }
}

void CoreController::stopRumbleMgr() {
    if (mRumbleMgr) {
        mRumbleMgr->stop();
    }
}

CoreStatus *CoreController::getCoreStatus(int idx) {
    return &mCoreStatus[idx];
}

void CoreController::calc_posture_matrix(Matrix34f &posture, bool checkStable) {
    if (checkStable && !isStable(7)) {
        return;
    }
    Vector3f acc = -getAccel();
    Vector3f vy = acc;
    vy.normalise();

    Vector3f vz(0.0f, 0.0f, 1.0f);
    Vector3f vx(1.0f, 0.0f, 0.0f);

    Vector3f bx, by, bz;
    mPostureMatrixPrev.getBase(0, bx);
    mPostureMatrixPrev.getBase(1, by);
    mPostureMatrixPrev.getBase(2, bz);

    Vector3f vz_n(0.0f, 0.0f, -1.0f);
    Vector3f vx_n(-1.0f, 0.0f, 0.0f);

    if (vz.dot(bz) < vz_n.dot(bz)) {
        vz = vz_n;
    }

    if (vx.dot(bx) < vx_n.dot(bx)) {
        vx = vx_n;
    }

    if (Math<f32>::abs(vy.dot(vz)) < Math<f32>::abs(vy.dot(vx))) {
        vx = vy.cross(vz);
        vx.normalise();
        vz = vx.cross(vy);
        vz.normalise();
    } else {
        vz = vx.cross(vy);
        vz.normalise();
        vx = vy.cross(vz);
        vx.normalise();
    }

    posture.setBase(0, vx);
    posture.setBase(1, vy);
    posture.setBase(2, vz);
}

void CoreController::beginFrame(PADStatus *padStatus) {
    s32 kpad_result;
    mReadStatusIdx = KPADReadEx(mChannelID, mCoreStatus, 0x10, &kpad_result);
    if (mReadStatusIdx == 0 && kpad_result == -1 /* Rvl usually uses negative nums for results */) {
        mReadStatusIdx = 1;
    }

    WPADDeviceType dev_type;
    switch (WPADProbe(mChannelID, &dev_type)) {
        case WPAD_ERR_OK: {
            if ((u32)dev_type == WPAD_DEV_NONE) {
                mFlag.resetBit(0);
            } else {
                mFlag.setBit(0);
            }
        } break;
        case WPAD_ERR_NO_CONTROLLER: {
            mFlag.resetBit(0);
        } break;
    }

    if (mReadStatusIdx > 0) {
        CoreStatus *pStatus = mCoreStatus;
        u32 prev_held = mButtonHeld;
        if (pStatus->isFreestyle()) {
            mButtonHeld = pStatus->getFSStickButton();
        } else {
            mButtonHeld = 0;
        }

        mButtonTriggered = mButtonHeld & ~prev_held;
        mButtonReleased = prev_held & ~mButtonHeld;
        pStatus->mHold &= ~0xF0000;
        pStatus->mTrig &= ~0xF0000;
        pStatus->mRelease &= ~0xF0000;
        pStatus->mHold |= (mButtonHeld & 0xF0000);
        pStatus->mTrig |= (mButtonTriggered & 0xF0000);
        pStatus->mRelease |= (mButtonReleased & 0xF0000);
    }

    mPostureMatrixPrev.copyFrom(mPostureMatrix);
    mAccelFlags.makeAllZero();
    Vector3f acc = mCoreStatus->getAccel();

    for (int i = 0; i < 3; i++) {
        if (acc(i) - mAccel(i) < mMaxAccelDiff) {
            if (mMaxAccelFrameTime <= ++mAccelFrameTime[i]) {
                mAccelFrameTime[i] = mMaxAccelFrameTime;
                mAccelFlags.set(1 << i);
                mPrevAccel(i) = acc(i);
            }
        } else {
            mAccelFlags.value &= ~(1 << i); // ?
            mAccelFrameTime[i] = 0;
        }
    }

    calc_posture_matrix(mPostureMatrix, true);

    if (mEnableMotor) {
        if (mMotorPattern & (1 << mMotorPatternPos)) {
            startMotor();
        } else {
            stopMotor();
        }
        if (mMotorPatternPos == 0) {
            mMotorPatternPos = mMotorPatternLength - 1;
        } else {
            mMotorPatternPos = mMotorPatternPos - 1;
        }

        if (--mMotorFrameDuration == 0) {
            stopMotor();
            mEnableMotor = false;
        }
    }

    if (mRumbleMgr) {
        mRumbleMgr->calc();
    }
    bool increment = true;

    if (mCoreStatus->getHold() != 0) {
        increment = false;
    }

    if (increment) {
        Vector3f diff = (mAccel - mCoreStatus->getAccel());
        if (diff.squaredLength() > 0.01f) {
            increment = false;
        }
    }
    if (increment) {
        Vector2f diff = (mDpdPos - getDpdRawPos());
        if (diff.squaredLength() > 0.05f) {
            increment = false;
        }
    }

    if (increment) {
        // I dont know what this is for...
        //   if no interaction, this increments? (30fps -> 2hrs)
        if (mIdleTime < 216000) {
            mIdleTime++;
        }
    } else {
        mIdleTime = 0;
    }
}

void CoreController::endFrame() {
    mAccel = getAccel();
    mDpdPos = getDpdRawPos();
}

/* 0x80499AC0 */
f32 CoreController::getFreeStickX() const {
    if (mCoreStatus->getDevType() == WPAD_DEV_CORE) {
        return 0.0;
    }
    return mCoreStatus[0].getFSStickX();
}

/* 0x80499AE0 */
f32 CoreController::getFreeStickY() const {
    if (mCoreStatus->getDevType() == WPAD_DEV_CORE) {
        return 0.0;
    }
    return mCoreStatus[0].getFSStickY();
}

/* 0x80499B00 */
CoreControllerMgr::T__Disposer::~T__Disposer() {
    if (this == CoreControllerMgr::T__Disposer::sStaticDisposer) {
        deleteInstance();
    }
}

/* 0x80499B80 */
CoreControllerMgr *CoreControllerMgr::createInstance() {
    if (CoreControllerMgr::sInstance == nullptr) {
        CoreControllerMgr *mgr = new CoreControllerMgr();
        CoreControllerMgr::sInstance = mgr;
        CoreControllerMgr::T__Disposer::sStaticDisposer = &mgr->mDisposer;
    }
    return CoreControllerMgr::sInstance;
}

/* 0x80499BD0 */
void CoreControllerMgr::deleteInstance() {
    CoreControllerMgr::sInstance = nullptr;
    CoreControllerMgr::T__Disposer::sStaticDisposer = nullptr;
}

/* 0x80499BE0 */
EGG::CoreController *CoreControllerMgr::getNthController(int n) {
    return mControllers(n);
}

namespace {

static Allocator *sAllocator;

void *alloc(size_t size) {
    return sAllocator->alloc(size);
}

int free(void *ptr) {
    sAllocator->free(ptr);
    return 1;
}

} // namespace

/* 0x80499CD0 */
void CoreControllerMgr::connectCallback(s32 a1, s32 a2) {
    int args[] = {a1, a2};
    if (sConnectCallback != nullptr) {
        (sConnectCallback)(args);
    }
}

/* 0x80499D10 */
CoreControllerMgr::CoreControllerMgr() {
    const int idxes[] = {0, 1, 2, 3};
    if (sUseBuiltinWpadAllocator == false) {
        Heap *heap = ExpHeap::create(sWPADWorkSize, BaseSystem::mConfigData->mRootHeapMem2, 0);
        heap->mName = "EGG::CoreControllerMgr";
        sAllocator = new Allocator(heap, 0x20);
        WPADRegisterAllocator(alloc, free);
    }
    KPADInitEx(field_0x20, 0x40);
    beginFrame();
    endFrame();
    VIWaitForRetrace();

    for (int i = 0; i < 4; i++) {
        KPADSetConnectCallback(idxes[i], connectCallback);
    }

    mControllers.allocate(4, 0);
    for (int i = 0; i < 4; i++) {
        if (sCoreControllerFactory != nullptr) {
            mControllers(i) = (sCoreControllerFactory)();
        } else {
            mControllers(i) = new CoreController();
        }
    }

    mDevTypes.allocate(4, 0);
    for (int i = 0; i < 4; i++) {
        mControllers(i)->mChannelID = idxes[i];
        mDevTypes(i) = (eCoreDevType)0xfd;
    }
    field_0x10A0 = nullptr;
}

/* 0x8049A130 */
void CoreControllerMgr::beginFrame() {
    for (int i = 0; i < mControllers.getSize(); ++i) {
        mControllers(i)->beginFrame(NULL);
    }
}

/* 0x8049A1E0 */
void CoreControllerMgr::endFrame() {
    for (int i = 0; i < mControllers.mSize; i++) {
        mControllers(i)->endFrame();

        WPADDeviceType dev_type;
        s32 result = WPADProbe(i, &dev_type);

        WPADDeviceType res_dev_type;
        if (result == WPAD_ERR_OK) {
            res_dev_type = dev_type;
        } else if (result == WPAD_ERR_NO_CONTROLLER) {
            res_dev_type = WPAD_DEV_NONE;
        } else {
            continue;
        }

        if (field_0x10A0) {
            if (res_dev_type != mDevTypes(i)) {
                s32 sp0C[3];
                sp0C[0] = res_dev_type;
                sp0C[1] = mDevTypes(i);
                sp0C[2] = i;

                field_0x10A0->v_08(sp0C);
            }
        }

        mDevTypes(i) = (eCoreDevType)res_dev_type;
    }
}

/* 0x8049A3B0 */
void ControllerRumbleUnit::init() {
    mPattern = nullptr;
    mPatternPos = nullptr;
    mTimer = 0;
    mIntensity = 0.0;
    mRampUp = 0.0;
    mFlag.makeAllZero();
}

/* 0x8049A3E0 */
void ControllerRumbleUnit::startPattern(const char *pattern, int duration) {
    mPattern = pattern;
    mPatternPos = pattern;

    mFlag.value &= 0xef;
    mFlag.value &= 0xdf;

    if (duration < 0) {
        mFlag.set(0x10);
    } else if (duration > 0) {
        mFlag.set(0x30);
    }

    mTimer = duration;

    mFlag.setBit(0);
    mFlag.resetBit(1);
    mFlag.setBit(3);
}

/* 0x8049A440 */
f32 ControllerRumbleUnit::calc() {
    f32 result = 0.0f;
    if (mFlag.onBit(3)) {
        if (mFlag.onBit(0)) {
            char x = *++mPatternPos;
            if (x == '\0') {
                if (mFlag.onBit(4)) {
                    mPatternPos = mPattern;
                } else {
                    mFlag.resetBit(3);
                }
            } else if (x == '*') {
                result = 1.0f;
            }

            if (mFlag.offBit(5)) {
                return result;
            }

            if (--mTimer > 0) {
                return result;
            }
            mFlag.resetBit(3);
            return result;
        } else {
            f32 intensity = mIntensity + mRampUp;
            mIntensity = intensity;
            if (intensity >= 1.0f) {
                result = 1.0f;
                mIntensity = 0.0f;
            }

            if (mFlag.onBit(2)) {
                return result;
            }

            if (--mTimer > 0) {
                return result;
            }
        }
        mFlag.resetBit(3);
        return result;
    } else {
        return -1.0f;
    }
}

/* 0x8049A530 */
ControllerRumbleMgr::ControllerRumbleMgr() {
    mController = nullptr;
    List_Init(&mActiveUnitList, offsetof(ControllerRumbleUnit, mNode));
    List_Init(&mInactiveUnitList, offsetof(ControllerRumbleUnit, mNode));
}

/* 0x8049A590 */
void ControllerRumbleMgr::createUnit(u8 numUnits, CoreController *ctrl) {
    for (u8 created = 0; created < numUnits; created++) {
        ControllerRumbleUnit *unit = new ControllerRumbleUnit();
        List_Append(&mInactiveUnitList, unit);
    }
    mController = ctrl;
}

/* 0x8049A620 */
void ControllerRumbleMgr::stop() {
    mController->stopMotor();
    while (List_GetSize(&mActiveUnitList) != 0) {
        ControllerRumbleUnit *unit = static_cast<ControllerRumbleUnit *>(List_GetNext(&mActiveUnitList, nullptr));
        List_Remove(&mActiveUnitList, unit);
        List_Append(&mInactiveUnitList, unit);
    }
}

/* 0x8049A690 */
void ControllerRumbleMgr::calc() {
    if (List_GetSize(&mActiveUnitList) != 0) {
        void *object = List_GetFirst(&mActiveUnitList);
        f32 acc = 0.0f;

        while (object != nullptr) {
            ControllerRumbleUnit *unit = static_cast<ControllerRumbleUnit *>(object);
            f32 x = unit->calc();
            void *nextObject = List_GetNext(&mActiveUnitList, object);
            if (x < 0.0f) {
                List_Remove(&mActiveUnitList, object);
                List_Append(&mInactiveUnitList, object);
            } else {
                acc += x;
            }
            object = nextObject;
        }

        if (acc >= 1.0f) {
            mController->startMotor();
        } else {
            mController->stopMotor();
        }
    }
}

/* 0x8049A7A0 */
void ControllerRumbleMgr::startPattern(const char *pattern, int duration, bool bGrabActive) {
    EGG::ControllerRumbleUnit *unit = getUnitFromList(bGrabActive);
    if (unit) {
        unit->startPattern(pattern, duration);
    }
}

/* 0x8049A7F0 */
ControllerRumbleUnit *ControllerRumbleMgr::getUnitFromList(bool bGrabActive) {
    void *first = List_GetFirst(&this->mInactiveUnitList);
    if (first) {
        List_Remove(&mInactiveUnitList, first);
        List_Append(&mActiveUnitList, first);
    } else if (bGrabActive && (first = List_GetFirst(&mActiveUnitList), first != nullptr)) {
        List_Remove(&mActiveUnitList, first);
        List_Append(&mActiveUnitList, first);
    }

    return static_cast<ControllerRumbleUnit *>(first);
}

} // namespace EGG
