#include "d/snd/d_snd_bgm_seq_config.h"

#include "common.h"
#include "d/snd/d_snd_wzsound.h"

static const dSndBgmBattleConfig sBattleConfigs[] = {
    {         BGM_BATTLE2, {{(1 << 7) | (1 << 4) | (1 << 3), 0}, {(1 << 9) | (1 << 6), 0}, {0, 0}}},
    {BGM_BATTLE_ROOM_MAIN,                       {{(1 << 7) | (1 << 4), 0}, {(1 << 9), 0}, {0, 0}}},
    {     BGM_MIDDLE_BOSS,                                  {{(1 << 1), 0}, {(1 << 2), 0}, {0, 0}}}
};

const dSndBgmBattleConfig *dSndBgmBattleConfig::getConfig(u32 soundId) {
    for (int i = 0; i < ARRAY_LENGTH(sBattleConfigs); i++) {
        if (soundId == sBattleConfigs[i].soundId) {
            return &sBattleConfigs[i];
        }
    }
    return nullptr;
}

bool dSndBgmBattleConfig::hasConfig(u32 soundId) {
    if (soundId == -1) {
        return false;
    }

    for (int i = 0; i < ARRAY_LENGTH(sBattleConfigs); i++) {
        if (soundId == sBattleConfigs[i].soundId) {
            return true;
        }
    }
    return false;
}

static const dSndBgmSeqConfig sBgmConfigs[] = {
    {         BGM_BATTLE2, 0, 0x180, 1,    4},
    {         BGM_BATTLE2, 1, 0x180, 5, 0x2F},
    {BGM_BATTLE_ROOM_MAIN, 1, 0x180, 5, 0x2C},
    {            BGM_MUTE, 1, 0x120, 0, 0x1F},
};

const dSndBgmSeqConfig *dSndBgmSeqConfig::getConfig(u32 soundId, s32 unkParam) {
    if (soundId == -1) {
        return nullptr;
    }

    for (const dSndBgmSeqConfig *def = sBgmConfigs; def < sBgmConfigs + ARRAY_LENGTH(sBgmConfigs); def++) {
        if (soundId == def->soundId && def->field_0x04 == unkParam) {
            return def;
        }
    }
    return nullptr;
}
