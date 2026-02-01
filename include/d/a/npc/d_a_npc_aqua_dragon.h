#ifndef D_A_NPC_AQUA_DRAGON_H
#define D_A_NPC_AQUA_DRAGON_H

#include "d/a/npc/d_a_npc.h"
#include "s/s_State.hpp"

class dAcNpcAdr_c : public dAcNpc_c {
public:
    dAcNpcAdr_c() : mStateMgr(*this) {}
    virtual ~dAcNpcAdr_c() {}

private:
    /* 0x??? */ STATE_MGR_DECLARE(dAcNpcAdr_c);
};

#endif
