#ifndef D_A_OBJ_UPDOWN_LAVA_H
#define D_A_OBJ_UPDOWN_LAVA_H

#include "d/a/obj/d_a_obj_base.h"
#include "s/s_State.hpp"
#include "s/s_StateMgr.hpp"

class dAcOupdownLava_c : public dAcObjBase_c {
public:
    dAcOupdownLava_c() : mStateMgr(*this, sStateID::null) {}
    virtual ~dAcOupdownLava_c() {}

    STATE_FUNC_DECLARE(dAcOupdownLava_c, Wait);
    STATE_FUNC_DECLARE(dAcOupdownLava_c, UpDown);
    STATE_FUNC_DECLARE(dAcOupdownLava_c, Stop);
    STATE_FUNC_DECLARE(dAcOupdownLava_c, Up);
    STATE_FUNC_DECLARE(dAcOupdownLava_c, Sink);
    STATE_FUNC_DECLARE(dAcOupdownLava_c, Down);

private:
    /* 0x??? */ STATE_MGR_DECLARE(dAcOupdownLava_c);
};

#endif
