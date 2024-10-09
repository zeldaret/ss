#ifndef D_A_OBJ_FLOWER_ANCIENT_H
#define D_A_OBJ_FLOWER_ANCIENT_H

#include <d/a/obj/d_a_obj_base.h>
#include <s/s_State.hpp>
#include <s/s_StateMgr.hpp>

class dAcOFlowerAncient_c : public dAcObjBase_c {
public:
    dAcOFlowerAncient_c() : mStateMgr(*this, sStateID::null) {}
    virtual ~dAcOFlowerAncient_c() {}

    STATE_FUNC_DECLARE(dAcOFlowerAncient_c, Wait);
    STATE_FUNC_DECLARE(dAcOFlowerAncient_c, TimeSlip);
    STATE_FUNC_DECLARE(dAcOFlowerAncient_c, Break);
    STATE_FUNC_DECLARE(dAcOFlowerAncient_c, WhipCarry);

private:
    /* 0x??? */ STATE_MGR_DECLARE(dAcOFlowerAncient_c);
};

#endif
