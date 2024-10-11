#ifndef D_A_PUMPKIN_H
#define D_A_PUMPKIN_H

#include <d/a/obj/d_a_obj_base.h>
#include <s/s_State.hpp>
#include <s/s_StateMgr.hpp>

class dAcPumpkin_c : public dAcObjBase_c {
public:
    dAcPumpkin_c() : mStateMgr(*this, sStateID::null) {}
    virtual ~dAcPumpkin_c() {}

    STATE_FUNC_DECLARE(dAcPumpkin_c, Stick);
    STATE_FUNC_DECLARE(dAcPumpkin_c, Wait);
    STATE_FUNC_DECLARE(dAcPumpkin_c, CarryGrab);
    STATE_FUNC_DECLARE(dAcPumpkin_c, StickSword);
    STATE_FUNC_DECLARE(dAcPumpkin_c, Throw);
    STATE_FUNC_DECLARE(dAcPumpkin_c, Roll);
    STATE_FUNC_DECLARE(dAcPumpkin_c, CarryBeetle);
    STATE_FUNC_DECLARE(dAcPumpkin_c, CarryWhip);

private:
    /* 0x??? */ STATE_MGR_DECLARE(dAcPumpkin_c);
};

#endif
