#ifndef D_A_NPC_TOILET_GHOST_H
#define D_A_NPC_TOILET_GHOST_H

#include "d/a/npc/d_a_npc.h"
#include "s/s_State.hpp"

class dAcNpcToiletGhost_c : public dAcNpc_c {
public:
    dAcNpcToiletGhost_c() : mStateMgr(*this) {}
    virtual ~dAcNpcToiletGhost_c() {}

private:
    /* 0x??? */ STATE_MGR_DECLARE(dAcNpcToiletGhost_c);
};

#endif
