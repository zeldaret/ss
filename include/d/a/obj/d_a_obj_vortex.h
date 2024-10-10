#ifndef D_A_OBJ_VORTEX_H
#define D_A_OBJ_VORTEX_H

#include <d/a/obj/d_a_obj_base.h>
#include <s/s_State.hpp>
#include <s/s_StateMgr.hpp>

class dAcOVortex_c : public dAcObjBase_c {
public:
    dAcOVortex_c() : mStateMgr(*this, sStateID::null) {}
    virtual ~dAcOVortex_c() {}

    STATE_FUNC_DECLARE(dAcOVortex_c, Wait);
    STATE_FUNC_DECLARE(dAcOVortex_c, Appear);

    void triggerActivation() {
        actor_properties |= 0x4;
        field_0x870 = 1;
    }

private:
    /* 0x??? */ STATE_MGR_DECLARE(dAcOVortex_c);
    /* 0x??? */ u8 field____[0x870 - 0x36C];
    /* 0x870 */ u8 field_0x870;
};

#endif
