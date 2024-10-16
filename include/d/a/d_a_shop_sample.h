#ifndef D_A_SHOP_SAMPLE_H
#define D_A_SHOP_SAMPLE_H

#include "d/a/obj/d_a_obj_base.h"
#include "s/s_State.hpp"
#include "s/s_StateMgr.hpp"

class dAcShopSample_c : public dAcObjBase_c {
public:
    dAcShopSample_c() : mStateMgr(*this, sStateID::null) {}
    virtual ~dAcShopSample_c() {}

    STATE_FUNC_DECLARE(dAcShopSample_c, Wait);
    STATE_FUNC_DECLARE(dAcShopSample_c, PickUp);
    STATE_FUNC_DECLARE(dAcShopSample_c, PickingUp);
    STATE_FUNC_DECLARE(dAcShopSample_c, PutBack);

private:
    /* 0x??? */ STATE_MGR_DECLARE(dAcShopSample_c);
};

#endif
