#ifndef D_A_NPC_SORAJIMA_MOTHER_H
#define D_A_NPC_SORAJIMA_MOTHER_H

#include "d/a/npc/d_a_npc.h"
#include "s/s_State.hpp"

class dAcNpcSorajimaMother_c : public dAcNpc_c {
public:
    dAcNpcSorajimaMother_c() : mStateMgr(*this) {}
    virtual ~dAcNpcSorajimaMother_c() {}

private:
    /* 0x??? */ STATE_MGR_DECLARE(dAcNpcSorajimaMother_c);
};

#endif
