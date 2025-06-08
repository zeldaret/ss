
#include "d/snd/d_snd_player_mgr.h"

#include "common.h"
#include "d/snd/d_snd_control_player_mgr.h"
#include "d/snd/d_snd_mgr.h"
#include "d/snd/d_snd_small_effect_mgr.h"
#include "d/snd/d_snd_source_enums.h"
#include "d/snd/d_snd_wzsound.h"
#include "egg/core/eggDvdRipper.h"
#include "nw4r/snd/snd_SoundHandle.h"
#include "nw4r/snd/snd_SoundStartable.h"
#include "sized_string.h"

const char *dSndPlayerMgr_c::getSoundArchivePath() {
    return "Sound/WZSound.brsar";
}

SND_DISPOSER_DEFINE(dSndPlayerMgr_c);

dSndPlayerMgr_c::dSndPlayerMgr_c()
    : field_0x010(0), field_0x011(0), field_0x014(-1), field_0x018(-1), field_0x01C(-1), mFlags(0) {}

void dSndPlayerMgr_c::enterPauseState() {
    dSndControlPlayerMgr_c::GetInstance()->setVolume(PLAYER_FAN, 0.3f, 5);
    dSndControlPlayerMgr_c::GetInstance()->setVolume(PLAYER_AREA, 0.3f, 5);
    dSndControlPlayerMgr_c::GetInstance()->setVolume(PLAYER_AREA_IN_WATER_LV, 0.3f, 5);
    // has other effects, such as reducing BGM volume
    onFlag(MGR_PAUSE);
}

u32 dSndPlayerMgr_c::getFreeSize() {
    return dSndMgr_c::GetInstance()->getSoundHeap()->GetFreeSize();
}

void dSndPlayerMgr_c::leavePauseState() {
    dSndControlPlayerMgr_c::GetInstance()->setVolume(PLAYER_FAN, 1.0f, 5);
    dSndControlPlayerMgr_c::GetInstance()->setVolume(PLAYER_AREA, 1.0f, 5);
    dSndControlPlayerMgr_c::GetInstance()->setVolume(PLAYER_AREA_IN_WATER_LV, 1.0f, 5);
    offFlag(MGR_PAUSE);
}

void dSndPlayerMgr_c::enterMenu() {
    enterPauseState();
    dSndSmallEffectMgr_c::GetInstance()->playSound(SE_S_MENU_IN);
}

void dSndPlayerMgr_c::leaveMenu() {
    leavePauseState();
    dSndSmallEffectMgr_c::GetInstance()->playSound(SE_S_MENU_OUT);
}

void dSndPlayerMgr_c::enterMap() {
    enterPauseState();
    onFlag(MGR_MAP);
    dSndSmallEffectMgr_c::GetInstance()->playSound(SE_S_MAP_OPEN);
}

void dSndPlayerMgr_c::leaveMap() {
    leavePauseState();
    offFlag(MGR_MAP);
    dSndSmallEffectMgr_c::GetInstance()->playSound(SE_S_MAP_CLOSE);
}

void dSndPlayerMgr_c::enterHelp() {
    onFlag(MGR_HELP);
    dSndSmallEffectMgr_c::GetInstance()->playSound(SE_S_HELP_IN);
}

void dSndPlayerMgr_c::leaveHelp() {
    offFlag(MGR_HELP);
    dSndSmallEffectMgr_c::GetInstance()->playSound(SE_S_HELP_OUT);
}

nw4r::snd::SoundStartable::StartResult dSndPlayerMgr_c::startSound(
    nw4r::snd::SoundHandle *pHandle, u32 soundId, const nw4r::snd::SoundStartable::StartInfo *pStartInfo
) {
    if (checkFlag(MGR_UNK_0x2)) {
        return nw4r::snd::SoundStartable::START_ERR_USER;
    }

    nw4r::snd::SoundHandle handle;
    if (pHandle == nullptr) {
        pHandle = &handle;
    }
    nw4r::snd::SoundStartable::StartResult res;
    if (pStartInfo != nullptr) {
        res = dSndMgr_c::getPlayer()->detail_StartSound(pHandle, soundId, pStartInfo);
    } else {
        res = dSndMgr_c::getPlayer()->detail_StartSound(pHandle, soundId, nullptr);
    }
    return res;
}

nw4r::snd::SoundStartable::StartResult dSndPlayerMgr_c::startSound(
    nw4r::snd::SoundHandle *pHandle, const char *soundLabel, const nw4r::snd::SoundStartable::StartInfo *pStartInfo
) {
    u32 id = dSndPlayerMgr_c::GetInstance()->convertLabelStringToSoundId(soundLabel);
    return startSound(pHandle, id, pStartInfo);
}

u32 dSndPlayerMgr_c::convertLabelStringToSoundId(const char *label) const {
    return dSndMgr_c::GetInstance()->changeNameToId(label);
}

nw4r::snd::SoundArchivePlayer &dSndPlayerMgr_c::getSoundArchivePlayerForType(u8 sourceType) {
    if (canUseThisPlayer(sourceType)) {
        return mSoundArchivePlayer;
    }
    return *dSndMgr_c::getPlayer();
}

bool dSndPlayerMgr_c::canUseThisPlayer(u8 sourceType) const {
    if (!mSoundArchivePlayer.IsAvailable()) {
        return false;
    }

    switch (sourceType) {
        case SND_SOURCE_PLAYER:
        case SND_SOURCE_PLAYER_HEAD:
        case SND_SOURCE_58:
            return true;
        default:
            return false;
    }
}

bool dSndPlayerMgr_c::loadDemoArchive(const char *demoArchiveName) {
    if (demoArchiveName == nullptr) {
        return false;
    }

    int stateId = dSndMgr_c::GetInstance()->saveState();
    SizedString<64> path;
    path.sprintf("Sound/demo/%s.brsar", demoArchiveName);
    void *buf = dSndMgr_c::GetInstance()->getSoundHeap()->Alloc(getFreeSize() - 0x40000);
    if (buf == nullptr) {
        return false;
    }
    bool ok = false;
    u32 amountRead = 0;
    u32 fileSize = 0;
    EGG::DvdRipper::Arg arg(path, (u8 *)buf, nullptr, EGG::DvdRipper::ALLOC_DIR_TOP, 0, &amountRead, &fileSize);
    void *ptr = EGG::DvdRipper::loadToMainRAM(arg);
    if (ptr != nullptr) {
        ok = mSoundArchive.Setup(ptr);
        if (ok) {
            u32 size = mSoundArchivePlayer.GetRequiredMemSize(&mSoundArchive);
            void *buf2 = dSndMgr_c::GetInstance()->getSoundHeap()->Alloc(size);
            if (buf2 != nullptr) {
                ok = mSoundArchivePlayer.Setup(&mSoundArchive, buf2, size, nullptr, 0);
            }
        }
    }
    if (!ok) {
        dSndMgr_c::GetInstance()->loadState(stateId);
    }
    return ok;
}

void dSndPlayerMgr_c::shutdown() {
    mSoundArchivePlayer.Shutdown();
    mSoundArchive.Shutdown();
}

void dSndPlayerMgr_c::calc() {
    if (mSoundArchivePlayer.IsAvailable()) {
        mSoundArchivePlayer.Update();
    }
}
