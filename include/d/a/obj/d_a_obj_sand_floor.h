#ifndef D_A_OBJ_SAND_FLOOR_H
#define D_A_OBJ_SAND_FLOOR_H

#include "d/a/obj/d_a_obj_base.h"
#include "s/s_State.hpp"

class dAcOSandFloor_c : public dAcObjBase_c {
public:
    dAcOSandFloor_c() : mStateMgr(*this) {}
    virtual ~dAcOSandFloor_c() {}

    STATE_FUNC_DECLARE(dAcOSandFloor_c, Wait);

private:
    /* 0x??? */ STATE_MGR_DECLARE(dAcOSandFloor_c);
};

#endif
