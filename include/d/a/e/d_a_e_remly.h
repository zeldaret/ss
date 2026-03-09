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

        /* 0x04 */ mAng3_c mAng;
        /* 0x0C */ UNKWORD field_0x0C; // Guess
    };

    enum State_e {
        STATE_0 = 0,
        STATE_1 = 1,
        STATE_2 = 2,
        STATE_3 = 3,
        STATE_4 = 4,
        STATE_5 = 5,
        STATE_6 = 6,
        STATE_7 = 7,
        STATE_8 = 8,
        STATE_9 = 9,
        STATE_10 = 10,
        STATE_11 = 11,
        STATE_12 = 12,
        STATE_13 = 13,
        STATE_14 = 14,
        STATE_15 = 15,
        STATE_16 = 16,
        STATE_17 = 17,
        STATE_18 = 18,
        STATE_19 = 19,
        STATE_20 = 20,
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

    STATE_MGR_DEFINE_UTIL_EXECUTESTATE(dAcEremly_c);
    STATE_MGR_DEFINE_UTIL_ISSTATE(dAcEremly_c);

private:
    void fn_177_6A40();

    // Checks if Batreaux is Human
    bool fn_177_7330();

    bool fn_177_75E0();

    /** false -> comapres to camera
     *   true -> compares to player */
    bool fn_177_7650(bool comparePlayer);

    // Set Scary Face
    void fn_177_78D0();

    bool fn_177_79D0(bool);

    bool fn_177_7B10();

    void fn_177_8520(bool);
    bool fn_177_8980(f32);
    bool fn_177_8AC0();

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

    /* 0xA44 */ mVec3_c field_0xA44;
    /* 0xA50 */ mVec3_c field_0xA50;

    /* 0xA5C */ dWaterEffect_c mWaterEffect;

    /* 0xAA4 */ mVec3_c field_0xAA4;

    /* 0xAB0 */ u8 _AB0[0xAF8 - 0xAB0];

    /* 0xAF8 */ mVec3_c field_0xAF8;

    /* 0xB04 */ f32 field_0xB04;
    /* 0xB08 */ f32 field_0xB08;
    /* 0xB0C */ f32 field_0xB0C;
    /* 0xB10 */ f32 field_0xB10;
    /* 0xB14 */ u8 _B14[0xB1E - 0xB14];
    /* 0xB1E */ mAng3_c field_0xB1E;
    /* 0xB24 */ s16 field_0xB24;
    /* 0xB26 */ s16 field_0xB26;
    /* 0xB28 */ u8 _B28[0xB38 - 0xB28];
    /* 0xB38 */ f32 field_0xB38;
    /* 0xB3C */ f32 field_0xB3C;
    /* 0xB40 */ f32 field_0xB40;
    /*       */ u8 _B44[0xB48 - 0xB44];
    /* 0xB48 */ u16 field_0xB48;
    /*       */ u8 _B4A[0xB4E - 0xB4A];
    /* 0xB4E */ u16 field_0xB4E;
    /*       */ u8 _B50[0xB52 - 0xB50];
    /* 0xB52 */ u16 field_0xB52;
    /* 0xB54 */ u16 field_0xB54;
    /* 0xB56 */ u16 field_0xB56;
    /* 0xB58 */ u16 field_0xB58;
    /*       */ u8 _B56[0xB60 - 0xB5A];
    /* 0xB60 */ u8 field_0xB60; // Some State representation
    /* 0xB61 */ u8 field_0xB61;
    /* 0xB62 */ u8 mSleepDemoPlayedSceneflag;
    /*       */ u8 _B63;
    /* 0xB64 */ u8 field_0xB64;
    /*       */ u8 _B65;
    /* 0xB66 */ u8 field_0xB66;
    /*       */ u8 _B67;
    /* 0xB68 */ u8 field_0xB68;
    /* 0xB69 */ u8 field_0xB69;
    /* 0xB6A */ u8 field_0xB6A;
    /* 0xB6B */ u8 field_0xB6B;
    /* 0xB6C */ u8 field_0xB6C;
};

#endif
