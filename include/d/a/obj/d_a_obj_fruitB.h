#ifndef D_A_OBJ_FRUITB_H
#define D_A_OBJ_FRUITB_H

#include <d/a/obj/d_a_obj_base.h>
#include <s/s_State.hpp>
#include <s/s_StateMgr.hpp>

class dAcOFruitB_c : public dAcObjBase_c {
public:
    dAcOFruitB_c() : mStateMgr(*this, sStateID::null) {}
    virtual ~dAcOFruitB_c() {}

    STATE_FUNC_DECLARE(dAcOFruitB_c, Stick);
    STATE_FUNC_DECLARE(dAcOFruitB_c, Wait);
    STATE_FUNC_DECLARE(dAcOFruitB_c, DummyEvent);

private:
    /* 0x??? */ STATE_MGR_DECLARE(dAcOFruitB_c);
};

#endif
