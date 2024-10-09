#ifndef D_A_OBJ_TOGE_TRAP_H
#define D_A_OBJ_TOGE_TRAP_H

#include <d/a/obj/d_a_obj_base.h>
#include <s/s_State.hpp>
#include <s/s_StateMgr.hpp>

class dAcOtogeTrap_c : public dAcObjBase_c {
public:
    dAcOtogeTrap_c() : mStateMgr(*this, sStateID::null) {}
    virtual ~dAcOtogeTrap_c() {}

    STATE_FUNC_DECLARE(dAcOtogeTrap_c, Wait);
    STATE_FUNC_DECLARE(dAcOtogeTrap_c, Move);

private:
    /* 0x??? */ STATE_MGR_DECLARE(dAcOtogeTrap_c);
};

#endif
