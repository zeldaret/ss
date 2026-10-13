#ifndef D_A_B_GIRAHIMU_BASE_H
#define D_A_B_GIRAHIMU_BASE_H

#include "common.h"
#include "d/a/d_a_base.h"
#include "d/a/e/d_a_en_base.h"
#include "d/a/npc/d_a_npc.h"
#include "d/a/obj/d_a_obj_base.h"
#include "d/col/bg/d_bg_s_acch.h"
#include "d/col/c/c_cc_d.h"
#include "d/col/cc/d_cc_d.h"
#include "d/d_cc.h"
#include "d/d_message.h"
#include "d/d_shadow.h"
#include "d/snd/d_snd_source_if.h"
#include "m/m3d/m_anmchr.h"
#include "m/m3d/m_anmchrblend.h"
#include "m/m3d/m_anmtexpat.h"
#include "m/m3d/m_fanm.h"
#include "m/m3d/m_mdl.h"
#include "m/m3d/m_smdl.h"
#include "m/m_angle.h"
#include "m/m_mtx.h"
#include "m/m_quat.h"
#include "m/m_vec.h"
#include "nw4r/g3d/g3d_calcworld.h"
#include "nw4r/g3d/res/g3d_resanmchr.h"
#include "nw4r/g3d/res/g3d_resfile.h"
#include "nw4r/g3d/res/g3d_resmdl.h"
#include "s/s_State.hpp"
#include "toBeSorted/d_emitter.h"
#include "toBeSorted/d_enemy_sword_mdl.h"
#include "toBeSorted/d_flow_mgr.h"

class dAcObjGirahimuSwordLink_c;

// Needed for ordering
struct LinkSph {
    dCcD_Linked<dCcD_Sph> mSph;
};
class dAcGirahimuBase_c : public dAcEnBase_c {
public:
    friend class dAcObjGirahimuSwordLink_c;
    friend class dAcObjGirahimuKnife_c;

    dAcGirahimuBase_c() : mStateMgr(*this), mFlowMgr(&mFlow) {}
    virtual ~dAcGirahimuBase_c();

    class callback_c : public m3d::callback_c {
    public:
        friend class dAcGirahimuBase_c;

        callback_c() {}
        virtual ~callback_c() {}
        virtual void timingB(u32, nw4r::g3d::WorldMtxManip *, nw4r::g3d::ResMdl) override;

        void fn_226_DC0(mMtx_c &, const mAng3_c &, const mAng);
        void fn_226_E80();
        void fn_226_1160();
        void init(dAcGirahimuBase_c *pGhirahim);
        void fn_226_1580(u32, nw4r::g3d::WorldMtxManip *, nw4r::g3d::ResMdl);
        void fn_226_1B60(mVec3_c &);
        void fn_226_1DD0();
        void fn_226_24E0();
        void fn_226_24F0();
        void fn_226_2FD0();
        void fn_226_3080();
        void fn_226_3130();
        void fn_226_3150(mMtx_c &);

