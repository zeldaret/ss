#ifndef NW4R_SND_LFO_H
#define NW4R_SND_LFO_H
#include <nw4r/types_nw4r.h>

namespace nw4r {
namespace snd {
namespace detail {

struct LfoParam {
    LfoParam() { Init(); }
    void Init();

    f32 depth; // at 0x0
    f32 speed; // at 0x4
    u32 delay; // at 0x8
    u8 range;  // at 0xC
};

class Lfo {
public:
    static s8 GetSinIdx(int i);

    Lfo() : mDelayCounter(0), mCounter(0.0f) {}

    LfoParam& GetParam() { return mParam; }
    void SetParam(const LfoParam& param) { mParam = param; }

    void Reset();
    void Update(int msec);

    f32 GetValue() const;

private:
    LfoParam mParam;   // at 0x0
    u32 mDelayCounter; // at 0x10
    f32 mCounter;      // at 0x14
};

} // namespace detail
} // namespace snd
} // namespace nw4r

#endif
