#ifndef D_A_E_REMLY_H
#define D_A_E_REMLY_H

#include "common.h"
#include "d/a/d_a_base.h"
#include "d/a/e/d_a_en_base.h"
#include "d/col/bg/d_bg_s_acch.h"
#include "d/col/cc/d_cc_d.h"
#include "d/d_shadow.h"
#include "m/m3d/m_anmtexpat.h"
#include "m/m3d/m_mdl.h"
#include "m/m_angle.h"
#include "m/m_vec.h"
#include "s/s_State.hpp"
#include "toBeSorted/d_d3d.h"
#include "toBeSorted/d_emitter.h"

class dAcBomb_c;
class dAcNpcSkn2_c;
class dAcEremly_c : public dAcEnBase_c {
public:
    class callback_c : public m3d::callback_c {
    public:
        virtual ~callback_c() {}
        virtual void timingB(u32, nw4r::g3d::WorldMtxManip *, nw4r::g3d::ResMdl) override;

        /* 0x04 */ mAng3_c mHeadRotation;
        /* 0x0C */ UNKWORD field_0x0C; // Guess
    };

    enum Animation_e {
        ANM_WaitStand = 0,
        ANM_WaitSit = 1,
        ANM_WaitSitCry = 2,
        ANM_CryWalk = 3,
        ANM_RemlyWalk = 4,
        ANM_Run = 5,
        ANM_Swim = 6,
        ANM_Water = 7,
        ANM_SwimDamage = 8,
        ANM_Wind = 9,
        ANM_WindBack = 10,
        ANM_Hold = 11,
        ANM_HoldMozo = 12,
        ANM_HoldBata = 13,
        ANM_HoldJumpStart = 14,
        ANM_HoldJumpEnd = 15,
        ANM_Fly = 16,
        ANM_FlyDamage = 17,
        ANM_Damage = 18,
        ANM_Sleep = 19,
        ANM_WakeUp = 20,
        ANM_Scared = 21,
        ANM_Piyo1 = 22,
        ANM_Piyo2 = 23,
        ANM_Piyo3 = 24,
        ANM_SleepNight = 25,
        ANM_Demo = 26,
        ANM_27 = 27, // UNUSED
        ANM_Foo = 28,
        ANM_WalkNight = 29,
        ANM_RunFast = 30,
        ANM_Jump = 31,
        ANM_Sound = 32,
    };

public:
    dAcEremly_c() : mStateMgr(*this) {}
    virtual ~dAcEremly_c() {}

    virtual int doDelete() override;
    virtual int draw() override;
    virtual bool createHeap() override;
    virtual int actorCreate() override;
    virtual int actorPostCreate() override;
    virtual int actorExecute() override;

    STATE_FUNC_DECLARE(dAcEremly_c, Wait);
    STATE_FUNC_DECLARE(dAcEremly_c, Walk);
    STATE_FUNC_DECLARE(dAcEremly_c, Run);
    STATE_FUNC_DECLARE(dAcEremly_c, Escape);
    STATE_FUNC_DECLARE(dAcEremly_c, EscapeDash);
    STATE_FUNC_DECLARE(dAcEremly_c, Wind);
    STATE_FUNC_DECLARE(dAcEremly_c, Hold);
    STATE_FUNC_DECLARE(dAcEremly_c, Jump);
    STATE_FUNC_DECLARE(dAcEremly_c, Fly);
    STATE_FUNC_DECLARE(dAcEremly_c, Damage);
    STATE_FUNC_DECLARE(dAcEremly_c, Sleep);
    STATE_FUNC_DECLARE(dAcEremly_c, Scared);
    STATE_FUNC_DECLARE(dAcEremly_c, Stun);
    STATE_FUNC_DECLARE(dAcEremly_c, Water);
    STATE_FUNC_DECLARE(dAcEremly_c, Hear);
    STATE_FUNC_DECLARE(dAcEremly_c, NightSleepDemo);
    STATE_FUNC_DECLARE(dAcEremly_c, NightFoo);
    STATE_FUNC_DECLARE(dAcEremly_c, NightReflectionFoo);
    STATE_FUNC_DECLARE(dAcEremly_c, NightWait);
    STATE_FUNC_DECLARE(dAcEremly_c, NightWalk);
    STATE_FUNC_DECLARE(dAcEremly_c, NightRun);
    STATE_FUNC_DECLARE(dAcEremly_c, NightRet);
    STATE_FUNC_DECLARE(dAcEremly_c, NightJumpAttack);
    STATE_FUNC_DECLARE(dAcEremly_c, BirthWait);

    bool isAnimation(Animation_e state) {
        return mAnimation == (u32)state;
    }

    STATE_MGR_DEFINE_UTIL_EXECUTESTATE(dAcEremly_c);
    STATE_MGR_DEFINE_UTIL_ISSTATE(dAcEremly_c);
    STATE_MGR_DEFINE_UTIL_CHANGESTATE(dAcEremly_c);

private:
    void playWink();
    bool fn_177_6B10(bool lookAtTarget, const mAng &);
    void calcHeadRotation(bool);
    void resetHeadRotation(bool);

    bool fn_177_7040(u32, f32);

