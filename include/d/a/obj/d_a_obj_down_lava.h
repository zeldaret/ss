#ifndef D_A_OBJ_DOWN_LAVA_H
#define D_A_OBJ_DOWN_LAVA_H

#include <d/a/obj/d_a_obj_base.h>
#include <s/s_State.hpp>
#include <s/s_StateMgr.hpp>

class dAcOdownLava_c : public dAcObjBase_c {
public:
    dAcOdownLava_c() : mStateMgr(*this, sStateID::null) {}
    virtual ~dAcOdownLava_c() {}

    STATE_FUNC_DECLARE(dAcOdownLava_c, Wait);
    STATE_FUNC_DECLARE(dAcOdownLava_c, Move);
    STATE_FUNC_DECLARE(dAcOdownLava_c, After);

private:
    /* 0x??? */ STATE_MGR_DECLARE(dAcOdownLava_c);
};

#endif
