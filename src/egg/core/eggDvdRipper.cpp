#include <egg/core/eggDvdRipper.h>
#include <rvl/VI.h>

namespace EGG {

bool DvdRipper::sErrorRetry = true;
DvdRipper::UnkCallback DvdRipper::sCallback;

/* 80494680 */
u8 *DvdRipper::loadToMainRAM(s32 entryNum, u8 *dst, Heap *heap, EAllocDirection allocDir, u32 offset, u32 *amountRead,
    u32 *fileSize) {
    DvdFile file = DvdFile();
    if (!file.open(entryNum)) {
        return nullptr;
    }
    return loadToMainRAM(&file, dst, heap, allocDir, offset, amountRead, fileSize);
}

/* 80494730 */
u8 *DvdRipper::loadToMainRAM(const char *path, u8 *dst, Heap *heap, EAllocDirection allocDir, u32 offset,
    u32 *amountRead, u32 *fileSize) {
    DvdFile file = DvdFile();
    if (!file.open(path)) {
        return nullptr;
    }
    return loadToMainRAM(&file, dst, heap, allocDir, offset, amountRead, fileSize);
}

/* 804947e0 */
u8 *DvdRipper::loadToMainRAM(DvdFile *pFile, u8 *pOut, Heap *pHeap, EAllocDirection allocDir, u32 offset, u32 *pRead,
    u32 *pSize) {
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

/* 804949b0 */
void *DvdRipper::loadToMainRAMDecomp(DvdFile *file, StreamDecomp *decompressor, u8 *pOut, Heap *heap,
    EAllocDirection allocDir, s32 offset, u32 size, u32 chunkSize, u32 *pRead, u32 *pSize) {
    // TODO Regswaps

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
    s32 read_size = decompressor->getHeaderSize();

    if (result == read_size) {
        read_size = decompressor->getUncompressedSize(read_buffer);
    } else {
        // BUG - Read buffer never freed
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
        pOut = (u8 *)heap->alloc(read_size, uncomp_align);
    }

    if (!pOut) {
        return nullptr;
    }

    // Read and decompress the chunks
    read_buffer = heap->alloc(chunkSize, align);
    if (read_buffer) {
        read_size = 0;
        decompressor->init(pOut, max_read);

        while (true) {
            u32 chunk_size = MIN(max_read - read_size, chunkSize);
            u32 chunk_offset = offset + read_size;

            result =
                DVDReadPrio(&file->mFileInfo, read_buffer, ROUND_UP(chunk_size, 0x20), chunk_offset, DVD_PRIO_MEDIUM);

            if (result < DVD_RESULT_OK || decompressor->decomp(read_buffer, result)) {
                break;
            }
            read_size += result;
        }
        heap->free(read_buffer);
        if (pRead) {
            *pRead = read_size + decompressor->getHeaderSize();
        }
    }

    return pOut;
}

} // namespace EGG
