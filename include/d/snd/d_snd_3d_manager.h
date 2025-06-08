#ifndef D_SND_3D_MANAGER_H
#define D_SND_3D_MANAGER_H

#include "d/snd/d_snd_3d_engine.h"
#include "d/snd/d_snd_util.h"
#include "egg/gfx/eggCamera.h"
#include "m/m_vec.h"
#include "nw4r/math/math_types.h"
#include "nw4r/snd/snd_Sound3DListener.h"
#include "nw4r/snd/snd_Sound3DManager.h"

SND_DISPOSER_FORWARD_DECL(dSnd3DManager_c);

class dSnd3DManager_c {
    SND_DISPOSER_MEMBERS(dSnd3DManager_c);

public:
    dSnd3DManager_c();

    void setup();
    void updateFromCamera(EGG::LookAtCamera &camera);
    void setCamDistance(f32 value);
    void resetCamDistance();
    void calc();
    void clearState();

    nw4r::snd::Sound3DManager &getManager() {
        return mManager;
    }

    const mVec3_c &getCameraTargetPos() const {
        return mCameraTargetPosition;
    }

    const mVec3_c &getNrmCameraDirection() const {
        return mCameraDirectionNormalized;
    }

    const nw4r::math::VEC3 &getSndListenerPos() const {
        return mListener.GetPosition();
    }

    const nw4r::math::MTX34 &getSndListenerMatrix() const {
        return mListener.GetMatrix();
    }

private:
    void updateListenerPos(f32);
    /* 0x010 */ bool mIsSetup;
    /* 0x011 */ bool mCalculationsFresh;
    /* 0x014 */ dSnd3DEngine_c *mpEngine;
    /* 0x018 */ nw4r::snd::Sound3DManager mManager;
    /* 0x044 */ nw4r::snd::Sound3DListener mListener;
    /* 0xB0 */ EGG::LookAtCamera mCamera;
    /* 0x138 */ mVec3_c mCameraDirectionNormalized;
    /* 0x144 */ mVec3_c mCameraTargetPosition;
    /* 0x150 */ f32 mCameraPosSqVelocity;
    /* 0x154 */ f32 mCameraAtSqVelocity;
    /* 0x158 */ f32 mCamDistance;
    /* 0x15C */ s16 mTimer;
};

#endif
