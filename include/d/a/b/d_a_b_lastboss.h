#ifndef D_A_B_LASTBOSS_H
#define D_A_B_LASTBOSS_H

#include "d/a/e/d_a_en_base.h"
#include "d/d_cc.h"
#include "d/d_light_env.h"
#include "m/m3d/m_anmmatclr.h"
#include "m/m3d/m_anmtexsrt.h"
#include "m/m3d/m_mdl.h"
#include "m/m3d/m_smdl.h"
#include "m/m_angle.h"
#include "m/m_vec.h"
#include "s/s_State.hpp"
#include "toBeSorted/d_emitter.h"
#include "toBeSorted/d_enemy_sword_mdl.h"
#include "toBeSorted/stage_render_stuff.h"

class dTgSwordBattleGame_c;

// TODO this might be a template but what about the offset?
struct UnkLastBossCcSph1 {
    /* 0x000 */ u8 _0x000[0x008 - 0x000];
    /* 0x008 */ mVec3_c field_0x008[5];
    /* 0x044 */ mVec3_c field_0x044[5];
    /* 0x080 */ mVec3_c field_0x080[5];
    /* 0x0BC */ dCcD_Linked_Sph mCc;
};

struct UnkLastBossCcSph2 {
    /* 0x000 */ s16 field_0x000;
    /* 0x002 */ s16 field_0x002;
    /* 0x004 */ u8 _0x004[0x008 - 0x004];
    /* 0x008 */ s16 field_0x008;
    /* 0x00A */ u8 _0x00A[0x014 - 0x00A];
    /* 0x014 */ mVec3_c field_0x014[10];
    /* 0x08C */ mVec3_c field_0x08C[10];
    /* 0x104 */ mVec3_c field_0x104[10];
    /* 0x17C */ dCcD_Linked_Sph mCc;
};

class dAcBlastboss_c : public dAcEnBase_c {
    class callback_c : public m3d::callback_c {
    public:
        virtual void timingB(u32, nw4r::g3d::WorldMtxManip *, nw4r::g3d::ResMdl) override;

        /* 0x04 */ mAng field_0x04;
        /* 0x06 */ mAng field_0x06;
        /* 0x08 */ mAng field_0x08;
        /* 0x0A */ mAng field_0x0A;
        /* 0x0C */ mAng field_0x0C;
        /* 0x0E */ mAng field_0x0E;
        /* 0x10 */ mAng field_0x10;
        /* 0x12 */ mAng field_0x12;
        /* 0x14 */ mAng field_0x14;
        /* 0x16 */ mAng field_0x16;
        /* 0x18 */ mAng field_0x18;
        /* 0x1A */ mAng field_0x1A;
        /* 0x1C */ mAng field_0x1C[9];
        /* 0x2E */ mAng field_0x2E;
        /* 0x30 */ f32 field_0x30;
        /* 0x34 */ dAcBlastboss_c *mpOwner;
    };

    friend class callback_c;

