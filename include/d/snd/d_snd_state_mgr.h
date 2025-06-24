#ifndef D_SND_STATE_MGR_H
#define D_SND_STATE_MGR_H

#include "common.h"
#include "d/snd/d_snd_util.h"
#include "nw4r/snd/snd_FxReverbStdDpl2.h"
#include "sized_string.h"

SND_DISPOSER_FORWARD_DECL(dSndStateMgr_c);

// Previous Ghidra name: EnemySoundMgr
class dSndStateMgr_c {
public:
    SND_DISPOSER_MEMBERS(dSndStateMgr_c);
    
public:
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

    dSndStateMgr_c();

    void setup(EGG::Heap *pHeap);
    void onStageOrLayerUpdate();
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

    s32 getField_0x058() const {
        return field_0x058;
    }

    u32 getField_0x11C() const {
        return field_0x11C;
    }

    f32 getField_0x49C() const {
        return field_0x49C;
    }

    void resetStageName() {
        field_0x098 = "EVENT_NONE";
    }

    bool checkFlag0x10(u32 mask) const {
        return field_0x010 & mask;
    }

    void onFlag0x10(u32 mask) {
        field_0x010 |= mask;
    }

    void offFlag0x10(u32 mask) {
        field_0x010 &= ~mask;
    }

    void setFlowEvent(u32 eventId);

    const char *getCurrentStageMusicDemoName() const;
    bool isInDemo() const {
        return getCurrentStageMusicDemoName() != nullptr;
    }
    bool isInEvent(const char *eventName);

private:
    u32 getStageTypeFlags(const char *stageName) const;
    // ET, FS, or corresponding Sky Keep rooms
    static bool isVolcanicDungeon(u32 stageId);
    // Checks if the given stage + layer is the stage you're transported
    // to when viewing hint movies.
    static bool isSeekerStoneStage(const char *stageName, s32 layer);
    
    void setCallbacksForStage();

    u32 getStageId(const char *name, s32 layer);
    static u32 getStageUnk2(u32 stageId);

    /* 0x010 */ u32 field_0x010;
    /* 0x014 */ UNKWORD field_0x014;
    /* 0x018 */ UNKWORD field_0x018;
    /* 0x01C */ SizedString<32> field_0x01C;
    /* 0x03C */ UNKWORD field_0x03C;
    /* 0x040 */ s32 mStageId;
    /* 0x044 */ UNKWORD field_0x044;
    /* 0x048 */ u8 _0x048[0x050 - 0x048];
    /* 0x050 */ s32 mPreviousStageId;
    /* 0x054 */ UNKWORD field_0x054;
    /* 0x058 */ UNKWORD field_0x058;
    /* 0x05C */ s32 mLayer;
    /* 0x060 */ UNKWORD field_0x060;
    /* 0x064 */ u8 field_0x064;
    /* 0x065 */ bool field_0x065;
    /* 0x066 */ u8 field_0x066;
    /* 0x067 */ u8 field_0x067;
    /* 0x068 */ UNKWORD field_0x068;
    /* 0x06C */ UNKWORD field_0x06C;
    /* 0x070 */ UNKWORD field_0x070;
    /* 0x074 */ UNKWORD field_0x074;
    /* 0x078 */ UNKWORD field_0x078;
    /* 0x07C */ UNKWORD field_0x07C;
    /* 0x080 */ UNKWORD field_0x080;
    /* 0x084 */ UNKWORD field_0x084;
    /* 0x088 */ UNKWORD field_0x088;
    /* 0x08C */ UNKWORD field_0x08C;
    /* 0x090 */ UNKWORD field_0x090;
    /* 0x094 */ u32 field_0x094;
    /* 0x098 */ SizedString<64> field_0x098;
    /* 0x0D8 */ SizedString<64> field_0x0D8;
    /* 0x118 */ const char *field_0x118;
    /* 0x11C */ u32 field_0x11C;
    /* 0x120 */ UNKWORD field_0x120;
    /* 0x124 */ UNKWORD field_0x124;
    /* 0x128 */ UNKWORD field_0x128;
    /* 0x12C */ SizedString<64> field_0x12C;
    /* 0x16C */ SizedString<64> field_0x16C;
    /* 0x1AC */ SizedString<64> field_0x1AC;
    /* 0x1EC */ SizedString<64> field_0x1EC;
    /* 0x22C */ UNKWORD field_0x22C;
    /* 0x230 */ UNKWORD field_0x230;
    /* 0x234 */ UNKWORD field_0x234;
    /* 0x238 */ UNKWORD field_0x238;
    /* 0x23C */ u8 field_0x23C;
    /* 0x23D */ u8 field_0x23D;
    /* 0x240 */ UNKWORD field_0x240;
    /* 0x244 */ UNKWORD field_0x244;
    /* 0x248 */ UNKWORD field_0x248;
    /* 0x24C */ UNKWORD field_0x24C;
    /* 0x250 */ UNKWORD field_0x250;
    /* 0x254 */ u8 field_0x254;
    /* 0x258 */ UNKWORD field_0x258;
    /* 0x25C */ nw4r::snd::FxReverbStdDpl2 mFx;
    /* 0x48C */ UNKWORD field_0x48C;
    /* 0x490 */ f32 field_0x490;
    /* 0x494 */ f32 field_0x494;
    /* 0x498 */ f32 field_0x498;
    /* 0x49C */ f32 field_0x49C;
    /* 0x4A0 */ f32 field_0x4A0;
    /* 0x4A4 */ UNKWORD field_0x4A4;
    /* 0x4A8 */ u8 field_0x4A8;
    /* 0x4A9 */ bool needsGroupsReload;
};

#endif