        /* 0x004 */ mQuat_c field_0x004; // Set, never used
        /* 0x014 */ f32 field_0x014;     // Set, never used
        /* 0x018 */ u16 mNodeID_ShoulderR;
        /* 0x01A */ u16 mNodeID_ShoulderL;
        /* 0x01C */ u16 mNodeID_Hip;
        /* 0x01E */ u16 mNodeID_ArmR;
        /* 0x020 */ u16 mNodeID_ArmL;
        /* 0x022 */ u16 mNodeID_ElbowR;
        /* 0x024 */ u16 mNodeID_ElbowL;
        /* 0x026 */ u16 mNodeID_HandR;
        /* 0x028 */ u16 mNodeID_Head;
        /* 0x02A */ u16 mNodeID_Neck;
        /* 0x02C */ u16 mNodeID_Hair1;
        /* 0x02E */ u16 mNodeID_Hair2;
        /* 0x030 */ u16 mNodeID_Hair3;
        /* 0x032 */ u16 mNodeID_Hair4;
        /* 0x034 */ u16 mNodeID_Spine1;
        /* 0x036 */ u16 mNodeID_Spine2;
        /* 0x038 */ u16 mNodeID_BrowL;
        /* 0x03A */ u16 mNodeID_BrowR;
        /* 0x03C */ u16 mNodeID_Chin;
        /* 0x03E */ u16 mNodeID_MouthL;
        /* 0x040 */ u16 mNodeID_MouthR;
        /* 0x042 */ u16 mNodeID_Upperjaw;
        /* 0x044 */ u16 mNodeID_SklRoot;
        /* 0x046 */ u16 mNodeID_LegR;
        /* 0x048 */ u16 mNodeID_LegL;
        /* 0x04A */ u16 mNodeID_KneeR;
        /* 0x04C */ u16 mNodeID_KneeL;
        /* 0x04E */ bool field_0x04E;
        /* 0x04F */ bool field_0x04F;               // ??
        /* 0x050 */ bool field_0x050;               // ??
        /* 0x052 */ mAng field_0x052;               // unused
        /* 0x054 */ mAng mLegRotationY;             // Leg RotY
        /* 0x056 */ mAng mLegRotationYTarget;       // Leg RotY Target
        /* 0x058 */ mAng mSpineRotationY;           // Spine RotY
        /* 0x05A */ mAng mSpineRotationX;           // Spine RotX
        /* 0x05C */ mAng mSpineRotationYTarget;     // Spine RotY Target
        /* 0x05E */ mAng mSpineRotationXTarget;     // Spine RotX Target
        /* 0x060 */ mAng mSpineRotationYCorrection; // Spine RotY Correction?
        /* 0x062 */ mAng3_c field_0x062;            // Used on Hair and Spine rots
        /* 0x068 */ mAng field_0x068;               // field_0x062 Y Target Adjusted
        /* 0x06A */ mAng field_0x06A;               // field_0x062 Z Target Adjusted
        /* 0x06C */ mAng field_0x06C;               // field_0x062 Z Target
        /* 0x06E */ mAng field_0x06E;               // field_0x062 Y Target
        /* 0x070 */ mVec3_c mHairTranslation;       // Hair translation
        /* 0x07C */ mAng mHeadRotationY;            // Head RotY
        /* 0x07E */ mAng mHeadRotationX;            // Head RotX
        /* 0x080 */ mAng mHeadRotationYTarget;      // Head RotY Target
        /* 0x082 */ mAng mHeadRotationXTarget;      // Head RotX Target
        /* 0x084 */ mAng mHeadRotationYCorrection;  // Head RotY Correction?

        /* 0x086*/ u8 _0x086[0x088 - 0x086];

        /* 0x088 */ bool field_0x088;
        /* 0x08C */ mVec3_c mHeadTranslation; // Head Translation
        /* 0x098 */ f32 field_0x098;          // ??

        /* 0x09C*/ u8 _0x09C[0x0A8 - 0x09C];

        /* 0x0A8 */ mVec3_c field_0x0A8; // Set, never used
        /* 0x0B4 */ mAng field_0x0B4;    // Set, never used

        /* 0x0B6*/ u8 _0x0B6[0x0E8 - 0x0B6];

        /* 0x0E8 */ mMtx_c mElbowRTransform; // ElbowR Transform
        /* 0x118 */ mQuat_c mElbowRRotation; // EbowR Rotation
        /* 0x128 */ mVec3_c field_0x128;     // Set, never used

        /* 0x134*/ u8 _0x134[0x164 - 0x134];

        /* 0x164 */ mVec3_c mElblowRTranslation; // ElbowR Translation

        /* 0x170*/ u8 _0x170[0x17C - 0x170];

        /* 0x17C */ mVec3_c mElbowRScale;        // ElbowR Scale
        /* 0x18C */ f32 mArmRForearmLength;      // ElbowR distance to HandR?
        /* 0x18C */ mVec3_c mArmRUpperArmVector; // ElbowR Translation from ArmR?
        /* 0x198 */ bool mbElbowRTransformGet;   // ElbowR first Retrieve
        /* 0x19C */ mMtx_c mArmRTransform;       // ArmR Transform
        /* 0x1CC */ mQuat_c mArmRRotation;       // ArmR Rotation

        /* 0x1DC*/ u8 _0x1DC[0x1E8 - 0x1DC];

        /* 0x1E8 */ bool mbArmRTransformGet; // ArmR first Retrieve

        /* 0x1E9*/ u8 _0x1E9[0x21C - 0x1E9];

