#ifndef D_SND_BGM_BATTLE_DATA_MGR_H
#define D_SND_BGM_BATTLE_DATA_MGR_H

#include "common.h"
#include "d/snd/d_snd_util.h"
#include "d/snd/d_snd_types.h"

SND_DISPOSER_FORWARD_DECL(dSndBgmBattleDataMgr_c);

// Previous Ghidra name: SomeUnusedSoundMgr
class dSndBgmBattleDataMgr_c {
public:
    SND_DISPOSER_MEMBERS(dSndBgmBattleDataMgr_c);

public:
    dSndBgmBattleDataMgr_c();

    static const char *getDataPrefix();
    static u32 getDataPrefixLength();

    dSndBgmSoundHarpMgr_c *getHarpMgrForSoundId(u32 soundId);

private:
    /* 0x10 */ dSndBgmSoundHarpMgr_c *mpMgrs;
    /* 0x14 */ u8 field_0x14;
};

#endif
