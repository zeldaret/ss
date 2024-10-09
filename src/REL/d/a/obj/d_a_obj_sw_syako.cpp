#include <d/a/obj/d_a_obj_sw_syako.h>

SPECIAL_ACTOR_PROFILE(OBJ_SW_SYAKO, dAcOSwSyako_c, fProfile::OBJ_SW_SYAKO, 0X177, 0, 4099);

STATE_DEFINE(dAcOSwSyako_c, Wait);
STATE_DEFINE(dAcOSwSyako_c, Event);

void dAcOSwSyako_c::initializeState_Wait() {}
void dAcOSwSyako_c::executeState_Wait() {}
void dAcOSwSyako_c::finalizeState_Wait() {}
void dAcOSwSyako_c::initializeState_Event() {}
void dAcOSwSyako_c::executeState_Event() {}
void dAcOSwSyako_c::finalizeState_Event() {}
