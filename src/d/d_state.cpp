#include "d/d_main.h"
#include "d/d_state.h"
#include "m/m3d/m_state.h"

void dState::fn_80062E40() {
    m3d::mState::fn_802EE0A0();
}

void dState::fn_80062E50() {
    m3d::mState::fn_802EE0B0();

    if (m3d::mState::fn_802EE0D0()) {
        if (dMain::fn_80054F90(1)) {
            dMain::fn_80054FE0(1);
        }
    }
    else {
        if (!dMain::fn_80054F90(1)) {
            dMain::fn_80054FB0(1);
        }
    }
}

void dState::fn_80062EB0() {
    m3d::mState::fn_802EE0C0();
}

bool dState::fn_80062EC0() {
    return m3d::mState::fn_802EE0D0();
}
