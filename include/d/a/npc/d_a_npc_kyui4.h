#ifndef D_A_NPC_KYUI4_H
#define D_A_NPC_KYUI4_H

#include <d/a/npc/d_a_npc.h>
#include <s/s_State.hpp>
#include <s/s_StateMgr.hpp>

class dAcNpcKyui4_c : public dAcNpc_c {
public:
    dAcNpcKyui4_c() : mStateMgr(*this, sStateID::null) {}
    virtual ~dAcNpcKyui4_c() {}

private:
    /* 0x??? */ STATE_MGR_DECLARE(dAcNpcKyui4_c);
};

#endif
