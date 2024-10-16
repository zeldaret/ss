#ifndef D_A_OBJ_PAINT_H
#define D_A_OBJ_PAINT_H

#include "d/a/obj/d_a_obj_base.h"
#include "s/s_State.hpp"
#include "s/s_StateMgr.hpp"

class dAcOpaint_c : public dAcObjBase_c {
public:
    dAcOpaint_c() : mStateMgr(*this, sStateID::null) {}
    virtual ~dAcOpaint_c() {}

    STATE_FUNC_DECLARE(dAcOpaint_c, Wait);

private:
    /* 0x??? */ STATE_MGR_DECLARE(dAcOpaint_c);
};

#endif
