#ifndef D_A_NPC_RIVAL_LOD_H
#define D_A_NPC_RIVAL_LOD_H

#include <d/a/npc/d_a_npc.h>
#include <s/s_State.hpp>
#include <s/s_StateMgr.hpp>

class dAcNpcRivalLOD_c : public dAcNpc_c {
public:
    dAcNpcRivalLOD_c() : mStateMgr(*this, sStateID::null) {}
    virtual ~dAcNpcRivalLOD_c() {}

private:
    /* 0x??? */ STATE_MGR_DECLARE(dAcNpcRivalLOD_c);
};

#endif
