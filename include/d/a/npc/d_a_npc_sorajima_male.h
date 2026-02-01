#ifndef D_A_NPC_SORAJIMA_MALE_H
#define D_A_NPC_SORAJIMA_MALE_H

#include "d/a/npc/d_a_npc.h"
#include "s/s_State.hpp"

class dAcNpcSorajimaMale_c : public dAcNpc_c {
public:
    dAcNpcSorajimaMale_c() : mStateMgr(*this) {}
    virtual ~dAcNpcSorajimaMale_c() {}

private:
    /* 0x??? */ STATE_MGR_DECLARE(dAcNpcSorajimaMale_c);
};

#endif
