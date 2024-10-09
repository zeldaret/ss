#ifndef D_A_OBJ_SLICE_LOG_H
#define D_A_OBJ_SLICE_LOG_H

#include <d/a/obj/d_a_obj_base.h>
#include <s/s_State.hpp>
#include <s/s_StateMgr.hpp>

class dAcOsliceLog_c : public dAcObjBase_c {
public:
    dAcOsliceLog_c() : mStateMgr(*this, sStateID::null) {}
    virtual ~dAcOsliceLog_c() {}

    STATE_FUNC_DECLARE(dAcOsliceLog_c, Fall);
    STATE_FUNC_DECLARE(dAcOsliceLog_c, Bound);
    STATE_FUNC_DECLARE(dAcOsliceLog_c, Init);
    STATE_FUNC_DECLARE(dAcOsliceLog_c, Wait);
    STATE_FUNC_DECLARE(dAcOsliceLog_c, Slice);

private:
    /* 0x??? */ STATE_MGR_DECLARE(dAcOsliceLog_c);
};

#endif
