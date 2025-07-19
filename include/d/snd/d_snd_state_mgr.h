#ifndef D_SND_STATE_MGR_H
#define D_SND_STATE_MGR_H

#include "common.h"
#include "d/snd/d_snd_event.h"
#include "d/snd/d_snd_util.h"
#include "nw4r/snd/snd_FxReverbStdDpl2.h"
#include "nw4r/snd/snd_SoundHandle.h"
#include "sized_string.h"

SND_DISPOSER_FORWARD_DECL(dSndStateMgr_c);

// Previous Ghidra name: EnemySoundMgr
class dSndStateMgr_c {
public:
    SND_DISPOSER_MEMBERS(dSndStateMgr_c);

public:
    typedef void (*OnEventStartCallback)(s32 soundEventId, u32 flags);
    typedef void (*OnFlagsChangeCallback)(u32 newFlags, u32 oldFlags);

    enum StageFlags_e {
        STAGE_FIELD = 0x1,
        STAGE_DUNGEON = 0x2,
        STAGE_BOSS = 0x4,
        STAGE_SILENT_REALM = 0x8,

        STAGE_ROOM = 0x10, // only for sky

        STAGE_SKY = 0x20,
        STAGE_FOREST = 0x40,
        STAGE_MOUNTAIN = 0x80,
        STAGE_DESERT = 0x100,
        STAGE_SILENT_GROUNDS = 0x200,
    };

    enum EventFlags_e {
        EVENT_IN_EVENT = 0x1,
        EVENT_DEMO = 0x2,
        EVENT_0x04 = 0x4,
        EVENT_MUTE_BGM_PARTIAL = 0x8,
        EVENT_MUTE_BGM_FULL = 0x10,
        EVENT_MUTE_STAGE_EFFECTS_PARTIAL = 0x20,
        EVENT_MUTE_STAGE_EFFECTS_FULL = 0x40,
        EVENT_MUTE_ENEMY_PARTIAL = 0x80,
        EVENT_MUTE_ENEMY_FULL = 0x100,
        EVENT_MUTE_OBJ_PARTIAL = 0x200,
        EVENT_MUTE_OBJ_FULL = 0x400,
        EVENT_0x800 = 0x800,
        EVENT_0x400000 = 0x400000,
    };

    dSndStateMgr_c();

    void setup(EGG::Heap *pHeap);
    void calc();
    void restoreEffects();

    static bool isInStage(const char *stageName);

    s32 getField_0x14() const {
        return field_0x014;
    }

    s32 getStageId_0x040() const {
        return mStageId;
    }

    s32 getStageId_0x044() const {
        return field_0x044;
    }

    s32 getLayer() const {
        return mLayer;
    }

    u32 getFrameCounter() const {
        return mFrameCounter;
    }

    f32 getFxSend3D() const {
        return mFxSend3D;
    }

    void resetEventName() {
        mEventName = "EVENT_NONE";
    }

    const char *getCameraFmt() {
        return "%s_C%d_%d";
    }

    const char *getMsgWaitFmt() {
        return "%s_M%d_W%d_%d";
    }

    const char *getFrameFmt() {
        return "%s_%d";
    }

    bool checkFlag0x18(u32 mask);

    bool checkFlag0x258(u32 mask) {
        return field_0x258 & mask;
    }

    void resetFlag0x258() {
        field_0x258 = 0;
    }

    void onFlag0x258(u32 mask) {
        field_0x258 |= mask;
    }

    void offFlag0x258(u32 mask) {
        field_0x258 &= ~mask;
    }

    bool checkStageTypeFlag(u32 mask) {
        return mStageTypeFlags & mask;
    }

    bool checkFlag0x10(u32 mask) {
        return field_0x010 & mask;
    }

    void onFlag0x10(u32 mask) {
        field_0x010 |= mask;
    }

    void offFlag0x10(u32 mask) {
        field_0x010 &= ~mask;
    }

    bool checkEventFlag(u32 mask) {
        return mEventFlags & mask;
    }

    void onEventFlag(u32 mask) {
        mEventFlags |= mask;
    }

    void offEventFlag(u32 mask) {
        mEventFlags &= ~mask;
    }

    void setFlowEvent(u32 eventId);

