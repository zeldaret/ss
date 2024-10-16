#ifndef D_A_NPC_CE_FRIEND_H
#define D_A_NPC_CE_FRIEND_H

#include <d/a/npc/d_a_npc.h>
#include <s/s_State.hpp>
#include <s/s_StateMgr.hpp>

class dAcNpcCeFriend_c : public dAcNpc_c {
public:
    dAcNpcCeFriend_c() : mStateMgr(*this, sStateID::null) {}
    virtual ~dAcNpcCeFriend_c() {}

private:
    /* 0x??? */ STATE_MGR_DECLARE(dAcNpcCeFriend_c);
};

#endif
