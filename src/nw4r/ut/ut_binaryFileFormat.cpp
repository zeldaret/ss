#include "nw4r/ut/ut_binaryFileFormat.h"

namespace nw4r {
namespace ut {

/* 8042a9e0 */
bool IsValidBinaryFile(const BinaryFileHeader *header, u32 magic, u16 version, u16 numBlocks) {
    if (header->signature != magic) {
        return false;
    }
    if (header->byteOrder != 0xFEFF) {
        return false;
    }
    if (header->version != version) {
        return false;
    }
    if (header->fileSize < (numBlocks * sizeof(BinaryBlockHeader) + sizeof(BinaryFileHeader))) {
        return false;
    }

    if (header->numBlocks < numBlocks) {
        return false;
    }
    return true;
}

} // namespace ut
} // namespace nw4r
