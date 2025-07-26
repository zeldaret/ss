#include "d/d_pad_nav.h"

#include "d/d_pad.h"
#include "m/m_vec.h"

namespace dPadNav {

bool sIsNavEnabled = false;
bool sIsMplsNavLeftGesture = false;
bool sIsMplsNavRightGesture = false;
static bool sDisableAutoReturnToPointerNav = false;

bool sIsPointerVisible = true;
bool sPrevIsPointerVisible = true;
bool sIsCursorStickVisible = true;

s32 sFSStickDirection;
s32 sFSStickNavDirection;

static s32 sFSStickNavActiveTimer = 0;
static s32 sFSStickDirectionStableTimer = 0;
static s32 sMPLSNavGestureTimer = 0;

void init() {
    sIsNavEnabled = false;
    sIsPointerVisible = true;
    sIsMplsNavLeftGesture = false;
    sIsMplsNavRightGesture = false;
    sDisableAutoReturnToPointerNav = false;
    sPrevIsPointerVisible = true;
    sFSStickDirection = FS_STICK_NONE;
    sFSStickNavDirection = FS_STICK_NONE;
    sFSStickNavActiveTimer = 0;
    sFSStickDirectionStableTimer = 0;
    sMPLSNavGestureTimer = 0;
    setCursorStickInvisible();
}

void calc() {
    sPrevIsPointerVisible = sIsPointerVisible;
    sFSStickNavDirection = FS_STICK_NONE;

    if (!sIsNavEnabled) {
        return;
    }

    mVec3_c vel = dPad::ex_c::getInstance()->getMPLSVelocity();
    if (sMPLSNavGestureTimer != 0) {
        sMPLSNavGestureTimer--;
        sIsMplsNavLeftGesture = false;
        sIsMplsNavRightGesture = false;
    } else {
        checkForNavLeftGesture();
        checkForNavRightGesture();
    }

    if (vel.squareMagXY() > 1.5f) {
        // Large MPLS movement - stop FS stick navigation
        stopFSStickNav();
        return;
    }

    if (sIsPointerVisible) {
        if (dPad::ex_c::getInstance()->getFSStickTrig()) {
            // starting FS stick navigation - immediate nav event, with delay of
            // 8 frames (267ms) for repeat nav event
            sFSStickDirection = getFSStickDirectionTrig();
            sFSStickNavDirection = sFSStickDirection;
            sFSStickNavActiveTimer = 120;
            sFSStickDirectionStableTimer = 8;
            sIsPointerVisible = false;
        }
    } else {
        s32 direction = getFSStickDirection();
        if (sFSStickDirection != FS_STICK_NONE && direction == sFSStickDirection) {
            // no change in FS stick direction
            sFSStickNavActiveTimer = 120;
            if (sFSStickDirectionStableTimer == 0) {
                // repeat event - now with a delay of 3 frames (100ms)
                sFSStickNavDirection = sFSStickDirection;
                sFSStickDirectionStableTimer = 3;
            } else {
                sFSStickDirectionStableTimer--;
            }
        } else {
            if (dPad::ex_c::getInstance()->getFSStickTrig()) {
                // change in FS stick direction - same as if started navigation
                direction = getFSStickDirectionTrig();
                sFSStickDirectionStableTimer = 8;
                sFSStickDirection = direction;
                sFSStickNavDirection = direction;
                sFSStickNavActiveTimer = 120;
            } else {
                // let go of FS stick
                sFSStickDirection = FS_STICK_NONE;
                sFSStickDirectionStableTimer = 0;
            }
        }

        if (!sDisableAutoReturnToPointerNav) {
            if (sFSStickNavActiveTimer == 0) {
                // FS stick nav timer expired (or was set to 0 via explicit call),
                // check if we need to return to pointer nav
                if (direction != FS_STICK_NONE && sFSStickDirection == direction) {
                    sFSStickNavActiveTimer = 120;
                } else {
                    if (direction == FS_STICK_NONE) {
                        // No direction, return to pointer nav
                        sFSStickDirection = FS_STICK_NONE;
                        sIsPointerVisible = true;
                    } else {
                        sFSStickDirectionStableTimer = 8;
                        // Note: Redundant check
                        if (!sDisableAutoReturnToPointerNav) {
                            sFSStickDirection = direction;
                        }
                        sFSStickNavDirection = sFSStickDirection;
                        sFSStickNavActiveTimer = 120;
                    }
                }
            } else {
                sFSStickNavActiveTimer--;
            }
        }
    }
}

void setNavEnabled(bool navEnabled, bool disableAutoReturnToPointerNav) {
    sIsNavEnabled = navEnabled;
    sIsPointerVisible = true;
    if (navEnabled) {
        setCursorStickVisible();
        sDisableAutoReturnToPointerNav = disableAutoReturnToPointerNav;
    } else {
        setCursorStickInvisible();
        sDisableAutoReturnToPointerNav = false;
    }
}

s32 getFSStickDirectionTrig() {
    if (dPad::ex_c::getInstance()->getFSStickTrig(0x1)) {
        return FS_STICK_UP;
    }

    if (dPad::ex_c::getInstance()->getFSStickTrig(0x80)) {
        return FS_STICK_UP_RIGHT;
    }

    if (dPad::ex_c::getInstance()->getFSStickTrig(0x40)) {
        return FS_STICK_RIGHT;
    }

    if (dPad::ex_c::getInstance()->getFSStickTrig(0x20)) {
        return FS_STICK_DOWN_RIGHT;
    }

    if (dPad::ex_c::getInstance()->getFSStickTrig(0x10)) {
        return FS_STICK_DOWN;
    }

    if (dPad::ex_c::getInstance()->getFSStickTrig(0x8)) {
        return FS_STICK_DOWN_LEFT;
    }

    if (dPad::ex_c::getInstance()->getFSStickTrig(0x4)) {
        return FS_STICK_LEFT;
    }

    if (dPad::ex_c::getInstance()->getFSStickTrig(0x2)) {
        return FS_STICK_UP_LEFT;
    }

    return FS_STICK_NONE;
}

s32 getFSStickDirection() {
    switch (dPad::ex_c::getInstance()->mFSStickMask) {
        case 0x1:  return FS_STICK_UP;
        case 0x80: return FS_STICK_UP_RIGHT;
        case 0x40: return FS_STICK_RIGHT;
        case 0x20: return FS_STICK_DOWN_RIGHT;
        case 0x10: return FS_STICK_DOWN;
        case 0x8:  return FS_STICK_DOWN_LEFT;
        case 0x4:  return FS_STICK_LEFT;
        case 0x2:  return FS_STICK_UP_LEFT;
    }

    return FS_STICK_NONE;
}

void stopFSStickNav() {
    sFSStickNavActiveTimer = 0;
    sIsPointerVisible = true;
}

void hidePointer() {
    sIsPointerVisible = false;
}

void setCursorStickInvisible() {
    sIsCursorStickVisible = false;
}

void setCursorStickVisible() {
    sIsCursorStickVisible = true;
}

void checkForNavLeftGesture() {
    mVec3_c vel = dPad::ex_c::getInstance()->getMPLSVelocity();
    if (vel.y <= 0.0f && vel.squareMagXY() > 1.5f) {
        if (!sIsPointerVisible) {
            sMPLSNavGestureTimer = 10;
            sIsMplsNavLeftGesture = false;
        } else {
            sIsMplsNavLeftGesture = true;
        }
    } else {
        sIsMplsNavLeftGesture = false;
    }
}

void checkForNavRightGesture() {
    mVec3_c vel = dPad::ex_c::getInstance()->getMPLSVelocity();
    if (vel.y >= 0.0f && vel.squareMagXY() > 1.5f) {
        if (!sIsPointerVisible) {
            sMPLSNavGestureTimer = 10;
            sIsMplsNavRightGesture = false;
        } else {
            sIsMplsNavRightGesture = true;
        }
    } else {
        sIsMplsNavRightGesture = false;
    }
}

void scrollRelated() {
    sFSStickDirectionStableTimer = 3;
    sFSStickNavActiveTimer = 120;
}

} // namespace dPadNav
