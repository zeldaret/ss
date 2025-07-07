#include "d/snd/d_snd_bgm_mml_parsers.h"

#include "common.h"
#include "d/snd/d_snd_mgr.h"
#include "d/snd/d_snd_bgm_sound_harp_mgr.h"
#include "d/snd/d_snd_bgm_seq_data_mgr.h"
#include "d/snd/d_snd_player_mgr.h"
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
    if (dataId != -1 && dSndMgr_c::GetInstance()->getArchive()->GetSoundType(dataId) == nw4r::snd::SoundArchive::SOUND_TYPE_SEQ) {
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
        case 15:
            field_0x298->addVar(wait, key, velocity);
            break;
        default:
            break;

    }
}
