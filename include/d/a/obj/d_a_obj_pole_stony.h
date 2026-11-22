#ifndef D_A_OBJ_POLE_STONY_H
#define D_A_OBJ_POLE_STONY_H

#include "context_bte.h"
#include "d/a/obj/d_a_obj_base.h"
#include "d/col/cc/d_cc_d.h"
#include "egg/egg_types.h"
#include "egg/math/eggVector.h"
#include "m/m3d/m_smdl.h"

class dAcOPoleStony_c : public dAcObjBase_c {
public:
    dAcOPoleStony_c() {}
    virtual ~dAcOPoleStony_c() {}

    int draw();
    bool createHeap();

private:
    
    /* 0x330 */ m3d::smdl_c field_0x330[4];
    /* 0x3a0 */ dCcD_Cyl field_0x3a0;
    /* 0x4f0 */ EGG::Matrix34f field_0x4f0[4];
    /* 0x5b0 */ EGG::Vector3f field_0x5b0[4];
    /* 0x5e0 */ EGG::Vector3f field_0x5e0[4];
    /* 0x610 */ EGG::Vector3s field_0x610;
    /* 0x628 */ bool field_0x628;

};

#endif
