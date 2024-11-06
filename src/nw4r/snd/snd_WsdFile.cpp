#include "nw4r/snd/snd_WsdFile.h"
#include "nw4r/snd/snd_WaveFile.h"

namespace nw4r {
namespace snd {
namespace detail {

bool WsdFileReader::IsValidFileHeader(const void* pWsdBin) {
    const ut::BinaryFileHeader* pFileHeader =
        static_cast<const ut::BinaryFileHeader*>(pWsdBin);

    if (pFileHeader->signature != SIGNATURE) {
        return false;
    }

    if (pFileHeader->version < NW4R_VERSION(1, 0)) {
        return false;
    }

    if (pFileHeader->version > VERSION) {
        return false;
    }

    return true;
}

WsdFileReader::WsdFileReader(const void* pWsdBin)
    : mHeader(NULL), mDataBlock(NULL), mWaveBlock(NULL) {
    if (!IsValidFileHeader(pWsdBin)) {
        return;
    }

    mHeader = static_cast<const WsdFile::Header*>(pWsdBin);

    if (mHeader->dataBlockOffset != 0) {
        mDataBlock = static_cast<const WsdFile::DataBlock*>(
            ut::AddOffsetToPtr(mHeader, mHeader->dataBlockOffset));
    }

    if (mHeader->waveBlockOffset != 0) {
        mWaveBlock = static_cast<const WsdFile::WaveBlock*>(
            ut::AddOffsetToPtr(mHeader, mHeader->waveBlockOffset));
    }
}

bool WsdFileReader::ReadWaveSoundInfo(WaveSoundInfo* pSoundInfo, int id) const {
    const WsdFile::Wsd* pWsd =
        Util::GetDataRefAddress0(mDataBlock->refWsd[id], &mDataBlock->wsdCount);

    const WsdFile::WsdInfo* pWsdInfo =
        Util::GetDataRefAddress0(pWsd->refWsdInfo, &mDataBlock->wsdCount);

    if (mHeader->fileHeader.version >= NW4R_VERSION(1, 2)) {
        pSoundInfo->pitch = pWsdInfo->pitch;
        pSoundInfo->pan = pWsdInfo->pan;
        pSoundInfo->surroundPan = pWsdInfo->surroundPan;
        pSoundInfo->fxSendA = pWsdInfo->fxSendA;
        pSoundInfo->fxSendB = pWsdInfo->fxSendB;
        pSoundInfo->fxSendC = pWsdInfo->fxSendC;
        pSoundInfo->mainSend = pWsdInfo->mainSend;
    } else if (mHeader->fileHeader.version >= NW4R_VERSION(1, 1)) {
        pSoundInfo->pitch = pWsdInfo->pitch;
        pSoundInfo->pan = pWsdInfo->pan;
        pSoundInfo->surroundPan = pWsdInfo->surroundPan;
        pSoundInfo->fxSendA = 0;
        pSoundInfo->fxSendB = 0;
        pSoundInfo->fxSendC = 0;
        pSoundInfo->mainSend = 127;
    } else {
        pSoundInfo->pitch = 1.0f;
        pSoundInfo->pan = 64;
        pSoundInfo->surroundPan = 0;
        pSoundInfo->fxSendA = 0;
        pSoundInfo->fxSendB = 0;
        pSoundInfo->fxSendC = 0;
        pSoundInfo->mainSend = 127;
    }

    return true;
}

bool WsdFileReader::ReadWaveSoundNoteInfo(WaveSoundNoteInfo* pSoundNoteInfo,
                                          int id, int note) const {
    const WsdFile::Wsd* pWsd =
        Util::GetDataRefAddress0(mDataBlock->refWsd[id], &mDataBlock->wsdCount);

    const WsdFile::NoteTable* pTable =
        Util::GetDataRefAddress0(pWsd->refNoteTable, &mDataBlock->wsdCount);

    const WsdFile::NoteInfo* pNoteInfo =
        Util::GetDataRefAddress0(pTable->items[note], &mDataBlock->wsdCount);

    pSoundNoteInfo->waveIndex = pNoteInfo->waveIndex;
    pSoundNoteInfo->attack = pNoteInfo->attack;
    pSoundNoteInfo->decay = pNoteInfo->decay;
    pSoundNoteInfo->sustain = pNoteInfo->sustain;
    pSoundNoteInfo->release = pNoteInfo->release;
    pSoundNoteInfo->originalKey = pNoteInfo->originalKey;
    pSoundNoteInfo->volume = pNoteInfo->volume;

    if (mHeader->fileHeader.version >= NW4R_VERSION(1, 1)) {
        pSoundNoteInfo->pan = pNoteInfo->pan;
        pSoundNoteInfo->surroundPan = pNoteInfo->surroundPan;
        pSoundNoteInfo->pitch = pNoteInfo->pitch;
    } else {
        pSoundNoteInfo->pan = 64;
        pSoundNoteInfo->surroundPan = 0;
        pSoundNoteInfo->pitch = 1.0f;
    }

    return true;
}

bool WsdFileReader::ReadWaveParam(int id, WaveInfo* pWaveData,
                                  const void* pWaveAddr) const {
    const WaveFile::WaveInfo* pWaveInfo;

    if (mHeader->fileHeader.version == NW4R_VERSION(1, 0)) {
        const WsdFile::WaveBlockOld* pWaveBlockOld =
            reinterpret_cast<const WsdFile::WaveBlockOld*>(mWaveBlock);

        pWaveInfo = static_cast<const WaveFile::WaveInfo*>(
            ut::AddOffsetToPtr(pWaveBlockOld, pWaveBlockOld->offsetTable[id]));
    } else {
        if (id >= mWaveBlock->waveCount) {
            return false;
        }

        pWaveInfo = static_cast<const WaveFile::WaveInfo*>(
            ut::AddOffsetToPtr(mWaveBlock, mWaveBlock->offsetTable[id]));
    }

    WaveFileReader reader(pWaveInfo);
    return reader.ReadWaveParam(pWaveData, pWaveAddr);
}

} // namespace detail
} // namespace snd
} // namespace nw4r
