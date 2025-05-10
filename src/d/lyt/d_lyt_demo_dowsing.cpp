#include "d/lyt/d_lyt_demo_dowsing.h"

#include "f/f_profile.h"
#include "s/s_State.hpp"

SPECIAL_BASE_PROFILE(LYT_DEMO_DOWSING, dLytDemoDowsing_c, fProfile::LYT_DEMO_DOWSING, 0x2AA, 0x6F9);

STATE_DEFINE(dLytDemoDowsing_c, Wait);
STATE_DEFINE(dLytDemoDowsing_c, Start);
