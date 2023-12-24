#include <c/c_math.h>
#include <m/m_math.h>
#include <nw4r/math/arithmetic.h>

void mMtx_c::toRot(mAng3_c &out) const {
    f32 t1 = 0.0f;
    f32 t0 = m[0][2] + m[2][2];
    if (t0 > 0.0f) {
        t1 = t0 * nw4r::math::FrSqrt(t0);
    }
    s16 anglex = cM::atan2s(-m[1][2], t1);
}
