#ifndef D_A_OBJ_BELT_CVR_H
#define D_A_OBJ_BELT_CVR_H

#include "d/a/obj/d_a_obj_base.h"
#include "s/s_State.hpp"

class dAcObeltCvr_c : public dAcObjBase_c {
public:
    dAcObeltCvr_c() : mStateMgr(*this) {}
    virtual ~dAcObeltCvr_c() {}

    STATE_FUNC_DECLARE(dAcObeltCvr_c, Wait);

private:
    /* 0x??? */ STATE_MGR_DECLARE(dAcObeltCvr_c);
};

#endif
