#ifndef D_A_OBJ_PROPERA_H
#define D_A_OBJ_PROPERA_H

#include "d/a/d_a_salbage_obj.h"
#include "d/col/bg/d_bg_w.h"
#include "m/m3d/m_smdl.h"

class dAcObjPropera_c : public dAcSalbageObj_c {
public:
    dAcObjPropera_c() : dAcSalbageObj_c(&mMdl, SALVAGE_OBJ_PROPERA) {}
    virtual ~dAcObjPropera_c() {}

    STATE_VIRTUAL_OVERRIDE_FUNC_DECLARE(dAcObjPropera_c, dAcSalbageObj_c, DemoThrow);

private:
    /* 0x970 */ m3d::smdl_c mMdl;
    /* 0x98C */ dBgW mBgW;
};

#endif
