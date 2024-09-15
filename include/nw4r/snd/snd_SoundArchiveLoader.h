#ifndef NW4R_SND_SOUND_ARCHIVE_LOADER_H
#define NW4R_SND_SOUND_ARCHIVE_LOADER_H
#include <nw4r/types_nw4r.h>

#include <nw4r/ut.h>

#include <rvl/OS.h>

namespace nw4r {
namespace snd {

// Forward declarations
class SoundMemoryAllocatable;

namespace detail {

class FileStreamHandle {
public:
    FileStreamHandle(ut::FileStream* pFileStream) : mStream(pFileStream) {}

    ~FileStreamHandle() {
        if (mStream != NULL) {
            mStream->Close();
        }
    }

    ut::FileStream* GetFileStream() {
        return mStream;
    }

    ut::FileStream* operator->() {
        return mStream;
    }

    operator bool() const {
        return mStream;
    }

private:
    ut::FileStream* mStream; // at 0x0
};

class SoundArchiveLoader {
public:
    explicit SoundArchiveLoader(const SoundArchive& rArchive);
    ~SoundArchiveLoader();

    void* LoadGroup(u32 id, SoundMemoryAllocatable* pAllocatable,
                    void** ppWaveBuffer, u32 blockSize);

private:
    mutable OSMutex mMutex;   // at 0x0
    const SoundArchive& mArc; // at 0x18
    u8 mStreamArea[512];      // at 0x1C
    ut::FileStream* mStream;  // at 0x21C
};

} // namespace detail
} // namespace snd
} // namespace nw4r

#endif
