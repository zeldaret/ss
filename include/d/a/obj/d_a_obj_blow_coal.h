#ifndef D_A_OBJ_BLOW_COAL_H
#define D_A_OBJ_BLOW_COAL_H

#include "d/a/obj/d_a_obj_base.h"
#include "s/s_State.hpp"

class dAcOblowCoal_c : public dAcObjBase_c {
public:
    dAcOblowCoal_c() : mStateMgr(*this) {}
    virtual ~dAcOblowCoal_c() {}

    STATE_FUNC_DECLARE(dAcOblowCoal_c, Wait);
    STATE_FUNC_DECLARE(dAcOblowCoal_c, Dead);

private:
    /* 0x??? */ STATE_MGR_DECLARE(dAcOblowCoal_c);
};

#endif
