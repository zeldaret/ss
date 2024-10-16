#ifndef D_A_OBJ_BELL_H
#define D_A_OBJ_BELL_H

#include "d/a/obj/d_a_obj_base.h"
#include "s/s_State.hpp"
#include "s/s_StateMgr.hpp"

class dAcObell_c : public dAcObjBase_c {
public:
    dAcObell_c() : mStateMgr(*this, sStateID::null) {}
    virtual ~dAcObell_c() {}

    STATE_FUNC_DECLARE(dAcObell_c, Wait);
    STATE_FUNC_DECLARE(dAcObell_c, WaitRupee);
    STATE_FUNC_DECLARE(dAcObell_c, After);

private:
    /* 0x??? */ STATE_MGR_DECLARE(dAcObell_c);
};

#endif
