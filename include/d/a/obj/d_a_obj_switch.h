#ifndef D_A_OBJ_SWITCH_H
#define D_A_OBJ_SWITCH_H

#include <d/a/obj/d_a_obj_base.h>
#include <m/m3d/m_mdl.h>
#include <s/s_State.hpp>
#include <s/s_StateMgr.hpp>
#include <toBeSorted/actor_collision.h>
#include <toBeSorted/time_area_mgr.h>


// OBJ_VSD?
class dAcOScatterSand_tmp : public dAcObjBase_c {
public:
    /* 0x330 */ u8 field_0x330[0x6DC - 0x330];
    /* 0x6DC */ u16 field_0x6DC;
    /* 0x6DE */ u8 field_0x6DE[0x6E4 - 0x6DE];
    /* 0x6E4 */ bool field_0x6E4;

    inline f32 getSomeScale() {
        return scale.y * 41.65f / 100.0f;
    }
};

/**
 * A callback implementation that controls the button node in the switch model.
 */
class dAcOswMdlCallback_c : public m3d::callback_c {
public:
    dAcOswMdlCallback_c() {}
    virtual ~dAcOswMdlCallback_c() {}
    virtual void timingB(u32, nw4r::g3d::WorldMtxManip *, nw4r::g3d::ResMdl) override;

    /* 0x04 */ f32 mElevation;
    /* 0x08 */ u32 mNodeId;

    void init(u32 nodeId) {
        mNodeId = nodeId;
        mElevation = 0.0f;
    }
};

class dAcOsw_c : public dAcObjBase_c {
public:
    dAcOsw_c() : mStateMgr(*this, sStateID::null), field_0x5A0(0.0f) {}
    virtual ~dAcOsw_c() {}

    virtual int actorCreate() override;
    virtual int actorPostCreate() override;

    virtual bool createHeap() override;
    virtual int doDelete() override;
    virtual int actorExecute() override;
    virtual int actorExecuteInEvent() override;
    virtual int draw() override;

    bool someInteractCheck(bool);

    STATE_FUNC_DECLARE(dAcOsw_c, OnWait);
    STATE_FUNC_DECLARE(dAcOsw_c, On);
    STATE_FUNC_DECLARE(dAcOsw_c, OffWait);
    STATE_FUNC_DECLARE(dAcOsw_c, Off);
    STATE_FUNC_DECLARE(dAcOsw_c, None);

    static void interactCallback(void *unknown, dAcBase_c *actor, dAcObjBase_c *interactor);

private:
    /* 0x330 */ m3d::mdl_c mModel;
    /* 0x354 */ ActorCollision mCollision;
    /* 0x564 */ STATE_MGR_DECLARE(dAcOsw_c);
    /* 0x5A0 */ TimeAreaStruct field_0x5A0;
    /* 0x5AC */ dAcRef_c<dAcOScatterSand_tmp> mObjRef;
    /* 0x5B8 */ mMtx_c field_0x5B8;
    /* 0x5E8 */ f32 field_0x5E8;
    /* 0x5EC */ u16 field_0x5EC;
    /* 0x5EE */ u8 mOffSceneFlag;
    /* 0x5EF */ u8 mOnSceneFlag;
    /* 0x5F0 */ u8 mSwitchType;
    /* 0x5F1 */ u8 field_0x5F1;
    /* 0x5F2 */ u8 field_0x5F2;
    /* 0x5F3 */ u8 field_0x5F3;
    /* 0x5F4 */ u8 mActivatedByPlayer;
    /* 0x5F5 */ bool mCanBeSeen;
    /* 0x5F6 */ bool mShown;
    /* 0x5F7 */ bool mHidden;
    /* 0x5F8 */ dAcOswMdlCallback_c mButtonCtrl;
};

#endif
