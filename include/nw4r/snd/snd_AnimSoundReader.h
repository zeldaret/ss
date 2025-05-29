#include "nw4r/snd/snd_Util.h"
#include "nw4r/ut/ut_binaryFileFormat.h"

namespace nw4r {
namespace snd {
namespace detail {

struct AnimEvent {};

struct AnimEventRef {
    u8 _0x00[0xC];
    Util::DataRef<AnimEvent> event; // at 0x08
};

struct AnimSoundFile {
    static u32 const SIGNATURE_FILE = NW4R_FOUR_BYTE('R', 'A', 'S', 'D');
    static int const FILE_VERSION = NW4R_FILE_VERSION(1, 0);

    struct Header {
        ut::BinaryFileHeader fileHeader; // size 0x10, offset 0x00
        u32 blockOffset;                 // at 0x10
    };

    struct EventTable {
        Util::Table<AnimEventRef> eventRef; // at 0x00
    };

    struct Block {
        ut::BinaryBlockHeader blockHeader;    // at 0x00
        UNKWORD _0x08;                        // at 0x08
        Util::DataRef<EventTable> eventTable; // at 0x0C
    };
};

class AnimSoundFileReader {
public:
    AnimSoundFileReader();
    bool Setup(const void *buf);
    void Shutdown();
    u32 GetEventCount() const;

    const AnimSoundFile::EventTable *GetEventTable() const;
    const AnimEventRef *GetEventRef(u32 id) const;
    const AnimEvent *GetEvent(const AnimEventRef *ref) const;

private:
    const AnimSoundFile::Header *mpHeader; // at 0x00
    const AnimSoundFile::Block *mpBlock;   // at 0x04
};

} // namespace detail
} // namespace snd
} // namespace nw4r
