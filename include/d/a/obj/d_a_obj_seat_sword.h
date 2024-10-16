#ifndef D_A_OBJ_SEAT_SWORD_H
#define D_A_OBJ_SEAT_SWORD_H

#include "d/a/obj/d_a_obj_base.h"
#include "s/s_State.hpp"
#include "s/s_StateMgr.hpp"

class dAcOSeatSword_c : public dAcObjBase_c {
public:
    dAcOSeatSword_c() : mStateMgr(*this, sStateID::null) {}
    virtual ~dAcOSeatSword_c() {}

    STATE_FUNC_DECLARE(dAcOSeatSword_c, Wait);
    STATE_FUNC_DECLARE(dAcOSeatSword_c, Get);

private:
    /* 0x??? */ STATE_MGR_DECLARE(dAcOSeatSword_c);
};

#endif
