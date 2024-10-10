#ifndef D_A_NPC_MOLE_TACKLE_H
#define D_A_NPC_MOLE_TACKLE_H

#include <d/a/npc/d_a_npc.h>
#include <s/s_State.hpp>
#include <s/s_StateMgr.hpp>

class dAcNpcMoleTackle_c : public dAcNpc_c {
public:
    dAcNpcMoleTackle_c() : mStateMgr(*this, sStateID::null) {}
    virtual ~dAcNpcMoleTackle_c() {}

private:
    /* 0x??? */ STATE_MGR_DECLARE(dAcNpcMoleTackle_c);
};

#endif
