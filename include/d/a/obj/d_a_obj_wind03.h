#ifndef D_A_OBJ_WIND03_H
#define D_A_OBJ_WIND03_H

#include "d/a/obj/d_a_obj_base.h"
#include "s/s_State.hpp"

class dAcOwind03_c : public dAcObjBase_c {
public:
    dAcOwind03_c() : mStateMgr(*this) {}
    virtual ~dAcOwind03_c() {}

    STATE_FUNC_DECLARE(dAcOwind03_c, Wait);

private:
    /* 0x??? */ STATE_MGR_DECLARE(dAcOwind03_c);
};

#endif
