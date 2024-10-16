#ifndef D_A_NPC_KYUI_FIRST_H
#define D_A_NPC_KYUI_FIRST_H

#include "d/a/npc/d_a_npc.h"
#include "s/s_State.hpp"
#include "s/s_StateMgr.hpp"

class dAcNpcKyuiFirst_c : public dAcNpc_c {
public:
    dAcNpcKyuiFirst_c() : mStateMgr(*this, sStateID::null) {}
    virtual ~dAcNpcKyuiFirst_c() {}

    STATE_FUNC_DECLARE(dAcNpcKyuiFirst_c, Fear);
    STATE_FUNC_DECLARE(dAcNpcKyuiFirst_c, Rest);
    STATE_FUNC_DECLARE(dAcNpcKyuiFirst_c, Hide);
    STATE_FUNC_DECLARE(dAcNpcKyuiFirst_c, Kill);
    STATE_FUNC_DECLARE(dAcNpcKyuiFirst_c, Ambash);
    STATE_FUNC_DECLARE(dAcNpcKyuiFirst_c, AmbashWalk);
    STATE_FUNC_DECLARE(dAcNpcKyuiFirst_c, InitAmbash);

private:
    /* 0x??? */ STATE_MGR_DECLARE(dAcNpcKyuiFirst_c);
};

#endif
