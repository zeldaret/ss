#ifndef D_A_OBJ_TRAP_BIRD_WOOD_H
#define D_A_OBJ_TRAP_BIRD_WOOD_H

#include "d/a/obj/d_a_obj_base.h"
#include "s/s_State.hpp"
#include "s/s_StateMgr.hpp"

class dAcOTrapBirdWood_c : public dAcObjBase_c {
public:
    dAcOTrapBirdWood_c() : mStateMgr(*this, sStateID::null) {}
    virtual ~dAcOTrapBirdWood_c() {}

    STATE_FUNC_DECLARE(dAcOTrapBirdWood_c, Wait);
    STATE_FUNC_DECLARE(dAcOTrapBirdWood_c, Broken);

private:
    /* 0x??? */ STATE_MGR_DECLARE(dAcOTrapBirdWood_c);
};

#endif
