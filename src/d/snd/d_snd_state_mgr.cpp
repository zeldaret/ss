#include "d/snd/d_snd_state_mgr.h"

#include "common.h"
#include "d/d_sc_game.h"
#include "d/snd/d_snd_event.h"
#include "d/snd/d_snd_player_mgr.h"
#include "d/snd/d_snd_stage_data.h"
#include "d/snd/d_snd_util.h"
#include "egg/core/eggHeap.h"
#include "nw4r/snd/snd_FxReverbStdDpl2.h"
#include "sized_string.h"

#include <cstring>

SndEventCallback dSndStateMgr_c::sEventExecuteCallback;
SND_DISPOSER_DEFINE(dSndStateMgr_c);

dSndStateMgr_c::dSndStateMgr_c()
    : field_0x010(0),
      field_0x014(0),
      field_0x018(0),
      field_0x03C(0),
      mStageId(0xAC),
      mPreviousStageId(0xAC),
      field_0x054(0),
      field_0x058(0),
      mLayer(-1),
      field_0x060(0),
      field_0x064(0),
      field_0x065(false),
      field_0x066(0),
      field_0x067(0),
      field_0x068(-1),
      field_0x06C(-1),
      field_0x070(0),
      field_0x074(0),
      field_0x078(0),
      field_0x07C(0),
      field_0x080(0),
      field_0x084(0),
      field_0x088(0),
      mSoundEventId(SND_EVENT_0x89),
      field_0x090(0),
      field_0x094(0),
      field_0x118(nullptr),
      field_0x11C(0),
      field_0x120(0),
      field_0x124(0),
      field_0x128(-1),
      field_0x22C(0),
      field_0x230(0),
      mpSoundEventDef(nullptr),
      field_0x238(-1),
      field_0x23C(0),
      field_0x23D(0),
      field_0x240(0),
      field_0x244(0),
      field_0x248(0),
      field_0x24C(-1),
      field_0x250(0),
      field_0x254(0),
      field_0x258(0),
      field_0x48C(0),
      field_0x490(0.02f),
      field_0x494(-1.0f),
      field_0x498(-1.0f),
      field_0x49C(0.02f),
      field_0x4A0(0.02f),
      field_0x4A4(-1),
      field_0x4A8(0),
      needsGroupsReload(false) {}

extern "C" void fn_803665B0(nw4r::snd::detail::FxReverbStdParam &, int);

void dSndStateMgr_c::setup(EGG::Heap *pHeap) {
    u32 max = 0;
    for (int i = 0; i < 5; i++) {
        nw4r::snd::detail::FxReverbStdParam param;
        fn_803665B0(param, i);
        mFx.SetParam(param);
        u32 thisSize = mFx.GetRequiredMemSize();
        if (max < thisSize) {
            max = thisSize;
        }
    }
    void *mem = pHeap->alloc(max, 4);
    mFx.AssignWorkBuffer(mem, max);
    resetStageName();
}

void dSndStateMgr_c::onStageOrLayerUpdate() {
    if (dSndPlayerMgr_c::GetInstance()->checkFlag(dSndPlayerMgr_c::MGR_CAUTION)) {
        dSndPlayerMgr_c::GetInstance()->leaveCaution();
    }

    bool unk_0x065 = field_0x065;

    onFlag0x10(0x2);
    field_0x064 = 0;

    if (!streq(field_0x098, dScGame_c::currentSpawnInfo.getStageName())) {
        needsGroupsReload = true;
        field_0x01C = dScGame_c::currentSpawnInfo.getStageName();
    }

    if (!needsGroupsReload && dScGame_c::currentSpawnInfo.layer != mLayer) {
        needsGroupsReload = true;
    }

    mLayer = dScGame_c::currentSpawnInfo.layer;
    u32 newStageId = getStageId(dScGame_c::currentSpawnInfo.getStageName(), dScGame_c::currentSpawnInfo.layer);
    mPreviousStageId = mStageId;
    field_0x044 = getStageUnk2(newStageId);
    mStageId = newStageId;

    if (unk_0x065) {
        setCallbacksForStage();
    }
}

inline bool checkStagePrefix(const char *stageName, const char *prefix) {
    return !std::strncmp(prefix, stageName, 2);
}

