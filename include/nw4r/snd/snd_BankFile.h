#ifndef NW4R_SND_BANK_FILE_H
#define NW4R_SND_BANK_FILE_H
#include <nw4r/types_nw4r.h>

#include <nw4r/snd/snd_Util.h>
#include <nw4r/snd/snd_WaveFile.h>

#include <nw4r/ut.h>

namespace nw4r {
namespace snd {
namespace detail {
namespace BankFile {

struct InstParam {
    s32 waveIndex; // at 0x0
    u8 attack;     // at 0x4
    u8 decay;      // at 0x5
    u8 sustain;    // at 0x6
    u8 release;    // at 0x7
    char UNK_0x8[0x4];
    u8 originalKey; // at 0xC
    u8 volume;      // at 0xD
    u8 pan;         // at 0xE
    f32 tune;       // at 0x10
};

struct RangeTable {
    u8 tableSize; // at 0x0
    u8 key[];     // at 0x1
};

struct IndexTable {
    u8 min;       // at 0x0
    u8 max;       // at 0x1
    u16 reserved; // at 0x2
    u8 ref[];     // at 0x4
};

typedef Util::DataRef<void, InstParam, RangeTable, IndexTable> DataRegion;
typedef Util::DataRef<WaveFile::WaveInfo> WaveRegion;

struct Header {
    ut::BinaryFileHeader fileHeader; // at 0x0
    u32 dataBlockOffset;             // at 0x10
    u32 dataBlockSize;               // at 0x14
    u32 waveBlockOffset;             // at 0x18
    u32 waveBlockSize;               // at 0x1C
};

struct DataBlock {
    ut::BinaryBlockHeader blockHeader; // at 0x0
    Util::Table<DataRegion> instTable; // at 0x8
};

struct WaveBlock {
    ut::BinaryBlockHeader blockHeader;     // at 0x0
    Util::Table<WaveRegion> waveInfoTable; // at 0x8
};

} // namespace BankFile

struct InstInfo {
    s32 waveIndex;  // at 0x0
    u8 attack;      // at 0x4
    u8 decay;       // at 0x5
    u8 sustain;     // at 0x6
    u8 release;     // at 0x7
    u8 originalKey; // at 0x8
    u8 pan;         // at 0x9
    u8 volume;      // at 0xA
    f32 tune;       // at 0xC
};

inline u8 ReadByte(const void* pAddr) {
    return *static_cast<const u8*>(pAddr);
}

class BankFileReader {
public:
    static const u32 SIGNATURE = 'RBNK';
    static const int VERSION = NW4R_VERSION(1, 1);

public:
    explicit BankFileReader(const void* pBankBin);

    bool IsValidFileHeader(const void* pBankBin);

    bool ReadInstInfo(InstInfo* pInfo, int prgNo, int key, int velocity) const;
    bool ReadWaveParam(WaveData* pData, int waveIndex,
                       const void* pWaveAddr) const;

private:
    const BankFile::DataRegion*
    GetReferenceToSubRegion(const BankFile::DataRegion* pRef,
                            int splitKey) const;

private:
    const BankFile::Header* mHeader;       // at 0x0
    const BankFile::DataBlock* mDataBlock; // at 0x4
    const BankFile::WaveBlock* mWaveBlock; // at 0x8
};

} // namespace detail
} // namespace snd
} // namespace nw4r

#endif
