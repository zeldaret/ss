#ifndef M_QUAT_H
#define M_QUAT_H

#include <egg/math/eggQuat.h>
#include <m/m_vec.h>

class mQuat_c : public EGG::Quatf {
public:
    mQuat_c() {}
    mQuat_c(f32 x, f32 y, f32 z, f32 w): EGG::Quatf(w, x, y, z) {}
    void fn_802F2780(const mQuat_c &other);
};

#endif
