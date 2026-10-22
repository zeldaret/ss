#ifndef D_A_ORDINARY_NPC_H
#define D_A_ORDINARY_NPC_H

#include "common.h"
#include "d/a/npc/d_a_npc.h"
#include "d/col/bg/d_bg_s_acch.h"
#include "d/col/c/c_cc_d.h"
#include "m/m3d/m_anmchr.h"
#include "m/m3d/m_anmmdl.h"
#include "m/m3d/m_anmtexpat.h"
#include "m/m3d/m_smdl.h"
#include "m/m_allocator.h"
#include "m/m_angle.h"
#include "m/m_color.h"
#include "nw4r/g3d/res/g3d_resanmchr.h"
#include "s/s_State.hpp"
#include "toBeSorted/actor_event.h"
#include "toBeSorted/d_d3d.h"
#include "toBeSorted/d_path.h"

// dAcNpc_c defintion or dAcOrdinary is unsure
class dNpcAttention_c {
public:
    virtual ~dNpcAttention_c();

private:
    /* 0x04 */ mColor mClr0[3];
    /* 0x10 */ mColor mClr1[3];
    /* 0x1C */ mColor mClr2[3];
    /* 0x28 */ mColor mClr3[3];
    /* 0x34 */ mHeapAllocator_c mAllocator;
    /* 0x50 */ m3d::smdl_c mMdl;
    /* 0x6C */ m3d::anmTexPat_c *mpTexPat;
    /* 0x70 */ u8 _0x70[0x80 - 0x70];
};

class dNpcPath_c {
public:
    dNpcPath_c(dAcNpc_c *pNpc) : mpNpc(pNpc) {}
    virtual ~dNpcPath_c() {}

    /* 0x04 */ u8 field_0x04;
    /* 0x08 */ ActorOnRail3 mPath;

    /* 0x?? */ dAcNpc_c *mpNpc;
};

class dcNpcMdlCallback0 : public m3d::mdl_c::mdlCallback_c {};

class dAcOrdinaryNpc_c : public dAcNpc_c {
public:
    dAcOrdinaryNpc_c(cCcD_Obj *pCc, dBgS_Acch *pAcch)
        : mEventRelated(*this, 0), mPath(this), mpCc(pCc), mpAcch(pAcch) {}
    ~dAcOrdinaryNpc_c() {}

    virtual int preCreate() override;
    virtual void postCreate(MAIN_STATE_e state) override;
    virtual int create() override;
    virtual int doDelete() override;
    virtual int preExecute() override;
    virtual bool createHeap() override;
    virtual int actorExecute() override;

    /* vt 0x160 */ virtual void acNpc_vt_0x160() override {
        acOrdinaryNpc_vt_0x2D4(0, 0, 0);
        fn_80047780();
    }

    /* vt 0x1F4 */ virtual int acNpc_vt_0x1F4() override;

    /* vt 0x208 */ virtual const char *acOrdinaryNpc_vt_0x208() = 0; // Res Name
    /* vt 0x20C */ virtual const char *acOrdinaryNpc_vt_0x20C() = 0; // Model Name
    /* vt 0x210 */ virtual const char *acOrdinaryNpc_vt_0x210() = 0; // AnmName
    /* vt 0x214 */ virtual const char *acOrdinaryNpc_vt_0x214() {
        return "m_Eyeball";
    }
    /* vt 0x218 */ virtual UNKTYPE acOrdinaryNpc_vt_0x218() = 0;
    /* vt 0x21C */ virtual UNKTYPE acOrdinaryNpc_vt_0x21C() = 0;
    /* vt 0x220 */ virtual UNKTYPE acOrdinaryNpc_vt_0x220() = 0;
    /* vt 0x224 */ virtual UNKTYPE acOrdinaryNpc_vt_0x224() = 0;

    STATE_VIRTUAL_OVERRIDE_FUNC_DECLARE(dAcOrdinaryNpc_c, dAcNpc_c, Wait);
    STATE_VIRTUAL_FUNC_DECLARE(dAcOrdinaryNpc_c, Pain);
    STATE_VIRTUAL_FUNC_DECLARE(dAcOrdinaryNpc_c, Surprised);
    STATE_VIRTUAL_FUNC_DECLARE(dAcOrdinaryNpc_c, Withstand);
    STATE_VIRTUAL_FUNC_DECLARE(dAcOrdinaryNpc_c, Walk);
    STATE_VIRTUAL_FUNC_DECLARE(dAcOrdinaryNpc_c, LookAwaySt);
    STATE_VIRTUAL_FUNC_DECLARE(dAcOrdinaryNpc_c, LookAway);
    STATE_VIRTUAL_FUNC_DECLARE(dAcOrdinaryNpc_c, Turn);
    STATE_FUNC_DECLARE(dAcOrdinaryNpc_c, PreWalkTurn);

