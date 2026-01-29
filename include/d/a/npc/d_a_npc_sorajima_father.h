#ifndef D_A_NPC_SORAJIMA_FATHER_H
#define D_A_NPC_SORAJIMA_FATHER_H

#include "d/a/npc/d_a_npc.h"
#include "s/s_State.hpp"

class dAcNpcSorajimaFather_c : public dAcNpc_c {
public:
    dAcNpcSorajimaFather_c() : mStateMgr(*this) {}
    virtual ~dAcNpcSorajimaFather_c() {}

private:
    /* 0x??? */ STATE_MGR_DECLARE(dAcNpcSorajimaFather_c);
};

#endif
