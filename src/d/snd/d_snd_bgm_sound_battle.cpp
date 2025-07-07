#include "d/snd/d_snd_bgm_sound_battle.h"

#include "common.h"
#include "d/snd/d_snd_bgm_seq_config.h"
#include "d/snd/d_snd_bgm_sound.h"
#include "d/snd/d_snd_mgr.h"
#include "d/snd/d_snd_wzsound.h"

dSndBgmBattleSound_c::dSndBgmBattleSound_c()
    : mpBgmBattleConfig(nullptr), mMuteApplyStateMask(0), mTickRelated(0), field_0x190(0), field_0x191(1) {}

void dSndBgmBattleSound_c::cancel() {
    dSndBgmSound_c::cancel();
    mMuteApplyStateMask = 0;
    mTickRelated = 0;
    field_0x190 = 0;
    field_0x191 = 1;
}

void dSndBgmBattleSound_c::loadSeqConfig(u32 soundId) {
    if (dSndMgr_c::GetInstance()->getArchive()->GetSoundType(soundId) == nw4r::snd::SoundArchive::SOUND_TYPE_SEQ) {
        if (soundId == BGM_BATTLE2) {
            mpSeqConfig = dSndBgmSeqConfig::getConfig(soundId, 0);
        }

        if (mpSeqConfig == nullptr) {
            mpSeqConfig = dSndBgmSeqConfig::getConfig(soundId, 1);
        }
    }
}

void dSndBgmBattleSound_c::fadeIn(u32 id, s32 fadeFrames) {
    if (id == -1) {
        return;
    }

    // TODO - hmmm
    mpBgmBattleConfig = nullptr;
    mpBgmBattleConfig = dSndBgmBattleConfig::getConfig(GetId());

    dSndBgmSound_c::fadeIn(id, fadeFrames);

    if (GetId() != BGM_BATTLE2) {
        field_0x191 = 0;
    }
}
