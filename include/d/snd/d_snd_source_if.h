#ifndef D_SOUND_SOURCE_IF_H
#define D_SOUND_SOURCE_IF_H

#include "common.h"
#include "nw4r/math/math_types.h"

class dAcBase_c;
class dSndSoundCtxParam;

/** An abstract interface for sound relating to a particular actor. */
class dSoundSourceIf_c {
public:
    virtual ~dSoundSourceIf_c() {}
#define SOUNDSOURCE_VIRTUAL(offset) virtual void vt_##offset() = 0;

    /*  0x00C */ virtual void setup() = 0;
    /*  0x010 */ virtual s32 getCategory() const = 0;
    /*  0x014 */ virtual bool isCategory(s32 category) const = 0;
    /*  0x018 */ virtual s32 getSourceType() const = 0;
    /*  0x01C */ virtual bool isSourceType(s32 type) const = 0;
    SOUNDSOURCE_VIRTUAL(0x20);
    SOUNDSOURCE_VIRTUAL(0x24);
    /* 0x028 */ virtual const nw4r::math::VEC3 &getListenerPosition() const = 0;
    /* 0x02C */ virtual void calc(const nw4r::math::VEC3 &) = 0;
    /* 0x030 */ virtual void onFlag1(u32 mask) = 0;
    /* 0x034 */ virtual void offFlag1(u32 mask) = 0;
    /* 0x038 */ virtual bool checkFlag(u32 mask) const = 0;
    /* 0x03C */ virtual void onFlag2(u32 mask) = 0;
    /* 0x040 */ virtual void stopAllSound(s32 fadeFrames) = 0;
    /* 0x044 */ virtual void shutdown() = 0;
    /* 0x048 */ virtual bool hasPlayingSounds() const = 0;
    /* 0x04C */ virtual bool isPlayingSound(u32 soundId) = 0;
    /* 0x050 */ virtual bool isPlayingSound(const char *soundId) = 0;
    /* 0x054 */ virtual void setCtxParam(const dSndSoundCtxParam *) = 0;
    /* 0x058 */ virtual void setCtxFloat(f32 value) = 0;
    /* 0x05C */ virtual void setCtxInt(s32 value) = 0;
    /* 0x060 */ virtual bool startSound(u32 soundId) = 0;
    /* 0x064 */ virtual bool startSoundWithIntParam(u32 soundId, s32 value) = 0;
    /* 0x068 */ virtual bool startSoundWithFloatParam(u32 soundId, f32 value) = 0;
    /* 0x06C */ virtual bool startSoundWithParams(u32 soundId, f32 fValue, s32 value) = 0;
    /* 0x070 */ virtual bool startSound(const char *label) = 0;
    /* 0x074 */ virtual bool startSoundWithIntParam(const char *label, s32 value) = 0;
    /* 0x078 */ virtual bool startSoundWithFloatParam(const char *label, f32 value) = 0;
    /* 0x07C */ virtual bool startSoundWithParams(const char *label, f32 fValue, s32 value) = 0;
    /* 0x080 */ virtual bool startSoundAtPosition(u32 soundId, const nw4r::math::VEC3 *position) = 0;
    /* 0x084 */ virtual bool
    startObjHitSound(u32 baseSoundId, dSoundSourceIf_c *source, const nw4r::math::VEC3 *position) = 0;
    /* 0x088 */ virtual bool
    startBgHitSound(u32 baseSoundId, u32 polyAttr0, u32 polyAttr1, const nw4r::math::VEC3 *position) = 0;
    /* 0x08C */ virtual bool startSoundAtPosition(const char *label, const nw4r::math::VEC3 *position) = 0;
    /* 0x090 */ virtual bool
    startObjHitSound(const char *label, dSoundSourceIf_c *source, const nw4r::math::VEC3 *position) = 0;
    /* 0x094 */ virtual bool
    startBgHitSound(const char *label, u32 polyAttr0, u32 polyAttr1, const nw4r::math::VEC3 *position) = 0;
    /* 0x098 */ virtual bool startSoundAtPosition2(u32 soundId, const nw4r::math::VEC3 *position) = 0;
    /* 0x09C */ virtual bool startSoundAtPosition2(const char *label, const nw4r::math::VEC3 *position) = 0;
    SOUNDSOURCE_VIRTUAL(0xA0);
    /* 0x0A4 */ virtual void stopSounds(u32 soundId, s32 fadeFrames) = 0;
    /* 0x0A8 */ virtual void stopSounds(const char *label, s32 fadeFrames) = 0;
    /* 0x0AC */ virtual bool holdSound(u32 soundId) = 0;
    /* 0x0B0 */ virtual bool holdSoundWithIntParam(u32 soundId, s32 value) = 0;
    /* 0x0B4 */ virtual bool holdSoundWithFloatParam(u32 soundId, f32 value) = 0;
    /* 0x0B8 */ virtual bool holdSoundWithParams(u32 soundId, f32 fValue, s32 value) = 0;
    /* 0x0BC */ virtual bool holdSound(const char *label) = 0;
    /* 0x0C0 */ virtual bool holdSoundWithIntParam(const char *label, s32 value) = 0;
    /* 0x0C4 */ virtual bool holdSoundWithFloatParam(const char *label, f32 value) = 0;
    /* 0x0C8 */ virtual bool holdSoundWithParams(const char *label, f32 fValue, s32 value) = 0;
    /* 0x0CC */ virtual bool holdSoundAtPosition(u32 soundId, const nw4r::math::VEC3 *position) = 0;
    /* 0x0D0 */ virtual bool holdSoundAtPosition(const char *label, const nw4r::math::VEC3 *position) = 0;
    SOUNDSOURCE_VIRTUAL(0xD4);
    SOUNDSOURCE_VIRTUAL(0xD8);
    SOUNDSOURCE_VIRTUAL(0xDC);
    SOUNDSOURCE_VIRTUAL(0xE0);
    SOUNDSOURCE_VIRTUAL(0xE4);
    SOUNDSOURCE_VIRTUAL(0xE8);
    SOUNDSOURCE_VIRTUAL(0xEC);
    SOUNDSOURCE_VIRTUAL(0xF0);
    SOUNDSOURCE_VIRTUAL(0xF4);
    SOUNDSOURCE_VIRTUAL(0xF8);
    SOUNDSOURCE_VIRTUAL(0xFC);

    /* 0x100 */ virtual bool isReadyMaybe() = 0;
    /* 0x104 */ virtual void load(void *data, const char *name) = 0;
    /* 0x108 */ virtual void setFrame(f32 frame) = 0;
    /* 0x10C */ virtual void setRate(f32 frame) = 0;
    /* 0x110 */ virtual void setPolyAttrs(u8 polyAttr0, u8 polyAttr1) = 0;
    /* 0x114 */ virtual void setBattleBgmRelated(UNKWORD) = 0;
    /* 0x118 */ virtual bool checkBattleBgmRelated() = 0;
};

#endif
