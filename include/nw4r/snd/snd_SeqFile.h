#ifndef NW4R_SND_SEQ_FILE_H
#define NW4R_SND_SEQ_FILE_H
#include <nw4r/types_nw4r.h>

#include <nw4r/ut.h>

namespace nw4r {
namespace snd {
namespace detail {
namespace SeqFile {

struct Header {
    ut::BinaryFileHeader fileHeader; // at 0x0
    u32 dataBlockOffset;             // at 0x10
    u32 dataBlockSize;               // at 0x14
    u32 labelBlockOffset;            // at 0x18
    u32 labelBlockSize;              // at 0x1C
};

struct DataBlock {
    ut::BinaryBlockHeader blockHeader; // at 0x0
    u32 baseOffset;                    // at 0x8
};

} // namespace SeqFile

class SeqFileReader {
public:
    static const u32 SIGNATURE = 'RSEQ';
    static const int VERSION = NW4R_VERSION(1, 0);

public:
    explicit SeqFileReader(const void* pSeqBin);

    bool IsValidFileHeader(const void* pSeqBin);

    const void* GetBaseAddress() const;

private:
    const SeqFile::Header* mHeader;       // at 0x0
    const SeqFile::DataBlock* mDataBlock; // at 0x4
};

} // namespace detail
} // namespace snd
} // namespace nw4r

#endif
