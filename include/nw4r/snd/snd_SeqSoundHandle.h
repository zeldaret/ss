#ifndef NW4R_SND_SEQ_SOUND_HANDLE_H
#define NW4R_SND_SEQ_SOUND_HANDLE_H
#include <nw4r/types_nw4r.h>

#include <nw4r/snd/snd_SeqSound.h>

#include <nw4r/ut.h>

namespace nw4r {
namespace snd {

// Forward declarations
class SoundHandle;

class SeqSoundHandle : private ut::NonCopyable {
public:
    explicit SeqSoundHandle(SoundHandle* pHandle);
    ~SeqSoundHandle() {
        DetachSound();
    }

    bool IsAttachedSound() const {
        return mSound != NULL;
    }

    void DetachSound();

    void SetTempoRatio(f32 tempo) {
        if (IsAttachedSound()) {
            mSound->SetTempoRatio(tempo);
        }
    }

    void SetTrackVolume(u32 trackFlags, f32 volume) {
        if (IsAttachedSound()) {
            mSound->SetTrackVolume(trackFlags, volume);
        }
    }

    void WriteVariable(int i, s16 value) {
        if (IsAttachedSound()) {
            mSound->WriteVariable(i, value);
        }
    }

private:
    detail::SeqSound* mSound; // at 0x0
};

} // namespace snd
} // namespace nw4r

#endif
