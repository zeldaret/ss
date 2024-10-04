#include <d/lyt/meter/d_lyt_meter_remocon_bg.h>
#include <toBeSorted/event_manager.h>

STATE_DEFINE(dLytMeterRemoconBg_c, Wait);
STATE_DEFINE(dLytMeterRemoconBg_c, On);
STATE_DEFINE(dLytMeterRemoconBg_c, Active);
STATE_DEFINE(dLytMeterRemoconBg_c, Off);

void dLytMeterRemoconBg_c::initializeState_Wait() {}
void dLytMeterRemoconBg_c::executeState_Wait() {}
void dLytMeterRemoconBg_c::finalizeState_Wait() {}

void dLytMeterRemoconBg_c::initializeState_On() {}
void dLytMeterRemoconBg_c::executeState_On() {}
void dLytMeterRemoconBg_c::finalizeState_On() {}

void dLytMeterRemoconBg_c::initializeState_Active() {}
void dLytMeterRemoconBg_c::executeState_Active() {}
void dLytMeterRemoconBg_c::finalizeState_Active() {}

void dLytMeterRemoconBg_c::initializeState_Off() {}
void dLytMeterRemoconBg_c::executeState_Off() {}
void dLytMeterRemoconBg_c::finalizeState_Off() {}

static const char *n1 = "N_remoConBg_00";

bool dLytMeterRemoconBg_c::build(d2d::ResAccIf_c *resAcc) {
    mLyt.setResAcc(resAcc);
    mLyt.build("remoConBg_00.brlyt", nullptr);
    mpPane = mLyt.findPane(n1);
    field_0xE8 = 0x61;
    field_0xDC = 0x61;
    field_0xE0 = 0x61;
    field_0xEC = false;
    field_0xE4 = 1;
    mStateMgr.changeState(StateID_Wait);
    return true;
}

bool dLytMeterRemoconBg_c::LytMeter0x10() {
    return true;
}

bool dLytMeterRemoconBg_c::LytMeter0x14() {}

const char *dLytMeterRemoconBg_c::LytMeter0x20() const {
    return mLyt.getName();
}

void *dLytMeterRemoconBg_c::LytMeter0x1C() {
    return &mLyt;
}

nw4r::lyt::Pane *dLytMeterRemoconBg_c::LytMeter0x18() {
    return mLyt.getLayout()->GetRootPane();
}
