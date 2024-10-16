#ifndef D_A_OBJ_BBLARGEBOMB_H
#define D_A_OBJ_BBLARGEBOMB_H

#include "d/a/obj/d_a_obj_base.h"
#include "s/s_State.hpp"
#include "s/s_StateMgr.hpp"

class dAcOBblargebomb_c : public dAcObjBase_c {
public:
    dAcOBblargebomb_c() : mStateMgr(*this, sStateID::null) {}
    virtual ~dAcOBblargebomb_c() {}

    STATE_FUNC_DECLARE(dAcOBblargebomb_c, Wait);
    STATE_FUNC_DECLARE(dAcOBblargebomb_c, Explode);
    STATE_FUNC_DECLARE(dAcOBblargebomb_c, Carry);
    STATE_FUNC_DECLARE(dAcOBblargebomb_c, WindCarry);
    STATE_FUNC_DECLARE(dAcOBblargebomb_c, Birth);
    STATE_FUNC_DECLARE(dAcOBblargebomb_c, Controled);

private:
    /* 0x??? */ STATE_MGR_DECLARE(dAcOBblargebomb_c);
};

#endif
