#include <d/a/obj/d_a_obj_musasabi.h>

SPECIAL_ACTOR_PROFILE(OBJ_MUSASABI, dAcOmusasabi_c, fProfile::OBJ_MUSASABI, 0x1C3, 0, 6);

STATE_DEFINE(dAcOmusasabi_c, Wait);
STATE_DEFINE(dAcOmusasabi_c, Approach);
STATE_DEFINE(dAcOmusasabi_c, Near);
STATE_DEFINE(dAcOmusasabi_c, Far);
STATE_DEFINE(dAcOmusasabi_c, Formation);
STATE_DEFINE(dAcOmusasabi_c, Away);
STATE_DEFINE(dAcOmusasabi_c, Disappear);
STATE_DEFINE(dAcOmusasabi_c, FormNotPlayer);

void dAcOmusasabi_c::initializeState_Wait() {}
void dAcOmusasabi_c::executeState_Wait() {}
void dAcOmusasabi_c::finalizeState_Wait() {}
void dAcOmusasabi_c::initializeState_Approach() {}
void dAcOmusasabi_c::executeState_Approach() {}
void dAcOmusasabi_c::finalizeState_Approach() {}
void dAcOmusasabi_c::initializeState_Near() {}
void dAcOmusasabi_c::executeState_Near() {}
void dAcOmusasabi_c::finalizeState_Near() {}
void dAcOmusasabi_c::initializeState_Far() {}
void dAcOmusasabi_c::executeState_Far() {}
void dAcOmusasabi_c::finalizeState_Far() {}
void dAcOmusasabi_c::initializeState_Formation() {}
void dAcOmusasabi_c::executeState_Formation() {}
void dAcOmusasabi_c::finalizeState_Formation() {}
void dAcOmusasabi_c::initializeState_Away() {}
void dAcOmusasabi_c::executeState_Away() {}
void dAcOmusasabi_c::finalizeState_Away() {}
void dAcOmusasabi_c::initializeState_Disappear() {}
void dAcOmusasabi_c::executeState_Disappear() {}
void dAcOmusasabi_c::finalizeState_Disappear() {}
void dAcOmusasabi_c::initializeState_FormNotPlayer() {}
void dAcOmusasabi_c::executeState_FormNotPlayer() {}
void dAcOmusasabi_c::finalizeState_FormNotPlayer() {}
