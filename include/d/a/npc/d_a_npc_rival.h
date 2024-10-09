#ifndef D_A_NPC_RIVAL_H
#define D_A_NPC_RIVAL_H

#include <d/a/npc/d_a_npc.h>
#include <s/s_State.hpp>
#include <s/s_StateMgr.hpp>

class dAcNpcRvl_c : public dAcNpc_c {
public:
    dAcNpcRvl_c() : mStateMgr(*this, sStateID::null) {}
    virtual ~dAcNpcRvl_c() {}

    STATE_FUNC_DECLARE(dAcNpcRvl_c, RaceInit);
    STATE_FUNC_DECLARE(dAcNpcRvl_c, RaceRun);
    STATE_FUNC_DECLARE(dAcNpcRvl_c, RaceDive);
    STATE_FUNC_DECLARE(dAcNpcRvl_c, RaceRide);

private:
    /* 0x??? */ STATE_MGR_DECLARE(dAcNpcRvl_c);
};

#endif
