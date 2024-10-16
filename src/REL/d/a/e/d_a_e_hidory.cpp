#include "d/a/e/d_a_e_hidory.h"

SPECIAL_ACTOR_PROFILE(E_HIDORY, dAcEHidory_c, fProfile::E_HIDORY, 0x10A, 0, 3);

STATE_DEFINE(dAcEHidory_c, Wait);
STATE_DEFINE(dAcEHidory_c, Hovering);
STATE_DEFINE(dAcEHidory_c, Attack);
STATE_DEFINE(dAcEHidory_c, Whip_Damage);
STATE_DEFINE(dAcEHidory_c, Damage);
STATE_DEFINE(dAcEHidory_c, Death);

void dAcEHidory_c::initializeState_Wait() {}
void dAcEHidory_c::executeState_Wait() {}
void dAcEHidory_c::finalizeState_Wait() {}
void dAcEHidory_c::initializeState_Hovering() {}
void dAcEHidory_c::executeState_Hovering() {}
void dAcEHidory_c::finalizeState_Hovering() {}
void dAcEHidory_c::initializeState_Attack() {}
void dAcEHidory_c::executeState_Attack() {}
void dAcEHidory_c::finalizeState_Attack() {}
void dAcEHidory_c::initializeState_Whip_Damage() {}
void dAcEHidory_c::executeState_Whip_Damage() {}
void dAcEHidory_c::finalizeState_Whip_Damage() {}
void dAcEHidory_c::initializeState_Damage() {}
void dAcEHidory_c::executeState_Damage() {}
void dAcEHidory_c::finalizeState_Damage() {}
void dAcEHidory_c::initializeState_Death() {}
void dAcEHidory_c::executeState_Death() {}
void dAcEHidory_c::finalizeState_Death() {}
