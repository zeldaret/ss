#ifndef D_A_OBJ_STOPPER_ROCK_H
#define D_A_OBJ_STOPPER_ROCK_H

#include <d/a/obj/d_a_obj_base.h>
#include <s/s_State.hpp>
#include <s/s_StateMgr.hpp>

class dAcOstopperRock_c : public dAcObjBase_c {
public:
    dAcOstopperRock_c() : mStateMgr(*this, sStateID::null) {}
    virtual ~dAcOstopperRock_c() {}

    STATE_FUNC_DECLARE(dAcOstopperRock_c, Wait);
    STATE_FUNC_DECLARE(dAcOstopperRock_c, Bomb);

private:
    /* 0x??? */ STATE_MGR_DECLARE(dAcOstopperRock_c);
};

#endif
