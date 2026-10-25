#ifndef D_ZEV_H
#define D_ZEV_H

#include "common.h"
#include "d/a/d_a_base.h"
#include "d/d_bzs_types.h"
#include "sized_string.h"
#include "toBeSorted/d_emitter.h"
#include "toBeSorted/d_flow_mgr.h"

// Scenes > Actors > Cuts
// Actors each execute their cuts sequentially, with optional
// waits for other actors' cuts

// raw data definitions for .zev file binary contents

/** Ghidra: "ZevDatHeader" */
struct ZevHeader {
    /* 0x00 */ char magic[2]; // "wZ"
    /* 0x02 */ s16 numScene;
    /* 0x04 */ s16 numActor;
    /* 0x06 */ s16 numCuts;
    /* 0x08 */ s16 numCutData;
    /* 0x0A */ s16 numData;
    /* 0x0C */ char magic_ev[2]; // "Ev"
    /* 0x0E */ s16 numInt;
    /* 0x10 */ s16 numFloat;
    /* 0x12 */ s16 numString;
};

/** Ghidra: "ZevEventEntry" */
struct ZevScene {
    /* 0x00 */ char name[0x20];
    /* 0x20 */ s16 field_0x20;
    /* 0x22 */ u8 _0x22[0x24 - 0x22];
    /* 0x24 */ s16 actorOfs;
    /* 0x28 */ s16 actorNum;
};

/** Ghidra: "ZevActorDef" */
struct ZevActor {
    /* 0x00 */ char name[0x20];
    /* 0x20 */ s16 field_0x20;
    /* 0x22 */ u8 _0x22[0x24 - 0x22];
    /* 0x24 */ s16 cutOfs;
    /* 0x28 */ s16 cutNum;
};

/** Ghidra: "ZevStep1" */
struct ZevCut {
    /* 0x00 */ char name[0x10];
    /* 0x10 */ s16 waitFor;
    /* 0x12 */ s16 actorNo;
    /* 0x14 */ s16 field_0x14;
    /* 0x16 */ u8 _0x16[2];
    /* 0x18 */ s16 index;
    /* 0x1A */ s16 advanceConditionNum;
};

/** Ghidra: "ZevStep2" */
struct ZevCutData {
    /* 0x00 */ u32 name;
    /* 0x04 */ s16 field_0x4;
    /* 0x06 */ u16 index;
    /* 0x08 */ s16 dataOfs;
    /* 0x0A */ s16 dataNum;
};

enum ZevDatumType {
    ZEV_TYPE_INTEGER = 0,
    ZEV_TYPE_FLOAT = 1,
    ZEV_TYPE_STRING = 2,
};

/** Ghidra: "ZevLinkToData" */
struct ZevDatum {
    /* 0x00 */ s32 command;
    /* 0x04 */ u8 _0x04[0x06 - 0x04];
    /* 0x06 */ s16 type;
    /* 0x08 */ s16 index;
    /* 0x0A */ s16 length;
};

// game objects with parsed zev data, referring to raw file data

class dZevEvent_c;

/** Ghidra: EventRelocEntry */
class dZevFile_c {
public:
    dZevFile_c() {
        clear();
    }

    void clear();
    void setZev(void *zev);

    ZevScene *findScene(const char *sceneName);

    ZevScene *getScene(int i) {
        return &mpScenes[i];
    }

    ZevActor *getActor(int i) {
        return &mpActors[i];
    }

    ZevCut *getCut(int i) {
        return &mpCuts[i];
    }

    ZevCutData *getCutData(int i) {
        return &mpCutData[i];
    }

    ZevDatum *getDatum(int i) {
        return &mpData[i];
    }

    int *getInteger(int i) {
        return &mpInts[i];
    }

    f32 *getFloat(int i) {
        return &mpFloats[i];
    }

    char *getString(int i) {
        return &mpStrings[i];
    }

    static void *getDefaultZevDat();

private:
    /* 0x00 */ bool mInitialized;
    /* 0x04 */ ZevHeader *mpHeader;
    /* 0x08 */ ZevScene *mpScenes;
    /* 0x0C */ ZevActor *mpActors;
    /* 0x10 */ ZevCut *mpCuts;
    /* 0x14 */ ZevCutData *mpCutData;
    /* 0x18 */ ZevDatum *mpData;
    /* 0x1C */ int *mpInts;
    /* 0x20 */ f32 *mpFloats;
    /* 0x24 */ char *mpStrings;
};

