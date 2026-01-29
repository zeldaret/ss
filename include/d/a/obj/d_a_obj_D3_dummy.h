#ifndef D_A_OBJ_D3_DUMMY_H
#define D_A_OBJ_D3_DUMMY_H

#include "d/a/obj/d_a_obj_base.h"
#include "s/s_State.hpp"

class dAcOD3Dummy_c : public dAcObjBase_c {
public:
    dAcOD3Dummy_c() : mStateMgr(*this) {}
    virtual ~dAcOD3Dummy_c() {}

    STATE_FUNC_DECLARE(dAcOD3Dummy_c, Wait);

private:
    /* 0x??? */ STATE_MGR_DECLARE(dAcOD3Dummy_c);
};

#endif
