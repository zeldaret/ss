#include "d/lyt/meter/d_lyt_meter_drink.h"

STATE_DEFINE(dLytMeterDrinkParts_c, Invisible);
STATE_DEFINE(dLytMeterDrinkParts_c, In);
STATE_DEFINE(dLytMeterDrinkParts_c, Visible);
STATE_DEFINE(dLytMeterDrinkParts_c, Out);

STATE_DEFINE(dLytMeterDrink_c, Invisible);
STATE_DEFINE(dLytMeterDrink_c, In);
STATE_DEFINE(dLytMeterDrink_c, Wait);
STATE_DEFINE(dLytMeterDrink_c, Move);
STATE_DEFINE(dLytMeterDrink_c, Out);
