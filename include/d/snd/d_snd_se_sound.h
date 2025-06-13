#ifndef D_SND_SE_SOUND_H
#define D_SND_SE_SOUND_H

#include "common.h"
#include "d/snd/d_snd_sound.h"
#include "nw4r/snd/snd_SoundHandle.h"
#include "nw4r/ut/ut_list.h"

class dSoundSource_c;

struct UnkSeSoundStruct {
    UnkSeSoundStruct() : field_0x00(0), field_0x04(0.0f) {}
    ~UnkSeSoundStruct() {}

    /* 0x00 */ UNKWORD field_0x00;
    /* 0x04 */ f32 field_0x04;
};

class dSndSeSound_c {
public:
    dSndSeSound_c();
    ~dSndSeSound_c();

    enum Type_e {
        SE_HANDLE_TYPE_1 = 0,
        SE_HANDLE_TYPE_2 = 1,
    };

    // vtable at 0x000
    /* vt 0x08 */ virtual void setSource(dSoundSource_c *source, u32 soundId);
    /* vt 0x0C */ virtual void onStart(dSoundSource_c *source);
    /* vt 0x10 */ virtual void onCalc(dSoundSource_c *source);

    void calc(dSoundSource_c *source);
    void clear();

    // not sure what this does
    void shiftPair(const UnkSeSoundStruct *p);
    void shiftValue(f32 value);

    typedef void Callback(dSndSeSound_c *sound, dSoundSource_c *source, nw4r::snd::SoundHandle &handle);

    void stop(s32 fadeFrames) {
        mpSoundHandle->Stop(fadeFrames);
    }

    bool isAttachedSound() const {
        return mpSoundHandle->IsAttachedSound();
    }

protected:
    /* 0x004 */ nw4r::ut::Node mSourceLink;
    /* 0x00C */ nw4r::ut::Node mPoolLink;
    /* 0x014 */ UNKWORD field_0x014;
    /* 0x018 */ dSndSound_c mSound;
    /* 0x108 */ dSndSound_c *mpSoundHandle;
    /* 0x10C */ Type_e mHandleType;
    /* 0x110 */ u32 mSoundId;
    /* 0x114 */ s32 field_0x114;
    /* 0x118 */ UnkSeSoundStruct *mpUnk;
    /* 0x11C */ u8 field_0x11C;
    /* 0x11D */ u8 field_0x11D;
    /* 0x11E */ u8 field_0x11E;
    /* 0x120 */ Callback *mpStartCallback;
    /* 0x124 */ Callback *mpCalcCallback;
};

#endif
