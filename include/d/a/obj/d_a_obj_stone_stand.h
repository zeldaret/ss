#ifndef D_A_OBJ_STONE_STAND_H
#define D_A_OBJ_STONE_STAND_H

#include "d/a/obj/d_a_obj_base.h"
#include "d/a/obj/d_a_obj_sw_sword_beam.h"
#include "d/col/bg/d_bg_w.h"
#include "m/m3d/m_anmmatclr.h"
#include "m/m3d/m_anmtexpat.h"
#include "m/m3d/m_smdl.h"
#include "m/m_mtx.h"
#include "m/m_vec.h"
#include "nw4r/g3d/res/g3d_resfile.h"
#include "s/s_State.hpp"
#include "s/s_StateMgr.hpp"
#include "toBeSorted/actor_event.h"

class dAcOStoneStand_c : public dAcGoddessCrestHolder_c {
public:
    dAcOStoneStand_c() : mEvent(*this, nullptr), mStateMgr(*this) {}
    virtual ~dAcOStoneStand_c() {}

    virtual bool createHeap() override;
    virtual int create() override;

    virtual int actorExecute() override;
    virtual int actorExecuteInEvent() override;

    STATE_FUNC_DECLARE(dAcOStoneStand_c, Wait);
    STATE_FUNC_DECLARE(dAcOStoneStand_c, Shake);
    STATE_FUNC_DECLARE(dAcOStoneStand_c, OnSwitch);

private:
    void fn_513_14D0();

    ActorEventRelated mEvent;
    nw4r::g3d::ResFile mResFile;
    m3d::smdl_c mMdl;
    m3d::smdl_c mMdls[3];
    m3d::anmMatClr_c mAnm[3];
    dBgW mBgW;
    /* 0x??? */ STATE_MGR_DECLARE(dAcOStoneStand_c);
    mMtx_c mTabletMatrix[3];
    mVec3_c field_0x78C;
    mVec3_c mLinkPos;
    f32 field_0x7A4;

    u32 mLocatorABone;
    u32 mLocatorBBone;
    u32 mLocatorACone;
    mAng3_c mLinkRot;

    u32 mVisibleTabletState;
    u8 mActivatedSceneflag;
    u8 mPillarCSExitOffset;
    u8 mTabletCSIndex;
    u8 mOpenedLightPillarState;
    u8 field_0x7C8;

    bool mInsertedTablet;

    static char *sResMdlAnmNames[3];
};

#endif
