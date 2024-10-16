#ifndef D_A_OBJ_LAMP_H
#define D_A_OBJ_LAMP_H

#include "d/a/obj/d_a_obj_base.h"
#include "s/s_State.hpp"
#include "s/s_StateMgr.hpp"

class dAcOLamp_c : public dAcObjBase_c {
public:
    dAcOLamp_c() : mStateMgr(*this, sStateID::null) {}
    virtual ~dAcOLamp_c() {}

    STATE_FUNC_DECLARE(dAcOLamp_c, Wait);

private:
    /* 0x??? */ STATE_MGR_DECLARE(dAcOLamp_c);
};

#endif
