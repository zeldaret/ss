#include "d/snd/d_snd_source_equipment.h"

#include "common.h"
#include "d/a/d_a_player.h"
#include "d/snd/d_snd_source.h"
#include "d/snd/d_snd_source_enums.h"
#include "d/snd/d_snd_state_mgr.h"
#include "d/snd/d_snd_wzsound.h"

dSndSourceEquipment_c::dSndSourceEquipment_c(
    s32 sourceType, dAcBase_c *ac, const char *name, dSndSourceGroup_c *pOwnerGroup
)
    : dSoundSource_c(sourceType, ac, name, pOwnerGroup) {}

void dSndSourceEquipment_c::shutdown() {
    dSoundSource_c::shutdown();
    if (mSourceType == SND_SOURCE_ARROW && !mIsDisabled) {
        stopAllSound(2);
    }
}

u32 dSndSourceEquipment_c::overrideStartSoundId(u32 soundId) {
    switch (mSourceType) {
        case SND_SOURCE_BLADE:
            if (soundId == SE_SW_PUTIN && (dSndStateMgr_c::GetInstance()->isInEvent("ECaptain_c_dead") ||
                                           dSndStateMgr_c::GetInstance()->isInEvent("ECaptain_c_deadType2"))) {
                // TODO: I guess there would be a distracting sword stow sound in the Scervo death sequence?
                return -1;
            }
            break;
        case SND_SOURCE_SHIELD:
            switch (soundId) {
                case SE_SH_GIRD_HIT:
                    if (mpCtxParam != nullptr) {
                        switch (mpCtxParam->intParam) {
                            case 1: soundId = SE_SH_GIRD_HIT_FIRE; break;
                            case 2:
                            case 4: soundId = SE_SH_GIRD_HIT_ELEC; break;
                            case 3: soundId = SE_SH_GIRD_HIT_CURSE; break;
                        }
                    }
                    break;
                case SE_SH_GIRD_JUST:
                    if (mpCtxParam != nullptr) {
                        switch (mpCtxParam->intParam) {
                            case 1: soundId = SE_SH_GIRD_JUST_FIRE; break;
                            case 2:
                            case 4: soundId = SE_SH_GIRD_JUST_ELEC; break;
                            case 3: soundId = SE_SH_GIRD_JUST_CURSE; break;
                        }
                    }
                    break;
                case SE_SH_BREAK:
                    // TODO enum
                    if (dAcPy_c::getCurrentlyEquippedShieldType() == 0) {
                        soundId = SE_SH_BREAK_WOOD;
                        break;
                    }
                    break;
            }
            break;
        case SND_SOURCE_BOOMERANG:
            if (soundId == SE_BE_HIT_DMG || soundId == SE_BE_COME_BACK) {
                // Not sure if this override handler is the right place for a stop call
                stopSounds(SE_BE_THROW_RC, 10);
            }
            break;
        case SND_SOURCE_BOMB:
            if (soundId == SE_BM_FALL_WATER) {
                // Not sure if this override handler is the right place for a shutdown call
                shutdown();
            }
            break;
        case SND_SOURCE_ARROW:
            if (soundId != SE_AW_FLY) {
                stopSounds(SE_AW_FLY, 1);
                shutdown();
            }
    }
    return soundId;
}

void dSndSourceEquipment_c::postStartSound(nw4r::snd::SoundHandle &handle, dSndSeSound_c *pSound, u32 id) {
    dSoundSource_c::postStartSound(handle, pSound, id);
    handleRemoCon(&handle, id);
}

u32 dSndSourceEquipment_c::overrideHitObjSoundId(u32 soundId, dSoundSource_c *source) {
    if (source == nullptr) {
        return soundId;
    }
    u32 overrideId = soundId;
    if (checkUserParamMask(soundId, 0x200)) {
        overrideId = modifySoundId(soundId, source);
    }
    if (overrideId == soundId) {
        switch (source->getCategory()) {
            case SND_SOURCE_CATEGORY_OBJECT:
                switch (soundId) {
                    case SE_SW_HIT:
                        if (source->getSourceType() == SND_SOURCE_ITEM ||
                            source->getSourceType() == SND_SOURCE_BAMBOO) {
                            return -1;
                        } else {
                            overrideId = SE_SW_HIT_OBJ;
                        }
                        break;
                    case SE_SW_HIT_DMG:      overrideId = SE_SW_HIT_OBJ; break;
                    case SE_SW_HIT_DMG_STAB: overrideId = SE_SW_HIT_OBJ; break;
                    case SE_SW_HIT_NO_DMG:   overrideId = SE_SW_HIT_NO_DMG_OBJ; break;
                    case SE_SW_BEAM_HIT_DMG: overrideId = SE_SW_BEAM_HIT; break;
                    case SE_WI_HIT_DMG:      overrideId = SE_WI_HIT; break;
                    case SE_HS_HIT_DMG:      overrideId = SE_HS_REBOUND; break;
                }
                break;
            case SND_SOURCE_CATEGORY_ENEMY: break;
            case SND_SOURCE_CATEGORY_NPC:
                if (source->getSourceType() != SND_SOURCE_INSECT) {
                    if (soundId == SE_BE_HIT) {
                        overrideId = SE_BE_HIT_NPC;
                    } else {
                        overrideId = -1;
                    }
                }
                break;
            case SND_SOURCE_CATEGORY_EQUIPMENT:
                if (source->getSourceType() == SND_SOURCE_BOMB && soundId == SE_BE_HIT_DMG) {
                    overrideId = SE_BE_HIT;
                } else {
                    overrideId = -1;
                }
                break;
            default: overrideId = -1; break;
        }
    }
    return overrideId;
}

dSndSourceEquipmentWhip_c::dSndSourceEquipmentWhip_c(
    s32 sourceType, dAcBase_c *ac, const char *name, dSndSourceGroup_c *pOwnerGroup
)
    : dSndSourceEquipment_c(sourceType, ac, name, pOwnerGroup) {}

void dSndSourceEquipmentWhip_c::postCalc() {
    if (dAcPy_c::GetLink() != nullptr && dAcPy_c::GetLink()->isUsingWhip()) {
        holdSound(SE_WI_FX_LV);
    }
}
