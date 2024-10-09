#include <d/a/d_a_avater_race_mng.h>

SPECIAL_ACTOR_PROFILE(AVATER_RACE_MNG, dAvater_Race_Mng_c, fProfile::AVATER_RACE_MNG, 0XAF, 0, 2);

STATE_DEFINE(dAvater_Race_Mng_c, StartWait);
STATE_DEFINE(dAvater_Race_Mng_c, Race);
STATE_DEFINE(dAvater_Race_Mng_c, Hide);
STATE_DEFINE(dAvater_Race_Mng_c, Finish);
STATE_DEFINE(dAvater_Race_Mng_c, LevelUpDemo);
STATE_DEFINE(dAvater_Race_Mng_c, Message);

void dAvater_Race_Mng_c::initializeState_StartWait() {}
void dAvater_Race_Mng_c::executeState_StartWait() {}
void dAvater_Race_Mng_c::finalizeState_StartWait() {}
void dAvater_Race_Mng_c::initializeState_Race() {}
void dAvater_Race_Mng_c::executeState_Race() {}
void dAvater_Race_Mng_c::finalizeState_Race() {}
void dAvater_Race_Mng_c::initializeState_Hide() {}
void dAvater_Race_Mng_c::executeState_Hide() {}
void dAvater_Race_Mng_c::finalizeState_Hide() {}
void dAvater_Race_Mng_c::initializeState_Finish() {}
void dAvater_Race_Mng_c::executeState_Finish() {}
void dAvater_Race_Mng_c::finalizeState_Finish() {}
void dAvater_Race_Mng_c::initializeState_LevelUpDemo() {}
void dAvater_Race_Mng_c::executeState_LevelUpDemo() {}
void dAvater_Race_Mng_c::finalizeState_LevelUpDemo() {}
void dAvater_Race_Mng_c::initializeState_Message() {}
void dAvater_Race_Mng_c::executeState_Message() {}
void dAvater_Race_Mng_c::finalizeState_Message() {}
