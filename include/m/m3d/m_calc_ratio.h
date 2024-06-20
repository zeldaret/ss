#ifndef M3D_M_CALC_RATIO_H
#define M3D_M_CALC_RATIO_H

#include <egg/core/eggFrmHeap.h>
#include <m/m_allocator.h>

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

private:
    f32 mf1;
    f32 mf2;
    f32 mf3;
    f32 mf4;
    f32 mf5;
    u8 mb1;
    u8 mb2;
};

} // namespace m3d

#endif
