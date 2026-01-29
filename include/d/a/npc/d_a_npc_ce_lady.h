#ifndef D_A_NPC_CE_LADY_H
#define D_A_NPC_CE_LADY_H

#include "d/a/npc/d_a_npc.h"
#include "d/col/c/c_cc_d.h"
#include "s/s_State.hpp"

class dAcNpcCeLady_c : public dAcNpc_c {
public:
    dAcNpcCeLady_c() : mStateMgr(*this) {}
    virtual ~dAcNpcCeLady_c() {}

    bool fn_12_1C20(dAcObjBase_c *);
    void fn_12_1E00(dAcObjBase_c *, bool *);
    void fn_12_1E80(dAcObjBase_c *, bool *);

private:
    /* 0x??? */ STATE_MGR_DECLARE(dAcNpcCeLady_c);
};

#endif
