#ifndef D_SND_BGM_SEQ_CONFIG_H
#define D_SND_BGM_SEQ_CONFIG_H

#include "common.h"

struct dSndBgmSeqConfig {
    /* 0x00 */ u32 soundId;
    /* 0x04 */ s32 field_0x04;
    /* 0x08 */ s32 field_0x08;
    /* 0x0C */ u16 field_0x0C;
    /* 0x0E */ u16 field_0x0E;


    static const dSndBgmSeqConfig *getConfig(u32 soundId, s32 unkParam);
};

struct dSndBgmBattleConfig_MuteUnmuteMasks {
    /* 0x00 */ u16 unmuteMask;
    /* 0x02 */ u16 muteMask;
};

struct dSndBgmBattleConfig {
    /* 0x00 */ u32 soundId;
    /* 0x04 */ dSndBgmBattleConfig_MuteUnmuteMasks mTrackMasks[3];

    static bool hasConfig(u32 soundId);
    static const dSndBgmBattleConfig *getConfig(u32 soundId);
};

#endif
