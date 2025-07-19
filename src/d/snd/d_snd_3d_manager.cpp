#include "d/snd/d_snd_3d_manager.h"

#include "d/snd/d_snd_3d_engine.h"
#include "d/snd/d_snd_mgr.h"
#include "d/snd/d_snd_state_mgr.h"
#include "egg/math/eggMatrix.h"
#include "egg/math/eggVector.h"
#include "nw4r/math/math_arithmetic.h"
#include "nw4r/math/math_types.h"
#include "nw4r/snd/snd_SoundArchive.h"

SND_DISPOSER_DEFINE(dSnd3DManager_c);

dSnd3DManager_c::dSnd3DManager_c() : mIsSetup(false), mCalculationsFresh(0) {
    mCameraPosSqVelocity = 0.0f;
    mCameraAtSqVelocity = 0.0f;
    mCamDistance = 0.9f;
    mTimer = 0;
    mpEngine = new dSnd3DEngine_c();
    mCameraDirectionNormalized.x = 0.0f;
    mCameraDirectionNormalized.y = 0.0f;
    mCameraDirectionNormalized.z = 1.0f;
}

void dSnd3DManager_c::setup() {
    if (mIsSetup) {
        return;
    }

    nw4r::snd::SoundArchive *archive = dSndMgr_c::GetInstance()->getArchive();
    u32 requiredSize = mManager.GetRequiredMemSize(archive);
    void *buf = dSndMgr_c::GetInstance()->getSoundHeap()->Alloc(requiredSize);
    mManager.Setup(archive, buf, requiredSize);
    mManager.SetEngine(mpEngine);
    mManager.SetMaxPriorityReduction(32);
    mManager.SetField0x20(0.9f);
    mManager.SetField0x24(3400.0f / 3.0f);
    mListener.SetMaxVolumeDistance(300.0f);
    mListener.SetUnitDistance(1000.0f);
    mListener.SetInteriorSize(400.0f);
    mManager.SetBiquadFilterType(3);
    mListener.SetUnitBiquadFilterMax(0.5f);
    mListener.SetUnitBiquadFilterValue(0.2f);
    mManager.GetListenerList().Insert(mManager.GetListenerList().GetEndIter(), &mListener);
    mIsSetup = true;
}

void dSnd3DManager_c::resetCamDistance() {
    mCamDistance = 0.9f;
}

void dSnd3DManager_c::setCamDistance(f32 value) {
    // @bug ? not actually clamped
    (void)nw4r::ut::Clamp(value, -1.0f, 2.0f);
    mCamDistance = value;
}

void dSnd3DManager_c::calc() {
    // Don't actually calculate anything,
    // but allow re-calculations this frame.
    mCalculationsFresh = false;
}

void dSnd3DManager_c::clearState() {
    mCameraPosSqVelocity = 0.0f;
    mCameraAtSqVelocity = 0.0f;
    mTimer = 0;
}


void dSnd3DManager_c::updateFromCamera(EGG::LookAtCamera &camera) {
    if (mCalculationsFresh) {
        return;
    }

    f32 dist = 0.5f;
    if (dSndStateMgr_c::GetInstance()->isInEvent()) {
        dist = mCamDistance;
        f32 prevCameraAtSqVelocity = mCameraAtSqVelocity;
        f32 prevCameraPosSqVelocity = mCameraPosSqVelocity;
        mCameraAtSqVelocity = VECSquareDistance(camera.mAt, mCamera.mAt);
        mCameraPosSqVelocity = VECSquareDistance(camera.mPos, mCamera.mPos);
        if (dSndStateMgr_c::GetInstance()->getFrameCounter() > 30) {
            bool bigMovement = false;
            bool hugeMovement = false;
            if (mTimer > 0) {
                mTimer--;
            } else {
                f32 atAccel = prevCameraAtSqVelocity - mCameraAtSqVelocity;
                f32 atAccelAbs = nw4r::math::FAbs(atAccel);
                if (mCameraAtSqVelocity > 490000.0f) {
                    bigMovement = true;
                    hugeMovement = true;
                } else if (atAccelAbs > 10000.0f) {
                    bigMovement = true;
                }
                if (!hugeMovement) {
                    f32 posAccel = prevCameraPosSqVelocity - mCameraPosSqVelocity;
                    f32 posAccelAbs = nw4r::math::FAbs(posAccel);
                    if (mCameraPosSqVelocity > 490000.0f) {
                        bigMovement = true;
                        hugeMovement = true;
                    } else if (posAccelAbs > 10000.0f) {
                        bigMovement = true;
                    }
                    if (bigMovement && !hugeMovement && nw4r::math::FAbs(atAccelAbs - posAccelAbs) < 22500.0f) {
                        nw4r::math::VEC3 myDir = mCamera.getOtherDirection();
                        nw4r::math::VEC3 camDir = camera.getOtherDirection();
                        VECNormalize(myDir, myDir);
                        VECNormalize(camDir, camDir);
                        if (nw4r::math::VEC3Dot(&myDir, &camDir) > 0.99f) {
                            bigMovement = false;
                        }
                    }
                }

                if (bigMovement) {
                    dSndStateMgr_c::GetInstance()->onCameraCut(-1);
                    mTimer = 30;
                }
            }
        }
    }

    mCamera = camera;
    updateListenerPos(dist);
    mCalculationsFresh = true;
}

void dSnd3DManager_c::updateListenerPos(f32 f) {
    const EGG::Matrix34f &mtx = mCamera.getViewMatrix();
    mListener.SetMatrix(*mtx);
    EGG::Vector3f dir = mCamera.getDirection();
    VECNormalize(dir, mCameraDirectionNormalized);
    dir *= f;
    mCameraTargetPosition = mCamera.mPos + dir;
}
