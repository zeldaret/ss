#ifndef NW4R_SND_STRM_FILE_H
#define NW4R_SND_STRM_FILE_H
#include <nw4r/types_nw4r.h>

#include <nw4r/snd/snd_Common.h>
#include <nw4r/snd/snd_Util.h>

#include <nw4r/ut.h>

namespace nw4r {
namespace snd {
namespace detail {
namespace StrmFile {

struct StrmDataInfo {
    u8 format;               // at 0x0
    u8 loopFlag;             // at 0x1
    u8 numChannels;          // at 0x2
    u8 sampleRate24;         // at 0x3
    u16 sampleRate;          // at 0x4
    u16 blockHeaderOffset;   // at 0x6
    u32 loopStart;           // at 0x8
    u32 loopEnd;             // at 0xC
    u32 dataOffset;          // at 0x10
    u32 numBlocks;           // at 0x14
    u32 blockSize;           // at 0x18
    u32 blockSamples;        // at 0x1C
    u32 lastBlockSize;       // at 0x20
    u32 lastBlockSamples;    // at 0x24
    u32 lastBlockPaddedSize; // at 0x28
    u32 adpcmDataInterval;   // at 0x2C
    u32 adpcmDataSize;       // at 0x30
};

struct TrackInfo {
    u8 channelCount;        // at 0x0
    u8 channelIndexTable[]; // at 0x1
};

struct TrackTable {
    u8 trackCount;                             // at 0x0
    u8 trackDataType;                          // at 0x1
    Util::DataRef<TrackInfo> refTrackHeader[]; // at 0x4
};

struct ChannelInfo {
    Util::DataRef<AdpcmInfo> refAdpcmInfo; // at 0x0
};

struct ChannelTable {
    u8 channelCount;                               // at 0x0
    Util::DataRef<ChannelInfo> refChannelHeader[]; // at 0x4
};

struct Header {
    ut::BinaryFileHeader fileHeader; // at 0x0
    u32 headBlockOffset;             // at 0x10
    u32 headBlockSize;               // at 0x14
    u32 adpcBlockOffset;             // at 0x18
    u32 adpcBlockSize;               // at 0x1C
    u32 dataBlockOffset;             // at 0x20
    u32 dataBlockSize;               // at 0x24
};

struct HeadBlock {
    ut::BinaryBlockHeader blockHeader;           // at 0x0
    Util::DataRef<StrmDataInfo> refDataHeader;   // at 0x8
    Util::DataRef<TrackTable> refTrackTable;     // at 0x10
    Util::DataRef<ChannelTable> refChannelTable; // at 0x18
};

} // namespace StrmFile

struct StrmInfo {
    u8 format;               // at 0x0
    u8 loopFlag;             // at 0x1
    u8 numChannels;          // at 0x2
    int sampleRate;          // at 0x4
    u16 blockHeaderOffset;   // at 0x8
    u32 loopStart;           // at 0xC
    u32 loopEnd;             // at 0x10
    u32 dataOffset;          // at 0x14
    u32 numBlocks;           // at 0x18
    u32 blockSize;           // at 0x1C
    u32 blockSamples;        // at 0x20
    u32 lastBlockSize;       // at 0x24
    u32 lastBlockSamples;    // at 0x28
    u32 lastBlockPaddedSize; // at 0x2C
    u32 adpcmDataInterval;   // at 0x30
    u32 adpcmDataSize;       // at 0x34
};

class StrmFileReader {
public:
    static const u32 SIGNATURE = 'RSTM';
    static const int VERSION = NW4R_VERSION(1, 0);

public:
    StrmFileReader();

    bool IsAvailable() const {
        return mHeader != NULL;
    }

    bool IsValidFileHeader(const void* pStrmBin);
    void Setup(const void* pStrmBin);

    bool ReadStrmInfo(StrmInfo* pStrmInfo) const;
    bool ReadAdpcmInfo(AdpcmInfo* pAdpcmInfo, int channels) const;

    u32 GetAdpcBlockOffset() const {
        if (IsAvailable()) {
            return mHeader->adpcBlockOffset;
        }

        return 0;
    }

private:
    const StrmFile::Header* mHeader;       // at 0x0
    const StrmFile::HeadBlock* mHeadBlock; // at 0x4
};

class StrmFileLoader {
public:
    explicit StrmFileLoader(ut::FileStream& rFileStream)
        : mStream(rFileStream) {}

    bool LoadFileHeader(void* pStrmBin, u32 size);
    bool ReadAdpcBlockData(u16* pYN1, u16* pYN2, int block, int channels);

    bool ReadStrmInfo(StrmInfo* pStrmInfo) const {
        if (!mReader.IsAvailable()) {
            return false;
        }

        return mReader.ReadStrmInfo(pStrmInfo);
    }

    bool ReadAdpcmInfo(AdpcmInfo* pAdpcmInfo, int channel) const {
        if (!mReader.IsAvailable()) {
            return false;
        }

        return mReader.ReadAdpcmInfo(pAdpcmInfo, channel);
    }

private:
    static const int HEADER_ALIGNED_SIZE =
        ROUND_UP(sizeof(StrmFile::Header), 32);

private:
    ut::FileStream& mStream; // at 0x0
    StrmFileReader mReader;  // at 0x4
};

} // namespace detail
} // namespace snd
} // namespace nw4r

#endif
