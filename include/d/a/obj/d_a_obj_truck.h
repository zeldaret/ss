#ifndef D_A_OBJ_TRUCK_H
#define D_A_OBJ_TRUCK_H

#include "d/a/obj/d_a_obj_base.h"
#include "s/s_State.hpp"

class dAcOtruck_c : public dAcObjBase_c {
public:
    dAcOtruck_c() : mStateMgr(*this) {}
    virtual ~dAcOtruck_c() {}

    STATE_FUNC_DECLARE(dAcOtruck_c, Wait);
    STATE_FUNC_DECLARE(dAcOtruck_c, Ready);
    STATE_FUNC_DECLARE(dAcOtruck_c, PathMove);
    STATE_FUNC_DECLARE(dAcOtruck_c, Sink);

private:
    /* 0x??? */ STATE_MGR_DECLARE(dAcOtruck_c);
};

#endif
