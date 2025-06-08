#ifndef EGG_DVD_RIPPER_H
#define EGG_DVD_RIPPER_H

#include "common.h"
#include "egg/core/eggDvdFile.h"
#include "egg/core/eggHeap.h"
#include "egg/core/eggStreamDecomp.h"

namespace EGG {

class DvdRipper {
public:
    enum EAllocDirection {
        ALLOC_CIR_PAD,
        ALLOC_DIR_TOP,
        ALLOC_DIR_BOTTOM
    };

    struct UnkCallback_Arg {
        u8 UNK_0x00[4];
        u32 UNK_0x04;
        u32 UNK_0x08;
        u32 UNK_0x0C;
        u32 UNK_0x10;
        void *buf;
    };
    typedef void (*UnkCallback)(void *);

public:
    // Not sure
    struct Arg {
        Arg(const char *path, u8 *pOut, Heap *pHeap, EAllocDirection allocDir, u32 offset, u32 *pRead, u32 *fileSize)
            : path(path),
              pOut(pOut),
              pHeap(pHeap),
              allocDir(allocDir),
              offset(offset),
              pRead(pRead),
              fileSize(fileSize) {}
        const char *path;
        u8 *pOut;
        Heap *pHeap;
        EAllocDirection allocDir;
        u32 offset;
        u32 *pRead;
        u32 *fileSize;
    };

    static u8 *loadToMainRAM(Arg &arg) {
        return loadToMainRAM(arg.path, arg.pOut, arg.pHeap, arg.allocDir, arg.offset, arg.pRead, arg.fileSize);
    }

    static u8 *
    loadToMainRAM(s32 entryNum, u8 *pOut, Heap *pHeap, EAllocDirection allocDir, u32 offset, u32 *pRead, u32 *fileSize);

    static u8 *loadToMainRAM(
        const char *path, u8 *pOut, Heap *pHeap, EAllocDirection allocDir, u32 offset, u32 *pRead, u32 *fileSize
    );

    static u8 *loadToMainRAM(
        DvdFile *pFile, u8 *pOut, Heap *pHeap, EAllocDirection allocDir, u32 offset, u32 *pRead, u32 *fileSize
    );

    static void *loadToMainRAMDecomp(
        DvdFile *pFile, StreamDecomp *decompressor, u8 *pOut, Heap *pHeap, EAllocDirection allocDir, s32 offset,
        u32 size, u32 chunkSize, u32 *pRead, u32 *fileSize
    );

public:
    static bool sErrorRetry;
    static UnkCallback sCallback;
};

} // namespace EGG

#endif