        /* 0x21C */ mVec3_c mArmRTranslation;       // ArmR Translation
        /* 0x228*/ mVec3_c mArmRScale;              // ArmR Scale
        /* 0x234 */ f32 mArmRUpperArmLength;        // ArmR Distance to Elbow?
        /* 0x238 */ mAng mElbowRRotationY;          // ElbowR Y Rot
        /* 0x23A */ mAng mElbowLRotation;           // ElbowL Y Rot
        /* 0x23C */ mAng mElbowRRotationYAmplitude; // ElbowR Y Rot Amp
        /* 0x23E */ mAng mElbowLRotationYAmplitude; // ElbowL Y Rot Amp
        /* 0x240 */ mAng mElbowRRotationYPhase;     // ElbowR Y Rot Phase
        /* 0x242 */ mAng mElbowLRotationYPhase;     // ElbowL Y Rot Phase
        /* 0x244 */ bool field_0x244;               // Set, never used
        /* 0x244 */ bool field_0x245;               // Set, never used

        /* 0x246*/ u8 _0x246[0x254 - 0x246];

        /* 0x254 */ mVec3_c mHandRAdjustedPosition;
        /* 0x260 */ mVec3_c mHandRTranslation; // HandR Translation
        /* 0x26C */ mVec3_c field_0x26C;
        /* 0x278 */ mMtx_c mHandRTransform;   // HandR Transform
        /* 0x2A8 */ mQuat_c mHandRRotation;   // HandR Rotation
        /* 0x2B8 */ bool mbHandRTransformGet; // HandR First Retrieve
        /* 0x2BA */ mAng mHandRRotationY;     // HandR Rot Y
        /* 0x2BC */ s16 field_0x2BC;
        /* 0x2C0 */ nw4r::g3d::WorldMtxManip *mpArmRManip;   // ArmR Manip
        /* 0x2C4 */ nw4r::g3d::WorldMtxManip *mpElbowRManip; // ElbowR Manip
        /* 0x2C8 */ dAcGirahimuBase_c *mpGhirahim;
        /* 0x2CC */ f32 mHandRPositionMaxStep;
        /* 0x2D0 */ f32 mSlerpFactor; // slerp interpolation
        /* 0x2D4 */ f32 mSlerpFactorTarget;
        /* 0x2D8 */ f32 field_0x2D8;
        /* 0x2DC */ f32 field_0x2DC; // unused
        /* 0x2E0 */ mVec3_c field_0x2E0;
        /* 0x2EC */ mVec3_c field_0x2EC; // Set, never used
    };

    enum AnmID_e {
        ANM_NONE = -1,
        ANM_WaitA = 0x00,
        ANM_Walk = 0x01,
        ANM_Catch = 0x02,
        ANM_CatchLoop = 0x03,
        ANM_Damage = 0x04,
        ANM_GetSword = 0x05,
        ANM_WaitBtB = 0x06,
        ANM_AttackLSwordA = 0x07,
        ANM_AttackLSwordB = 0x08,
        ANM_GuardLSwordA = 0x09,
        ANM_GuardLSwordB = 0x0A,
        ANM_MovePose = 0x0B,

        ANM_PickUp02 = 0x0D,
        ANM_PullCenter = 0x0E,
        ANM_PullUp = 0x0F,
        ANM_FreeCenter = 0x10,
        ANM_FreeUp = 0x11,
        ANM_LookSword = 0x12,
        ANM_SwordAppear = 0x13,
        ANM_WaitBtC = 0x14,
        ANM_AttackPoseL = 0x15,
        ANM_AttackLRun = 0x16,
        ANM_AttackSwordL = 0x17,
        ANM_AttackPoseR = 0x18,
        ANM_AttackRRun = 0x19,
        ANM_AttackSwordR = 0x1A,
        ANM_PoseL = 0x1B,
        ANM_PoseR = 0x1C,
        ANM_PoseC = 0x1D,
        ANM_PoseC2 = 0x1E,
        ANM_PoseLR = 0x1F,
        ANM_PoseLU = 0x20,
        ANM_PoseRU = 0x21,
        ANM_PoseUD = 0x22,
        ANM_PoseLAttack = 0x23,
        ANM_PoseRAttack = 0x24,
        ANM_PoseCAttack = 0x25,
        ANM_PoseCAttack2 = 0x26,
        ANM_PoseLRAttack = 0x27,
        ANM_PoseUDAttack = 0x28,
        ANM_PoseLUAttack = 0x29,
        ANM_PoseRUAttack = 0x2A,
        ANM_AttackTwoSword = 0x2B,
        ANM_DamageHit = 0x2C,
        ANM_DamageWait = 0x2D,

