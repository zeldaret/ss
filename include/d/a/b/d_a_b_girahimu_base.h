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

        /* 0x004 */ mQuat_c field_0x004;
        /* 0x014 */ f32 field_0x014;
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
        /* 0x04F */ bool field_0x04F;
        /* 0x050 */ bool field_0x050;
        /* 0x052 */ mAng field_0x052;
        /* 0x054 */ mAng field_0x054;
        /* 0x056 */ mAng field_0x056;
        /* 0x058 */ mAng field_0x058;
        /* 0x05A */ mAng field_0x05A;
        /* 0x05C */ mAng field_0x05C;
        /* 0x05E */ mAng field_0x05E;
        /* 0x060 */ mAng field_0x060;
        /* 0x062 */ mAng3_c field_0x062;
        /* 0x068 */ mAng field_0x068;
        /* 0x06A */ mAng field_0x06A;
        /* 0x06C */ mAng field_0x06C;
        /* 0x06E */ mAng field_0x06E;
        /* 0x070 */ mVec3_c field_0x070;
        /* 0x07C */ mAng field_0x07C;
        /* 0x07E */ mAng field_0x07E;
        /* 0x080 */ mAng field_0x080;
        /* 0x082 */ mAng field_0x082;
        /* 0x084 */ mAng field_0x084;

        /* 0x086*/ u8 _0x086[0x088 - 0x086];

        /* 0x088 */ bool field_0x088;
        /* 0x08C */ mVec3_c field_0x08C; // Head Translation
        /* 0x098 */ f32 field_0x098;

        /* 0x09C*/ u8 _0x09C[0x0A8 - 0x09C];

        /* 0x0A8 */ mVec3_c field_0x0A8;
        /* 0x0B4 */ mAng field_0x0B4;

        /* 0x0B6*/ u8 _0x0B6[0x0E8 - 0x0B6];

        /* 0x0E8 */ mMtx_c field_0x0E8;  // ElbowR Transform
        /* 0x118 */ mQuat_c field_0x118; // EbowR Rotation
        /* 0x128 */ mVec3_c field_0x128;

        /* 0x134*/ u8 _0x134[0x164 - 0x134];

        /* 0x164 */ mVec3_c field_0x164; // ElbowR Translation

        /* 0x170*/ u8 _0x170[0x17C - 0x170];

        /* 0x17C */ mVec3_c field_0x17C; // ElbowR Scale
        /* 0x18C */ f32 field_0x188;     // ElbowR distance to HandR?
        /* 0x18C */ mVec3_c field_0x18C; // ElbowR Translation from ArmR?
        /* 0x198 */ bool field_0x198;    // ElbowR first Retrieve
        /* 0x19C */ mMtx_c field_0x19C;  // ArmR Transform
        /* 0x1CC */ mQuat_c field_0x1CC; // ArmR Rotation

        /* 0x1DC*/ u8 _0x1DC[0x1E8 - 0x1DC];

        /* 0x1E8 */ bool field_0x1E8; // ArmR first Retrieve

        /* 0x1E9*/ u8 _0x1E9[0x21C - 0x1E9];

        /* 0x21C */ mVec3_c field_0x21C; // ArmR Translation
        /* 0x228*/ mVec3_c field_0x228;  // ArmR Scale
        /* 0x234 */ f32 field_0x234;     // ArmR Distance to Elbow?
        /* 0x238 */ mAng field_0x238;
        /* 0x23A */ mAng field_0x23A;
        /* 0x23C */ mAng field_0x23C;
        /* 0x23E */ mAng field_0x23E;
        /* 0x240 */ mAng field_0x240;
        /* 0x242 */ mAng field_0x242;
        /* 0x244 */ bool field_0x244;
        /* 0x244 */ bool field_0x245;

        /* 0x246*/ u8 _0x246[0x254 - 0x246];

        /* 0x254 */ mVec3_c field_0x254;
        /* 0x260 */ mVec3_c field_0x260; // HandR Translation
        /* 0x26C */ mVec3_c field_0x26C;
        /* 0x278 */ mMtx_c field_0x278;  // HandR Transform
        /* 0x2A8 */ mQuat_c field_0x2A8; // HandR Rotation
        /* 0x2B8 */ bool field_0x2B8;    // HandR First Retrieve
        /* 0x2BA */ mAng field_0x2BA;    // HandR Pitch
        /* 0x2BC */ s16 field_0x2BC;
        /* 0x2C0 */ nw4r::g3d::WorldMtxManip *field_0x2C0;
        /* 0x2C4 */ nw4r::g3d::WorldMtxManip *field_0x2C4;
        /* 0x2C8 */ dAcGirahimuBase_c *mpGhirahim;
        /* 0x2CC */ f32 field_0x2CC;
        /* 0x2D0 */ f32 field_0x2D0; // slerp interpolation
        /* 0x2D4 */ f32 field_0x2D4;
        /* 0x2D8 */ f32 field_0x2D8;
        /* 0x2DC */ f32 field_0x2DC;
        /* 0x2E0 */ mVec3_c field_0x2E0;
        /* 0x2EC */ mVec3_c field_0x2EC;
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

        ANM_DamageHit = 0x2C,
        ANM_DamageWait = 0x2D,

        ANM_Call = 0x30,

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
    /* vt 0x200 */ virtual void vt_0x200();
    /* vt 0x204 */ virtual void vt_0x204();
    /* vt 0x208 */ virtual bool vt_0x208();
    /* vt 0x20C */ virtual void vt_0x20C();
    /* vt 0x210 */ virtual bool vt_0x210() {
        return 0;
    }
    /* vt 0x214 */ virtual void vt_0x214();
    /* vt 0x218 */ virtual bool vt_0x218() {
        return false;
    }
    /* vt 0x21C */ virtual bool vt_0x21C(bool);
    /* vt 0x220 */ virtual void vt_0x220();
    /* vt 0x224 */ virtual void vt_0x224() {
        setAnm("PoseC", ANM_PoseC2, nullptr, m3d::PLAY_MODE_4, 5.0f, 1.0f);
    }
    /* vt 0x228 */ virtual void vt_0x228();
    /* vt 0x22C */ virtual void vt_0x22C();
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

    void fn_226_9620();
    void fn_226_9690(); // Head/Hair and Attention related
    void fn_256_99F0();
    void fn_256_9C90();
    bool fn_226_A070(mAng);
    bool fn_226_A120(mAng, f32);
    bool fn_226_A280(mAng);
    bool turn(mAng);
    void fn_226_A400(cCcD_Obj *pObj);
    bool fn_226_A570();
    mVec3_c getPlayerHeadOffset();
    void executeEarringTransform();
    void calcJumpMovement(f32, f32);
    void fn_226_ADC0(cCcD_Obj *pObj);
    void setSwordLinkTransform();
    void setSwordLinkTransformPlayer();
    void fn_226_AFB0();

    void fn_226_B960(f32);
    void fn_226_BC00();
    void setFrame(f32);

    bool fn_226_BE90(const char *soundName);
    void fn_226_C060();
    void fn_226_C1C0();
    void fn_226_C320();
    void fn_226_C560();
    void fn_226_C6A0();
    void fn_226_C790(cCcD_Obj &cc, s16 anm);
    void fn_226_CC40();
    void fn_226_CC50();
    bool fn_226_CC80();
    void fn_226_CDA0();
    void fn_226_CE00();

    void fn_226_D070();
    void fn_226_D090();
    void fn_226_D2C0();
    void fn_226_D2D0();
    void fn_226_D5A0();

