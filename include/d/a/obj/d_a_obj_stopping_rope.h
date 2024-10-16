#ifndef D_A_OBJ_STOPPING_ROPE_H
#define D_A_OBJ_STOPPING_ROPE_H

#include <d/a/obj/d_a_obj_base.h>
#include <s/s_State.hpp>
#include <s/s_StateMgr.hpp>

class dAcOStoppingRope_c : public dAcObjBase_c {
public:
    dAcOStoppingRope_c() : mStateMgr(*this, sStateID::null) {}
    virtual ~dAcOStoppingRope_c() {}

    STATE_FUNC_DECLARE(dAcOStoppingRope_c, Wait);
    STATE_FUNC_DECLARE(dAcOStoppingRope_c, SwitchOn);

private:
    /* 0x??? */ STATE_MGR_DECLARE(dAcOStoppingRope_c);
};

#endif