    // BLastBoss.arc > g3d > model.brres > 3DModels(NW4R) > LastBoss > Bones > ...
    enum BLastBossModelNode_e {
        B_LAST_BOSS_NODE_center = 0,
        B_LAST_BOSS_NODE_backbone1 = 1,
        B_LAST_BOSS_NODE_backbone2 = 2,
        B_LAST_BOSS_NODE_chest = 3,
        B_LAST_BOSS_NODE_neck = 4,
        B_LAST_BOSS_NODE_head = 5,
        B_LAST_BOSS_NODE_browL = 6,
        B_LAST_BOSS_NODE_browR = 7,
        B_LAST_BOSS_NODE_chin = 8,
        B_LAST_BOSS_NODE_hairA1 = 9,
        B_LAST_BOSS_NODE_hairA2 = 10,
        B_LAST_BOSS_NODE_hairA3 = 11,
        B_LAST_BOSS_NODE_hairA4 = 12,
        B_LAST_BOSS_NODE_hairBL1 = 13,
        B_LAST_BOSS_NODE_hairBL2 = 14,
        B_LAST_BOSS_NODE_hairBL3 = 15,
        B_LAST_BOSS_NODE_hairBR1 = 16,
        B_LAST_BOSS_NODE_hairBR2 = 17,
        B_LAST_BOSS_NODE_hairBR3 = 18,
        B_LAST_BOSS_NODE_hairC1 = 19,
        B_LAST_BOSS_NODE_hairC2 = 20,
        B_LAST_BOSS_NODE_hairC3 = 21,
        B_LAST_BOSS_NODE_mouthL = 22,
        B_LAST_BOSS_NODE_mouthR = 23,
        B_LAST_BOSS_NODE_shoulderL = 24,
        B_LAST_BOSS_NODE_armL1 = 25,
        B_LAST_BOSS_NODE_armL2 = 26,
        B_LAST_BOSS_NODE_assistanceL = 27,
        B_LAST_BOSS_NODE_wristL = 28,
        B_LAST_BOSS_NODE_indexL1 = 29,
        B_LAST_BOSS_NODE_indexL2 = 30,
        B_LAST_BOSS_NODE_littleL1 = 31,
        B_LAST_BOSS_NODE_littleL2 = 32,
        B_LAST_BOSS_NODE_middleL1 = 33,
        B_LAST_BOSS_NODE_middleL2 = 34,
        B_LAST_BOSS_NODE_ringL1 = 35,
        B_LAST_BOSS_NODE_ringL2 = 36,
        B_LAST_BOSS_NODE_thumbL1 = 37,
        B_LAST_BOSS_NODE_thumbL2 = 38,
        B_LAST_BOSS_NODE_shoulderR = 39,
        B_LAST_BOSS_NODE_armR1 = 40,
        B_LAST_BOSS_NODE_armR2 = 41,
        B_LAST_BOSS_NODE_assistanceR = 42,
        B_LAST_BOSS_NODE_wristR = 43,
        B_LAST_BOSS_NODE_indexR1 = 44,
        B_LAST_BOSS_NODE_indexR2 = 45,
        B_LAST_BOSS_NODE_littleR1 = 46,
        B_LAST_BOSS_NODE_littleR2 = 47,
        B_LAST_BOSS_NODE_loc_sword01 = 48,
        B_LAST_BOSS_NODE_middleR1 = 49,
        B_LAST_BOSS_NODE_middleR2 = 50,
        B_LAST_BOSS_NODE_ringR1 = 51,
        B_LAST_BOSS_NODE_ringR2 = 52,
        B_LAST_BOSS_NODE_thumbR1 = 53,
        B_LAST_BOSS_NODE_thumbR2 = 54,
        B_LAST_BOSS_NODE_belly = 55,
        B_LAST_BOSS_NODE_waist = 56,
        B_LAST_BOSS_NODE_legL1 = 57,
        B_LAST_BOSS_NODE_legL2 = 58,
        B_LAST_BOSS_NODE_ankleL = 59,
        B_LAST_BOSS_NODE_toeL = 60,
        B_LAST_BOSS_NODE_legR1 = 61,
        B_LAST_BOSS_NODE_legR2 = 62,
        B_LAST_BOSS_NODE_ankleR = 63,
        B_LAST_BOSS_NODE_toeR = 64,
        B_LAST_BOSS_NODE_skirtA1 = 65,
        B_LAST_BOSS_NODE_skirtA2 = 66,
        B_LAST_BOSS_NODE_skirtAU1 = 67,
        B_LAST_BOSS_NODE_skirtB1 = 68,
        B_LAST_BOSS_NODE_skirtB2 = 69,
        B_LAST_BOSS_NODE_skirtB3 = 70,
        B_LAST_BOSS_NODE_skirtB4 = 71,
        B_LAST_BOSS_NODE_skirtB5 = 72,
        B_LAST_BOSS_NODE_skirtB6 = 73,
        B_LAST_BOSS_NODE_skirtB7 = 74,
        B_LAST_BOSS_NODE_skirtB8 = 75,
        B_LAST_BOSS_NODE_skirtB9 = 76,
        B_LAST_BOSS_NODE_skirtBU1 = 77,
        B_LAST_BOSS_NODE_skirtC1 = 78,
        B_LAST_BOSS_NODE_skirtC2 = 79,
        B_LAST_BOSS_NODE_skirtC3 = 80,
        B_LAST_BOSS_NODE_skirtC4 = 81,
        B_LAST_BOSS_NODE_skirtC5 = 82,
        B_LAST_BOSS_NODE_skirtC6 = 83,
        B_LAST_BOSS_NODE_skirtC7 = 84,
        B_LAST_BOSS_NODE_skirtC8 = 85,
        B_LAST_BOSS_NODE_skirtC9 = 86,
        B_LAST_BOSS_NODE_skirtCU1 = 87,
        B_LAST_BOSS_NODE_skirtD1 = 88,
        B_LAST_BOSS_NODE_skirtD2 = 89,
        B_LAST_BOSS_NODE_skirtD3 = 90,
        B_LAST_BOSS_NODE_skirtD4 = 91,
        B_LAST_BOSS_NODE_skirtD5 = 92,
        B_LAST_BOSS_NODE_skirtD6 = 93,
        B_LAST_BOSS_NODE_skirtD7 = 94,
        B_LAST_BOSS_NODE_skirtD8 = 95,
        B_LAST_BOSS_NODE_skirtD9 = 96,
        B_LAST_BOSS_NODE_skirtDU1 = 97,
        B_LAST_BOSS_NODE_skirtE1 = 98,
        B_LAST_BOSS_NODE_skirtE2 = 99,
        B_LAST_BOSS_NODE_skirtE3 = 100,
        B_LAST_BOSS_NODE_skirtE4 = 101,
        B_LAST_BOSS_NODE_skirtE5 = 102,
        B_LAST_BOSS_NODE_skirtE6 = 103,
        B_LAST_BOSS_NODE_skirtE7 = 104,
        B_LAST_BOSS_NODE_skirtE8 = 105,
        B_LAST_BOSS_NODE_skirtE9 = 106,
        B_LAST_BOSS_NODE_skirtEU1 = 107,
        B_LAST_BOSS_NODE_skirtEU2 = 108,
        B_LAST_BOSS_NODE_skirtF1 = 109,
        B_LAST_BOSS_NODE_skirtF2 = 110,
        B_LAST_BOSS_NODE_skirtF3 = 111,
        B_LAST_BOSS_NODE_skirtF4 = 112,
        B_LAST_BOSS_NODE_skirtF5 = 113,
        B_LAST_BOSS_NODE_skirtF6 = 114,
        B_LAST_BOSS_NODE_skirtF7 = 115,
        B_LAST_BOSS_NODE_skirtF8 = 116,
        B_LAST_BOSS_NODE_skirtF9 = 117,
        B_LAST_BOSS_NODE_skirtFU1 = 118,
        B_LAST_BOSS_NODE_skirtG1 = 119,
        B_LAST_BOSS_NODE_skirtG2 = 120,
        B_LAST_BOSS_NODE_skirtG3 = 121,
        B_LAST_BOSS_NODE_skirtG4 = 122,
        B_LAST_BOSS_NODE_skirtG5 = 123,
        B_LAST_BOSS_NODE_skirtG6 = 124,
        B_LAST_BOSS_NODE_skirtG7 = 125,
        B_LAST_BOSS_NODE_skirtG8 = 126,
        B_LAST_BOSS_NODE_skirtG9 = 127,
        B_LAST_BOSS_NODE_skirtGU1 = 128,
        B_LAST_BOSS_NODE_skirtH1 = 129,
        B_LAST_BOSS_NODE_skirtH2 = 130,
        B_LAST_BOSS_NODE_skirtH3 = 131,
        B_LAST_BOSS_NODE_skirtH4 = 132,
        B_LAST_BOSS_NODE_skirtH5 = 133,
        B_LAST_BOSS_NODE_skirtHU1 = 134,
    };

