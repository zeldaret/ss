#ifndef D_A_OBJ_TOWER_GEARD101_H
#define D_A_OBJ_TOWER_GEARD101_H

#include "common.h"
#include "d/a/obj/d_a_obj_base.h"
#include "m/m3d/m_mdl.h"
#include "m/m_angle.h"
#include "m/m_mtx.h"
#include "m/m_vec.h"
#include "nw4r/g3d/res/g3d_resfile.h"
#include "s/s_State.hpp"
#include "toBeSorted/d_emitter.h"

/**
 * A callback implementation that controls the button node in the switch model.
 */
class dAcOTowerGearD101_callback_c : public m3d::callback_c {
public:
    dAcOTowerGearD101_callback_c() {}
    virtual ~dAcOTowerGearD101_callback_c() {}
    virtual void timingB(u32, nw4r::g3d::WorldMtxManip *, nw4r::g3d::ResMdl) override;

    /* 0x04 */ u32 mNodeId;
    /* 0x08 */ mAng mGearRotation;

    void init(u32 nodeId) {
        mNodeId = nodeId;
    }
};

class dAcOTowerGearD101_c : public dAcObjBase_c {
public:
    dAcOTowerGearD101_c()
        : mStateMgr(*this, sStateID::null),
          field_0x3A0(mVec3_c::Zero.x, mVec3_c::Zero.y, mVec3_c::Zero.z),
          field_0x3AC(mVec3_c::Zero.x, mVec3_c::Zero.y, mVec3_c::Zero.z),
          field_0x3B8(mAng3_c::Zero) {}
    virtual ~dAcOTowerGearD101_c() {}

    STATE_FUNC_DECLARE(dAcOTowerGearD101_c, Wait);

    virtual bool createHeap() override;
    virtual int actorCreate() override;
    virtual int actorPostCreate() override;
    virtual int doDelete() override;
    virtual int actorExecute() override;
    virtual int draw() override;

private:
    void initTransform();
    void playVisualEffect();

    /* 0x330 */ nw4r::g3d::ResFile mRes;
    /* 0x334 */ m3d::mdl_c mMdl;
    /* 0x350 */ STATE_MGR_DECLARE(dAcOTowerGearD101_c);
    /* 0x394 */ dAcOTowerGearD101_callback_c mMdlCallback;
    /* 0x3A0 */ mVec3_c field_0x3A0;
    /* 0x3AC */ mVec3_c field_0x3AC;
    /* 0x3B8 */ mAng3_c field_0x3B8;
    /* 0x3BE */ u8 field_0x3BE[2];
    /* 0x3C0 */ mAng mCurrRotation;
    /* 0x3C2 */ s16 mPreviousTurnSpeed;
    /* 0x3C4 */ mMtx_c field_0x3C4;
    /* 0x3F4 */ mVec3_c field_0x3F4;
    /* 0x400 */ dEmitter_c mEffects;
};

#endif
