#include "d/lyt/meter/d_lyt_meter_shield_gauge.h"

STATE_DEFINE(dLytMeterShieldGauge_c, Normal);
STATE_DEFINE(dLytMeterShieldGauge_c, Use);
STATE_DEFINE(dLytMeterShieldGauge_c, ToMax);

void dLytMeterShieldGauge_c::initializeState_Normal() {}
void dLytMeterShieldGauge_c::executeState_Normal() {}
void dLytMeterShieldGauge_c::finalizeState_Normal() {}

void dLytMeterShieldGauge_c::initializeState_Use() {}
void dLytMeterShieldGauge_c::executeState_Use() {}
void dLytMeterShieldGauge_c::finalizeState_Use() {}

void dLytMeterShieldGauge_c::initializeState_ToMax() {}
void dLytMeterShieldGauge_c::executeState_ToMax() {}
void dLytMeterShieldGauge_c::finalizeState_ToMax() {}

bool dLytMeterShieldGauge_c::build(d2d::ResAccIf_c *resAcc) {
    // TODO
    return true;
}

bool dLytMeterShieldGauge_c::remove() {
    for (int i = 0; i < 8; i++) {
        mAnmGroups[i].unbind();
        mAnmGroups[i].afterUnbind();
    }
    return true;
}

bool dLytMeterShieldGauge_c::LytMeter0x14() {
    // TODO
    return true;
}

const char *dLytMeterShieldGauge_c::getName() const {
    return mLyt.getName();
}

d2d::LytBase_c *dLytMeterShieldGauge_c::getLyt() {
    return &mLyt;
}

nw4r::lyt::Pane *dLytMeterShieldGauge_c::getPane() {
    return mLyt.getLayout()->GetRootPane();
}
