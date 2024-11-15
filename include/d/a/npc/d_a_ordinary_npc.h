#ifndef D_A_ORDINARY_NPC_H
#define D_A_ORDINARY_NPC_H

#include "d/a/npc/d_a_npc.h"
#include "s/s_State.hpp"

class dAcOrdinaryNpc_c : public dAcNpc_c {
public:
    /* vt 0x1F4 */ virtual int acNpc_vt_0x1F4() override;

    STATE_VIRTUAL_OVERRIDE_FUNC_DECLARE(dAcOrdinaryNpc_c, dAcNpc_c, Wait);
    STATE_VIRTUAL_FUNC_DECLARE(dAcOrdinaryNpc_c, Pain);
    STATE_VIRTUAL_FUNC_DECLARE(dAcOrdinaryNpc_c, Surprised);
    STATE_VIRTUAL_FUNC_DECLARE(dAcOrdinaryNpc_c, Withstand);
    STATE_VIRTUAL_FUNC_DECLARE(dAcOrdinaryNpc_c, Walk);
    STATE_VIRTUAL_FUNC_DECLARE(dAcOrdinaryNpc_c, LookAwaySt);
    STATE_VIRTUAL_FUNC_DECLARE(dAcOrdinaryNpc_c, LookAway);
    STATE_VIRTUAL_FUNC_DECLARE(dAcOrdinaryNpc_c, Turn);
    STATE_FUNC_DECLARE(dAcOrdinaryNpc_c, PreWalkTurn);
};

#endif
