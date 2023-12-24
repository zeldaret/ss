#ifndef NW4R_SND_STRMSOUNDHANDLE_H
#define NW4R_SND_STRMSOUNDHANDLE_H
#include "types_nw4r.h"

namespace nw4r
{
    namespace snd
    {
        struct StrmSoundHandle
        {
            detail::StrmSound *mStrmSound; // at 0x0

            inline StrmSoundHandle() : mStrmSound(NULL) {}
            inline bool IsAttachedSound()
            {
                return mStrmSound != NULL;
            }
            
            void DetachSound();
        };
    }
}

#endif