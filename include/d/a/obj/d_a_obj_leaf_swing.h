#ifndef D_A_OBJ_LEAF_SWING_H
#define D_A_OBJ_LEAF_SWING_H

#include "d/a/obj/d_a_obj_base.h"
#include "s/s_State.hpp"
#include "s/s_StateMgr.hpp"

class dAcOleafSwing_c : public dAcObjBase_c {
public:
    dAcOleafSwing_c() : mStateMgr(*this, sStateID::null) {}
    virtual ~dAcOleafSwing_c() {}

    STATE_FUNC_DECLARE(dAcOleafSwing_c, SwingCalc);

private:
    /* 0x??? */ STATE_MGR_DECLARE(dAcOleafSwing_c);
};

#endif
