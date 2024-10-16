#ifndef D_A_OBJ_UTAJIMA_ISLAND_H
#define D_A_OBJ_UTAJIMA_ISLAND_H

#include "d/a/obj/d_a_obj_base.h"
#include "s/s_State.hpp"
#include "s/s_StateMgr.hpp"

class dAcOutajimaIsland_c : public dAcObjBase_c {
public:
    dAcOutajimaIsland_c() : mStateMgr(*this, sStateID::null) {}
    virtual ~dAcOutajimaIsland_c() {}

    STATE_FUNC_DECLARE(dAcOutajimaIsland_c, Wait);

private:
    /* 0x??? */ STATE_MGR_DECLARE(dAcOutajimaIsland_c);
};

#endif
