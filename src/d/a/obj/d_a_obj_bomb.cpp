#include "d/a/obj/d_a_obj_bomb.h"

#include "f/f_base.h"
#include "f/f_profile.h"
#include "f/f_profile_name.h"
#include "s/s_State.hpp"

SPECIAL_ACTOR_PROFILE(BOMB, dAcBomb_c, fProfile::BOMB, 0x128, 0, 2);

STATE_DEFINE(dAcBomb_c, Wait);
STATE_DEFINE(dAcBomb_c, FlowerWait);
STATE_DEFINE(dAcBomb_c, Explode);
STATE_DEFINE(dAcBomb_c, Carry);
STATE_DEFINE(dAcBomb_c, WindCarry);

bool dAcBomb_c::createHeap() {}

int dAcBomb_c::create() {
    return SUCCEEDED;
}