u32 dSndStateMgr_c::getStageTypeFlags(const char *stageName) const {
    u32 flags = 0;
    if (checkStagePrefix(stageName, "F0")) {
        flags |= STAGE_SKY | STAGE_FIELD;
    } else if (checkStagePrefix(stageName, "D0")) {
        flags |= STAGE_SKY | STAGE_DUNGEON;
    } else if (checkStagePrefix(stageName, "B0")) {
        flags |= STAGE_SKY | STAGE_BOSS;
    } else if (checkStagePrefix(stageName, "S0")) {
        flags |= STAGE_SKY | STAGE_SILENT_REALM;
    } else if (checkStagePrefix(stageName, "F1")) {
        flags |= STAGE_FOREST | STAGE_FIELD;
    } else if (checkStagePrefix(stageName, "D1")) {
        flags |= STAGE_FOREST | STAGE_DUNGEON;
    } else if (checkStagePrefix(stageName, "B1")) {
        flags |= STAGE_FOREST | STAGE_BOSS;
    } else if (checkStagePrefix(stageName, "S1")) {
        flags |= STAGE_FOREST | STAGE_SILENT_REALM;
    } else if (checkStagePrefix(stageName, "F2")) {
        flags |= STAGE_MOUNTAIN | STAGE_FIELD;
    } else if (checkStagePrefix(stageName, "D2")) {
        flags |= STAGE_MOUNTAIN | STAGE_DUNGEON;
    } else if (checkStagePrefix(stageName, "B2")) {
        flags |= STAGE_MOUNTAIN | STAGE_BOSS;
    } else if (checkStagePrefix(stageName, "S2")) {
        flags |= STAGE_MOUNTAIN | STAGE_SILENT_REALM;
    } else if (checkStagePrefix(stageName, "F3")) {
        flags |= STAGE_DESERT | STAGE_FIELD;
    } else if (checkStagePrefix(stageName, "D3")) {
        flags |= STAGE_DESERT | STAGE_DUNGEON;
    } else if (checkStagePrefix(stageName, "B3")) {
        flags |= STAGE_DESERT | STAGE_BOSS;
    } else if (checkStagePrefix(stageName, "S3")) {
        flags |= STAGE_DESERT | STAGE_SILENT_REALM;
    } else if (checkStagePrefix(stageName, "F4")) {
        flags |= STAGE_SILENT_GROUNDS | STAGE_FIELD;
    } else if (checkStagePrefix(stageName, "D4")) {
        flags |= STAGE_SILENT_GROUNDS | STAGE_DUNGEON;
    } else if (checkStagePrefix(stageName, "B4")) {
        flags |= STAGE_SILENT_GROUNDS | STAGE_BOSS;
    }

    if ((flags & STAGE_SKY) != 0 && std::strstr(stageName, "r") != nullptr) {
        flags |= STAGE_ROOM;
    }

    return flags;
}

bool dSndStateMgr_c::isVolcanicDungeon(u32 stageId) {
    u32 base = getStageUnk2(stageId);
    switch (base) {
        case SND_STAGE_D200:
        case SND_STAGE_D201:
        case SND_STAGE_D201_1:
        case SND_STAGE_D003_0:
        case SND_STAGE_D003_1: return true;
        default:               return false;
    }
}

bool dSndStateMgr_c::isSeekerStoneStage(const char *stageName, s32 layer) {
    if (streq(stageName, "F000") && layer == 22) {
        // Skyloft hint stone
        return true;
    }

    if (streq(stageName, "F202") && layer == 10) {
        // Boko base hint stone
        return true;
    }

    return false;
}

bool dSndStateMgr_c::isInStage(const char *stageName) {
    return !std::strcmp(dScGame_c::currentSpawnInfo.getStageName(), stageName);
}

void dSndStateMgr_c::initializeEventCallbacks(const char *name) {
    if (name == nullptr) {
        return;
    }

    mpSoundEventDef = nullptr;
    sEventExecuteCallback = nullptr;
    resetOverrides();

    if (checkFlag0x94(0x2)) {
        handleDemoEvent(name);
        return;
    }

    if (mSoundEventId == SND_EVENT_0x87) {
        return;
    }

    if (!handleStageEvent(name)) {
        handleGlobalEvent(name);
    }

    if (mSoundEventId == SND_EVENT_0x89) {
        if (strstr(name, "Intro")) {
            mSoundEventId = SND_EVENT_0x88;
        } else {
            mSoundEventId = SND_EVENT_0x86;
            cbUnkNoop();
        }
    }
}

const char *dSndStateMgr_c::getStageName(s32 id) {
    if (id >= SND_STAGE_MAX) {
        return "NULL";
    }
    return dSndStageInfo::sInfos[id].stageName;
}

const char *dSndStateMgr_c::getStageName4(s32 id) {
    if (id >= SND_STAGE_MAX) {
        return "NULL";
    }
    return dSndStageInfo::sInfos[getSndStageId4(id)].stageName;
}

const char *dSndStateMgr_c::getCurrentStageName4() {
    return getStageName(dSndStageInfo::sInfos[mStageId].unk4);
}

