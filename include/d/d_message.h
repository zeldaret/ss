#ifndef D_MESSAGE_H
#define D_MESSAGE_H

#include "common.h"
#include "d/d_base.h"
#include "d/d_tag_processor.h"
#include "libms/flowfile.h"
#include "libms/msgfile.h"
#include "sized_string.h"

class dFlowBase_c {
protected:
    enum Type_e {
        FLOW_NONE,
        FLOW_MESSAGE,
        FLOW_BRANCH,
        FLOW_EVENT,
        FLOW_ENTRY,
        FLOW_JUMP,
    };

public:
    virtual ~dFlowBase_c() {}

    /* vt 0x0C */ virtual void triggerEntryPoint(s32, s32) = 0;
    /* vt 0x10 */ virtual void triggerEntryPoint(const char *) = 0;
    /* vt 0x14 */ virtual void advanceFlow() = 0;
    /* vt 0x18 */ virtual bool vt_0x18() const = 0;
    /* vt 0x1C */ virtual bool handleEventInternal(const MsbFlowInfo *element) = 0;
    /* vt 0x20 */ virtual bool handleEvent() = 0;
    /* vt 0x24 */ virtual bool handleMessage() = 0;
    /* vt 0x28 */ virtual bool handleBranch() = 0;
    /* vt 0x2C */ virtual bool handleEntry() = 0;
    /* vt 0x30 */ virtual bool handleJump() = 0;
    /* vt 0x34 */ virtual void vt_0x34() {}
};

// Ghidra: ActorEventFlowManager
class dFlow_c : dFlowBase_c {
protected:
    enum BranchValue_e {
        BRANCH_SELECTED_OPTION_0,
        BRANCH_SELECTED_OPTION_1,
        BRANCH_SELECTED_OPTION_FI,
        BRANCH_STORYFLAG,
        BRANCH_NONE,
        BRANCH_ZONEFLAG,
        BRANCH_SCENEFLAG,
        BRANCH_EVENT_COUNTER_THRESHOLD_1,
        BRANCH_EVENT_COUNTER_THRESHOLD_2,
        BRANCH_TEMPFLAG,
        BRANCH_CURRENT_RUPEES,
        BRANCH_RAND_2,
        BRANCH_RAND_3,
        BRANCH_RAND_4,
        BRANCH_NPC_1, // handled by subclass
        BRANCH_NPC_2, // handled by subclass
        BRANCH_NPC_3, // handled by subclass
        BRANCH_FREE_SPACE_IN_POUCH,
        BRANCH_18,
        BRANCH_19,
        BRANCH_FREE_SPACE_IN_ITEM_CHECK,
        BRANCH_TARGET_ACTOR_HAS_KILL_COUNT,
        BRANCH_22,
    };

    enum Event_e {
        EVENT_SET_STORYFLAG = 0,
        EVENT_UNSET_STORYFLAG = 1,
        EVENT_SET_SCENEFLAG = 2,
        EVENT_UNSET_SCENEFLAG = 3,
        EVENT_SET_ZONEFLAG = 4,
        EVENT_UNSET_ZONEFLAG = 5,
        EVENT_DELAY = 6,
        EVENT_LOAD_FI_FLOW = 7,
        EVENT_RUPEES = 8,
        EVENT_SET_ITEM = 9,
        EVENT_EXIT = 10,

        EVENT_12 = 12,

        EVENT_COUNTER_THRESHOLD = 23,
        EVENT_PLAY_SOUND = 24,
        EVENT_ADD_ITEM = 25,

        EVENT_SET_TEMPFLAG = 28,
        EVENT_UNSET_TEMPFLAG = 29,

        EVENT_START_MAP_EVENT = 30,

        EVENT_END_MAP_EVENT = 34,

        EVENT_SET_STORYFLAG_217 = 37,
        EVENT_DEMO_METER_ITEM_SELECT = 38,
        EVENT_CAMERA_42 = 42,
        EVENT_LYT_MINI_GAME = 44,
        EVENT_LYT_MINI_GAME_END = 45,
        EVENT_46 = 46,

        EVENT_RESET_STORYFLAG = 52,
        EVENT_SET_ITEMFLAG = 53,
        EVENT_PALETTE = 54,
        EVENT_DEMO_DOWSING = 55,
        EVENT_DEMO_METER_DOWSING = 56,
        EVENT_DEMO_METER_MINUS_BTN = 57,
        EVENT_SELECT_STORY_DOWSING = 58,
        EVENT_DEMO_COLLECTION_SCREEN = 59,
    };

public:
    dFlow_c();

    virtual ~dFlow_c();

    typedef u16 (dFlow_c::*BranchHandler)(const MsbFlowInfo *element) const;