        ANM_Call = 0x30,
        ANM_CallWalk = 0x31,
        ANM_AttackKnife = 0x32,
        ANM_FaceTongue = 0x33,
        ANM_FaceSmaile = 0x34,
        ANM_LSwordReturn = 0x35,
        ANM_TurnL = 0x36,
        ANM_TurnR = 0x37,
        ANM_AttackBinta = 0x38,
        ANM_StepStart = 0x39,
        ANM_StepLoop = 0x3A,
        ANM_StepEnd = 0x3B,
        ANM_PoseUpper = 0x3C,
        ANM_PoseFree = 0x3D,
        ANM_PoseUpperAttack = 0x3E,
        ANM_GuardTwoSword = 0x3F,
        ANM_EndA = 0x40,
        ANM_EndBLoop = 0x41,
        ANM_EndC = 0x42,
        ANM_EndDLoop = 0x43,
        ANM_EndE = 0x44,
        ANM_KnifeDamage = 0x45,
    };

    enum AnmIdx_e {
        ANMIDX_Body = 0,
        ANMIDX_Hip = 1,
        ANMIDX_Head = 2,
        ANMIDX_ShoulderR = 3,
        ANMIDX_ShoulderL = 4,
    };

    virtual int doDelete() override;
    virtual int preExecute() override;
    virtual int draw() override;
    virtual bool createHeap() override;
    virtual int actorPostCreate() override;

    /* vt 0x08C-0x094 */ STATE_VIRTUAL_FUNC_DECLARE(dAcGirahimuBase_c, Wait);
    /* vt 0x098-0x0A0 */ STATE_VIRTUAL_FUNC_DECLARE(dAcGirahimuBase_c, Walk);
    /* vt 0x0A4-0x0AC */ STATE_VIRTUAL_FUNC_DECLARE(dAcGirahimuBase_c, Panch);
    /* vt 0x0B0-0x0B8 */ STATE_VIRTUAL_FUNC_DECLARE(dAcGirahimuBase_c, Catch);
    /* vt 0x0BC-0x0C4 */ STATE_VIRTUAL_FUNC_DECLARE(dAcGirahimuBase_c, CatchDamage);
    /* vt 0x0C8-0x0D0 */ STATE_VIRTUAL_FUNC_DECLARE(dAcGirahimuBase_c, GetSword);
    /* vt 0x0D4-0x0DC */ STATE_VIRTUAL_FUNC_DECLARE(dAcGirahimuBase_c, Link_SwordWait);
    /* vt 0x0E0-0x0E8 */ STATE_VIRTUAL_FUNC_DECLARE(dAcGirahimuBase_c, Link_SwordWalk);
    /* vt 0x0EC-0x0F4 */ STATE_VIRTUAL_FUNC_DECLARE(dAcGirahimuBase_c, Link_SwordAttack);
    /* vt 0x0F8-0x100 */ STATE_VIRTUAL_FUNC_DECLARE(dAcGirahimuBase_c, ReturnSword);
    /* vt 0x104-0x10C */ STATE_VIRTUAL_FUNC_DECLARE(dAcGirahimuBase_c, LinkSwordGuardJust);
    /* vt 0x110-0x118 */ STATE_VIRTUAL_FUNC_DECLARE(dAcGirahimuBase_c, SearchSword);
    /* vt 0x11C-0x124 */ STATE_VIRTUAL_FUNC_DECLARE(dAcGirahimuBase_c, HomeWarp);
    /* vt 0x128-0x130 */ STATE_VIRTUAL_FUNC_DECLARE(dAcGirahimuBase_c, PickUpSword);
    /* vt 0x134-0x13C */ STATE_VIRTUAL_FUNC_DECLARE(dAcGirahimuBase_c, ReleaseSword);
    /* vt 0x140-0x148 */ STATE_VIRTUAL_FUNC_DECLARE(dAcGirahimuBase_c, BackStep);
    /* vt 0x14C-0x154 */ STATE_VIRTUAL_FUNC_DECLARE(dAcGirahimuBase_c, Escape);
    /* vt 0x158-0x160 */ STATE_VIRTUAL_FUNC_DECLARE(dAcGirahimuBase_c, EscapeBack);
    /* vt 0x164-0x16C */ STATE_VIRTUAL_FUNC_DECLARE(dAcGirahimuBase_c, FrontWarp);
    /* vt 0x170-0x178 */ STATE_VIRTUAL_FUNC_DECLARE(dAcGirahimuBase_c, FrontAttack);
    /* vt 0x17C-0x184 */ STATE_VIRTUAL_FUNC_DECLARE(dAcGirahimuBase_c, BackWarp);
    /* vt 0x188-0x190 */ STATE_VIRTUAL_FUNC_DECLARE(dAcGirahimuBase_c, BackAttack);
    /* vt 0x194-0x19C */ STATE_VIRTUAL_FUNC_DECLARE(dAcGirahimuBase_c, Counter);
    /* vt 0x1A0-0x1A8 */ STATE_VIRTUAL_FUNC_DECLARE(dAcGirahimuBase_c, Run);
    /* vt 0x1AC-0x1B4 */ STATE_VIRTUAL_FUNC_DECLARE(dAcGirahimuBase_c, RunAttack);
    /* vt 0x1B8-0x1C0 */ STATE_VIRTUAL_FUNC_DECLARE(dAcGirahimuBase_c, BackWalk);
    /* vt 0x1C4-0x1CC */ STATE_VIRTUAL_FUNC_DECLARE(dAcGirahimuBase_c, G_SwordDamage);
    /* vt 0x1D0-0x1D8 */ STATE_VIRTUAL_FUNC_DECLARE(dAcGirahimuBase_c, KnifeDamage);

