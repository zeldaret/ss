#include "d/lyt/meter/d_lyt_meter_minus_btn.h"

STATE_DEFINE(dLytMeterMinusBtnMenuIcon_c, Wait);
STATE_DEFINE(dLytMeterMinusBtnMenuIcon_c, On);
STATE_DEFINE(dLytMeterMinusBtnMenuIcon_c, Select);
STATE_DEFINE(dLytMeterMinusBtnMenuIcon_c, Off);

STATE_DEFINE(dLytMeterMinusBtnBlink_c, Wait);
STATE_DEFINE(dLytMeterMinusBtnBlink_c, On);
STATE_DEFINE(dLytMeterMinusBtnBlink_c, Select);
STATE_DEFINE(dLytMeterMinusBtnBlink_c, Off);

STATE_DEFINE(dLytMeterMinusBtnText_c, Wait);
STATE_DEFINE(dLytMeterMinusBtnText_c, On);
STATE_DEFINE(dLytMeterMinusBtnText_c, Select);
STATE_DEFINE(dLytMeterMinusBtnText_c, Off);

STATE_DEFINE(dLytMeterMinusBtn_c, Wait);
STATE_DEFINE(dLytMeterMinusBtn_c, MenuSelectingIn);
STATE_DEFINE(dLytMeterMinusBtn_c, MenuSelecting);
STATE_DEFINE(dLytMeterMinusBtn_c, MenuSelectingOut);
STATE_DEFINE(dLytMeterMinusBtn_c, ToUse);
STATE_DEFINE(dLytMeterMinusBtn_c, ToUnuse);
STATE_DEFINE(dLytMeterMinusBtn_c, Unuse);
STATE_DEFINE(dLytMeterMinusBtn_c, DemoMove);
STATE_DEFINE(dLytMeterMinusBtn_c, DemoOut);
