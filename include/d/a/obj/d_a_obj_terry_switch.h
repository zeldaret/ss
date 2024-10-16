#ifndef D_A_OBJ_TERRY_SWITCH_H
#define D_A_OBJ_TERRY_SWITCH_H

#include "d/a/obj/d_a_obj_base.h"
#include "s/s_State.hpp"
#include "s/s_StateMgr.hpp"

class dAcOTerrySwitch_c : public dAcObjBase_c {
public:
    dAcOTerrySwitch_c() : mStateMgr(*this, sStateID::null) {}
    virtual ~dAcOTerrySwitch_c() {}

    STATE_FUNC_DECLARE(dAcOTerrySwitch_c, Wait);

private:
    /* 0x??? */ STATE_MGR_DECLARE(dAcOTerrySwitch_c);
};

#endif
