#include "egg/core/eggController.h"
#include "MSL_C/string.h"

extern EGG::CoreControllerMgr *lbl_805767A8;
extern EGG::CoreControllerMgr *lbl_805767AC;

const char *str = "EGG::CoreControllerMgr"; // TODO

EGG::NullController null_controller;

namespace EGG
{

void CoreStatus::init() {
    memset(this, 0, sizeof(CoreStatus));
}

u32 CoreStatus::getFSStickButton() const {
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

CoreController::CoreController(): mDpdPos(), mAccel(), mAccelFlags(nullptr) {
    this->mRumbleMgr = nullptr;
    this->mButtonHeld = 0;
    this->mButtonTriggered = 0;
    this->mButtonReleased = 0;
    this->sceneReset();
    this->mFlag.makeAllZero();
}

void CoreController::sceneReset() {
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

Vector2f CoreController::getDpdRawPos() const {
    return Vector2f(this->coreStatus[0].dpdRawX, this->coreStatus[0].dpdRawY);
}

s32 CoreController::getDpdDistance() const {
    return this->coreStatus[0].dpdDistance;
}

// TODO
extern "C" void fn_803DB1E0(s32 channel, bool arg);

void CoreController::startMotor() {
    fn_803DB1E0(mChannelID, true);
}

void CoreController::stopMotor() {
    fn_803DB1E0(mChannelID, false);
}

void CoreController::createRumbleMgr(u8 numUnits) {
    this->mRumbleMgr = new ControllerRumbleMgr();
    this->mRumbleMgr->createUnit(numUnits, this);
}

void CoreController::startPatternRumble(const char *pattern, int duration, bool bGrabActive) {
    if (this->mRumbleMgr) {
        this->mRumbleMgr->startPattern(pattern, duration, bGrabActive);
    }
}

void CoreController::stopRumbleMgr() {
    if (this->mRumbleMgr) {
        this->mRumbleMgr->stop();
    }
}

CoreStatus *CoreController::getCoreStatus(s32 idx) {
    return &this->coreStatus[idx];
}

void CoreController::calc_posture_matrix(Matrix34f &mat, bool someBool) {
    if (!someBool || this->mAccelFlags.onBit(7)) {
        Vector3f vec = Vector3f(-mat(2, 3), -mat(2, 2), -mat(2, 1));
        Vector3f vec3 = vec;
        vec.normalise();
        // TODO
    }
}

void CoreController::beginFrame(void *padStatus) {

}

void CoreController::endFrame() {
    this->mAccel(0) = this->coreStatus[0].accel[0];
    this->mAccel(1) = this->coreStatus[0].accel[1];
    this->mAccel(2) = this->coreStatus[0].accel[2];
    this->mDpdPos = this->getDpdRawPos();
}

f32 CoreController::getFreeStickX() const {
    if (this->coreStatus[0].field_0x00[0x5C] == 0) {
        return 0.0;
    }
    return ((f32*)(&this->coreStatus[0].field_0x00))[0x18];
}

f32 CoreController::getFreeStickY() const {
    if (this->coreStatus[0].field_0x00[0x5C] == 0) {
        return 0.0;
    }
    return ((f32*)(&this->coreStatus[0].field_0x00))[0x19];
}

CoreControllerMgr *CoreControllerMgr::createInstance() {
    if (lbl_805767A8 == nullptr) {
        CoreControllerMgr *mgr = new CoreControllerMgr();
        lbl_805767A8 = mgr;
        lbl_805767AC = mgr;
    }
    return lbl_805767A8;
}

void CoreControllerMgr::deleteInstance() {
    lbl_805767A8 = nullptr;
    lbl_805767AC = nullptr;
}

EGG::CoreController *CoreControllerMgr::getNthController(s32 n) {
    return this->mControllers(n);
}

void CoreControllerMgr::connectCallback(int a1, int a2) {

}

CoreControllerMgr::CoreControllerMgr() {

}

void CoreControllerMgr::beginFrame() {
    for (int i = 0; i < mControllers.getSize(); ++i) {
        mControllers(i)->beginFrame(NULL);
    }
}

void CoreControllerMgr::endFrame() {

}

void ControllerRumbleUnit::init() {
    this->mPattern = nullptr;
    this->mPatternPos = nullptr;
    this->mTimer = 0;
    this->mIntensity = 0.0;
    this->mRampUp = 0.0;
    this->mFlag.makeAllZero();
}

void ControllerRumbleUnit::startPattern(const char *pattern, int duration) {
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

f32 ControllerRumbleUnit::calc() {
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

ControllerRumbleMgr::ControllerRumbleMgr() {
    this->mController = nullptr;
    // TODO offsetof macro
    List_Init(&this->mActiveUnitList, 0x1c);
    List_Init(&this->mInactiveUnitList, 0x1c);
}

void ControllerRumbleMgr::createUnit(u8 numUnits, CoreController *ctrl) {
    for (u8 created = 0; created < numUnits; created++) {
        ControllerRumbleUnit *unit = new ControllerRumbleUnit();
        List_Append(&this->mInactiveUnitList, unit);
    }
    this->mController = ctrl;
}

void ControllerRumbleMgr::stop() {
    this->mController->stopMotor();
    while (List_GetSize(&this->mActiveUnitList) != 0) {
        ControllerRumbleUnit *unit = static_cast<ControllerRumbleUnit*>(List_GetNext(&this->mActiveUnitList, nullptr));
        List_Remove(&this->mActiveUnitList, unit);
        List_Append(&this->mInactiveUnitList, unit);
    }
}

void ControllerRumbleMgr::calc() {
    if (List_GetSize(&this->mActiveUnitList) != 0) {
        void *object = List_GetFirst(&this->mActiveUnitList);
        f32 acc = 0.0f;

        while (object != nullptr) {
            ControllerRumbleUnit *unit = static_cast<ControllerRumbleUnit*>(object);
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

void ControllerRumbleMgr::startPattern(const char *pattern, int duration, bool bGrabActive) {
    EGG::ControllerRumbleUnit *unit = getUnitFromList(bGrabActive);
    if (unit != nullptr) {
        unit->startPattern(pattern, duration);
    }
}

ControllerRumbleUnit *ControllerRumbleMgr::getUnitFromList(bool bGrabActive) {
    void *first = List_GetFirst(&this->mInactiveUnitList);
    if (first != nullptr) {
        List_Remove(&this->mInactiveUnitList, first);
        List_Append(&this->mActiveUnitList, first);
    } else if (bGrabActive && (first = List_GetFirst(&this->mActiveUnitList), first != nullptr)) {    
        List_Remove(&this->mActiveUnitList, first);
        List_Append(&this->mActiveUnitList, first);
    }

    return static_cast<ControllerRumbleUnit*>(first);
}

ControllerRumbleMgr::~ControllerRumbleMgr() {

}

ControllerRumbleUnit::~ControllerRumbleUnit() {

}

void CoreController::setAccParam(f32 param1, f32 param2) {

}

void CoreController::setDistParam(f32 param1, f32 param2) {

}

void CoreController::setHoriParam(f32 param1, f32 param2) {

}


} // namespace EGG
