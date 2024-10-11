#ifndef D_A_NPC_GRA_H
#define D_A_NPC_GRA_H

#include <d/a/npc/d_a_npc.h>
#include <s/s_State.hpp>
#include <s/s_StateMgr.hpp>

class dAcNpcGra_c : public dAcNpc_c {
public:
    dAcNpcGra_c() : mStateMgr(*this, sStateID::null) {}
    virtual ~dAcNpcGra_c() {}

private:
    /* 0x??? */ STATE_MGR_DECLARE(dAcNpcGra_c);
};

#endif
