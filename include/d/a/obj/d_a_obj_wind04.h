#ifndef D_A_OBJ_WIND04_H
#define D_A_OBJ_WIND04_H

#include <d/a/obj/d_a_obj_base.h>
#include <s/s_State.hpp>
#include <s/s_StateMgr.hpp>

class dAcOwind04_c : public dAcObjBase_c {
public:
    dAcOwind04_c() : mStateMgr(*this, sStateID::null) {}
    virtual ~dAcOwind04_c() {}

    STATE_FUNC_DECLARE(dAcOwind04_c, Wait);
    STATE_FUNC_DECLARE(dAcOwind04_c, Loop);
    STATE_FUNC_DECLARE(dAcOwind04_c, Stop);
    STATE_FUNC_DECLARE(dAcOwind04_c, Dead);

private:
    /* 0x??? */ STATE_MGR_DECLARE(dAcOwind04_c);
};

#endif
