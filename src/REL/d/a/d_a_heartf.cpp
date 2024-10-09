#include <d/a/d_a_heartf.h>

SPECIAL_ACTOR_PROFILE(HEART_FLOWER, dAcHeartf_c, fProfile::HEART_FLOWER, 0X127, 0, 7);

STATE_DEFINE(dAcHeartf_c, Wait);
STATE_DEFINE(dAcHeartf_c, Wind);
STATE_DEFINE(dAcHeartf_c, Break);
STATE_DEFINE(dAcHeartf_c, Sync);
STATE_DEFINE(dAcHeartf_c, Water);
STATE_DEFINE(dAcHeartf_c, Spore);

void dAcHeartf_c::initializeState_Wait() {}
void dAcHeartf_c::executeState_Wait() {}
void dAcHeartf_c::finalizeState_Wait() {}
void dAcHeartf_c::initializeState_Wind() {}
void dAcHeartf_c::executeState_Wind() {}
void dAcHeartf_c::finalizeState_Wind() {}
void dAcHeartf_c::initializeState_Break() {}
void dAcHeartf_c::executeState_Break() {}
void dAcHeartf_c::finalizeState_Break() {}
void dAcHeartf_c::initializeState_Sync() {}
void dAcHeartf_c::executeState_Sync() {}
void dAcHeartf_c::finalizeState_Sync() {}
void dAcHeartf_c::initializeState_Water() {}
void dAcHeartf_c::executeState_Water() {}
void dAcHeartf_c::finalizeState_Water() {}
void dAcHeartf_c::initializeState_Spore() {}
void dAcHeartf_c::executeState_Spore() {}
void dAcHeartf_c::finalizeState_Spore() {}