    // TODO: mFightState probably means different things in
    // different states, so maybe we need to either remove
    // this again or split this up further.
    enum SubState_e {
        SUB_STATE_0 = 0,
        SUB_STATE_1,
        SUB_STATE_2,
        SUB_STATE_3,
        SUB_STATE_4,
        SUB_STATE_5,
        SUB_STATE_6,

        SUB_STATE_10 = 10,
        SUB_STATE_11,

        SUB_STATE_20 = 20,
        SUB_STATE_21,
    };

    enum Timer_e {
        TIMER_0,
        TIMER_1,
        TIMER_2,
        TIMER_3,
        TIMER_4,
        TIMER_5,

        TIMER_MAX,
    };

public:
    dAcBlastboss_c() : mStateMgr(*this, sStateID::null), mScnCallback(this) {}
    virtual ~dAcBlastboss_c() {}

    virtual bool createHeap() override;
    int create() override;
    int doDelete() override;
    int actorExecute() override;
    int draw() override;

    STATE_FUNC_DECLARE(dAcBlastboss_c, Fight);
    STATE_FUNC_DECLARE(dAcBlastboss_c, Guard);
    STATE_FUNC_DECLARE(dAcBlastboss_c, GuardBreak);
    STATE_FUNC_DECLARE(dAcBlastboss_c, Attack);
    STATE_FUNC_DECLARE(dAcBlastboss_c, CounterAttack);
    STATE_FUNC_DECLARE(dAcBlastboss_c, PunchAttack);
    STATE_FUNC_DECLARE(dAcBlastboss_c, DashAttack);
    STATE_FUNC_DECLARE(dAcBlastboss_c, SmallAttack);
    STATE_FUNC_DECLARE(dAcBlastboss_c, ThunderAttack);
    STATE_FUNC_DECLARE(dAcBlastboss_c, Damage);
    STATE_FUNC_DECLARE(dAcBlastboss_c, SitDamage);
    STATE_FUNC_DECLARE(dAcBlastboss_c, Down);
    STATE_FUNC_DECLARE(dAcBlastboss_c, Stun);
    STATE_FUNC_DECLARE(dAcBlastboss_c, ThunderWait);

private:
    void setAnm(const char *anim, f32 blend);
    void forceSetAnm(const char *anim, f32 blend);
    void setAnmRate(f32 rate);

