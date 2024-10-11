#ifndef D_A_OBJ_SINK_FLOOR_F_H
#define D_A_OBJ_SINK_FLOOR_F_H

#include <d/a/obj/d_a_obj_base.h>
#include <s/s_State.hpp>
#include <s/s_StateMgr.hpp>

class dAcOSinkFloorF_c : public dAcObjBase_c {
public:
    dAcOSinkFloorF_c() : mStateMgr(*this, sStateID::null) {}
    virtual ~dAcOSinkFloorF_c() {}

    STATE_FUNC_DECLARE(dAcOSinkFloorF_c, Wait);
    STATE_FUNC_DECLARE(dAcOSinkFloorF_c, Ride);
    STATE_FUNC_DECLARE(dAcOSinkFloorF_c, Sink);

private:
    /* 0x??? */ STATE_MGR_DECLARE(dAcOSinkFloorF_c);
};

#endif
