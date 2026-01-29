#ifndef D_A_OBJ_GEAR_H
#define D_A_OBJ_GEAR_H

#include "d/a/obj/d_a_obj_base.h"
#include "s/s_State.hpp"

class dAcOGear_c : public dAcObjBase_c {
public:
    dAcOGear_c() : mStateMgr(*this) {}
    virtual ~dAcOGear_c() {}

    STATE_FUNC_DECLARE(dAcOGear_c, Wait);
    STATE_FUNC_DECLARE(dAcOGear_c, Rotation);
    STATE_FUNC_DECLARE(dAcOGear_c, Deboost);
    STATE_FUNC_DECLARE(dAcOGear_c, Stop);

private:
    /* 0x??? */ STATE_MGR_DECLARE(dAcOGear_c);
};

#endif
