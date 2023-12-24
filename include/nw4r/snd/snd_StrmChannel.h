#ifndef NW4R_SND_STRMCHANNEL_H
#define NW4R_SND_STRMCHANNEL_H
#include "types_nw4r.h"

#define BLOCK_MAX 0x20

namespace nw4r
{
    namespace snd
    {
        namespace detail
        {
            struct StrmBufferPool
            {
                void Setup(void *, u32, int);
                void Shutdown();
                void * Alloc();
                void Free(void *);

                void *mBuffer; // at 0x0
                u32 mStrmBufferSize; // at 0x4
                u32 mBlockSize; // at 0x8
                s32 mBlockCount; // at 0xC (< BLOCK_MAX)
                s32 mAllocCount; // at 0x10
                u8 mAllocFlags[4]; // at 0x14 (1 bit per block)
            };
        }
    }
}

#endif