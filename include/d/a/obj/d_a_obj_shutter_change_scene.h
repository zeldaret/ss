#ifndef D_A_OBJ_SHUTTER_CHANGE_SCENE_H
#define D_A_OBJ_SHUTTER_CHANGE_SCENE_H

#include "d/a/obj/d_a_obj_door_base.h"
#include "s/s_State.hpp"
#include "s/s_StateMgr.hpp"

class dAcOShutterChangeScene_c : public dAcObjDoor_c {
public:
    dAcOShutterChangeScene_c() : mStateMgr(*this, sStateID::null) {}
    virtual ~dAcOShutterChangeScene_c() {}

    STATE_FUNC_DECLARE(dAcOShutterChangeScene_c, WaitOpenShutter);
    STATE_FUNC_DECLARE(dAcOShutterChangeScene_c, WaitOpenShutterForEvent);
    STATE_FUNC_DECLARE(dAcOShutterChangeScene_c, OpenShutter);
    STATE_FUNC_DECLARE(dAcOShutterChangeScene_c, WaitCloseShutter);
    STATE_FUNC_DECLARE(dAcOShutterChangeScene_c, CloseShutter);
    STATE_FUNC_DECLARE(dAcOShutterChangeScene_c, WaitOpenFence);
    STATE_FUNC_DECLARE(dAcOShutterChangeScene_c, OpenFence);
    STATE_FUNC_DECLARE(dAcOShutterChangeScene_c, CloseFence);
    STATE_FUNC_DECLARE(dAcOShutterChangeScene_c, ObserveSwEye);

private:
    /* 0x??? */ STATE_MGR_DECLARE(dAcOShutterChangeScene_c);
};

#endif
