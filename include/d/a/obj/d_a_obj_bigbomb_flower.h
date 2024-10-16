#ifndef D_A_OBJ_BIGBOMB_FLOWER_H
#define D_A_OBJ_BIGBOMB_FLOWER_H

#include "d/a/obj/d_a_obj_base.h"
#include "s/s_State.hpp"
#include "s/s_StateMgr.hpp"

class dAcOBBombFlower_c : public dAcObjBase_c {
public:
    dAcOBBombFlower_c() : mStateMgr(*this, sStateID::null) {}
    virtual ~dAcOBBombFlower_c() {}

    STATE_FUNC_DECLARE(dAcOBBombFlower_c, Wait);

private:
    /* 0x??? */ STATE_MGR_DECLARE(dAcOBBombFlower_c);
};

#endif