    const char *getCurrentStageMusicDemoName() const;
    bool isInDemo() const {
        // implicit conversion required for regalloc in loadStageSound
        return getCurrentStageMusicDemoName();
    }

    void setStbEventName(const char *eventName);
    void setEvent(const char *eventName);
    // not sure, subtype is unused
    bool isActiveDemoMaybe(s32 subtype) const;
    bool isInEvent(const char *eventName);
    bool isInEvent();
    bool onSkipEvent() const;

    void onMsgStart(s32 idx);
    void onMsgEnd();

    void onMsgWaitStart();
    void onMsgWaitEnd();

    void onLinkDie();

    f32 getUserParamVolume(u32 userParam);

    void onStageOrLayerUpdate();
    void onStageLoad();
    void onRestartScene(s32 fadeFrames);
    void onGotoStage(s32 fadeFrames);
    void loadStageSound();
    void loadObjectSound();

    // TODO better names
    static const char *getStageName(s32 id);
    const char *getStageName4(s32 id);
    const char *getCurrentStageName4();

    static s32 getSndStageId(const char *stageName, s32 layer);
    static s32 getSndStageId3(const char *stageName, s32 layer);
    static s32 getSndStageId4(const char *stageName, s32 layer);

    static s32 getSndStageId2(s32 id);
    static s32 getSndStageId4(s32 id);

    static s32 getNextSndStageId(s32 id);
    static bool specialLayerVersionExists(const char *stageName, s32 layer);

    enum SoundIdLookup_e {
        LOOKUP_BGM,
        LOOKUP_BGM_MAIN,
        LOOKUP_SE_A,
    };

    u32 getSoundIdForStageAndLayer(SoundIdLookup_e lookup, const char *stageName, s32 layer, s32 stageId);

    void setFiltersIfUnderwater();
    void setBgmLpfAndFxSendIfUnderwater();
    void resetLpfAndFxSend();
    void resetBgmLpfAndFxSend();
    void setBgmAndStageEffectLpf();
    void setBgmLpf();
    void setBgmLpf(s32 fadeFrames);
    void resetBgmAndStageEffectLpf();
    void resetBgmLpf();

    void onCameraCut(s32 cutIdx);

    static void clearEventExecuteCallback();

private:
    void loadStageSound(bool force);

    void resetOverrides();
    void initializeEventCallbacks(const char *name);
    bool handleGlobalEvent(const char *name);
    bool handleStageEvent(const char *name);
    void handleDemoEvent(const char *name);

    void calcEvent();
    void calcRoomId();
    void setRoomId(s32 roomId);
    void calcTgSnd();
    void calcFilters();
    void calcFxSend3D();
    void setFxSend3DTarget();

    u32 getBgmLabelSoundId();
    void doLabelSuffix(const char *suffix);

    u32 convertSeLabelToSoundId(const char *label);
    u32 convertBgmLabelToSoundId(const char *label);
    u32 convertCmdLabelToSoundId(const char *label);

    void doBgm(const char *label);
    void doSe(const char *label);
    void doCmd(const char *label);

    u32 getSeCameraId();
    u32 getSeMsgWaitId();
    u32 getSeFrameCountId();

    u32 getBgmCameraId();
    u32 getBgmMsgWaitId();
    u32 getBgmFrameCountId();

    u32 getCmdCameraId();
    u32 getCmdMsgWaitId();
    u32 getCmdFrameCountId();

    void calcSe();
    bool calcBgm();
    void calcCmd();

    bool playSe(u32 soundId);
    bool playFanOrBgm(u32 soundId);
    bool playCmd(u32 soundId);

    void handleFan();
    void handleSe();
    void handleCmd();
    void handleSeLv();

    /** Called when no event is running anymore */
    void endEvent(bool skipped);
    /** Called when an event stops */
    bool finalizeEvent(bool skipped);

    void resetEventVars();

    // Callbacks start
    static void cbUnkNoop();

    // Callbacks end

    u32 getStageTypeFlags(const char *stageName) const;
    // ET, FS, or corresponding Sky Keep rooms
    static bool isVolcanicDungeon(u32 stageId);
    // Checks if the given stage + layer is the stage you're transported
    // to when viewing hint movies.
    static bool isSeekerStoneStage(const char *stageName, s32 layer);
    bool isSomeSkyloftRoom() const;

