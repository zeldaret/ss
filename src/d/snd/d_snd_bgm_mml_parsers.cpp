#include "d/snd/d_snd_bgm_mml_parsers.h"

#include "common.h"
#include "d/snd/d_snd_bgm_seq_data_mgr.h"
#include "d/snd/d_snd_bgm_sound_harp_mgr.h"
#include "d/snd/d_snd_harp_song_data.h"
#include "d/snd/d_snd_mgr.h"
#include "d/snd/d_snd_player_mgr.h"
#include "d/snd/d_snd_wzsound.h"
#include "nw4r/snd/snd_SoundArchive.h"
#include "sized_string.h"

dSndBgmMmlParserHarp_c::dSndBgmMmlParserHarp_c(dSndBgmSoundHarpMgr_c *mgr)
    : dSndBgmMmlParser_c(&dSndMgr_c::GetInstance()->getPlayer(), dSndMgr_c::GetInstance()->getArchive()) {
    field_0x294 = mgr;
    field_0x298 = mgr;
}

bool dSndBgmMmlParserHarp_c::parseData(const char *soundLabel, u32 baseSoundId, u32 trackMask) {
    if (soundLabel == nullptr) {
        return false;
    }
    bool ok = false;
    field_0x294->reset();
    SizedString<64> dataLabel;
    dataLabel.sprintf("%s%s", dSndBgmSeqDataMgr_c::getDataPrefix(), soundLabel);
    u32 dataId = dSndPlayerMgr_c::GetInstance()->convertLabelStringToSoundId(dataLabel);
    if (dataId != -1 &&
        dSndMgr_c::GetInstance()->getArchive()->GetSoundType(dataId) == nw4r::snd::SoundArchive::SOUND_TYPE_SEQ) {
        ok = loadAndParse(dataId, trackMask, true);
        if (ok) {
            field_0x294->setSoundId(baseSoundId);
        }
    }
    field_0x294->setLoaded();
    return ok;
}

bool dSndBgmMmlParserHarp_c::parseData(u32 baseSoundId, u32 trackMask) {
    if (baseSoundId == -1) {
        return false;
    }
    return parseData(dSndMgr_c::getSoundLabelString(baseSoundId), baseSoundId, trackMask);
}

void dSndBgmMmlParserHarp_c::CommandProc(u32 trackNo, int wait, u32 command, s32 commandArg1, s32 commandArg2) const {
    if (command > 0xFF) {
        return;
    }

    // TODO type
    switch ((s32)trackNo) {
        case 15:
            if (command == MML_JUMP) {
                field_0x298->setField_0x08(wait);
            }
            break;
        default:
            if (command == MML_SET_TEMPO) {
                field_0x294->setTempo(commandArg1);
            }
            break;
    }
}

void dSndBgmMmlParserHarp_c::NoteOnCommandProc(u32 trackNo, int wait, int key, int velocity, s32 length) const {
    // TODO type
    switch ((s32)trackNo) {
        case 15: field_0x298->addVar(wait, key, velocity); break;
        default: break;
    }
}

dSndBgmMmlParserHarpSong_c::dSndBgmMmlParserHarpSong_c()
    : dSndBgmMmlParser_c(&dSndMgr_c::GetInstance()->getPlayer(), dSndMgr_c::GetInstance()->getArchive()) {
    field_0x294 = nullptr;
}

bool dSndBgmMmlParserHarpSong_c::parseData(u32 soundId, dSndHarpSongData_c *pData) {
    if (soundId == -1) {
        return false;
    }
    if (pData == nullptr) {
        return false;
    }

    field_0x294 = pData;
    bool ok = loadAndParse(soundId, 0xFFFF, true);
    field_0x294 = nullptr;

    return ok;
}

void dSndBgmMmlParserHarpSong_c::CommandProc(u32 trackNo, int wait, u32 command, s32 commandArg1, s32 commandArg2)
    const {
    // no-op
}

void dSndBgmMmlParserHarpSong_c::NoteOnCommandProc(u32 trackNo, int wait, int key, int velocity, s32 length) const {
    if (trackNo >= 0 && trackNo < 2) {
        if (field_0x294 != nullptr) {
            field_0x294->insert2(wait, key, trackNo);
        }
    } else if (trackNo == 15) {
        if (field_0x294 != nullptr) {
            field_0x294->insert1(wait, key);
        }
    }
}

dSndBgmMmlParserTagBattle_c::dSndBgmMmlParserTagBattle_c()
    : dSndBgmMmlParser_c(&dSndMgr_c::GetInstance()->getPlayer(), dSndMgr_c::GetInstance()->getArchive()),
      mpBuf(nullptr),
      mBufSize(0) {}

