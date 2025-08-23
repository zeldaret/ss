#ifndef D_A_NPC_H
#define D_A_NPC_H

#include "common.h"
#include "d/a/d_a_base.h"
#include "d/a/d_a_itembase.h"
#include "d/a/obj/d_a_obj_base.h"
#include "d/d_message.h"
#include "libms/flowfile.h"
#include "m/m3d/m_mdl.h"
#include "m/m_angle.h"
#include "m/m_mtx.h"
#include "m/m_quat.h"
#include "m/m_vec.h"
#include "s/s_State.hpp"
#include "s/s_StateInterfaces.hpp"
#include "toBeSorted/d_d3d.h"

class dAcNpc_c;

// NPC-specific mdl callback stuff
class dNpcMdlCallbackBase_c {
protected:
    /* 0x00 */ u32 mNodeId;
    /* 0x04 */ mAng field_0x04;
    /* 0x06 */ mAng field_0x06;
    /* 0x08 */ bool mForceCalc;

    dNpcMdlCallbackBase_c() : mNodeId(-1), field_0x04(0), field_0x06(0), mForceCalc(false) {}

public:
    u32 getNodeId() const {
        return mNodeId;
    }

    void loadNodeId(d3d::AnmMdlWrapper *mdl, const char *nodeName);

    // vtable at 0xC
    /* 0x08 */ virtual void apply(mMtx_c *result) const = 0;
    /* 0x0C */ virtual void calc() = 0;
    /* 0x10 */ virtual bool isDone() = 0;
    /* 0x14 */ virtual void timingA(nw4r::g3d::ChrAnmResult *result);
    /* 0x18 */ virtual void timingB(mMtx_c *result);
};

/**
 * A mdl callback commonly used for controlling NPC head movement.
 * NPCs will look at Link or certain objects of interest (e.g. a Bomb held by Link).
 * This class smoothly interpolates the additive rotation of its node (head, neck, spine).
 */
class dNpcMdlCallbackAng_c : public dNpcMdlCallbackBase_c {
protected:
    /* 0x10 */ mAng3_c mTarget;
    /* 0x16 */ s16 mNumSteps;
    /* 0x18 */ mAng mMaxStepSizeYZ;
    /* 0x1A */ mAng mMinStepSizeYZ;
    /* 0x1C */ mAng mMaxStepSizeX;
    /* 0x1E */ mAng mMinStepSizeX;
    /* 0x20 */ s32 mScaleZ;
    /* 0x24 */ s32 mScaleX;
    /* 0x28 */ s32 mScaleY;
    /* 0x2C */ mAng3_c mCurrent;

    s32 getScaleX() const {
        return mScaleX;
    }

    s32 getScaleY() const {
        return mScaleY;
    }

    s32 getScaleZ() const {
        return mScaleZ;
    }

public:
    dNpcMdlCallbackAng_c();
    /* 0x1C */ virtual ~dNpcMdlCallbackAng_c() {}

    void setMinMaxStepSize(const mAng &max, const mAng &min);
    void setMinMaxStepSizeX(const mAng &max, const mAng &min);
    void reset();
    void resetTarget();
    void setTarget(const mAng3_c &target);
    void setTargetNow(const mAng3_c &target);
    void finishTarget();

    /* 0x0C */ virtual void calc() override;
    /* 0x10 */ virtual bool isDone() override {
        // TODO - this matches, but maybe an operator overload?
        return mCurrent.x == mTarget.x && mCurrent.y == mTarget.y && mCurrent.z == mTarget.z;
    }
    /* 0x20 */ virtual void vt_0x20(const dAcBase_c *) {}
};

/**
 * Used by dAcOrdinaryNpc - applies rotation in YXZ order.
 */
class dNpcMdlCallbackYXZ_c : public dNpcMdlCallbackAng_c {
public:
    dNpcMdlCallbackYXZ_c() {}

