#ifndef D_A_NPC_MOLE_ELDER_H
#define D_A_NPC_MOLE_ELDER_H

#include "d/a/npc/d_a_npc.h"
#include "s/s_State.hpp"

class dAcNpcMoleElder_c : public dAcNpc_c {
public:
    dAcNpcMoleElder_c() : mStateMgr(*this) {}
    virtual ~dAcNpcMoleElder_c() {}

private:
    /* 0x??? */ STATE_MGR_DECLARE(dAcNpcMoleElder_c);
};

#endif
