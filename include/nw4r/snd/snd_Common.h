#ifndef NW4R_SND_COMMON_H
#define NW4R_SND_COMMON_H
#include <nw4r/types_nw4r.h>

namespace nw4r {
namespace snd {

static const int CHANNEL_MIN = 1;
static const int CHANNEL_MAX = 2;

static const int REMOTE_FILTER_MAX = 127;

// 12 notes each with 256 microtones
static const int SEMITONE_MAX = 12;
static const int MICROTONE_MAX = 256;

// Volume in range [-90.4db, 6.0db]
static const f32 VOLUME_MIN_DB = -90.4f;
static const f32 VOLUME_MAX_DB = 6.0f;
static const f32 VOLUME_RANGE_DB = -(VOLUME_MIN_DB - VOLUME_MAX_DB);
static const int VOLUME_RANGE_MB = static_cast<int>(10 * VOLUME_RANGE_DB);

enum OutputLineFlag {
    OUTPUT_LINE_MAIN = (1 << 0),
    OUTPUT_LINE_REMOTE_N = (1 << 1),
};

enum AuxBus { AUX_A, AUX_B, AUX_C, AUX_BUS_NUM };

enum OutputMode {
    OUTPUT_MODE_STEREO,
    OUTPUT_MODE_SURROUND,
    OUTPUT_MODE_DPL2,
    OUTPUT_MODE_MONO
};

struct SoundParam {
    f32 volume;      // at 0x0
    f32 pitch;       // at 0x4
    f32 pan;         // at 0x8
    f32 surroundPan; // at 0xC
    f32 fxSend;      // at 0x10
    f32 lpf;         // at 0x14
    int priority;    // at 0x18
};

namespace detail {

enum PanMode {
    PAN_MODE_DUAL,
    PAN_MODE_BALANCE,
};

enum PanCurve {
    PAN_CURVE_SQRT,
    PAN_CURVE_SQRT_0DB,
    PAN_CURVE_SQRT_0DB_CLAMP,
    PAN_CURVE_SINCOS,
    PAN_CURVE_SINCOS_0DB,
    PAN_CURVE_SINCOS_0DB_CLAMP,
    PAN_CURVE_LINEAR,
    PAN_CURVE_LINEAR_0DB,
    PAN_CURVE_LINEAR_0DB_CLAMP,
};

struct AdpcmParam {
    u16 coef[16];   // at 0x0
    u16 gain;       // at 0x20
    u16 pred_scale; // at 0x22
    u16 yn1;        // at 0x24
    u16 yn2;        // at 0x26
};

struct AdpcmLoopParam {
    u16 loop_pred_scale; // at 0x0
    u16 loop_yn1;        // at 0x2
    u16 loop_yn2;        // at 0x4
};

struct AdpcmInfo {
    AdpcmParam param;         // at 0x0
    AdpcmLoopParam loopParam; // at 0x28
    u16 padding;
};

} // namespace detail
} // namespace snd
} // namespace nw4r

#endif
