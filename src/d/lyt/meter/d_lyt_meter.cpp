#include "d/lyt/meter/d_lyt_meter.h"
#include "d/lyt/d_lyt_meter_configuration.h"

static dLytMeterConfiguration_c sConf;

STATE_DEFINE(dLytMeter1Button_c, Wait);
STATE_DEFINE(dLytMeter1Button_c, ToUse);
STATE_DEFINE(dLytMeter1Button_c, ToUnuse);
STATE_DEFINE(dLytMeter1Button_c, Unuse);

STATE_DEFINE(dLytMeter2Button_c, Wait);
STATE_DEFINE(dLytMeter2Button_c, ToUse);
STATE_DEFINE(dLytMeter2Button_c, ToUnuse);
STATE_DEFINE(dLytMeter2Button_c, Unuse);

STATE_DEFINE(dLytMeterParts_c, Invisible);
STATE_DEFINE(dLytMeterParts_c, In);
STATE_DEFINE(dLytMeterParts_c, Visible);
STATE_DEFINE(dLytMeterParts_c, Out);

void floats() {
    32.0f;
    1.0f;
    0.0f;
    int x = 0;
    f32 f = x;
}

void floats2() {
    5.0f;
    3.0f;
}

#pragma dont_inline on
dLytMeter_c::dLytMeter_c() {}
#pragma dont_inline off

dLytMeterContainer_c::dLytMeterContainer_c() {
    sInstance = this;
}
