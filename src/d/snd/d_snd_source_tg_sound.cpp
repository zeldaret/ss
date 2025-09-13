#include "d/snd/d_snd_source_tg_sound.h"

#include "common.h"
#include "d/snd/d_snd_player_mgr.h"
#include "d/snd/d_snd_source.h"
#include "sized_string.h"

dSndSourceTgSound_c::dSndSourceTgSound_c(
    s32 sourceType, dAcBase_c *ac, const char *name, dSndSourceGroup_c *pOwnerGroup
)
    : dSoundSource_c(sourceType, ac, name, pOwnerGroup), mTgActive(true) {}

void dSndSourceTgSound_c::postSetup() {
    switch (mSubtype) {
        // TODO subtypes
        case 7:
            mTgActive = false;
            break;
        default:
            mTgActive = true;
            break;
    }

    SizedString<64> label;
    label.sprintf("SE_%s", mpName);
    mTgSoundId = dSndPlayerMgr_c::GetInstance()->convertLabelStringToSoundId(label);
}

void dSndSourceTgSound_c::setTgActive(bool active) {
    switch (mSubtype) {
        case 34:
        case 35:
            mTgActive = !active;
            break;
        default:
            mTgActive = active;
            break;
    }
}

void dSndSourceTgSound_c::postCalc() {
    if (mTgSoundId == -1) {
        return;
    }
    if (!mTgActive) {
        return;
    }
    holdSound(mTgSoundId);
}
