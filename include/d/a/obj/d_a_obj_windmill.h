#ifndef D_A_OBJ_WINDMILL_H
#define D_A_OBJ_WINDMILL_H

#include "d/a/d_a_base.h"
#include "d/a/obj/d_a_obj_base.h"
#include "d/a/obj/d_a_obj_pinwheel.h"
#include "d/col/bg/d_bg_w.h"
#include "m/m3d/m_anmchr.h"
#include "m/m3d/m_anmmatclr.h"
#include "m/m3d/m_smdl.h"
#include "m/m_angle.h"
#include "m/m_vec.h"
#include "nw4r/g3d/res/g3d_resfile.h"
#include "s/s_State.hpp"
#include "toBeSorted/stage_render_stuff.h"

class dAcOwindmill_c : public dAcObjBase_c {
public:
    dAcOwindmill_c() : mStateMgr(*this, sStateID::null), mCallback(this) {}
    virtual ~dAcOwindmill_c() {}

    virtual bool createHeap() override;
    virtual int create() override;
    virtual int doDelete() override;
    virtual int actorExecute() override;
    virtual int draw() override;

    STATE_FUNC_DECLARE(dAcOwindmill_c, Wait);
    STATE_FUNC_DECLARE(dAcOwindmill_c, CameraMove);
    STATE_FUNC_DECLARE(dAcOwindmill_c, Move);
    STATE_FUNC_DECLARE(dAcOwindmill_c, Collect);
    STATE_FUNC_DECLARE(dAcOwindmill_c, CollectStick);

private:
    bool isWindmillRepaired();
    bool isLightTowerRaised2();
    void attachPinwheel();

    /* 0x330 */ nw4r::g3d::ResFile mResFile;
    /* 0x334 */ m3d::smdl_c mMdl;
    /* 0x330 */ m3d::anmChr_c mAnmChr;
    /* 0x350 */ m3d::anmMatClr_c mAnmClr;
    /* 0x388 */ dBgW mBgW;
    /* 0x5C4 */ STATE_MGR_DECLARE(dAcOwindmill_c);
    /* 0x600 */ dScnCallback_c mCallback;
    /* 0x60C */ dAcRef_c<dAcOpinwheel_c> mPinwheel;
    /* 0x618 */ mVec3_c mWindmillPosition;
    /* 0x624 */ mAng mWindmillRotation;
    /* 0x626 */ s16 mTimer1;
    /* 0x628 */ s16 mTimer2;
    /* 0x62A */ s16 mTimer3;
    /* 0x62C */ u8 mSceneFlagInPlace;
    /* 0x62D */ u8 mSceneFlagBeingBlown;
    /* 0x62E */ u8 mCurrentStuckPos;
    /* 0x62F */ u8 mSetRotation;
    /* 0x630 */ f32 field_0x630;
    /* 0x634 */ f32 field_0x634;
    /* 0x638 */ bool mHasPropellor;
    /* 0x639 */ bool mHasPinwheel;
    /* 0x63A */ bool field_0x63A;

    static const s16 sStateTimer1_0x50;
    static const f32 sTargetHalf;
    static const f32 sTargetOne;
    static const s16 sStateTimer1_0xC;
    static const s16 sStateTimer1_0x32;
    static const s16 sStateTimer2_0xF;
    static const s16 s0xF_unused;

    static s32 sRotationIncrement1;
    static s32 sRotationIncrement2;
    static s32 sRotationIncrement3;
};

#endif
