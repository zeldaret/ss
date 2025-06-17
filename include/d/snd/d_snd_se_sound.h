#ifndef D_SND_SE_SOUND_H
#define D_SND_SE_SOUND_H

#include "common.h"
#include "d/snd/d_snd_sound.h"
#include "d/snd/d_snd_types.h"
#include "nw4r/snd/snd_SoundHandle.h"
#include "nw4r/ut/ut_list.h"

/**
 * Examples:
 * * Chandelier hold sound. Game code (dAcOChandelier_c::executeState_Wait) will
 *   push the current swing angle, callback (80397a40) will play additional sounds
 *   when chandelier swing speed exceeds certain values.
 * * SE_L_ROPE_WIND_LV: callback (80228814) will read and write values to create a feedback
 *   loop for certain parameters.
 */
struct dSndSoundCtxParam {
    dSndSoundCtxParam() : intParam(0), floatParam(0.0f) {}
    ~dSndSoundCtxParam() {}

    /* 0x00 */ s32 intParam;
    /* 0x04 */ f32 floatParam;
};

/**
 * A sound effect handle. Typically acquired by sound sources from a pool.
 * This allows sound sources to play sounds with no pre-determined limit
 * on the number of sounds. It also allows the source to handle the statefulness
 * of holdSound without requiring users to manage handles.
 *
 * Additionally, some sound sources will register callbacks to customize
 * sound parameters (pitch, volume, ...). These callbacks can additionally
 * write and read context parameters (see above). The game will shift these
 * parameters with each call, so a lot of this is obviously frame-rate dependent.
 */
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
    // TODO these names are probably wrong
    /* vt 0x0C */ virtual void onStart(dSoundSource_c *source);
    /* vt 0x10 */ virtual void onCalc(dSoundSource_c *source);

    void calc(dSoundSource_c *source);
    void clear();

    // not sure what this does
    void shiftParam(const dSndSoundCtxParam *p);
    void shiftFloatParam(f32 value);

    typedef void Callback(dSndSeSound_c *sound, dSoundSource_c *source, nw4r::snd::SoundHandle &handle);

    void stop(s32 fadeFrames) {
        mpSoundHandle->Stop(fadeFrames);
    }

    Type_e getHandleType() const {
        return mHandleType;
    }

    // cannot be const...
    bool isAttachedSound() {
        return mpSoundHandle->IsAttachedSound();
    }

    nw4r::snd::SoundHandle *getHandle() {
        return mpSoundHandle;
    }

    void setHandle(nw4r::snd::SoundHandle *handle) {
        if (handle != nullptr) {
            mpSoundHandle = handle;
            field_0x11C = 0;
        } else {
            mpSoundHandle = &mSound;
            field_0x11C = 1;
        }
    }

    void attachSource(dSoundSource_c *source) {
        mpSource = source;
    }

    bool isAttachedSource() const {
        return mpSource != nullptr;
    }

    bool unkRemovalCheck() const {
        return !field_0x11D && !field_0x11E;
    }

    bool isSoundId(u32 id) const {
        return mSoundId == id;
    }

    void setAcquiredMaybe(bool value) {
        field_0x11D = value;
    }

    void setInUseMaybe(bool value) {
        field_0x11E = value;
    }

protected:
    /* 0x004 */ nw4r::ut::Node mSourceLink;
    /* 0x00C */ nw4r::ut::Node mPoolLink;
    /* 0x014 */ dSoundSource_c *mpSource;
    /* 0x018 */ dSndSound_c mSound;
    /* 0x108 */ nw4r::snd::SoundHandle *mpSoundHandle;
    /* 0x10C */ Type_e mHandleType;
    /* 0x110 */ u32 mSoundId;
    /* 0x114 */ s32 field_0x114;
    /* 0x118 */ dSndSoundCtxParam *mpUnk;
    /* 0x11C */ u8 field_0x11C;
    /* 0x11D */ u8 field_0x11D;
    /* 0x11E */ u8 field_0x11E;
    /* 0x120 */ Callback *mpStartCallback;
    /* 0x124 */ Callback *mpCalcCallback;
};

#endif
