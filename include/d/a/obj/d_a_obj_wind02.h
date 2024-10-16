#ifndef D_A_OBJ_WIND02_H
#define D_A_OBJ_WIND02_H

#include "d/a/obj/d_a_obj_base.h"
#include "s/s_State.hpp"
#include "s/s_StateMgr.hpp"

class dAcOwind02_c : public dAcObjBase_c {
public:
    dAcOwind02_c() : mStateMgr(*this, sStateID::null) {}
    virtual ~dAcOwind02_c() {}

    STATE_FUNC_DECLARE(dAcOwind02_c, Wait);
    STATE_FUNC_DECLARE(dAcOwind02_c, Loop);
    STATE_FUNC_DECLARE(dAcOwind02_c, Stop);
    STATE_FUNC_DECLARE(dAcOwind02_c, Dead);

private:
    /* 0x??? */ STATE_MGR_DECLARE(dAcOwind02_c);
};

#endif