/** Ghidra: EventSystemActorRelation */
class dZevActor_c {
public:
    enum ActorType_e {
        AC_GAME = -1,
        AC_NONE = 0,
        AC_ACTOR = 1,
        AC_PROFILE = 2,
    };

    // Maybe overloads aren't the best idea here with u16 vs int
    void init(ZevActor *zevActor, dZevEvent_c *event, dAcBase_c *gameActor);    // "(o)"
    void init(ZevActor *zevActor, dZevEvent_c *event, u16 profileName);         // "(g)"
    void init(ZevActor *zevActor, dZevEvent_c *event, int arg);                 // "(i)"
    void init(int arg, const char *actorName, dZevEvent_c *event);              // "(xi)"
    void init(dAcBase_c *gameActor, const char *actorName, dZevEvent_c *event); // "(xo)"
    void init(u16 profileName, const char *actorName, dZevEvent_c *event);      // "(xg)"
    void init(ZevActor *zevActor, dZevEvent_c *event);
    void clear();

    ZevCut *getCut(int idx);
    const char *getCutName(int idx);
    ZevCutData *getCutData(int dataIdx, int cutIdx);

    bool advanceConditionMet(int cutIdx);
    bool advanceBlocked();
    bool tryAdvanceCut();
    void signalAdvance(int signal);
    u32 getCommand(int dataIdx);
    int getCutUnk(int dataIdx);
    int getIntData(int **outData, int name, int dataIdx);
    int getFloatData(f32 **outData, int name, int dataIdx);
    int getVecData(Vec **outData, int name, int dataIdx);
    int getStringData(char **outData, int name, int dataIdx);

    const char *getName() const {
        return mInit ? mZevActorName.mChars : "???";
    }

    bool isInit() const {
        return mInit;
    }

    UNKWORD getField_0x04() const {
        return field_0x04;
    }

    void setField_0x04(UNKWORD i) {
        field_0x04 = i;
    }

    void tickField_0x04() {
        if (field_0x04 != 0) {
            field_0x04--;
        }
    }

    UNKWORD getField_0x44() const {
        return field_0x44;
    }

    int getCurrentCutIdx() const {
        return mCurrentCutIdx;
    }

    u16 getProfileId() const {
        return mProfileName;
    }

    bool isProfile(u16 profile) const {
        return (mActorType == AC_PROFILE || mActorType == AC_ACTOR) && mProfileName == profile;
    }

    bool isInitAndName(const char *name) const {
        return mInit && mZevActorName == SizedString<0x20>(name);
    }

    bool isInitAndProfile(u16 profile) const {
        return mInit && isProfile(profile);
    }

    bool isArg(int arg) const {
        return mActorType == AC_ACTOR && field_0x40 == arg;
    }

    bool isInitAndArg(int arg) const {
        return mInit && isArg(arg);
    }

    void link(dAcBase_c *ac) {
        mGameActor.link(ac);
        mActorType = AC_ACTOR;
    }

    bool isActor(dAcBase_c *ac) const {
        return mActorType == AC_ACTOR && mGameActor.get() == ac;
    }

    ZevActor *getZevActor() const {
        return mpZevActor;
    }

    bool isZevActor(ZevActor *ac) const {
        return mpZevActor == ac;
    }

    bool isInitAndActor(dAcBase_c *ac) const {
        return mInit && isActor(ac);
    }

    bool isOutstandingCut(u32 mask) const {
        return (mAvailableCutsMask & mask) != 0 && (mDoneCutsMask & mask) == 0;
    }

    bool isExtra() const {
        return mInit && (field_0x40 == 0x80 || mpZevActor == nullptr);
    }

private:
    /* 0x00 */ bool mInit;
    /* 0x04 */ UNKWORD field_0x04;
    /* 0x08 */ ZevActor *mpZevActor;
    /* 0x0C */ SizedString<0x20> mZevActorName;
    /* 0x2C */ int mActorType;
    /* 0x30 */ u16 mProfileName;
    /* 0x34 */ dAcRef_c<dAcBase_c> mGameActor;
    /* 0x40 */ UNKWORD field_0x40;
    /* 0x44 */ UNKWORD field_0x44;
    /* 0x48 */ int mCurrentCutIdx;
    /* 0x4C */ UNKWORD field_0x4C[0x20];
    /* 0xCC */ dZevEvent_c *mpEvent;
    /* 0xD0 */ u32 mAvailableCutsMask;
    /* 0xD4 */ u32 mDoneCutsMask;
    /* 0xD8 */ u32 mAdvanceCondition;
    /* 0xDC */ u32 mAdvanceSignal;
};

