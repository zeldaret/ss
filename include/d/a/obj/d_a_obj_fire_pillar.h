#ifndef D_A_OBJ_FIRE_PILLAR_H
#define D_A_OBJ_FIRE_PILLAR_H

#include "d/a/obj/d_a_obj_base.h"
#include "s/s_State.hpp"

class dAcOfirePillar_c : public dAcObjBase_c {
public:
    dAcOfirePillar_c() : mStateMgr(*this) {}
    virtual ~dAcOfirePillar_c() {}

    STATE_FUNC_DECLARE(dAcOfirePillar_c, Wait);
    STATE_FUNC_DECLARE(dAcOfirePillar_c, WaterWait);
    STATE_FUNC_DECLARE(dAcOfirePillar_c, Vacuum);
    STATE_FUNC_DECLARE(dAcOfirePillar_c, Small);

private:
    /* 0x??? */ STATE_MGR_DECLARE(dAcOfirePillar_c);
};

#endif