    /* vt 0x27C */ virtual UNKTYPE acOrdinaryNpc_vt_0x27C();
    /* vt 0x280 */ virtual UNKWORD acOrdinaryNpc_vt_0x280() {
        return 60;
    }
    /* vt 0x284 */ virtual UNKWORD acOrdinaryNpc_vt_0x284() {
        return 120;
    }
    /* vt 0x288 */ virtual UNKWORD acOrdinaryNpc_vt_0x288() {
        return 60;
    }
    /* vt 0x28C */ virtual UNKWORD acOrdinaryNpc_vt_0x28C() {
        return 120;
    }
    /* vt 0x290 */ virtual f32 acOrdinaryNpc_vt_0x290() {
        return 1.0f;
    }
    /* vt 0x294 */ virtual f32 acOrdinaryNpc_vt_0x294() {
        return 5.0f;
    }
    /* vt 0x298 */ virtual f32 acOrdinaryNpc_vt_0x298() {
        return 1200.0f;
    }

    /* vt 0x29C */ virtual UNKWORD acOrdinaryNpc_vt_0x29C() {
        mSpeed = 0.0f;
        mVelocity.set(0.0f, 0.0f, 0.0f);
        return 0;
    }
    /* vt 0x2A0 */ virtual UNKWORD acOrdinaryNpc_vt_0x2A0() {
        mSpeed = 0.0f;
        mVelocity.set(0.0f, 0.0f, 0.0f);
        return 0;
    }
    /* vt 0x2A4 */ virtual UNKWORD acOrdinaryNpc_vt_0x2A4() {
        mSpeed = 0.0f;
        mVelocity.set(0.0f, 0.0f, 0.0f);
        return 0;
    }
    /* vt 0x2A8 */ virtual void acOrdinaryNpc_vt_0x2A8(f32) {
        mSpeed = 0.0f;
        mVelocity.set(0.0f, 0.0f, 0.0f);
    }
    /* vt 0x2AC */ virtual UNKTYPE acOrdinaryNpc_vt_0x2AC();
    /* vt 0x2B0 */ virtual void acOrdinaryNpc_vt_0x2B0(f32) {
        mSpeed = 0.0f;
        mVelocity.set(0.0f, 0.0f, 0.0f);
    }
    /* vt 0x2B4 */ virtual void acOrdinaryNpc_vt_0x2B4(f32) {
        mSpeed = 0.0f;
        mVelocity.set(0.0f, 0.0f, 0.0f);
    }
    /* vt 0x2B8 */ virtual UNKTYPE acOrdinaryNpc_vt_0x2B8();
    /* vt 0x2BC */ virtual void acOrdinaryNpc_vt_0x2BC(f32) {
        mSpeed = 0.0f;
        mVelocity.set(0.0f, 0.0f, 0.0f);
        field_0x9F0.resetTarget(
            acOrdinaryNpc_vt_0x340(), acOrdinaryNpc_vt_0x344(), acOrdinaryNpc_vt_0x348(), &field_0x9BC
        );
    }
    /* vt 0x2C0 */ virtual bool acOrdinaryNpc_vt_0x2C0() {
        return /* some function call == */ 0;
    }
    /* vt 0x2C4 */ virtual UNKTYPE acOrdinaryNpc_vt_0x2C4();
    /* vt 0x2C8 */ virtual f32 acOrdinaryNpc_vt_0x2C8() {
        return acOrdinaryNpc_vt_0x294();
    }
    /* vt 0x2CC */ virtual UNKTYPE acOrdinaryNpc_vt_0x2CC();
    /* vt 0x2D0 */ virtual UNKTYPE acOrdinaryNpc_vt_0x2D0();
    /* vt 0x2D4 */ virtual UNKTYPE acOrdinaryNpc_vt_0x2D4(UNKWORD, UNKWORD, UNKWORD);
    /* vt 0x2D8 */ virtual UNKTYPE acOrdinaryNpc_vt_0x2D8();
    /* vt 0x2DC */ virtual UNKTYPE acOrdinaryNpc_vt_0x2DC();
    /* vt 0x2E0 */ virtual UNKTYPE acOrdinaryNpc_vt_0x2E0();
    /* vt 0x2E4 */ virtual UNKTYPE acOrdinaryNpc_vt_0x2E4();
    /* vt 0x2E8 */ virtual UNKTYPE acOrdinaryNpc_vt_0x2E8(void *resFile) {
        acOrdinaryNpc_vt_0x35C()->create3(
            *this, resFile, acOrdinaryNpc_vt_0x20C(), acOrdinaryNpc_vt_0x210(), acOrdinaryNpc_vt_0x358(), 0x327
        );
    }
    /* vt 0x2EC */ virtual nw4r::g3d::ResAnmChr acOrdinaryNpc_vt_0x2EC() {
        return acOrdinaryNpc_vt_0x35C()->getMdlFile().GetResAnmChr(acOrdinaryNpc_vt_0x210());
    }
    /* vt 0x2F0 */ virtual UNKWORD acOrdinaryNpc_vt_0x2F0() {
        return 1;
    }
    /* vt 0x2F4 */ virtual UNKWORD acOrdinaryNpc_vt_0x2F4() {
        return 1;
    }
    /* vt 0x2F8 */ virtual void acOrdinaryNpc_vt_0x2F8() {}
    /* vt 0x2FC */ virtual UNKTYPE acOrdinaryNpc_vt_0x2FC() {}
    /* vt 0x300 */ virtual void acOrdinaryNpc_vt_0x300() {}
    /* vt 0x304 */ virtual UNKTYPE acOrdinaryNpc_vt_0x304();
    /* vt 0x308 */ virtual void acOrdinaryNpc_vt_0x308() {}
    /* vt 0x30C */ virtual UNKWORD acOrdinaryNpc_vt_0x30C() {
        return 1;
    }
    /* vt 0x310 */ virtual UNKWORD acOrdinaryNpc_vt_0x310() {
        return 1;
    }
    /* vt 0x314 */ virtual UNKWORD acOrdinaryNpc_vt_0x314() {
        return 0;
    }
    /* vt 0x318 */ virtual UNKWORD acOrdinaryNpc_vt_0x318() {
        return 1;
    }
    /* vt 0x31C */ virtual UNKWORD acOrdinaryNpc_vt_0x31C() {
        return 0;
    }
    /* vt 0x320 */ virtual UNKWORD acOrdinaryNpc_vt_0x320() {
        return 1;
    }
    /* vt 0x324 */ virtual UNKTYPE acOrdinaryNpc_vt_0x324();
    /* vt 0x328 */ virtual void acOrdinaryNpc_vt_0x328() {}
    /* vt 0x32C */ virtual UNKWORD acOrdinaryNpc_vt_0x32C() {
        return 1;
    }
    /* vt 0x330 */ virtual UNKWORD acOrdinaryNpc_vt_0x330() {
        return 0;
    }
    /* vt 0x334 */ virtual UNKTYPE acOrdinaryNpc_vt_0x334();
    /* vt 0x338 */ virtual void acOrdinaryNpc_vt_0x338() {}
    /* vt 0x33C */ virtual UNKTYPE acOrdinaryNpc_vt_0x33C();
    /* vt 0x340 */ virtual dNpcMdlCallbackAng_c *acOrdinaryNpc_vt_0x340() {
        return &field_0x920;
    }
    /* vt 0x344 */ virtual dNpcMdlCallbackAng_c *acOrdinaryNpc_vt_0x344() {
        return &field_0x954;
    }
    /* vt 0x348 */ virtual dNpcMdlCallbackAng_c *acOrdinaryNpc_vt_0x348() {
        return &field_0x988;
    }
    /* vt 0x34C */ virtual void acOrdinaryNpc_vt_0x34C() {}
    /* vt 0x350 */ virtual UNKTYPE acOrdinaryNpc_vt_0x350();
    /* vt 0x354 */ virtual void acOrdinaryNpc_vt_0x354() {
        field_0xA2C = nullptr;
    }
    /* vt 0x358 */ virtual dcNpcMdlCallback0 *acOrdinaryNpc_vt_0x358() {
        return &mMdlCallback;
    }
    /* vt 0x35C */ virtual d3d::AnmMdlWrapper *acOrdinaryNpc_vt_0x35C();

