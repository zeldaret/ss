#ifndef D_A_OBJ_SIDE_SHUTTER_H
#define D_A_OBJ_SIDE_SHUTTER_H

#include <d/a/obj/d_a_obj_base.h>
#include <s/s_State.hpp>
#include <s/s_StateMgr.hpp>

class dAcOsideShutter_c : public dAcObjBase_c {
public:
    dAcOsideShutter_c() : mStateMgr(*this, sStateID::null) {}
    virtual ~dAcOsideShutter_c() {}

    STATE_FUNC_DECLARE(dAcOsideShutter_c, Off);
    STATE_FUNC_DECLARE(dAcOsideShutter_c, On);
    STATE_FUNC_DECLARE(dAcOsideShutter_c, OffToOnWait);
    STATE_FUNC_DECLARE(dAcOsideShutter_c, OffToOn);
    STATE_FUNC_DECLARE(dAcOsideShutter_c, OnToOffWait);
    STATE_FUNC_DECLARE(dAcOsideShutter_c, OnToOff);

private:
    /* 0x??? */ STATE_MGR_DECLARE(dAcOsideShutter_c);
};

#endif
