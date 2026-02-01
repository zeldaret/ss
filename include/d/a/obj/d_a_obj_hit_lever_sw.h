#ifndef D_A_OBJ_HIT_LEVER_SW_H
#define D_A_OBJ_HIT_LEVER_SW_H

#include "d/a/obj/d_a_obj_base.h"
#include "s/s_State.hpp"

class dAcOhitLeverSw_c : public dAcObjBase_c {
public:
    dAcOhitLeverSw_c() : mStateMgr(*this) {}
    virtual ~dAcOhitLeverSw_c() {}

    STATE_FUNC_DECLARE(dAcOhitLeverSw_c, Right);
    STATE_FUNC_DECLARE(dAcOhitLeverSw_c, Left);
    STATE_FUNC_DECLARE(dAcOhitLeverSw_c, Move);

private:
    /* 0x??? */ STATE_MGR_DECLARE(dAcOhitLeverSw_c);
};

#endif
