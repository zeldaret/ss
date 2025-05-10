#include "nw4r/g3d.h" // IWYU pragma: export

namespace nw4r {
namespace g3d {
namespace detail {
namespace dcc {
namespace {

void MakeTexSrtMtx_S(math::MTX34 *pMtx, const TexSrt &rSrt) {
    pMtx->_00 = rSrt.Su;
    pMtx->_01 = 0.0f;
    pMtx->_02 = 0.0f;
    pMtx->_03 = 0.5f * (1.0f - rSrt.Su);

    pMtx->_10 = 0.0f;
    pMtx->_11 = rSrt.Sv;
    pMtx->_12 = 0.0f;
    pMtx->_13 = 0.5f * (1.0f - rSrt.Sv);
}

void MakeTexSrtMtx_R(math::MTX34 *pMtx, const TexSrt &rSrt) {
    f32 sinR, cosR;
    math::SinCosDeg(&sinR, &cosR, rSrt.R);

    pMtx->_00 = cosR;
    pMtx->_01 = sinR;
    pMtx->_02 = 0.0f;
    pMtx->_03 = -0.5f * (cosR + sinR - 1.0f);

    pMtx->_10 = -sinR;
    pMtx->_11 = cosR;
    pMtx->_12 = 0.0f;
    pMtx->_13 = -0.5f * (-sinR + cosR - 1.0f);
}

void MakeTexSrtMtx_T(math::MTX34 *pMtx, const TexSrt &rSrt) {
    pMtx->_00 = 1.0f;
    pMtx->_01 = 0.0f;
    pMtx->_02 = 0.0f;
    pMtx->_03 = -rSrt.Tu;

    pMtx->_10 = 0.0f;
    pMtx->_11 = 1.0f;
    pMtx->_12 = 0.0f;
    pMtx->_13 = rSrt.Tv;
}

void MakeTexSrtMtx_SR(math::MTX34 *pMtx, const TexSrt &rSrt) {
    f32 sinR, cosR;
    math::SinCosDeg(&sinR, &cosR, rSrt.R);

    f32 sucr = rSrt.Su * cosR;
    f32 susr = rSrt.Su * sinR;
    f32 svcr = rSrt.Sv * cosR;
    f32 svsr = rSrt.Sv * sinR;

    pMtx->_00 = sucr;
    pMtx->_01 = susr;
    pMtx->_02 = 0.0f;
    pMtx->_03 = -0.5f * (sucr + susr - 1.0f);

    pMtx->_10 = -svsr;
    pMtx->_11 = svcr;
    pMtx->_12 = 0.0f;
    pMtx->_13 = -0.5f * (-svsr + svcr - 1.0f);
}

void MakeTexSrtMtx_RT(math::MTX34 *pMtx, const TexSrt &rSrt) {
    f32 sinR, cosR;
    math::SinCosDeg(&sinR, &cosR, rSrt.R);

    pMtx->_00 = cosR;
    pMtx->_01 = sinR;
    pMtx->_02 = 0.0f;
    pMtx->_03 = -cosR * (0.5f + rSrt.Tu) + sinR * (rSrt.Tv - 0.5f) + 0.5f;

    pMtx->_10 = -sinR;
    pMtx->_11 = cosR;
    pMtx->_12 = 0.0f;
    pMtx->_13 = sinR * (0.5f + rSrt.Tu) + cosR * (rSrt.Tv - 0.5f) + 0.5f;
}

void MakeTexSrtMtx_ST(math::MTX34 *pMtx, const TexSrt &rSrt) {
    pMtx->_00 = rSrt.Su;
    pMtx->_01 = 0.0f;
    pMtx->_02 = 0.0f;
    pMtx->_03 = -rSrt.Su * (0.5f + rSrt.Tu) + 0.5f;

    pMtx->_10 = 0.0f;
    pMtx->_11 = rSrt.Sv;
    pMtx->_12 = 0.0f;
    pMtx->_13 = rSrt.Sv * (rSrt.Tv - 0.5f) + 0.5f;
}

void MakeTexSrtMtx_SRT(math::MTX34 *pMtx, const TexSrt &rSrt) {
    f32 sinR, cosR;
    math::SinCosDeg(&sinR, &cosR, rSrt.R);

    f32 sucr = rSrt.Su * cosR;
    f32 susr = rSrt.Su * sinR;
    f32 svcr = rSrt.Sv * cosR;
    f32 svsr = rSrt.Sv * sinR;

    pMtx->_00 = sucr;
    pMtx->_01 = susr;
    pMtx->_02 = 0.0f;
    pMtx->_03 = -sucr * (0.5f + rSrt.Tu) + susr * (rSrt.Tv - 0.5f) + 0.5f;

    pMtx->_10 = -svsr;
    pMtx->_11 = svcr;
    pMtx->_12 = 0.0f;
    pMtx->_13 = svsr * (0.5f + rSrt.Tu) + svcr * (rSrt.Tv - 0.5f) + 0.5f;
}

void ProductTexSrtMtx_S(math::MTX34 *pMtx, const TexSrt &rSrt) {
    pMtx->_00 *= rSrt.Su;
    pMtx->_01 *= rSrt.Su;
    pMtx->_02 *= rSrt.Su;
    pMtx->_03 = rSrt.Su * (pMtx->_03 - 0.5f) + 0.5f;

    pMtx->_10 *= rSrt.Sv;
    pMtx->_11 *= rSrt.Sv;
    pMtx->_12 *= rSrt.Sv;
    pMtx->_13 = rSrt.Sv * (pMtx->_13 - 0.5f) + 0.5f;
}

void ProductTexSrtMtx_R(math::MTX34 *pMtx, const TexSrt &rSrt) {
    f32 sinR, cosR;
    f32 _0x, _1x;
    math::SinCosDeg(&sinR, &cosR, rSrt.R);

    _0x = cosR * pMtx->_00 + sinR * pMtx->_10;
    _1x = -sinR * pMtx->_00 + cosR * pMtx->_10;
    pMtx->_00 = _0x;
    pMtx->_10 = _1x;

    _0x = cosR * pMtx->_01 + sinR * pMtx->_11;
    _1x = -sinR * pMtx->_01 + cosR * pMtx->_11;
    pMtx->_01 = _0x;
    pMtx->_11 = _1x;

    _0x = cosR * pMtx->_02 + sinR * pMtx->_12;
    _1x = -sinR * pMtx->_02 + cosR * pMtx->_12;
    pMtx->_02 = _0x;
    pMtx->_12 = _1x;

    _0x = cosR * (pMtx->_03 - 0.5f) + sinR * (pMtx->_13 - 0.5f) + 0.5f;
    _1x = -sinR * (pMtx->_03 - 0.5f) + cosR * (pMtx->_13 - 0.5f) + 0.5f;
    pMtx->_03 = _0x;
    pMtx->_13 = _1x;
}

void ProductTexSrtMtx_T(math::MTX34 *pMtx, const TexSrt &rSrt) {
    pMtx->_03 -= rSrt.Tu;
    pMtx->_13 += rSrt.Tv;
}

void ProductTexSrtMtx_SR(math::MTX34 *pMtx, const TexSrt &rSrt) {
    f32 sinR, cosR;
    f32 _0x, _1x;
    math::SinCosDeg(&sinR, &cosR, rSrt.R);

    f32 sucr = rSrt.Su * cosR;
    f32 susr = rSrt.Su * sinR;
    f32 svcr = rSrt.Sv * cosR;
    f32 svsr = rSrt.Sv * sinR;

    _0x = sucr * pMtx->_00 + susr * pMtx->_10;
    _1x = -svsr * pMtx->_00 + svcr * pMtx->_10;
    pMtx->_00 = _0x;
    pMtx->_10 = _1x;

    _0x = sucr * pMtx->_01 + susr * pMtx->_11;
    _1x = -svsr * pMtx->_01 + svcr * pMtx->_11;
    pMtx->_01 = _0x;
    pMtx->_11 = _1x;

    _0x = sucr * pMtx->_02 + susr * pMtx->_12;
    _1x = -svsr * pMtx->_02 + svcr * pMtx->_12;
    pMtx->_02 = _0x;
    pMtx->_12 = _1x;

    _0x = sucr * (pMtx->_03 - 0.5f) + susr * (pMtx->_13 - 0.5f) + 0.5f;
    _1x = -svsr * (pMtx->_03 - 0.5f) + svcr * (pMtx->_13 - 0.5f) + 0.5f;

    pMtx->_03 = _0x;
    pMtx->_13 = _1x;
}

void ProductTexSrtMtx_RT(math::MTX34 *pMtx, const TexSrt &rSrt) {
    f32 sinR, cosR;
    f32 _0x, _1x;
    math::SinCosDeg(&sinR, &cosR, rSrt.R);

    _0x = cosR * pMtx->_00 + sinR * pMtx->_10;
    _1x = -sinR * pMtx->_00 + cosR * pMtx->_10;
    pMtx->_00 = _0x;
    pMtx->_10 = _1x;

    _0x = cosR * pMtx->_01 + sinR * pMtx->_11;
    _1x = -sinR * pMtx->_01 + cosR * pMtx->_11;
    pMtx->_01 = _0x;
    pMtx->_11 = _1x;

    _0x = cosR * pMtx->_02 + sinR * pMtx->_12;
    _1x = -sinR * pMtx->_02 + cosR * pMtx->_12;
    pMtx->_02 = _0x;
    pMtx->_12 = _1x;

    _0x = cosR * (pMtx->_03 - rSrt.Tu - 0.5f) + sinR * (pMtx->_13 + rSrt.Tv - 0.5f) + 0.5f;

    _1x = -sinR * (pMtx->_03 - rSrt.Tu - 0.5f) + cosR * (pMtx->_13 + rSrt.Tv - 0.5f) + 0.5f;

    pMtx->_03 = _0x;
    pMtx->_13 = _1x;
}

void ProductTexSrtMtx_ST(math::MTX34 *pMtx, const TexSrt &rSrt) {
    pMtx->_00 *= rSrt.Su;
    pMtx->_01 *= rSrt.Su;
    pMtx->_02 *= rSrt.Su;
    pMtx->_03 = rSrt.Su * (pMtx->_03 - rSrt.Tu - 0.5f) + 0.5f;

    pMtx->_10 *= rSrt.Sv;
    pMtx->_11 *= rSrt.Sv;
    pMtx->_12 *= rSrt.Sv;
    pMtx->_13 *= rSrt.Sv * (pMtx->_13 + rSrt.Tv - 0.5f) + 0.5f;
}

void ProductTexSrtMtx_SRT(math::MTX34 *pMtx, const TexSrt &rSrt) {
    f32 sinR, cosR;
    f32 _0x, _1x;
    math::SinCosDeg(&sinR, &cosR, rSrt.R);

    f32 sucr = rSrt.Su * cosR;
    f32 susr = rSrt.Su * sinR;
    f32 svcr = rSrt.Sv * cosR;
    f32 svsr = rSrt.Sv * sinR;

    _0x = sucr * pMtx->_00 + susr * pMtx->_10;
    _1x = -svsr * pMtx->_00 + svcr * pMtx->_10;
    pMtx->_00 = _0x;
    pMtx->_10 = _1x;

    _0x = sucr * pMtx->_01 + susr * pMtx->_11;
    _1x = -svsr * pMtx->_01 + svcr * pMtx->_11;
    pMtx->_01 = _0x;
    pMtx->_11 = _1x;

    _0x = sucr * pMtx->_02 + susr * pMtx->_12;
    _1x = -svsr * pMtx->_02 + svcr * pMtx->_12;
    pMtx->_02 = _0x;
    pMtx->_12 = _1x;

    _0x = sucr * (pMtx->_03 - rSrt.Tu - 0.5f) + susr * (pMtx->_13 + rSrt.Tv - 0.5f) + 0.5f;

    _1x = -svsr * (pMtx->_03 - rSrt.Tu - 0.5f) + svcr * (pMtx->_13 + rSrt.Tv - 0.5f) + 0.5f;

    pMtx->_03 = _0x;
    pMtx->_13 = _1x;
}

} // namespace

typedef void (*CalcTexMtxFunc)(math::MTX34 *pMtx, const TexSrt &rSrt);

bool CalcTexMtx_3dsmax(math::MTX34 *pMtx, bool set, const TexSrt &rSrt, TexSrt::Flag flag) {
    // Extract S/R/T flags
    u32 index = flag >> 1 & 0b111;

    // Scale-one, no rotate/trans
    if (index == 0b111) {
        return false;
    }

    if (set) {
        static const CalcTexMtxFunc funcTable[] = {
            MakeTexSrtMtx_SRT, // 0 0 0
            MakeTexSrtMtx_RT,  // 0 0 1
            MakeTexSrtMtx_ST,  // 0 1 0
            MakeTexSrtMtx_T,   // 0 1 1
            MakeTexSrtMtx_SR,  // 1 0 0
            MakeTexSrtMtx_R,   // 1 0 1
            MakeTexSrtMtx_S    // 1 1 0
        };

        funcTable[index](pMtx, rSrt);
    } else {
        static const CalcTexMtxFunc funcTable[] = {
            ProductTexSrtMtx_SRT, // 0 0 0
            ProductTexSrtMtx_RT,  // 0 0 1
            ProductTexSrtMtx_ST,  // 0 1 0
            ProductTexSrtMtx_T,   // 0 1 1
            ProductTexSrtMtx_SR,  // 1 0 0
            ProductTexSrtMtx_R,   // 1 0 1
            ProductTexSrtMtx_S    // 1 1 0
        };

        funcTable[index](pMtx, rSrt);
    }

    pMtx->_20 = 0.0f;
    pMtx->_21 = 0.0f;
    pMtx->_22 = 1.0f;
    pMtx->_23 = 0.0f;

    return true;
}

} // namespace dcc
} // namespace detail
} // namespace g3d
} // namespace nw4r
