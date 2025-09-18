#include "egg/core/eggDvdRipper.h"

#include "rvl/VI.h" // IWYU pragma: export

namespace EGG {

bool DvdRipper::sErrorRetry = true;
DvdRipper::UnkCallback DvdRipper::sCallback;

u8 *DvdRipper::loadToMainRAM(
    s32 entryNum, u8 *dst, Heap *heap, EAllocDirection allocDir, u32 offset, u32 *amountRead, u32 *fileSize
) {
    DvdFile file = DvdFile();
    if (!file.open(entryNum)) {
        return nullptr;
    }
    return loadToMainRAM(&file, dst, heap, allocDir, offset, amountRead, fileSize);
}

u8 *DvdRipper::loadToMainRAM(
    const char *path, u8 *dst, Heap *heap, EAllocDirection allocDir, u32 offset, u32 *amountRead, u32 *fileSize
) {
    DvdFile file = DvdFile();
    if (!file.open(path)) {
        return nullptr;
    }
    return loadToMainRAM(&file, dst, heap, allocDir, offset, amountRead, fileSize);
}

u8 *DvdRipper::loadToMainRAM(
    DvdFile *pFile, u8 *pOut, Heap *pHeap, EAllocDirection allocDir, u32 offset, u32 *pRead, u32 *pSize
) {
    u32 size;
    bool memAllocated = false;
    u32 alignedSize;
    s32 result;
    u8 buf[0x40];

    // msg could be a buffer, it could be a completely different structure,
    // there is no good way to know. (callback is never set D: )
    // Issue with the callback argument not being placed on the stack properly
    if (sCallback) {
        UnkCallback_Arg cb_arg;
        cb_arg.UNK_0x04 = 0;
        cb_arg.UNK_0x08 = 0;
        cb_arg.UNK_0x0C = 1;
        cb_arg.UNK_0x10 = 0;
        cb_arg.buf = &buf;
        sCallback(&cb_arg);
    }

    size = pFile->getFileSize();
    if (pSize) {
        *pSize = size;
    }
    alignedSize = nw4r::ut::RoundUp(size, 0x20);

    if (!pOut) {
        u32 size = alignedSize - offset;
        pOut = (u8 *)Heap::alloc(size, allocDir == ALLOC_DIR_TOP ? 0x20 : -0x20, pHeap);
        memAllocated = true;
    }
    if (pOut == nullptr) {
        return nullptr;
    }

    if (offset != 0) {
        // Fakematch???? This just feels weird
        void *ptr = ROUND_UP_PTR(&buf[8], 0x20);
        while (true) {
            result = DVDReadPrio(&pFile->mFileInfo, ptr, 0x20, offset, DVD_PRIO_MEDIUM);
            if (result >= DVD_RESULT_OK) {
                break;
            }

            if (result == DVD_RESULT_CANCELED || !sErrorRetry) {
                if (memAllocated) {
                    Heap::free(pOut, nullptr);
                }
                return nullptr;
            }

            VIWaitForRetrace();
        }
        DCInvalidateRange(ptr, 0x20);
    }
    while (true) {
        result = DVDReadPrio(&pFile->mFileInfo, pOut, alignedSize - offset, offset, DVD_PRIO_MEDIUM);
        if (result >= DVD_RESULT_OK) {
            break;
        }
        if (result == DVD_RESULT_CANCELED || !sErrorRetry) {
            if (memAllocated) {
                Heap::free(pOut, nullptr);
            }
            return nullptr;
        }
        VIWaitForRetrace();
    }
    if (pRead) {
        *pRead = alignedSize - offset;
    }

    return pOut;
}

void *DvdRipper::loadToMainRAMDecomp(
    DvdFile *file, StreamDecomp *decompressor, u8 *pOut, Heap *heap, EAllocDirection allocDir, s32 offset, u32 size,
    u32 chunkSize, u32 *pRead, u32 *pSize
) {
    s32 uncomp_align = allocDir == ALLOC_DIR_TOP ? 0x20 : -0x20;
    s32 align = allocDir == ALLOC_DIR_TOP ? -0x20 : 0x20;

    // Ouput size of file
    s32 file_size = file->getFileSize();
    if (pSize) {
        *pSize = file_size;
    }

    // Setup amount to read
    s32 max_read = ROUND_UP(file->getFileSize(), 0x20) - offset;
    if (size != 0) {
        max_read = size;
    }

    void *read_buffer = heap->alloc(decompressor->getHeaderSize(), align);

    if (!read_buffer) {
        return nullptr;
    }

    s32 result = DVDReadPrio(&file->mFileInfo, read_buffer, decompressor->getHeaderSize(), offset, DVD_PRIO_MEDIUM);
    file_size = decompressor->getHeaderSize();

    if (result == file_size) {
        file_size = decompressor->getUncompressedSize(read_buffer);
    } else {
        // BUG? - Read buffer never freed
        return nullptr;
    }
    heap->free(read_buffer);
    read_buffer = nullptr;

    // Already Read header to get size
    if (pRead) {
        *pRead = decompressor->getHeaderSize();
    }

    // Ensure decompression space
    if (!pOut) {
        pOut = (u8 *)heap->alloc(file_size, uncomp_align);
    }

    if (!pOut) {
        return nullptr;
    }

    // Read and decompress the chunks
    void *chunk_buffer = heap->alloc(chunkSize, align);
    if (chunk_buffer) {
        s32 file_size = 0;
        decompressor->init(pOut, max_read);

        while (true) {
            u32 chunk_size = MIN(max_read - file_size, chunkSize);
            u32 chunk_offset = offset + file_size;

            s32 result =
                DVDReadPrio(&file->mFileInfo, chunk_buffer, ROUND_UP(chunk_size, 0x20), chunk_offset, DVD_PRIO_MEDIUM);

            if (result < DVD_RESULT_OK || decompressor->decomp(chunk_buffer, result)) {
                break;
            }
            file_size += result;
        }
        heap->free(chunk_buffer);
        if (pRead) {
            *pRead = file_size + decompressor->getHeaderSize();
        }
    }

    return pOut;
}

} // namespace EGG