    /* 0x08 */ virtual void apply(mMtx_c *result) const override;
    /* 0x1C */ virtual ~dNpcMdlCallbackYXZ_c() {}
};

/**
 * Used by Salesman, Terry, Rescue Bird applies rotation in YXZ order,
 * but makes sure to restore translation after applying.
 */
class dNpcMdlCallbackYXZFixed_c : public dNpcMdlCallbackAng_c {
private:
    /* 0x32 */ mAng3_c mActorRotation;

public:
    dNpcMdlCallbackYXZFixed_c() {}

    /* 0x08 */ virtual void apply(mMtx_c *result) const override;
    // /* 0x1C */ virtual ~dNpcMdlCallbackYXZFixed_c() {}
    /* 0x20 */ virtual void vt_0x20(const dAcBase_c *) override;
};

/**
 * Unused - applies rotation in YZX order, with X and Z swapped.
 */
class dNpcMdlCallbackYZX_c : public dNpcMdlCallbackAng_c {
public:
    dNpcMdlCallbackYZX_c() {}

    /* 0x08 */ virtual void apply(mMtx_c *result) const override;
    // /* 0x1C */ virtual ~dNpcMdlCallbackYZX_c() {}
};

/**
 * Unused - applies rotation in XZY order, with X, Y, Z swizzled aroumd.
 */
class dNpcMdlCallbackXZY_c : public dNpcMdlCallbackAng_c {
public:
    dNpcMdlCallbackXZY_c() {}

    /* 0x08 */ virtual void apply(mMtx_c *result) const override;
    // /* 0x1C */ virtual ~dNpcMdlCallbackXZY_c() {}
};

/**
 * Quat - used for Kikwis.
 */
class dNpcMdlCallbackQuat_c : public dNpcMdlCallbackBase_c {
protected:
    /* 0x10 */ f32 mSlerpFactor;
    /* 0x14 */ f32 mRatio;
    /* 0x18 */ f32 mMaxStepSize;
    /* 0x1C */ f32 mMinStepSize;
    /* 0x20 */ mAng *mActorRotY;
    /* 0x24 */ mQuat_c mStart;
    /* 0x34 */ mQuat_c mTarget;

public:
    dNpcMdlCallbackQuat_c() {}
    /* 0x1C */ virtual ~dNpcMdlCallbackQuat_c() {}

    /* 0x08 */ virtual void apply(mMtx_c *result) const override;
    /* 0x0C */ virtual void calc() override;
    /* 0x10 */ virtual bool isDone() override {
        return std::fabsf(mSlerpFactor - 1.0f) < 0.001;
    }
};

struct dNpcMdlCallbackNode {
    /* 0x00 */ dNpcMdlCallbackBase_c *mpCallback;
    /* 0x04 */ dNpcMdlCallbackNode *mpNext;
};

/**
 * Since a mdl can only have one callback, this is a generic
 * mechanism with which multiple callbacks can be registered.
 */
class dNpcMdlCallbackMulti_c : public m3d::callback_c {
public:
    virtual ~dNpcMdlCallbackMulti_c() {
        clearList();
    }
    virtual void timingA(u32, nw4r::g3d::ChrAnmResult *, nw4r::g3d::ResMdl) override;
    virtual void timingB(u32, nw4r::g3d::WorldMtxManip *, nw4r::g3d::ResMdl) override;

    bool addCallback(dNpcMdlCallbackBase_c *callback);

private:
    dNpcMdlCallbackNode *newCallbackNode();
    bool createCallbackNode(dNpcMdlCallbackBase_c *cb);
    void clearList();

    /* 0x04 */ dNpcMdlCallbackNode *mpHead;
    /* 0x08 */ dNpcMdlCallbackNode *mpTail;
    /* 0x0C */ s32 mNumNodes;
};

// NPC-specific "flow" / MSBF code
class dFlowNpc_c : public dFlow_c {
public:
    dFlowNpc_c(dAcNpc_c *owner) : mpOwner(owner) {}
    virtual ~dFlowNpc_c() {}

