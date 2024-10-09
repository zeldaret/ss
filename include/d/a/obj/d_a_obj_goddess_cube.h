#ifndef D_A_OBJ_GODDESS_CUBE_H
#define D_A_OBJ_GODDESS_CUBE_H

#include <d/a/obj/d_a_obj_base.h>
#include <s/s_State.hpp>
#include <s/s_StateMgr.hpp>

class dAcOGoddessCube_c : public dAcObjBase_c {
public:
    dAcOGoddessCube_c() : mStateMgr(*this, sStateID::null) {}
    virtual ~dAcOGoddessCube_c() {}

    STATE_FUNC_DECLARE(dAcOGoddessCube_c, Wait);
    STATE_FUNC_DECLARE(dAcOGoddessCube_c, Shot);

private:
    /* 0x??? */ STATE_MGR_DECLARE(dAcOGoddessCube_c);
};

#endif
