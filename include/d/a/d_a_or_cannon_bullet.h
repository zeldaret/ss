#ifndef D_A_OR_CANNON_BULLET_H
#define D_A_OR_CANNON_BULLET_H

#include <d/a/obj/d_a_obj_base.h>
#include <s/s_State.hpp>
#include <s/s_StateMgr.hpp>

class dAcOrCannBullet_c : public dAcObjBase_c {
public:
    dAcOrCannBullet_c() : mStateMgr(*this, sStateID::null) {}
    virtual ~dAcOrCannBullet_c() {}

    STATE_FUNC_DECLARE(dAcOrCannBullet_c, Wait);

private:
    /* 0x??? */ STATE_MGR_DECLARE(dAcOrCannBullet_c);
};

#endif
