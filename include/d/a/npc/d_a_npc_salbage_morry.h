#ifndef D_A_NPC_SALBAGE_MORRY_H
#define D_A_NPC_SALBAGE_MORRY_H

#include "d/a/npc/d_a_npc.h"
#include "s/s_State.hpp"
#include "s/s_StateMgr.hpp"

class dAcNpcSalbageMorry_c : public dAcNpc_c {
public:
    dAcNpcSalbageMorry_c() : mStateMgr(*this, sStateID::null) {}
    virtual ~dAcNpcSalbageMorry_c() {}

private:
    /* 0x??? */ STATE_MGR_DECLARE(dAcNpcSalbageMorry_c);
};

#endif
