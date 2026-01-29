#ifndef D_A_NPC_RESCUE_H
#define D_A_NPC_RESCUE_H

#include "d/a/obj/d_a_obj_base.h"
#include "s/s_State.hpp"

class dAcNpcRescue_c : public dAcObjBase_c {
public:
    dAcNpcRescue_c() : mStateMgr(*this) {}
    virtual ~dAcNpcRescue_c() {}

    STATE_FUNC_DECLARE(dAcNpcRescue_c, Wait);

private:
    /* 0x??? */ STATE_MGR_DECLARE(dAcNpcRescue_c);
};

#endif
