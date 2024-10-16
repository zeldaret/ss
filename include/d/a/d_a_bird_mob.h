#ifndef D_A_BIRD_MOB_H
#define D_A_BIRD_MOB_H

#include "d/a/obj/d_a_obj_base.h"
#include "s/s_State.hpp"
#include "s/s_StateMgr.hpp"

class dAcBirdMob_c : public dAcObjBase_c {
public:
    dAcBirdMob_c() : mStateMgr(*this, sStateID::null) {}
    virtual ~dAcBirdMob_c() {}

    STATE_FUNC_DECLARE(dAcBirdMob_c, Wait);

private:
    /* 0x??? */ STATE_MGR_DECLARE(dAcBirdMob_c);
};

#endif
