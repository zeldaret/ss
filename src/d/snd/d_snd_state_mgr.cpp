#include "d/snd/d_snd_state_mgr.h"

#include "common.h"
#include "d/d_sc_game.h"
#include "d/snd/d_snd_player_mgr.h"
#include "d/snd/d_snd_stage_data.h"
#include "egg/core/eggHeap.h"
#include "nw4r/snd/snd_FxReverbStdDpl2.h"

#include <cstring>

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
      field_0x08C(0x89),
      field_0x090(0),
      field_0x094(0),
      field_0x118(nullptr),
      field_0x11C(0),
      field_0x120(0),
      field_0x124(0),
      field_0x128(-1),
      field_0x22C(0),
      field_0x230(0),
      field_0x234(nullptr),
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
    if (dSndPlayerMgr_c::GetInstance()->checkFlag(dSndPlayerMgr_c::MGR_UNK_0x80)) {
        dSndPlayerMgr_c::GetInstance()->stopAllSound();
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
        case SND_STAGE_D003_1:
            return true;
        default:
            return false;
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
