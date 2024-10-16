#ifndef D_A_OBJ_SYAKO_SHUTTER_H
#define D_A_OBJ_SYAKO_SHUTTER_H

#include "d/a/obj/d_a_obj_base.h"
#include "s/s_State.hpp"
#include "s/s_StateMgr.hpp"

class dAcOSyakoShutter_c : public dAcObjBase_c {
public:
    dAcOSyakoShutter_c() : mStateMgr(*this, sStateID::null) {}
    virtual ~dAcOSyakoShutter_c() {}

    STATE_FUNC_DECLARE(dAcOSyakoShutter_c, Wait);
    STATE_FUNC_DECLARE(dAcOSyakoShutter_c, OpenEvent);
    STATE_FUNC_DECLARE(dAcOSyakoShutter_c, End);

private:
    /* 0x??? */ STATE_MGR_DECLARE(dAcOSyakoShutter_c);
};

#endif
