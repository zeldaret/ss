#include "d/snd/d_snd_source_enemy.h"

#include "common.h"
#include "d/snd/d_snd_anim_sound.h"
#include "d/snd/d_snd_source.h"
#include "d/snd/d_snd_source_enums.h"
#include "d/snd/d_snd_source_mgr.h"
#include "d/snd/d_snd_wzsound.h"

dSndSourceEnemy_c::dSndSourceEnemy_c(u8 sourceType, dAcBase_c *ac, const char *name, dSndSourceGroup_c *pOwnerGroup)
    : dSoundSource_c(sourceType, ac, name, pOwnerGroup), field_0x164(2), field_0x168(0), field_0x169(0) {
    if ((SoundSourceType_e)sourceType < SND_SOURCE_ENEMY_28) {
        dSndSourceMgr_c::GetInstance()->incrementEnemyCount();
        mIsRegisteredEnemy = true;
    } else {
        mIsRegisteredEnemy = false;
    }
}

dSndSourceEnemy_c::~dSndSourceEnemy_c() {
    unregisterEnemySource();
}

void dSndSourceEnemy_c::shutdown() {
    dSoundSource_c::shutdown();
    unregisterEnemySource();
}

void dSndSourceEnemy_c::unregisterEnemySource() {
    if (!mIsRegisteredEnemy) {
        return;
    }

    if (mSourceType < SND_SOURCE_ENEMY_28) {
        dSndSourceMgr_c::GetInstance()->decrementEnemyCount();
    }
    mIsRegisteredEnemy = false;
}

void dSndSourceEnemy_c::postSetup() {
    if (isName("EBfish")) {
        field_0x169 = 1;
    } else if (isName("BLasBos")) {
        setVolumeFade(0.9f, 0);
    } else if (isName("BBigBos") || isName("BBsFootL") || isName("BBsFootR") || isName("BBsMane")) {
        setVolumeFade(0.9f, 0);
    } else if (isName("BAsura")) {
        getAnimSound()->setField_0x98(100.0f);
    } else if (isName("BGh") || isName("BGh2") || isName("BGh3")) {
        getAnimSound()->setField_0x98(0.0f);
    }
}

void dSndSourceEnemy_c::vt_0x020() {
    dSndSourceMgr_c::GetInstance()->registerUnkEnemyType(this);
}

UNKWORD dSndSourceEnemy_c::checkBattleBgmRelated() {
    if (getRoomId() == dSndSourceMgr_c::GetInstance()->getPlayerSourceRoomId()) {
        f32 dist = getDistanceToPlayer();
        if (field_0x168 == 0 && dist <= 1200.0f) {
            return field_0x164;
        }

        if (field_0x168 != 0 && dist < 700.0f) {
            return field_0x164;
        }
    }

    return 0;
}

u32 dSndSourceEnemy_c::overrideStartSoundId(u32 soundId) {
    switch (mSourceType) {
        case SND_SOURCE_BULLET_MAGUPPO:
            switch (mSubtype) {
                case 1:
                    switch (soundId) {
                        case SE_MagpB_HIT:    soundId = SE_MagpB_HIT_SAND; break;
                        case SE_MagpB_BROKEN: soundId = SE_MagpB_BROKEN_SAND; break;
                    }
                    break;
                case 2:
                    switch (soundId) {
                        case SE_MagpB_BROKEN: soundId = SE_MagpB_BROKEN_MA; break;
                    }
                    break;
                case 3:
                    switch (soundId) {
                        case SE_MagpB_BROKEN: soundId = SE_MagpB_BROKEN_WATER; break;
                    }
                    break;
            }
            break;
        case SND_SOURCE_LIZARUFOS:
        case SND_SOURCE_BC_Z:
            if (soundId == SE_E_DISAPPEAR) {
                unregisterEnemySource();
            }
            break;
    }

    return soundId;
}

u32 dSndSourceEnemy_c::overrideHoldSoundId(u32 soundId, bool initial) {
    if (mSourceType == SND_SOURCE_BULLET_MAGUPPO) {
        switch (mSubtype) {
            case 1:
                switch (soundId) {
                    case SE_MagpB_FLY_LV: soundId = SE_MagpB_FLY_LV_SAND; break;
                }
                break;
            case 2:
            case 3:
                switch (soundId) {
                    case SE_MagpB_FLY_LV: soundId = SE_MagpB_FLY_LV_MA; break;
                }
                break;
        }
    }
    return soundId;
}
