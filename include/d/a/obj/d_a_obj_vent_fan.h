#ifndef D_A_OBJ_VENT_FAN_H
#define D_A_OBJ_VENT_FAN_H

#include "d/a/obj/d_a_obj_base.h"
#include "s/s_State.hpp"

class dAcOventFan_c : public dAcObjBase_c {
public:
    dAcOventFan_c() : mStateMgr(*this) {}
    virtual ~dAcOventFan_c() {}

    STATE_FUNC_DECLARE(dAcOventFan_c, Normal);
    STATE_FUNC_DECLARE(dAcOventFan_c, Reverse);
    STATE_FUNC_DECLARE(dAcOventFan_c, Vacuum);

private:
    /* 0x??? */ STATE_MGR_DECLARE(dAcOventFan_c);
};

#endif
