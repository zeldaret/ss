#ifndef D_A_OBJ_TR_SHUTTER_CS_H
#define D_A_OBJ_TR_SHUTTER_CS_H

#include "d/a/obj/d_a_obj_door_base.h"
#include "s/s_State.hpp"
#include "s/s_StateMgr.hpp"

class dAcOTrlyShtrChgScn_c : public dAcObjDoor_c {
public:
    dAcOTrlyShtrChgScn_c() : mStateMgr(*this, sStateID::null) {}
    virtual ~dAcOTrlyShtrChgScn_c() {}

    virtual bool canOpen() override;

    STATE_FUNC_DECLARE(dAcOTrlyShtrChgScn_c, Wait);
    STATE_FUNC_DECLARE(dAcOTrlyShtrChgScn_c, OpenEvent);

private:
    /* 0x??? */ STATE_MGR_DECLARE(dAcOTrlyShtrChgScn_c);
};

#endif
