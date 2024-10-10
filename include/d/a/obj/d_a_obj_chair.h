#ifndef D_A_OBJ_CHAIR_H
#define D_A_OBJ_CHAIR_H

#include <d/a/obj/d_a_obj_base.h>
#include <s/s_State.hpp>
#include <s/s_StateMgr.hpp>

class dAcOChair_c : public dAcObjBase_c {
public:
    dAcOChair_c() : mStateMgr(*this, sStateID::null) {}
    virtual ~dAcOChair_c() {}

    STATE_FUNC_DECLARE(dAcOChair_c, Wait);

private:
    /* 0x??? */ STATE_MGR_DECLARE(dAcOChair_c);
};

#endif
