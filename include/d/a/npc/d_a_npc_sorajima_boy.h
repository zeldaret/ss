#ifndef D_A_NPC_SORAJIMA_BOY_H
#define D_A_NPC_SORAJIMA_BOY_H

#include "d/a/npc/d_a_npc.h"
#include "s/s_State.hpp"

class dAcNpcSorajimaBoy_c : public dAcNpc_c {
public:
    dAcNpcSorajimaBoy_c() : mStateMgr(*this) {}
    virtual ~dAcNpcSorajimaBoy_c() {}

private:
    /* 0x??? */ STATE_MGR_DECLARE(dAcNpcSorajimaBoy_c);
};

#endif
