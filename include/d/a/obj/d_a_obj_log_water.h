#ifndef D_A_OBJ_LOG_WATER_H
#define D_A_OBJ_LOG_WATER_H

#include "d/a/obj/d_a_obj_base.h"
#include "s/s_State.hpp"

class dAcOlogWater_c : public dAcObjBase_c {
public:
    dAcOlogWater_c() : mStateMgr(*this) {}
    virtual ~dAcOlogWater_c() {}

    STATE_FUNC_DECLARE(dAcOlogWater_c, Wait);

private:
    /* 0x??? */ STATE_MGR_DECLARE(dAcOlogWater_c);
};

#endif
