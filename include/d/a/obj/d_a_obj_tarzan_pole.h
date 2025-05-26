#ifndef D_A_OBJ_TARZAN_POLE_H
#define D_A_OBJ_TARZAN_POLE_H

#include "d/a/obj/d_a_obj_base.h"
#include "d/col/bg/d_bg_w.h"
#include "d/col/cc/d_cc_d.h"
#include "m/m3d/m_smdl.h"
#include "m/m_vec.h"
#include "nw4r/g3d/res/g3d_resfile.h"

class dAcOTarzanPole_c : public dAcObjBase_c {
public:
    dAcOTarzanPole_c() {}
    virtual ~dAcOTarzanPole_c() {}

    virtual bool createHeap() override;
    virtual int create() override;
    virtual int doDelete() override;
    virtual int actorExecute() override;
    virtual int draw() override;

private:
    static dCcD_SrcSph sCcSrc;

    static const f32 sXOffset;
    static const f32 sYOffset;

    /* 0x330 */ nw4r::g3d::ResFile mBrres;
    /* 0x334 */ m3d::smdl_c mMdl;
    /* 0x350 */ dBgW mBgCollider;
    /* 0x560 */ dCcD_Sph mCollider;
    /* 0x6b0 */ mVec3_c mVec; // TODO: Find better name, once we've figured out it's purpose
    /* 0x6bc */ f32 mFloat;   // TODO: No usage found yet
};

#endif
