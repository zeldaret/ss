#ifndef D_A_OBJ_GOD_MARK_H
#define D_A_OBJ_GOD_MARK_H

#include "d/a/obj/d_a_obj_base.h"
#include "s/s_State.hpp"

class dAcOgodMark_c : public dAcObjBase_c {
public:
    dAcOgodMark_c() : mStateMgr(*this) {}
    virtual ~dAcOgodMark_c() {}

    STATE_FUNC_DECLARE(dAcOgodMark_c, Wait);
    STATE_FUNC_DECLARE(dAcOgodMark_c, Shine);

private:
    /* 0x??? */ STATE_MGR_DECLARE(dAcOgodMark_c);
};

#endif
