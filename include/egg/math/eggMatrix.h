#pragma once

#include "types.h"
#include "egg/math/eggVector.h"
#include "egg/math/eggQuat.h"

namespace EGG
{

// Maybe inherits 
class Matrix34f {
public:
    f32 mData[3][4];
public:
    Matrix34f() {};
    f32& operator()(int i, int j) {return mData[i][j]; }
    /* 8049ac70 */ bool InverseTo(Matrix34f* dest) const;
    /* 8049ac80 */ bool InverseTransposeTo(Matrix34f* dest) const;
    /* 8049ac90 */ void makeIdentity();
    /* 8049acd0 */ Matrix34f( \
            f32 fxx, f32 fxy, f32 fxz, f32 fxw, \
            f32 fyx, f32 fyy, f32 fyz, f32 fyw, \
            f32 fzx, f32 fzy, f32 fzz, f32 fzw);
    /* 8049ad20 */ void makeQT(const Quatf&, const Vector3f&);
    /* 8049ade0 */ void makeQ(const Quatf&); // these could be swapped
    /* 8049ae90 */ void fromQuat(const Quatf& from); // these could be swapped
    /* 8049af40 */ void toQuat(Quatf& dest);
    /* 8049b250 */ void slerpTo(const Matrix34f&, Matrix34f&, f32) const;
    /* 8049b2d0 */ void setAxisRotation(const Vector3f&, f32);
    /* 8049b310 */ void loadPosMtx(u32);
    /* 8049b320 */ void multiplyTo(const Matrix34f&, Matrix34f& out);
public:
    /* 80674c00 */ static Matrix34f ident;
};

} // namespace EGG
