#ifndef D_A_OBJ_TIME_BLOCK_H
#define D_A_OBJ_TIME_BLOCK_H

#include "d/a/obj/d_a_obj_base.h"
#include "s/s_State.hpp"

class dAcOTimeBlock_c : public dAcObjBase_c {
public:
    dAcOTimeBlock_c() : mStateMgr(*this) {}
    virtual ~dAcOTimeBlock_c() {}

    STATE_FUNC_DECLARE(dAcOTimeBlock_c, Wait);
    STATE_FUNC_DECLARE(dAcOTimeBlock_c, TimeSlip);

private:
    /* 0x??? */ STATE_MGR_DECLARE(dAcOTimeBlock_c);
};

#endif
