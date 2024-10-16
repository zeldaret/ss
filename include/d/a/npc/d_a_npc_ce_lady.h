#ifndef D_A_NPC_CE_LADY_H
#define D_A_NPC_CE_LADY_H

#include <d/a/npc/d_a_npc.h>
#include <s/s_State.hpp>
#include <s/s_StateMgr.hpp>

class dAcNpcCeLady_c : public dAcNpc_c {
public:
    dAcNpcCeLady_c() : mStateMgr(*this, sStateID::null) {}
    virtual ~dAcNpcCeLady_c() {}

private:
    /* 0x??? */ STATE_MGR_DECLARE(dAcNpcCeLady_c);
};

#endif
