#ifndef D_A_OBJ_DOOR_H
#define D_A_OBJ_DOOR_H

#include "common.h"
#include "d/a/d_a_base.h"
#include "d/a/obj/d_a_obj_base.h"
#include "d/a/obj/d_a_obj_door_base.h"
#include "m/m3d/m_anmmdl.h"
#include "m/m3d/m_smdl.h"
#include "m/m_mtx.h"
#include "m/m_vec.h"
#include "toBeSorted/actor_event.h"
#include "toBeSorted/d_emitter.h"
#include "toBeSorted/d_flow_mgr.h"

struct dTimeBits {
    dTimeBits() : field_0x00(false), field_0x01(false) {}
    bool field_0x00;
    bool field_0x01;
};

class dAcOLock_c;

class dAcOdoor_c : public dAcObjDoor_c {
public:
    enum Subtype_e {
        SUBTYPE_0, ///< DoorA00
        SUBTYPE_1, ///< DoorA01
        SUBTYPE_2, ///< DoorC00
        SUBTYPE_3, ///< DoorC01
        SUBTYPE_4, ///< DoorB00
        SUBTYPE_5, ///< DoorE
        SUBTYPE_6, ///< DoorA02
        SUBTYPE_7, ///< DoorF
        SUBTYPE_8, ///< DoorH
    };

public:
    dAcOdoor_c() : mEventRelated(*this, nullptr), mEmmiterL(this), mEmmiterR(this) {}
    virtual ~dAcOdoor_c() {}

    virtual int doDelete() override;
    virtual int draw() override;
    virtual bool createHeap() override;
    virtual int actorCreate() override;
    virtual int actorPostCreate() override;
    virtual int actorExecute() override;
    virtual int actorExecuteInEvent() override;
    virtual void registerInEvent() override;
    virtual void unkVirtFunc_0x6C() override;
    virtual void doInteraction(s32 /* InteractionType */) override;
    virtual bool canOpen() override;

    /** */
    bool isLocked();

    /** fn_572_33E0 - I Think this gets the Door open range*/
    static f32 fn_572_33E0(); // -80.f

    /** */
    void startPullEventWithoutCallback(void *zevData);

    /** */
    void startPullEventWithCallback(void *zevData);

    /** */
    void startPushEventWithoutCallback(void *zevData);

    /** */
    void startPushEventWithCallback(void *zevData);

    /** */
    void startPullEvent(void *zevData);

    /** */
    void startPushEvent(void *zevData);

    /** */
    void startOpenEvent(void *zevData);

    /** */
    static void doorPullEventCallback(dAcOdoor_c *);

    /** */
    static void doorPushEventCallback(dAcOdoor_c *);

    /** */
    void startDoorPullLockedEvent(void *zevData);

    /** */
    void startDoorPushLockedEvent(void *zevData);

    /** */
    void startLockedEvent(void *zevData);

    /** */
    static void pullLockedEventCallback(dAcOdoor_c *);

    /** */
    static void pushLockedEventCallback(dAcOdoor_c *);

    /** */
    void startUnlockEvent();

    /** */
    static void unlockEventCallback(dAcOdoor_c *);

    /** */
    static void openCallbackCommon(dAcOdoor_c *);

    /** */
    s32 getKeyParameter(); // getFromParams(6, 0x1)

    /**  */
    s32 getSubtype2(); // (mRotation.x >> 0) & 0xFF

    /**  */
    u8 getSceneflag(); // (mRotation.x >> 8) & 0xFF

    /** fn_572_3F60 */
    u8 fn_572_3F60(); // (mRotation.z >> 0) & 0xF

    /** fn_572_3F70 */
    u8 fn_572_3F70(); // (mRotation.z >> 4) & 0x3F

    /** fn_572_3F80 */
    u8 fn_572_3F80(); // (mRotation.z >> 10) & 0x3F

    /** fn_572_3F90 */
    u8 fn_572_3F90(); // (getParams2Lower() >> 0) & 0xFF

    /** fn_572_3FC0 */
    static f32 getInteractionMinX(); // -80.f

    /** fn_572_3FD0 */
    static f32 getInteractionMaxX(); // 80.f

    /** fn_572_3FE0 */
    static f32 getInteractionMinZ(); // -100.f

    /** fn_572_3FF0 */
    static f32 getInteractionMaxZ(); // 100.f

    /** fn_572_4000 */
    static f32 getInteractionLockMinX(); // -160.f

    /** fn_572_4010 */
    static f32 getInteractionLockMaxX(); // 160.f

    /** fn_572_4020 */
    static f32 getInteractionLockMinZ(); // -100.f

    /** fn_572_4030 */
    static f32 getInteractionLockMaxZ(); // 100.f

    /** */
    void setRoomId(s8 roomId); // mRoomId = roomId;

    /** fn_572_4050 */
    void fn_572_4050(u32 flags); // field_0x5A8 = flags

    /** fn_572_4060 */
    void fn_572_4060(s8); // field_0x5B3 = in