protected:
    /* 0x0378 */ STATE_MGR_DECLARE(dAcGirahimuBase_c);
    /* 0x03B4 */ m3d::mdl_c mMdlBody;
    /* 0x03D8 */ m3d::anmChrBlend_c mAnmChrBlend;
    /* 0x0400 */ nw4r::g3d::ResFile mResAnmChr0;
    /* 0x0404 */ nw4r::g3d::ResFile mResAnmChr1;
    /* 0x0408 */ m3d::anmChr_c mAnmChrs[5];
    /* 0x0520 */ dSoundSourceIf_c *pSoundIface;
    /* 0x0524 */ void *mpSoundBrasd;
    /* 0x0528 */ m3d::anmTexPat_c mAnmTexPatWink;
    /* 0x0554 */ dNpcMdlCallbackEye_c mEyeMdlCallback;
    /* 0x0584 */ f32 field_0x584;
    /* 0x0588 */ f32 field_0x588;
    /* 0x058C */ f32 field_0x58C;
    /* 0x0590 */ f32 field_0x590;
    /* 0x0594 */ callback_c mCallback;
    /* 0x088C */ mVec3_c field_0x88C;
    /* 0x0898 */ mVec3_c field_0x898;
    /* 0x08A4 */ mVec3_c field_0x8A4;
    /* 0x08B0 */ mVec3_c field_0x8B0;
    /* 0x08BC */ mVec3_c field_0x8BC;
    /* 0x08C8 */ s16 mAnmIDBody;
    /* 0x08CA */ s16 mAnmIDHip;
    /* 0x08CC */ s16 mAnmIDShoulderR;
    /* 0x08CE */ s16 mAnmIDShoulderL;
    /* 0x08D0 */ s16 mAnmIDHead;
    /* 0x08D4 */ dShadowCircle_c mShadow;
    /* 0x08DC */ s32 field_0x8DC;
    /* 0x08E0 */ m3d::smdl_c mMdlPias; // Earring
    /* 0x08FC */ mMtx_c field_0x8FC;
    /* 0x092C */ mVec3_c field_0x92C;
    /* 0x0938 */ dBgS_AcchCir mAcchCir;
    /* 0x0994 */ dBgS_ObjAcch mAcch;
    /* 0x0D44 */ s16 field_0xD44;
    /* 0x0D46 */ s16 field_0xD46;
    /* 0x0D48 */ s16 field_0xD48;
    /* 0x0D4A */ s16 field_0xD4A;
    /* 0x0D4C */ s16 field_0xD4C;
    /* 0x0D4E */ s16 field_0xD4E;
    /* 0x0D50 */ s16 field_0xD50;
    /* 0x0D52 */ s16 field_0xD52;
    /* 0x0D54 */ s16 field_0xD54;
    /* 0x0D56 */ s16 field_0xD56;
    /* 0x0D58 */ s16 field_0xD58;
    /* 0x0D5A */ s16 field_0xD5A;
    /* 0x0D5C */ s16 field_0xD5C;
    /* 0x0D5E */ s16 field_0xD5E;
    /* 0x0D60 */ s16 field_0xD60;
    /* 0x0D62 */ s16 field_0xD62;
    /* 0x0D64 */ s16 field_0xD64;
    /* 0x0D66 */ s16 field_0xD66;
    /* 0x0D68 */ s16 field_0xD68;
    /* 0x0D6A */ s16 mBossCaptionTimer;
    /* 0x0D6C */ s32 field_0xD6C;
    /* 0x0D70 */ bool field_0xD70;
    /* 0x0D71 */ bool field_0xD71;
    /* 0x0D72 */ bool field_0xD72;
    /* 0x0D74 */ s32 field_0xD74;
    /* 0x0D78 */ bool field_0xD78;
    /* 0x0D79 */ bool mbLinkSwordWaitFirstComplete;
    /* 0x0D7A */ bool field_0xD7A;
    /* 0x0D7B */ bool field_0xD7B;
    /* 0x0D7C */ bool field_0xD7C;
    /* 0x0D7D */ bool field_0xD7D;
    /* 0x0D7E */ bool field_0xD7E;

    /* 0x0D7F */ u8 _0xD7F;

    /* 0x0D80 */ bool field_0xD80;
    /* 0x0D81 */ bool field_0xD81;
    /* 0x0D82 */ bool field_0xD82;
    /* 0x0D83 */ bool mbShownBossCaption;
    /* 0x0D84 */ bool field_0xD84;
    /* 0x0D85 */ bool field_0xD85;
    /* 0x0D88 */ u32 mPlayerAttackDir;
    /* 0x0D8C */ bool field_0xD8C;
    /* 0x0D8D */ bool field_0xD8D;
    /* 0x0D8E */ s16 field_0xD8E;
    /* 0x0D90 */ s32 mPreviousHitCutDir;
    /* 0x0D94 */ dEnemySwordMdl_c mMdlSwordA;
    /* 0x12D0 */ mVec3_c field_0x12D0; // SwordA Translation
    /* 0x12DC */ f32 field_0x12DC;
    /* 0x12E0 */ bool field_0x12E0;
    /* 0x12E4 */ dEnemySwordMdl_c mMdlSwordB;
    /* 0x1820 */ mVec3_c field_0x1820; // SwordB Translation

    /* 0x182C */ u8 _0x182C[0x1830 - 0x182C];

    /* 0x1830 */ f32 field_0x1830;
    /* 0x1834 */ f32 field_0x1834;
    /* 0x1838 */ f32 field_0x1838;
    /* 0x183C */ mAng field_0x183C;
    /* 0x183E */ mAng field_0x183E;
    /* 0x1840 */ mVec3_c field_0x1840;
    /* 0x184C */ mVec3_c field_0x184C;
    /* 0x1858 */ f32 field_0x1858;
    /* 0x185C */ mVec3_c field_0x185C;
    /* 0x1868 */ f32 field_0x1868;
    /* 0x186C */ f32 field_0x186C;
    /* 0x1870 */ bool field_0x1870;
    /* 0x1871 */ bool field_0x1871;
    /* 0x1872 */ bool field_0x1872;

    /* 0x1873 */ u8 _0x1873[0x1878 - 0x1873];

    /* 0x1878 */ dCcD_Cyl mCyl0;
    /* 0x19C8 */ dCcD_Cyl mCyl1;
    /* 0x1B18 */ dCcD_Linked<dCcD_Cyl> mCyl2;
    /* 0x1C78 */ dCcD_Linked<dCcD_Sph> mSph0;
    /* 0x1DD8 */ dCcD_Linked<dCcD_Cps> mCps0;
    /* 0x1F58 */ dCcD_Linked<dCcD_Cps> mCps1;
    /* 0x20D8 */ dColliderLinkedList mCollider;
    /* 0x20E4 */ f32 field_0x20E4;
    /* 0x20E8 */ f32 field_0x20E8;
    /* 0x20EC */ mVec3_c field_0x20EC;
    /* 0x20F8 */ mVec3_c field_0x20F8;
    /* 0x2104 */ dAcObjRef_c mRef0;
    /* 0x2110 */ dAcRef_c<dAcObjGirahimuSwordLink_c> mSwordLink;
    /* 0x211C */ dFlow_c mFlow;
    /* 0x2180 */ dFlowMgrBase_c mFlowMgr;
    /* 0x21D8 */ dEmitter_c mEmitter;
};

#endif
