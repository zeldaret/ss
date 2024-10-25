#include "d/lyt/meter/d_lyt_meter_rupy.h"

STATE_DEFINE(dLytMeterRupyStart_c, Wait);
STATE_DEFINE(dLytMeterRupyStart_c, Start);

STATE_DEFINE(dLytMeterRupyBlink_c, Wait);
STATE_DEFINE(dLytMeterRupyBlink_c, Flash);

STATE_DEFINE(dLytMeterRupy_c, In);
STATE_DEFINE(dLytMeterRupy_c, Active);
STATE_DEFINE(dLytMeterRupy_c, Start);
STATE_DEFINE(dLytMeterRupy_c, Out);
