
#include "d/snd/d_snd_player_mgr.h"

#include "common.h"
#include "d/a/d_a_base.h"
#include "d/snd/d_snd_area_sound_effect_mgr.h"
#include "d/snd/d_snd_bgm_mgr.h"
#include "d/snd/d_snd_control_player_mgr.h"
#include "d/snd/d_snd_file_mgr.h"
#include "d/snd/d_snd_mgr.h"
#include "d/snd/d_snd_small_effect_mgr.h"
#include "d/snd/d_snd_source.h"
#include "d/snd/d_snd_source_enums.h"
#include "d/snd/d_snd_source_mgr.h"
#include "d/snd/d_snd_state_mgr.h"
#include "d/snd/d_snd_wzsound.h"
#include "egg/audio/eggAudioRmtSpeakerMgr.h"
#include "egg/core/eggDvdRipper.h"
#include "nw4r/snd/snd_SoundHandle.h"
#include "nw4r/snd/snd_SoundStartable.h"
#include "sized_string.h"
#include "toBeSorted/music_mgrs.h"

const char *dSndPlayerMgr_c::getSoundArchivePath() {
    return "Sound/WZSound.brsar";
}

SND_DISPOSER_DEFINE(dSndPlayerMgr_c);

dSndPlayerMgr_c::dSndPlayerMgr_c() : field_0x010(0), field_0x011(0), mState0(-1), mState1(-1), mState2(-1), mFlags(0) {}

void dSndPlayerMgr_c::enterPauseState() {
    dSndControlPlayerMgr_c::GetInstance()->setVolume(PLAYER_FAN, 0.3f, 5);
    dSndControlPlayerMgr_c::GetInstance()->setVolume(PLAYER_AREA, 0.3f, 5);
    dSndControlPlayerMgr_c::GetInstance()->setVolume(PLAYER_AREA_IN_WATER_LV, 0.3f, 5);
    // has other effects, such as reducing BGM volume
    onFlag(MGR_PAUSE);
}

void dSndPlayerMgr_c::createFileManager() {
    dSndMgr_c::getPlayer().detail_SetFileManager(
        dSndFileManager::create(dSndMgr_c::GetInstance()->getArchive(), dSndMgr_c::GetInstance()->getSoundHeap())
    );
}

void dSndPlayerMgr_c::setupState0() {
    if (mState0 > 0) {
        return;
    }
    initialize();

    // TODO: Ugh, maybe convert the enums to unsigned ints?
    dSndMgr_c::GetInstance()->loadGroup((unsigned int)GRP_STATIC, nullptr, 0);
    mState0 = dSndMgr_c::GetInstance()->saveState();

    s32 tmpState = dSndMgr_c::GetInstance()->saveState();
    dSndMgr_c::GetInstance()->loadGroup((unsigned int)GRP_BGM_PLAY_DATA_STATIC, nullptr, 0);

    // TODO - loading static play data

    dSndMgr_c::GetInstance()->loadState(tmpState);
}

void dSndPlayerMgr_c::popToState0() {
    if (mState0 < 0) {
        return;
    }
    dSndMgr_c::GetInstance()->loadState(mState0);
    mState1 = -1;
    mState2 = -1;
}

void dSndPlayerMgr_c::saveState1() {
    mState1 = dSndMgr_c::GetInstance()->saveState();
}

void dSndPlayerMgr_c::popToState1() {
    if (mState1 < 0) {
        return;
    }
    dSndMgr_c::GetInstance()->loadState(mState1);
    mState2 = -1;
}

void dSndPlayerMgr_c::saveState2() {
    mState2 = dSndMgr_c::GetInstance()->saveState();
}

