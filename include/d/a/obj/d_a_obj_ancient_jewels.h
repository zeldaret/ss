#ifndef D_A_OBJ_ANCIENT_JEWELS_H
#define D_A_OBJ_ANCIENT_JEWELS_H

#include "d/a/obj/d_a_obj_base.h"
#include "s/s_State.hpp"

class dAcOAncientJewels_c : public dAcObjBase_c {
public:
    dAcOAncientJewels_c() : mStateMgr(*this) {}
    virtual ~dAcOAncientJewels_c() {}

    STATE_FUNC_DECLARE(dAcOAncientJewels_c, Wait);
    STATE_FUNC_DECLARE(dAcOAncientJewels_c, WaitDrop);
    STATE_FUNC_DECLARE(dAcOAncientJewels_c, Drop);
    STATE_FUNC_DECLARE(dAcOAncientJewels_c, Buried);
    STATE_FUNC_DECLARE(dAcOAncientJewels_c, WhipCarry);
    STATE_FUNC_DECLARE(dAcOAncientJewels_c, GetItem);
    STATE_FUNC_DECLARE(dAcOAncientJewels_c, UnderLog);

private:
    /* 0x??? */ STATE_MGR_DECLARE(dAcOAncientJewels_c);
};

#endif
