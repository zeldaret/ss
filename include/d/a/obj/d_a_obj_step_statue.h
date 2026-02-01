#ifndef D_A_OBJ_STEP_STATUE_H
#define D_A_OBJ_STEP_STATUE_H

#include "d/a/obj/d_a_obj_base.h"
#include "s/s_State.hpp"

class dAcOStepStatue_c : public dAcObjBase_c {
public:
    dAcOStepStatue_c() : mStateMgr(*this) {}
    virtual ~dAcOStepStatue_c() {}

    STATE_FUNC_DECLARE(dAcOStepStatue_c, Wait);
    STATE_FUNC_DECLARE(dAcOStepStatue_c, CarryBomb);
    STATE_FUNC_DECLARE(dAcOStepStatue_c, BreakDown);
    STATE_FUNC_DECLARE(dAcOStepStatue_c, SandSink);
    STATE_FUNC_DECLARE(dAcOStepStatue_c, End);

private:
    /* 0x??? */ STATE_MGR_DECLARE(dAcOStepStatue_c);
};

#endif
