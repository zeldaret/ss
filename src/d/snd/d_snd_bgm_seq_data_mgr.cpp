#include "d/snd/d_snd_bgm_seq_data_mgr.h"
#include "common.h"
#include "d/snd/d_snd_bgm_mgr.h"
#include "d/snd/d_snd_bgm_mml_parsers.h"
#include "d/snd/d_snd_bgm_sound_harp_mgr.h"

#include "d/snd/d_snd_mgr.h"
#include "d/snd/d_snd_player_mgr.h"
#include "d/snd/d_snd_util.h"
#include "d/snd/d_snd_wzsound.h"

SND_DISPOSER_DEFINE(dSndBgmSeqDataMgr_c)

dSndBgmSeqDataMgr_c::dSndBgmSeqDataMgr_c() : mInitialized(false) {
    mpMgrs = new dSndBgmSoundHarpMgr_c[NUM_SOUNDS];
}

const char *dSndBgmSeqDataMgr_c::getDataPrefix() {
    return "DATA_";
}

u32 dSndBgmSeqDataMgr_c::getDataPrefixLength() {
    return std::strlen(getDataPrefix());
}

void dSndBgmSeqDataMgr_c::initialize() {
    // no-op
}

void dSndBgmSeqDataMgr_c::setupState0() {
    if (mInitialized) {
        return;
    }

    // DATA_BGM_BATTLE2, DATA_BGM_BATTLE2_OUTRO
    for (int i = 0; i < NUM_SOUNDS; i++) {
        u32 dataId = DATA_BGM_BATTLE2 + i;
        const char *baseLabel = dSndMgr_c::getSoundLabelString(dataId) + getDataPrefixLength();
        u32 baseSoundId = dSndPlayerMgr_c::GetInstance()->convertLabelStringToSoundId(baseLabel);
        dSndBgmMmlParserHarp_c parser(&mpMgrs[i]);
        (void)dSndBgmMgr_c::isSoundPlayedByBgmBattlePlayer(dataId);
        parser.parseData(baseSoundId, (1 << 15) | (1 << 0));
    }

    mInitialized = true;
}

dSndBgmSoundHarpMgr_c *dSndBgmSeqDataMgr_c::getHarpMgrForSoundId(u32 soundId) {
    if (soundId == -1) {
        return nullptr;
    }
    
    for (dSndBgmSoundHarpMgr_c *it = &mpMgrs[0]; it < &mpMgrs[NUM_SOUNDS]; it++) {
        if (it->getSoundId() == soundId) {
            return it;
        }
    }

    return nullptr;
}
