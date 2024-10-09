#include <d/a/obj/d_a_obj_fence_iron.h>

SPECIAL_ACTOR_PROFILE(OBJ_FENCE_IRON, dAcOfenceIron_c, fProfile::OBJ_FENCE_IRON, 0x1C4, 0, 2);

STATE_DEFINE(dAcOfenceIron_c, Close);
STATE_DEFINE(dAcOfenceIron_c, PreMoveUp);
STATE_DEFINE(dAcOfenceIron_c, MoveUp);
STATE_DEFINE(dAcOfenceIron_c, AfterMoveUp);
STATE_DEFINE(dAcOfenceIron_c, Open);
STATE_DEFINE(dAcOfenceIron_c, PreMoveDown);
STATE_DEFINE(dAcOfenceIron_c, MoveDown);
STATE_DEFINE(dAcOfenceIron_c, AfterMoveDown);

void dAcOfenceIron_c::initializeState_Close() {}
void dAcOfenceIron_c::executeState_Close() {}
void dAcOfenceIron_c::finalizeState_Close() {}
void dAcOfenceIron_c::initializeState_PreMoveUp() {}
void dAcOfenceIron_c::executeState_PreMoveUp() {}
void dAcOfenceIron_c::finalizeState_PreMoveUp() {}
void dAcOfenceIron_c::initializeState_MoveUp() {}
void dAcOfenceIron_c::executeState_MoveUp() {}
void dAcOfenceIron_c::finalizeState_MoveUp() {}
void dAcOfenceIron_c::initializeState_AfterMoveUp() {}
void dAcOfenceIron_c::executeState_AfterMoveUp() {}
void dAcOfenceIron_c::finalizeState_AfterMoveUp() {}
void dAcOfenceIron_c::initializeState_Open() {}
void dAcOfenceIron_c::executeState_Open() {}
void dAcOfenceIron_c::finalizeState_Open() {}
void dAcOfenceIron_c::initializeState_PreMoveDown() {}
void dAcOfenceIron_c::executeState_PreMoveDown() {}
void dAcOfenceIron_c::finalizeState_PreMoveDown() {}
void dAcOfenceIron_c::initializeState_MoveDown() {}
void dAcOfenceIron_c::executeState_MoveDown() {}
void dAcOfenceIron_c::finalizeState_MoveDown() {}
void dAcOfenceIron_c::initializeState_AfterMoveDown() {}
void dAcOfenceIron_c::executeState_AfterMoveDown() {}
void dAcOfenceIron_c::finalizeState_AfterMoveDown() {}
