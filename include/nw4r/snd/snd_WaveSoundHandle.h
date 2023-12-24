#ifndef NW4R_SND_WAVESOUNDHANDLE_H
#define NW4R_SND_WAVESOUNDHANDLE_H
#include "types_nw4r.h"

namespace nw4r
{
    namespace snd
    {
        struct WaveSoundHandle
        {
            detail::WaveSound *mWaveSound; // at 0x0

            inline WaveSoundHandle() : mWaveSound(NULL) {}
            inline bool IsAttachedSound()
            {
                return mWaveSound != NULL;
            }
            
            void DetachSound();
        };
    }
}

#endif