    /* vt 0x0C */ virtual void triggerEntryPoint(s32 labelPart1, s32 labelPart2) override;
    /* vt 0x10 */ virtual void triggerEntryPoint(const char *) override;
    /* vt 0x1C */ virtual bool handleEventInternal(const MsbFlowInfo *element) override;
    /* vt 0x34 */ virtual void vt_0x34() override {}
    /* vt 0x38 */ virtual bool vt_0x38() const override {
        return true;
    }
    /* vt 0x3C */ virtual u16 getSwitchChoice(const MsbFlowInfo *element, u16 param) const override;
    /* vt 0x40 */ virtual bool triggerEntryPointChecked(s32 labelPart1, s32 labelPart2) override;

private:
    void extract2xU16Params(const MsbFlowInfo *element, u16 *p1, u16 *p2);
    void extract4xU8Params(const MsbFlowInfo *element, u8 *p1, u8 *p2, u8 *p3, u8 *p4);

    void setupActorRefs();

    /* 0x064 */ dAcNpc_c *mpOwner;
    // exact type isn't known but this Makes Sense
    /* 0x068 */ dAcRef_c<dAcObjBase_c> mObjRefs[16];
    /* 0x128 */ UNKWORD field_0x128[4];
    /* 0x138 */ UNKWORD field_0x138;
    /* 0x13C */ UNKWORD field_0x13C;
};

// This is the NPC base. Most npcs actually use dAcOrdinaryNpc, but this just is a simpler one?

// Ghidra: ActorNpcBase
//   size: 0x6e4
// official name
class dAcNpc_c : public dAcObjBase_c {
public:
    dAcNpc_c();
    virtual ~dAcNpc_c();

    /* vt 0x080 */ virtual bool giveItem(u8 param, ITEM_ID item_id);
    /* vt 0x084 */ virtual void getPosCopy3(mVec3_c &outResult) {
        outResult.copyFrom(poscopy3);
    }
    /* vt 0x088 */ virtual void acNpc_vt_0x88() {}
    /* vt 0x08C */ virtual void acNpc_vt_0x8C() {}
    /* vt 0x090 */ virtual int acNpc_vt_0x90() {
        return 1;
    }
    /* vt 0x094 */ virtual int acNpc_vt_0x94() {
        return 1;
    }
    /* vt 0x098 */ virtual int acNpc_vt_0x98() {
        return 1;
    }
    /* vt 0x09C */ virtual void acNpc_vt_0x9C();
    /* vt 0x0A0 */ virtual void acNpc_vt_0xA0();
    /* vt 0x0A4 */ virtual void acNpc_vt_0xA4();
    /* vt 0x0A8 */ virtual void acNpc_vt_0xA8();
    /* vt 0x0AC */ virtual void acNpc_vt_0xAC();
    /* vt 0x0B0 */ virtual void acNpc_vt_0xB0();
    /* vt 0x0B4 */ virtual void acNpc_vt_0xB4();
    /* vt 0x0B8 */ virtual void acNpc_vt_0xB8();
    /* vt 0x0BC */ virtual void acNpc_vt_0xBC();
    /* vt 0x0C0 */ virtual u16 eventFlowSwitch1(u32 arg) {
        return 0;
    }
    /* vt 0x0C4 */ virtual u16 eventFlowSwitch2(u32 arg) {
        return 0;
    }
    /* vt 0x0C8 */ virtual u16 eventFlowSwitch3(u32 arg) {
        return 0;
    }
    /* vt 0x0CC */ virtual void acNpc_vt_0xCC();
    /* vt 0x0D0 */ virtual int acNpc_vt_0xD0();
    /* vt 0x0D4 */ virtual void acNpc_vt_0xD4();
    /* vt 0x0D8 */ virtual int acNpc_vt_0xD8();
    /* vt 0x0DC */ virtual int acNpc_vt_0xDC() {
        return 0;
    }
    /* vt 0x0E0 */ virtual int acNpc_vt_0xE0() {
        return 0;
    }
    /* vt 0x0E4 */ virtual int acNpc_vt_0xE4() {
        return 1;
    }
    /* vt 0x0E8 */ virtual void acNpc_vt_0xE8();

