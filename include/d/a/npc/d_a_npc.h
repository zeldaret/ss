#ifndef D_A_NPC_H
#define D_A_NPC_H

#include "d/a/obj/d_a_obj_base.h"
#include "s/s_State.hpp"

// This is the NPC base. Most npcs actually use dAcOrdinaryNpc, but this just is a simpler one?

// Ghidra: ActorNpcBase
//   size: 0x6e4
// official name
class dAcNpc_c : public dAcObjBase_c {
public:
    dAcNpc_c();

    STATE_VIRTUAL_FUNC_DECLARE(dAcNpc_c, Wait);
    STATE_VIRTUAL_FUNC_DECLARE(dAcNpc_c, Demo);

    STATE_MGR_DECLARE(dAcNpc_c);
};

#endif