/** Ghidra: EventRelocTable */
class dZevEvent_c {
public:
    dZevEvent_c() : field_0x024(0), mCurrentFrame(-1) {
        clearFiles();
        init();
    }

    void init();
    void stop();

    void loadFile(int slot, void *data);

    bool isInit() const {
        return mIsStart;
    }

    int getCurrentFrame() const {
        return mCurrentFrame;
    }

    UNKWORD getField_0x024() const {
        return field_0x024;
    }

    dZevFile_c *getFile() {
        return mpCurrentFile;
    }

    dZevActor_c *getActorsPtr() {
        return mActors;
    }

    dAcBase_c *getEventStarter() {
        return mEventStarter.get();
    }

    dAcBase_c *getEventPartner() {
        return mEventPartner.get();
    }

    void setEventPartner(dAcBase_c *ac) {
        mEventPartner.set(ac);
    }

    dAcBase_c *getEventSpeaker() {
        return mEventSpeaker.get();
    }

    void setEventSpeaker(dAcBase_c *ac) {
        mEventSpeaker.set(ac);
    }

    static dZevEvent_c *GetInstance() {
        return &sInstance;
    }

    bool cutCompletedMaybe(int cutIdx);

    bool isIdle() const;
    bool isBusy() const;
    bool calc();
    bool startScene(const char *sceneName, dAcBase_c *ac1, dAcBase_c *ac2);
    bool startSoloScene(const char *sceneName, const char *actorName);
    bool stopScene(const char *name);

    int findAndInitEventActor(const char *name);
    int findAndInitEventActor(int arg, const char *name);
    int findAndInitEventActor(dAcBase_c *ac, const char *name);
    int findAndForceInitEventActor(dAcBase_c *ac, const char *name);
    int findAndInitEventActor(u16 profile, const char *name);

    int findActorIndexByName(const char *name);
    int findActorIndexByPointer(dAcBase_c *ac);
    int findActorIndexByArg(int arg);
    dAcBase_c *findActorBySpecialName(const char *name);

    bool setActorRef(int idx, dAcBase_c *ac);

    bool checkSceneMask(u32 mask);

private:
    void clearFiles();
    void clear(bool force);

    bool startSceneInternal(const char *sceneName, dAcBase_c *ac1, dAcBase_c *ac2);
    bool startSoloSceneInternal(const char *sceneName, const char *actorName);
    bool stopSceneInternal(const char *name);

    void initAllEventActor();
    int findAndInitEventActor(ZevActor *ac);
    bool processAdvancement();

    struct SplitHelper {
        SizedString<0x20> left;
        SizedString<0x20> right;
    };

    bool trySplit(const char *src, char sep, SplitHelper &out);

    bool isOutstandingActor(u32 mask) const {
        return (mAvailableActorsMask & mask) != 0 && (mDoneActorsMask & mask) == 0;
    }

    const char *getEventName() const {
        return mIsStart ? mEventName.mChars : "???";
    }

    bool isStartAndName(const char *name) const {
        return mIsStart && mEventName == SizedString<0x20>(name);
    }

    static dZevEvent_c sInstance;

    /* 0x000 */ bool mIsStart;
    /* 0x001 */ bool mIsOver;
    /* 0x002 */ SizedString<0x20> mEventName;
    /* 0x024 */ UNKWORD field_0x024;
    /* 0x028 */ int mCurrentFrame;
    /* 0x02C */ dZevFile_c mZevFiles[5];
    /* 0x0F4 */ dAcRef_c<dAcBase_c> mEventStarter;
    /* 0x100 */ dAcRef_c<dAcBase_c> mEventPartner;
    /* 0x10C */ dAcRef_c<dAcBase_c> mEventSpeaker;
    /* 0x118 */ dAcRef_c<dAcBase_c> mOtherActors[4];
    /* 0x148 */ dZevFile_c *mpCurrentFile;
    /* 0x14C */ ZevScene *mpCurrentScene;
    /* 0x150 */ dZevActor_c mActors[16];
    /* 0xF50 */ u32 mAvailableActorsMask;
    /* 0xF54 */ u32 mDoneActorsMask;
};

