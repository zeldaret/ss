#ifndef D_A_NPC_BIRD_ZELDA_H
#define D_A_NPC_BIRD_ZELDA_H

#include "d/a/obj/d_a_obj_base.h"
#include "s/s_State.hpp"

class dAcNpcBdz_c : public dAcObjBase_c {
public:
    dAcNpcBdz_c() : mStateMgr(*this) {}
    virtual ~dAcNpcBdz_c() {}

    STATE_FUNC_DECLARE(dAcNpcBdz_c, Wait);
    STATE_FUNC_DECLARE(dAcNpcBdz_c, Demo);

private:
    /* 0x??? */ STATE_MGR_DECLARE(dAcNpcBdz_c);
};

#endif
