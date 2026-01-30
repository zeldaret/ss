#ifndef D_A_OBJ_RO_AT_TARGET_H
#define D_A_OBJ_RO_AT_TARGET_H

#include "d/a/obj/d_a_obj_base.h"
#include "s/s_State.hpp"

class dAcOroAtTarget_c : public dAcObjBase_c {
public:
    dAcOroAtTarget_c() : mStateMgr(*this) {}
    virtual ~dAcOroAtTarget_c() {}

    STATE_FUNC_DECLARE(dAcOroAtTarget_c, Wait);
    STATE_FUNC_DECLARE(dAcOroAtTarget_c, Play);
    STATE_FUNC_DECLARE(dAcOroAtTarget_c, Death);

private:
    /* 0x??? */ STATE_MGR_DECLARE(dAcOroAtTarget_c);
};

#endif
