#include "nw4r/snd/snd_NandSoundArchive.h"
#include <cstring>
#include <new>

namespace nw4r {
namespace snd {

class NandSoundArchive::NandFileStream : public ut::NandFileStream {
public:
    NandFileStream(const NANDFileInfo* pFileInfo, u32 offset, u32 size);
    NandFileStream(const char* pPath, u32 offset, u32 size);

    virtual s32 Read(void* pDst, u32 size);    // at 0x14
    virtual void Seek(s32 offset, u32 origin); // at 0x44

    virtual u32 Tell() const {
        return ut::NandFileStream::Tell() - mOffset;
    } // at 0x58

    virtual u32 GetSize() const {
        return mSize;
    } // at 0x40

private:
    s32 mOffset; // at 0x16C
    s32 mSize;   // at 0x170
};

NandSoundArchive::NandSoundArchive() : mOpen(false) {}

NandSoundArchive::~NandSoundArchive() {
    Close();
}

bool NandSoundArchive::Open(const char* pPath) {
    if (mOpen) {
        Close();
    }

    if (NANDOpen(pPath, &mFileInfo, NAND_ACCESS_READ)) {
        return false;
    }

    mOpen = true;

    if (!LoadFileHeader()) {
        return false;
    }

    char currentDir[64];
    NANDGetCurrentDir(currentDir);
    u32 currDirLen = std::strlen(currentDir);

    char extRoot[FILE_PATH_MAX];
    std::strncpy(extRoot, currentDir, currDirLen + 1);

    for (int i = std::strlen(pPath) - 1; i >= 0; i--) {
        if (pPath[i] == '/' || pPath[i] == '\\') {
            // @bug Long path can overflow extRoot buffer
            std::strncat(extRoot, pPath, i);
            extRoot[currDirLen + i] = '\0';
            break;
        }
    }

    SetExternalFileRoot(extRoot);
    return true;
}

void NandSoundArchive::Close() {
    if (mOpen) {
        NANDClose(&mFileInfo);
        mOpen = false;
    }

    Shutdown();
}

ut::FileStream* NandSoundArchive::OpenStream(void* pBuffer, int size,
                                             u32 offset, u32 length) const {
    if (!mOpen) {
        return NULL;
    }

    if (size < sizeof(NandFileStream)) {
        return NULL;
    }

    return new (pBuffer) NandFileStream(&mFileInfo, offset, length);
}

ut::FileStream* NandSoundArchive::OpenExtStream(void* pBuffer, int size,
                                                const char* pExtPath,
                                                u32 offset, u32 length) const {
    if (!mOpen) {
        return NULL;
    }

    if (size < sizeof(NandFileStream)) {
        return NULL;
    }

    NandFileStream* pExtStream =
        new (pBuffer) NandFileStream(pExtPath, offset, length);

    if (!pExtStream->IsAvailable()) {
        pExtStream->~NandFileStream();
        return NULL;
    }

    return pExtStream;
}

int NandSoundArchive::detail_GetRequiredStreamBufferSize() const {
    return sizeof(NandFileStream);
}

bool NandSoundArchive::LoadFileHeader() {
    u8 headerArea[detail::SoundArchiveFile::HEADER_AREA_SIZE];

    static const u32 headerAlignSize =
        ut::RoundUp(sizeof(detail::SoundArchiveFile::Header), 32);

    void* pFile = ut::RoundUp<u8>(headerArea, 32);

    if (NANDSeek(&mFileInfo, 0, NAND_SEEK_BEG) != NAND_RESULT_OK) {
        return false;
    }

    s32 bytesRead = NANDRead(&mFileInfo, pFile, headerAlignSize);
    if (bytesRead != headerAlignSize) {
        return false;
    }

    mFileReader.Init(pFile);
    Setup(&mFileReader);

    return true;
}

bool NandSoundArchive::LoadHeader(void* pBuffer, u32 size) {
    u32 infoSize = mFileReader.GetInfoChunkSize();

    u32 infoOffsetU = mFileReader.GetInfoChunkOffset();
    s32 infoOffset = *reinterpret_cast<s32*>(&infoOffsetU);

    if (size < infoSize) {
        return false;
    }

    s32 currOffset = NANDSeek(&mFileInfo, infoOffset, NAND_SEEK_BEG);
    if (currOffset != infoOffset) {
        return false;
    }

    s32 bytesRead = NANDRead(&mFileInfo, pBuffer, infoSize);
    if (bytesRead != infoSize) {
        return false;
    }

    mFileReader.SetInfoChunk(pBuffer, infoSize);
    return true;
}

bool NandSoundArchive::LoadLabelStringData(void* pBuffer, u32 size) {
    u32 labelSize = mFileReader.GetLabelStringChunkSize();

    u32 labelOffsetU = mFileReader.GetLabelStringChunkOffset();
    s32 labelOffset = *reinterpret_cast<s32*>(&labelOffsetU);

    if (size < labelSize) {
        return false;
    }

    s32 currOffset = NANDSeek(&mFileInfo, labelOffset, NAND_SEEK_BEG);
    if (currOffset != labelOffset) {
        return false;
    }

    s32 bytesRead = NANDRead(&mFileInfo, pBuffer, labelSize);
    if (bytesRead != labelSize) {
        return false;
    }

    mFileReader.SetStringChunk(pBuffer, labelSize);
    return true;
}

NandSoundArchive::NandFileStream::NandFileStream(const NANDFileInfo* pFileInfo,
                                                 u32 offset, u32 size)
    : ut::NandFileStream(pFileInfo, NAND_ACCESS_READ, false),
      mOffset(offset),
      mSize(size) {

    if (IsAvailable()) {

        ut::NandFileStream::Seek(mOffset, SEEKORG_BEG);
    }
}

NandSoundArchive::NandFileStream::NandFileStream(const char* pPath, u32 offset,
                                                 u32 size)
    : ut::NandFileStream(pPath, NAND_ACCESS_READ),
      mOffset(offset),
      mSize(size) {

    if (IsAvailable()) {

        ut::NandFileStream::Seek(mOffset, SEEKORG_BEG);
    }
}

s32 NandSoundArchive::NandFileStream::Read(void* pDst, u32 size) {
    u32 endOffset = mOffset + mSize;
    u32 startOffset = ut::NandFileStream::Tell();

    if (startOffset + size > endOffset) {
        size = ut::RoundUp(endOffset - ut::NandFileStream::Tell(), 32);
    }

    s32 bytesRead = ut::NandFileStream::Read(pDst, size);
    DCStoreRange(pDst, size);

    return bytesRead;
}

void NandSoundArchive::NandFileStream::Seek(s32 offset, u32 origin) {
    switch (origin) {
    case SEEKORG_BEG: {
        offset += mOffset;
        break;
    }

    case SEEKORG_CUR: {
        offset += ut::NandFileStream::Tell();
        break;
    }

    case SEEKORG_END: {
        offset = mOffset + mSize - offset;
        break;
    }

    default: {
        return;
    }
    }

    if (offset < mOffset) {
        offset = mOffset;
    } else if (offset > mOffset + mSize) {
        offset = mOffset + mSize;
    }

    ut::NandFileStream::Seek(offset, SEEKORG_BEG);
}

}}
