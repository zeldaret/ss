#ifndef D_A_NPC_AKUMAKUN_H
#define D_A_NPC_AKUMAKUN_H

#include <d/a/npc/d_a_npc.h>
#include <s/s_State.hpp>
#include <s/s_StateMgr.hpp>

class dAcNpcAkumakun_c : public dAcNpc_c {
public:
    dAcNpcAkumakun_c() : mStateMgr(*this, sStateID::null) {}
    virtual ~dAcNpcAkumakun_c() {}

private:
    /* 0x??? */ STATE_MGR_DECLARE(dAcNpcAkumakun_c);
};

#endif
