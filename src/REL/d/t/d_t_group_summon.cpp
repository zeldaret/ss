#include <d/t/d_t_group_summon.h>

SPECIAL_ACTOR_PROFILE(GROUP_SUMMON, dTgGroupSummon_c, fProfile::GROUP_SUMMON, 0xC5, 0, 0);

STATE_DEFINE(dTgGroupSummon_c, Wait);
STATE_DEFINE(dTgGroupSummon_c, PreSummon);
STATE_DEFINE(dTgGroupSummon_c, Summoning);
STATE_DEFINE(dTgGroupSummon_c, Watch);
STATE_DEFINE(dTgGroupSummon_c, Demo);

void dTgGroupSummon_c::initializeState_Wait() {}
void dTgGroupSummon_c::executeState_Wait() {}
void dTgGroupSummon_c::finalizeState_Wait() {}
void dTgGroupSummon_c::initializeState_PreSummon() {}
void dTgGroupSummon_c::executeState_PreSummon() {}
void dTgGroupSummon_c::finalizeState_PreSummon() {}
void dTgGroupSummon_c::initializeState_Summoning() {}
void dTgGroupSummon_c::executeState_Summoning() {}
void dTgGroupSummon_c::finalizeState_Summoning() {}
void dTgGroupSummon_c::initializeState_Watch() {}
void dTgGroupSummon_c::executeState_Watch() {}
void dTgGroupSummon_c::finalizeState_Watch() {}
void dTgGroupSummon_c::initializeState_Demo() {}
void dTgGroupSummon_c::executeState_Demo() {}
void dTgGroupSummon_c::finalizeState_Demo() {}