    void setCallbacksForStage();

    u32 getStageId(const char *name, s32 layer);
    static u32 getStageUnk2(u32 stageId);

    static SndEventCallback sEventExecuteCallback;

    /* 0x010 */ u32 field_0x010;
    /* 0x014 */ UNKWORD field_0x014;
    /* 0x018 */ UNKWORD field_0x018;
    /* 0x01C */ SizedString<32> mStageName;
    /* 0x03C */ s32 field_0x03C;
    /* 0x040 */ s32 mStageId;
    /* 0x044 */ UNKWORD field_0x044;
    /* 0x048 */ u8 _0x048[0x050 - 0x048];
    /* 0x050 */ s32 mPreviousStageId;
    /* 0x054 */ u32 mStageTypeFlags;
    /* 0x058 */ s32 mLayer;
    /* 0x05C */ s32 mRoomId;
    /* 0x060 */ UNKWORD field_0x060;
    /* 0x064 */ u8 field_0x064;
    /* 0x065 */ bool field_0x065;
    /* 0x066 */ bool mHasChangedTgSndAreaFlags;
    /* 0x067 */ bool mHasChangedTgSndAreaMgFlags;
    /* 0x068 */ u32 mSavedTgSndAreaFlags;
    /* 0x06C */ u32 mSavedTgSndAreaMgFlags;
    /* 0x070 */ void (*mpUnkCallback)();
    /* 0x074 */ UNKWORD field_0x074;
    /* 0x078 */ OnEventStartCallback mpOnEventStartCallback;
    /* 0x07C */ UNKWORD field_0x07C;
    /* 0x080 */ OnFlagsChangeCallback mpTgSndAreaFlagsChangeCallback;
    /* 0x084 */ OnFlagsChangeCallback mpTgSndAreaMgFlagsChangeCallback;
    /* 0x088 */ UNKWORD field_0x088;
    /* 0x08C */ s32 mSoundEventId;
    /* 0x090 */ s32 mCameraCutCounter;
    /* 0x094 */ u32 mEventFlags;
    /* 0x098 */ SizedString<64> mEventName;
    /* 0x0D8 */ SizedString<64> mPrevEventName;
    /* 0x118 */ const char *mpStbEventName;
    /* 0x11C */ u32 mFrameCounter;
    /* 0x120 */ u32 mCameraCutFrameCounter;
    /* 0x124 */ u32 mMsgFrameCounter;
    /* 0x128 */ u32 mSeLvSoundId;
    /* 0x12C */ SizedString<64> mSeName;
    /* 0x16C */ SizedString<64> mBgmName;
    /* 0x1AC */ SizedString<64> mFanName;
    /* 0x1EC */ SizedString<64> mCmdName;
    /* 0x22C */ UNKWORD field_0x22C;
    /* 0x230 */ nw4r::snd::SoundHandle mSeLvSoundHandle;
    /* 0x234 */ const SndEventDef *mpSoundEventDef;
    /* 0x238 */ u32 field_0x238;
    /* 0x23C */ u8 field_0x23C;
    /* 0x23D */ u8 field_0x23D;
    /* 0x240 */ UNKWORD field_0x240;
    /* 0x244 */ UNKWORD field_0x244;
    /* 0x248 */ UNKWORD field_0x248;
    /* 0x24C */ s32 mMsgCounter;
    /* 0x250 */ s32 mMsgWaitSelectCounter;
    /* 0x254 */ u8 field_0x254;
    /* 0x258 */ u32 field_0x258;
    /* 0x25C */ nw4r::snd::FxReverbStdDpl2 mFx;
    /* 0x48C */ UNKWORD field_0x48C;
    /* 0x490 */ f32 mFxSend3DDefault;
    /* 0x494 */ f32 mFxSend3DOverride;
    /* 0x498 */ f32 mFxSend3DNext;
    /* 0x49C */ f32 mFxSend3D;
    /* 0x4A0 */ f32 mFxSend3DTarget;
    /* 0x4A4 */ u32 field_0x4A4;
    /* 0x4A8 */ u8 field_0x4A8;
    /* 0x4A9 */ bool mNeedsGroupsReload;
};

#endif
