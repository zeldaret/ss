#ifndef D_A_NPC_SENPAI_B_H
#define D_A_NPC_SENPAI_B_H

#include <d/a/npc/d_a_npc.h>
#include <s/s_State.hpp>
#include <s/s_StateMgr.hpp>

class dAcNpcSenpaiB_c : public dAcNpc_c {
public:
    dAcNpcSenpaiB_c() : mStateMgr(*this, sStateID::null) {}
    virtual ~dAcNpcSenpaiB_c() {}

    STATE_FUNC_DECLARE(dAcNpcSenpaiB_c, Think);
    STATE_FUNC_DECLARE(dAcNpcSenpaiB_c, ThinkWait);
    STATE_FUNC_DECLARE(dAcNpcSenpaiB_c, ThinkTurn);
    STATE_FUNC_DECLARE(dAcNpcSenpaiB_c, ThinkRandom);

private:
    /* 0x??? */ STATE_MGR_DECLARE(dAcNpcSenpaiB_c);
};

#endif
