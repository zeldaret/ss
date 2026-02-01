#ifndef D_A_OBJ_BALLISTA_H
#define D_A_OBJ_BALLISTA_H

#include "d/a/obj/d_a_obj_base.h"
#include "s/s_State.hpp"

class dAcOBallista_c : public dAcObjBase_c {
public:
    dAcOBallista_c() : mStateMgr(*this) {}
    virtual ~dAcOBallista_c() {}

    STATE_FUNC_DECLARE(dAcOBallista_c, Wait);
    STATE_FUNC_DECLARE(dAcOBallista_c, Grab);
    STATE_FUNC_DECLARE(dAcOBallista_c, Shot);
    STATE_FUNC_DECLARE(dAcOBallista_c, ShotEnd);
    STATE_FUNC_DECLARE(dAcOBallista_c, Cancel);
    STATE_FUNC_DECLARE(dAcOBallista_c, DemoShot);
    STATE_FUNC_DECLARE(dAcOBallista_c, DemoShot2);
    STATE_FUNC_DECLARE(dAcOBallista_c, DemoShot3);

private:
    /* 0x??? */ STATE_MGR_DECLARE(dAcOBallista_c);
};

#endif
