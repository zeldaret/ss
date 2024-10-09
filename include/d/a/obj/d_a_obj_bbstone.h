#ifndef D_A_OBJ_BBSTONE_H
#define D_A_OBJ_BBSTONE_H

#include <d/a/obj/d_a_obj_base.h>
#include <s/s_State.hpp>
#include <s/s_StateMgr.hpp>

class dAcOBBStone : public dAcObjBase_c {
public:
    dAcOBBStone() : mStateMgr(*this, sStateID::null) {}
    virtual ~dAcOBBStone() {}

    STATE_FUNC_DECLARE(dAcOBBStone, Wait);
    STATE_FUNC_DECLARE(dAcOBBStone, FirstWait);
    STATE_FUNC_DECLARE(dAcOBBStone, FirstDemo);

private:
    /* 0x??? */ STATE_MGR_DECLARE(dAcOBBStone);
};

#endif
