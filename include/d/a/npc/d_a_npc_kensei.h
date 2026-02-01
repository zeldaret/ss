#ifndef D_A_NPC_KENSEI_H
#define D_A_NPC_KENSEI_H

#include "d/a/npc/d_a_npc.h"
#include "s/s_State.hpp"

class dAcNpcKensei_c : public dAcNpc_c {
public:
    dAcNpcKensei_c() : mStateMgr(*this) {}
    virtual ~dAcNpcKensei_c() {}

    static dAcNpcKensei_c* GetInstance() {
        return sInstance;
    }

private:
    static dAcNpcKensei_c *sInstance;
    
    /* 0x??? */ STATE_MGR_DECLARE(dAcNpcKensei_c);
};



#endif
