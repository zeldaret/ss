#ifndef D_SOUND_SOURCE_IF_H
#define D_SOUND_SOURCE_IF_H

#include "common.h"
#include "nw4r/math/math_types.h"

class dAcBase_c;

/** An abstract interface for sound relating to a particular actor. */
class dSoundSourceIf_c {
public:
    virtual ~dSoundSourceIf_c() {}
#define SOUNDSOURCE_VIRTUAL(offset) virtual void vt_##offset() = 0;
    SOUNDSOURCE_VIRTUAL(0x0C);
    SOUNDSOURCE_VIRTUAL(0x10);
    SOUNDSOURCE_VIRTUAL(0x14);
    virtual s32 getActorType() const;
    SOUNDSOURCE_VIRTUAL(0x1C);
    SOUNDSOURCE_VIRTUAL(0x20);
    SOUNDSOURCE_VIRTUAL(0x24);
    virtual const nw4r::math::VEC3 &getListenerPosition() const = 0; // 0x28
    SOUNDSOURCE_VIRTUAL(0x2C);
    SOUNDSOURCE_VIRTUAL(0x30);
    SOUNDSOURCE_VIRTUAL(0x34);
    SOUNDSOURCE_VIRTUAL(0x38);
    SOUNDSOURCE_VIRTUAL(0x3C);
    SOUNDSOURCE_VIRTUAL(0x40);
    SOUNDSOURCE_VIRTUAL(0x44);
    virtual bool hasPlayingSounds() const = 0;            // 0x48
    virtual bool isPlayingSound(u32 soundId) = 0;         // 0x4C
    virtual bool isPlayingSound(const char *soundId) = 0; // 0x50
    SOUNDSOURCE_VIRTUAL(0x54);
    SOUNDSOURCE_VIRTUAL(0x58);
    SOUNDSOURCE_VIRTUAL(0x5C);
    SOUNDSOURCE_VIRTUAL(0x60);
    SOUNDSOURCE_VIRTUAL(0x64);
    SOUNDSOURCE_VIRTUAL(0x68);
    SOUNDSOURCE_VIRTUAL(0x6C);
    SOUNDSOURCE_VIRTUAL(0x70);
    SOUNDSOURCE_VIRTUAL(0x74);
    SOUNDSOURCE_VIRTUAL(0x78);
    SOUNDSOURCE_VIRTUAL(0x7C);
    SOUNDSOURCE_VIRTUAL(0x80);
    SOUNDSOURCE_VIRTUAL(0x84);
    SOUNDSOURCE_VIRTUAL(0x88);
    SOUNDSOURCE_VIRTUAL(0x8C);
    SOUNDSOURCE_VIRTUAL(0x90);
    SOUNDSOURCE_VIRTUAL(0x94);
    SOUNDSOURCE_VIRTUAL(0x98);
    SOUNDSOURCE_VIRTUAL(0x9C);
    SOUNDSOURCE_VIRTUAL(0xA0);
    SOUNDSOURCE_VIRTUAL(0xA4);
    SOUNDSOURCE_VIRTUAL(0xA8);
    SOUNDSOURCE_VIRTUAL(0xAC);
    SOUNDSOURCE_VIRTUAL(0xB0);
    SOUNDSOURCE_VIRTUAL(0xB4);
    SOUNDSOURCE_VIRTUAL(0xB8);
    SOUNDSOURCE_VIRTUAL(0xBC);
    SOUNDSOURCE_VIRTUAL(0xC0);
    SOUNDSOURCE_VIRTUAL(0xC4);
    SOUNDSOURCE_VIRTUAL(0xC8);
    SOUNDSOURCE_VIRTUAL(0xCC);
    SOUNDSOURCE_VIRTUAL(0xD0);
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

    virtual bool isReadyMaybe() = 0;                     // 0x100
    virtual void load(void *data, const char *name) = 0; // 0x104
    virtual void setFrame(f32 frame) = 0;                // 0x108
    virtual void setRate(f32 frame) = 0;                 // 0x10C

    SOUNDSOURCE_VIRTUAL(0x110);
    SOUNDSOURCE_VIRTUAL(0x114);
    SOUNDSOURCE_VIRTUAL(0x118);
};

#endif
