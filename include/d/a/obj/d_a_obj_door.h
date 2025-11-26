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
        DOOR_NORMAL,            ///< DoorA00 - Normal Wooden Door
        DOOR_METAL,             ///< DoorA01 - Metal Door - Academy(bathrooms)
        DOOR_DOUBLE,            ///< DoorC00 - Normal Double Doors
        DOOR_DOUBLE_OUTSIDE,    ///< DoorC01 - Outdoor Double Doors - Academy(to outside)
        DOOR_TEMPLE_SIDE,       ///< DoorB00 - Sealed Temple Side Doors
        DOOR_SANDSEA_TIMESHIFT, ///< DoorE   - Sandsea Timeshift Door
        DOOR_BATREAUX,          ///< DoorA02 - Batreaux's Door
        DOOR_TEMPLE_MAIN,       ///< DoorF   - Sealed Temple Main Door
        DOOR_SPARRING_HALL,     ///< DoorH   - Sparring Hall Outer Doors
    };

    enum LockType_e {
        LOCK_NONE = 0, ///< Door does not have a lock
        LOCK_KEY = 1,  ///< Door usually is locked
    };

    enum LockBehavior_e {
        LOCK_NEVER = 0,     ///< Door is always unlocked
        LOCK_NOT_SET = 1,   ///< Door is locked when mSceneflag is unset
        LOCK_NEVER1 = 2,    ///< Door is always unlocked (Kukiel's House)
        LOCK_TIMESHIFT = 3, ///< Door is locked when mSceneflag is unset (Timeshift)
        LOCK_SET = 4,       ///< Door is locked when mSceneflag is set
        LOCK_DAY = 5,       ///< Door is locked during the day
        LOCK_NIGHT = 6,     ///< Door is locked during the night
        LOCK_MAX,
    };

public:
    dAcOdoor_c()
        : mEventRelated(*this, nullptr),
          mEmmiterL(this),
          mEmmiterR(this),
          mFlags(0),
          mExitTimer(0),
          mbInSandshipBoss(false),
          field_0x5BA(false) {}
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

    /** Gets whether the player can open the door */
    bool isLocked();

    /** fn_572_33E0 - I Think this gets the Door open range*/
    static f32 fn_572_33E0();

    /** Self Explanatory */
    void startPullEventWithoutCallback(void *zevData);

    /** Self Explanatory */
    void startPullEventWithCallback(void *zevData);

    /** Self Explanatory */
    void startPushEventWithoutCallback(void *zevData);

    /** Self Explanatory */
    void startPushEventWithCallback(void *zevData);

    /** Self Explanatory */
    void startPullEvent(void *zevData);

    /** Self Explanatory */
    void startPushEvent(void *zevData);

    /** Self Explanatory */
    void startOpenEvent(void *zevData);

    /** Self Explanatory */
    static void doorPullEventCallback(void *);

    /** Self Explanatory */
    static void doorPushEventCallback(void *);

    /** Self Explanatory */
    void startDoorPullLockedEvent(void *zevData);

    /** Self Explanatory */
    void startDoorPushLockedEvent(void *zevData);

    /** Self Explanatory */
    void startLockedEvent(void *zevData);

    /** Self Explanatory */
    static void pullLockedEventCallback(void *);

    /** Self Explanatory */
    static void pushLockedEventCallback(void *);

    /** Self Explanatory */
    void startUnlockEvent();

    /** Self Explanatory */
    static void unlockEventCallback(void *);

    /** Self Explanatory */
    static void openCallbackCommon(void *);

    /** Returns if the door is physically locked with the dAcOLock_c Actor */
    s32 getLockParameter();

    /** Gets the behavior of being locked when not locked with the dAcOLock_c Actor */
    s32 getLockBehavior();

    /** Get Scenflag used for locking status */
    u8 getSceneflag();

    /** Gets whether the door has callbacks enabled */
    u8 getDoorCallbackType();

    /** getFrontRoomParam */
    u8 getFrontRoomParam(); // (mRotation.z >> 4) & 0x3F

    /** getBackRoomParam */
    u8 getBackRoomParam(); // (mRotation.z >> 10) & 0x3F

    /** Gets the Flag set when failed to open - Called from Try Locked Callback */
    u16 getFailedToOpenFlag();

    /** Interaction Radius -X */
    static f32 getInteractionMinX();

    /** Interaction Radius +X */
    static f32 getInteractionMaxX();

    /** Interaction Radius -Z */
    static f32 getInteractionMinZ();

    /** Interaction Radius +Z */
    static f32 getInteractionMaxZ();

    /** Interaction Radius -X - Door has Lock */
    static f32 getInteractionLockMinX();

    /** Interaction Radius +X - Door has Lock */
    static f32 getInteractionLockMaxX();

    /** Interaction Radius -Z - Door has Lock */
    static f32 getInteractionLockMinZ();

    /** Interaction Radius +Z - Door has Lock */
    static f32 getInteractionLockMaxZ();

    /** Sets the RoomID */
    void setRoomId(s8 roomId);

    /** Set Flag - Usually Time Door related */
    void setFlag(u32 flags);

    /** Sets the behavior on Event creation  */
    void setEventCallbackType(u8);

    /** setFrontRoom */
    void setFrontRoom(s8); // mFrontRoomId = in

    /** setBackRoom */
    void setBackRoom(s8); // mBackRoomId = in

    /**  Sets status indicating it can be a double door  */
    void setDoubleDoor();

    /**  Sets status indicating it cant be a double door */
    void setNotDoubleDoor();

    /** Sets status indicating its a single door / Main door */
    void setSingleDoor();

    /** Sets status indicating its not a single door / not Main door */
    void setNotSingleDoor();

    /** setInSandshipBoss */
    void setInSandshipBoss(); // mbInSandshipBoss = true

    /** isTimeDoorEventActive */
    bool isTimeDoorEventActive() const;

    /** setTimeDoorEventActive */
    void setTimeDoorEventActive();

    /** setTimeDoorEventInactive */
    void setTimeDoorEventInactive();

    /** Sets status indicating it is effected by Timeshift */
    void setTimeEffected();

    /**  Sets status indicating it is not effected by Timeshift */
    void setTimeStatic();

    /** Checks to see if the Front room exists(return) and has flags set(b) */
    bool checkFrontRoom(bool &b) const;

    /** Checks to see if the Back room exists(return) and has flags set(b) */
    bool checkBackRoom(bool &b) const;

    /** Checks to see if the Front and Back room exists(return) and has flags set(b) */
    bool checkRooms(bool &b) const;

    /** Transitions the game to the next room */
    void triggerExit();

    /** TODO(Zeldex) Whats special about this? */
    bool isLeavingSealedTempleSideDoorPostSkyKeep() const;

    /**  */
    void transitionPushRoomFlags() const;

    /**  */
    void transitionPullRoomFlags() const;

    /** Checks a point to determine which side of the door it is on */
    bool isPositionInFrontOfDoor(const mVec3_c &point) const;

    /** Checks the players postion to see if it is front of the door. This means it will be pulled open */
    bool checkPullDoor() const;

    /** Checks to see if the door has a flow entry point */
    bool hasFlowEntryPoint();

    /** Checks to see if the Door is effected by Timeshift */
    bool isTimeEffected() const;

    /** Sets the Position of the Doorknob based on the Animation */
    void setDoorKnobPosition();

    /** Checks to see if the Player is within an interactable range */
    bool isPlayerInteractable() const;

    /** Checks to see if the Player is within an interactable range when door is locked */
    bool isPlayerInteractableLocked() const;

    /** Plays the talk event for when the door is locked */
    void playInteractionLocked();

