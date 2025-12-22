#ifndef D_A_OBJ_SOIL_H
#define D_A_OBJ_SOIL_H

#include "d/a/obj/d_a_obj_base.h"
#include "s/s_State.hpp"
#include "s/s_StateMgr.hpp"

class dAcOsoil_c : public dAcObjBase_c {
public:
    dAcOsoil_c() : mStateMgr(*this, sStateID::null) {}
    virtual ~dAcOsoil_c() {}

    STATE_FUNC_DECLARE(dAcOsoil_c, Soil);
    STATE_FUNC_DECLARE(dAcOsoil_c, Hole);
    STATE_FUNC_DECLARE(dAcOsoil_c, Ready);

    bool isStateHole() {
        return mStateMgr.isState(StateID_Hole);
    }

private:
    /* 0x330 */ u8 field_0x330[0x6CC-0x330];
    /* 0x6CC */ STATE_MGR_DECLARE(dAcOsoil_c);
};

#endif
