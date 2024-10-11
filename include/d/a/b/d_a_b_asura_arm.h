#ifndef D_A_B_ASURA_ARM_H
#define D_A_B_ASURA_ARM_H

#include <d/a/obj/d_a_obj_base.h>
#include <s/s_State.hpp>
#include <s/s_StateMgr.hpp>

class dAcAsuraArm_c : public dAcObjBase_c {
public:
    dAcAsuraArm_c() : mStateMgr(*this, sStateID::null) {}
    virtual ~dAcAsuraArm_c() {}

    STATE_FUNC_DECLARE(dAcAsuraArm_c, Pulled);
    STATE_FUNC_DECLARE(dAcAsuraArm_c, Wait);
    STATE_FUNC_DECLARE(dAcAsuraArm_c, Demo);

private:
    /* 0x??? */ STATE_MGR_DECLARE(dAcAsuraArm_c);
};

#endif
