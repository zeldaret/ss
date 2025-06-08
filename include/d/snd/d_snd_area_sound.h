#ifndef D_SND_AREA_SOUND_H
#define D_SND_AREA_SOUND_H

#include "common.h"
#include "d/snd/d_snd_sound.h"

/** A sound class used for dSndAreaSoundEffectMgr */
class dSndAreaSound_c : public dSndSound_c {
public:
    dSndAreaSound_c();

    virtual void cancel() override;                       // 0x08
    virtual void calc() override;                         // 0x0C
    virtual void fadeIn(u32 id, s32 fadeFrames) override; // 0x10

    virtual void initCallback(u32 id); // 0x28

    typedef void (*Callback)(dSndAreaSound_c &sound);

    using dSndSound_c::pause;

    void pause(s32 fadeFrames);
    void unpause(s32 fadeFrames);

private:
    bool isFadingOut() const {
        return mIsFadingOut && IsAttachedSound();
    }

    /* 0x0F0 */ bool field_0xF0;
    /* 0x0F4 */ Callback mpCalcCallback;
    /* 0x0F8 */ UNKWORD field_0xF8;
    /* 0x0FC */ UNKWORD field_0xFC;
    /* 0x100 */ UNKWORD field_0x100;
    /* 0x104 */ UNKWORD field_0x104;
};

#endif
