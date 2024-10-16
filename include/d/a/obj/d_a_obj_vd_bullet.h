#ifndef D_A_OBJ_VD_BULLET_H
#define D_A_OBJ_VD_BULLET_H

#include "d/a/obj/d_a_obj_base.h"
#include "s/s_State.hpp"
#include "s/s_StateMgr.hpp"

class dAcObjVdb_c : public dAcObjBase_c {
public:
    dAcObjVdb_c() : mStateMgr(*this, sStateID::null) {}
    virtual ~dAcObjVdb_c() {}

    STATE_FUNC_DECLARE(dAcObjVdb_c, BulletMove);
    STATE_FUNC_DECLARE(dAcObjVdb_c, BulletLost);

private:
    /* 0x??? */ STATE_MGR_DECLARE(dAcObjVdb_c);
};

#endif
