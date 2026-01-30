#ifndef D_A_NPC_DESERTROBOT_H
#define D_A_NPC_DESERTROBOT_H

#include "d/a/npc/d_a_npc.h"
#include "s/s_State.hpp"

class dAcNpcDrb_c : public dAcNpc_c {
public:
    dAcNpcDrb_c() : mStateMgr(*this) {}
    virtual ~dAcNpcDrb_c() {}

private:
    /* 0x??? */ STATE_MGR_DECLARE(dAcNpcDrb_c);
};

#endif