    STATE_MGR_DEFINE_UTIL_CHANGESTATE(dAcGirahimuBase_c);
    STATE_MGR_DEFINE_UTIL_ISSTATE(dAcGirahimuBase_c);
    STATE_MGR_DEFINE_UTIL_EXECUTESTATE(dAcGirahimuBase_c);
    STATE_MGR_DEFINE_UTIL_GETSTATEID(dAcGirahimuBase_c);

    /* vt 0x1DC */ virtual void vt_0x1DC();
    /* vt 0x1E0 */ virtual bool vt_0x1E0();
    /* vt 0x1E4 */ virtual void vt_0x1E4();
    /* vt 0x1E8 */ virtual void vt_0x1E8();
    /* vt 0x1EC */ virtual void vt_0x1EC();
    /* vt 0x1F0 */ virtual void vt_0x1F0();
    /* vt 0x1F4 */ virtual void vt_0x1F4();
    /* vt 0x1F8 */ virtual void vt_0x1F8();
    /* vt 0x1FC */ virtual void vt_0x1FC() {}
    /* vt 0x200 */ virtual void loadMainResFile();
    /* vt 0x204 */ virtual void setStateIdleAttack();
    /* vt 0x208 */ virtual bool updateSwordGrabAnim();
    /* vt 0x20C */ virtual void vt_0x20C();
    /* vt 0x210 */ virtual bool vt_0x210() {
        return false;
    }
    /* vt 0x214 */ virtual void changeStateEscape();
    /* vt 0x218 */ virtual bool isNearHalfHealth() {
        return false;
    }
    /* vt 0x21C */ virtual bool reactToItemHit(bool);
    /* vt 0x220 */ virtual void vt_0x220();
    /* vt 0x224 */ virtual void setBackAttackPose() {
        setAnm("PoseC", ANM_PoseC2, nullptr, m3d::PLAY_MODE_4, 5.0f, 1.0f);
    }
    /* vt 0x228 */ virtual void correctPoseHip();
    /* vt 0x22C */ virtual void changeStateGetPlayerSword();
    /* vt 0x230 */ virtual void vt_0x230() {
        setAnmHip("WalkBt", ANM_Walk, m3d::PLAY_MODE_4, 5.0f, 1.0f);
    }

    nw4r::g3d::ResAnmChr GetResAnmChr(const char *name) const {
        nw4r::g3d::ResAnmChr res = mResAnmChr1.GetResAnmChr(name);
        if (!res.IsValid()) {
            res = mResAnmChr0.GetResAnmChr(name);
        }

        return res;
    }

    void setAnmRate(f32 rate) {
        for (int i = 0; i < 5; ++i) {
            mAnmChrs[i].setRate(rate);
        }
    }

    void initNodeData();
    void createCollision();

    static s32 getPlayerSwordType();

    // Set Anm Functions
    bool
    setAnm(const char *anmName, s32 anmId, const char *anmNameShoulderL, m3d::playMode_e playMode, f32 f0, f32 rate);
    bool setAnm(
        const char *anmName, s32 anmId, const char *anmNameHip, s32 anmIdHip, const char *anmNameShoulderL,
        s32 anmIdShoulderL, m3d::playMode_e playMode, f32 f0, f32 rate
    );
    bool setAnm(const char *anmName, s32 anmId, m3d::playMode_e playMode, f32 f0, f32 rate);
    bool setAnmBody(const char *anmName, s32 anmId, m3d::playMode_e playMode, f32 f0, f32 rate);
    bool setAnmHip(const char *anmName, s32 anmId, m3d::playMode_e playMode, f32 f0, f32 rate);
    bool setAnmShoulderR(const char *anmName, s32 anmId, m3d::playMode_e playMode, f32 f0, f32 rate);
    bool setAnmShoulderL(const char *anmName, s32 anmId, m3d::playMode_e playMode, f32 f0, f32 rate);
    bool setAnmHead(const char *anmName, s32 anmId, m3d::playMode_e playMode, f32 f0, f32 rate);

