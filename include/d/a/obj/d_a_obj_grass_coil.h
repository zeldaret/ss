#ifndef D_A_OBJ_GRASS_COIL_H
#define D_A_OBJ_GRASS_COIL_H

#include "d/a/obj/d_a_obj_base.h"
#include "s/s_State.hpp"
#include "s/s_StateMgr.hpp"

class dAcOgrassCoil_c : public dAcObjBase_c {
public:
    dAcOgrassCoil_c() : mStateMgr(*this, sStateID::null) {}
    virtual ~dAcOgrassCoil_c() {}

    STATE_FUNC_DECLARE(dAcOgrassCoil_c, Wait);

private:
    /* 0x??? */ STATE_MGR_DECLARE(dAcOgrassCoil_c);
};

#endif
