#ifndef D_A_OBJ_GUARD_LOG_H
#define D_A_OBJ_GUARD_LOG_H

#include <d/a/obj/d_a_obj_base.h>
#include <s/s_State.hpp>
#include <s/s_StateMgr.hpp>

class dAcOguardLog_c : public dAcObjBase_c {
public:
    dAcOguardLog_c() : mStateMgr(*this, sStateID::null) {}
    virtual ~dAcOguardLog_c() {}

    STATE_FUNC_DECLARE(dAcOguardLog_c, Init);
    STATE_FUNC_DECLARE(dAcOguardLog_c, Wait);
    STATE_FUNC_DECLARE(dAcOguardLog_c, Shake);

private:
    /* 0x??? */ STATE_MGR_DECLARE(dAcOguardLog_c);
};

#endif