    bool checkDamage();
    bool checkForCloseRangeAttack();
    bool fn_143_75A0();
    bool fn_143_77C0();
    void fn_143_7B00();
    void fn_143_7F80();
    bool fn_143_9420();
    u8 fn_143_9570(s32 dir);
    void fn_143_9610();
    void fn_143_A110(s32);

    /* 0x0378 */ d3d::AnmMdlWrapper mMdl;
    /* 0x03E8 */ m3d::anmTexSrt_c mAnmTexSrt;
    /* 0x0414 */ m3d::anmMatClr_c mAnmMatClr1;
    /* 0x0440 */ m3d::smdl_c mThunderMdl;
    /* 0x045C */ m3d::anmMatClr_c mThunderAnmMatClr;
    /* 0x0488 */ callback_c mMdlCallback;
    /* 0x04C0 */ dCcD_Linked_Cyl mCc1;
    /* 0x0620 */ dCcD_Linked_Cyl mCc2;
    /* 0x0780 */ dCcD_Linked_Cyl mCc3;
    /* 0x08E0 */ dCcD_Linked_Sph mCc4;
    /* 0x0A40 */ dCcD_Linked_Sph mCc5;
    /* 0x0BA0 */ dScnCallback_c mScnCallback;
    /* 0x0BAC */ STATE_MGR_DECLARE(dAcBlastboss_c);
    /* 0x0BE8 */ dColliderLinkedList mCcList;
    /* 0x0BF4 */ dEnemySwordMdl_c mSwordMdl;
    /* 0x1130 */ u8 field_0x1130;
    /* 0x1131 */ u8 field_0x1131;
    /* 0x1132 */ u8 field_0x1132;
    /* 0x1133 */ u8 field_0x1133;
    /* 0x1134 */ u8 field_0x1134;
    /* 0x1135 */ s8 field_0x1135;
    /* 0x1136 */ s8 field_0x1136;
    /* 0x1137 */ u8 field_0x1137;
    /* 0x1138 */ u8 field_0x1138;
    /* 0x1139 */ u8 field_0x1139;
    /* 0x113A */ u8 mChanceAttackCounter;
    /* 0x113B */ u8 field_0x113B;
    /* 0x113C */ u8 mChanceCounter;
    /* 0x113D */ u8 field_0x113D;
    /* 0x113E */ u8 field_0x113E;
    /* 0x113F */ u8 field_0x113F;
    /* 0x1140 */ u8 field_0x1140;
    /* 0x1141 */ u8 field_0x1141;
    /* 0x1142 */ u8 field_0x1142;
    /* 0x1143 */ u8 field_0x1143;
    /* 0x1144 */ s8 field_0x1144;
    /* 0x1145 */ s8 mLinkCloseRangeShieldTime;
    /* 0x1146 */ bool mIsSwordEmpowered;
    /* 0x1147 */ u8 field_0x1147;
    /* 0x1148 */ u8 field_0x1148;
    /* 0x1149 */ s8 field_0x1149;
    /* 0x114A */ u8 field_0x114A;
    /* 0x114B */ u8 field_0x114B;
    /* 0x114C */ u8 field_0x114C;
    /* 0x114D */ u8 field_0x114D;
    /* 0x114E */ u8 field_0x114E;
    /* 0x114F */ u8 field_0x114F;
    /* 0x1150 */ u8 field_0x1150;
    /* 0x1152 */ u16 field_0x1152;
    /* 0x1154 */ u16 field_0x1154;
    /* 0x1156 */ u16 mTimers[TIMER_MAX];
    /* 0x1162 */ u16 field_0x1162;
    /* 0x1164 */ s16 field_0x1164;
    /* 0x1166 */ s16 mSubState;

