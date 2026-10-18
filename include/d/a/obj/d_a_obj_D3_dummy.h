#ifndef D_A_OBJ_D3_DUMMY_H
#define D_A_OBJ_D3_DUMMY_H

#include "d/a/obj/d_a_obj_base.h"
#include "d/col/bg/d_bg_w.h"
#include "m/m_mtx.h"
#include "s/s_State.hpp"

class dAcOD3Dummy_c : public dAcObjBase_c {
public:
    dAcOD3Dummy_c() : mStateMgr(*this) {}
    virtual ~dAcOD3Dummy_c() {}

    virtual int create() override;
    virtual int doDelete() override;
    virtual bool createHeap() override;
    virtual int draw() override;
    virtual int actorExecute() override;

    STATE_FUNC_DECLARE(dAcOD3Dummy_c, Wait);

    void setField_0x1570(u8 val) {
        field_0x1570 = val;
    }

    s32 fn_599_7A0();
    void fn_599_810();
    void fn_599_940();

private:
    /* 0x0330 */ dBgW mBgWs[8];
    /* 0x13B0 */ mMtx_c field_13B0[8];
    /* 0x1530 */ STATE_MGR_DECLARE(dAcOD3Dummy_c);
    /* 0x156C */ s32 field_0x156C;
    /* 0x1570 */ u8 field_0x1570;
    /* 0x1571 */ u8 field_0x1571[3];
    /* 0x1574 */ void *field_0x1574;
    /* 0x1578 */ u8 field_0x1578;
    /* 0x1579 */ u8 field_0x1579[0x157C - 0x1579];
};

#endif
