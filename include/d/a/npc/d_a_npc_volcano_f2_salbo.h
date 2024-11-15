#ifndef D_A_NPC_VOLCANO_F2_SALBO_H
#define D_A_NPC_VOLCANO_F2_SALBO_H

#include "d/a/npc/d_a_ordinary_npc.h"
#include "s/s_State.hpp"

class dAcNpcSlb2_c : public dAcOrdinaryNpc_c {
public:
    dAcNpcSlb2_c() : mStateMgr(*this, sStateID::null) {}
    virtual ~dAcNpcSlb2_c() {}

    void fn_61_58C0(int, int);
    bool fn_61_6A10();
    void fn_61_68D0(int);

private:
    /* 0x??? */ STATE_MGR_DECLARE(dAcNpcSlb2_c);
};

#endif
