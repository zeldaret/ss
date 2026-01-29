#ifndef D_A_OBJ_FIRE_OBSTACLE_H
#define D_A_OBJ_FIRE_OBSTACLE_H

#include "d/a/obj/d_a_obj_base.h"
#include "s/s_State.hpp"

class dAcOfireObstacle_c : public dAcObjBase_c {
public:
    dAcOfireObstacle_c() : mStateMgr(*this) {}
    virtual ~dAcOfireObstacle_c() {}

    STATE_FUNC_DECLARE(dAcOfireObstacle_c, Wait);
    STATE_FUNC_DECLARE(dAcOfireObstacle_c, Vanish);

private:
    /* 0x??? */ STATE_MGR_DECLARE(dAcOfireObstacle_c);
};

#endif
