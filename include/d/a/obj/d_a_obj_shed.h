#ifndef D_A_OBJ_SHED_H
#define D_A_OBJ_SHED_H

#include "d/a/obj/d_a_obj_base.h"
#include "s/s_State.hpp"
#include "s/s_StateMgr.hpp"

class dAcOShed_c : public dAcObjBase_c {
public:
    dAcOShed_c() : mStateMgr(*this, sStateID::null) {}
    virtual ~dAcOShed_c() {}

    STATE_FUNC_DECLARE(dAcOShed_c, Wait);
    STATE_FUNC_DECLARE(dAcOShed_c, Move);

private:
    /* 0x??? */ STATE_MGR_DECLARE(dAcOShed_c);
};

#endif
