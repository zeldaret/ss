#ifndef D_A_OBJ_BAMBOO_H
#define D_A_OBJ_BAMBOO_H

#include "d/a/obj/d_a_obj_base.h"
#include "s/s_State.hpp"
#include "s/s_StateMgr.hpp"

class dAcObamboo_c : public dAcObjBase_c {
public:
    dAcObamboo_c();
    virtual ~dAcObamboo_c();

    STATE_FUNC_DECLARE(dAcObamboo_c, Wait);
    STATE_FUNC_DECLARE(dAcObamboo_c, Fall);
    STATE_FUNC_DECLARE(dAcObamboo_c, CutMark);

private:
    /* 0x??? */ STATE_MGR_DECLARE(dAcObamboo_c);
};

#endif
