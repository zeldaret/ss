#ifndef D_A_NPC_GRC_H
#define D_A_NPC_GRC_H

#include "d/a/npc/d_a_npc.h"
#include "s/s_State.hpp"

class dAcNpcGrc_c : public dAcNpc_c {
public:
    dAcNpcGrc_c() : mStateMgr(*this) {}
    virtual ~dAcNpcGrc_c() {}

private:
    /* 0x??? */ STATE_MGR_DECLARE(dAcNpcGrc_c);
};

#endif
