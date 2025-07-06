#include "d/snd/d_snd_bgm_battle_data_mgr.h"
#include "d/snd/d_snd_bgm_sound_harp_mgr.h"

#include "d/snd/d_snd_util.h"

SND_DISPOSER_DEFINE(dSndBgmBattleDataMgr_c)

dSndBgmBattleDataMgr_c::dSndBgmBattleDataMgr_c() : field_0x14(0) {
    mpMgrs = new dSndBgmSoundHarpMgr_c[2];
}

const char *dSndBgmBattleDataMgr_c::getDataPrefix() {
    return "DATA_";
}

u32 dSndBgmBattleDataMgr_c::getDataPrefixLength() {
    return std::strlen(getDataPrefix());
}