    /* vt 0x0C */ virtual void triggerEntryPoint(s32 labelPart1, s32 labelPart2) override;
    /* vt 0x10 */ virtual void triggerEntryPoint(const char *) override;
    /* vt 0x14 */ virtual void advanceFlow() override;
    /* vt 0x18 */ virtual bool vt_0x18() const override;
    /* vt 0x1C */ virtual bool handleEventInternal(const MsbFlowInfo *element) override;
    /* vt 0x20 */ virtual bool handleEvent() override;
    /* vt 0x24 */ virtual bool handleMessage() override;
    /* vt 0x28 */ virtual bool handleBranch() override;
    /* vt 0x2C */ virtual bool handleEntry() override;
    /* vt 0x30 */ virtual bool handleJump() override;
    /* vt 0x38 */ virtual bool vt_0x38() const {
        return false;
    }
    /* vt 0x3C */ virtual u16 getSwitchChoice(const MsbFlowInfo *element, u16 param) const;
    /* vt 0x40 */ virtual bool triggerEntryPointChecked(s32 labelPart1, s32 labelPart2);

    /**
     * "Speculatively" evaluate a flow until a given event gets hit, and write
     * its parameters into the provided pointer. Probably has no side effects.
     */
    bool advanceUntilEvent(s32 searchParam3, s32 *pOutParams1n2);
    bool advanceUntil(s32 searchType, s32 searchParam3, s32 *pOutParams1n2);

    static void playSound(u32);

    u16 getField_0x44() const;
    u16 getNextFiFlow() const;

protected:
    static s32 sExitId;

    static BranchHandler sBranchHandlers[];
    u16 branchHandler00(const MsbFlowInfo *element) const;
    u16 branchHandler01(const MsbFlowInfo *element) const;
    u16 branchHandler02(const MsbFlowInfo *element) const;
    u16 branchHandler03(const MsbFlowInfo *element) const;
    u16 branchHandler04(const MsbFlowInfo *element) const;
    u16 branchHandler05(const MsbFlowInfo *element) const;
    u16 branchHandler06(const MsbFlowInfo *element) const;
    u16 branchHandler07(const MsbFlowInfo *element) const;
    u16 branchHandler08(const MsbFlowInfo *element) const;
    u16 branchHandler09(const MsbFlowInfo *element) const;
    u16 branchHandler10(const MsbFlowInfo *element) const;
    u16 branchHandler11(const MsbFlowInfo *element) const;
    u16 branchHandler12(const MsbFlowInfo *element) const;
    u16 branchHandler13(const MsbFlowInfo *element) const;
    u16 branchHandler14(const MsbFlowInfo *element) const;
    u16 branchHandler15(const MsbFlowInfo *element) const;
    u16 branchHandler16(const MsbFlowInfo *element) const;
    u16 branchHandler17(const MsbFlowInfo *element) const;
    u16 branchHandler18(const MsbFlowInfo *element) const;
    u16 branchHandler19(const MsbFlowInfo *element) const;
    u16 branchHandler20(const MsbFlowInfo *element) const;
    u16 branchHandler21(const MsbFlowInfo *element) const;
    u16 branchHandler22(const MsbFlowInfo *element) const;

    u16 findEntryPoint(u16 labelPart1, u16 labelPart2);
    u16 findEntryPoint(const char *label);

    void start(u16 entry);
    void clear();
    void setNext(u16 next);
    void setField0x3C();
    bool checkField0x3C() const;
    bool shouldHideKillCountForActor(s32 id) const;
    void createLytMiniGame();
    void clearMinigame();

    /* 0x04 */ MsbfInfo *mpMsbf;
    /* 0x08 */ s32 mCurrentFlowIndex;
    /* 0x0C */ u16 field_0x0C;
    /* 0x0E */ u8 field_0x0E;
    /* 0x0F */ u8 field_0x0F;
    /* 0x10 */ u8 field_0x10;
    /* 0x14 */ s32 field_0x14;
    /* 0x18 */ s32 mResultFromCounterCheck;
    /* 0x1C */ SizedString<32> mCurrentTextLabelName;
    /* 0x3C */ u8 field_0x3C;
    /* 0x40 */ s32 field_0x40;
    /* 0x44 */ u16 field_0x44;
    /* 0x46 */ u16 mNextFiFlow;
    /* 0x48 */ s32 mDelayTimer;
    /* 0x4C */ MsbFlowInfo mFlowInfo;
    /* 0x5C */ s32 mFiSpeechArgument;
    /* 0x60 */ s32 mFiInfo0;
};

class dMessage_c : public dBase_c {
public:
    dMessage_c();
    virtual ~dMessage_c() {
        sInstance = nullptr;
    }

    int create() override;
    int doDelete() override;
    int execute() override;
    int draw() override;

    static const char *getLanguageIdentifier();

    static dMessage_c *getInstance() {
        return sInstance;
    }

    static dTagProcessor_c *getGlobalTagProcessor() {
        return sTagProcessor;
    }

