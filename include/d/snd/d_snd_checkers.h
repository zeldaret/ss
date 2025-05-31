#ifndef D_SND_CHECKERS_H
#define D_SND_CHECKERS_H

#include "nw4r/snd/snd_SoundHandle.h"

class SoundPropertiesChecker {
public:
    virtual void operator()(nw4r::snd::SoundHandle &pHandle);

private:
    /* 0x04 */ u32 mSoundId;
    /* 0x08 */ u32 mPlayCounter;
    /* 0x0C */ u32 mPauseCounter;
    /* 0x10 */ u32 mCounter3;
    /* 0x14 */ u32 mCounter4;
    /* 0x18 */ u32 *mpPlayCounter;
    /* 0x1C */ u32 *mpPauseCounter;
    /* 0x20 */ u32 *mpCounter3;
    /* 0x24 */ u32 *mpCounter4;
};

#endif
