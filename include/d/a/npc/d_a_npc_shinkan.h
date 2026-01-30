#ifndef D_A_NPC_SHINKAN_H
#define D_A_NPC_SHINKAN_H

#include "d/a/npc/d_a_npc.h"
#include "s/s_State.hpp"

class dAcNpcSkn_c : public dAcNpc_c {
public:
    dAcNpcSkn_c() : mStateMgr(*this) {}
    virtual ~dAcNpcSkn_c() {}

    STATE_FUNC_DECLARE(dAcNpcSkn_c, RaceWatch);
    STATE_FUNC_DECLARE(dAcNpcSkn_c, RaceEvt);

private:
    /* 0x??? */ STATE_MGR_DECLARE(dAcNpcSkn_c);
};

#endif
