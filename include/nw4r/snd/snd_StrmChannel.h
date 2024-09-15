#ifndef NW4R_SND_STRM_CHANNEL_H
#define NW4R_SND_STRM_CHANNEL_H
#include <nw4r/types_nw4r.h>

#include <nw4r/snd/snd_Common.h>

namespace nw4r {
namespace snd {
namespace detail {

struct StrmChannel {
    void* bufferAddress; // at 0x0
    u32 bufferSize;      // at 0x4
    AdpcmInfo adpcmInfo; // at 0x8
};

class StrmBufferPool {
public:
    void Setup(void* pBase, u32 size, int count);
    void Shutdown();

    void* Alloc();
    void Free(void* pBuffer);

    u32 GetBlockSize() const {
        return mBlockSize;
    }

private:
    static const int BLOCK_MAX = 32;
    static const int BITS_PER_BYTE = 8;

private:
    void* mBuffer;   // at 0x0
    u32 mBufferSize; // at 0x4

    u32 mBlockSize;  // at 0x8
    int mBlockCount; // at 0xC

    int mAllocCount;                           // at 0x10
    u8 mAllocFlags[BLOCK_MAX / BITS_PER_BYTE]; // at 0x14
};

} // namespace detail
} // namespace snd
} // namespace nw4r

#endif
