#ifndef D_A_OBJ_LIGHT_SHAFT_SMALL_H
#define D_A_OBJ_LIGHT_SHAFT_SMALL_H

#include "d/a/obj/d_a_obj_base.h"
#include "s/s_State.hpp"
#include "s/s_StateMgr.hpp"

class dAcOLightShaftSmall_c : public dAcObjBase_c {
public:
    dAcOLightShaftSmall_c() : mStateMgr(*this, sStateID::null) {}
    virtual ~dAcOLightShaftSmall_c() {}

    STATE_FUNC_DECLARE(dAcOLightShaftSmall_c, Wait);
    STATE_FUNC_DECLARE(dAcOLightShaftSmall_c, Inside);

private:
    /* 0x??? */ STATE_MGR_DECLARE(dAcOLightShaftSmall_c);
};

#endif
