#ifndef D_A_OBJ_BRIDGE_FALL_H
#define D_A_OBJ_BRIDGE_FALL_H

#include "d/a/obj/d_a_obj_base.h"
#include "s/s_State.hpp"

class dAcOBridgeFall_c : public dAcObjBase_c {
public:
    dAcOBridgeFall_c() : mStateMgr(*this) {}
    virtual ~dAcOBridgeFall_c() {}

    STATE_FUNC_DECLARE(dAcOBridgeFall_c, Wait);
    STATE_FUNC_DECLARE(dAcOBridgeFall_c, Fall);
    STATE_FUNC_DECLARE(dAcOBridgeFall_c, Shake);

private:
    /* 0x??? */ STATE_MGR_DECLARE(dAcOBridgeFall_c);
};

#endif
