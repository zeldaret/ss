#ifndef D_A_OBJ_ELECTRIC_LIGHT_H
#define D_A_OBJ_ELECTRIC_LIGHT_H

#include "d/a/obj/d_a_obj_base.h"
#include "s/s_State.hpp"
#include "s/s_StateMgr.hpp"

class dAcOelectricLight_c : public dAcObjBase_c {
public:
    dAcOelectricLight_c() : mStateMgr(*this, sStateID::null) {}
    virtual ~dAcOelectricLight_c() {}

    STATE_FUNC_DECLARE(dAcOelectricLight_c, Wait);

private:
    /* 0x??? */ STATE_MGR_DECLARE(dAcOelectricLight_c);
};

#endif
