#ifndef D_A_NPC_MOLE_MG_H
#define D_A_NPC_MOLE_MG_H

#include "d/a/npc/d_a_npc.h"
#include "s/s_State.hpp"
#include "s/s_StateMgr.hpp"

class dAcNpcMoleMg_c : public dAcNpc_c {
public:
    dAcNpcMoleMg_c() : mStateMgr(*this, sStateID::null) {}
    virtual ~dAcNpcMoleMg_c() {}

    STATE_FUNC_DECLARE(dAcNpcMoleMg_c, Result);
    STATE_FUNC_DECLARE(dAcNpcMoleMg_c, Hide);

private:
    /* 0x??? */ STATE_MGR_DECLARE(dAcNpcMoleMg_c);
};

#endif
