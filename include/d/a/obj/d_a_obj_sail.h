#ifndef D_A_OBJ_SAIL_H
#define D_A_OBJ_SAIL_H

#include "d/a/obj/d_a_obj_base.h"
#include "s/s_State.hpp"

class dAcOSail_c : public dAcObjBase_c {
public:
    dAcOSail_c() : mStateMgr(*this) {}
    virtual ~dAcOSail_c() {}

    STATE_FUNC_DECLARE(dAcOSail_c, Wait);

private:
    /* 0x??? */ STATE_MGR_DECLARE(dAcOSail_c);
};

#endif
