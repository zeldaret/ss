#ifndef D_A_OBJ_SW_HARP_H
#define D_A_OBJ_SW_HARP_H

#include <d/a/obj/d_a_obj_base.h>
#include <s/s_State.hpp>
#include <s/s_StateMgr.hpp>

class dAcOSwHarp_c : public dAcObjBase_c {
public:
    dAcOSwHarp_c() : mStateMgr(*this, sStateID::null) {}
    virtual ~dAcOSwHarp_c() {}

    STATE_FUNC_DECLARE(dAcOSwHarp_c, Wait);
    STATE_FUNC_DECLARE(dAcOSwHarp_c, OnSwitch);
    STATE_FUNC_DECLARE(dAcOSwHarp_c, EndDemo);

private:
    /* 0x??? */ STATE_MGR_DECLARE(dAcOSwHarp_c);
};

#endif
