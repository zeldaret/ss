#ifndef D_A_OBJ_GRAVE_H
#define D_A_OBJ_GRAVE_H

#include "d/a/obj/d_a_obj_base.h"
#include "d/col/bg/d_bg_w.h"
#include "d/col/cc/d_cc_d.h"
#include "d/d_shadow.h"
#include "m/m3d/m_anmmatclr.h"
#include "m/m3d/m_smdl.h"
#include "m/m_vec.h"
#include "nw4r/g3d/res/g3d_resfile.h"
#include "s/s_State.hpp"

class dAcOGrave_c : public dAcObjBase_c {
public:
    dAcOGrave_c() : mStateMgr(*this) {}
    virtual ~dAcOGrave_c() {}

    virtual bool createHeap();
    virtual int create();
    virtual int doDelete();
    virtual int actorExecute();
    virtual int draw();

    void fn_580_480(dAcObjBase_c *link, dBgW_Base::PushPullLabel label);

    void executeState() {
        mStateMgr.executeState();
    }

    STATE_FUNC_DECLARE(dAcOGrave_c, Wait);
    STATE_FUNC_DECLARE(dAcOGrave_c, Move);

private:
    static dAcObjBase_c *pushPullCb(dAcObjBase_c *grave, dAcObjBase_c *link, dBgW_Base::PushPullLabel label);
    static bool unkCb(dBgW *bgW, dAcObjBase_c *grave, dAcObjBase_c *link);

    /* 0x330 */ nw4r::g3d::ResFile mResFile;
    /* 0x334 */ m3d::smdl_c mMdl;
    /* 0x350 */ dShadowCircle_c mShadow;
    /* 0x358 */ dBgW mBgW;
    /* 0x568 */ STATE_MGR_DECLARE(dAcOGrave_c);
    /* 0x5A4 */ dCcD_Unk mCollider;
    /* 0x7AC */ m3d::anmMatClr_c mAnmMatClr;
    /* 0x7D8 */ mVec3_c mOrigPos1;
    /* 0x7E4 */ mVec3_c mOrigPos2;
    /* 0x7F0 */ u16 mTimer;
    /* 0x7F2 */ u8 mSceneflag;
    /* 0x7F3 */ bool mShouldGlow;
    /* 0x7F4 */ bool mPush;
    /* 0x7F5 */ bool mPull;
    /* 0x7F6 */ bool field_0x7F6;
    /* 0x7F7 */ bool field_0x7F7;

    static dCcD_SrcUnk sUnkSrc;
    static const u8 lbl_580_rodata_0;
    static const f32 lbl_580_rodata_30;
};

#endif
