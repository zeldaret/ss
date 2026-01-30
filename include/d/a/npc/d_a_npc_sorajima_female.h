#ifndef D_A_NPC_SORAJIMA_FEMALE_H
#define D_A_NPC_SORAJIMA_FEMALE_H

#include "d/a/npc/d_a_npc.h"
#include "s/s_State.hpp"

class dAcNpcSorajimaFemale_c : public dAcNpc_c {
public:
    dAcNpcSorajimaFemale_c() : mStateMgr(*this) {}
    virtual ~dAcNpcSorajimaFemale_c() {}

private:
    /* 0x??? */ STATE_MGR_DECLARE(dAcNpcSorajimaFemale_c);
};

#endif
