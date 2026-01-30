#ifndef D_A_OBJ_SHUTTER_FENCE_H
#define D_A_OBJ_SHUTTER_FENCE_H

#include "d/a/obj/d_a_obj_base.h"
#include "s/s_State.hpp"

class dAcOshutterfence_c : public dAcObjBase_c {
public:
    dAcOshutterfence_c() : mStateMgr(*this) {}
    virtual ~dAcOshutterfence_c() {}

    STATE_FUNC_DECLARE(dAcOshutterfence_c, Wait);
    STATE_FUNC_DECLARE(dAcOshutterfence_c, Open);

private:
    /* 0x??? */ STATE_MGR_DECLARE(dAcOshutterfence_c);
};

#endif
