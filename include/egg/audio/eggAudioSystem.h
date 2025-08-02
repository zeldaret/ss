#ifndef EGG_AUDIO_SYSTEM_H
#define EGG_AUDIO_SYSTEM_H

#include "common.h"

namespace EGG {

class AudioSystem {
public:
    AudioSystem();
    ~AudioSystem();

    void reset(s32 ms);
    void recoverReset();
    void shutdown(s32 ms);
    void calc();

    // Non-official inline names
    bool isReset() {
        return mResetStatus == 2;
    }

    bool isShutdown() {
        return mShutdownStatus == 2;
    }

    bool isShuttingDown() {
        return mShutdownStatus != 0;
    }

    bool isResetting() {
        return mResetStatus != 0;
    }

    static AudioSystem *sInstanse; ///< sic

private:
    /* 0x00 */ f32 mSavedMasterVolume;
    /* 0x04 */ s32 mResetStatus;
    /* 0x08 */ s32 mShutdownStatus;
};

} // namespace EGG

#endif
