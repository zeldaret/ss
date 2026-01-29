#ifndef D_A_OBJ_LOTUS_H
#define D_A_OBJ_LOTUS_H

#include "d/a/obj/d_a_obj_base.h"
#include "s/s_State.hpp"

class dAcOLotus_c : public dAcObjBase_c {
public:
    dAcOLotus_c() : mStateMgr(*this) {}
    virtual ~dAcOLotus_c() {}

    STATE_FUNC_DECLARE(dAcOLotus_c, Wait);
    STATE_FUNC_DECLARE(dAcOLotus_c, Reverse);
    STATE_FUNC_DECLARE(dAcOLotus_c, Rotate);
    STATE_FUNC_DECLARE(dAcOLotus_c, EndRotate);

private:
    /* 0x??? */ STATE_MGR_DECLARE(dAcOLotus_c);
};

#endif
