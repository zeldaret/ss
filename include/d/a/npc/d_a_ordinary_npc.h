#ifndef D_A_ORDINARY_NPC_H
#define D_A_ORDINARY_NPC_H

#include "d/a/npc/d_a_npc.h"
#include "d/col/bg/d_bg_s_acch.h"
#include "m/m3d/m_anmchr.h"
#include "m/m3d/m_anmtexpat.h"
#include "m/m3d/m_smdl.h"
#include "m/m_allocator.h"
#include "m/m_color.h"
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
    dAcOrdinaryNpc_c() : mEventRelated(*this, 0), mPath(this) {}
    ~dAcOrdinaryNpc_c() {}
    /* vt 0x1F4 */ virtual int acNpc_vt_0x1F4() override;

    STATE_VIRTUAL_OVERRIDE_FUNC_DECLARE(dAcOrdinaryNpc_c, dAcNpc_c, Wait);
    STATE_VIRTUAL_FUNC_DECLARE(dAcOrdinaryNpc_c, Pain);
    STATE_VIRTUAL_FUNC_DECLARE(dAcOrdinaryNpc_c, Surprised);
    STATE_VIRTUAL_FUNC_DECLARE(dAcOrdinaryNpc_c, Withstand);
    STATE_VIRTUAL_FUNC_DECLARE(dAcOrdinaryNpc_c, Walk);
    STATE_VIRTUAL_FUNC_DECLARE(dAcOrdinaryNpc_c, LookAwaySt);
    STATE_VIRTUAL_FUNC_DECLARE(dAcOrdinaryNpc_c, LookAway);
    STATE_VIRTUAL_FUNC_DECLARE(dAcOrdinaryNpc_c, Turn);
    STATE_FUNC_DECLARE(dAcOrdinaryNpc_c, PreWalkTurn);

protected:
    /* 0x6E4 */ u8 _0x6E4[8];
    /* 0x6EC */ d3d::AnmMdlWrapper mMdlWrapper;
    /* 0x75C */ m3d::anmChr_c mAnmChr;
    /* 0x    */ dcNpcMdlCallback0 mMdlCallback;

    /* 0x7D8 */ ActorEventRelated mEventRelated;

    dNpcPath_c mPath;

    /* 0x87C */ dNpcAttention_c mAttention;

    /* 0x8AC */ dNpcMdlCallbackMulti_c mCbMulti;
    /* 0x8BC */ dNpcMdlCallbackYXZ_c field_0x8BC;
    /* 0x8F0 */ dNpcMdlCallbackYXZ_c field_0x8F0;
    /* 0x924 */ dNpcMdlCallbackYXZ_c field_0x924;
    /* 0x958 */ dNpcMdlCallbackEye_c field_0x958;

    /* 0xAA4 */ dBgS_AcchCir *mpAcchCir;
    /* 0xAA8 */ dBgS_Acch *mpAcch;

    /* 0xAB0 */
};

#endif
