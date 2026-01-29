#ifndef D_A_NPC_URANAIYA_H
#define D_A_NPC_URANAIYA_H

#include "d/a/npc/d_a_npc.h"
#include "s/s_State.hpp"

class dAcNpcSha_c : public dAcNpc_c {
public:
    dAcNpcSha_c() : mStateMgr(*this) {}
    virtual ~dAcNpcSha_c() {}

private:
    /* 0x??? */ STATE_MGR_DECLARE(dAcNpcSha_c);
};

#endif
