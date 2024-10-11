#ifndef D_A_NPC_THUNDER_DRAGON_H
#define D_A_NPC_THUNDER_DRAGON_H

#include <d/a/npc/d_a_npc.h>
#include <s/s_State.hpp>
#include <s/s_StateMgr.hpp>

class dAcNpcTdr_c : public dAcNpc_c {
public:
    dAcNpcTdr_c() : mStateMgr(*this, sStateID::null) {}
    virtual ~dAcNpcTdr_c() {}

private:
    /* 0x??? */ STATE_MGR_DECLARE(dAcNpcTdr_c);
};

#endif
