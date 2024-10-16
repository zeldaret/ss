#ifndef D_A_FLY_SLB_H
#define D_A_FLY_SLB_H

#include <d/a/obj/d_a_obj_base.h>
#include <s/s_State.hpp>
#include <s/s_StateMgr.hpp>

class dAcFlySlb_c : public dAcObjBase_c {
public:
    dAcFlySlb_c() : mStateMgr(*this, sStateID::null) {}
    virtual ~dAcFlySlb_c() {}

    STATE_FUNC_DECLARE(dAcFlySlb_c, Fly);

private:
    /* 0x??? */ STATE_MGR_DECLARE(dAcFlySlb_c);
};

#endif
