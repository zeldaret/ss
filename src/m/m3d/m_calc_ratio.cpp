#include <m/m3d/m_calc_ratio.h>

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
    f32 max = 1.0f;
    f32 start = mf1;
    if (start == 0.0f) {
        return;
    }

    mf2 += mf3;
    if (mf2 >= 1.0f) {
        reset();
        return;
    } else {
        mb2 = 1;
        mf1 = start - start * mf2 * mf2;
        max = 1.0f - mf1;
        // TODO here be ASM
        f32 f4 = mf1 / start + max;
        f32 f1 = __fres(f4);
        f4 = -(f4 * f1 * f1 - (f1 + f1));
        mf4 = (mf1 / start) * f4;
        mf5 = max * f4;
    }
}

bool calcRatio_c::isEnd() const {
    return mf1 == 0.0f;
}

} // namespace m3d
