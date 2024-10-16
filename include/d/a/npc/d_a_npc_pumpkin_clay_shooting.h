#ifndef D_A_NPC_PUMPKIN_CLAY_SHOOTING_H
#define D_A_NPC_PUMPKIN_CLAY_SHOOTING_H

#include "d/a/npc/d_a_npc.h"
#include "s/s_State.hpp"
#include "s/s_StateMgr.hpp"

class dAcNpcPcs_c : public dAcNpc_c {
public:
    dAcNpcPcs_c() : mStateMgr(*this, sStateID::null) {}
    virtual ~dAcNpcPcs_c() {}

private:
    /* 0x??? */ STATE_MGR_DECLARE(dAcNpcPcs_c);
};

#endif
