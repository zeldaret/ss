#ifndef D_A_OBJ_BB_OBJECTS_H
#define D_A_OBJ_BB_OBJECTS_H

#include "d/a/obj/d_a_obj_base.h"
#include "s/s_State.hpp"

class dAcObbObjects_c : public dAcObjBase_c {
public:
    dAcObbObjects_c() : mStateMgr(*this) {}
    virtual ~dAcObbObjects_c() {}

    STATE_FUNC_DECLARE(dAcObbObjects_c, Wait);

private:
    /* 0x??? */ STATE_MGR_DECLARE(dAcObbObjects_c);
};

#endif
