#ifndef D_A_OBJ_F400_GATE_LEAF_H
#define D_A_OBJ_F400_GATE_LEAF_H

#include "d/a/obj/d_a_obj_base.h"

class dAcOF400GateLeaf_c : public dAcObjBase_c {
public:
    dAcOF400GateLeaf_c() {}
    virtual ~dAcOF400GateLeaf_c() {}

    virtual bool createHeap() override;
    virtual int create() override;
    virtual int draw() override;

    u16 getStoryflag();

private:
    /* 0x330 */ m3d::smdl_c mMdl;
};

#endif
