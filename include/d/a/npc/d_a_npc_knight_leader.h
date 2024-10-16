#ifndef D_A_NPC_KNIGHT_LEADER_H
#define D_A_NPC_KNIGHT_LEADER_H

#include <d/a/npc/d_a_npc.h>
#include <s/s_State.hpp>
#include <s/s_StateMgr.hpp>

class dAcNpcKnightLeader_c : public dAcNpc_c {
public:
    dAcNpcKnightLeader_c() : mStateMgr(*this, sStateID::null) {}
    virtual ~dAcNpcKnightLeader_c() {}

private:
    /* 0x??? */ STATE_MGR_DECLARE(dAcNpcKnightLeader_c);
};

#endif
