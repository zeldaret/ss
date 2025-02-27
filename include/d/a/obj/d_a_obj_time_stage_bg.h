#ifndef D_A_OBJ_TIME_STAGE_BG_H
#define D_A_OBJ_TIME_STAGE_BG_H

#include "d/a/obj/d_a_obj_base.h"
#include "m/m3d/m_anmmatclr.h"
#include "m/m3d/m_smdl.h"
#include "m/m_vec.h"
#include "nw4r/g3d/res/g3d_resfile.h"
#include "s/s_State.hpp"
#include "s/s_StateMgr.hpp"
#include "toBeSorted/stage_render_stuff.h"
#include "toBeSorted/time_area_mgr.h"

class dAcOTimeStageBg_c : public dAcObjBase_c {
public:
    dAcOTimeStageBg_c() : mStateMgr(*this, sStateID::null), mSceneCallback(this) {}
    virtual ~dAcOTimeStageBg_c() {}

    virtual bool createHeap() override;
    virtual int actorCreate() override;
    virtual int doDelete() override;
    virtual int actorPostCreate() override;
    virtual int actorExecute() override;
    virtual int draw() override;

    STATE_FUNC_DECLARE(dAcOTimeStageBg_c, Wait);

private:
    bool secondMdl() const {
        return mSubType == 2 || mSubType == 3;
    }

    /* 0x330 */ nw4r::g3d::ResFile mRes;
    /* 0x334 */ m3d::smdl_c mMdl1;
    /* 0x350 */ m3d::smdl_c mMdl2;
    /* 0x36C */ STATE_MGR_DECLARE(dAcOTimeStageBg_c);
    /* 0x3A8 */ TimeAreaStruct mTimeArea;
    /* 0x3B4 */ dScnCallback_c mSceneCallback;
    /* 0x3C0 */ m3d::anmMatClr_c mAnm;
    /* 0x3EC */ mVec3_c field_0x3EC;
    /* 0x3F8 */ f32 field_0x3F8;
    /* 0x3FC */ u8 mSubType;
};

#endif
