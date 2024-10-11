#ifndef D_A_OBJ_TERRY_ISLAND_H
#define D_A_OBJ_TERRY_ISLAND_H

#include <d/a/obj/d_a_obj_base.h>
#include <s/s_State.hpp>
#include <s/s_StateMgr.hpp>

class dAcOterryIsland_c : public dAcObjBase_c {
public:
    dAcOterryIsland_c() : mStateMgr(*this, sStateID::null) {}
    virtual ~dAcOterryIsland_c() {}

    STATE_FUNC_DECLARE(dAcOterryIsland_c, Wait);
    STATE_FUNC_DECLARE(dAcOterryIsland_c, Vanish);
    STATE_FUNC_DECLARE(dAcOterryIsland_c, Vacuum);
    STATE_FUNC_DECLARE(dAcOterryIsland_c, Smoke);

private:
    /* 0x??? */ STATE_MGR_DECLARE(dAcOterryIsland_c);
};

#endif
