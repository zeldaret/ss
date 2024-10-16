#ifndef NW4R_SND_DVD_SOUND_ARCHIVE_H
#define NW4R_SND_DVD_SOUND_ARCHIVE_H
#include "nw4r/snd/snd_SoundArchive.h"
#include "nw4r/snd/snd_SoundArchiveFile.h"
#include "nw4r/types_nw4r.h"
#include "nw4r/ut.h" // IWYU pragma: export
#include "rvl/DVD.h" // IWYU pragma: export

namespace nw4r {
namespace snd {

class DvdSoundArchive : public SoundArchive {
private:
    class DvdFileStream;

public:
    DvdSoundArchive();
    virtual ~DvdSoundArchive(); // at 0x8

    virtual const void *detail_GetFileAddress(u32 /* id */) const {
        return NULL;
    } // at 0xC

    virtual const void *detail_GetWaveDataFileAddress(u32 /* id */) const {
        return NULL;
    } // at 0x10

    virtual int detail_GetRequiredStreamBufferSize() const; // at 0x14

    virtual ut::FileStream *OpenStream(void *pBuffer, int size, u32 offset,
                                       u32 length) const; // at 0x18

    virtual ut::FileStream *OpenExtStream(
        void *pBuffer, int size, const char *pExtPath, u32 offset,
        u32 length
    ) const; // at 0x1C

    bool Open(s32 entrynum);
    bool Open(const char *pPath);

    void Close();

    bool LoadHeader(void *pBuffer, u32 size);
    bool LoadLabelStringData(void *pBuffer, u32 size);

    u32 GetHeaderSize() const {
        return mFileReader.GetInfoChunkSize();
    }
    u32 GetLabelStringDataSize() const {
        return mFileReader.GetLabelStringChunkSize();
    }

private:
    bool LoadFileHeader();

private:
    detail::SoundArchiveFileReader mFileReader; // at 0x108
    DVDFileInfo mFileInfo;                      // at 0x14C
    bool mOpen;                                 // at 0x188
};

} // namespace snd
} // namespace nw4r

#endif
