#include "d/d_state.h"

#include "d/d_main.h"
#include "m/m3d/m_state.h"

void dState::fn_80062E40() {
    m3d::mState::unkStub1();
}

void dState::fn_80062E50() {
    m3d::mState::unkStub2();

    if (m3d::mState::unkAlwaysTrue()) {
        if (dMain::isStopFlagSet(1)) {
            dMain::clearStopFlag(1);
        }
    } else {
        if (!dMain::isStopFlagSet(1)) {
            dMain::setStopFlag(1);
        }
    }
}

void dState::fn_80062EB0() {
    m3d::mState::unkStub3();
}

bool dState::fn_80062EC0() {
    return m3d::mState::unkAlwaysTrue();
}
