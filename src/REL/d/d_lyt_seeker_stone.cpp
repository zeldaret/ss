#include <d/d_lyt_seeker_stone.h>

SPECIAL_BASE_PROFILE(LYT_SEEKER_STONE, dLytSeekerStone_c, fProfile::LYT_SEEKER_STONE, 0x2BC, 1024);

STATE_DEFINE(dLytSeekerStone_c, None);
STATE_DEFINE(dLytSeekerStone_c, In);
STATE_DEFINE(dLytSeekerStone_c, Move);
STATE_DEFINE(dLytSeekerStone_c, Decide);
STATE_DEFINE(dLytSeekerStone_c, SelectScroll);
STATE_DEFINE(dLytSeekerStone_c, Scroll);
STATE_DEFINE(dLytSeekerStone_c, FadeOut);
STATE_DEFINE(dLytSeekerStone_c, FadeIn);
STATE_DEFINE(dLytSeekerStone_c, Movie);
STATE_DEFINE(dLytSeekerStone_c, MovieFadeOut);
STATE_DEFINE(dLytSeekerStone_c, MovieFadeIn);
STATE_DEFINE(dLytSeekerStone_c, Out);

dLytSeekerStone_c::dLytSeekerStone_c() : mStateMgr(*this, sStateID::null) {}
dLytSeekerStone_c::~dLytSeekerStone_c() {}

void dLytSeekerStone_c::initializeState_None() {}
void dLytSeekerStone_c::executeState_None() {}
void dLytSeekerStone_c::finalizeState_None() {}
void dLytSeekerStone_c::initializeState_In() {}
void dLytSeekerStone_c::executeState_In() {}
void dLytSeekerStone_c::finalizeState_In() {}
void dLytSeekerStone_c::initializeState_Move() {}
void dLytSeekerStone_c::executeState_Move() {}
void dLytSeekerStone_c::finalizeState_Move() {}
void dLytSeekerStone_c::initializeState_Decide() {}
void dLytSeekerStone_c::executeState_Decide() {}
void dLytSeekerStone_c::finalizeState_Decide() {}
void dLytSeekerStone_c::initializeState_SelectScroll() {}
void dLytSeekerStone_c::executeState_SelectScroll() {}
void dLytSeekerStone_c::finalizeState_SelectScroll() {}
void dLytSeekerStone_c::initializeState_Scroll() {}
void dLytSeekerStone_c::executeState_Scroll() {}
void dLytSeekerStone_c::finalizeState_Scroll() {}
void dLytSeekerStone_c::initializeState_FadeOut() {}
void dLytSeekerStone_c::executeState_FadeOut() {}
void dLytSeekerStone_c::finalizeState_FadeOut() {}
void dLytSeekerStone_c::initializeState_FadeIn() {}
void dLytSeekerStone_c::executeState_FadeIn() {}
void dLytSeekerStone_c::finalizeState_FadeIn() {}
void dLytSeekerStone_c::initializeState_Movie() {}
void dLytSeekerStone_c::executeState_Movie() {}
void dLytSeekerStone_c::finalizeState_Movie() {}
void dLytSeekerStone_c::initializeState_MovieFadeOut() {}
void dLytSeekerStone_c::executeState_MovieFadeOut() {}
void dLytSeekerStone_c::finalizeState_MovieFadeOut() {}
void dLytSeekerStone_c::initializeState_MovieFadeIn() {}
void dLytSeekerStone_c::executeState_MovieFadeIn() {}
void dLytSeekerStone_c::finalizeState_MovieFadeIn() {}
void dLytSeekerStone_c::initializeState_Out() {}
void dLytSeekerStone_c::executeState_Out() {}
void dLytSeekerStone_c::finalizeState_Out() {}
