#ifndef D_A_OBJ_PUSH_BLOCK_H
#define D_A_OBJ_PUSH_BLOCK_H

#include "d/a/obj/d_a_obj_base.h"
#include "s/s_State.hpp"

class dAcOpushBlk_c : public dAcObjBase_c {
public:
    dAcOpushBlk_c() : mStateMgr(*this) {}
    virtual ~dAcOpushBlk_c() {}

    STATE_FUNC_DECLARE(dAcOpushBlk_c, Wait);
    STATE_FUNC_DECLARE(dAcOpushBlk_c, Move);
    STATE_FUNC_DECLARE(dAcOpushBlk_c, RopeGrab);

private:
    /* 0x??? */ STATE_MGR_DECLARE(dAcOpushBlk_c);
};

#endif
