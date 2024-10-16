#ifndef D_A_NPC_MOLE_TACKLE2_H
#define D_A_NPC_MOLE_TACKLE2_H

#include <d/a/npc/d_a_npc.h>
#include <s/s_State.hpp>
#include <s/s_StateMgr.hpp>

class dAcNpcMoleTackle2_c : public dAcNpc_c {
public:
    dAcNpcMoleTackle2_c() : mStateMgr(*this, sStateID::null) {}
    virtual ~dAcNpcMoleTackle2_c() {}

private:
    /* 0x??? */ STATE_MGR_DECLARE(dAcNpcMoleTackle2_c);
};

#endif
