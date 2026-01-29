#ifndef D_A_OBJ_FIREWALL_H
#define D_A_OBJ_FIREWALL_H

#include "d/a/obj/d_a_obj_base.h"
#include "s/s_State.hpp"

class dAcOFireWall_c : public dAcObjBase_c {
public:
    dAcOFireWall_c() : mStateMgr(*this) {}
    virtual ~dAcOFireWall_c() {}

    STATE_FUNC_DECLARE(dAcOFireWall_c, Wait);

private:
    /* 0x??? */ STATE_MGR_DECLARE(dAcOFireWall_c);
};

#endif