s32 dSndStateMgr_c::getSndStageId(const char *stageName, s32 layer) {
    if (stageName != nullptr) {
        for (s32 idx = 0; idx < dSndStageInfo::sNumStageInfos; idx++) {
            if (streq(stageName, dSndStageInfo::sInfos[idx].stageName)) {
                if (dSndStageInfo::sInfos[idx].layer == -1) {
                    return dSndStageInfo::sInfos[idx].unk1;
                }
                if (dSndStageInfo::sInfos[idx].layer == layer) {
                    return dSndStageInfo::sInfos[idx].unk1;
                }
            }
        }
    }

    return SND_STAGE_MAX;
}

s32 dSndStateMgr_c::getSndStageId3(const char *stageName, s32 layer) {
    if (stageName != nullptr) {
        for (s32 idx = 0; idx < dSndStageInfo::sNumStageInfos; idx++) {
            if (streq(stageName, dSndStageInfo::sInfos[idx].stageName)) {
                if (dSndStageInfo::sInfos[idx].layer == -1 || dSndStageInfo::sInfos[idx].layer == layer) {
                    return dSndStageInfo::sInfos[idx].unk3;
                }
            }
        }
    }

    return SND_STAGE_MAX;
}

s32 dSndStateMgr_c::getSndStageId4(const char *stageName, s32 layer) {
    if (stageName != nullptr) {
        for (s32 idx = 0; idx < dSndStageInfo::sNumStageInfos; idx++) {
            if (streq(stageName, dSndStageInfo::sInfos[idx].stageName)) {
                if (dSndStageInfo::sInfos[idx].layer == -1 || dSndStageInfo::sInfos[idx].layer == layer) {
                    return dSndStageInfo::sInfos[idx].unk4;
                }
            }
        }
    }

    return SND_STAGE_MAX;
}

s32 dSndStateMgr_c::getSndStageId2(s32 id) {
    if (id >= SND_STAGE_MAX) {
        return SND_STAGE_MAX;
    }
    return dSndStageInfo::sInfos[id].unk2;
}

s32 dSndStateMgr_c::getSndStageId4(s32 id) {
    if (id >= SND_STAGE_MAX) {
        return SND_STAGE_MAX;
    }
    return dSndStageInfo::sInfos[id].unk4;
}

s32 dSndStateMgr_c::getNextSndStageId(s32 id) {
    return getSndStageId(dScGame_c::nextSpawnInfo.getStageName(), dScGame_c::nextSpawnInfo.layer);
}

bool dSndStateMgr_c::specialLayerVersionExists(const char *stageName, s32 layer) {
    if (stageName != nullptr && layer > 0) {
        return getSndStageId(stageName, layer) != getSndStageId(stageName, 0);
    }

    return false;
}

u32 dSndStateMgr_c::getSoundIdForStageAndLayer(SoundIdLookup_e lookup, const char *stageName, s32 layer, s32 stageId) {
    // Ternary causes regswaps...
    const char *prefix = "BGM_";
    if (lookup == LOOKUP_SE_A) {
        prefix = "SE_A_";
    }
    SizedString<64> label;

    u32 soundId = -1;
    s32 round = 3;

    bool hasSpecialLayerVersion = false;
    if (layer > 0) {
        hasSpecialLayerVersion = specialLayerVersionExists(stageName, layer);
    }

    if (hasSpecialLayerVersion) {
        label.sprintf("%s%s_L%d", prefix, stageName, layer);
    } else if (layer > 0) {
        label.sprintf("%s%s_L%d", prefix, stageName, layer);
    } else {
        label.sprintf("%s%s", prefix, stageName);
        // We didn't bother with a _L layer variant, so we're going
        // in with the base variant
        round = 2;
    }

    while (round > 0) {
        if (lookup == LOOKUP_BGM_MAIN) {
            label += "_MAIN";
        }

        soundId = dSndPlayerMgr_c::GetInstance()->convertLabelStringToSoundId(label);
        if (soundId != -1) {
            break;
        }

        if (round == 3) {
            // If _L layer variant didn't exist, try the one without a layer...
            label.sprintf("%s%s", prefix, stageName);
        } else if (round == 2) {
            if (lookup == LOOKUP_BGM) {
                // If the one without a layer doesn't exist, continue with the one referenced
                // in field 3, unless it's the same as the original one
                s32 otherStageId = getSndStageId3(stageName, layer);
                if (stageId != otherStageId) {
                    label.sprintf("%s%s", prefix, getStageName(otherStageId));
                } else {
                    // break to prevent infinite loop
                    round = 0;
                }
            } else if (lookup == LOOKUP_SE_A) {
                // If the one without a layer doesn't exist, continue with the one referenced
                // in field 4, unless it's the same as the original one
                s32 otherStageId = getSndStageId4(stageName, layer);
                if (stageId != otherStageId) {
                    label.sprintf("%s%s", prefix, getStageName(otherStageId));
                } else {
                    // break to prevent infinite loop
                    break;
                }
            } else {
                break;
            }
        }

        round--;
    }

    return soundId;
}