    /* vt 0x074 */ virtual void *getObjectListEntry() override {
        return &mActorListEntry;
    }

    /* vt 0x0EC */ virtual int acNpc_vt_0xEC() {
        return 0;
    }

    /* vt 0x0F0 */ virtual bool isInState(const sStateIDIf_c &otherState) const {
        return *mStateMgr.getStateID() == otherState;
    }

    /* vt 0x0F4 */ virtual void setState(const sStateIDIf_c &otherState) {
        // Result is discarded
        (void)isInState(StateID_Demo);
        mStateMgr.changeState(otherState);
    }

    /* vt 0xF8 */ virtual void executeState() {
        mStateMgr.executeState();
    }

    STATE_VIRTUAL_FUNC_DECLARE(dAcNpc_c, Wait);
    STATE_VIRTUAL_FUNC_DECLARE(dAcNpc_c, Demo);

    /* vt 0x114 */ virtual int acNpc_vt_0x114() {
        return 0;
    }

    /* vt 0x118 */ virtual int acNpc_vt_0x118() {
        return 0;
    }
    /* vt 0x11C */ virtual void acNpc_vt_0x11C();
    /* vt 0x120 */ virtual void acNpc_vt_0x120() {}
    /* vt 0x124 */ virtual void acNpc_vt_0x124() {}
    /* vt 0x128 */ virtual int acNpc_vt_0x128() {
        return 0;
    }
    /* vt 0x12C */ virtual int acNpc_vt_0x12C() {
        return 0;
    }
    /* vt 0x130 */ virtual void acNpc_vt_0x130();
    /* vt 0x134 */ virtual void acNpc_vt_0x134();
    /* vt 0x138 */ virtual void acNpc_vt_0x138();
    /* vt 0x13C */ virtual void acNpc_vt_0x13C();
    /* vt 0x140 */ virtual void acNpc_vt_0x140();
    /* vt 0x144 */ virtual void acNpc_vt_0x144();
    /* vt 0x148 */ virtual void acNpc_vt_0x148();
    /* vt 0x14C */ virtual int acNpc_vt_0x14C() {
        return 0;
    }
    /* vt 0x150 */ virtual int acNpc_vt_0x150() {
        return 1;
    }
    /* vt 0x154 */ virtual void acNpc_vt_0x154() {}
    /* vt 0x158 */ virtual void acNpc_vt_0x158() {}
    /* vt 0x15C */ virtual void acNpc_vt_0x15C() {}
    /* vt 0x160 */ virtual void acNpc_vt_0x160() {}
    /* vt 0x164 */ virtual void acNpc_vt_0x164();
    /* vt 0x168 */ virtual int acNpc_vt_0x168() {
        return 0;
    }
    /* vt 0x16C */ virtual void acNpc_vt_0x16C();
    /* vt 0x170 */ virtual void acNpc_vt_0x170();
    /* vt 0x174 */ virtual void acNpc_vt_0x174();
    /* vt 0x178 */ virtual void acNpc_vt_0x178();
    /* vt 0x17C */ virtual void acNpc_vt_0x17C();
    /* vt 0x180 */ virtual void acNpc_vt_0x180();
    /* vt 0x184 */ virtual void acNpc_vt_0x184();
    /* vt 0x188 */ virtual void acNpc_vt_0x188();
    /* vt 0x18C */ virtual void acNpc_vt_0x18C();
    /* vt 0x190 */ virtual void acNpc_vt_0x190();
    /* vt 0x194 */ virtual void acNpc_vt_0x194();
    /* vt 0x198 */ virtual void acNpc_vt_0x198();
    /* vt 0x19C */ virtual void acNpc_vt_0x19C();
    /* vt 0x1A0 */ virtual void acNpc_vt_0x1A0();
    /* vt 0x1A4 */ virtual void acNpc_vt_0x1A4();
    /* vt 0x1A8 */ virtual void acNpc_vt_0x1A8();
    /* vt 0x1AC */ virtual void acNpc_vt_0x1AC();
    /* vt 0x1B0 */ virtual void acNpc_vt_0x1B0();
    /* vt 0x1B4 */ virtual void acNpc_vt_0x1B4();
    /* vt 0x1B8 */ virtual void acNpc_vt_0x1B8();
    /* vt 0x1BC */ virtual void acNpc_vt_0x1BC();
    /* vt 0x1C0 */ virtual void acNpc_vt_0x1C0();
    /* vt 0x1C4 */ virtual void acNpc_vt_0x1C4();
    /* vt 0x1C8 */ virtual void acNpc_vt_0x1C8();
    /* vt 0x1CC */ virtual void acNpc_vt_0x1CC();
    /* vt 0x1D0 */ virtual void acNpc_vt_0x1D0();
    /* vt 0x1D4 */ virtual void acNpc_vt_0x1D4();
    /* vt 0x1D8 */ virtual void acNpc_vt_0x1D8();
    /* vt 0x1DC */ virtual void acNpc_vt_0x1DC();
    /* vt 0x1E0 */ virtual void acNpc_vt_0x1E0();
    /* vt 0x1E4 */ virtual void acNpc_vt_0x1E4();
    /* vt 0x1E8 */ virtual int acNpc_vt_0x1E8() {
        return 1;
    }
    /* vt 0x1EC */ virtual int acNpc_vt_0x1EC() {
        return 1;
    }
    /* vt 0x1F0 */ virtual void *acNpc_vt_0x1F0() {
        return &mFlow;
    }
    /* vt 0x1F4 */ virtual int acNpc_vt_0x1F4() {
        return 1;
    }
    /* vt 0x1F8 */ virtual bool acNpc_vt_0x1F8() {
        return !acNpc_vt_0x1F4();
    }
    /* vt 0x1FC */ virtual bool acNpc_vt_0x1FC() {
        return field_0x684 == 1;
    }
    /* vt 0x200 */ virtual bool acNpc_vt_0x200() {
        return !acNpc_vt_0x1FC();
    }
    /* vt 0x204 */ virtual int acNpc_vt_0x204() {
        return 0;
    }

