#ifndef D_A_NPC_TESTMAN_H
#define D_A_NPC_TESTMAN_H

#include "d/a/npc/d_a_npc.h"
#include "s/s_State.hpp"

class dAcNpcTmn_c : public dAcNpc_c {
public:
    dAcNpcTmn_c() : mStateMgr(*this) {}
    virtual ~dAcNpcTmn_c() {}

private:
    /* 0x??? */ STATE_MGR_DECLARE(dAcNpcTmn_c);
};

#endif
