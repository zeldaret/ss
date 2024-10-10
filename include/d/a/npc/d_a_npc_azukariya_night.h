#ifndef D_A_NPC_AZUKARIYA_NIGHT_H
#define D_A_NPC_AZUKARIYA_NIGHT_H

#include <d/a/npc/d_a_npc.h>
#include <s/s_State.hpp>
#include <s/s_StateMgr.hpp>

class dAcNpcAzukariyaNight_c : public dAcNpc_c {
public:
    dAcNpcAzukariyaNight_c() : mStateMgr(*this, sStateID::null) {}
    virtual ~dAcNpcAzukariyaNight_c() {}

private:
    /* 0x??? */ STATE_MGR_DECLARE(dAcNpcAzukariyaNight_c);
};

#endif
