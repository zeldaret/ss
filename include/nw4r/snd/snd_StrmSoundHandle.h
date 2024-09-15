#ifndef NW4R_SND_STRM_SOUND_HANDLE_H
#define NW4R_SND_STRM_SOUND_HANDLE_H
#include <nw4r/types_nw4r.h>

#include <nw4r/snd/snd_StrmSound.h>

#include <nw4r/ut.h>

namespace nw4r {
namespace snd {

class StrmSoundHandle : private ut::NonCopyable {
public:
    void DetachSound();

    bool IsAttachedSound() {
        return mSound != NULL;
    }

private:
    detail::StrmSound* mSound; // at 0x0
};

} // namespace snd
} // namespace nw4r

#endif
