#ifndef D_A_OBJ_SW_SWORD_BEAM_H
#define D_A_OBJ_SW_SWORD_BEAM_H

#include "d/a/obj/d_a_obj_base.h"
#include "s/s_State.hpp"

class dAcOSwSwordBeam_c : public dAcObjBase_c {
public:
    dAcOSwSwordBeam_c() : mStateMgr(*this) {}
    virtual ~dAcOSwSwordBeam_c() {}

    STATE_FUNC_DECLARE(dAcOSwSwordBeam_c, Wait);
    STATE_FUNC_DECLARE(dAcOSwSwordBeam_c, Rotate);
    STATE_FUNC_DECLARE(dAcOSwSwordBeam_c, RotateEnd);
    STATE_FUNC_DECLARE(dAcOSwSwordBeam_c, OnSwitch);
    STATE_FUNC_DECLARE(dAcOSwSwordBeam_c, End);

private:
    /* 0x??? */ STATE_MGR_DECLARE(dAcOSwSwordBeam_c);
};

#endif
