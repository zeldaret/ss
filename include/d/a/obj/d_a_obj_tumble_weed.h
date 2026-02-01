#ifndef D_A_OBJ_TUMBLE_WEED_H
#define D_A_OBJ_TUMBLE_WEED_H

#include "common.h"
#include "d/a/obj/d_a_obj_base.h"
#include "d/col/bg/d_bg_s_acch.h"
#include "d/col/cc/d_cc_d.h"
#include "d/d_shadow.h"
#include "m/m3d/m_smdl.h"
#include "m/m_mtx.h"
#include "m/m_quat.h"
#include "m/m_vec.h"
#include "nw4r/g3d/res/g3d_resfile.h"
#include "s/s_State.hpp"
#include "toBeSorted/dowsing_target.h"
#include "toBeSorted/time_area_mgr.h"

class dAcOTumbleWeed_c : public dAcObjBase_c {
public:
    dAcOTumbleWeed_c() : mDowsingTarget(this, DowsingTarget::SLOT_NONE), mStateMgr(*this) {}
    virtual ~dAcOTumbleWeed_c() {}

    virtual int doDelete() override;
    virtual int draw() override;
    virtual bool createHeap() override;
    virtual int actorCreate() override;
    virtual int actorPostCreate() override;
    virtual int actorExecute() override;

    bool checkCollect();
    bool checkBreak();
    void doBreak();

    void adjustTimeScale();
    void adjustTumble(const mVec3_c &dir);
    void calcWind();
    mVec3_c getTumblePosition() const;
    void calcMatrix();

    void adjustAngle();
    void adjustSpeed();
    void tumbleBounceMaybe();

    void setWind(const mVec3_c &);

    bool checkSlope();

    bool fn_475_E40() const;
    bool checkInvalidGround() const;

    // Look at `isStopped` in dAcObjBase. This may be a diff inline
    f32 getWindMag() const {
        return fabsf(nw4r::math::VEC3LenSq(mWind));
    }
    bool isWindStop() const {
        return getWindMag() <= EGG::Math<f32>::epsilon();
    }

    STATE_FUNC_DECLARE(dAcOTumbleWeed_c, Wait);
    STATE_FUNC_DECLARE(dAcOTumbleWeed_c, Slope);

    STATE_MGR_DEFINE_UTIL_EXECUTESTATE(dTgTumbleWeed_c);

private:
    /* 0x330 */ nw4r::g3d::ResFile mResFile;
    /* 0x334 */ m3d::smdl_c mMdl;
    /* 0x350 */ dShadowCircle_c mShdw;
    /* 0x358 */ dCcD_Sph mSph;
    /* 0x4A8 */ dBgS_AcchCir mAcchCir;
    /* 0x504 */ dBgS_ObjAcch mObjAcch;
    /* 0x8B4 */ DowsingTarget mDowsingTarget;
    /* 0x8D4 */ STATE_MGR_DECLARE(dAcOTumbleWeed_c);
    /* 0x910 */ mQuat_c field_0x910;
    /* 0x920 */ TimeAreaStruct mTimeArea;
    /* 0x92C */ mMtx_c mShadowMtx;
    /* 0x95C */ mVec3_c mWind;
    /* 0x968 */ mVec3_c field_0x968;
    /* 0x974 */ f32 field_0x974;
    /* 0x978 */ f32 field_0x978;
    /* 0x97C */ f32 field_0x97C;
    /* 0x980 */ f32 field_0x980;
    /* 0x984 */ f32 mSpeedTarget;
    /* 0x988 */ u16 mTumbleTimer;
    /* 0x98A */ u8 mWindTimer;
    /* 0x98B */ u8 field_0x98B;
    /* 0x98C */ u8 field_0x98C;
    /* 0x98D */ bool mbOnGround;

    static dCcD_SrcSph sSphSrc;
};

#endif
