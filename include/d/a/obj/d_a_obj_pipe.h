#ifndef D_A_OBJ_PIPE_H
#define D_A_OBJ_PIPE_H

#include <d/a/obj/d_a_obj_base.h>
#include <s/s_State.hpp>
#include <s/s_StateMgr.hpp>

class dAcOPipe_c : public dAcObjBase_c {
public:
    dAcOPipe_c() : mStateMgr(*this, sStateID::null) {}
    virtual ~dAcOPipe_c() {}

    STATE_FUNC_DECLARE(dAcOPipe_c, Hold);
    STATE_FUNC_DECLARE(dAcOPipe_c, Wait);
    STATE_FUNC_DECLARE(dAcOPipe_c, Drop);

private:
    /* 0x??? */ STATE_MGR_DECLARE(dAcOPipe_c);
};

#endif
