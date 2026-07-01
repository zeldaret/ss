#ifndef D_A_NPC_INSECT_CAPTURE_GAME_KOBUN_H
#define D_A_NPC_INSECT_CAPTURE_GAME_KOBUN_H

#include "d/a/npc/d_a_npc.h"
#include "s/s_State.hpp"

class dAcNpcIcgKobun_c : public dAcNpc_c {
public:
    dAcNpcIcgKobun_c() : mStateMgr(*this) {}
    virtual ~dAcNpcIcgKobun_c() {}
    void fn_66_30C0(u32 itemId);

private:
    /* 0x??? */ STATE_MGR_DECLARE(dAcNpcIcgKobun_c);
};

#endif