    void bindAnmHead();
    void bindAnmBody();
    void bindAnmHip();
    void bindAnmShoulderR();
    void bindAnmShoulderL();

    void updateSpineMovement();
    void updateHeadMovement();
    void updateSwordLocations();
    void updateCollision();
    bool setTurnAnimation(mAng);
    bool adjustHipAngle(mAng, f32);
    bool adjustLegAngle(mAng);
    bool turn(mAng);
    void adjustSpineHitRotation(cCcD_Obj *pObj);
    bool checkCatchPlayerSword();
    mVec3_c getPlayerHeadOffset();
    void executeEarringTransform();
    void calcJumpMovement(f32, f32);
    void updateConsecutiveHits(cCcD_Obj *pObj);
    void setSwordLinkTransform();
    void setSwordLinkTransformPlayer();
    void fn_226_B960(f32);
    void fn_226_BC00();
    void setFrame(f32);

    bool setSound(const char *soundName);
    void fn_226_C060();
    void fn_226_C1C0();
    void fn_226_C320();
    void fn_226_C560();
    void configureBodyShield();
    void configureSwordShield(cCcD_Obj &cc, s16 anm);
    void setTgVulnerable();
    void setTgInvulnerable();
    bool isAttackingLeft();
    void simplifyShoulderLAnmID();
    void simplifyShoulderRAnmID();

    void clearAnms();
    void handleEnterExitEffect();
    void fn_226_D2C0();
    void executeHeadMovement();
    void notifyTgSwordBattleGame();

protected:
    /* 0x0378 */ STATE_MGR_DECLARE(dAcGirahimuBase_c);
    /* 0x03B4 */ m3d::mdl_c mMdlBody;
    /* 0x03D8 */ m3d::anmChrBlend_c mAnmChrBlend;
    /* 0x0400 */ nw4r::g3d::ResFile mResAnmChr0;
    /* 0x0404 */ nw4r::g3d::ResFile mResAnmChr1;
    /* 0x0408 */ m3d::anmChr_c mAnmChrs[5];
    /* 0x0520 */ dSoundSourceIf_c *mpSound;
    /* 0x0524 */ void *mpSoundBrasd;
    /* 0x0528 */ m3d::anmTexPat_c mAnmTexPatWink;
    /* 0x0554 */ dNpcMdlCallbackEye_c mEyeMdlCallback;
    /* 0x0584 */ f32 mEyeTargetOffsetXTarget;
    /* 0x0588 */ f32 mEyeTargetOffsetYTarget;
    /* 0x058C */ f32 mEyeTargetOffsetX;
    /* 0x0590 */ f32 mEyeTargetOffsetY;
    /* 0x0594 */ callback_c mCallback;
    /* 0x088C */ mVec3_c mLookAtTarget;
    /* 0x0898 */ mVec3_c mRelativeHandCatch;
    /* 0x08A4 */ mVec3_c field_0x8A4;
    /* 0x08B0 */ mVec3_c mWarpScale;
    /* 0x08BC */ mVec3_c mWarpScaleSnapshot;
    /* 0x08C8 */ s16 mAnmIDBody;
    /* 0x08CA */ s16 mAnmIDHip;
    /* 0x08CC */ s16 mAnmIDShoulderR;
    /* 0x08CE */ s16 mAnmIDShoulderL;
    /* 0x08D0 */ s16 mAnmIDHead;
    /* 0x08D4 */ dShadowCircle_c mShadow;
    /* 0x08DC */ s32 mCurrentAttack;   // Currently not sure how to describe values
    /* 0x08E0 */ m3d::smdl_c mMdlPias; // Earring
    /* 0x08FC */ mMtx_c mEarringTransform;
    /* 0x092C */ mVec3_c mEarringTranslation;
    /* 0x0938 */ dBgS_AcchCir mAcchCir;
    /* 0x0994 */ dBgS_ObjAcch mAcch;
    /* 0x0D44 */ s16 mSubstate;
    /* 0x0D46 */ s16 mSubstateTimer;
    /* 0x0D48 */ s16 mMovePoseTimer;
    /* 0x0D4A */ s16 mIdleAttackTimer;
    /* 0x0D4C */ s16 mHoldPlayerSwordTimer;
    /* 0x0D4E */ s16 mConsecutiveCatchDamageCount;
    /* 0x0D50 */ s16 field_0xD50;
    /* 0x0D52 */ s16 mCreateKnifeTimer;
    /* 0x0D54 */ s16 mShieldTimer;
    /* 0x0D56 */ s16 field_0xD56; // Set, Never used
    /* 0x0D58 */ s16 field_0xD58;
    /* 0x0D5A */ s16 field_0xD5A;
    /* 0x0D5C */ s16 field_0xD5C;
    /* 0x0D5E */ s16 field_0xD5E;
    /* 0x0D60 */ s16 field_0xD60;         // Slight delay in re-enabling some colliders?
    /* 0x0D62 */ s16 mRecoverDamageTimer; // When timer hits zero, Ghiramim will recover even if you did not hit him
    /* 0x0D64 */ s16 mBlinkTimer;
    /* 0x0D66 */ s16 field_0xD66;
    /* 0x0D68 */ s16 mCatchBlockTimer;
    /* 0x0D6A */ s16 mBossCaptionTimer;
    /* 0x0D6C */ s32 field_0xD6C; // See Notes at bottom
    /* 0x0D70 */ bool mbSpawnedSwords;
    /* 0x0D71 */ bool mbCatchingPlayerSword;
    /* 0x0D72 */ bool mbUsingPlayerSword;
    /* 0x0D74 */ s32 mSwordGrabType;
    /* 0x0D78 */ bool field_0xD78;
    /* 0x0D79 */ bool mbLinkSwordWaitFirstComplete;
    /* 0x0D7A */ bool mbPlayerAttackUpdated;
    /* 0x0D7B */ bool mbHoldingSwordA;
    /* 0x0D7C */ bool field_0xD7C;
    /* 0x0D7D */ bool mbOnGround;
    /* 0x0D7E */ bool field_0xD7E; // Tongue related

