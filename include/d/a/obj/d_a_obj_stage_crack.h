#ifndef D_A_OBJ_STAGE_CRACK_H
#define D_A_OBJ_STAGE_CRACK_H

#include <d/a/obj/d_a_obj_base.h>
#include <s/s_State.hpp>
#include <s/s_StateMgr.hpp>

class dAcOstageCrack_c : public dAcObjBase_c {
public:
    dAcOstageCrack_c() : mStateMgr(*this, sStateID::null) {}
    virtual ~dAcOstageCrack_c() {}

    STATE_FUNC_DECLARE(dAcOstageCrack_c, Wait);
    STATE_FUNC_DECLARE(dAcOstageCrack_c, Stay);
    STATE_FUNC_DECLARE(dAcOstageCrack_c, Reverse);
    STATE_FUNC_DECLARE(dAcOstageCrack_c, Delete);

private:
    /* 0x??? */ STATE_MGR_DECLARE(dAcOstageCrack_c);
};

#endif
