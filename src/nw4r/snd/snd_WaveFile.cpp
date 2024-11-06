#include "nw4r/snd/snd_WaveFile.h"

namespace nw4r {
namespace snd {
namespace detail {

WaveFileReader::WaveFileReader(const WaveFile::WaveInfo* pWaveInfo)
    : mWaveInfo(pWaveInfo) {}

bool WaveFileReader::ReadWaveParam(WaveInfo* pWaveData,
                                   const void* pWaveAddr) const {
    SampleFormat format =
        GetAxVoiceFormatFromWaveFileFormat(mWaveInfo->format);

    pWaveData->sampleFormat = mWaveInfo->format;
    pWaveData->numChannels = mWaveInfo->numChannels;
    pWaveData->sampleRate =
        (mWaveInfo->sampleRate24 << 16) + mWaveInfo->sampleRate;

    pWaveData->loopFlag = mWaveInfo->loopFlag;
    pWaveData->loopStart =
        AxVoice::GetSampleByDspAddress(NULL, mWaveInfo->loopStart, format);
    pWaveData->loopEnd =
        AxVoice::GetSampleByDspAddress(NULL, mWaveInfo->loopEnd, format) + 1;

    const u32* pInfoOffsetTable = static_cast<const u32*>(
        ut::AddOffsetToPtr(mWaveInfo, mWaveInfo->channelInfoTableOffset));

    for (int i = 0; i < mWaveInfo->numChannels; i++) {
        ChannelParam& rParam = pWaveData->channelParam[i];

        WaveFile::WaveChannelInfo* pChannelInfo =
            const_cast<WaveFile::WaveChannelInfo*>(
                ut::AddOffsetToPtr(mWaveInfo, pInfoOffsetTable[i]));

        rParam.volumeFrontLeft = pChannelInfo->volumeFrontLeft;
        rParam.volumeFrontRight = pChannelInfo->volumeFrontRight;
        rParam.volumeRearLeft = pChannelInfo->volumeRearLeft;
        rParam.volumeRearRight = pChannelInfo->volumeRearRight;

        if (pChannelInfo->adpcmOffset != 0) {
            const AdpcmInfo* pAdpcmInfo = static_cast<const AdpcmInfo*>(
                ut::AddOffsetToPtr(mWaveInfo, pChannelInfo->adpcmOffset));

            rParam.adpcmInfo = *pAdpcmInfo;
        }

        rParam.dataAddr = const_cast<void*>(
            ut::AddOffsetToPtr(pWaveAddr, mWaveInfo->dataOffset +
                                              pChannelInfo->channelDataOffset));
    }

    return true;
}

SampleFormat WaveFileReader::GetAxVoiceFormatFromWaveFileFormat(u32 format) {
    if (format == WaveFile::FORMAT_PCM16) {
        return SAMPLE_FORMAT_PCM_S16;
    }

    if (format == WaveFile::FORMAT_PCM8) {
        return SAMPLE_FORMAT_PCM_S8;
    }

    return SAMPLE_FORMAT_DSP_ADPCM;
}

} // namespace detail
} // namespace snd
} // namespace nw4r
