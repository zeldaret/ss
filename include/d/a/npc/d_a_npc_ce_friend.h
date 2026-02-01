#ifndef D_A_NPC_CE_FRIEND_H
#define D_A_NPC_CE_FRIEND_H

#include "d/a/npc/d_a_npc.h"
#include "d/col/c/c_cc_d.h"
#include "s/s_State.hpp"

class dAcNpcCeFriend_c : public dAcNpc_c {
public:
    dAcNpcCeFriend_c() : mStateMgr(*this) {}
    virtual ~dAcNpcCeFriend_c() {}

    bool fn_11_17C0(dAcObjBase_c *);
    bool fn_11_2A40();

private:
    /* 0x??? */ STATE_MGR_DECLARE(dAcNpcCeFriend_c);
};

#endif
