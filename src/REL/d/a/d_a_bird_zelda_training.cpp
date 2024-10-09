#include <d/a/d_a_bird_zelda_training.h>

SPECIAL_ACTOR_PROFILE(BIRD_ZELDA_TRAINING, dBird_ZeldaTraining_c, fProfile::BIRD_ZELDA_TRAINING, 0xAE, 0, 2);

STATE_DEFINE(dBird_ZeldaTraining_c, Wait);
STATE_DEFINE(dBird_ZeldaTraining_c, Chase);
STATE_DEFINE(dBird_ZeldaTraining_c, Finish);

void dBird_ZeldaTraining_c::initializeState_Wait() {}
void dBird_ZeldaTraining_c::executeState_Wait() {}
void dBird_ZeldaTraining_c::finalizeState_Wait() {}
void dBird_ZeldaTraining_c::initializeState_Chase() {}
void dBird_ZeldaTraining_c::executeState_Chase() {}
void dBird_ZeldaTraining_c::finalizeState_Chase() {}
void dBird_ZeldaTraining_c::initializeState_Finish() {}
void dBird_ZeldaTraining_c::executeState_Finish() {}
void dBird_ZeldaTraining_c::finalizeState_Finish() {}
