#ifndef D_A_OBJ_FLAG_H
#define D_A_OBJ_FLAG_H

#include <d/a/obj/d_a_obj_base.h>
#include <s/s_State.hpp>
#include <s/s_StateMgr.hpp>

class dAcOFlag_c : public dAcObjBase_c {
public:
    dAcOFlag_c() : mStateMgr(*this, sStateID::null) {}
    virtual ~dAcOFlag_c() {}

    STATE_FUNC_DECLARE(dAcOFlag_c, Wait);

private:
    /* 0x??? */ STATE_MGR_DECLARE(dAcOFlag_c);
};

#endif
