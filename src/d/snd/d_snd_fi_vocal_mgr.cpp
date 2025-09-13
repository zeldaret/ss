#include "d/snd/d_snd_fi_vocal_mgr.h"

#include "d/snd/d_snd_harp_song_mgr.h"
#include "d/snd/d_snd_small_effect_mgr.h"
#include "d/snd/d_snd_util.h"
#include "d/snd/d_snd_wzsound.h"
#include "nw4r/snd/snd_SeqSoundHandle.h"
#include "nw4r/snd/snd_SoundHandle.h"

SND_DISPOSER_DEFINE(dSndFiVocalMgr_c);

dSndFiVocalMgr_c::dSndFiVocalMgr_c() {}

void dSndFiVocalMgr_c::executeOutputText(u16, u16) {
    // no-op
}

bool dSndFiVocalMgr_c::startFiVocal(s16 pitch, s16 bank) {
    nw4r::snd::SoundHandle &handle = dSndHarpSongMgr_c::GetInstance()->getFiSingHandle();
    handle.Stop(3);
    mSpeechHandle.Stop(3);

    if (pitch < 0x39 || pitch > 0x4C) {
        return false;
    }

    if (bank < 0 || bank >= 9) {
        return false;
    }

    // Already stopped above, but again doesn't hurt
    mSpeechHandle.Stop(3);

    bool ok = dSndSmallEffectMgr_c::GetInstance()->playSoundInternalChecked(SE_NpcKen_VOCAL, &handle);
    if (ok) {
        nw4r::snd::SeqSoundHandle seqHandle(&handle);
        seqHandle.WriteVariable(0, pitch - 0x30);
        seqHandle.WriteVariable(2, bank + 0x1F);
    }

    return ok;
}
