#ifndef D_A_OBJ_B300_SAND_H
#define D_A_OBJ_B300_SAND_H

#include <d/a/obj/d_a_obj_base.h>
#include <s/s_State.hpp>
#include <s/s_StateMgr.hpp>

class dAcOB300Sand_c : public dAcObjBase_c {
public:
    dAcOB300Sand_c() : mStateMgr(*this, sStateID::null) {}
    virtual ~dAcOB300Sand_c() {}

    STATE_FUNC_DECLARE(dAcOB300Sand_c, Wait);
    STATE_FUNC_DECLARE(dAcOB300Sand_c, Clear);

private:
    /* 0x??? */ STATE_MGR_DECLARE(dAcOB300Sand_c);
};

#endif
