#ifndef D_A_OBJ_WATERFALL_D101_H
#define D_A_OBJ_WATERFALL_D101_H

#include "d/a/obj/d_a_obj_base.h"
#include "s/s_State.hpp"
#include "s/s_StateMgr.hpp"

class dAcOWaterFallD101_c : public dAcObjBase_c {
public:
    dAcOWaterFallD101_c() : mStateMgr(*this, sStateID::null) {}
    virtual ~dAcOWaterFallD101_c() {}

    STATE_FUNC_DECLARE(dAcOWaterFallD101_c, Wait);

private:
    /* 0x??? */ STATE_MGR_DECLARE(dAcOWaterFallD101_c);
};

#endif
