
#include "d/snd/d_snd_player_mgr.h"

#include "common.h"
#include "d/snd/d_snd_mgr.h"
#include "egg/core/eggDvdRipper.h"
#include "nw4r/snd/snd_SoundHandle.h"
#include "nw4r/snd/snd_SoundStartable.h"
#include "sized_string.h"

const char *dSndPlayerMgr_c::getSoundArchivePath() {
    return "Sound/WZSound.brsar";
}

template class SndMgrDisposer<dSndPlayerMgr_c>;

dSndPlayerMgr_c::dSndPlayerMgr_c()
    : field_0x010(0), field_0x011(0), field_0x014(-1), field_0x018(-1), field_0x01C(-1), mFlags(0) {}

u32 dSndPlayerMgr_c::getFreeSize() {
    return dSndMgr_c::GetInstance()->getSoundHeap()->GetFreeSize();
}

nw4r::snd::SoundStartable::StartResult dSndPlayerMgr_c::startSound(
    nw4r::snd::SoundHandle *pHandle, u32 soundId, const nw4r::snd::SoundStartable::StartInfo *pStartInfo
) {
    if (mFlags & 0x2) {
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
