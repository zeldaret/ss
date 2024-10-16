#ifndef D_A_OBJ_OCT_GRASS_H
#define D_A_OBJ_OCT_GRASS_H

#include "d/a/obj/d_a_obj_base.h"
#include "s/s_State.hpp"
#include "s/s_StateMgr.hpp"

class dAcOOctGrass_c : public dAcObjBase_c {
public:
    dAcOOctGrass_c() : mStateMgr(*this, sStateID::null) {}
    virtual ~dAcOOctGrass_c() {}

    STATE_FUNC_DECLARE(dAcOOctGrass_c, Wait);

private:
    /* 0x??? */ STATE_MGR_DECLARE(dAcOOctGrass_c);
};

#endif
