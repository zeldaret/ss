#include <d/a/d_a_insect_grasshopper.h>

SPECIAL_ACTOR_PROFILE(INSECT_GRASSHOPPER, dAcInsectGrasshopper_c, fProfile::INSECT_GRASSHOPPER, 0X16B, 0, 3);

STATE_DEFINE(dAcInsectGrasshopper_c, Wait);
STATE_DEFINE(dAcInsectGrasshopper_c, Walk);
STATE_DEFINE(dAcInsectGrasshopper_c, Jump);
STATE_DEFINE(dAcInsectGrasshopper_c, Fly);
STATE_DEFINE(dAcInsectGrasshopper_c, Finalize);
STATE_DEFINE(dAcInsectGrasshopper_c, Dead);

void dAcInsectGrasshopper_c::initializeState_Wait() {}
void dAcInsectGrasshopper_c::executeState_Wait() {}
void dAcInsectGrasshopper_c::finalizeState_Wait() {}
void dAcInsectGrasshopper_c::initializeState_Walk() {}
void dAcInsectGrasshopper_c::executeState_Walk() {}
void dAcInsectGrasshopper_c::finalizeState_Walk() {}
void dAcInsectGrasshopper_c::initializeState_Jump() {}
void dAcInsectGrasshopper_c::executeState_Jump() {}
void dAcInsectGrasshopper_c::finalizeState_Jump() {}
void dAcInsectGrasshopper_c::initializeState_Fly() {}
void dAcInsectGrasshopper_c::executeState_Fly() {}
void dAcInsectGrasshopper_c::finalizeState_Fly() {}
void dAcInsectGrasshopper_c::initializeState_Finalize() {}
void dAcInsectGrasshopper_c::executeState_Finalize() {}
void dAcInsectGrasshopper_c::finalizeState_Finalize() {}
void dAcInsectGrasshopper_c::initializeState_Dead() {}
void dAcInsectGrasshopper_c::executeState_Dead() {}
void dAcInsectGrasshopper_c::finalizeState_Dead() {}
