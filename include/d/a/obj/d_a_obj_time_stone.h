#ifndef D_A_OBJ_TIME_STONE_H
#define D_A_OBJ_TIME_STONE_H

#include "d/a/obj/d_a_obj_base.h"
#include "s/s_State.hpp"
#include "s/s_StateMgr.hpp"

class dAcOTimeStone_c : public dAcObjBase_c {
public:
    dAcOTimeStone_c() : mStateMgr(*this, sStateID::null) {}
    virtual ~dAcOTimeStone_c() {}

    STATE_FUNC_DECLARE(dAcOTimeStone_c, Wait);
    STATE_FUNC_DECLARE(dAcOTimeStone_c, TimeSlip);
    STATE_FUNC_DECLARE(dAcOTimeStone_c, TimeSlipEnd);

private:
    /* 0x??? */ STATE_MGR_DECLARE(dAcOTimeStone_c);
};

#endif
