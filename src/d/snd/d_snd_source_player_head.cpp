#include "d/snd/d_snd_source_player_head.h"

#include "common.h"
#include "d/d_player_act.h"
#include "d/snd/d_snd_rng_link_voice.h"
#include "d/snd/d_snd_wzsound.h"

dSndSourcePlayerHead_c::dSndSourcePlayerHead_c(
    s32 sourceType, dAcBase_c *ac, const char *name, dSndSourceGroup_c *pOwnerGroup
)
    : dSoundSource_c(sourceType, ac, name, pOwnerGroup) {
    configureVoiceRngs();
}

void dSndSourcePlayerHead_c::configureVoiceRngs() {
    field_0x22A0 = SND_LV_MAX;
    mLastPlayedSound = SND_LV_MAX;

    for (int i = 0; i < SND_LV_MAX; i++) {
        mLinkVoiceRngs[i].configure(dSndRngLinkVoice_c::getConfig(i), this);
    }
}

bool dSndSourcePlayerHead_c::startVoiceLine(u32 id) {
    // id might be a sound ID, or it might be the ID of one of our RNGs
    if (id > LABEL_LV_START && id < LABEL_LV_END) {
        return startVoiceSoundInternal(id);
    }

    if (id >= SND_LV_MAX) {
        return false;
    }

    if (checkFlag(1)) {
        switch (id) {
            case SND_LV_JUMP_S:
            case SND_LV_SHOOT_START:
            case SND_LV_SHOOT_SLOPE:
            case SND_LV_RUN_UP:
            case SND_LV_CLIMB_STEP:
            case SND_LV_BACKFLIP:
            case SND_LV_CLIMB_WALL:  {
                id = SND_LV_QUIETLY_S;
                break;
            }
            case SND_LV_JUMP_M:
            case SND_LV_JUMP_L:
            case SND_LV_ZENTEN: {
                id = SND_LV_QUIETLY_M;
                break;
            }
            case SND_LV_GRAB:
            case SND_LV_HUNG: {
                return false;
            }
        }
    }

    f32 stamina = daPlayerActBase_c::getStamina();
    switch (id) {
        case SND_LV_TIRED_S_1: {
            if (stamina < 0.25) {
                id = SND_LV_TIRED_L;
            } else if (stamina < 0.5) {
                id = SND_LV_TIRED_M;
            } else if (stamina < 0.8) {
                id = SND_LV_TIRED_S_2;
            } else {
                return false;
            }
            break;
        }
        case SND_LV_CLIMB_TIRED_S_1: {
            if (stamina < 0.125) {
                id = SND_LV_CLIMB_TIRED_L;
            } else if (stamina < 0.375) {
                id = SND_LV_CLIMB_TIRED_M;
            } else {
                return false;
            }
            break;
        }
        case SND_LV_LIFT_TIRED_S_1: {
            if (stamina < 0.125) {
                id = SND_LV_LIFT_TIRED_L;
            } else if (stamina < 0.375) {
                id = SND_LV_LIFT_TIRED_M;
            } else {
                return false;
            }
            break;
        }
        case SND_LV_TIRED_RECOVER: {
            if (stamina > 0.75) {
                id = SND_LV_TIRED_RECOVER_S;
            } else if (stamina > 0.5) {
                id = SND_LV_TIRED_RECOVER_M;
            } else if (stamina > 0.375) {
                id = SND_LV_TIRED_RECOVER_L;
            }
            break;
        }

        case SND_LV_BREATH_TIRED_S_1: {
            if (stamina <= 0.0f) {
                id = SND_LV_BREATH_TIRED_L;
            } else if (stamina < 0.125) {
                id = SND_LV_BREATH_TIRED_M;
            } else if (stamina < 0.25) {
                id = SND_LV_BREATH_TIRED_S_2;
            } else {
                return false;
            }
            break;
        }

        case SND_LV_BADSMELL_TIRED_S_1: {
            if (stamina < 0.125) {
                id = SND_LV_BADSMELL_TIRED_L;
            } else if (stamina < 0.375) {
                id = SND_LV_BADSMELL_TIRED_M;
            } else if (stamina < 0.99) {
                id = SND_LV_BADSMELL_TIRED_S_2;
            } else {
                return false;
            }
            break;
        }

        case SND_LV_DIE_WATER:
        case SND_LV_DAMAGE_WATER_M:
        case SND_LV_DAMAGE_WATER_S:
        case SND_LV_DAMAGE_ELEC_W:  startSound(SE_L_WATER_AIR_OUT); break;
    }

    if (mLastPlayedSound != SND_LV_MAX && isPlayingActionSound()) {
        s32 oldPriority = mLinkVoiceRngs[mLastPlayedSound].getPriority();
        s32 newPriority = mLinkVoiceRngs[id].getPriority();

        if (oldPriority > newPriority) {
            return false;
        }

        if (oldPriority == newPriority && !mLinkVoiceRngs[id].getPriorityTiebreaker()) {
            return false;
        }
    }

    field_0x22A0 = id;
    bool ok = mLinkVoiceRngs[id].doRndIdNoReuse();
    if (ok) {
        mLastPlayedSound = id;
    }

    return ok;
}

bool dSndSourcePlayerHead_c::startVoiceSoundInternal(u32 id) {
    bool ok = false;
    // @bug shouldn't this be > and < ?
    if (id >= LABEL_LV_START && id <= LABEL_LV_END) {
        ok = startSound(id, &mSoundHandle) != nullptr;
        field_0x22A0 = SND_LV_MAX;
    }
    return ok;
}
