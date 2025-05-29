/* Only implemented to the extent necessary to match data sections. */

#include "nw4r/snd/snd_AnimSound.h"

#include "common.h" // nullptr
#include "nw4r/snd/snd_Util.h"
#include "nw4r/ut/ut_binaryFileFormat.h"

namespace nw4r {
namespace snd {
namespace detail {

bool IsValidFileHeader(const void *data) {
    ut::BinaryFileHeader const *fileHeader = static_cast<ut::BinaryFileHeader const *>(data);

    if (fileHeader->signature != AnimSoundFile::SIGNATURE_FILE) {
        return false;
    }

    if (fileHeader->version < AnimSoundFile::FILE_VERSION) {
        return false;
    }

    if (fileHeader->version > AnimSoundFile::FILE_VERSION) {
        return false;
    }

    return true;
}

AnimSoundFileReader::AnimSoundFileReader() : mpHeader(NULL) {}

bool AnimSoundFileReader::Setup(const void *buf) {
    if (!IsValidFileHeader(buf)) {
        return false;
    }
    mpHeader = static_cast<const AnimSoundFile::Header *>(buf);
    mpBlock = static_cast<const AnimSoundFile::Block *>(ut::AddOffsetToPtr(buf, mpHeader->blockOffset));
    return true;
}

void AnimSoundFileReader::Shutdown() {
    mpHeader = NULL;
    mpBlock = NULL;
}

u32 AnimSoundFileReader::GetEventCount() const {
    const AnimSoundFile::EventTable *event = GetEventTable();
    return event->eventRef.count;
}

const AnimEventRef *AnimSoundFileReader::GetEventRef(u32 id) const {
    const AnimSoundFile::EventTable *event = GetEventTable();
    return &event->eventRef.item[id];
}

const AnimSoundFile::EventTable *AnimSoundFileReader::GetEventTable() const {
    if (mpBlock == NULL) {
        return NULL;
    }
    return Util::GetDataRefAddress0(mpBlock->eventTable, &mpBlock->_0x08);
}

const AnimEvent *AnimSoundFileReader::GetEvent(const AnimEventRef *ref) const {
    if (ref == NULL) {
        return NULL;
    }
    const AnimSoundFile::EventTable *event = GetEventTable(); // unused
    return Util::GetDataRefAddress0(ref->event, &mpBlock->_0x08);
}

} // namespace detail
} // namespace snd
} // namespace nw4r