public:
    s32 getType() const {
        return mParams & 0x3F;
    }
    s32 getField_0x5B4() const {
        return mFrontRoomId;
    }
    s32 getField_0x5B5() const {
        return mBackRoomId;
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
    /* 0x554 */ dAcRef_c<dAcOdoor_c> mConnectedDoor; ///< When the door is a double door, this is its pair
    /* 0x560 */ dAcRef_c<dAcOLock_c> mLock;
    /* 0x56C */ dAcRef_c<dAcObjBase_c> mObjRef; ///< TODO(Zeldex) This is probably the Sandship stage for boss

    /* 0x578 */ mMtx_c mMtx; ///< Currently unknown. I am assuming this is used to move the door on the Sandship stage
                             ///< while the boat is rocking

    /* 0x5A8 */ u32 mFlags;            ///< 0x2 - Time Effected
    /* 0x5AC */ u32 mRumbleIdx;        ///< Rumble used for when opening the main sealed temple door
    /* 0x5B0 */ u8 mExitTimer;         ///< Timer used to advance the Begin Pull/Push events
    /* 0x5B1 */ u8 mLockBehavior;      ///< see LockBehavior_e
    /* 0x5B2 */ u8 mSceneflag;         ///< Used for unlocking/locking the door.
    /* 0x5B3 */ u8 mEventCallbackType; ///< 0 for events without callbacks, 1 for events with a callback
    /* 0x5B4 */ s8 mFrontRoomId;       ///< Room id of the front side of the door
    /* 0x5B5 */ s8 mBackRoomId;        ///< Room id of the back side of the door
    /* 0x5B6 */ s8 mFramesInEvent;     ///< The count of frames in one particular event
    /* 0x5B7 */ bool mbDoubleDoor;     ///< Set if the type was matched to be a double door variant
    /* 0x5B8 */ bool mbSingleDoor;     ///< UNUSHED - Set if the type was matched to be a single door
    /* 0x5B9 */ bool mbInSandshipBoss; ///< Indicates if in the Tentalus fight
    /* 0x5BA */ bool field_0x5BA;      ///< ???
    /* 0x5BB */ bool mbKobunDoor;      ///< Used to indicate its Cawlin and Strich's door
};

#endif