    bool doGiveItem(u8 id, s32 item);
    u16 doFlowSwitch1(u32 arg);
    u16 doFlowSwitch2(u32 arg);
    u16 doFlowSwitch3(u32 arg);

protected:
    void npcExecute();
    static mVec3_c getLinkPos();
    static bool addHeadNeckSpineCallbacks(
        d3d::AnmMdlWrapper *mdl, dNpcMdlCallbackMulti_c *multi, dNpcMdlCallbackBase_c *head,
        dNpcMdlCallbackBase_c *neck, dNpcMdlCallbackBase_c *spine
    );

    static const char *sHeadNodeName;
    static const char *sNeckNodeName;
    static const char *sSpine2NodeName;

private:
    /* 0x330 */ u8 field_0x330[0x358 - 0x330];
    /* 0x358 */ fLiNdBa_c mActorListEntry;
    /* 0x364 */ dFlowNpc_c mFlow;
    /* 0x4A4 */ u8 field_0x4A4[0x4C0 - 0x4A4];
    /* 0x4C0 */ STATE_MGR_DECLARE(dAcNpc_c);
    /* 0x4FC */ u8 field_0x4FC[0x684 - 0x4FC];
    /* 0x684 */ u8 field_0x684;
    /* 0x685 */ u8 field_0x685[0x6E4 - 0x685];

    static fLiMgBa_c NPC_ACTOR_LIST;
};

#endif
