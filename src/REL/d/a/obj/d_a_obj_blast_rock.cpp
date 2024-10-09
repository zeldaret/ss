#include <d/a/obj/d_a_obj_blast_rock.h>

SPECIAL_ACTOR_PROFILE(OBJ_BLAST_ROCK, dAcOBlastRock_c, fProfile::OBJ_BLAST_ROCK, 0x199, 0, 6);

STATE_DEFINE(dAcOBlastRock_c, Wait);
STATE_DEFINE(dAcOBlastRock_c, Piece);
STATE_DEFINE(dAcOBlastRock_c, Broken);

void dAcOBlastRock_c::initializeState_Wait() {}
void dAcOBlastRock_c::executeState_Wait() {}
void dAcOBlastRock_c::finalizeState_Wait() {}
void dAcOBlastRock_c::initializeState_Piece() {}
void dAcOBlastRock_c::executeState_Piece() {}
void dAcOBlastRock_c::finalizeState_Piece() {}
void dAcOBlastRock_c::initializeState_Broken() {}
void dAcOBlastRock_c::executeState_Broken() {}
void dAcOBlastRock_c::finalizeState_Broken() {}
