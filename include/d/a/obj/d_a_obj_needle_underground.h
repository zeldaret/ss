#ifndef D_A_OBJ_NEEDLE_UNDERGROUND_H
#define D_A_OBJ_NEEDLE_UNDERGROUND_H

#include <d/a/obj/d_a_obj_base.h>
#include <s/s_State.hpp>
#include <s/s_StateMgr.hpp>

class dAcONeedleUnderground_c : public dAcObjBase_c {
public:
    dAcONeedleUnderground_c() : mStateMgr(*this, sStateID::null) {}
    virtual ~dAcONeedleUnderground_c() {}

    STATE_FUNC_DECLARE(dAcONeedleUnderground_c, RemainOpen);
    STATE_FUNC_DECLARE(dAcONeedleUnderground_c, Close);
    STATE_FUNC_DECLARE(dAcONeedleUnderground_c, RemainClosed);
    STATE_FUNC_DECLARE(dAcONeedleUnderground_c, Open);

private:
    /* 0x??? */ STATE_MGR_DECLARE(dAcONeedleUnderground_c);
};

#endif
