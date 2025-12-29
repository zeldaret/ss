#ifndef D_A_OBJ_ROPE_IGAIGA_H
#define D_A_OBJ_ROPE_IGAIGA_H

#include "d/a/d_a_base.h"
#include "d/a/obj/d_a_obj_base.h"
#include "d/col/bg/d_bg_s_acch.h"
#include "d/col/cc/d_cc_d.h"
#include "m/m3d/m_smdl.h"
#include "m/m_vec.h"
#include "nw4r/g3d/res/g3d_resfile.h"
#include "s/s_State.hpp"
#include "s/s_StateMgr.hpp"

class dAcOivyRope_c;

class dAcOropeIgaiga_c : public dAcObjBase_c {
public:
    dAcOropeIgaiga_c() : mStateMgr(*this, sStateID::null) {}
    virtual ~dAcOropeIgaiga_c() {}

    virtual bool createHeap() override;
    virtual int create() override;
    virtual int doDelete() override;
    virtual int actorExecute() override;
    virtual int draw() override;

    u32 fn_257_1CA0();
    s32 fn_257_1D20();
    void fn_257_1DF0();
    void fn_257_1F20();
    void fn_257_1FD0();
    bool fn_257_1FF0(f32 *posY, bool p2);
    void fn_257_20F0(f32 speed, f32 velocityY);
    void fn_257_21A0();
    bool fn_257_21D0();
    bool fn_257_2310(s16 rotationOffset);
    bool fn_257_2720();

    STATE_FUNC_DECLARE(dAcOropeIgaiga_c, Init);
    STATE_FUNC_DECLARE(dAcOropeIgaiga_c, Rope);
    STATE_FUNC_DECLARE(dAcOropeIgaiga_c, Ground);
    STATE_FUNC_DECLARE(dAcOropeIgaiga_c, Tree);
    STATE_FUNC_DECLARE(dAcOropeIgaiga_c, Stick);
    STATE_FUNC_DECLARE(dAcOropeIgaiga_c, Water);

private:
    /* 0x330 */ nw4r::g3d::ResFile mResFile;
    /* 0x334 */ m3d ::smdl_c mMdl;
    /* 0x350 */ dCcD_Sph mCollider;
    /* 0x4A0 */ dBgS_AcchCir mAcchCir;
    /* 0x4FC */ dBgS_ObjAcch mObjAcch;
    /* 0x8AC */ STATE_MGR_DECLARE(dAcOropeIgaiga_c);
    /* 0x8E8 */ dAcRef_c<dAcOivyRope_c> mIvyRopeRef;
    /* 0x8F4 */ mVec3_c mPositionOffset;
    /* 0x900 */ mVec3_c mPositionRelated;
    /* 0x90C */ mVec3_c mLinkPosDiff;
    /* 0x918 */ u16 mRopeTimer;
    /* 0x91A */ u16 field_0x91A;
    /* 0x91C */ s16 mRotationYOffset;
    /* 0x91E */ s16 field_0x91E;
    /* 0x920 */ s16 mRotationXOffset;
    /* 0x922 */ u8 field_0x922;
    /* 0x923 */ u8 mPnts2Idx;
    /* 0x924 */ u8 mInitTimer;
    /* 0x925 */ u8 mStartStateTree;
    /* 0x926 */ u8 mSceneflag;
    /* 0x927 */ u8 mStickTimer;
    /* 0x928 */ u8 mYOffsetIdx;
    /* 0x929 */ u8 mGroundTimer;
    /* 0x92A */ s8 mVelocityYRelated;
    /* 0x92C */ f32 mLinkSpeedRelated;
    /* 0x930 */ f32 mPositionYRelated;
    /* 0x934 */ f32 mRadius;
    /* 0x938 */ bool field_0x938;
    /* 0x939 */ bool mLinkShieldBash;
    /* 0x93A */ bool mLinkHigher;
    /* 0x93B */ bool field_0x93B;
    /* 0x93C */ bool mSoundPlayed;

    static dCcD_SrcSph sSphSrc;
    static const s16 sRopeTimerMin1;
    static const s16 sRopeTimerMin2;
    static const f32 lbl_257_rodata_4[3];
    static const s16 lbl_257_rodata_10;
    static const s16 lbl_257_rodata_12;
    static const s16 lbl_257_rodata_14;
    static const s16 lbl_257_rodata_16;
    static const s16 lbl_257_rodata_18;
    static const s16 lbl_257_rodata_1A;
    static const s16 lbl_257_rodata_1C;
    static const s16 lbl_257_rodata_1E;
    static const s16 lbl_257_rodata_20;
    static const f32 lbl_257_rodata_28;
    static const s16 lbl_257_rodata_90;
    static const f32 sSquareXZDistanceThreshold;
    static const f32 sLinkCenterTranslationYOffset[6];
};

#endif
