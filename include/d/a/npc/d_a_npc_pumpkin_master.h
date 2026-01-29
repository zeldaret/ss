#ifndef D_A_NPC_PUMPKIN_MASTER_H
#define D_A_NPC_PUMPKIN_MASTER_H

#include "d/a/npc/d_a_npc.h"
#include "s/s_State.hpp"

class dAcNpcPma_c : public dAcNpc_c {
public:
    dAcNpcPma_c() : mStateMgr(*this) {}
    virtual ~dAcNpcPma_c() {}

private:
    /* 0x??? */ STATE_MGR_DECLARE(dAcNpcPma_c);
};

#endif
