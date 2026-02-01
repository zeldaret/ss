#ifndef D_A_OBJ_LAVA_PLATE_H
#define D_A_OBJ_LAVA_PLATE_H

#include "d/a/obj/d_a_obj_base.h"
#include "s/s_State.hpp"

class dAcOLavaPlate_c : public dAcObjBase_c {
public:
    dAcOLavaPlate_c() : mStateMgr(*this) {}
    virtual ~dAcOLavaPlate_c() {}

    STATE_FUNC_DECLARE(dAcOLavaPlate_c, Wait);
    STATE_FUNC_DECLARE(dAcOLavaPlate_c, Move);
    STATE_FUNC_DECLARE(dAcOLavaPlate_c, Success);

private:
    /* 0x??? */ STATE_MGR_DECLARE(dAcOLavaPlate_c);
};

#endif
