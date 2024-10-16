#ifndef D_A_OBJ_BRIDGE_STRETCH_H
#define D_A_OBJ_BRIDGE_STRETCH_H

#include "d/a/obj/d_a_obj_base.h"
#include "s/s_State.hpp"
#include "s/s_StateMgr.hpp"

class dAcObridgeStretch_c : public dAcObjBase_c {
public:
    dAcObridgeStretch_c() : mStateMgr(*this, sStateID::null) {}
    virtual ~dAcObridgeStretch_c() {}

    STATE_FUNC_DECLARE(dAcObridgeStretch_c, Wait);
    STATE_FUNC_DECLARE(dAcObridgeStretch_c, Move);
    STATE_FUNC_DECLARE(dAcObridgeStretch_c, Shake);
    STATE_FUNC_DECLARE(dAcObridgeStretch_c, Stretch);

private:
    /* 0x??? */ STATE_MGR_DECLARE(dAcObridgeStretch_c);
};

#endif
