#ifndef D_A_HIDORY_FIRE_H
#define D_A_HIDORY_FIRE_H

#include "d/a/obj/d_a_obj_base.h"
#include "s/s_State.hpp"

class dAcHidoryFire_c : public dAcObjBase_c {
public:
    dAcHidoryFire_c() : mStateMgr(*this) {}
    virtual ~dAcHidoryFire_c() {}

    STATE_FUNC_DECLARE(dAcHidoryFire_c, BulletMove);
    STATE_FUNC_DECLARE(dAcHidoryFire_c, Damage);

private:
    /* 0x??? */ STATE_MGR_DECLARE(dAcHidoryFire_c);
};

#endif