// TODO - not sure
struct EventDataPair {
    s32 toolDataId;
    s32 roomId;
};

// TODO - rename to something like EventParams?
class Event {
public:
    typedef void (*EventCallback)(dAcBase_c *ac);

    /* 0x00 */ UNKWORD field_0x00;
    /* 0x04 */ u32 mEventFlags;
    /* 0x08 */ EventDataPair p;
    /* 0x10 */ SizedString<0x20> mEventName;
    /* 0x30 */ void *eventZevData;
    /* 0x34 */ EventCallback mStartCallback;
    /* 0x38 */ EventCallback mEndCallback;

public:
    static u32 makeEventFlag(const u32 &clearMask, const u32 &unsetBits) {
        return clearMask & ~unsetBits;
    }

    bool isName(const char *name) const {
        return mEventName == SizedString<0x20>(name);
    }

    Event()
        : field_0x00(0x1FF),
          mEventFlags(0x100001),
          eventZevData(nullptr),
          mStartCallback(nullptr),
          mEndCallback(nullptr) {}
    void clear() {
        field_0x00 = 0x1FF;
        mEventFlags = 0x100001;
        mEventName.empty();
        eventZevData = nullptr;
        mStartCallback = nullptr;
        mEndCallback = nullptr;
    }

    Event(const char *eventName, u32 unk, u32 eventFlags, EventCallback startCallback, EventCallback endCallback);
    Event(
        const char *eventName, void *zevDat, u32 unk, u32 eventFlags, EventCallback startCallback,
        EventCallback endCallback
    );
    Event(s32 eventId, s32 roomId, u32 eventFlags, EventCallback startCallback, EventCallback endCallback);
    Event(
        s32 eventId, s32 roomId, void *zevDat, u32 eventFlags, EventCallback startCallback, EventCallback endCallback
    );
    virtual ~Event();
};

struct StoredEvent {
    /* 0x00 */ bool field_0x00;
    /* 0x04 */ dAcRef_c<dAcBase_c> mEventStarter;
    /* 0x10 */ dAcRef_c<dAcBase_c> mEventPartner;
    /* 0x1C */ UNKWORD field_0x1C;
    /* 0x20 */ Event mEvent;

    void clear() {
        field_0x00 = 0;
        mEventStarter.unlink();
        mEventPartner.unlink();
        field_0x1C = -1;
        mEvent.clear();
    }

    void copyFrom(const StoredEvent &other) {
        field_0x00 = other.field_0x00;
        mEventStarter.set(const_cast<dAcBase_c *>(other.mEventStarter.get()));
        mEventPartner.set(const_cast<dAcBase_c *>(other.mEventPartner.get()));
        field_0x1C = other.field_0x1C;
        mEvent = other.mEvent;
    }
};

class ActorEventRelated {
private:
    /* 0x00 */ bool mInit;
    /* 0x04 */ dZevEvent_c *mpZev;
    /* 0x08 */ dAcRef_c<dAcBase_c> mAcRef;
    /* 0x14 */ int mArg;
    /* 0x18 */ SizedString<0x20> mName;
    /* 0x38 */ UNKWORD field_0x38;
    /* 0x3C */ UNKWORD field_0x3C;
    /* 0x40 */ UNKWORD mCutIndex;
    /* 0x44 */ int mZevActorIndex;
    /* 0x48 */ u32 mFlags;

    void initActor(dAcBase_c *actor);
    void initArg(int arg);

    void remove();
    void setup();

    int updateInternal();

    // looks kinda fake, works with instructions but still has a regswap
    dZevActor_c *getActor() {
        return mpZev->getActorsPtr() + mZevActorIndex;
    }

    enum Flag_e {
        FLAG_0x01 = 0x01,
        FLAG_0x02 = 0x02,
        FLAG_0x04 = 0x04,
        FLAG_0x08 = 0x08,
        FLAG_0x10 = 0x10,
    };

