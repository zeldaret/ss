#ifndef D_A_OBJ_FENCE_IRON_H
#define D_A_OBJ_FENCE_IRON_H

#include <d/a/obj/d_a_obj_base.h>
#include <s/s_State.hpp>
#include <s/s_StateMgr.hpp>

class dAcOfenceIron_c : public dAcObjBase_c {
public:
    dAcOfenceIron_c() : mStateMgr(*this, sStateID::null) {}
    virtual ~dAcOfenceIron_c() {}

    STATE_FUNC_DECLARE(dAcOfenceIron_c, Close);
    STATE_FUNC_DECLARE(dAcOfenceIron_c, PreMoveUp);
    STATE_FUNC_DECLARE(dAcOfenceIron_c, MoveUp);
    STATE_FUNC_DECLARE(dAcOfenceIron_c, AfterMoveUp);
    STATE_FUNC_DECLARE(dAcOfenceIron_c, Open);
    STATE_FUNC_DECLARE(dAcOfenceIron_c, PreMoveDown);
    STATE_FUNC_DECLARE(dAcOfenceIron_c, MoveDown);
    STATE_FUNC_DECLARE(dAcOfenceIron_c, AfterMoveDown);

private:
    /* 0x??? */ STATE_MGR_DECLARE(dAcOfenceIron_c);
};

#endif
