#include <d/a/e/d_a_e_beamos.h>

SPECIAL_ACTOR_PROFILE(E_BEAMOS, dAcEbeamos, fProfile::E_BEAMOS, 0XEC, 0, 4098);

STATE_DEFINE(dAcEbeamos, Attack);
STATE_DEFINE(dAcEbeamos, Beetle);
STATE_DEFINE(dAcEbeamos, Blow);
STATE_DEFINE(dAcEbeamos, Damage);
STATE_DEFINE(dAcEbeamos, Dead);
STATE_DEFINE(dAcEbeamos, Rail);
STATE_DEFINE(dAcEbeamos, Shock);
STATE_DEFINE(dAcEbeamos, Stun);
STATE_DEFINE(dAcEbeamos, Wait);
STATE_DEFINE(dAcEbeamos, Watch);

void dAcEbeamos::initializeState_Attack() {}
void dAcEbeamos::executeState_Attack() {}
void dAcEbeamos::finalizeState_Attack() {}
void dAcEbeamos::initializeState_Beetle() {}
void dAcEbeamos::executeState_Beetle() {}
void dAcEbeamos::finalizeState_Beetle() {}
void dAcEbeamos::initializeState_Blow() {}
void dAcEbeamos::executeState_Blow() {}
void dAcEbeamos::finalizeState_Blow() {}
void dAcEbeamos::initializeState_Damage() {}
void dAcEbeamos::executeState_Damage() {}
void dAcEbeamos::finalizeState_Damage() {}
void dAcEbeamos::initializeState_Dead() {}
void dAcEbeamos::executeState_Dead() {}
void dAcEbeamos::finalizeState_Dead() {}
void dAcEbeamos::initializeState_Rail() {}
void dAcEbeamos::executeState_Rail() {}
void dAcEbeamos::finalizeState_Rail() {}
void dAcEbeamos::initializeState_Shock() {}
void dAcEbeamos::executeState_Shock() {}
void dAcEbeamos::finalizeState_Shock() {}
void dAcEbeamos::initializeState_Stun() {}
void dAcEbeamos::executeState_Stun() {}
void dAcEbeamos::finalizeState_Stun() {}
void dAcEbeamos::initializeState_Wait() {}
void dAcEbeamos::executeState_Wait() {}
void dAcEbeamos::finalizeState_Wait() {}
void dAcEbeamos::initializeState_Watch() {}
void dAcEbeamos::executeState_Watch() {}
void dAcEbeamos::finalizeState_Watch() {}