    /* 0x0D7F */ u8 _0xD7F;

    /* 0x0D80 */ bool mbSwordAtHit;
    /* 0x0D81 */ bool mbVulnerable;
    /* 0x0D82 */ bool mbShortStun;
    /* 0x0D83 */ bool mbShownBossCaption;
    /* 0x0D84 */ bool mbRequestRun;
    /* 0x0D85 */ bool mbRequestKnife;
    /* 0x0D88 */ s32 mPlayerAttackDir;
    /* 0x0D8C */ bool mbFirstSwordWaitEntered;
    /* 0x0D8D */ bool mbStandardDamage;
    /* 0x0D8E */ s16 mConsecutiveHitCount;
    /* 0x0D90 */ s32 mPreviousHitCutDir;
    /* 0x0D94 */ dEnemySwordMdl_c mMdlSwordA;
    /* 0x12D0 */ mVec3_c mSwordATranslation;
    /* 0x12DC */ f32 mSwordScaleY;
    /* 0x12E0 */ bool mbHoldingSwordB;
    /* 0x12E4 */ dEnemySwordMdl_c mMdlSwordB;
    /* 0x1820 */ mVec3_c mSwordBTranslation;

    /* 0x182C */ u8 _0x182C[0x1830 - 0x182C];

    /* 0x1830 */ f32 field_0x1830; // Set, not used
    /* 0x1834 */ f32 mSpeedTarget;
    /* 0x1838 */ f32 mDistanceToPlayer;
    /* 0x183C */ mAng field_0x183C; // unused
    /* 0x183E */ mAng mAngleToPlayer;
    /* 0x1840 */ mVec3_c mTargetPlayerPos;
    /* 0x184C */ mVec3_c mRunPlayerInRangePosition;
    /* 0x1858 */ f32 mSpeedChangeFactor;
    /* 0x185C */ mVec3_c mTeleportDodgeAmount;
    /* 0x1868 */ f32 mDistanceFromStart;
    /* 0x186C */ f32 field_0x186C;  // Set, not used
    /* 0x1870 */ bool field_0x1870; // unused
    /* 0x1871 */ bool mbFirstTongue;
    /* 0x1872 */ bool mbFirstLinkSwordWaitEnter;

    /* 0x1873 */ u8 _0x1873[0x1878 - 0x1873];