    /* 0x1168 */ u8 _0x1168[0x116A - 0x1168];

    /* 0x116A */ s16 mYAngleToLink;

    /* 0x116C */ u8 _0x116C[0x1172 - 0x116C];

    /* 0x1172 */ s16 field_0x1172;

    /* 0x1174 */ u8 _0x1174[0x1178 - 0x1174];

    /* 0x1178 */ s16 field_0x1178;
    /* 0x117A */ s16 field_0x117A;
    /* 0x117C */ s16 field_0x117C;
    /* 0x117E */ s16 field_0x117E;
    /* 0x1180 */ s16 field_0x1180;
    /* 0x1182 */ s16 field_0x1182;
    /* 0x1184 */ s16 field_0x1184;
    /* 0x1186 */ s16 mYRotationRelativeToLink;
    /* 0x1188 */ s16 field_0x1188;
    /* 0x118A */ mAng field_0x118A;
    /* 0x118C */ mAng field_0x118C;

    /* 0x118E */ u8 _0x118E[0x1190 - 0x118E];

    /* 0x1190 */ f32 mXZDistanceToLink;

    /* 0x1194 */ u8 _0x1194[0x119C - 0x1194];

    /* 0x119C */ f32 field_0x119C;
    /* 0x11A0 */ f32 field_0x11A0;
    /* 0x11A4 */ f32 field_0x11A4;
    /* 0x11A8 */ f32 mAnmRate;

    /* 0x11AC */ u8 _0x11AC[0x11B0 - 0x11AC];

    /* 0x11B0 */ f32 field_0x11B0;

    /* 0x11B4 */ u8 _0x11B4[0x11B8 - 0x11B4];
    
    /* 0x11B8 */ f32 field_0x11B8;

    /* 0x11BC */ f32 field_0x11BC;
    /* 0x11C0 */ f32 field_0x11C0;

    /* 0x11C4 */ u8 _0x11C4[0x11C8 - 0x11C4];

    /* 0x11CC */ f32 field_0x11C8;
    /* 0x11CC */ f32 field_0x11CC;

    /* 0x11D0 */ u8 _0x11D0[0x11D4 - 0x11D0];

    /* 0x11D4 */ s32 mLastAttackDir;
    /* 0x11D8 */ s32 field_0x11D8;

    /* 0x11DC */ mAng3_c field_0x11DC;
    /* 0x11E4 */ mVec3_c mThumbL2Translation;
    /* 0x11F0 */ mVec3_c mBackbone1Translation;
    /* 0x11FC */ mVec3_c mChestTranslation;
    /* 0x1208 */ mVec3_c field_0x1208;

    /* 0x1214 */ u8 _0x1214[0x1220 - 0x1214];

    /* 0x1220 */ mVec3_c field_0x1220;
    /* 0x122C */ mVec3_c field_0x122C;
    /* 0x1238 */ mVec3_c mToeTranslation[2];
    /* 0x1250 */ mVec3_c field_0x1250[2];

    /* 0x1268 */ UnkLastBossCcSph2 field_0x1268[8];
    /* 0x2948 */ UnkLastBossCcSph1 field_0x2948[1];

    /* 0x2B64 */ dEmitter_c mWaterEmitter;
    /* 0x2B98 */ dEmitter_c mEmitter2;
    /* 0x2BCC */ dEmitter_c mEmitter3;
    /* 0x2C00 */ dEmitter_c mEmitter4;
    /* 0x2C34 */ dEmitter_c mEmitter5;
    /* 0x2C68 */ dEmitter_c mEmitter6;
    /* 0x2C9C */ dEmitter_c mEmitter7;
    /* 0x2CD0 */ LIGHT_INFLUENCE mLightInfo;
    /* 0x2CEC */ s16 field_0x2CEC;
    /* 0x2CEE */ u8 _0x2CEE[0x2D00 - 0x2CEE];
    /* 0x2D00 */ f32 field_0x2D00;
    /* 0x2D04 */ u8 _0x2D04[0x2D1C - 0x2D04];
    /* 0x2D1C */ const char *mpCurrentAnm;
    /* 0x2D20 */ dTgSwordBattleGame_c *mpSwordBattleGame;
};

#endif
