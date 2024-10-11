#ifndef D_A_OBJ_SKULL_H
#define D_A_OBJ_SKULL_H

#include <d/a/obj/d_a_obj_base.h>
#include <s/s_State.hpp>
#include <s/s_StateMgr.hpp>

class dAcOSkull_c : public dAcObjBase_c {
public:
    dAcOSkull_c() : mStateMgr(*this, sStateID::null) {}
    virtual ~dAcOSkull_c() {}

    STATE_FUNC_DECLARE(dAcOSkull_c, NormalWait);
    STATE_FUNC_DECLARE(dAcOSkull_c, GrabCarry);
    STATE_FUNC_DECLARE(dAcOSkull_c, BoomerangCarry);
    STATE_FUNC_DECLARE(dAcOSkull_c, WhipCarry);
    STATE_FUNC_DECLARE(dAcOSkull_c, Dead);

private:
    /* 0x??? */ STATE_MGR_DECLARE(dAcOSkull_c);
};

#endif
