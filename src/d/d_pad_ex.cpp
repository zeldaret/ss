#include "common.h"
#include "d/d_pad.h"
#include "egg/core/eggController.h"
#include "m/m_angle.h"
#include "m/m_pad.h"
#include "m/m_vec.h"

namespace dPad {

void fn_80059620() {
    return;
}

void fn_80059630() {
    return;
}

void fn_80059640() {
    return;
}

void fn_80059650() {
    return;
}

// GET DOWN TRIG

bool getDownTrig(u32 btns) {
    return mPad::g_currentCore->downTrigger(btns);
}

bool getDownTrigA() {
    return mPad::g_currentCore->downTrigger(mPad::BUTTON_A);
}

bool getDownTrigB() {
    return mPad::g_currentCore->downTrigger(mPad::BUTTON_B);
}

bool getDownTrigC() {
    return mPad::g_currentCore->downTrigger(mPad::BUTTON_C);
}

bool getDownTrigZ() {
    return mPad::g_currentCore->downTrigger(mPad::BUTTON_Z);
}

bool getDownTrigUp() {
    return mPad::g_currentCore->downTrigger(mPad::BUTTON_UP);
}

bool getDownTrigDown() {
    return mPad::g_currentCore->downTrigger(mPad::BUTTON_DOWN);
}

bool getDownTrigLeft() {
    return mPad::g_currentCore->downTrigger(mPad::BUTTON_LEFT);
}

bool getDownTrigRight() {
    return mPad::g_currentCore->downTrigger(mPad::BUTTON_RIGHT);
}

bool getDownTrig1() {
    return mPad::g_currentCore->downTrigger(mPad::BUTTON_1);
}

bool getDownTrig2() {
    return mPad::g_currentCore->downTrigger(mPad::BUTTON_2);
}

bool getDownTrigMinus() {
    return mPad::g_currentCore->downTrigger(mPad::BUTTON_MINUS);
}

bool getDownTrigPlus() {
    return mPad::g_currentCore->downTrigger(mPad::BUTTON_PLUS);
}

// GET DOWN

bool getDown(u32 btns) {
    return mPad::g_currentCore->down(btns);
}
bool getDownAll(u32 btns) {
    return mPad::g_currentCore->downAll(btns);
}
bool getDownA() {
    return mPad::g_currentCore->down(mPad::BUTTON_A);
}
bool getDownB() {
    return mPad::g_currentCore->down(mPad::BUTTON_B);
}
bool getDownC() {
    return mPad::g_currentCore->down(mPad::BUTTON_C);
}
bool getDownZ() {
    return mPad::g_currentCore->down(mPad::BUTTON_Z);
}
bool getDownUp() {
    return mPad::g_currentCore->down(mPad::BUTTON_UP);
}
bool getDownDown() {
    return mPad::g_currentCore->down(mPad::BUTTON_DOWN);
}
bool getDownLeft() {
    return mPad::g_currentCore->down(mPad::BUTTON_LEFT);
}
bool getDownRight() {
    return mPad::g_currentCore->down(mPad::BUTTON_RIGHT);
}
bool getDown1() {
    return mPad::g_currentCore->down(mPad::BUTTON_1);
}
bool getDown2() {
    return mPad::g_currentCore->down(mPad::BUTTON_2);
}
bool getDownMinus() {
    return mPad::g_currentCore->down(mPad::BUTTON_MINUS);
}
bool getDownPlus() {
    return mPad::g_currentCore->down(mPad::BUTTON_PLUS);
}

// GET UP TRIG

bool getUpTrig(u32 btns) {
    return mPad::g_currentCore->upTrigger(btns);
}
bool getUpTrigA() {
    return mPad::g_currentCore->upTrigger(mPad::BUTTON_A);
}
bool getUpTrigB() {
    return mPad::g_currentCore->upTrigger(mPad::BUTTON_B);
}
bool getUpTrigC() {
    return mPad::g_currentCore->upTrigger(mPad::BUTTON_C);
}
bool getUpTrigZ() {
    return mPad::g_currentCore->upTrigger(mPad::BUTTON_Z);
}
bool getUpTrigUp() {
    return mPad::g_currentCore->upTrigger(mPad::BUTTON_UP);
}
bool getUpTrigDown() {
    return mPad::g_currentCore->upTrigger(mPad::BUTTON_DOWN);
}
bool getUpTrigLeft() {
    return mPad::g_currentCore->upTrigger(mPad::BUTTON_LEFT);
}
bool getUpTrigRight() {
    return mPad::g_currentCore->upTrigger(mPad::BUTTON_RIGHT);
}
bool getUpTrig1() {
    return mPad::g_currentCore->upTrigger(mPad::BUTTON_1);
}
bool getUpTrig2() {
    return mPad::g_currentCore->upTrigger(mPad::BUTTON_2);
}
bool getUpTrigMinus() {
    return mPad::g_currentCore->upTrigger(mPad::BUTTON_MINUS);
}
bool getUpTrigPlus() {
    return mPad::g_currentCore->upTrigger(mPad::BUTTON_PLUS);
}

// GET UP

bool getUp(u32 btns) {
    return mPad::g_currentCore->up(btns);
}
bool getUpAll(u32 btns) {
    return mPad::g_currentCore->upAll(btns);
}
bool getUpA() {
    return mPad::g_currentCore->up(mPad::BUTTON_A);
}
bool getUpB() {
    return mPad::g_currentCore->up(mPad::BUTTON_B);
}
bool getUpC() {
    return mPad::g_currentCore->up(mPad::BUTTON_C);
}
bool getUpZ() {
    return mPad::g_currentCore->up(mPad::BUTTON_Z);
}
bool getUpUp() {
    return mPad::g_currentCore->up(mPad::BUTTON_UP);
}
bool getUpDown() {
    return mPad::g_currentCore->up(mPad::BUTTON_DOWN);
}
bool getUpLeft() {
    return mPad::g_currentCore->up(mPad::BUTTON_LEFT);
}
bool getUpRight() {
    return mPad::g_currentCore->up(mPad::BUTTON_RIGHT);
}
bool getUp1() {
    return mPad::g_currentCore->up(mPad::BUTTON_1);
}
bool getUp2() {
    return mPad::g_currentCore->up(mPad::BUTTON_2);
}
bool getUpMinus() {
    return mPad::g_currentCore->up(mPad::BUTTON_MINUS);
}
bool getUpPlus() {
    return mPad::g_currentCore->up(mPad::BUTTON_PLUS);
}

mVec2_c &getDpdPos() {
    return ex_c::m_current_ex->mDpdPos;
}
mVec2_c &getDpdPosScreen() {
    return ex_c::m_current_ex->mDpdPosScreen;
}
mVec2_c &getFSStick() {
    return ex_c::m_current_ex->mFSStick;
}
f32 getFSStickX() {
    return ex_c::m_current_ex->mFSStick.x;
}
f32 getFSStickY() {
    return ex_c::m_current_ex->mFSStick.y;
}
f32 getFSStickDistance() {
    return ex_c::m_current_ex->mFSStickDistance;
}
mAng getFSStickAngle() {
    return ex_c::m_current_ex->mFSStickAngle;
}
mVec3_c getAcc() {
    const EGG::CoreStatus *status = mPad::g_currentCore->getCoreStatus();
    return mVec3_c(status->acc.x, status->acc.y, status->acc.z);
}

u32 fn_80059B750() {
    return ex_c::getInstance()->mMotion.field_0x10C8 >> 0 & 1;
}
u32 fn_80059B760() {
    return ex_c::getInstance()->mMotion.field_0x10C8 >> 1 & 1;
}
u32 fn_80059B70() {
    return ex_c::getInstance()->mMotion.field_0x10C8 >> 2 & 1;
}
u32 fn_80059B80() {
    return ex_c::getInstance()->mMotion.field_0x10C8 >> 3 & 1;
}
u32 fn_80059B90() {
    return ex_c::getInstance()->mMotion.field_0x10C8 >> 5 & 1;
}

mVec3_c getFSAcc() {
    if (mPad::g_padMg->getDevType(mPad::g_currentCoreId) == EGG::cDEV_FREESTYLE ||
        mPad::g_padMg->getDevType(mPad::g_currentCoreId) == EGG::cDEV_MPLS_PT_FS) {
        const EGG::CoreStatus *status = mPad::g_currentCore->getCoreStatus();
        return mVec3_c(status->ex_status.fs.acc.x, status->ex_status.fs.acc.y, status->ex_status.fs.acc.z);
    } else {
        return mVec3_c(mVec3_c::Zero.x, mVec3_c::Zero.y, mVec3_c::Zero.z);
    }
}
u32 fn_80059CC0() {
    return ex_c::getInstance()->mFSMotion.field_0x10C8 >> 4 & 1;
}
u32 fn_80059CD0() {
    return ex_c::getInstance()->mFSMotion.field_0x10C8 >> 5 & 1;
}

} // namespace dPad
