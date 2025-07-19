#include "d/snd/d_snd_se_sound.h"
#include "d/snd/d_snd_source_py_bird.h"
#include "d/snd/d_snd_source_npc.h"
#include "d/snd/d_snd_state_mgr.h"
#include "d/snd/d_snd_wzsound.h"
#include "nw4r/snd/snd_SeqSoundHandle.h"

void dSndSourcePyBird_c::cbPyBirdGlide(dSndSeSound_c *sound, dSoundSource_c *source, nw4r::snd::SoundHandle &handle) {
    s32 var = 0;
    if (!dSndStateMgr_c::GetInstance()->isInEvent()) {
        var = static_cast<dSndSourcePyBird_c*>(source)->mSeqVarValue;
    }
    nw4r::snd::SeqSoundHandle seqHandle(&handle);
    seqHandle.WriteVariable(10, var);
}

void dSndSourceNpc_c::postSetupSound(u32 playingId, u32 requestedId, dSndSeSound_c *seSound) {
    // TODO
}

void dSndSourcePyBird_c::postSetupSound(u32 playingId, u32 requestedId, dSndSeSound_c *seSound) {
    if (requestedId == SE_PyBird_GLIDE) {
        seSound->setCallback2(&cbPyBirdGlide);
    }
}
