#ifndef D_A_NPC_ORD_KYUI_H
#define D_A_NPC_ORD_KYUI_H

#include "d/a/npc/d_a_npc.h"
#include "s/s_State.hpp"

class dAcNpcOrdKyui_c : public dAcNpc_c {
public:
    dAcNpcOrdKyui_c() : mStateMgr(*this) {}
    virtual ~dAcNpcOrdKyui_c() {}

    STATE_FUNC_DECLARE(dAcNpcOrdKyui_c, Hide);

private:
    /* 0x??? */ STATE_MGR_DECLARE(dAcNpcOrdKyui_c);
};

#endif
