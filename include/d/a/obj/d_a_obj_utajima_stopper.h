#ifndef D_A_OBJ_UTAJIMA_STOPPER_H
#define D_A_OBJ_UTAJIMA_STOPPER_H

#include "d/a/obj/d_a_obj_base.h"
#include "s/s_State.hpp"
#include "s/s_StateMgr.hpp"

class dAcOutajimaStopper_c : public dAcObjBase_c {
public:
    dAcOutajimaStopper_c() : mStateMgr(*this, sStateID::null) {}
    virtual ~dAcOutajimaStopper_c() {}

    STATE_FUNC_DECLARE(dAcOutajimaStopper_c, Down);
    STATE_FUNC_DECLARE(dAcOutajimaStopper_c, Rise);
    STATE_FUNC_DECLARE(dAcOutajimaStopper_c, Up);
    STATE_FUNC_DECLARE(dAcOutajimaStopper_c, Fall);
    STATE_FUNC_DECLARE(dAcOutajimaStopper_c, Wait);

private:
    /* 0x??? */ STATE_MGR_DECLARE(dAcOutajimaStopper_c);
};

#endif
