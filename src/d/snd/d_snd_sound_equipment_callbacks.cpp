
#include "common.h"
#include "d/a/d_a_player.h"
#include "d/snd/d_snd_checkers.h"
#include "d/snd/d_snd_control_player_mgr.h"
#include "d/snd/d_snd_player_mgr.h"
#include "d/snd/d_snd_se_sound.h"
#include "d/snd/d_snd_se_sound_2.h"
#include "d/snd/d_snd_source_equipment.h"
#include "d/snd/d_snd_wzsound.h"
#include "nw4r/snd/snd_SeqSoundHandle.h"
#include "nw4r/snd/snd_SoundPlayer.h"

void dSndSourceEquipment_c::cbBeFlyLv(dSndSeSound_c *sound, dSoundSource_c *source, nw4r::snd::SoundHandle &handle) {
    if (sound->getCtxParams()[0].floatParam > 0.0f) {
        nw4r::snd::SeqSoundHandle seqHandle(&handle);
        f32 volume = 0.6f;
        f32 pitch = 1.0f;
        if (sound->getCtxParams()[0].floatParam == 1.0f) {
            volume = 1.0f;
            pitch = 1.3f;
        }
        handle.SetVolume(volume, 0);
        handle.SetPitch(pitch);
        seqHandle.SetTempoRatio(pitch);
    }
}

void dSndSourceEquipment_c::cbBeThrowRc(dSndSeSound_c *sound, dSoundSource_c *source, nw4r::snd::SoundHandle &handle) {
    if (dAcPy_c::GetLink() != nullptr && !dAcPy_c::GetLink()->hasvt_0x1C0()) {
        handle.Stop(10);
        return;
    }

    dSndSeSound2_c *flyHoldHandle = source->getHandleType2ForSoundId(SE_BE_FLY_LV);
    if (flyHoldHandle == nullptr) {
        if (sound->getField0x114() != 0) {
            handle.Stop(3);
            sound->setCallback2(nullptr);
        }
        return;
    }

    nw4r::snd::SeqSoundHandle seqHandle(sound->getHandle());
    if (flyHoldHandle->getCtxParams()[0].floatParam > 0.0f) {
        f32 volume = 0.6f;
        f32 pitch = 1.0f;
        if (flyHoldHandle->getCtxParams()[0].floatParam == 1.0f) {
            volume = 1.0f;
            pitch = 1.3f;
        }
        handle.SetVolume(volume, 0);
        handle.SetPitch(pitch);
        seqHandle.SetTempoRatio(pitch);
    }
}

void dSndSourceEquipment_c::cbBmIgnitionLv(
    dSndSeSound_c *sound, dSoundSource_c *source, nw4r::snd::SoundHandle &handle
) {
    // TODO how are these flags set
    if (source->checkFlag(0x1)) {
        handle.SetLpfFreq(-0.4f);
    } else {
        handle.SetLpfFreq(0.0f);
    }
}

void dSndSourceEquipment_c::cbBmBound(dSndSeSound_c *sound, dSoundSource_c *source, nw4r::snd::SoundHandle &handle) {
    f32 param = sound->getCtxParams()[0].floatParam;
    if (param <= 2.0f) {
        handle.Stop(0);
        return;
    }
    // TODO what does this do?
    f32 volume = param / 70.0f + 0.3f;
    if (volume > 1.0f) {
        volume = 1.0f;
    }
    handle.SetVolume(volume, 0);
}

static const f32 beMinSpeed = 2.0f;
static const f32 beMaxSpeed = 40.0f;

void dSndSourceEquipment_c::cbWiFxLv(dSndSeSound_c *sound, dSoundSource_c *source, nw4r::snd::SoundHandle &handle) {
    if (dAcPy_c::GetLink() == nullptr) {
        return;
    }
    f32 volume = 1.0f;
    f32 pitch = 1.0f;
    if (dAcPy_c::GetLink()->checkCurrentAction(/* WHIP_LOCKED */ 0x8E)) {
        volume = 0.9f;
    } else {
        if (dAcPy_c::GetLink()->checkCurrentAction(/* USE_WHIP */ 0x8D)) {
            pitch = 1.2f;
        } else if (dAcPy_c::GetLink()->isUsingWhip()) {
            f32 speed = source->getSpeed();

            if (speed <= beMinSpeed) {
                volume = 0.2;
            } else if (speed >= beMaxSpeed) {
                volume = 0.8f;
            } else {
                volume = speed * 0.6f / 38.0f;
                volume += 0.8f;
                volume -= (beMaxSpeed * 0.6f / 38.0f);
                // @bug not actually clamped
                (void)nw4r::ut::Clamp(volume, 0.2f, 0.8f);
            }
            pitch = 0.9f;
        }
    }
    // Save the current speed value...
    sound->shiftFloatParam(pitch);
    // And read the speed value from 4 frames earlier (~133ms)
    f32 actualPitch = sound->getCtxParams()[5].floatParam;
    handle.SetVolume(volume, 0);
    handle.SetPitch(actualPitch);
}

void dSndSourceEquipment_c::cbPcHit(dSndSeSound_c *sound, dSoundSource_c *source, nw4r::snd::SoundHandle &handle) {
    nw4r::snd::SoundPlayer *equipmentPlayer =
        dSndControlPlayerMgr_c::GetInstance()->getPlayer2(dSndPlayerMgr_c::PLAYER_LINK_EQUIPMENT);
    s32 result = 0;
    SoundPropertiesChecker checker(handle.GetId());
    // due to a @bug in SoundPropertiesChecker's ctor, mCounter4 is uninitialized,
    // so when MWCC creates a copy of `checker` for ForEachSound,
    // it uses r3 to store the ininitialized field, which currently holds
    // `equipmentPlayer`. So this behaves as if someone wrote:
    //     checker.mCounter4 = reinterpret_cast<u32>(equipmentPlayer);
    // This is harmless here, since we never use the result.
    checker.mpPlayCounter = &result;

    // The more equipment sounds are playing, the more
    // the volume on SE_PC_HIT is reduced
    equipmentPlayer->ForEachSound(checker, false);
    if (result == 2) {
        handle.SetVolume(0.7f, 0);
    } else if (result == 3) {
        handle.SetVolume(0.5f, 0);
    } else if (result >= 4) {
        handle.SetVolume(0.3f, 0);
    }
}

void dSndSourceEquipment_c::postSetupSound(u32 playingId, u32 requestedId, dSndSeSound_c *seSound) {
    switch (requestedId) {
        case SE_BE_FLY_LV:      seSound->setCallback2(&cbBeFlyLv); break;
        case SE_BE_THROW_RC:    seSound->setCallback2(&cbBeThrowRc); break;
        case SE_BM_IGNITION_LV: seSound->setCallback2(&cbBmIgnitionLv); break;
        case SE_BM_BOUND:       seSound->setCallback1(&cbBmBound); break;
        case SE_WI_FX_LV:       seSound->setCallback2(&cbWiFxLv); break;
        case SE_PC_HIT:         seSound->setCallback1(&cbPcHit); break;
    }
}
