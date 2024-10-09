#ifndef D_A_OBJ_CHEST_H
#define D_A_OBJ_CHEST_H

#include <d/a/obj/d_a_obj_base.h>
#include <s/s_State.hpp>
#include <s/s_StateMgr.hpp>

class dAcOChest_c : public dAcObjBase_c {
public:
    dAcOChest_c() : mStateMgr(*this, sStateID::null) {}
    virtual ~dAcOChest_c() {}

    STATE_FUNC_DECLARE(dAcOChest_c, Wait);
    STATE_FUNC_DECLARE(dAcOChest_c, OpenEvent);
    STATE_FUNC_DECLARE(dAcOChest_c, FailEvent);
    STATE_FUNC_DECLARE(dAcOChest_c, OrderOpenEventAfter);

private:
    /* 0x??? */ STATE_MGR_DECLARE(dAcOChest_c);
};

#endif
