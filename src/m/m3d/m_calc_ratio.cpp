#include <m/m3d/m_calc_ratio.h>
#include <nw4r/math/math_arithmetic.h>

namespace m3d {

calcRatio_c::calcRatio_c() {
    mf1 = 0.0f;
    mf2 = 1.0f;
    mf3 = 0.0f;
    mf4 = 0.0f;
    mf5 = 1.0f;
    mb1 = 0;
    mb2 = 0;
}

calcRatio_c::~calcRatio_c() {}

void calcRatio_c::remove() {
    mb1 = 0;
    reset();
}

void calcRatio_c::reset() {
    mf1 = 0.0f;
    mf2 = 1.0f;
    mf4 = 0.0f;
    mf5 = 1.0f;
}

void calcRatio_c::offUpdate() {
    mb1 = 1;
    mb2 = 0;
}

void calcRatio_c::set(f32 value) {
    if (value == 0.0f) {
        reset();
        return;
    }
    mf1 = 1.0f;
    mf2 = 0.0f;
    mf4 = 1.0f;
    mf5 = 0.0f;
    mb2 = 1;
    mf3 = 1.0f / value;
}

void calcRatio_c::calc() {
    if (mf1 == 0.0f) {
        return;
    }

    mf2 += mf3;
    if (mf2 >= 1.0f) {
        reset();
    } else {
        mb2 = 1;
        f32 start = mf1;
        f32 tmp = start - start * mf2 * mf2;
        mf1 = tmp;
        f32 max = 1.0f - mf1;
        tmp = mf1 / start;
        f32 inv = nw4r::math::FInv(tmp + max);
        mf4 = tmp * inv;
        mf5 = max * inv;
    }
}

bool calcRatio_c::isEnd() const {
    return mf1 == 0.0f;
}

} // namespace m3d
