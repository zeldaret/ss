#include "d/lyt/meter/d_lyt_meter_dowsing.h"

STATE_DEFINE(dLytMeterDowsingMenuIcon_c, Wait);
STATE_DEFINE(dLytMeterDowsingMenuIcon_c, On);
STATE_DEFINE(dLytMeterDowsingMenuIcon_c, Select);
STATE_DEFINE(dLytMeterDowsingMenuIcon_c, Off);

STATE_DEFINE(dLytMeterDowsingBlink_c, Wait);
STATE_DEFINE(dLytMeterDowsingBlink_c, On);
STATE_DEFINE(dLytMeterDowsingBlink_c, Select);
STATE_DEFINE(dLytMeterDowsingBlink_c, Off);

STATE_DEFINE(dLytMeterDowsingInput_c, Wait);
STATE_DEFINE(dLytMeterDowsingInput_c, On);
STATE_DEFINE(dLytMeterDowsingInput_c, Select);
STATE_DEFINE(dLytMeterDowsingInput_c, Off);

STATE_DEFINE(dLytMeterDowsingText_c, Wait);
STATE_DEFINE(dLytMeterDowsingText_c, On);
STATE_DEFINE(dLytMeterDowsingText_c, Select);
STATE_DEFINE(dLytMeterDowsingText_c, Off);

STATE_DEFINE(dLytMeterDowsing_c, Invisible);
STATE_DEFINE(dLytMeterDowsing_c, Wait);
STATE_DEFINE(dLytMeterDowsing_c, MenuSelectingIn);
STATE_DEFINE(dLytMeterDowsing_c, MenuSelecting);
STATE_DEFINE(dLytMeterDowsing_c, MenuSelectingOut);
STATE_DEFINE(dLytMeterDowsing_c, Reset);
STATE_DEFINE(dLytMeterDowsing_c, ToUse);
STATE_DEFINE(dLytMeterDowsing_c, ToUnuse);
STATE_DEFINE(dLytMeterDowsing_c, Unuse);
STATE_DEFINE(dLytMeterDowsing_c, DemoMove);
STATE_DEFINE(dLytMeterDowsing_c, DemoOut);
