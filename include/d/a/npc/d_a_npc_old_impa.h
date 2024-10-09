#ifndef D_A_NPC_OLD_IMPA_H
#define D_A_NPC_OLD_IMPA_H

#include <d/a/npc/d_a_npc.h>
#include <s/s_State.hpp>
#include <s/s_StateMgr.hpp>

class dAcNpcOim_c : public dAcNpc_c {
public:
    dAcNpcOim_c() : mStateMgr(*this, sStateID::null) {}
    virtual ~dAcNpcOim_c() {}

private:
    /* 0x??? */ STATE_MGR_DECLARE(dAcNpcOim_c);
};

#endif
