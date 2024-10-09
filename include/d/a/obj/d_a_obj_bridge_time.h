#ifndef D_A_OBJ_BRIDGE_TIME_H
#define D_A_OBJ_BRIDGE_TIME_H

#include <d/a/obj/d_a_obj_base.h>
#include <s/s_State.hpp>
#include <s/s_StateMgr.hpp>

class dAcOBridgeTime_c : public dAcObjBase_c {
public:
    dAcOBridgeTime_c() : mStateMgr(*this, sStateID::null) {}
    virtual ~dAcOBridgeTime_c() {}

    STATE_FUNC_DECLARE(dAcOBridgeTime_c, Wait);

private:
    /* 0x??? */ STATE_MGR_DECLARE(dAcOBridgeTime_c);
};

#endif
