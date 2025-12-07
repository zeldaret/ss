#ifndef D_A_OBJ_GRASS_COIL_H
#define D_A_OBJ_GRASS_COIL_H

#include "d/a/obj/d_a_obj_base.h"
#include "d/col/cc/d_cc_d.h"
#include "m/m3d/m_smdl.h"
#include "m/m_angle.h"
#include "m/m_vec.h"
#include "nw4r/g3d/res/g3d_resfile.h"
#include "s/s_State.hpp"
#include "s/s_StateMgr.hpp"

class dAcOgrassCoil_c : public dAcObjBase_c {
public:
    dAcOgrassCoil_c() : mStateMgr(*this, sStateID::null) {}
    virtual ~dAcOgrassCoil_c() {}

    virtual bool createHeap() override;
    virtual int create() override;
    virtual int doDelete() override;
    virtual int actorExecute() override;
    virtual int draw() override;

    STATE_FUNC_DECLARE(dAcOgrassCoil_c, Wait);

    STATE_MGR_DEFINE_UTIL_EXECUTESTATE(dAcOgrassCoil_c);

private:
    /* 0x330 */ nw4r::g3d::ResFile mResFile;
    /* 0x334 */ m3d::smdl_c mMdl[2];
    /* 0x36C */ STATE_MGR_DECLARE(dAcOgrassCoil_c);
    /* 0x3A8 */ dCcD_Cyl mCollider;
    /* 0x4F8 */ mVec3_c mSpawnPos;
    /* 0x504 */ mAng3_c mEffectRot;
    /* 0x50A */ mAng field_0x50A;
    /* 0x50C */ mAng field_0x50C;
    /* 0x50E */ s16 field_0x50E;
    /* 0x510 */ mAng field_0x510;
    /* 0x512 */ u8 pad_0x512;
    /* 0x513 */ bool mCut;

    static dCcD_SrcCyl sCylSrc;
    static const u32 sCalcAngleRatio;
    static const s16 sEffectRotIncrement;
    static const float sSqDistThreshold;
    static const u32 lbl_268_rodata_4C;
};

#endif
