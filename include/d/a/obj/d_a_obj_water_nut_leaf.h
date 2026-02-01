#ifndef D_A_OBJ_WATER_NUT_LEAF_H
#define D_A_OBJ_WATER_NUT_LEAF_H

#include "d/a/obj/d_a_obj_base.h"
#include "s/s_State.hpp"

class dAcOWaterNutLeaf_c : public dAcObjBase_c {
public:
    dAcOWaterNutLeaf_c() : mStateMgr(*this) {}
    virtual ~dAcOWaterNutLeaf_c() {}

    STATE_FUNC_DECLARE(dAcOWaterNutLeaf_c, Wait);
    STATE_FUNC_DECLARE(dAcOWaterNutLeaf_c, WaitBirth);

private:
    /* 0x??? */ STATE_MGR_DECLARE(dAcOWaterNutLeaf_c);
};

#endif