    /* 0x1878 */ dCcD_Cyl mCyl0; // IDK
    /* 0x19C8 */ dCcD_Cyl mCyl1; // Doesnt do much?
    /* 0x1B18 */ dCcD_Linked<dCcD_Cyl> mCcBody;
    /* 0x1C78 */ dCcD_Linked<dCcD_Sph> mCcHand;
    /* 0x1DD8 */ dCcD_Linked<dCcD_Cps> mCcSwordR;
    /* 0x1F58 */ dCcD_Linked<dCcD_Cps> mCcSwordL;
    /* 0x20D8 */ dColliderLinkedList mCollider;
    /* 0x20E4 */ f32 mCyl1Radius;
    /* 0x20E8 */ f32 mCyl1RadiusTarget;
    /* 0x20EC */ mVec3_c mCameraPosition;
    /* 0x20F8 */ mVec3_c mCameraTarget;
    /* 0x2104 */ dAcObjRef_c mRef0; // Unused
    /* 0x2110 */ dAcRef_c<dAcObjGirahimuSwordLink_c> mSwordLink;
    /* 0x211C */ dFlow_c mFlow;
    /* 0x2180 */ dFlowMgrBase_c mFlowMgr;
    /* 0x21D8 */ dEmitter_c mEmitter;
};

// NOTES:
// field_0xD6C
//          case 0:
//                  initializeState_HomeWarp;       // Base
//                  initializeState_BackWarp        // Base
//                  initializeState_FrontWarp;      // Base
//                  initializeState_BackStep;       // Base
//                  initializeState_FrontAttack;    // Base
//                  initializeState_BackAttack;     // Base
//                  executeState_BackAttack;        // Base
//                  initializeState_KnifeAttack;    // G1
//                  initializeState_Death;          // G1, G2
//                  initializeState_DeathMiniGame;  // G1, G2
//                  executeState_CreateKnife;       // G2
//                  initializeState_UpWarp;         // G2
//                  initializeState_WaitKnifeAttack;// G2

// case 1: vt_0x1E4(); // Hit by Sword? -> Try changing to Catch. Knife? -> Catch Damage
//         initializeState_Wait;                   // Base
//         initializeState_Walk;                   // Base, G1
//         initializeState_ReleaseSword;           // Base
//         initializeState_LinkSwordGuardJust;     // Base
//         initializeState_ReturnSword:            // Base
//         initializeState_EscapeBack;             // Base
//         initializeState_CatchDamage;            // G1, G2
//         initializeState_CreateSpinKnife;        // G2
//         executeState_WaitKnifeAttack;           // G2

// case 2: vt_0x1E8()
//         initializeState_GetSword;               // Base
//         initializeState_Link_SwordWait;         // Base
//         initializeState_Link_SwordWalk;         // Base

// case 3: fn_226_C060()
//         initializeState_Run; // Base

// case 4: fn_226_C1C0()
//         initializeState_RunAttack; // Base

// case 5: fn_226_C320() / vt_0x234()
//         executeState_FrontAttack; // Base
//         executeState_BackAttack; // Base
//         initializeState_Counter; // Base
//         executeState_Counter; // Base, G2
//         executeState_UpAttack; // G2

// case 6: fn_227_1EA0() / fn_228_4C20()
//         executeState_Counter; // Base, G2
//         initializeState_BackWalk; // Base
//         initializeState_G_SwordWait; // G1, G2
//         executeState_KnifeAttack; // G1, G2
//         initializeState_Knife; // G1
//         initializeState_CreateKnife; // g2
//         initializeState_KnifePreAttack; // g2
//         initializeState_KnifeAttack; // g2
//         initializeState_UpAttack; // G2
//         executeState_UpAttack; // G2
//         initializeState_Counter; // G2
//         initializeState_GuardJustCounter; // G2
//         executeState_TwoSwordAttack; // G2

// case 7: vt_0x1EC()
//         initializeState_G_SwordDamage; // G1, G2
//         initializeState_G_SwordPiyori; // G1, G2

// case 8: vt_0x1F0()
//         initializeState_KnifeDamage; // Base
//         initializeState_Escape; // Base
//         executeState_RunAttack; // Base

// case 9: fn_228_5400()
//         initializeState_HitAction; // G2
//         initializeState_ComeOn; // G2
//         initializeState_ComeOnGuard; // G2

// case 10:
//         // None

// case 11: fn_228_4A30()
//         initializeState_TwoSwordAttack; // G2

#endif
