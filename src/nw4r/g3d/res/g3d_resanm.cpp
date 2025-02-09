#include <nw4r/g3d.h>

#include <nw4r/math.h>
#include <nw4r/ut.h>

namespace nw4r {
namespace g3d {
namespace detail {
namespace {

/**
 * The Hermite interpolating polynomial f satisfies the following conditions:
 *        f(0)  = v0
 *        f(d)  = v1
 *        f'(0) = t0
 *        f'(d) = t1
 *
 * The value returned is f(p).
 */
inline f32 HermiteInterpolation(f32 v0, f32 t0, f32 v1, f32 t1, f32 p, f32 d) {
    f32 invd = math::FInv(d);

    // Linear factors
    f32 s = p * invd;   // p / d
    f32 s_1 = s - 1.0f; // (p - d) / d

    return v0 + s * (s * ((2.0f * s - 3.0f) * (v0 - v1))) +
           p * s_1 * (s_1 * t0 + s * t1);
}

/**
 * The linear interpolating function f satisfies the following conditions:
 *        f(0)      = a
 *        f(0x8000) = b
 *
 * The value returned is f(ratio).
 */
inline u8 LinearInterpColorElem(u8 a, u8 b, s16 ratio) {
    return a + ((b - a) * ratio >> 15);
}

} // namespace

f32 GetResKeyFrameAnmResult(const ResKeyFrameAnmData* pData, f32 frame) {
    const ResKeyFrameData& rFirst = pData->keyFrames[0];
    const ResKeyFrameData& rLast = pData->keyFrames[pData->numKeyFrame - 1];

    if (frame <= rFirst.frame) {
        return rFirst.value;
    }

    if (rLast.frame <= frame) {
        return rLast.value;
    }

    f32 frameOffset = frame - rFirst.frame;
    f32 numKeyFrame = math::U16ToF32(pData->numKeyFrame);

    f32 fEstimate = frameOffset * numKeyFrame * pData->invKeyFrameRange;
    u16 iEstimate = math::F32ToU16(fEstimate);

    const ResKeyFrameData* pLeft = &pData->keyFrames[iEstimate];

    if (frame < pLeft->frame) {
        do {
            pLeft--;
        } while (frame < pLeft->frame);
    } else {
        do {
            pLeft++;
        } while (pLeft->frame <= frame);

        pLeft--;
    }

    if (pLeft->frame == frame) {
        return pLeft->value;
    }

    const ResKeyFrameData* pRight = pLeft + 1;
    f32 curFrameDelta = frame - pLeft->frame;
    f32 keyFrameDelta = pRight->frame - pLeft->frame;

    return HermiteInterpolation(pLeft->value, pLeft->slope, pRight->value,
                                pRight->slope, curFrameDelta, keyFrameDelta);
}

u32 GetResColorAnmResult(const ResColorAnmFramesData* pData, f32 frame) {
    const u32* pColorArray = pData->frameColors;

    f32 intPart;
    f32 fracPart = math::FModf(frame, &intPart);
    int intFrame = static_cast<int>(intPart);

    if (fracPart == 0.0f) {
        return pColorArray[intFrame];
    }

    ut::Color left(pColorArray[intFrame]);
    ut::Color right(pColorArray[intFrame + 1]);

    f32 biasedRatio = 32768 * fracPart;
    s16 fpRatio = math::F32ToS16(biasedRatio);

    return ut::Color(LinearInterpColorElem(left.r, right.r, fpRatio),
                     LinearInterpColorElem(left.g, right.g, fpRatio),
                     LinearInterpColorElem(left.b, right.b, fpRatio),
                     LinearInterpColorElem(left.a, right.a, fpRatio));
}

} // namespace detail
} // namespace g3d
} // namespace nw4r
