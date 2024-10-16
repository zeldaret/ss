#ifndef D_A_OBJ_DECOA_H
#define D_A_OBJ_DECOA_H

#include "d/a/obj/d_a_obj_base.h"
#include "s/s_State.hpp"
#include "s/s_StateMgr.hpp"

class dAcODecoA_c : public dAcObjBase_c {
public:
    dAcODecoA_c() : mStateMgr(*this, sStateID::null) {}
    virtual ~dAcODecoA_c() {}

    STATE_FUNC_DECLARE(dAcODecoA_c, Wait);

private:
    /* 0x??? */ STATE_MGR_DECLARE(dAcODecoA_c);
};

#endif
