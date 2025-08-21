#ifndef D_A_OBJ_POT_SAL_H
#define D_A_OBJ_POT_SAL_H

#include "d/a/d_a_salbage_obj.h"
#include "m/m3d/m_smdl.h"
#include "s/s_State.hpp"

class dAcOpotSal_c : public dAcSalbageObj_c {
public:
    dAcOpotSal_c() : dAcSalbageObj_c(&mMdl1, SALVAGE_OBJ_POT) {}
    virtual ~dAcOpotSal_c() {}

    STATE_VIRTUAL_OVERRIDE_FUNC_DECLARE(dAcOpotSal_c, dAcSalbageObj_c, DemoThrow);
    STATE_VIRTUAL_OVERRIDE_FUNC_DECLARE(dAcOpotSal_c, dAcSalbageObj_c, After);

private:
    /* 0x970 */ m3d::smdl_c mMdl1;
    /* 0x98C */ m3d::smdl_c mMdl2;
};

#endif
