#ifndef D_A_OBJ_WATER_NUT_H
#define D_A_OBJ_WATER_NUT_H

#include "d/a/obj/d_a_obj_base.h"
#include "s/s_State.hpp"

class dAcOWaterNut_c : public dAcObjBase_c {
public:
    dAcOWaterNut_c() : mStateMgr(*this) {}
    virtual ~dAcOWaterNut_c() {}

    STATE_FUNC_DECLARE(dAcOWaterNut_c, Wait);
    STATE_FUNC_DECLARE(dAcOWaterNut_c, SwordCarry);
    STATE_FUNC_DECLARE(dAcOWaterNut_c, Throw);
    STATE_FUNC_DECLARE(dAcOWaterNut_c, Drop);
    STATE_FUNC_DECLARE(dAcOWaterNut_c, Explode);
    STATE_FUNC_DECLARE(dAcOWaterNut_c, Birth);
    STATE_FUNC_DECLARE(dAcOWaterNut_c, Peel);

private:
    /* 0x??? */ STATE_MGR_DECLARE(dAcOWaterNut_c);
};

#endif
