#ifndef D_A_OBJ_EVIL_FIELD_H
#define D_A_OBJ_EVIL_FIELD_H

#include "d/a/obj/d_a_obj_base.h"
#include "s/s_State.hpp"

class dAcOevilField_c : public dAcObjBase_c {
public:
    dAcOevilField_c() : mStateMgr(*this) {}
    virtual ~dAcOevilField_c() {}

    STATE_FUNC_DECLARE(dAcOevilField_c, Wait);
    STATE_FUNC_DECLARE(dAcOevilField_c, UpDown);
    STATE_FUNC_DECLARE(dAcOevilField_c, Stop);

private:
    /* 0x??? */ STATE_MGR_DECLARE(dAcOevilField_c);
};

#endif
