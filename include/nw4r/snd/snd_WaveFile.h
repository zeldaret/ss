#ifndef NW4R_SND_WAVE_FILE_H
#define NW4R_SND_WAVE_FILE_H
#include <nw4r/types_nw4r.h>

#include <nw4r/snd/snd_AxVoice.h>
#include <nw4r/snd/snd_Common.h>

#include <nw4r/ut.h>

namespace nw4r {
namespace snd {
namespace detail {
namespace WaveFile {

enum Format { FORMAT_PCM8, FORMAT_PCM16, FORMAT_ADPCM };

struct WaveInfo {
    u8 format;                  // at 0x0
    u8 loopFlag;                // at 0x1
    u8 numChannels;             // at 0x2
    u8 sampleRate24;            // at 0x3
    u16 sampleRate;             // at 0x4
    u16 padding2;               // at 0x6
    u32 loopStart;              // at 0x8
    u32 loopEnd;                // at 0xC
    u32 channelInfoTableOffset; // at 0x10
    u32 dataOffset;             // at 0x14
    u32 reserved;               // at 0x18
};

struct WaveChannelInfo {
    u32 channelDataOffset; // at 0x0
    u32 adpcmOffset;       // at 0x4
    u32 volumeFrontLeft;   // at 0x8
    u32 volumeFrontRight;  // at 0xC
    u32 volumeRearLeft;    // at 0x10
    u32 volumeRearRight;   // at 0x14
    u32 reserved;          // at 0x18
};

}; // namespace WaveFile

struct ChannelParam {
    void* dataAddr;       // at 0x0
    u32 volumeFrontLeft;  // at 0x4
    u32 volumeFrontRight; // at 0x8
    u32 volumeRearLeft;   // at 0xC
    u32 volumeRearRight;  // at 0x10
    AdpcmInfo adpcmInfo;  // at 0x14
};

struct WaveData {
    u8 sampleFormat;                        // at 0x0
    u8 loopFlag;                            // at 0x1
    u8 numChannels;                         // at 0x2
    u32 sampleRate;                         // at 0x4
    u32 loopStart;                          // at 0x8
    u32 loopEnd;                            // at 0xC
    ChannelParam channelParam[CHANNEL_MAX]; // at 0x10
};

class WaveFileReader {
public:
    explicit WaveFileReader(const WaveFile::WaveInfo* pWaveInfo);

    bool ReadWaveParam(WaveData* pWaveData, const void* pWaveAddr) const;

    static AxVoice::Format GetAxVoiceFormatFromWaveFileFormat(u32 format);

private:
    const WaveFile::WaveInfo* mWaveInfo; // at 0x0
};

inline AxVoice::Format WaveFormatToAxFormat(u32 format) {
    if (format == WaveFile::FORMAT_PCM16) {
        return AxVoice::FORMAT_PCM16;
    }

    if (format == WaveFile::FORMAT_PCM8) {
        return AxVoice::FORMAT_PCM8;
    }

    return AxVoice::FORMAT_ADPCM;
}

} // namespace detail
} // namespace snd
} // namespace nw4r

#endif
