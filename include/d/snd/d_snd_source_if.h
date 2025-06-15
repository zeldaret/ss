#ifndef D_SOUND_SOURCE_IF_H
#define D_SOUND_SOURCE_IF_H

#include "common.h"
#include "d/snd/d_snd_se_sound.h"
#include "nw4r/math/math_types.h"

class dAcBase_c;

/** An abstract interface for sound relating to a particular actor. */
class dSoundSourceIf_c {
public:
    virtual ~dSoundSourceIf_c() {}
#define SOUNDSOURCE_VIRTUAL(offset) virtual void vt_##offset() = 0;

    virtual void setup() = 0;                        // 0x00C
    virtual s32 getCategory() const = 0;             // 0x010
    virtual bool isCategory(s32 category) const = 0; // 0x014
    virtual s32 getSourceType() const = 0;           // 0x018
    virtual bool isSourceType(s32 type) const = 0;   // 0x01C
    SOUNDSOURCE_VIRTUAL(0x20);
    SOUNDSOURCE_VIRTUAL(0x24);
    virtual const nw4r::math::VEC3 &getListenerPosition() const = 0;                    // 0x028
    virtual void calc(const nw4r::math::VEC3 &) = 0;                                    // 0x02C
    virtual void onFlag1(u32 mask) = 0;                                                 // 0x030
    virtual void offFlag1(u32 mask) = 0;                                                // 0x034
    virtual bool checkFlag(u32 mask) const = 0;                                         // 0x038
    virtual void onFlag2(u32 mask) = 0;                                                 // 0x03C
    virtual void stopAllSound(s32 fadeFrames) = 0;                                      // 0x040
    virtual void shutdown() = 0;                                                        // 0x044
    virtual bool hasPlayingSounds() const = 0;                                          // 0x048
    virtual bool isPlayingSound(u32 soundId) = 0;                                       // 0x04C
    virtual bool isPlayingSound(const char *soundId) = 0;                               // 0x050
    virtual void setUnkSe(const UnkSeSoundStruct *) = 0;                                // 0x054
    virtual void setUnkSeFloat(f32 value) = 0;                                          // 0x058
    virtual void setUnkSeWord(UNKWORD value) = 0;                                       // 0x05C
    virtual bool startSound(u32 soundId) = 0;                                           // 0x060
    virtual bool startSoundWithUnkSeWord(u32 soundId, UNKWORD value) = 0;               // 0x064
    virtual bool startSoundWithUnkSeFloat(u32 soundId, f32 value) = 0;                  // 0x068
    virtual bool startSoundWithUnkSe(u32 soundId, f32 fValue, UNKWORD value) = 0;       // 0x06C
    virtual bool startSound(const char *label) = 0;                                     // 0x070
    virtual bool startSoundWithUnkSeWord(const char *label, UNKWORD value) = 0;         // 0x074
    virtual bool startSoundWithUnkSeFloat(const char *label, f32 value) = 0;            // 0x078
    virtual bool startSoundWithUnkSe(const char *label, f32 fValue, UNKWORD value) = 0; // 0x07C
    SOUNDSOURCE_VIRTUAL(0x80);
    SOUNDSOURCE_VIRTUAL(0x84);
    SOUNDSOURCE_VIRTUAL(0x88);
    SOUNDSOURCE_VIRTUAL(0x8C);
    SOUNDSOURCE_VIRTUAL(0x90);
    SOUNDSOURCE_VIRTUAL(0x94);
    SOUNDSOURCE_VIRTUAL(0x98);
    SOUNDSOURCE_VIRTUAL(0x9C);
    SOUNDSOURCE_VIRTUAL(0xA0);
    virtual void stopSounds(u32 soundId, s32 fadeFrames) = 0;       // 0x0A4
    virtual void stopSounds(const char *label, s32 fadeFrames) = 0; // 0x0A8
    virtual bool holdSound(u32 soundId) = 0;                        // 0x0AC
    SOUNDSOURCE_VIRTUAL(0xB0);
    SOUNDSOURCE_VIRTUAL(0xB4);
    SOUNDSOURCE_VIRTUAL(0xB8);
    virtual bool holdSound(const char *label) = 0; // 0x0BC
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

    virtual bool isReadyMaybe() = 0;                           // 0x100
    virtual void load(void *data, const char *name) = 0;       // 0x104
    virtual void setFrame(f32 frame) = 0;                      // 0x108
    virtual void setRate(f32 frame) = 0;                       // 0x10C
    virtual void setPolyAttrs(u8 polyAttr0, u8 polyAttr1) = 0; // 0x110
    virtual void setBattleBgmRelated(UNKWORD) = 0;             // 0x114
    virtual bool checkBattleBgmRelated() = 0;                  // 0x118
};

#endif
