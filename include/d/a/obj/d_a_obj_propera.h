#ifndef D_A_OBJ_PROPERA_H
#define D_A_OBJ_PROPERA_H

#include <d/a/obj/d_a_obj_base.h>
#include <s/s_State.hpp>
#include <s/s_StateMgr.hpp>

class dAcObjPropera_c : public dAcObjBase_c {
public:
    dAcObjPropera_c() : mStateMgr(*this, sStateID::null) {}
    virtual ~dAcObjPropera_c() {}

private:
    /* 0x??? */ STATE_MGR_DECLARE(dAcObjPropera_c);
};

#endif
