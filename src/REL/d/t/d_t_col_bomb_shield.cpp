#include <d/t/d_t_col_bomb_shield.h>

SPECIAL_ACTOR_PROFILE(COL_BOMSLD, dTgColBombShield, fProfile::COL_BOMSLD, 0X12, 0, 0);

STATE_DEFINE(dTgColBombShield, Wait);

void dTgColBombShield::initializeState_Wait() {}
void dTgColBombShield::executeState_Wait() {}
void dTgColBombShield::finalizeState_Wait() {}
