#ifndef D_A_OBJ_TUBO_H
#define D_A_OBJ_TUBO_H

#include "d/a/obj/d_a_obj_base.h"
#include "s/s_State.hpp"
#include "s/s_StateMgr.hpp"

class dAcOtubo_c : public dAcObjBase_c {
public:
    dAcOtubo_c() : mStateMgr(*this, sStateID::null) {}
    virtual ~dAcOtubo_c() {}

    STATE_FUNC_DECLARE(dAcOtubo_c, Wait);
    STATE_FUNC_DECLARE(dAcOtubo_c, Grab);
    STATE_FUNC_DECLARE(dAcOtubo_c, Put);
    STATE_FUNC_DECLARE(dAcOtubo_c, Slope);
    STATE_FUNC_DECLARE(dAcOtubo_c, Rebirth);

private:
    /* 0x??? */ STATE_MGR_DECLARE(dAcOtubo_c);
};

#endif
