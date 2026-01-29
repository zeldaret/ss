#ifndef D_A_NPC_GODDESS_ZELDA_H
#define D_A_NPC_GODDESS_ZELDA_H

#include "d/a/npc/d_a_npc.h"
#include "s/s_State.hpp"

class dAcNpcGzl_c : public dAcNpc_c {
public:
    dAcNpcGzl_c() : mStateMgr(*this) {}
    virtual ~dAcNpcGzl_c() {}

private:
    /* 0x??? */ STATE_MGR_DECLARE(dAcNpcGzl_c);
};

#endif
