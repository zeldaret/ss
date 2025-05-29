#include "nw4r/snd/snd_Util.h"
#include "nw4r/ut/ut_binaryFileFormat.h"

namespace nw4r {
namespace snd {
namespace detail {

struct AnimEvent {
    u32 flags_0x00;                                  // at 0x00
    u32 soundId;                                     // at 0x04
    detail::Util::DataRef<const char> soundLabelRef; // at 0x08
    u8 volume;                                       // at 0x10
    u8 field_0x11;                                   // at 0x11
    u8 varNo;                                        // at 0x12
    f32 pitch;                                       // at 0x14
    UNKWORD field_0x18;                              // at 0x18
    UNKWORD field_0x1C;                              // at 0x1C

    bool ShouldPreventStart() const {
        return flags_0x00 & 1;
    }

    const char *GetSoundLabel() const {
        return Util::GetDataRefAddress0(soundLabelRef, this);
    }

    bool GetVarNo(u32 *var) const {
        if (ShouldSetVariable()) {
            *var = varNo;
            return true;
        }
        return false;
    }

    bool ShouldSetVariable() const {
        return flags_0x00 & 2;
    }

    bool MatchesDirection(int playDirection) const {
        bool ret = 0;
        u32 config = field_0x11;
        if ((config == 0 || (playDirection == 0 && config == 1) || (playDirection == 1 && config == 2))) {
            ret = true;
        }
        return ret;
    }
};

struct AnimEventRef {
    int field_0x00;                 // at 0x08
    int field_0x04;                 // at 0x04
    u8 flags_0x08;                  // at 0x08
    s8 field_0x09;                  // at 0x09
    u8 field_0x0A;                  // at 0x0A
    Util::DataRef<AnimEvent> event; // at 0x0C
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
        u32 duration;                         // at 0x08
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

    bool IsValid() const {
        return mpHeader != NULL;
    }

    u32 GetAnimDuration() const {
        if (!IsValid()) {
            return 0;
        }
        return mpBlock->duration;
    }

private:
    const AnimSoundFile::Header *mpHeader; // at 0x00
    const AnimSoundFile::Block *mpBlock;   // at 0x04
};

} // namespace detail
} // namespace snd
} // namespace nw4r
