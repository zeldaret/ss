#ifndef D_A_OBJ_WINDMILL_H
#define D_A_OBJ_WINDMILL_H

#include "d/a/obj/d_a_obj_base.h"
#include "s/s_State.hpp"
#include "s/s_StateMgr.hpp"

class dAcOwindmill_c : public dAcObjBase_c {
public:
    dAcOwindmill_c() : mStateMgr(*this, sStateID::null) {}
    virtual ~dAcOwindmill_c() {}

    STATE_FUNC_DECLARE(dAcOwindmill_c, Wait);
    STATE_FUNC_DECLARE(dAcOwindmill_c, CameraMove);
    STATE_FUNC_DECLARE(dAcOwindmill_c, Move);
    STATE_FUNC_DECLARE(dAcOwindmill_c, Collect);
    STATE_FUNC_DECLARE(dAcOwindmill_c, CollectStick);

private:
    /* 0x??? */ STATE_MGR_DECLARE(dAcOwindmill_c);
};

#endif
