#ifndef M3D_M_CALC_RATIO_H
#define M3D_M_CALC_RATIO_H

#include "egg/core/eggFrmHeap.h"
#include "m/m_allocator.h"

namespace m3d {

class calcRatio_c {
public:
    calcRatio_c();
    virtual ~calcRatio_c();

    void remove();
    void reset();
    void offUpdate();
    void set(f32);
    void calc();
    bool isEnd() const;

    f32 get0x10() const {
        return mf4;
    }

    f32 get0x14() const {
        return mf5;
    }

    bool is0x18() const {
        return mb1;
    }

    bool is0x19() const {
        return mb2;
    }

private:
    /* 0x04 */ f32 mf1;
    /* 0x08 */ f32 mf2;
    /* 0x0C */ f32 mf3;
    /* 0x10 */ f32 mf4;
    /* 0x14 */ f32 mf5;
    /* 0x18 */ bool mb1;
    /* 0x19 */ bool mb2;
};

} // namespace m3d

#endif
