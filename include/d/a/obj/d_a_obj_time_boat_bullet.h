#ifndef D_A_OBJ_TIME_BOAT_BULLET_H
#define D_A_OBJ_TIME_BOAT_BULLET_H

#include "d/a/obj/d_a_obj_base.h"
#include "s/s_State.hpp"
#include "s/s_StateMgr.hpp"

class dAcOTimeBoatBullet_c : public dAcObjBase_c {
public:
    dAcOTimeBoatBullet_c() : mStateMgr(*this, sStateID::null) {}
    virtual ~dAcOTimeBoatBullet_c() {}

    STATE_FUNC_DECLARE(dAcOTimeBoatBullet_c, Wait);
    STATE_FUNC_DECLARE(dAcOTimeBoatBullet_c, Explode);

private:
    /* 0x??? */ STATE_MGR_DECLARE(dAcOTimeBoatBullet_c);
};

#endif
