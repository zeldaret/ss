#ifndef D_A_OBJ_BLOCK_ROPE_H
#define D_A_OBJ_BLOCK_ROPE_H

#include <d/a/obj/d_a_obj_base.h>
#include <s/s_State.hpp>
#include <s/s_StateMgr.hpp>

class dAcOblockRope_c : public dAcObjBase_c {
public:
    dAcOblockRope_c() : mStateMgr(*this, sStateID::null) {}
    virtual ~dAcOblockRope_c() {}

    STATE_FUNC_DECLARE(dAcOblockRope_c, Wait);
    STATE_FUNC_DECLARE(dAcOblockRope_c, Delete);

private:
    /* 0x??? */ STATE_MGR_DECLARE(dAcOblockRope_c);
};

#endif
