#include <d/lyt/meter/d_lyt_meter_ganbari_gauge.h>

STATE_DEFINE(dLytMeterGanbariGauge_c, InvisibleWait);
STATE_DEFINE(dLytMeterGanbariGauge_c, In);
STATE_DEFINE(dLytMeterGanbariGauge_c, Wait);
STATE_DEFINE(dLytMeterGanbariGauge_c, OutWait);
STATE_DEFINE(dLytMeterGanbariGauge_c, Out);
STATE_DEFINE(dLytMeterGanbariGauge_c, CameraOut);
STATE_DEFINE(dLytMeterGanbariGauge_c, Full);
STATE_DEFINE(dLytMeterGanbariGauge_c, FullGutsUse);
STATE_DEFINE(dLytMeterGanbariGauge_c, Normal);
STATE_DEFINE(dLytMeterGanbariGauge_c, Caution);
STATE_DEFINE(dLytMeterGanbariGauge_c, ToMin);
STATE_DEFINE(dLytMeterGanbariGauge_c, Recovery);
STATE_DEFINE(dLytMeterGanbariGauge_c, ToMax);

void dLytMeterGanbariGauge_c::initializeState_InvisibleWait() {}
void dLytMeterGanbariGauge_c::executeState_InvisibleWait() {}
void dLytMeterGanbariGauge_c::finalizeState_InvisibleWait() {}

void dLytMeterGanbariGauge_c::initializeState_In() {}
void dLytMeterGanbariGauge_c::executeState_In() {}
void dLytMeterGanbariGauge_c::finalizeState_In() {}

void dLytMeterGanbariGauge_c::initializeState_Wait() {}
void dLytMeterGanbariGauge_c::executeState_Wait() {}
void dLytMeterGanbariGauge_c::finalizeState_Wait() {}

void dLytMeterGanbariGauge_c::initializeState_OutWait() {}
void dLytMeterGanbariGauge_c::executeState_OutWait() {}
void dLytMeterGanbariGauge_c::finalizeState_OutWait() {}

void dLytMeterGanbariGauge_c::initializeState_Out() {}
void dLytMeterGanbariGauge_c::executeState_Out() {}
void dLytMeterGanbariGauge_c::finalizeState_Out() {}

void dLytMeterGanbariGauge_c::initializeState_CameraOut() {}
void dLytMeterGanbariGauge_c::executeState_CameraOut() {}
void dLytMeterGanbariGauge_c::finalizeState_CameraOut() {}

void dLytMeterGanbariGauge_c::initializeState_Full() {}
void dLytMeterGanbariGauge_c::executeState_Full() {}
void dLytMeterGanbariGauge_c::finalizeState_Full() {}

void dLytMeterGanbariGauge_c::initializeState_FullGutsUse() {}
void dLytMeterGanbariGauge_c::executeState_FullGutsUse() {}
void dLytMeterGanbariGauge_c::finalizeState_FullGutsUse() {}

void dLytMeterGanbariGauge_c::initializeState_Normal() {}
void dLytMeterGanbariGauge_c::executeState_Normal() {}
void dLytMeterGanbariGauge_c::finalizeState_Normal() {}

void dLytMeterGanbariGauge_c::initializeState_Caution() {}
void dLytMeterGanbariGauge_c::executeState_Caution() {}
void dLytMeterGanbariGauge_c::finalizeState_Caution() {}

void dLytMeterGanbariGauge_c::initializeState_ToMin() {}
void dLytMeterGanbariGauge_c::executeState_ToMin() {}
void dLytMeterGanbariGauge_c::finalizeState_ToMin() {}

void dLytMeterGanbariGauge_c::initializeState_Recovery() {}
void dLytMeterGanbariGauge_c::executeState_Recovery() {}
void dLytMeterGanbariGauge_c::finalizeState_Recovery() {}

void dLytMeterGanbariGauge_c::initializeState_ToMax() {}
void dLytMeterGanbariGauge_c::executeState_ToMax() {}
void dLytMeterGanbariGauge_c::finalizeState_ToMax() {}

bool dLytMeterGanbariGauge_c::build(d2d::ResAccIf_c *resAcc) {
    // TODO
    return true;
}

bool dLytMeterGanbariGauge_c::remove() {
    return true;
}

bool dLytMeterGanbariGauge_c::LytMeter0x14() {
    // TODO
    return true;
}

const char *dLytMeterGanbariGauge_c::getName() const {
    return mLyt.getName();
}

d2d::LytBase_c *dLytMeterGanbariGauge_c::getLyt() {
    return &mLyt;
}

nw4r::lyt::Pane *dLytMeterGanbariGauge_c::getPane() {
    return mLyt.getLayout()->GetRootPane();
}
