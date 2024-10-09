#include <d/a/obj/d_a_obj_bblargebomb.h>

SPECIAL_ACTOR_PROFILE(OBJ_BBLARGEBOMB, dAcOBblargebomb_c, fProfile::OBJ_BBLARGEBOMB, 0XA7, 0, 3);

STATE_DEFINE(dAcOBblargebomb_c, Wait);
STATE_DEFINE(dAcOBblargebomb_c, Explode);
STATE_DEFINE(dAcOBblargebomb_c, Carry);
STATE_DEFINE(dAcOBblargebomb_c, WindCarry);
STATE_DEFINE(dAcOBblargebomb_c, Birth);
STATE_DEFINE(dAcOBblargebomb_c, Controled);

void dAcOBblargebomb_c::initializeState_Wait() {}
void dAcOBblargebomb_c::executeState_Wait() {}
void dAcOBblargebomb_c::finalizeState_Wait() {}
void dAcOBblargebomb_c::initializeState_Explode() {}
void dAcOBblargebomb_c::executeState_Explode() {}
void dAcOBblargebomb_c::finalizeState_Explode() {}
void dAcOBblargebomb_c::initializeState_Carry() {}
void dAcOBblargebomb_c::executeState_Carry() {}
void dAcOBblargebomb_c::finalizeState_Carry() {}
void dAcOBblargebomb_c::initializeState_WindCarry() {}
void dAcOBblargebomb_c::executeState_WindCarry() {}
void dAcOBblargebomb_c::finalizeState_WindCarry() {}
void dAcOBblargebomb_c::initializeState_Birth() {}
void dAcOBblargebomb_c::executeState_Birth() {}
void dAcOBblargebomb_c::finalizeState_Birth() {}
void dAcOBblargebomb_c::initializeState_Controled() {}
void dAcOBblargebomb_c::executeState_Controled() {}
void dAcOBblargebomb_c::finalizeState_Controled() {}
