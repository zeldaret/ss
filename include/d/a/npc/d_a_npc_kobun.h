#ifndef D_A_NPC_KOBUN_H
#define D_A_NPC_KOBUN_H

#include "d/a/npc/d_a_npc.h"
#include "s/s_State.hpp"
#include "s/s_StateMgr.hpp"

class dAcNpcKbn_c : public dAcNpc_c {
public:
    dAcNpcKbn_c() : mStateMgr(*this, sStateID::null) {}
    virtual ~dAcNpcKbn_c() {}

    STATE_FUNC_DECLARE(dAcNpcKbn_c, RaceInit);
    STATE_FUNC_DECLARE(dAcNpcKbn_c, RaceRun);
    STATE_FUNC_DECLARE(dAcNpcKbn_c, RaceDive);
    STATE_FUNC_DECLARE(dAcNpcKbn_c, RaceRide);
    STATE_FUNC_DECLARE(dAcNpcKbn_c, LetterWait);
    STATE_FUNC_DECLARE(dAcNpcKbn_c, Sleep);
    STATE_FUNC_DECLARE(dAcNpcKbn_c, Sit);

private:
    /* 0x??? */ STATE_MGR_DECLARE(dAcNpcKbn_c);
};

#endif
