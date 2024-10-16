#ifndef D_A_NPC_SUISEI_H
#define D_A_NPC_SUISEI_H

#include "d/a/npc/d_a_npc.h"
#include "s/s_State.hpp"
#include "s/s_StateMgr.hpp"

class dAcNpcSuisei_c : public dAcNpc_c {
public:
    dAcNpcSuisei_c() : mStateMgr(*this, sStateID::null) {}
    virtual ~dAcNpcSuisei_c() {}

private:
    /* 0x??? */ STATE_MGR_DECLARE(dAcNpcSuisei_c);
};

#endif
