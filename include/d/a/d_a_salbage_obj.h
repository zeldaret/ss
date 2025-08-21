#ifndef D_A_SALBAGE_OBJ_H
#define D_A_SALBAGE_OBJ_H

#include "common.h"
#include "d/a/obj/d_a_obj_base.h"
#include "d/col/bg/d_bg_s_acch.h"
#include "d/col/cc/d_cc_d.h"
#include "d/d_shadow.h"
#include "m/m3d/m_smdl.h"
#include "m/m_mtx.h"
#include "m/m_quat.h"
#include "m/m_vec.h"
#include "s/s_State.hpp"
#include "toBeSorted/dowsing_target.h"

class dAcSalbageObj_c;

enum SalvageObj_e {
    SALVAGE_OBJ_PROPERA = 0,
    SALVAGE_OBJ_ROULETTE = 1,
    SALVAGE_OBJ_DIVINER_CRYSTAL = 2,
    SALVAGE_OBJ_WATER_JAR = 5,
    SALVAGE_OBJ_POT = 6,
};

class SalbageRelated {
public:
    SalbageRelated(dAcSalbageObj_c *obj, s32 id) : mpObj(obj), mCarried(false), mHidden(false), mSalvageObjId(id) {}

    /* vt 0x08 */ virtual ~SalbageRelated() {}
    /* vt 0x0C */ virtual void setCarried() {
        mCarried = true;
    }
    /* vt 0x10 */ virtual void setNotCarried() {
        mCarried = false;
    }
    /* vt 0x14 */ virtual bool isCarried() const {
        return mCarried;
    }
    /* vt 0x18 */ virtual void setHidden() {
        mHidden = true;
    }
    /* vt 0x1C */ virtual void setNotHidden() {
        mHidden = false;
    }
    /* vt 0x20 */ virtual bool isHidden() const {
        return mHidden;
    }
    /* vt 0x24 */ virtual s32 getSalvageObjId() const {
        return mSalvageObjId;
    }
    /* vt 0x28 */ virtual void doDemoThrow();

private:
    /* 0x04 */ dAcSalbageObj_c *mpObj;
    /* 0x08 */ bool mCarried;
    /* 0x09 */ bool mHidden;
    /* 0x0C */ s32 mSalvageObjId;
};

/**
 * Actor - Object - Salvage
 *
 * Base class for objects that can be carried by
 * the Salvage Robot NPC, AKA Scrapper.
 */
class dAcSalbageObj_c : public dAcObjBase_c {
    friend class SalbageRelated;

    enum Behavior_e {
        /// The object is stationary and disappears when picked up
        BEHAVIOR_STATIONARY,
        /// The object is being carried and disappears when dropped
        BEHAVIOR_CARRY,
        /// The object is being carried in the Sky by the salvage robot
        BEHAVIOR_FLY,
    };

public:
    dAcSalbageObj_c(m3d::smdl_c *mdl, s32 salvageObjId)
        : field_0x330(0),
          mpMdl(mdl),
          mStateMgr(*this),
          mSalbageRelated(this, salvageObjId),
          mDowsingTarget(this, DowsingTarget::SLOT_NONE),
          mIsDemoState(false),
          mpNextStateId(nullptr) {}
    virtual ~dAcSalbageObj_c() {}

    virtual int preExecute() override;
    virtual int draw() override;
    virtual int preDraw() override;

    virtual int actorExecute() override;
    virtual void doInteraction(s32) override;

    /* vt 0x80 */ virtual bool vt_0x80() const {
        return false;
    }

    /* vt 0x84-0x8C */ STATE_VIRTUAL_FUNC_DECLARE(dAcSalbageObj_c, Wait);
    /* vt 0x90-0x98 */ STATE_VIRTUAL_FUNC_DECLARE(dAcSalbageObj_c, Carried);
    /* vt 0x9C-0xA4 */ STATE_VIRTUAL_FUNC_DECLARE(dAcSalbageObj_c, Demo);
    /* vt 0xA8-0xB0 */ STATE_VIRTUAL_FUNC_DECLARE(dAcSalbageObj_c, DemoThrow);
    /* vt 0xB4-0xBC */ STATE_VIRTUAL_FUNC_DECLARE(dAcSalbageObj_c, Fly);
    /* vt 0xC0-0xC8 */ STATE_VIRTUAL_FUNC_DECLARE(dAcSalbageObj_c, After);
    STATE_FUNC_DECLARE(dAcSalbageObj_c, Kill);

protected:
    bool shouldBeActiveDowsingTarget() const;
    void initSalbageObj();
    void loadBehaviorFromParams();
    void calcMtxFromSalbageNpc(mMtx_c &ret);
    void addAttentionTargetIfNeeded();
    void addAttentionTarget();

    void initCcAndBg();

    void executeInternal();
    void updateCc();
    void updateBgAcchCir();
    void updateMdl();

    /* vt 0xCC */ virtual mMtx_c calcWorldMtx();
    /* vt 0xD0 */ virtual void vt_0xD0() {}

    /* vt 0xD4 */ virtual bool isInStateWait() const {
        return mStateMgr.isState(StateID_Wait);
    }
    /* vt 0xD8 */ virtual bool isInStateWaitOrDemoThrow() const {
        return mStateMgr.isState(StateID_Wait) || mStateMgr.isState(StateID_DemoThrow);
    }
    /* vt 0xDC */ virtual DowsingTarget::DowsingSlot getDowsingSlot() const {
        return DowsingTarget::SLOT_QUEST;
    }
    /* vt 0xE0 */ virtual f32 getCcRadius() const {
        return 0.0f;
    }
    /* vt 0xE4 */ virtual f32 getCcHeight() const {
        return 0.0f;
    }
    /* vt 0xE8 */ virtual f32 getBgAcchCirRadius() const {
        return 0.0f;
    }
    /* vt 0xEC */ virtual f32 getBgAcchCirHeight() const {
        return 0.0f;
    }
    /* vt 0xF0 */ virtual f32 getPoscopy2YOffset() const {
        return 0.0f;
    }
    /* vt 0xF4 */ virtual f32 getPoscopy3YOffset() const {
        return 0.0f;
    }
    /* vt 0xF8 */ virtual f32 getDowsingUnusedF32() const {
        return 1500.0f;
    }
    /* vt 0xFC */ virtual f32 getDowsingTargetYOffset() const {
        return 50.0f;
    }

    /* 0x330 */ UNKWORD field_0x330;

    /* 0x334 */ m3d::smdl_c *mpMdl;
    /* 0x338 */ dShadowCircle_c mShadow;
    /* 0x340 */ mQuat_c mShadowRot;
    /* 0x350 */ dBgS_AcchCir mBgAcchCir;
    /* 0x3AC */ dBgS_ObjAcch mBgObjAcch;
    /* 0x75C */ dCcD_Cyl mCcCyl;
    /* 0x8AC */ STATE_MGR_DECLARE(dAcSalbageObj_c);
    /* 0x8E8 */ s32 mBehavior;
    /* 0x8EC */ bool mIsDemoState;
    /* 0x8F0 */ SalbageRelated mSalbageRelated;
    /* 0x900 */ f32 field_0x900;
    /* 0x904 */ s16 field_0x904;
    /* 0x908 */ mMtx_c mWorldSRMtx;
    /* 0x938 */ f32 field_0x938;
    /* 0x93C */ DowsingTarget mDowsingTarget;
    /* 0x95C */ mVec3_c field_0x95C;
    /* 0x968 */ bool mIsDowsingRegistered;
    /* 0x96C */ const sStateID_c *mpNextStateId;
};

#endif
