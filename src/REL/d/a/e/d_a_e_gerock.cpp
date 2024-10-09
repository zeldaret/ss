#include <d/a/e/d_a_e_gerock.h>

SPECIAL_ACTOR_PROFILE(E_GEROCK, dAcEgerock_c, fProfile::E_GEROCK, 0X108, 0, 4099);

STATE_DEFINE(dAcEgerock_c, Attack);
STATE_DEFINE(dAcEgerock_c, Damage);
STATE_DEFINE(dAcEgerock_c, FlyHome);
STATE_DEFINE(dAcEgerock_c, Rail);
STATE_DEFINE(dAcEgerock_c, Return);
STATE_DEFINE(dAcEgerock_c, Shock);
STATE_DEFINE(dAcEgerock_c, Stay);
STATE_DEFINE(dAcEgerock_c, Vanish);

void dAcEgerock_c::initializeState_Attack() {}
void dAcEgerock_c::executeState_Attack() {}
void dAcEgerock_c::finalizeState_Attack() {}
void dAcEgerock_c::initializeState_Damage() {}
void dAcEgerock_c::executeState_Damage() {}
void dAcEgerock_c::finalizeState_Damage() {}
void dAcEgerock_c::initializeState_FlyHome() {}
void dAcEgerock_c::executeState_FlyHome() {}
void dAcEgerock_c::finalizeState_FlyHome() {}
void dAcEgerock_c::initializeState_Rail() {}
void dAcEgerock_c::executeState_Rail() {}
void dAcEgerock_c::finalizeState_Rail() {}
void dAcEgerock_c::initializeState_Return() {}
void dAcEgerock_c::executeState_Return() {}
void dAcEgerock_c::finalizeState_Return() {}
void dAcEgerock_c::initializeState_Shock() {}
void dAcEgerock_c::executeState_Shock() {}
void dAcEgerock_c::finalizeState_Shock() {}
void dAcEgerock_c::initializeState_Stay() {}
void dAcEgerock_c::executeState_Stay() {}
void dAcEgerock_c::finalizeState_Stay() {}
void dAcEgerock_c::initializeState_Vanish() {}
void dAcEgerock_c::executeState_Vanish() {}
void dAcEgerock_c::finalizeState_Vanish() {}
