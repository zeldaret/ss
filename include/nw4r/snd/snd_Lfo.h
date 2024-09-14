#ifndef NW4R_SND_LFO_H
#define NW4R_SND_LFO_H
#include <nw4r/types_nw4r.h>

namespace nw4r {
namespace snd {
namespace detail {

struct LfoParam {
    LfoParam() {
        Init();
    }

    void Init();

    f32 depth;     // at 0x0
    f32 speed;     // at 0x4
    u32 delay;     // at 0x8
    u8 range;      // at 0xC
    u8 padding[3]; // at 0xD
};

class Lfo {
public:
    Lfo() : mDelayCounter(0), mCounter(0.0f) {}

    LfoParam& GetParam() {
        return mParam;
    }
    void SetParam(const LfoParam& rParam) {
        mParam = rParam;
    }

    void Reset();
    void Update(int msec);

    f32 GetValue() const;

private:
    static const int TABLE_SIZE = 32;

private:
    static s8 GetSinIdx(int i);

private:
    LfoParam mParam;   // at 0x0
    u32 mDelayCounter; // at 0x10
    f32 mCounter;      // at 0x14
};

} // namespace detail
} // namespace snd
} // namespace nw4r

#endif