    /* vt 0x204 */ virtual int acNpc_vt_0x204() override {
        return 1;
    }

    void fn_80047780();

    bool setField0xA7CFromParams(); // fn_8004AD00
    bool resolvePathId();           // fn_8004AD30
    s32 getPathId();                // fn_8004AD70

    void setField0xA4C(const mAng3_c &); // fn_80046450
protected:
    /* 0x6E4 */ u8 _0x6E4[8];
    /* 0x6EC */ d3d::AnmMdlWrapper mMdlWrapper;
    /* 0x75C */ m3d::anmChr_c mAnmChr;
    /* 0x    */ dcNpcMdlCallback0 mMdlCallback;

    /* 0x7D4 */ void *field_0x7D4;
    /* 0x7D8 */ ActorEventRelated mEventRelated;

    dNpcPath_c mPath;

    /* 0x87C */ dNpcAttention_c mAttention;

    /* 0x910 */ dNpcMdlCallbackMulti_c mCbMulti;
    /* 0x920 */ dNpcMdlCallbackYXZ_c field_0x920;
    /* 0x954 */ dNpcMdlCallbackYXZ_c field_0x954;
    /* 0x988 */ dNpcMdlCallbackYXZ_c field_0x988;
    /* 0x9BC */ dNpcMdlCallbackEye_c field_0x9BC;

    /* 0xA18 */ dNpcCallbackAngHelper_c field_0xA18;

    /* 0xA2C */ UNKTYPE *field_0xA2C;

    /* 0x9F0 */ dNpcUnknown2_c field_0x9F0;

    /* 0xA74 */ s32 mPathId0;
    /* 0xA78 */ s32 mPathId1;
    /* 0xA7C */ s32 field_0xA7C;
    /* 0xA80 */ s32 mResolvedPathId;

    /* 0xAAC */ u16 mSpawnStoryflag;
    /* 0xAAE */ u16 mDespawnStoryflag;

    /* 0xAA4 */ cCcD_Obj *mpCc;
    /* 0xAA8 */ dBgS_Acch *mpAcch;

    /* 0xAB0 */
};

#endif
