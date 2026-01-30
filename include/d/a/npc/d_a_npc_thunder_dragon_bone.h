#ifndef D_A_NPC_THUNDER_DRAGON_BONE_H
#define D_A_NPC_THUNDER_DRAGON_BONE_H

#include "d/a/obj/d_a_obj_base.h"
#include "s/s_State.hpp"

class dAcNpcTdrB_c : public dAcObjBase_c {
public:
    dAcNpcTdrB_c() : mStateMgr(*this) {}
    virtual ~dAcNpcTdrB_c() {}

    STATE_FUNC_DECLARE(dAcNpcTdrB_c, Wait);

private:
    /* 0x??? */ STATE_MGR_DECLARE(dAcNpcTdrB_c);
};

#endif
