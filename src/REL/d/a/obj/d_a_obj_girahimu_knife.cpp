#include <d/a/obj/d_a_obj_girahimu_knife.h>

SPECIAL_ACTOR_PROFILE(OBJ_GH_KNIFE, dAcObjGirahimuKnife_c, fProfile::OBJ_GH_KNIFE, 0X115, 0, 2);

STATE_DEFINE(dAcObjGirahimuKnife_c, Wait);
STATE_DEFINE(dAcObjGirahimuKnife_c, SpinWait);
STATE_DEFINE(dAcObjGirahimuKnife_c, SpinWaitPreAttack);
STATE_DEFINE(dAcObjGirahimuKnife_c, SpinFreeWait);
STATE_DEFINE(dAcObjGirahimuKnife_c, FreeWait);
STATE_DEFINE(dAcObjGirahimuKnife_c, Attack);
STATE_DEFINE(dAcObjGirahimuKnife_c, AttackEnd);
STATE_DEFINE(dAcObjGirahimuKnife_c, Return);
STATE_DEFINE(dAcObjGirahimuKnife_c, Hit);
STATE_DEFINE(dAcObjGirahimuKnife_c, CircleWait);

void dAcObjGirahimuKnife_c::initializeState_Wait() {}
void dAcObjGirahimuKnife_c::executeState_Wait() {}
void dAcObjGirahimuKnife_c::finalizeState_Wait() {}
void dAcObjGirahimuKnife_c::initializeState_SpinWait() {}
void dAcObjGirahimuKnife_c::executeState_SpinWait() {}
void dAcObjGirahimuKnife_c::finalizeState_SpinWait() {}
void dAcObjGirahimuKnife_c::initializeState_SpinWaitPreAttack() {}
void dAcObjGirahimuKnife_c::executeState_SpinWaitPreAttack() {}
void dAcObjGirahimuKnife_c::finalizeState_SpinWaitPreAttack() {}
void dAcObjGirahimuKnife_c::initializeState_SpinFreeWait() {}
void dAcObjGirahimuKnife_c::executeState_SpinFreeWait() {}
void dAcObjGirahimuKnife_c::finalizeState_SpinFreeWait() {}
void dAcObjGirahimuKnife_c::initializeState_FreeWait() {}
void dAcObjGirahimuKnife_c::executeState_FreeWait() {}
void dAcObjGirahimuKnife_c::finalizeState_FreeWait() {}
void dAcObjGirahimuKnife_c::initializeState_Attack() {}
void dAcObjGirahimuKnife_c::executeState_Attack() {}
void dAcObjGirahimuKnife_c::finalizeState_Attack() {}
void dAcObjGirahimuKnife_c::initializeState_AttackEnd() {}
void dAcObjGirahimuKnife_c::executeState_AttackEnd() {}
void dAcObjGirahimuKnife_c::finalizeState_AttackEnd() {}
void dAcObjGirahimuKnife_c::initializeState_Return() {}
void dAcObjGirahimuKnife_c::executeState_Return() {}
void dAcObjGirahimuKnife_c::finalizeState_Return() {}
void dAcObjGirahimuKnife_c::initializeState_Hit() {}
void dAcObjGirahimuKnife_c::executeState_Hit() {}
void dAcObjGirahimuKnife_c::finalizeState_Hit() {}
void dAcObjGirahimuKnife_c::initializeState_CircleWait() {}
void dAcObjGirahimuKnife_c::executeState_CircleWait() {}
void dAcObjGirahimuKnife_c::finalizeState_CircleWait() {}
