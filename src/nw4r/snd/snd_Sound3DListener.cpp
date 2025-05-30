#include "nw4r/snd/snd_Sound3DListener.h"

namespace nw4r {
namespace snd {

Sound3DListener::Sound3DListener()
    : mPosition(0.0f, 0.0f, 0.0f),
      mVelocity(0.0f, 0.0f, 0.0f),
      mInteriorSize(1.0f),
      mMaxVolumeDistance(1.0f),
      mUnitDistance(1.0f),
      field_0x54(0),
      mSkipVelocityUpdate(1),
      mUnitBiquadFilterValue(0.5f),
      mUnitBiquadFilterMax(1.0f) {
    math::MTX34Zero(&mMtx);
}

void Sound3DListener::SetMatrix(const math::MTX34 &mtx) {
    math::VEC3 prevPosition = mPosition;

    mPosition.x = -(mtx._00 * mtx._03 + mtx._10 * mtx._13 + mtx._20 * mtx._23);
    mPosition.y = -(mtx._01 * mtx._03 + mtx._11 * mtx._13 + mtx._21 * mtx._23);
    mPosition.z = -(mtx._02 * mtx._03 + mtx._12 * mtx._13 + mtx._22 * mtx._23);

    if (mSkipVelocityUpdate) {
        mMtx = mtx;
        mSkipVelocityUpdate = 0;
    } else {
        mMtx = mtx;
        VEC3Sub(&mVelocity, &mPosition, &prevPosition);
    }
}

void Sound3DListener::SetInteriorSize(f32 size) {
    mInteriorSize = size;
}

void Sound3DListener::SetMaxVolumeDistance(f32 distance) {
    mMaxVolumeDistance = distance;
}

void Sound3DListener::SetUnitDistance(f32 distance) {
    mUnitDistance = distance;
}

void Sound3DListener::SetUnitBiquadFilterValue(f32 value) {
    mUnitBiquadFilterValue = value;
}

void Sound3DListener::SetUnitBiquadFilterMax(f32 value) {
    mUnitBiquadFilterMax = value;
}

} // namespace snd
} // namespace nw4r
