#ifndef D_A_OBJ_TERRY_SHOP_H
#define D_A_OBJ_TERRY_SHOP_H

#include <d/a/obj/d_a_obj_base.h>
#include <s/s_State.hpp>
#include <s/s_StateMgr.hpp>

class dAcOterryShop_c : public dAcObjBase_c {
public:
    dAcOterryShop_c() : mStateMgr(*this, sStateID::null) {}
    virtual ~dAcOterryShop_c() {}

    STATE_FUNC_DECLARE(dAcOterryShop_c, Move);
    STATE_FUNC_DECLARE(dAcOterryShop_c, Stop);
    STATE_FUNC_DECLARE(dAcOterryShop_c, Enter);
    STATE_FUNC_DECLARE(dAcOterryShop_c, TakeOff);

private:
    /* 0x??? */ STATE_MGR_DECLARE(dAcOterryShop_c);
};

#endif
