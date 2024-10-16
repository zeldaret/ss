#ifndef D_A_OBJ_PINWHEEL_H
#define D_A_OBJ_PINWHEEL_H

#include <d/a/obj/d_a_obj_base.h>
#include <s/s_State.hpp>
#include <s/s_StateMgr.hpp>

class dAcOpinwheel_c : public dAcObjBase_c {
public:
    dAcOpinwheel_c() : mStateMgr(*this, sStateID::null) {}
    virtual ~dAcOpinwheel_c() {}

    STATE_FUNC_DECLARE(dAcOpinwheel_c, Wait);
    STATE_FUNC_DECLARE(dAcOpinwheel_c, Acceleration);
    STATE_FUNC_DECLARE(dAcOpinwheel_c, RollMaxSpeed);
    STATE_FUNC_DECLARE(dAcOpinwheel_c, Deceleration);

private:
    /* 0x??? */ STATE_MGR_DECLARE(dAcOpinwheel_c);
};

#endif
