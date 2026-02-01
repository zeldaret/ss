#ifndef D_A_OBJ_D300_H
#define D_A_OBJ_D300_H

#include "d/a/obj/d_a_obj_base.h"
#include "s/s_State.hpp"

class dAcOD300_c : public dAcObjBase_c {
public:
    dAcOD300_c() : mStateMgr(*this) {}
    virtual ~dAcOD300_c() {}

    STATE_FUNC_DECLARE(dAcOD300_c, Wait);
    STATE_FUNC_DECLARE(dAcOD300_c, Appear);
    STATE_FUNC_DECLARE(dAcOD300_c, End);

private:
    /* 0x??? */ STATE_MGR_DECLARE(dAcOD300_c);
};

#endif
