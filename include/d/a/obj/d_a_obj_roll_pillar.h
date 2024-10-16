#ifndef D_A_OBJ_ROLL_PILLAR_H
#define D_A_OBJ_ROLL_PILLAR_H

#include "d/a/obj/d_a_obj_base.h"
#include "s/s_State.hpp"
#include "s/s_StateMgr.hpp"

class dAcORollPillar_c : public dAcObjBase_c {
public:
    dAcORollPillar_c() : mStateMgr(*this, sStateID::null) {}
    virtual ~dAcORollPillar_c() {}

    STATE_FUNC_DECLARE(dAcORollPillar_c, Wait);
    STATE_FUNC_DECLARE(dAcORollPillar_c, Rotate);
    STATE_FUNC_DECLARE(dAcORollPillar_c, TimerWait);

private:
    /* 0x??? */ STATE_MGR_DECLARE(dAcORollPillar_c);
};

#endif
