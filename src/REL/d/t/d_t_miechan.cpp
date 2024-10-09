#include <d/t/d_t_miechan.h>

SPECIAL_ACTOR_PROFILE(TAG_MIECHAN, dTgMiechan_c, fProfile::TAG_MIECHAN, 0x2A, 0, 0);

STATE_DEFINE(dTgMiechan_c, Search);
STATE_DEFINE(dTgMiechan_c, Wait1);
STATE_DEFINE(dTgMiechan_c, Wait2);

void dTgMiechan_c::initializeState_Search() {}
void dTgMiechan_c::executeState_Search() {}
void dTgMiechan_c::finalizeState_Search() {}
void dTgMiechan_c::initializeState_Wait1() {}
void dTgMiechan_c::executeState_Wait1() {}
void dTgMiechan_c::finalizeState_Wait1() {}
void dTgMiechan_c::initializeState_Wait2() {}
void dTgMiechan_c::executeState_Wait2() {}
void dTgMiechan_c::finalizeState_Wait2() {}
