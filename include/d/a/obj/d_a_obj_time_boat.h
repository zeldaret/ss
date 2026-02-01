#ifndef D_A_OBJ_TIME_BOAT_H
#define D_A_OBJ_TIME_BOAT_H

#include "d/a/obj/d_a_obj_base.h"
#include "s/s_State.hpp"

class dAcOTimeBoat_c : public dAcObjBase_c {
public:
    dAcOTimeBoat_c() : mStateMgr(*this) {}
    virtual ~dAcOTimeBoat_c() {}

    STATE_FUNC_DECLARE(dAcOTimeBoat_c, Wait);
    STATE_FUNC_DECLARE(dAcOTimeBoat_c, StartTimeSlip);
    STATE_FUNC_DECLARE(dAcOTimeBoat_c, TSWait);
    STATE_FUNC_DECLARE(dAcOTimeBoat_c, TSRide);

private:
    /* 0x??? */ STATE_MGR_DECLARE(dAcOTimeBoat_c);
};

#endif
