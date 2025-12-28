#ifndef D_A_B_LASTBOSS_H
#define D_A_B_LASTBOSS_H

#include "d/a/e/d_a_en_base.h"
#include "d/d_cc.h"
#include "d/d_light_env.h"
#include "m/m3d/m_anmmatclr.h"
#include "m/m3d/m_anmtexsrt.h"
#include "m/m3d/m_mdl.h"
#include "m/m3d/m_smdl.h"
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

        /* 0x04 */ s16 field_0x04;
        /* 0x06 */ u8 _0x06[0x34 - 0x06];
        /* 0x34 */ dAcBlastboss_c *mpOwner;
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
    void fn_143_6720();
    void fn_143_A110(s32);
    void fn_143_7B00();

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
    
    /* 0x1134 */ u8 _0x1134[0x113D - 0x1134];

    /* 0x113D */ u8 field_0x113D;
    /* 0x113E */ u8 field_0x113E;
    /* 0x113F */ u8 field_0x113F;
    /* 0x1140 */ u8 field_0x1140;
    /* 0x1141 */ u8 field_0x1141;
    /* 0x1142 */ u8 field_0x1142;
    /* 0x1143 */ u8 field_0x1143;
    /* 0x1144 */ u8 field_0x1144;
    /* 0x1145 */ u8 field_0x1145;
    /* 0x1146 */ bool mIsSwordEmpowered;

    /* 0x1147 */ u8 _0x1147[0x114B - 0x1147];

    /* 0x114B */ u8 field_0x114B;
    /* 0x114C */ u8 field_0x114C;
    /* 0x114D */ u8 field_0x114D;
    /* 0x114E */ u8 field_0x114E;
    /* 0x114F */ u8 field_0x114F;

    /* 0x1150 */ u8 _0x1150[0x1156 - 0x1150];

    /* 0x1156 */ u16 field_0x1156;
    /* 0x1158 */ u16 field_0x1158;
    /* 0x115A */ u16 field_0x115A;
    /* 0x115C */ u16 field_0x115C;
    /* 0x115E */ u16 field_0x115E;
    /* 0x1160 */ u16 field_0x1160;
    
    /* 0x1162 */ u8 _0x1162[0x1164 - 0x1162];
    
    /* 0x1164 */ s16 field_0x1164;
    
    /* 0x1166 */ u8 _0x1166[0x116A - 0x1166];

    /* 0x116A */ s16 field_0x116A;

    /* 0x116C */ u8 _0x116C[0x117C - 0x116C];

    /* 0x117C */ s16 field_0x117C;

    /* 0x1186 */ u8 _0x117E[0x1186 - 0x117E];

    /* 0x1186 */ s16 field_0x1186;

    /* 0x1188 */ u8 _0x1188[0x1190 - 0x1188];

    /* 0x1190 */ f32 field_0x1190;

    /* 0x1194 */ u8 _0x1194[0x119C - 0x1194];

    /* 0x119C */ f32 field_0x119C;
    /* 0x11A0 */ f32 field_0x11A0;
    /* 0x11A4 */ f32 field_0x11A4;

    /* 0x11A8 */ u8 _0x11A8[0x11B0 - 0x11A8];

    /* 0x11B0 */ f32 field_0x11B0;

    /* 0x11B4 */ u8 _0x11B4[0x11BC - 0x11B4];

    /* 0x11BC */ f32 field_0x11BC;
    /* 0x11C0 */ f32 field_0x11C0;

    /* 0x11C4 */ u8 _0x11C4[0x11D4 - 0x11C4];

    /* 0x11D4 */ s32 field_0x11D4;
    /* 0x11D8 */ s32 field_0x11D8;

    /* 0x11DC */ u8 _0x11DC[0x11E4 - 0x11DC];

    /* 0x11E4 */ mVec3_c field_0x11E4;
    /* 0x11F0 */ mVec3_c field_0x11F0;
    /* 0x11FC */ mVec3_c field_0x11FC;
    /* 0x1208 */ mVec3_c field_0x1208;

    /* 0x1214 */ u8 _0x1214[0x1238 - 0x1214];

    /* 0x1238 */ mVec3_c field_0x1238[2];
    /* 0x1250 */ mVec3_c field_0x1250[2];

    /* 0x1268 */ UnkLastBossCcSph2 field_0x1260[8];
    /* 0x2948 */ UnkLastBossCcSph1 field_0x2948[1];

    /* 0x2B64 */ dEmitter_c mEmitter1;
    /* 0x2B98 */ dEmitter_c mEmitter2;
    /* 0x2BCC */ dEmitter_c mEmitter3;
    /* 0x2C00 */ dEmitter_c mEmitter4;
    /* 0x2C34 */ dEmitter_c mEmitter5;
    /* 0x2C68 */ dEmitter_c mEmitter6;
    /* 0x2C9C */ dEmitter_c mEmitter7;
    /* 0x2CD0 */ LIGHT_INFLUENCE mLightInfo;
    /* 0x2CEC */ u16 field_0x2CEC;
    /* 0x2CEE */ u8 _0x2CEE[0x2D20 - 0x2CEE];
    /* 0x2D20 */ dTgSwordBattleGame_c *mpSwordBattleGame;
};

#endif
