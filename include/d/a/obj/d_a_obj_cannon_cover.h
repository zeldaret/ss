#ifndef D_A_OBJ_CANNON_COVER_H
#define D_A_OBJ_CANNON_COVER_H

#include "d/a/obj/d_a_obj_base.h"
#include "s/s_State.hpp"
#include "s/s_StateMgr.hpp"

class dAcOcannonCover_c : public dAcObjBase_c {
public:
    dAcOcannonCover_c();
    virtual ~dAcOcannonCover_c();

    STATE_FUNC_DECLARE(dAcOcannonCover_c, Wait);
    STATE_FUNC_DECLARE(dAcOcannonCover_c, Open);
    STATE_FUNC_DECLARE(dAcOcannonCover_c, Shake);
    STATE_FUNC_DECLARE(dAcOcannonCover_c, Close);
    STATE_FUNC_DECLARE(dAcOcannonCover_c, Play);

private:
    /* 0x??? */ STATE_MGR_DECLARE(dAcOcannonCover_c);
};

#endif
