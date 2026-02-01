#ifndef D_A_OBJ_STAGE_WATER_H
#define D_A_OBJ_STAGE_WATER_H

#include "d/a/obj/d_a_obj_base.h"
#include "s/s_State.hpp"

class dAcOstageWater_c : public dAcObjBase_c {
public:
    dAcOstageWater_c() : mStateMgr(*this) {}
    virtual ~dAcOstageWater_c() {}

    STATE_FUNC_DECLARE(dAcOstageWater_c, Wait);
    STATE_FUNC_DECLARE(dAcOstageWater_c, Move);

private:
    /* 0x??? */ STATE_MGR_DECLARE(dAcOstageWater_c);
};

#endif
