#ifndef D_A_OBJ_STONE_STAND_H
#define D_A_OBJ_STONE_STAND_H

#include "d/a/obj/d_a_obj_sw_sword_beam.h"
#include "d/col/bg/d_bg_w.h"
#include "m/m3d/m_anmmatclr.h"
#include "m/m3d/m_smdl.h"
#include "m/m_mtx.h"
#include "m/m_vec.h"
#include "nw4r/g3d/res/g3d_resfile.h"
#include "s/s_State.hpp"
#include "toBeSorted/actor_event.h"

class dAcOStoneStand_c : public dAcGoddessCrestHolder_c {
public:
    dAcOStoneStand_c() : mEvent(*this, nullptr), mStateMgr(*this) {}
    virtual ~dAcOStoneStand_c() {}

    virtual bool createHeap() override;
    virtual int create() override;
    virtual int doDelete() override;
    virtual int actorExecute() override;
    virtual int actorExecuteInEvent() override;
    virtual int draw() override;
    virtual void doInteraction(s32) override;
    virtual void vt_0x88(f32 &param) override;

    STATE_FUNC_DECLARE(dAcOStoneStand_c, Wait);
    STATE_FUNC_DECLARE(dAcOStoneStand_c, Shake);
    STATE_FUNC_DECLARE(dAcOStoneStand_c, OnSwitch);

private:
    void fn_513_14D0();
    bool canInsertTablet();

    /* 0x368 */ ActorEventRelated mEvent;
    /* 0x3B8 */ nw4r::g3d::ResFile mResFile;
    /* 0x3BC */ m3d::smdl_c mMdl;
    /* 0x3D8 */ m3d::smdl_c mTabletMdls[3];
    /* 0x42C */ m3d::anmMatClr_c mAnm[3];
    /* 0x4B0 */ dBgW mBgW;
    /* 0x6C0 */ STATE_MGR_DECLARE(dAcOStoneStand_c);
    /* 0x6FC */ mMtx_c mTabletMatrix[3];
    /* 0x78C */ mVec3_c field_0x78C;
    /* 0x798 */ mVec3_c mLinkPos;
    /* 0x7A4 */ f32 field_0x7A4;
    /* 0x7A8 */ f32 field_0x7A8;
    /* 0x7AC */ u32 mLocatorABone;
    /* 0x7B0 */ u32 mLocatorBBone;
    /* 0x7B4 */ u32 mLocatorACone;
    /* 0x7B8 */ mAng3_c mLinkRot;
    /* 0x7BE */ mAng field_0x7BE;
    /* 0x7C0 */ u32 mVisibleTabletState;
    /* 0x7C4 */ u8 mActivatedSceneflag;
    /* 0x7C5 */ u8 mPillarCSExitOffset;
    /* 0x7C6 */ u8 mTabletCSIndex;
    /* 0x7C7 */ u8 mOpenedLightPillarState;
    /* 0x7C8 */ u8 field_0x7C8;
    /* 0x7C9 */ u8 field_0x7C9;
    /* 0x7CA */ bool mInsertedTablet;

    static char *sResMdlAnmNames[3];
};

#endif
