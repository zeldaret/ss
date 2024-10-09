#ifndef D_A_E_EYE_H
#define D_A_E_EYE_H

#include <d/a/obj/d_a_obj_base.h>
#include <s/s_State.hpp>
#include <s/s_StateMgr.hpp>

class dAcEEye_c : public dAcObjBase_c {
public:
    dAcEEye_c() : mStateMgr(*this, sStateID::null) {}
    virtual ~dAcEEye_c() {}

    STATE_FUNC_DECLARE(dAcEEye_c, Open);
    STATE_FUNC_DECLARE(dAcEEye_c, Wait);
    STATE_FUNC_DECLARE(dAcEEye_c, Close);
    STATE_FUNC_DECLARE(dAcEEye_c, Chance);
    STATE_FUNC_DECLARE(dAcEEye_c, Dead);

private:
    /* 0x??? */ STATE_MGR_DECLARE(dAcEEye_c);
};

#endif
