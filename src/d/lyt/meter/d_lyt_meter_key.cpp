#include "d/lyt/meter/d_lyt_meter_key.h"

STATE_DEFINE(dLytMeterKakeraKey_c, Wait);
STATE_DEFINE(dLytMeterKakeraKey_c, On);
STATE_DEFINE(dLytMeterKakeraKey_c, Increase);
STATE_DEFINE(dLytMeterKakeraKey_c, Active);
STATE_DEFINE(dLytMeterKakeraKey_c, Off);

STATE_DEFINE(dLytMeterBossKey_c, Wait);
STATE_DEFINE(dLytMeterBossKey_c, On);
STATE_DEFINE(dLytMeterBossKey_c, Active);
STATE_DEFINE(dLytMeterBossKey_c, Off);

STATE_DEFINE(dLytMeterSmallKey_c, Wait);
STATE_DEFINE(dLytMeterSmallKey_c, On);
STATE_DEFINE(dLytMeterSmallKey_c, Active);
STATE_DEFINE(dLytMeterSmallKey_c, Off);

void dLytMeterKakeraKey_c::initializeState_Wait() {}
void dLytMeterKakeraKey_c::executeState_Wait() {}
void dLytMeterKakeraKey_c::finalizeState_Wait() {}

void dLytMeterKakeraKey_c::initializeState_On() {}
void dLytMeterKakeraKey_c::executeState_On() {}
void dLytMeterKakeraKey_c::finalizeState_On() {}

void dLytMeterKakeraKey_c::initializeState_Increase() {}
void dLytMeterKakeraKey_c::executeState_Increase() {}
void dLytMeterKakeraKey_c::finalizeState_Increase() {}

void dLytMeterKakeraKey_c::initializeState_Active() {}
void dLytMeterKakeraKey_c::executeState_Active() {}
void dLytMeterKakeraKey_c::finalizeState_Active() {}

void dLytMeterKakeraKey_c::initializeState_Off() {}
void dLytMeterKakeraKey_c::executeState_Off() {}
void dLytMeterKakeraKey_c::finalizeState_Off() {}

void dLytMeterBossKey_c::initializeState_Wait() {}
void dLytMeterBossKey_c::executeState_Wait() {}
void dLytMeterBossKey_c::finalizeState_Wait() {}

void dLytMeterBossKey_c::initializeState_On() {}
void dLytMeterBossKey_c::executeState_On() {}
void dLytMeterBossKey_c::finalizeState_On() {}

void dLytMeterBossKey_c::initializeState_Active() {}
void dLytMeterBossKey_c::executeState_Active() {}
void dLytMeterBossKey_c::finalizeState_Active() {}

void dLytMeterBossKey_c::initializeState_Off() {}
void dLytMeterBossKey_c::executeState_Off() {}
void dLytMeterBossKey_c::finalizeState_Off() {}

void dLytMeterSmallKey_c::initializeState_Wait() {}
void dLytMeterSmallKey_c::executeState_Wait() {}
void dLytMeterSmallKey_c::finalizeState_Wait() {}

void dLytMeterSmallKey_c::initializeState_On() {}
void dLytMeterSmallKey_c::executeState_On() {}
void dLytMeterSmallKey_c::finalizeState_On() {}

void dLytMeterSmallKey_c::initializeState_Active() {}
void dLytMeterSmallKey_c::executeState_Active() {}
void dLytMeterSmallKey_c::finalizeState_Active() {}

void dLytMeterSmallKey_c::initializeState_Off() {}
void dLytMeterSmallKey_c::executeState_Off() {}
void dLytMeterSmallKey_c::finalizeState_Off() {}
