#ifndef NW4R_SND_SOUND_STARTABLE_H
#define NW4R_SND_SOUND_STARTABLE_H
#include "common.h"
#include "snd_BasicSound.h"


namespace nw4r {
namespace snd {
struct SoundStartable {
    struct StartInfo {};

    inline SoundStartable() {}
    inline virtual ~SoundStartable() {} // at 0x8
    virtual UNKWORD detail_SetupSound(SoundHandle *, u32, detail::BasicSound::AmbientArgInfo *,
            detail::ExternalSoundPlayer *, bool,
            const StartInfo *) = 0;                                       // at 0xc
    virtual UNKWORD detail_ConvertLabelStringToSoundId(const char *) = 0; // at 0x10

    UNKWORD detail_StartSound(SoundHandle *, u32, detail::BasicSound::AmbientArgInfo *, detail::ExternalSoundPlayer *,
            const StartInfo *);

    UNKWORD detail_HoldSound(SoundHandle *, u32, detail::BasicSound::AmbientArgInfo *, detail::ExternalSoundPlayer *,
            const StartInfo *);

    UNKWORD detail_PrepareSound(SoundHandle *, u32, detail::BasicSound::AmbientArgInfo *, detail::ExternalSoundPlayer *,
            const StartInfo *);
};
} // namespace snd
} // namespace nw4r

#endif
