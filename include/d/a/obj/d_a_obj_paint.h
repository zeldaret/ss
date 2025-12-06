#ifndef D_A_OBJ_PAINT_H
#define D_A_OBJ_PAINT_H

#include "d/a/obj/d_a_obj_base.h"
#include "d/col/bg/d_bg_w.h"
#include "d/d_shadow.h"
#include "m/m3d/m_smdl.h"
#include "m/m_angle.h"
#include "nw4r/g3d/res/g3d_resfile.h"
#include "s/s_State.hpp"
#include "s/s_StateMgr.hpp"

class dAcOpaint_c : public dAcObjBase_c {
public:
    dAcOpaint_c() : mStateMgr(*this, sStateID::null) {}
    virtual ~dAcOpaint_c() {}

    virtual bool createHeap() override;
    virtual int create() override;
    virtual int doDelete() override;
    virtual int actorExecute() override;
    virtual int draw() override;

    STATE_FUNC_DECLARE(dAcOpaint_c, Wait);

private:
    void updateMatrix();
    const char *getOarcName();
    const char *getModelName();

    /* 0x330 */ nw4r::g3d::ResFile mResFile;
    /* 0x334 */ m3d::smdl_c mMdl;
    /* 0x350 */ dShadowCircle_c mShadow;
    /* 0x358 */ dBgW mBgW;
    /* 0x568 */ STATE_MGR_DECLARE(dAcOpaint_c);
    /* 0x5A4 */ mAng mRotationZRelated;
    /* 0x5A6 */ mAng mRotationZOffset;
    /* 0x5A8 */ s8 field_0x5A8;
    /* 0x5A9 */ u8 mSubtype;
    /* 0x5AA */ bool field_0x5AA;

    static mAng rotationZRelated0;
    static mAng rotationZRelated1;
    static const f32 lbl_613_rodata_2C;
};

#endif
