#include <d/a/obj/d_a_obj_kanban_stone.h>

SPECIAL_ACTOR_PROFILE(OBJ_KANBAN_STONE, dAcOKanbanStone_c, fProfile::OBJ_KANBAN_STONE, 0x182, 0, 3);

STATE_DEFINE(dAcOKanbanStone_c, Wait);
STATE_DEFINE(dAcOKanbanStone_c, Message);

void dAcOKanbanStone_c::initializeState_Wait() {}
void dAcOKanbanStone_c::executeState_Wait() {}
void dAcOKanbanStone_c::finalizeState_Wait() {}
void dAcOKanbanStone_c::initializeState_Message() {}
void dAcOKanbanStone_c::executeState_Message() {}
void dAcOKanbanStone_c::finalizeState_Message() {}
