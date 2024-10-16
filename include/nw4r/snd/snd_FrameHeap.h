#ifndef NW4R_SND_FRAME_HEAP_H
#define NW4R_SND_FRAME_HEAP_H
#include "nw4r/types_nw4r.h" // IWYU pragma: export
#include "nw4r/ut.h"         // IWYU pragma: export
#include "rvl/MEM.h"         // IWYU pragma: export

namespace nw4r {
namespace snd {
namespace detail {

class FrameHeap {
public:
    typedef void (*FreeCallback)(void *pBuffer, u32 size, void *pCallbackArg);

public:
    FrameHeap();
    ~FrameHeap();

    bool Create(void *pBase, u32 size);
    void Destroy();
    void Clear();
    void *Alloc(u32 size, FreeCallback pCallback, void *pCallbackArg);

    int SaveState();
    void LoadState(int id);

    int GetCurrentLevel() const;
    u32 GetFreeSize() const;

    bool IsValid() const {
        return mHandle != NULL;
    }

private:
    struct Block {
        NW4R_UT_LIST_NODE_DECL(); // at 0x0
        u32 mSize;                // at 0x8
        FreeCallback mCallback;   // at 0xc
        void *mCallbackArg;       // at 0x10

        Block(u32 size, FreeCallback pCallback, void *pCallbackArg)
            : mSize(size), mCallback(pCallback), mCallbackArg(pCallbackArg) {}

        ~Block() {
            if (mCallback != NULL) {
                mCallback(GetBufferAddr(), mSize, mCallbackArg);
            }
        }

        void *GetBufferAddr() {
            return ut::AddOffsetToPtr(this, BLOCK_BUFFER_SIZE);
        }
    };

    NW4R_UT_LIST_TYPEDEF_DECL(Block);

    struct Section {
        NW4R_UT_LIST_NODE_DECL(); // at 0x0
        BlockList mBlockList;     // at 0x8

        ~Section() {
            for (BlockList::Iterator it = mBlockList.GetEndIter(); it != mBlockList.GetBeginIter();) {
                (--it)->~Block();
            }
        }

        void AppendBlock(Block *pBlock) {
            mBlockList.PushBack(pBlock);
        }
    };

    NW4R_UT_LIST_TYPEDEF_DECL(Section);

    static const int BLOCK_BUFFER_SIZE = ROUND_UP(sizeof(Block), 32);
    static const int HEAP_ALIGN = 32;

private:
    bool NewSection();
    void ClearSection();

private:
    MEMiHeapHead *mHandle;    // at 0x0
    SectionList mSectionList; // at 0x4
};

} // namespace detail
} // namespace snd
} // namespace nw4r

#endif
