#ifndef D_A_NPC_DIVE_GAME_JUDGE_H
#define D_A_NPC_DIVE_GAME_JUDGE_H

#include "d/a/npc/d_a_npc.h"
#include "s/s_State.hpp"

class dAcNpcDiveGameJudge_c : public dAcNpc_c {
public:
    dAcNpcDiveGameJudge_c() : mStateMgr(*this) {}
    virtual ~dAcNpcDiveGameJudge_c() {}

    STATE_FUNC_DECLARE(dAcNpcDiveGameJudge_c, Sad);
    STATE_FUNC_DECLARE(dAcNpcDiveGameJudge_c, PreWatch);
    STATE_FUNC_DECLARE(dAcNpcDiveGameJudge_c, Watch);
    STATE_FUNC_DECLARE(dAcNpcDiveGameJudge_c, Escape);
    STATE_FUNC_DECLARE(dAcNpcDiveGameJudge_c, EscapeEnd);

private:
    /* 0x??? */ STATE_MGR_DECLARE(dAcNpcDiveGameJudge_c);
};

#endif
