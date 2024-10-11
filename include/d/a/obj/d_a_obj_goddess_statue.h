#ifndef D_A_OBJ_GODDESS_STATUE_H
#define D_A_OBJ_GODDESS_STATUE_H

#include <d/a/obj/d_a_obj_base.h>
#include <s/s_State.hpp>
#include <s/s_StateMgr.hpp>

class dAcOGoddessStatue_c : public dAcObjBase_c {
public:
    dAcOGoddessStatue_c() : mStateMgr(*this, sStateID::null) {}
    virtual ~dAcOGoddessStatue_c() {}

    STATE_FUNC_DECLARE(dAcOGoddessStatue_c, Wait);

private:
    /* 0x??? */ STATE_MGR_DECLARE(dAcOGoddessStatue_c);
};

#endif
