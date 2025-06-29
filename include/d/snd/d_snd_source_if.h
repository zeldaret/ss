#ifndef D_SOUND_SOURCE_IF_H
#define D_SOUND_SOURCE_IF_H

#include "common.h"
#include "nw4r/math/math_types.h"

class dAcBase_c;

/**
 * Examples:
 * * Chandelier hold sound. Game code (dAcOChandelier_c::executeState_Wait) will
 *   push the current swing angle, callback (80397a40) will play additional sounds
 *   when chandelier swing speed exceeds certain values.
 * * SE_L_ROPE_WIND_LV: callback (80228814) will read and write values to create a feedback
 *   loop for certain parameters.
 * * `dSndSourceEquipment_c::overrideStartSoundId` will directly read the parameter to customize shield
 *   sounds.
 */
struct dSndSoundCtxParam {
    dSndSoundCtxParam() : intParam(0), floatParam(0.0f) {}
    ~dSndSoundCtxParam() {}

    /* 0x00 */ s32 intParam;
    /* 0x04 */ f32 floatParam;
};

/** An abstract interface for sound relating to a particular actor. */
class dSoundSourceIf_c {
public:
    static dSoundSourceIf_c *create(s32 sourceType, dAcBase_c *actor, const char *name, u8 subtype);

    virtual ~dSoundSourceIf_c() {}

    /* 0x00C */ virtual void setup() = 0;
    /* 0x010 */ virtual s32 getCategory() const = 0;
    /* 0x014 */ virtual bool isCategory(s32 category) const = 0;
    /* 0x018 */ virtual s32 getSourceType() const = 0;
    /* 0x01C */ virtual bool isSourceType(s32 type) const = 0;
    /* 0x020 */ virtual void vt_0x020() = 0;
    /* 0x024 */ virtual void setSubtype(u8 subtype) = 0;
    /* 0x028 */ virtual const nw4r::math::VEC3 &getListenerPosition() const = 0;
    /* 0x02C */ virtual void calc(const nw4r::math::VEC3 &) = 0;
    /* 0x030 */ virtual void onFlag(u32 mask) = 0;
    /* 0x034 */ virtual void offFlag(u32 mask) = 0;
    /* 0x038 */ virtual bool checkFlag(u32 mask) const = 0;
    /* 0x03C */ virtual void holdFlag(u32 mask) = 0;
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
    /* 0x0A0 */ virtual bool maybeUnusedEnemyDeath() = 0;
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
    /* 0x0D4 */ virtual bool startVoiceLine(u32 id) = 0;
    /* 0x0D8 */ virtual bool startVoiceLine(const char *label) = 0;
    /* 0x0DC */ virtual bool vt_0xDC(u32 id) = 0;
    /* 0x0E0 */ virtual void stopCurrentActionSound() = 0;
    /* 0x0E4 */ virtual void stopActionSound(u32 id) = 0;
    /* 0x0E8 */ virtual bool isPlayingActionSound() const = 0;
    /* 0x0EC */ virtual bool isCurrentActionSound(u32 id) const = 0;
    /* 0x0F0 */ virtual void vt_0x0F0_noop() = 0;
    /* 0x0F4 */ virtual void setField0x101(u8 value) = 0;
    /* 0x0F8 */ virtual void setField0x102(u8 value) = 0;
    /* 0x0FC */ virtual bool isInaudible() = 0;
    /* 0x100 */ virtual bool hasAnimSound() = 0;
    /* 0x104 */ virtual void load(void *data, const char *name) = 0;
    /* 0x108 */ virtual void setFrame(f32 frame) = 0;
    /* 0x10C */ virtual void setRate(f32 frame) = 0;
    /* 0x110 */ virtual void setPolyAttrs(u8 polyAttr0, u8 polyAttr1) = 0;
    /* 0x114 */ virtual void setBattleBgmRelated(UNKWORD) = 0;
    /* 0x118 */ virtual UNKWORD checkBattleBgmRelated() = 0;
};

#endif
