#ifndef D_A_OBJ_NEEDLE_DESERT_H
#define D_A_OBJ_NEEDLE_DESERT_H

#include "d/a/obj/d_a_obj_base.h"
#include "s/s_State.hpp"

class dAcONeedleDesert_c : public dAcObjBase_c {
public:
    dAcONeedleDesert_c() : mStateMgr(*this) {}
    virtual ~dAcONeedleDesert_c() {}

    STATE_FUNC_DECLARE(dAcONeedleDesert_c, On);
    STATE_FUNC_DECLARE(dAcONeedleDesert_c, Off);

private:
    /* 0x??? */ STATE_MGR_DECLARE(dAcONeedleDesert_c);
};

#endif
