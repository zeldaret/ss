#ifndef D_A_NPC_DESERTROBOT_CAPTAIN_H
#define D_A_NPC_DESERTROBOT_CAPTAIN_H

#include "d/a/npc/d_a_npc.h"
#include "s/s_State.hpp"

class dAcNpcDrbC_c : public dAcNpc_c {
public:
    dAcNpcDrbC_c() : mStateMgr(*this) {}
    virtual ~dAcNpcDrbC_c() {}

private:
    /* 0x??? */ STATE_MGR_DECLARE(dAcNpcDrbC_c);
};

#endif