    /** fn_572_4070 */
    void fn_572_4070(s8); // field_0x5B4 = in

    /** fn_572_4080 */
    void fn_572_4080(s8); // field_0x5B5 = in

    /** fn_572_4090 */
    void fn_572_4090(); // field_0x5B7 = true

    /** fn_572_40A0 */
    void fn_572_40A0(); // field_0x5B7 = false

    /** fn_572_40B0 */
    void fn_572_40B0(); // field_0x5B8 = true

    /** fn_572_40C0 */
    void fn_572_40C0(); // field_0x5B8 = false

    /** fn_572_40D0 */
    void fn_572_40D0(); // field_0x5B9 = true

    /** fn_572_40E0 */
    bool fn_572_40E0() const; // field_0x5A8 & 1

    /** fn_572_40F0 */
    void fn_572_40F0(); // field_0x5A8 |= 1

    /** fn_572_4100 */
    void fn_572_4100(); // field_0x5A8 &= ~1

    /** fn_572_4110 */
    void fn_572_4110(); // field_0x5A8 |= 2

    /** fn_572_4120 */
    void fn_572_4120(); // field_0x5A8 &= ~2

    /** fn_572_4130 */
    bool fn_572_4130(bool &b) const; // checkRoom(field_0x5B4,b)

    /** fn_572_4140 */
    bool fn_572_4140(bool &b) const; // checkRoom(field_0x5B5,b)

    /** fn_572_4150 */
    bool fn_572_4150(bool &b) const; // fn_572_4130(b0) && fn_572_4140(b1) -> b = b0 && b1

    /** */
    void triggerExit();

    /** */
    bool isLeavingSealedTempleSideDoorPostSkyKeep() const;

    /** fn_572_4310 */
    void transitionPushRoomFlags() const;

    /** fn_572_4320 */
    void fn_572_4320() const; // nop

    /** fn_572_4430 */
    bool transitionPullRoomFlags() const;

    /** fn_572_4340 */
    void fn_572_4340() const; // nop

    /** fn_572_4370 */
    bool fn_572_4370(const mVec3_c &point) const; // Is in front of door

    /** */
    bool isPlayerInFrontOfDoor() const;

    /** */
    bool hasFlowEntryPoint() const; // getFromParams(16, 0xFFFF)

    /** fn_572_4430 */
    bool fn_572_4430() const; // field_0x5A8 & 2

    /** fn_572_4440 */
    void fn_572_4440(); // Sets the Postion Copies

    /** */
    bool isPlayerInteractable() const;

    /** fn_572_4600 */
    bool isPlayerInteractableLocked() const;

    /** */
    void playInteractionLocked() const;

public:
    s32 getType() const {
        return getFromParams(0, 0x3F);
    }
    s32 getField_0x5B4() const {
        return field_0x5B4;
    }
    s32 getField_0x5B5() const {
        return field_0x5B5;
    }

public: // Functions in this section defined in main dol
    bool isConnectedToOtherDoor() const;
    dAcRef_c<dAcOdoor_c> &getConnectedDoorRef(); // return mConnectedDoor;

    /** fn_80194C70 : Rotates the input by this->mRotation.y and adds this->mPosition */
    void stepTowards(mVec3_c &newPosition) const;

private:
    /* 0x33C */ m3d::smdl_c mMdl0;
    /* 0x358 */ m3d::smdl_c mMdl1;
    /* 0x374 */ m3d::mdlAnmChr mAnmChr;
    /* 0x3DC */ ActorEventRelated mEventRelated;
    /* 0x42C */ dFlowMgr_c mFlowMgr;
    /* 0x4E8 */ dEmitter_c mEmmiterL;
    /* 0x51C */ dEmitter_c mEmmiterR;
    /* 0x550 */ dTimeBits mTimeBits;
    /* 0x554 */ dAcRef_c<dAcOdoor_c> mConnectedDoor;
    /* 0x560 */ dAcRef_c<dAcOLock_c> mLock;
    /* 0x56C */ dAcRef_c<dAcObjBase_c> mObjRef;
    /* 0x578 */ mMtx_c mMtx;
    /* 0x5A8 */ u8 _0x5A8[0x5AC - 0x5A8];
    /* 0x5AC */ u32 mRumbleIdx; ///<
    /* 0x5B0 */ u8 field_0x5B0; ///< some timer to trigger exit
    /* 0x5B1 */ u8 field_0x5B1; ///<
    /* 0x5B2 */ u8 mSceneflag;  ///< Used for unlocking the door
    /* 0x5B3 */ u8 field_0x5B3; ///<
    /* 0x5B4 */ s8 field_0x5B4;
    /* 0x5B5 */ s8 field_0x5B5;
    /* 0x5B6 */ s8 field_0x5B6;
    /* 0x5B7 */ bool field_0x5B7;
    /* 0x5B8 */ u8 _0x5B8[0x5BB - 0x5B8];
    /* 0x5BB */ bool field_0x5BB;
};

#endif
