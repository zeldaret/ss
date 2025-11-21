#include "d/snd/d_snd_source_harp_related.h"

#include "d/snd/d_snd_harp_song_mgr.h"
#include "d/snd/d_snd_source.h"

dSndSourceHarpRelated_c::dSndSourceHarpRelated_c(
    s32 sourceType, dAcBase_c *ac, const char *name, dSndSourceGroup_c *pOwnerGroup
)
    : dSoundSource_c(sourceType, ac, name, pOwnerGroup) {
    // TODO
}

dSndSourceHarpRelated_c::~dSndSourceHarpRelated_c() {
    // TODO
}

void dSndSourceHarpRelated_c::postCalc() {
    // TODO
}

void dSndSourceHarpRelated_c::setPause(bool flag, int fadeFrames) {
    // TODO
}

bool dSndSourceHarpRelated_c::harpSoundRelated() {
    bool isHarpSoundRelated = false;
    if (checkFlag(1) && dSndHarpSongMgr_c::sInstance->isPlayingHarpRelated()) {
        isHarpSoundRelated = true;
    }
    return isHarpSoundRelated;
}
