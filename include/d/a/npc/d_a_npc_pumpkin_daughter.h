#ifndef D_A_NPC_PUMPKIN_DAUGHTER_H
#define D_A_NPC_PUMPKIN_DAUGHTER_H

#include "d/a/npc/d_a_npc.h"
#include "s/s_State.hpp"
#include "s/s_StateMgr.hpp"

class dAcNpcPdu_c : public dAcNpc_c {
public:
    dAcNpcPdu_c() : mStateMgr(*this, sStateID::null) {}
    virtual ~dAcNpcPdu_c() {}

private:
    /* 0x??? */ STATE_MGR_DECLARE(dAcNpcPdu_c);
};

#endif
