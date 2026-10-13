#ifndef D_A_OBJ_RAIL_END_H
#define D_A_OBJ_RAIL_END_H

#include "d/a/obj/d_a_obj_base.h"
#include "d/col/cc/d_cc_d.h"
#include "m/m3d/m_smdl.h"

class dAcORailEnd_c : public dAcObjBase_c {
public:
    dAcORailEnd_c();
    virtual ~dAcORailEnd_c();

    virtual bool createHeap() override;
    virtual int create() override;
    virtual int doDelete() override;
    virtual int actorExecute() override;
    virtual int draw() override;

private:
    /* 0x330 */ m3d::smdl_c mMdl;
    /* 0x34C */ dCcD_Sph mSph;

    static const f32 lbl_525_rodata_0;
};

#endif
