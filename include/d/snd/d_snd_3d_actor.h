#ifndef D_SND_ACTOR_H
#define D_SND_ACTOR_H

#include "common.h"
#include "d/snd/d_snd_misc.h"
#include "d/snd/d_snd_types.h"
#include "nw4r/math/math_types.h"
#include "nw4r/snd/snd_Sound3DActor.h"
#include "nw4r/snd/snd_SoundHandle.h"

class dSnd3DActor_c : public nw4r::snd::Sound3DActor {
public:
    dSnd3DActor_c(dSndSourceParam *pSourceParam, s32 sourceType);

    virtual ~dSnd3DActor_c() {}

    virtual bool d_vt_0x30() {
        return true;
    }

    virtual void setPosition(const nw4r::math::VEC3 &);   // vt  0x34
    virtual void setPause(bool flag, int fadeFrames) = 0; // vt  0x38

    virtual UNKWORD d_vt_0x3C() {
        return 0;
    }

    virtual UNKWORD d_vt_0x40() {
        return 0;
    }

    virtual UNKWORD d_vt_0x44() {
        return 0;
    }

    virtual const nw4r::math::VEC3 &getPositionRelativeToPlayer();                                      // vt 0x48
    virtual f32 getDistanceToPlayer();                                                                  // vt 0x4C
    virtual void updatePositionRelativeToPlayer();                                                      // vt 0x50
    virtual void updateDistanceToPlayer();                                                              // vt 0x54
    virtual void postStartSound(nw4r::snd::SoundHandle &handle, dSndSeSound_c *pSound, u32 id) = 0;          // vt 0x58
    virtual void postHoldSound(nw4r::snd::SoundHandle &handle, dSndSeSound_c *pSound, u32 id, UNKWORD) = 0; // vt 0x5C

    void resetCachedRelativePositions();

    void updatePositionTransformedByListener();
    void updateCameraDirectionDot();
    void updatePositionRelativeToCameraTarget();
    void calculatePositionRelativeToCameraTarget();
    void updateDistanceToCameraTarget();
    void calculatePositionRelativeToListener();
    void updatePositionRelativeToListener();
    void updateDistanceToListener();

    void updateListenerParameters() {
        calculatePositionRelativeToListener();
        mDistanceToListener = VECMag(mPositionRelativeToListener);
        setFlag(0x1 | 0x2);
    }

    void updateCameraTargetParameters() {
        calculatePositionRelativeToCameraTarget();
        mDistanceToCameraTarget = VECMag(mPositionRelativeToCameraTarget);
        setFlag(0x4 | 0x8);
    }

    u32 getSomeUserParam(u32 soundId) const;
    bool checkUserParamMask(u32 soundId, u32 mask) const;

    bool checkFlag(u16 flag) const {
        return (mFlags & flag);
    }

    void setFlag(u16 flag) {
        mFlags |= flag;
    }

    void setDisabled(bool value) {
        mIsDisabled = value;
    }

    f32 getDistanceToListener() {
        updateDistanceToListener();
        return mDistanceToListener;
    }

    f32 getSpeed() {
        if (!checkFlag(0x400)) {
            mSpeed = VECMag(GetVelocity());
            setFlag(0x400);
        }
        return mSpeed;
    }

    f32 getDistanceToCameraTarget() {
        updateDistanceToCameraTarget();
        return mDistanceToCameraTarget;
    }

    const dSndSourceParam *getSourceParam() const {
        return mpSourceParam;
    }
    void setSourceParam(const dSndSourceParam *param);

    bool hasPlayingSounds() const;
    bool isPlayingSound(u32 id);

    const char *soundIdToSoundLabel(u32 soundId) const;
    u32 soundLabelToSoundId(const char *soundLabel) const;

protected:
    // a_ prefix to prevent multiple inheritance clashes
    /* 0x7D */ u8 a_field_0x7D;
    /* 0x7E */ bool mIsDisabled;
    /* 0x7F */ bool mIsDemoActor;
    /* 0x80 */ bool mIsPaused;
    /* 0x84 */ f32 a_field_0x84;
    /* 0x88 */ f32 a_field_0x88;
    /* 0x8C */ f32 a_field_0x8C;
    /* 0x90 */ f32 a_field_0x90;
    /* 0x94 */ f32 mDistanceToListener;
    /* 0x98 */ f32 mCameraDirectionDot;
    /* 0x9C */ f32 mSpeed;
    /* 0xA0 */ nw4r::math::VEC3 mPositionRelativeToListener;
    /* 0xAC */ nw4r::math::VEC3 mPositionTransformedByListener;
    /* 0xB8 */ f32 mDistanceToCameraTarget;
    /* 0xBC */ nw4r::math::VEC3 mPositionRelativeToCameraTarget;
    /* 0xC8 */ dSndSourceParam *mpSourceParam;
    /* 0xCC */ u16 mFlags;
    /* 0xD0 */ nw4r::math::VEC3 mPositionRelativeToPlayer;
    /* 0xDC */ f32 mDistanceToPlayer;
    /* 0xE0 */ f32 mFxSend3D;
};

#endif
