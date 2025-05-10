#ifndef NW4R_G3D_RESANM_H
#define NW4R_G3D_RESANM_H
#include "common.h"

namespace nw4r {
namespace g3d {

/******************************************************************************
 *
 * ResKeyFrame
 *
 ******************************************************************************/
struct ResKeyFrameData {
    f32 frame; // at 0x0
    f32 value; // at 0x4
    f32 slope; // at 0x8
};

struct ResKeyFrameAnmData {
    u16 numKeyFrame;              // at 0x0
    u8 PADDING_0x2[0x4 - 0x2];    // at 0x2
    f32 invKeyFrameRange;         // at 0x4
    ResKeyFrameData keyFrames[1]; // at 0x8
};

namespace detail {

f32 GetResKeyFrameAnmResult(const ResKeyFrameAnmData *pData, f32 frame);

} // namespace detail

/******************************************************************************
 *
 * ResAnm
 *
 ******************************************************************************/
enum AnmPolicy {
    ANM_POLICY_ONETIME,
    ANM_POLICY_LOOP,
    ANM_POLICY_MAX
};

union ResAnmData {
    f32 constValue;           // at 0x0
    s32 toResKeyFrameAnmData; // at 0x0
};

namespace detail {

inline f32 GetResAnmResult(const ResAnmData *pData, f32 frame, bool constant) {
    if (constant) {
        return pData->constValue;
    }

    const ResKeyFrameAnmData *pFrameData = reinterpret_cast<const ResKeyFrameAnmData *>(
        reinterpret_cast<const char *>(pData) + pData->toResKeyFrameAnmData
    );

    return GetResKeyFrameAnmResult(pFrameData, frame);
}

template <typename T>
inline f32 ClipFrame(const T &rInfo, f32 frame) {
    if (frame <= 0.0f) {
        return 0.0f;
    }

    if (rInfo.numFrame <= frame) {
        return rInfo.numFrame;
    }

    return frame;
}

} // namespace detail

/******************************************************************************
 *
 * ResColorAnm
 *
 ******************************************************************************/
union ResColorAnmData {
    u32 constValue;              // at 0x0
    s32 toResColorAnmFramesData; // at 0x0
};
struct ResColorAnmFramesData {
    u32 frameColors[1]; // at 0x0
};

namespace detail {

u32 GetResColorAnmResult(const ResColorAnmFramesData *pData, f32 frame);

inline u32 GetResColorAnmResult(const ResColorAnmData *pData, f32 frame, bool constant) {
    if (constant) {
        return pData->constValue;
    }

    const ResColorAnmFramesData *pFrameData = reinterpret_cast<const ResColorAnmFramesData *>(
        reinterpret_cast<const char *>(pData) + pData->toResColorAnmFramesData
    );

    return GetResColorAnmResult(pFrameData, frame);
}

} // namespace detail

/******************************************************************************
 *
 * ResBoolAnm
 *
 ******************************************************************************/
union ResBoolAnmData {
    s32 toResBoolAnmFramesData; // at 0x0
};
struct ResBoolAnmFramesData {
    u32 boolBits[1]; // at 0x0
};

namespace detail {

inline bool GetResBoolAnmFramesResult(const ResBoolAnmFramesData *pData, int frame) {
    const u32 *pBits = pData->boolBits;
    u32 index = static_cast<u32>(frame);

    u32 wordIdx = index / 32;
    u32 bitIdx = index % 32;

    u32 targetBit = (1U << 31) >> bitIdx;
    u32 bitWord = pBits[wordIdx];

    return bitWord & targetBit;
}

inline bool GetResBoolAnmResult(const ResBoolAnmData *pData, int frame) {
    const ResBoolAnmFramesData *pFrameData = reinterpret_cast<const ResBoolAnmFramesData *>(
        reinterpret_cast<const char *>(pData) + pData->toResBoolAnmFramesData
    );

    return GetResBoolAnmFramesResult(pFrameData, frame);
}

} // namespace detail

} // namespace g3d
} // namespace nw4r

#endif
