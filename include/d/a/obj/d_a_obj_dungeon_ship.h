#ifndef D_A_OBJ_DUNGEON_SHIP_H
#define D_A_OBJ_DUNGEON_SHIP_H

#include "common.h"
#include "d/a/obj/d_a_obj_base.h"
#include "d/col/bg/d_bg_w.h"
#include "d/col/cc/d_cc_d.h"
#include "m/m3d/m_anmchr.h"
#include "m/m3d/m_anmmatclr.h"
#include "m/m_vec.h"
#include "nw4r/g3d/res/g3d_resfile.h"
#include "s/s_State.hpp"
#include "toBeSorted/actor_event.h"
#include "toBeSorted/actor_on_rail.h"
#include "toBeSorted/dowsing_target.h"
#include "toBeSorted/effects_struct.h"
#include "toBeSorted/time_proc.h"

class dAcODungeonShip_c : public dAcObjBase_c {
public:
    dAcODungeonShip_c()
        : mStateMgr(*this, sStateID::null), mDowsingTarget(this, DowsingTarget::SLOT_NONE), mEvent(*this, nullptr) {}
    virtual ~dAcODungeonShip_c() {}

    virtual bool createHeap() override;
    virtual int create() override;
    virtual int draw() override;
    virtual int doDelete() override;
    virtual int actorExecute() override;

    STATE_FUNC_DECLARE(dAcODungeonShip_c, Wait);
    STATE_FUNC_DECLARE(dAcODungeonShip_c, Transparency);
    STATE_FUNC_DECLARE(dAcODungeonShip_c, AppearEvent);
    STATE_FUNC_DECLARE(dAcODungeonShip_c, Appear);
    STATE_FUNC_DECLARE(dAcODungeonShip_c, End);

private:
    static const f32 someFloats[18];

    void updateMatrixAndScale();

    void fn_485_1660();
    void fn_485_1720();
    u32 fn_485_1900();
    u32 fn_485_1960();
    void fn_485_1A90();
    void fn_485_1BF0();
    void fn_485_1DF0();
    void fn_485_1EE0();

    static void eventIn_Wrapper(void *arg);
    void eventIn();
    static void eventEnd_Wrapper(void *arg);
    void eventEnd();

    static dCcD_SrcCps sCcSrc;

    /* 0x300 */ nw4r::g3d::ResFile mRes;
    /* 0x334 */ MdlWithTimeProc mMdl;

    /* 0x374 */ m3d::anmChr_c mAnmChr;
    /* 0x3AC */ m3d::anmMatClr_c mAnmMatClr;
    /* 0x3D8 */ dBgW mBg;
    /* 0x5E8 */ dCcD_Cps mCc;
    /* 0x758 */ STATE_MGR_DECLARE(dAcODungeonShip_c);
    /* 0x794 */ DowsingTarget mDowsingTarget;
    /* 0x7B4 */ mVec3_c mDowsingOffset;
    /* 0x7C0 */ u8 mIsRoaming;
    /* 0x7C4 */ ActorOnRail_Ext mPath;
    /* 0x7F8 */ ActorEventRelated mEvent;
    /* 0x848 */ u8 mAppearEventFromParam;
    /* 0x849 */ u8 field_0x849;
    /* 0x84A */ u8 mAppearEvent;
    /* 0x84B */ u8 mPathIdx;
    /* 0x84C */ u8 field_0x84C[0x850 - 0x84C];
    /* 0x850 */ u32 field_0x850;
    /* 0x854 */ u8 mNumTimesHit;
    /* 0x855 */ u8 field_0x855;
    /* 0x856 */ s8 field_0x856;
    /* 0x858 */ f32 field_0x858;
    /* 0x862 */ u8 field_0x862;
    /* 0x863 */ u8 field_0x863;
    /* 0x864 */ u8 field_0x864;
    /* 0x865 */ u8 mIsDocked;
    /* 0x868 */ s32 field_0x868;
    /* 0x86C */ u32 field_0x86C;
    /* 0x870 */ EffectsStruct mEffects[2];
    /* 0x8D8 */ u8 field_0x8D8;
    /* 0x8D9 */ u8 field_0x8D9;
    /* 0x8DA */ u8 field_0x8DA;
};

#endif