    void clearLightPillarRelatedArgs();
    void init();
    void reset();

    bool getField_0x329() const {
        return field_0x329;
    }

    s32 getField_0x2FC() const {
        return field_0x2FC;
    }

    void setField_0x2FC(s32 val) {
        field_0x2FC = val;
    }

    bool getInMapEvent() const {
        return mInMapEvent;
    }

    void setInMapEvent(bool val) {
        mInMapEvent = val;
    }

    s32 getMapEvent() const {
        return mMapEvent;
    }

    void setMapEvent(s32 val) {
        mMapEvent = val;
    }

    void setField_0x329(bool v) {
        field_0x329 = v;
    }

    void setField_0x32A(bool v) {
        field_0x32A = v;
    }

    bool getField_0x32A() const {
        return field_0x32A;
    }

    bool getField_0x330() const {
        return field_0x330;
    }

    void setField_0x330(bool v) {
        field_0x330 = v;
    }

    void setMiniGameVariant(s32 v) {
        mMiniGameVariant = v;
    }

    s32 getMiniGameVariant() const {
        return mMiniGameVariant;
    }

    void setField_0x344(s32 v) {
        field_0x344 = v;
    }

    u8 getField_0x340() const {
        return field_0x340;
    }

    void setField_0x340(u8 v) {
        field_0x340 = v;
    }

    static bool isValidTextLabel(const char *name);
    static void loadTextByLabel(const char *label, dTagProcessor_c *tagProcessor, bool, u32, u32);

    static s32 getMsbtIndexForLabel(const char *name);
    static const char *getMsbtFileName(s32 index);
    static MsbfInfo *getMsbfInfoForIndex(s32 index);
    static s32 getMsbtIndexForMsbfIndex(s32);
    static const char *getArcNameByIndex(s32 idx, bool);
    static MsbtInfo *getMsbtInfoForIndex(s32 index);

    static const wchar_t *getTextMessageByLabel(const char *label, bool global, wchar_t *dstBuf, u32 maxLen);
    static const wchar_t *
    getTextMessageByLabel(const char *label, dTagProcessor_c *pTagProcessor, bool global, wchar_t *dstBuf, u32 maxLen);

    static const wchar_t *formatText(const wchar_t *text);

    void storeLightPillarRelatedArg(u32);
    u32 getLightPillarRelatedArg(s32);

    s32 getCurrentTextFileNumber() const {
        return mCurrentTextFileNumber;
    }

    void setCurrentTextFileNumber(s32 num) {
        mCurrentTextFileNumber = num;
    }

private:
    static void *sZev0;
    static void *sZevStage;

    static void *getZev0Internal();
    static void *getZevStageInternal();

    void executeMinigame();

    const wchar_t *formatTextInternal(const wchar_t *text);
    const wchar_t *getTextMessageByLabelInternal(
        const char *label, dTagProcessor_c *pTagProcessor, bool global, wchar_t *dstBuf, u32 maxLen
    );

    static s32 getArcIndexForFile(const char *fileName);

    bool checkIsValidTextLabel(const char *name);

    static void setZevFromMsbArc();
    static void setStageZevFromMsbArc();
    static s32 getMsbtNumberByIndex(s32 index);
    static const char *getMsbfFileName(s32 index);
    static s32 getMsbfNumberByIndex(s32 index);
    static void *getDataFromMsbArc(s32 number, const char *fileName, bool);

    s32 getTextIndexForLabel(const char *label);
    s32 getMsbtIndexForLabelInternal(const char *label);
    MsbtInfo *getMsbtInfoForIndexInternal(s32 index);
    MsbfInfo *getMsbfInfoForIndexInternal(s32 index);

    static const char *getArcNameByIndexInternal(s32 idx, bool global);

    static dMessage_c *sInstance;
    static dTagProcessor_c *sTagProcessor;

    /* 0x068 */ MsbtInfo *mpMsgs[82];
    /* 0x1B0 */ MsbfInfo *mpFlows[80];
    /* 0x2F0 */ SizedString<8> mLanguage;
    /* 0x2F8 */ u32 mCurrentTextFileNumber;

    /* 0x2FC */ s32 field_0x2FC;

    /* 0x300 */ u32 field_0x300[10];

    /* 0x328 */ bool mInMapEvent;
    /* 0x329 */ bool field_0x329;
    /* 0x32A */ bool field_0x32A;

    /* 0x32B */ u8 field_0x32B;
    /* 0x32C */ s32 mMapEvent;
    /* 0x330 */ bool field_0x330;

    /* 0x334 */ u32 mMinigameResultPoints;
    /* 0x338 */ u32 mMinigameTime;
    /* 0x33C */ s32 mMiniGameVariant;
    /* 0x340 */ u8 field_0x340;
    /* 0x344 */ s32 field_0x344;
};

#endif
