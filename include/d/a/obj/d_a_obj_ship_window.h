#ifndef D_A_OBJ_SHIP_WINDOW_H
#define D_A_OBJ_SHIP_WINDOW_H

#include "d/a/obj/d_a_obj_base.h"
#include "s/s_State.hpp"

class dAcOshipWindow_c : public dAcObjBase_c {
public:
    dAcOshipWindow_c() : mStateMgr(*this) {}
    virtual ~dAcOshipWindow_c() {}

    STATE_FUNC_DECLARE(dAcOshipWindow_c, Close);
    STATE_FUNC_DECLARE(dAcOshipWindow_c, Move);
    STATE_FUNC_DECLARE(dAcOshipWindow_c, Open);

private:
    /* 0x??? */ STATE_MGR_DECLARE(dAcOshipWindow_c);
};

#endif