void dSndPlayerMgr_c::popToState2() {
    if (mState2 < 0) {
        return;
    }
    dSndMgr_c::GetInstance()->loadState(mState2);
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

void dSndPlayerMgr_c::setup(s32 channel) {
    EGG::AudioRmtSpeakerMgr::setup(channel, nullptr);
}

void dSndPlayerMgr_c::shutdown(s32 channel) {
    EGG::AudioRmtSpeakerMgr::shutdown(channel, nullptr);
}

void dSndPlayerMgr_c::enterSystemMenu() {
    if (checkFlag(MGR_CAUTION)) {
        return;
    }
    enterPauseState();
    onFlag(MGR_SYSTEM);
    dSndSmallEffectMgr_c::GetInstance()->playSound(SE_S_MENU_IN);
}

void dSndPlayerMgr_c::leaveSystemMenu() {
    if (checkFlag(MGR_CAUTION)) {
        return;
    }
    leavePauseState();
    offFlag(MGR_SYSTEM);
}

void dSndPlayerMgr_c::enterCaution() {
    if (checkFlag(MGR_CAUTION)) {
        return;
    }

    s32 frames = 9;
    if (fn_80364DA0(ENEMY_SOUND_MGR)) {
        frames = 0;
    }

    for (int i = 0; i < dSndControlPlayerMgr_c::sNumPlayers; i++) {
        if ((u32)i != PLAYER_SMALL_IMPORTANT) {
            dSndControlPlayerMgr_c::GetInstance()->getPlayer1(i)->PauseAllSound(true, frames);
        }
    }

    onFlag(MGR_CAUTION);
    dSndSmallEffectMgr_c::GetInstance()->playSound(SE_S_CAUTION_IN);
}

void dSndPlayerMgr_c::leaveCaution() {
    if (!checkFlag(MGR_CAUTION)) {
        return;
    }

    EGG::AudioRmtSpeakerMgr::connectAllByForce();
    offFlag(MGR_CAUTION);
    for (int i = 0; i < dSndControlPlayerMgr_c::sNumPlayers; i++) {
        if ((u32)i != PLAYER_SMALL_IMPORTANT) {
            dSndControlPlayerMgr_c::GetInstance()->getPlayer1(i)->PauseAllSound(false, 9);
        }
    }

    dSndBgmMgr_c::GetInstance()->pauseAllBgm();
    dSndAreaSoundEffectMgr_c::GetInstance()->pauseAllSounds();
    dSndSourceMgr_c::GetInstance()->pauseAllSound();
}

void dSndPlayerMgr_c::setMsgActor(s32 msgIdx, dAcBase_c *actor) {
    if (actor != nullptr) {
        dSoundSource_c *source = static_cast<dSoundSource_c *>(actor->getSoundSource());
        if (source != nullptr) {
            dSndSmallEffectMgr_c::GetInstance()->setButtonPressSound(source);
            dSndSourceMgr_c::GetInstance()->setMsgSource(source);
        } else {
            dSndSmallEffectMgr_c::GetInstance()->resetButtonPressSound();
        }
    } else {
        dSndSmallEffectMgr_c::GetInstance()->resetButtonPressSound();
    }
    dSndStateMgr_c::GetInstance()->doMsgStart(msgIdx);
}

void dSndPlayerMgr_c::unsetMsgActor() {
    dSndStateMgr_c::GetInstance()->doMsgEnd();
    dSndSmallEffectMgr_c::GetInstance()->resetButtonPressSound();
    dSndSourceMgr_c::GetInstance()->setMsgSource(nullptr);
}

void dSndPlayerMgr_c::enterMsgWait() {
    onFlag(MGR_MSG_WAIT);
    dSndStateMgr_c::GetInstance()->doMsgWaitStart();
}

void dSndPlayerMgr_c::leaveMsgWait() {
    offFlag(MGR_MSG_WAIT);
    dSndStateMgr_c::GetInstance()->doMsgWaitEnd();
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
        res = dSndMgr_c::getPlayer().StartSoundReturnStatus(pHandle, soundId, pStartInfo);
    } else {
        res = dSndMgr_c::getPlayer().StartSoundReturnStatus(pHandle, soundId, nullptr);
    }
    return res;
}

nw4r::snd::SoundStartable::StartResult dSndPlayerMgr_c::startSound(
    nw4r::snd::SoundHandle *pHandle, const char *soundLabel, const nw4r::snd::SoundStartable::StartInfo *pStartInfo
) {
    u32 id = dSndPlayerMgr_c::GetInstance()->convertLabelStringToSoundId(soundLabel);
    return startSound(pHandle, id, pStartInfo);
}

u32 dSndPlayerMgr_c::getRemoConSoundVariant(u32 soundId) const {
    const char *label = dSndMgr_c::getSoundLabelString(soundId);
    SizedString<64> variant;
    variant.sprintf("%s_RC", label);
    return sInstance->convertLabelStringToSoundId(variant);
}

u32 dSndPlayerMgr_c::getRemoConSoundVariantDemo(u32 soundId) const {
    const char *label = getDemoArchive()->GetSoundLabelString(soundId);
    SizedString<64> variant;
    variant.sprintf("%s_RC", label);
    return sInstance->convertLabelStringToSoundId(variant);
}

u32 dSndPlayerMgr_c::convertLabelStringToSoundId(const char *label) const {
    return dSndMgr_c::GetInstance()->changeNameToId(label);
}

nw4r::snd::SoundArchivePlayer &dSndPlayerMgr_c::getSoundArchivePlayerForType(s32 sourceType) {
    if (shouldUseDemoPlayer(sourceType)) {
        return mDemoSoundArchivePlayer;
    }
    return dSndMgr_c::getPlayer();
}

bool dSndPlayerMgr_c::shouldUseDemoPlayer(s32 sourceType) const {
    if (!mDemoSoundArchivePlayer.IsAvailable()) {
        return false;
    }

    switch (sourceType) {
        case SND_SOURCE_PLAYER:
        case SND_SOURCE_PLAYER_HEAD:
        case SND_SOURCE_58:          return true;
        default:                     return false;
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
        ok = mDemoSoundArchive.Setup(ptr);
        if (ok) {
            u32 size = mDemoSoundArchivePlayer.GetRequiredMemSize(&mDemoSoundArchive);
            void *buf2 = dSndMgr_c::GetInstance()->getSoundHeap()->Alloc(size);
            if (buf2 != nullptr) {
                ok = mDemoSoundArchivePlayer.Setup(&mDemoSoundArchive, buf2, size, nullptr, 0);
            }
        }
    }
    if (!ok) {
        dSndMgr_c::GetInstance()->loadState(stateId);
    }
    return ok;
}

void dSndPlayerMgr_c::shutdown() {
    mDemoSoundArchivePlayer.Shutdown();
    mDemoSoundArchive.Shutdown();
}

void dSndPlayerMgr_c::calc() {
    if (mDemoSoundArchivePlayer.IsAvailable()) {
        mDemoSoundArchivePlayer.Update();
    }
}

bool dSndPlayerMgr_c::isBgmPlayerId(u32 id) {
    switch (id) {
        case PLAYER_BGM:
        case PLAYER_BGM_BATTLE: return true;
        default:                return false;
    }
}