bool dSndBgmMmlParserTagBattle_c::parseData(u32 soundId, dSndTagData **pOutData) {
    if (soundId == -1) {
        return false;
    }
    // First pass, check how much memory we need
    loadAndParse(soundId, (1 << 0), true);
    // alloc memory
    mpBuf = reinterpret_cast<dSndTagData *>(
        dSndMgr_c::GetInstance()->getSoundHeap()->Alloc(sizeof(dSndTagData) + mBufSize * sizeof(u32))
    );
    mpBuf->size = mBufSize;
    // second pass, actually parse
    mBufSize = 0;
    bool ok = loadAndParse(soundId, (1 << 0), true);
    *pOutData = mpBuf;
    return ok;
}

void dSndBgmMmlParserTagBattle_c::NoteOnCommandProc(u32 trackNo, int wait, int key, int velocity, s32 length) const {
    if (key < 0x3C) {
        return;
    }

    if (mpBuf != nullptr) {
        mpBuf->data[mBufSize] = wait;
    }
    mBufSize++;
}

dSndBgmMmlParserTagBgmBoss_c::dSndBgmMmlParserTagBgmBoss_c()
    : dSndBgmMmlParser_c(&dSndMgr_c::GetInstance()->getPlayer(), dSndMgr_c::GetInstance()->getArchive()),
      mpBuf1(nullptr),
      mpBuf2(nullptr),
      mBufSize1(0),
      mBufSize2(0) {}

void dSndBgmMmlParserTagBgmBoss_c::parseData(dSndTagData **pOutData1, dSndTagData **pOutData2) {
    // First pass, check how much memory we need
    loadAndParse(TAG_BGM_BOSS_A, (1 << 0), true);
    // alloc memory
    mpBuf1 = reinterpret_cast<dSndTagData *>(
        dSndMgr_c::GetInstance()->getSoundHeap()->Alloc(sizeof(dSndTagData) + mBufSize1 * sizeof(u32))
    );
    mpBuf2 = reinterpret_cast<dSndTagData *>(
        dSndMgr_c::GetInstance()->getSoundHeap()->Alloc(sizeof(dSndTagData) + mBufSize2 * sizeof(u32))
    );
    mpBuf1->size = mBufSize1;
    mpBuf2->size = mBufSize2;
    // second pass, actually parse
    mBufSize1 = 0;
    mBufSize2 = 0;
    loadAndParse(TAG_BGM_BOSS_A, (1 << 0), true);
    *pOutData1 = mpBuf1;
    *pOutData2 = mpBuf2;
}

void dSndBgmMmlParserTagBgmBoss_c::NoteOnCommandProc(u32 trackNo, int wait, int key, int velocity, s32 length) const {
    switch (key) {
        case 0x3C: {
            if (mpBuf1 != nullptr) {
                mpBuf1->data[mBufSize1] = wait;
            }
            mBufSize1++;
            break;
        }
        case 0x48: {
            if (mpBuf2 != nullptr) {
                mpBuf2->data[mBufSize2] = wait;
            }
            mBufSize2++;
            break;
        }
    }
}

dSndBgmMmlParserTagCompNote_c::dSndBgmMmlParserTagCompNote_c()
    : dSndBgmMmlParser_c(&dSndMgr_c::GetInstance()->getPlayer(), dSndMgr_c::GetInstance()->getArchive()),
      mpBuf(nullptr),
      mBufSize(0) {}

void dSndBgmMmlParserTagCompNote_c::parseData(dSndTagData **pOutData) {
    // First pass, check how much memory we need
    loadAndParse(TAG_BGM_EVENT_COMP_NOTE, (1 << 0), true);
    // alloc memory
    mpBuf = reinterpret_cast<dSndTagData *>(
        dSndMgr_c::GetInstance()->getSoundHeap()->Alloc(sizeof(dSndTagData) + mBufSize * sizeof(u32))
    );
    mpBuf->size = mBufSize;
    // second pass, actually parse
    mBufSize = 0;
    loadAndParse(TAG_BGM_EVENT_COMP_NOTE, (1 << 0), true);
    *pOutData = mpBuf;
}

void dSndBgmMmlParserTagCompNote_c::NoteOnCommandProc(u32 trackNo, int wait, int key, int velocity, s32 length) const {
    if (key < 0x3C) {
        return;
    }

    // TODO - signed/unsigned
    u32 w = wait;
    if (mpBuf != nullptr) {
        if (w != 0) {
            w = w < 0xC80 ? 0 : w - 0xC80;
        }
        mpBuf->data[mBufSize] = w;
    }

    mBufSize++;
}