    bool isScary();

    bool shouldBeScared();

    bool fn_177_7510(f32);

    bool fn_177_75E0();

    /** false -> compares to camera
     *   true -> compares to player */
    bool fn_177_7650(bool comparePlayer) const;

    void fn_177_77C0();

    // Set Scary Face
    void applyScaryMat();

    void fn_177_79D0(bool);

    bool fn_177_7B10();
    void nightSleepDemoImpl();

    void checkSlope(bool);
    void spawnGroundEffect();
    bool fn_177_86C0();
    bool fn_177_8980(f32);
    bool fn_177_8AC0();
    bool fn_177_8C20(mAng);
    bool fn_177_8F90();
    bool fn_177_9370(f32);

private:
    /* 0x378 */ STATE_MGR_DECLARE(dAcEremly_c);

    /* 0x3B4 */ dShadowCircle_c mShadow;
    /* 0x3BC */ d3d::AnmMdlWrapper mMdl;
    /* 0x438 */ m3d::anmTexPat_c mTexPat;

    /* 0x458 */ dBgS_AcchCir mAcchCir;
    /* 0x3B4 */ dBgS_ObjAcch mAcch;
    /* 0x864 */ dCcD_Sph mSph;

    /* 0x9B4 */ callback_c mMdlCallback;

    /* 0x9C4 */ dEmitter_c mEmitters[2];

    /* 0xA2C */ dAcRef_c<dAcNpcSkn2_c> mRef1; // Temporary until type known
    /* 0xA38 */ dAcRef_c<dAcBomb_c> mNearbyBombRef;

    /* 0xA44 */ mVec3_c mTargetPosition;
    /* 0xA50 */ mVec3_c field_0xA50;

    /* 0xA5C */ dWaterEffect_c mWaterEffect;

    /* 0xAA4 */ mVec3_c field_0xAA4;
    /* 0xAB0 */ mVec3_c field_0xAB0;
    /* 0xABC */ mVec3_c field_0xABC;
    /* 0xAC8 */ mVec3_c field_0xAC8;
    /* 0xAD4 */ mVec3_c field_0xAD4;
    /* 0xAE0 */ mVec3_c field_0xAE0;
    /* 0xAEC */ mVec3_c field_0xAEC;
    /* 0xAF8 */ mVec3_c field_0xAF8;

    /* 0xB04 */ f32 field_0xB04;
    /* 0xB08 */ f32 mPatrolAreaSize;
    /* 0xB0C */ f32 mWaterHeight;
    /* 0xB10 */ f32 mScaleF;
    /* 0xB14 */ s32 mSomeCounter;
    /* 0xB18 */ u32 mSomeCounter2;
    /* 0xB1C */ u8 _B1C[0xB1E - 0xB1C];
    /* 0xB1E */ mAng3_c mSlope;
    /* 0xB24 */ s16 mSlopeXTarget;
    /* 0xB26 */ s16 mSlopeZTarget;
    /* 0xB26 */ u32 mSlopeCheckCounter;
    /* 0xB2C */ s32 mHarpCounter;
    /* 0xB30 */ mAng mHeadTiltTarget;
    /* 0xB32 */ mAng field_0xB32;
    /* 0xB34 */ mAng field_0xB34;
    /* 0xB38 */ f32 mYPosition;
    /* 0xB3C */ f32 mYOffset;
    /* 0xB40 */ f32 mScaleFTarget;
    /* 0xB44 */ u16 field_0xB44;
    /* 0xB46 */ u16 field_0xB46;
    /* 0xB48 */ u16 mAnimTimer;
    /* 0xB4A */ u16 mWinkTimer;
    /* 0xB4C */ u16 mHeadTiltTimer;
    /* 0xB4E */ u16 mAnimSafeTimer;
    /* 0xB50 */ u16 field_0xB50;
    /* 0xB52 */ u16 field_0xB52;
    /* 0xB54 */ u16 mScaleFTimer;
    /* 0xB56 */ u16 field_0xB56;
    /* 0xB58 */ u16 mNoGroundCounter;
    /* 0xB5A */ u16 mSwimPosYCounter;
    /* 0xB5C */ u16 mFlySpeedCounter;
    /* 0xB5E */ u16 mFlyAngleCounter;
    /* 0xB60 */ u8 mAnimation;
    /* 0xB61 */ u8 field_0xB61;
    /* 0xB62 */ u8 mSleepDemoPlayedSceneflag;
    /* 0xB63 */ u8 mNightSleepDemoStep;
    /* 0xB64 */ u8 field_0xB64;
    /* 0xB65 */ u8 mAnmStep;
    /* 0xB66 */ bool field_0xB66;
    /* 0xB67 */ u8 field_0xB67;
    /* 0xB68 */ u8 field_0xB68;
    /* 0xB69 */ u8 field_0xB69;
    /* 0xB6A */ u8 field_0xB6A;
    /* 0xB6B */ bool field_0xB6B;
    /* 0xB6C */ u8 field_0xB6C;
    /* 0xB6D */ u8 field_0xB6D;
    /* 0xB6E */ u8 field_0xB6E;
    /* 0xB6F */ u8 field_0xB6F;
};

#endif
