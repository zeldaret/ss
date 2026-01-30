#ifndef D_A_NPC_SALESMAN_S_H
#define D_A_NPC_SALESMAN_S_H

#include "d/a/npc/d_a_npc.h"
#include "s/s_State.hpp"

class dAcNpcSalesmanS_c : public dAcNpc_c {
public:
    dAcNpcSalesmanS_c() : mStateMgr(*this) {}
    virtual ~dAcNpcSalesmanS_c() {}

private:
    /* 0x??? */ STATE_MGR_DECLARE(dAcNpcSalesmanS_c);
};

#endif
