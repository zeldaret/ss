#ifndef D_A_NPC_ZELDA_H
#define D_A_NPC_ZELDA_H

#include <d/a/npc/d_a_npc.h>
#include <s/s_State.hpp>
#include <s/s_StateMgr.hpp>

class dAcNpcZld_c : public dAcNpc_c {
public:
    dAcNpcZld_c() : mStateMgr(*this, sStateID::null) {}
    virtual ~dAcNpcZld_c() {}

private:
    /* 0x??? */ STATE_MGR_DECLARE(dAcNpcZld_c);
};

#endif
