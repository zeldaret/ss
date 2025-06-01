#ifndef D_SND_ACTOR_H
#define D_SND_ACTOR_H

#include "common.h"
#include "nw4r/math/math_types.h"
#include "nw4r/snd/snd_Sound3DActor.h"
#include "nw4r/snd/snd_SoundHandle.h"

class dSnd3DActor_c : public nw4r::snd::Sound3DActor {
    static const u32 NUM_SOUNDS = 4;

public:
    dSnd3DActor_c(UNKTYPE *, u8);

    virtual ~dSnd3DActor_c() {}

    virtual bool d_vt_0x30() {
        return true;
    }

    virtual void d_vt_0x34(const nw4r::math::VEC3 &);

    virtual void d_vt_0x38(bool flag, int fadeFrames) = 0;

    virtual UNKWORD d_vt_0x3C() {
        return 0;
    }

    virtual UNKWORD d_vt_0x40() {
        return 0;
    }

    virtual UNKWORD d_vt_0x44() {
        return 0;
    }

    virtual const nw4r::math::VEC3 &getPositionRelativeToPlayer();
    virtual f32 getDistanceToPlayer();             // vt 0x4C
    virtual void updatePositionRelativeToPlayer(); // vt 0x50
    virtual void updateDistanceToPlayer();         // vt 0x54
    virtual void d_vt_0x58() = 0;
    virtual void d_vt_0x5C() = 0;

    void calculatePositionRelativeToListener();
    void updatePositionRelativeToListener();
    void updateDistanceToListener();

    void resetFloats();

    bool checkFlag(u16 flag) const {
        return (mFlags & flag);
    }

    void setFlag(u16 flag) {
        mFlags |= flag;
    }

    bool hasPlayingSounds() const;
    bool isPlayingSound(u32 id);

    const char *soundIdToSoundLabel(u32 soundId) const;
    u32 soundLabelToSoundId(const char *soundLabel) const;

protected:
    // a_ prefix to prevent multiple inheritance clashes
    /* 0x7D */ u8 a_field_0x7D;
    /* 0x7E */ u8 a_field_0x7E;
    /* 0x7F */ u8 a_field_0x7F;
    /* 0x80 */ u8 a_field_0x80;
    /* 0x84 */ f32 a_field_0x84;
    /* 0x88 */ f32 a_field_0x88;
    /* 0x8C */ f32 a_field_0x8C;
    /* 0x90 */ f32 a_field_0x90;
    /* 0x94 */ f32 mDistanceToListener;
    /* 0x98 */ f32 a_field_0x98;
    /* 0x9C */ f32 a_field_0x9C;
    /* 0xA0 */ nw4r::math::VEC3 mPositionRelativeToListener;
    /* 0xAC */ nw4r::math::VEC3 mPositionTransformedByListener;
    /* 0xB8 */ u8 a_0xB8[0xC8 - 0xB8];
    /* 0xC8 */ UNKTYPE *a_field_0xC8;
    /* 0xCC */ u16 mFlags;
    /* 0xD0 */ nw4r::math::VEC3 mPositionRelativeToPlayer;
    /* 0xDC */ f32 mDistanceToPlayer;
    /* 0xE0 */ f32 a_field_0xE0;
};

// used at the very least when trying to finish Demise
class IsCurrentSoundIdChecker {
public:
    IsCurrentSoundIdChecker(u32 id, bool *pResult) : mSoundId(id), mpResult(pResult) {}
    ~IsCurrentSoundIdChecker() {}

    virtual void operator()(nw4r::snd::SoundHandle &pHandle) {
        if (mSoundId == pHandle.GetId()) {
            *mpResult = true;
        }
    }

private:
    /* 0x04 */ u32 mSoundId;
    /* 0x08 */ bool *mpResult;
};

#endif