    void setFlag(u32 flag) {
        mFlags |= flag;
    }

    bool checkFlag(u32 flag) const {
        return (mFlags & flag) != 0;
    }

public:
    ActorEventRelated(dAcBase_c &owner, dZevEvent_c *instance);
    ActorEventRelated(int arg, dZevEvent_c *instance);
    virtual ~ActorEventRelated();

    void reset();
    void initName(const char *name);

    bool cutEnd();
    int getCutIndex();

    bool isThisActorInEvent();
    bool scheduleEvent(Event &event, dAcBase_c *ac);
    bool isInEvent(char *name);
    u32 getCurrentEventCommand();
    void advanceNext();
    bool isAdvance();
    int getSomeEventRelatedNumber();

    int getSingleShortData(int *result, u32 code, u32);
    int getSingleIntData(int *result, u32 code, u32);
    int getSingleFloatData(f32 *result, u32 code, u32);
    int getSingleVecData(mVec3_c *result, u32 code, u32);

    int getSingleShortDataWithDefault(int *result, u32 code, const int &def, u32);
    int getSingleIntDataWithDefault(int *result, u32 code, const int &def, u32);
    int getSingleFloatDataWithDefault(f32 *result, u32 code, const f32 &def, u32);
    int getSingleVecDataWithDefault(mVec3_c *result, u32 code, const mVec3_c &def, u32);

    int getShortDataBuf(int **result, u32 code, u32);
    int getFloatDataBuf(f32 **result, u32 code, u32);
    int getVecDataBuf(mVec3_c **result, u32 code, u32);
    int getStringDataBuf(char **result, u32 code, u32);

    int getActorByStringData(dAcBase_c **result, u32 code, u32);
    int getActorByStringDataWithDefault(dAcBase_c **result, u32 code, dAcBase_c *def, u32);
    int getActorByStringDataOrSpecialName(dAcBase_c **result, u32 code, const char *name, u32);
};

class EventManager {
private:
    enum State_e {
        STATE_0,
        STATE_1,
        STATE_2,
        STATE_3,
        STATE_4,
        STATE_5,
        STATE_6,
        STATE_7,
    };

    enum Flags_e {
        FLAGS_0x1 = 1 << 0,
        FLAGS_0x2 = 1 << 1,
        FLAGS_0x4 = 1 << 2,
        FLAGS_0x8 = 1 << 3,

        FLAGS_0x10 = 1 << 4,
        FLAGS_0x20 = 1 << 5,
        FLAGS_0x40 = 1 << 6,
        FLAGS_0x80 = 1 << 7,

        FLAGS_0x100 = 1 << 8,
        FLAGS_0x200 = 1 << 9,
        FLAGS_0x400 = 1 << 10,
        FLAGS_0x800 = 1 << 11,

        FLAGS_0x1000 = 1 << 12,
        FLAGS_0x2000 = 1 << 13,
        FLAGS_0x4000 = 1 << 14,
        FLAGS_0x8000 = 1 << 15,

        FLAGS_0x10000 = 1 << 16,
        FLAGS_0x20000 = 1 << 17,
        FLAGS_0x40000 = 1 << 18,
        FLAGS_0x80000 = 1 << 19,

        FLAGS_0x100000 = 1 << 20,
        FLAGS_0x200000 = 1 << 21,
        FLAGS_0x400000 = 1 << 22,
        FLAGS_0x800000 = 1 << 23,

        FLAGS_0x1000000 = 1 << 24,
    };

public:
    EventManager();
    virtual ~EventManager();

    void startEventManager();
    void killEventManager();

    static bool setEvent(dAcBase_c *actor, const Event *, dAcBase_c *other);
    static bool finishEvent(dAcBase_c *actor, const char *eventName);
    static bool changeOwnEvent(dAcBase_c *actor, dAcBase_c *other, const Event *event, dAcBase_c *third);

    static bool isInEvent(int arg, const char *eventName);
    static bool isInEvent(dAcBase_c *actor, const char *eventName);

    static bool alsoSetAsCurrentEvent(dAcBase_c *actor, const Event *event, dAcBase_c *actor2);
    static bool alsoSetAsCurrentEvent(int arg, const Event *event, dAcBase_c *actor);

