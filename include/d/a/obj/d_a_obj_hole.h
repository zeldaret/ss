#ifndef D_A_OBJ_HOLE_H
#define D_A_OBJ_HOLE_H

#include <d/a/obj/d_a_obj_base.h>
#include <s/s_State.hpp>
#include <s/s_StateMgr.hpp>

class dAcOhole_c : public dAcObjBase_c {
public:
    dAcOhole_c() : mStateMgr(*this, sStateID::null) {}
    virtual ~dAcOhole_c() {}

    STATE_FUNC_DECLARE(dAcOhole_c, Wait);

private:
    /* 0x??? */ STATE_MGR_DECLARE(dAcOhole_c);
};

#endif
