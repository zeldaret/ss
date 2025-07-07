#ifndef D_A_OBJ_CLOUD_DIVE_H
#define D_A_OBJ_CLOUD_DIVE_H

#include "d/a/obj/d_a_obj_base.h"
#include "d/col/cc/d_cc_d.h"
#include "m/m3d/m_smdl.h"
#include "nw4r/g3d/res/g3d_resfile.h"
#include "s/s_State.hpp"

class dAcOcloudDive_c : public dAcObjBase_c {
public:
    dAcOcloudDive_c() : mStateMgr(*this, sStateID::null) {}
    virtual ~dAcOcloudDive_c() {}

    virtual bool createHeap() override;
    virtual int create() override;
    virtual int doDelete() override;
    virtual int actorExecute() override;
    virtual int draw() override;

    bool fn_350_6F0();
    bool fn_350_760();

    STATE_FUNC_DECLARE(dAcOcloudDive_c, Wait);
    STATE_FUNC_DECLARE(dAcOcloudDive_c, Dead);

private:
    /* 0x330 */ nw4r::g3d::ResFile mResFile;
    /* 0x334 */ m3d::smdl_c mMdl;
    /* 0x350 */ dCcD_Cyl mCollider;
    /* 0x4A0 */ STATE_MGR_DECLARE(dAcOcloudDive_c);
    /* 0x4DC */ u8 field_0x4DC;
    /* 0x4DD */ u8 mItemID;
    /* 0x4E0 */ f32 mRadius;

    static const f32 mRadius1;
    static const f32 mRadius2;
    static const f32 height;
    static dCcD_SrcCyl sCylSrc;
};

#endif
