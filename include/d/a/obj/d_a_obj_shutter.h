#ifndef D_A_OBJ_SHUTTER_H
#define D_A_OBJ_SHUTTER_H

#include "d/a/obj/d_a_obj_door_base.h"
#include "s/s_State.hpp"

class dAcOShutter_c : public dAcObjDoor_c {
public:
    dAcOShutter_c() : mStateMgr(*this) {}
    virtual ~dAcOShutter_c() {}

    virtual bool canOpen() override;

    STATE_FUNC_DECLARE(dAcOShutter_c, Wait);
    STATE_FUNC_DECLARE(dAcOShutter_c, Open);
    STATE_FUNC_DECLARE(dAcOShutter_c, Close);

private:
    /* 0x??? */ STATE_MGR_DECLARE(dAcOShutter_c);
};

#endif
