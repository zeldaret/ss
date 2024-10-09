#include <d/a/obj/d_a_obj_utajima_main_mecha.h>

SPECIAL_ACTOR_PROFILE(OBJ_UTAJIMA_MAIN_MECHA, dAcOutajimaMainMecha_c, fProfile::OBJ_UTAJIMA_MAIN_MECHA, 0x1D0, 0, 2);

STATE_DEFINE(dAcOutajimaMainMecha_c, Before);
STATE_DEFINE(dAcOutajimaMainMecha_c, Appear);
STATE_DEFINE(dAcOutajimaMainMecha_c, Main);
STATE_DEFINE(dAcOutajimaMainMecha_c, Roll);
STATE_DEFINE(dAcOutajimaMainMecha_c, Disappear);
STATE_DEFINE(dAcOutajimaMainMecha_c, After);
STATE_DEFINE(dAcOutajimaMainMecha_c, StopperReset);
STATE_DEFINE(dAcOutajimaMainMecha_c, Reset);

void dAcOutajimaMainMecha_c::initializeState_Before() {}
void dAcOutajimaMainMecha_c::executeState_Before() {}
void dAcOutajimaMainMecha_c::finalizeState_Before() {}
void dAcOutajimaMainMecha_c::initializeState_Appear() {}
void dAcOutajimaMainMecha_c::executeState_Appear() {}
void dAcOutajimaMainMecha_c::finalizeState_Appear() {}
void dAcOutajimaMainMecha_c::initializeState_Main() {}
void dAcOutajimaMainMecha_c::executeState_Main() {}
void dAcOutajimaMainMecha_c::finalizeState_Main() {}
void dAcOutajimaMainMecha_c::initializeState_Roll() {}
void dAcOutajimaMainMecha_c::executeState_Roll() {}
void dAcOutajimaMainMecha_c::finalizeState_Roll() {}
void dAcOutajimaMainMecha_c::initializeState_Disappear() {}
void dAcOutajimaMainMecha_c::executeState_Disappear() {}
void dAcOutajimaMainMecha_c::finalizeState_Disappear() {}
void dAcOutajimaMainMecha_c::initializeState_After() {}
void dAcOutajimaMainMecha_c::executeState_After() {}
void dAcOutajimaMainMecha_c::finalizeState_After() {}
void dAcOutajimaMainMecha_c::initializeState_StopperReset() {}
void dAcOutajimaMainMecha_c::executeState_StopperReset() {}
void dAcOutajimaMainMecha_c::finalizeState_StopperReset() {}
void dAcOutajimaMainMecha_c::initializeState_Reset() {}
void dAcOutajimaMainMecha_c::executeState_Reset() {}
void dAcOutajimaMainMecha_c::finalizeState_Reset() {}
