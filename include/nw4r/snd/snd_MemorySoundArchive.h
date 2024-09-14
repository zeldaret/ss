#ifndef NW4R_SND_MEMORY_SOUND_ARCHIVE_H
#define NW4R_SND_MEMORY_SOUND_ARCHIVE_H
#include <nw4r/types_nw4r.h>

#include <nw4r/snd/snd_SoundArchive.h>
#include <nw4r/snd/snd_SoundArchiveFile.h>

#include <nw4r/ut.h>

namespace nw4r {
namespace snd {

class MemorySoundArchive : public SoundArchive {
private:
    class MemoryFileStream;

public:
    MemorySoundArchive();
    virtual ~MemorySoundArchive(); // at 0x8

    virtual const void* detail_GetFileAddress(u32 id) const;         // at 0xC
    virtual const void* detail_GetWaveDataFileAddress(u32 id) const; // at 0x10

    virtual int detail_GetRequiredStreamBufferSize() const; // at 0x14

    virtual ut::FileStream* OpenStream(void* pBuffer, int size, u32 offset,
                                       u32 length) const; // at 0x18

    virtual ut::FileStream* OpenExtStream(void* pBuffer, int size,
                                          const char* pExtPath, u32 offset,
                                          u32 length) const; // at 0x1C

    bool Setup(const void* pBuffer);
    void Shutdown();

private:
    const void* mData;                          // at 0x108
    detail::SoundArchiveFileReader mFileReader; // at 0x10C
};

} // namespace snd
} // namespace nw4r

#endif
