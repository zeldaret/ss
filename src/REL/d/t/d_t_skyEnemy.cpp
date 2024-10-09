#include <d/t/d_t_skyEnemy.h>

SPECIAL_ACTOR_PROFILE(SKYENEMY_T, dTgSkyEnemyTag, fProfile::SKYENEMY_T, 0x28D, 0, 0);

STATE_DEFINE(dTgSkyEnemyTag, Wait);
STATE_DEFINE(dTgSkyEnemyTag, BirthRupeeGue);
STATE_DEFINE(dTgSkyEnemyTag, BirthSkytail);
STATE_DEFINE(dTgSkyEnemyTag, BirthTornado);
STATE_DEFINE(dTgSkyEnemyTag, BirthBirdNpc);
STATE_DEFINE(dTgSkyEnemyTag, BirthFrontBirdNpc);
STATE_DEFINE(dTgSkyEnemyTag, BirthNusiNpc);
STATE_DEFINE(dTgSkyEnemyTag, BirthHighSpeed);

void dTgSkyEnemyTag::initializeState_Wait() {}
void dTgSkyEnemyTag::executeState_Wait() {}
void dTgSkyEnemyTag::finalizeState_Wait() {}
void dTgSkyEnemyTag::initializeState_BirthRupeeGue() {}
void dTgSkyEnemyTag::executeState_BirthRupeeGue() {}
void dTgSkyEnemyTag::finalizeState_BirthRupeeGue() {}
void dTgSkyEnemyTag::initializeState_BirthSkytail() {}
void dTgSkyEnemyTag::executeState_BirthSkytail() {}
void dTgSkyEnemyTag::finalizeState_BirthSkytail() {}
void dTgSkyEnemyTag::initializeState_BirthTornado() {}
void dTgSkyEnemyTag::executeState_BirthTornado() {}
void dTgSkyEnemyTag::finalizeState_BirthTornado() {}
void dTgSkyEnemyTag::initializeState_BirthBirdNpc() {}
void dTgSkyEnemyTag::executeState_BirthBirdNpc() {}
void dTgSkyEnemyTag::finalizeState_BirthBirdNpc() {}
void dTgSkyEnemyTag::initializeState_BirthFrontBirdNpc() {}
void dTgSkyEnemyTag::executeState_BirthFrontBirdNpc() {}
void dTgSkyEnemyTag::finalizeState_BirthFrontBirdNpc() {}
void dTgSkyEnemyTag::initializeState_BirthNusiNpc() {}
void dTgSkyEnemyTag::executeState_BirthNusiNpc() {}
void dTgSkyEnemyTag::finalizeState_BirthNusiNpc() {}
void dTgSkyEnemyTag::initializeState_BirthHighSpeed() {}
void dTgSkyEnemyTag::executeState_BirthHighSpeed() {}
void dTgSkyEnemyTag::finalizeState_BirthHighSpeed() {}
