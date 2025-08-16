#ifndef D_PAD_NAV_H
#define D_PAD_NAV_H

/**
 * Navigation with Wiimote and Nunchuk - seamlessly switching
 * between pointer (dpd) and Nunchuk stick (FSStick).
 */
#include "common.h"

namespace dPadNav {

enum FSStickDirection_e {
    FS_STICK_NONE = 0,
    FS_STICK_UP = 1,
    FS_STICK_UP_RIGHT = 2,
    FS_STICK_RIGHT = 3,
    FS_STICK_DOWN_RIGHT = 4,
    FS_STICK_DOWN = 5,
    FS_STICK_DOWN_LEFT = 6,
    FS_STICK_LEFT = 7,
    FS_STICK_UP_LEFT = 8,
};

extern bool sIsNavEnabled;
// WARNING: Swinging the Wiimote to the right -> navigate left.
// This naming refers to the latter (where the navigation goes).
extern bool sIsMplsNavLeftGesture;
extern bool sIsMplsNavRightGesture;

extern bool sIsPointerVisible;
extern bool sPrevIsPointerVisible;
extern bool sIsCursorStickVisible;

extern s32 sFSStickDirection;
extern s32 sFSStickNavDirection;

inline bool isPointerVisible() {
    return sIsPointerVisible;
}

inline bool isPrevPointerVisible() {
    return sPrevIsPointerVisible;
}

inline bool isCursorStickVisible() {
    return sIsCursorStickVisible;
}

inline s32 getFSStickNavDirection() {
    return sFSStickNavDirection;
}

inline bool isMplsNavLeftGesture() {
    return sIsMplsNavLeftGesture;
}

inline bool isMplsNavRightGesture() {
    return sIsMplsNavRightGesture;
}

inline bool isMplsNavGesture() {
    return isMplsNavLeftGesture() || isMplsNavRightGesture();
}

void init();
void calc();
void setNavEnabled(bool navEnabled, bool disableAutoReturnToPointerNav);

// I really wish these returned enums but the codegen requires not-enums
s32 getFSStickDirection();
s32 getFSStickDirectionTrig();

void stopFSStickNav();
void hidePointer();

void setCursorStickInvisible();
void setCursorStickVisible();

// Not sure what this does. Related to Deposit
// and Seeker Stone scrolling
void scrollRelated();

// detail:
void checkForNavRightGesture();
void checkForNavLeftGesture();

} // namespace dPadNav

#endif
