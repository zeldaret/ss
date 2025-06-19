#ifndef D_SND_ANIM_SOUND_H
#define D_SND_ANIM_SOUND_H

#include "d/snd/d_snd_types.h"
#include "nw4r/snd/snd_AnimSound.h"

// Could also inherit from AnimSound
class dSndAnimSound_c {
public:
    dSndAnimSound_c(dSoundSource_c *source);
    void setData(const void *data, const char *name);
    void setFrame(f32 frame);
    void resetFrame(f32 frame);
    void setRate(f32 rate);

    void setCallback(dSoundSource_c *source);
    void setCallback(nw4r::snd::AnimSound::Callback cb, void *userData);

    void shutdown() {
        mSound.Shutdown();
    }

private:
    static void animCallback(int, s32, const char *, UNKWORD, void *userData);

    /* 0x00 */ nw4r::snd::AnimSound mSound;
    /* 0x90 */ const void *mpData;
    /* 0x94 */ f32 field_0x94;
    /* 0x98 */ f32 field_0x98;
    /* 0x9C */ bool mNeedFrameReset;
    /* 0xA0 */ nw4r::snd::AnimSound::PlayDirection mDirection;
};

#endif
