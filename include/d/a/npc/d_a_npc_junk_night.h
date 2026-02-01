#ifndef D_A_NPC_JUNK_NIGHT_H
#define D_A_NPC_JUNK_NIGHT_H

#include "d/a/npc/d_a_npc.h"
#include "s/s_State.hpp"

class dAcNpcJunkNight_c : public dAcNpc_c {
public:
    dAcNpcJunkNight_c() : mStateMgr(*this) {}
    virtual ~dAcNpcJunkNight_c() {}

private:
    /* 0x??? */ STATE_MGR_DECLARE(dAcNpcJunkNight_c);
};

#endif
