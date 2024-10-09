#include <d/a/npc/d_a_npc_salbage_robot_fly.h>

SPECIAL_ACTOR_PROFILE(NPC_SLFL, dAcNpcSalbageRobotFly_c, fProfile::NPC_SLFL, 0X8A, 0, 4);

STATE_DEFINE(dAcNpcSalbageRobotFly_c, ChasePlayerBird);
STATE_DEFINE(dAcNpcSalbageRobotFly_c, FlyWithPlayerBird);
STATE_DEFINE(dAcNpcSalbageRobotFly_c, Leave);
STATE_DEFINE(dAcNpcSalbageRobotFly_c, EscapeToUpward);

void dAcNpcSalbageRobotFly_c::initializeState_ChasePlayerBird() {}
void dAcNpcSalbageRobotFly_c::executeState_ChasePlayerBird() {}
void dAcNpcSalbageRobotFly_c::finalizeState_ChasePlayerBird() {}
void dAcNpcSalbageRobotFly_c::initializeState_FlyWithPlayerBird() {}
void dAcNpcSalbageRobotFly_c::executeState_FlyWithPlayerBird() {}
void dAcNpcSalbageRobotFly_c::finalizeState_FlyWithPlayerBird() {}
void dAcNpcSalbageRobotFly_c::initializeState_Leave() {}
void dAcNpcSalbageRobotFly_c::executeState_Leave() {}
void dAcNpcSalbageRobotFly_c::finalizeState_Leave() {}
void dAcNpcSalbageRobotFly_c::initializeState_EscapeToUpward() {}
void dAcNpcSalbageRobotFly_c::executeState_EscapeToUpward() {}
void dAcNpcSalbageRobotFly_c::finalizeState_EscapeToUpward() {}