    static bool canSkipCurrentEvent();
    static dAcBase_c *getCurrentEventActor();
    static dAcBase_c *getEventPartner();
    static dAcBase_c *getEventSpeaker();
    static void setEventPartner(dAcBase_c *ac);
    static void setEventSpeaker(dAcBase_c *ac);
    static void setActorRef(int idx, dAcBase_c *ac);
    static dAcBase_c *findActorBySpecialName(const char *name);
    static dAcBase_c *getCurrentActorIfGroup(u8 groupType);
    static dAcBase_c *getSpeakerIfGroup(u8 groupType);

    static Event *getSkipOnlyEvent();
    static bool fn_800A0B80();
    static bool eventRelatedStateFlags_shift0x11_1();
    static bool fn_800A0BA0();
    static bool fn_8009FA10(dAcBase_c *actor, const Event *event, void *unknown);
    static void execute();
    static void fn_8009E110();
    static void fn_8009EEA0();

    static EventManager *sInstance;

    static bool isInEvent() {
        return sInstance != nullptr && sInstance->mState != STATE_0;
    }

    static bool isInEvent0Or7() {
        return sInstance != nullptr && (sInstance->mState == STATE_0 || sInstance->mState == STATE_7);
    }

    static bool isInEventOtherThan0Or7() {
        return isInEvent() && !isInEvent0Or7();
    }

    static const char *getCurrentEventName() {
        return isInEvent() ? sInstance->mCurrentEvent.mEvent.mEventName.mChars : nullptr;
    }

    static s32 getCurrentToolDataId() {
        return isInEvent() ? sInstance->mCurrentEvent.mEvent.p.toolDataId : -1;
    }

    static bool isCurrentEvent(const char *name) {
        return strequals(getCurrentEventName(), name);
    }

    static bool checkFlagSafe(int flag) {
        if (!isInEvent()) {
            flag &= ~(FLAGS_0x1000000 | FLAGS_0x200000 | FLAGS_0x100000 | FLAGS_0x40000 | FLAGS_0x20000 | FLAGS_0x10000 |
                  FLAGS_0x8000 | FLAGS_0x4000 | FLAGS_0x2000 | FLAGS_0x1000 | FLAGS_0x800 | FLAGS_0x400 | FLAGS_0x200 |
                  FLAGS_0x100 | FLAGS_0x80 | FLAGS_0x40 | FLAGS_0x20 | FLAGS_0x10 | FLAGS_0x8 | FLAGS_0x4 | FLAGS_0x2 |
                  FLAGS_0x1);
        }
        return checkFlag(flag);
    }

    static void setFlagSafe(int flag) {
        if (!isInEvent()) {
            flag &= ~(FLAGS_0x1000000 | FLAGS_0x200000 | FLAGS_0x100000 | FLAGS_0x40000 | FLAGS_0x20000 | FLAGS_0x10000 |
                  FLAGS_0x8000 | FLAGS_0x4000 | FLAGS_0x2000 | FLAGS_0x1000 | FLAGS_0x800 | FLAGS_0x400 | FLAGS_0x200 |
                  FLAGS_0x100 | FLAGS_0x80 | FLAGS_0x40 | FLAGS_0x20 | FLAGS_0x10 | FLAGS_0x8 | FLAGS_0x4 | FLAGS_0x2 |
                  FLAGS_0x1);
        }
        setFlag(flag);
    }

    static void onRoomChange();

    static void initEventActor(dAcBase_c *actor);
    // Something like isActorInEvent maybe?
    static bool isActorInEvent(dAcBase_c *actor);
    static void clearEventActor(dAcBase_c *actor);
    static void findAndForceInitEventActor(dAcBase_c *ac, const char *name);
    static void findAndInitEventActor(u16 profileId, const char *name);
    static bool FUN_800a0ba0();
    static bool fn_8009F990();

    static void processEventFlags();

private:
    static bool checkButton2HeldForCutscenes();
    static void fn_8009EAF0(int);
    static void fn_8009EB00(int);
    static void fn_8009EB10(int);
    static bool fn_8009FE00(UNKWORD, const Event *event, dAcBase_c *actor);
    static bool fn_8009E0A0();
    static void copyQueuedEventToCurrentAndHandleSkipEvent();
    static const EVNT *getCurrentExecutedEVNT();
    static s32 getCurrentEventRoomId();
    static bool attachSkipEvent(dAcBase_c *actor, u8 flag, const Event *event, dAcBase_c *other);

