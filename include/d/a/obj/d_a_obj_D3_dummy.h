#ifndef D_A_OBJ_D3_DUMMY_H
#define D_A_OBJ_D3_DUMMY_H

#include "d/a/obj/d_a_obj_base.h"
#include "d/col/bg/d_bg_w.h"
#include "s/s_State.hpp"

class dAcOD3Dummy_c : public dAcObjBase_c {
public:
    dAcOD3Dummy_c() : mStateMgr(*this) {}
    virtual ~dAcOD3Dummy_c() {}

    STATE_FUNC_DECLARE(dAcOD3Dummy_c, Wait);

    void setField_0x1570(u8 val) {
        field_0x1570 = val;
    }

private:
    /* 0x0330 */ dBgW field_330[8];
    /* 0x13B0 */ u8 unk[0x180];
    /* 0x1530 */ STATE_MGR_DECLARE(dAcOD3Dummy_c);
    /* 0x156C */ u8 field_0x156C[4];
    /* 0x1570 */ u8 field_0x1570;
    /* 0x1571 */ u8 field_0x1571[0xB];
};

#endif
