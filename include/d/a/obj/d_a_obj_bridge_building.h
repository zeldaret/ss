#ifndef D_A_OBJ_BRIDGE_BUILDING_H
#define D_A_OBJ_BRIDGE_BUILDING_H

#include "d/a/obj/d_a_obj_base.h"
#include "s/s_State.hpp"

class dAcObridgeBuilding_c : public dAcObjBase_c {
public:
    dAcObridgeBuilding_c() : mStateMgr(*this) {}
    virtual ~dAcObridgeBuilding_c() {}

    STATE_FUNC_DECLARE(dAcObridgeBuilding_c, Wait);
    STATE_FUNC_DECLARE(dAcObridgeBuilding_c, PreMove);
    STATE_FUNC_DECLARE(dAcObridgeBuilding_c, Move);
    STATE_FUNC_DECLARE(dAcObridgeBuilding_c, Shake);
    STATE_FUNC_DECLARE(dAcObridgeBuilding_c, Across);

private:
    /* 0x??? */ STATE_MGR_DECLARE(dAcObridgeBuilding_c);
};

#endif
