#ifndef D_A_OBJ_SEA_F301_H
#define D_A_OBJ_SEA_F301_H

#include "d/a/obj/d_a_obj_base.h"
#include "s/s_State.hpp"
#include "s/s_StateMgr.hpp"

class dAcOSeaF301_c : public dAcObjBase_c {
public:
    dAcOSeaF301_c() : mStateMgr(*this, sStateID::null) {}
    virtual ~dAcOSeaF301_c() {}

    STATE_FUNC_DECLARE(dAcOSeaF301_c, Wait);

private:
    /* 0x??? */ STATE_MGR_DECLARE(dAcOSeaF301_c);
};

#endif
