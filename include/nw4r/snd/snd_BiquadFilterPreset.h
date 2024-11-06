#ifndef NW4R_SND_BIQUAD_FILTER_PRESET_H
#define NW4R_SND_BIQUAD_FILTER_PRESET_H
#include "nw4r/types_nw4r.h"
#include "nw4r/ut.h"


namespace nw4r {
namespace snd {
namespace detail {

class BiquadFilterCallback {
public:
    struct BiquadCoef {
        u16 b0;
        u16 b1;
        u16 b2;
        u16 a1;
        u16 a2;
    };

    virtual ~BiquadFilterCallback() {}
    virtual void GetCoef(int type, f32 value, BiquadCoef *coef) = 0;
};

class BiquadFilterLpf : public BiquadFilterCallback {
public:
    virtual ~BiquadFilterLpf() {}
    virtual void GetCoef(int type, f32 value, BiquadCoef *coef) override;
};

class BiquadFilterHpf : public BiquadFilterCallback {
public:
    virtual ~BiquadFilterHpf() {}
    virtual void GetCoef(int type, f32 value, BiquadCoef *coef) override;
};

class BiquadFilterBpf512 : public BiquadFilterCallback {
public:
    virtual ~BiquadFilterBpf512() {}
    virtual void GetCoef(int type, f32 value, BiquadCoef *coef) override;
};

class BiquadFilterBpf1024 : public BiquadFilterCallback {
public:
    virtual ~BiquadFilterBpf1024() {}
    virtual void GetCoef(int type, f32 value, BiquadCoef *coef) override;
};

class BiquadFilterBpf2048 : public BiquadFilterCallback {
public:
    virtual ~BiquadFilterBpf2048() {}
    virtual void GetCoef(int type, f32 value, BiquadCoef *coef) override;
};

} // namespace detail
} // namespace snd
} // namespace nw4r

#endif
