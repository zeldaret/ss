#ifndef D_A_OBJ_BRIDGE_STEP_H
#define D_A_OBJ_BRIDGE_STEP_H

#include "d/a/obj/d_a_obj_base.h"
#include "s/s_State.hpp"
#include "s/s_StateMgr.hpp"

class dAcOBridgeStep_c : public dAcObjBase_c {
public:
    dAcOBridgeStep_c() : mStateMgr(*this, sStateID::null) {}
    virtual ~dAcOBridgeStep_c() {}

    STATE_FUNC_DECLARE(dAcOBridgeStep_c, Wait);
    STATE_FUNC_DECLARE(dAcOBridgeStep_c, SwitchPushed);
    STATE_FUNC_DECLARE(dAcOBridgeStep_c, BridgeUp);
    STATE_FUNC_DECLARE(dAcOBridgeStep_c, BridgeUpAll);

private:
    /* 0x??? */ STATE_MGR_DECLARE(dAcOBridgeStep_c);
};

#endif
