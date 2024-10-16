#ifndef D_A_OBJ_CLAW_SHOT_TG_H
#define D_A_OBJ_CLAW_SHOT_TG_H

#include "d/a/obj/d_a_obj_base.h"
#include "s/s_State.hpp"
#include "s/s_StateMgr.hpp"

class dAcOclawShotTg_c : public dAcObjBase_c {
public:
    dAcOclawShotTg_c() : mStateMgr(*this, sStateID::null) {}
    virtual ~dAcOclawShotTg_c() {}

    STATE_FUNC_DECLARE(dAcOclawShotTg_c, Wait);

private:
    /* 0x??? */ STATE_MGR_DECLARE(dAcOclawShotTg_c);
};

#endif
