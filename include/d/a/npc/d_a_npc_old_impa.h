#ifndef D_A_NPC_OLD_IMPA_H
#define D_A_NPC_OLD_IMPA_H

#include "d/a/npc/d_a_npc.h"
#include "s/s_State.hpp"

class dAcNpcOim_c : public dAcNpc_c {
public:
    dAcNpcOim_c() : mStateMgr(*this) {}
    virtual ~dAcNpcOim_c() {}

private:
    /* 0x??? */ STATE_MGR_DECLARE(dAcNpcOim_c);
};

#endif