    static void setState(int state) {
        sInstance->mState = state;
    }

    static void setFlag(u32 flag) {
        sInstance->mFlags |= flag;
    }

    static void unsetFlag(u32 flag) {
        sInstance->mFlags &= ~flag;
    }

    static bool checkFlag(u32 flag) {
        return (sInstance->mFlags & flag) != 0;
    }

    static StoredEvent *getCurrentEvent() {
        return &sInstance->mCurrentEvent;
    }

    /* 0x004 */ StoredEvent field_0x004;
    /* 0x064 */ StoredEvent mCurrentEvent;
    /* 0x0C4 */ StoredEvent field_0x0C4;
    /* 0x124 */ StoredEvent field_0x124;
    /* 0x184 */ int mState;
    /* 0x188 */ u32 mFlags;
    /* 0x18C */ u16 field_0x18C;
    /* 0x18E */ u16 field_0x18E;
    /* 0x190 */ s16 field_0x190;
    /* 0x192 */ s16 field_0x192;
    /* 0x194 */ s16 field_0x194;
    /* 0x196 */ s16 field_0x196;
    /* 0x198 */ u8 field_0x198;
};

class ActorEventRelated1 {
protected:
    /* 0x00 */ ActorEventRelated field_0x00;

public:
    ActorEventRelated1(int arg) : field_0x00(arg, nullptr) {}

    /* 0x50 */ /* vtable */
    /* vt 0x08 */ virtual void vt_0x08() = 0;
    /* vt 0x0C */ virtual void execute() = 0;
    /* vt 0x10 */ virtual void vt_0x10() {}
    /* vt 0x14 */ virtual void vt_0x14() {
        field_0x00.reset();
    }
};

class ActorEventRelatedDirector : public ActorEventRelated1 {
public:
    ActorEventRelatedDirector() : ActorEventRelated1(0) {}

    /* vt 0x08 */ virtual void vt_0x08() override;
    /* vt 0x0C */ virtual void execute() override;

    void initName(const char *name) {
        field_0x00.reset();
        field_0x00.initName(name);
    }

    void fn_800A18E0();
    void start();
    void executeInEvent();

private:
    /* 0x054 */ u8 _0x054[0x064 - 0x054];
    /* 0x064 */ dAcRef_c<dAcBase_c> mAc1;
    /* 0x070 */ dAcRef_c<dAcBase_c> mAc2;
    /* 0x07C */ u8 _0x07C[0x10C - 0x07C]; // not sure
};

class ActorEventRelatedSound : public ActorEventRelated1 {
public:
    ActorEventRelatedSound() : ActorEventRelated1(3) {}

    /* vt 0x08 */ virtual void vt_0x08() override;
    /* vt 0x0C */ virtual void execute() override;

    void start();
    void executeInEvent();

private:
    /* 0x54 */ u8 _0x54[0x58 - 0x54]; // not sure
};

class ActorEventRelatedFlow : public ActorEventRelated1 {
public:
    ActorEventRelatedFlow() : ActorEventRelated1(4), mFlowMgr(&mFlow) {}

    /* vt 0x08 */ virtual void vt_0x08() override;
    /* vt 0x0C */ virtual void execute() override;

    void fn_800A4530();

    void start();
    void executeInEvent();

private:
    /* 0x54 */ u8 _0x54[0x58 - 0x54];
    /* 0x58 */ dFlow_c mFlow;
    /* 0xBC */ dFlowMgrBase_c mFlowMgr;
    /* 0x54 */ u8 _0x545[0x10]; // not sure
};

class ActorEventRelatedEmitter : public ActorEventRelated1 {
public:
    ActorEventRelatedEmitter() : ActorEventRelated1(0x37) {}

    /* vt 0x08 */ virtual void vt_0x08() override;
    /* vt 0x0C */ virtual void execute() override;

    void start();
    void executeInEvent();

private:
    /* 0x54 */ u8 _0x54[0x58 - 0x54];
    /* 0x58 */ dEmitter_c mEmitter;
    /* 0x54 */ u8 _0x545[0x14]; // not sure
};

#endif
