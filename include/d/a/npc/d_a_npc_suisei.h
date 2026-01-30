#ifndef D_A_NPC_SUISEI_H
#define D_A_NPC_SUISEI_H

#include "d/a/npc/d_a_npc.h"
#include "s/s_State.hpp"

class dAcNpcSuisei_c : public dAcNpc_c {
public:
    dAcNpcSuisei_c() : mStateMgr(*this) {}
    virtual ~dAcNpcSuisei_c() {}

private:
    /* 0x??? */ STATE_MGR_DECLARE(dAcNpcSuisei_c);
};

#endif
