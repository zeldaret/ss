#include "d/d_state.h"

#include "d/d_main.h"
#include "m/m3d/m_state.h"

void dState::fn_80062E40() {
    m3d::mState::fn_802EE0A0();
}

void dState::fn_80062E50() {
    m3d::mState::fn_802EE0B0();

    if (m3d::mState::fn_802EE0D0()) {
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
    m3d::mState::fn_802EE0C0();
}

bool dState::fn_80062EC0() {
    return m3d::mState::fn_802EE0D0();
}
