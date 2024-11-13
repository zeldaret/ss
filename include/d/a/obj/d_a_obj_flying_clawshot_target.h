#ifndef D_A_OBJ_FLYING_CLAWSHOT_TARGET_H
#define D_A_OBJ_FLYING_CLAWSHOT_TARGET_H

#include "d/a/obj/d_a_obj_base.h"
#include "d/col/cc/d_cc_d.h"
#include "m/m3d/m_smdl.h"
#include "m/m_quat.h"
#include "m/m_vec.h"

class dAcOFlyingClawshotTarget_c : public dAcObjBase_c {
public:
    dAcOFlyingClawshotTarget_c() {}
    virtual ~dAcOFlyingClawshotTarget_c() {}

    virtual int draw() override;
    virtual bool createHeap() override;
    virtual int actorCreate() override;
    virtual int actorPostCreate() override;
    virtual int actorExecute() override;

private:
    /* 0x330 */ m3d::smdl_c mMdl;
    /* 0x34C */ dCcD_Sph mSph;
    /* 0x49C */ int mState;
    /* 0x4A0 */ mVec3_c mMarkPoint;
    /* 0x4AC */ mQuat_c mToLink;

    static const cCcD_SrcGObj sColSrc;
};

#endif
