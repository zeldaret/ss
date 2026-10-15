#ifndef D_A_NPC_KNIGHT_LEADER_H
#define D_A_NPC_KNIGHT_LEADER_H

#include "d/a/npc/d_a_ordinary_npc.h"
#include "d/col/bg/d_bg_s_acch.h"
#include "d/col/cc/d_cc_d.h"
#include "m/m3d/m_anmchr.h"
#include "m/m3d/m_smdl.h"
#include "m/m_allocator.h"
#include "s/s_State.hpp"
#include "toBeSorted/actor_event.h"
#include "toBeSorted/d_d3d.h"
#include "toBeSorted/d_path.h"

class dAcNpcKnightLeader_c : public dAcOrdinaryNpc_c {
public:
    struct IndexNode {
        IndexNode() : mIndex(-1) {}
        ~IndexNode() {}
        s32 mIndex;
    };

    dAcNpcKnightLeader_c() : mEvent(*this, nullptr) {}
    virtual ~dAcNpcKnightLeader_c() {}

    static void triggerLordsLogCommentEvent(dAcNpcKnightLeader_c *);

private:
    /* 0x6E4 */ u32 field_0x6E4;
    /* 0x6E8 */ s16 field_0x6E8;
    /* 0x6EA */ u8 field_0x6EA[0x6EC - 0x6EA];
    /* 0x6EC */ d3d::AnmMdlWrapper mAnmMdlWrapper;
    /* 0x75C */ m3d::anmChr_c mAnmChr;
    /* 0x794 */ u8 field_0x794[0x79C - 0x794];
    /* 0x79C */ m3d::mdl_c::mdlCallback_c mMdlCallback;
    /* 0x7D4 */ u8 field_0x7D4[0x7D8 - 0x7D4];
    /* 0x7D8 */ ActorEventRelated mEvent;
    /* 0x828 */ void *field_0x828;
    /* 0x82C */ u8 field_0x82C[0x830 - 0x82C];
    /* 0x830 */ ActorOnRail3 mRail;
    /* 0x854 */ u8 field_0x854[0x878 - 0x854];
    /* 0x878 */ dAcNpcKnightLeader_c *mpThis;
    /* 0x87C */ void *field_0x87C;
    /* 0x880 */ IndexNode field_0x880[3];
    /* 0x88C */ IndexNode field_0x88C[3];
    /* 0x898 */ IndexNode field_0x898[3];
    /* 0x8A4 */ IndexNode field_0x8A4[3];
    /* 0x8B0 */ mHeapAllocator_c mAllocator;
    /* 0x8CC */ m3d::smdl_c mBodyModel;
    /* 0x8EC */ u8 field_0x8EC[0x90C - 0x8EC + 0xC];
    /* 0x90C */ f32 field_0x90C;
    /* 0x910 */ dNpcMdlCallbackMulti_c mMdlCallbackMulti;
    /* 0x920 */ dNpcMdlCallbackYXZ_c mHeadCallback;
    /* 0x954 */ dNpcMdlCallbackYXZ_c mNeckCallback;
    /* 0x988 */ dNpcMdlCallbackYXZ_c mSpineCallback;
    /* 0x9BC */ dNpcMdlCallbackEye_c mEyeCallback;
    /* 0x9EC */ u8 field_0x9EC[0x9F0 - 0x9EC];
    /* 0x9F0 */ s16 field_0x9F0;
    /* 0x9F2 */ s16 field_0x9F2;
    /* 0x9F4 */ s16 field_0x9F4;
    /* 0x9F6 */ s16 field_0x9F6;
    /* 0x9F8 */ u8 field_0x9F8;
    /* 0x9F9 */ u8 field_0x9F9;
    /* 0x9FA */ u8 field_0x9FA[0x9FC - 0x9FA];
    /* 0x9FC */ void *field_0x9FC;
    /* 0xA00 */ s16 field_0xA00;
    /* 0xA02 */ s16 field_0xA02;
    /* 0xA04 */ s16 field_0xA04;
    /* 0xA06 */ u8 field_0xA06[0xA08 - 0xA06];
    /* 0xA08 */ void *field_0xA08;
    /* 0xA0C */ u8 field_0xA0C;
    /* 0xA0D */ u8 field_0xA0D;
    /* 0xA0E */ u8 field_0xA0E[0xA10 - 0xA0E];
    /* 0xA10 */ u32 field_0xA10;
    /* 0xA14 */ void *field_0xA14;
    /* 0xA18 */ void *field_0xA18;
    /* 0xA1C */ void *field_0xA1C;
    /* 0xA20 */ f32 field_0xA20;
    /* 0xA24 */ f32 field_0xA24;
    /* 0xA28 */ f32 field_0xA28;
    /* 0xA2C */ u32 field_0xA2C;
    /* 0xA30 */ u8 field_0xA30[0xA34 - 0xA30];
    /* 0xA34 */ s32 field_0xA34;
    /* 0xA38 */ u8 field_0xA38[0xA4C - 0xA38];
    /* 0xA4C */ u8 field_0xA4C;
    /* 0xA4D */ u8 field_0xA4D[0xA80 - 0xA4D];
    /* 0xA80 */ s32 field_0xA80;
    /* 0xA84 */ u8 field_0xA84[0xA88 - 0xA84];
    /* 0xA88 */ f32 field_0xA88;
    /* 0xA8C */ s16 field_0xA8C;
    /* 0xA8E */ u8 field_0xA8E[0xA90 - 0xA8E];
    /* 0xA90 */ f32 field_0xA90;
    /* 0xA94 */ f32 field_0xA94;
    /* 0xA98 */ u8 field_0xA98[0xA9C - 0xA98];
    /* 0xA9C */ u32 field_0xA9C;
    /* 0xAA0 */ u8 field_0xAA0[0xAA2 - 0xAA0];
    /* 0xAA2 */ u8 field_0xAA2;
    /* 0xAA3 */ u8 field_0xAA3[0xAA4 - 0xAA3];
    /* 0xAA4 */ void *field_0xAA4;
    /* 0xAA8 */ void *field_0xAA8;
    /* 0xAAC */ u8 field_0xAAC[0xAB0 - 0xAAC];
    /* 0xAB0 */ u8 field_0xAB0;
    /* 0xAB1 */ u8 field_0xAB1[0xAB4 - 0xAB1];
    /* 0xAB4 */ void *field_0xAB4;
    /* 0xAB8 */ u32 field_0xAB8;
    /* 0xABC */ dBgS_AcchCir mAcchCir;
    /* 0xB18 */ dBgS_ObjAcch mObjAcch;
    /* 0xEC8 */ dCcD_Cyl mCyl;
    /* 0x1018 */ u32 field_0x1018;
    /* 0x101C */ u8 field_0x101C[0x102C - 0x101C];
    /* 0x102C */ m3d::smdl_c mShadowModel;
    /* 0x1048 */ s16 field_0x1048;
    /* 0x104A */ u8 field_0x104A[0x104C - 0x104A];
    /* 0x104C */ u32 field_0x104C;
    /* 0x1050 */ u32 field_0x1050;
    /* 0x1054 */ s16 field_0x1054;
    /* 0x1056 */ u8 field_0x1056[0x1058 - 0x1056];
    /* 0x1058 */ void *field_0x1058;
    /* 0x105C */ u32 field_0x105C;
    /* total size: 0x1060 */
};

#endif
