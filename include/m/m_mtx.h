#ifndef M_MTX_H
#define M_MTX_H

// NOTE:: Comments about functions pulled from NSMBW

#include "egg/math/eggMatrix.h"
#include "m/m_angle.h"
#include "nw4r/nw4r_types.h"
#include <common.h>

#pragma push
#pragma warning off(10402)
class mMtx_c {
    typedef f32 (*MtxRef)[4];
    typedef const f32 (*MtxRefConst)[4];

public:
    mMtx_c(){};
    /* 802f1660 */ mMtx_c(f32 xx, f32 xy, f32 xz, f32 xw, f32 yx, f32 yy, f32 yz, f32 yw, f32 zx, f32 zy, f32 zz,
            f32 zw);

    // not sure if this breaks anything but we need a matrix type
    // with an inline copy assignment operator
    void set(const mMtx_c &r) {
        xx = r.xx;
        xy = r.xy;
        xz = r.xz;
        xw = r.xw;

        yx = r.yx;
        yy = r.yy;
        yz = r.yz;
        yw = r.yw;

        zx = r.zx;
        zy = r.zy;
        zz = r.zz;
        zw = r.zw;
    }

    inline operator MtxRef() {
        return m;
    }
    inline operator MtxRefConst() const {
        return m;
    }
    operator nw4r::math::MTX34 *() {
        return &nw4rm;
    }

    /* 802f16b0 */ void XrotS(const mAng &angle); ///< Generates a rotation matrix for the X axis with the given angle.
    /* 802f1770 */ void XrotM(const mAng &angle); ///< Rotates the matrix on the X axis by the given angle.
    /* 802f17c0 */ void YrotS(const mAng &angle); ///< Generates a rotation matrix for the Y axis with the given angle.
    /* 802f1880 */ void YrotM(const mAng &angle); ///< Rotates the matrix on the Y axis by the given angle.
    /* 802f18d0 */ void ZrotS(const mAng &angle); ///< Generates a rotation matrix for the Z axis with the given angle.
    /* 802f1990 */ void ZrotM(const mAng &angle); ///< Rotates the matrix on the Z axis by the given angle.

    /* 802f19e0 */ void ZXYrotS(mAng xRot, mAng yRot,
            mAng zRot); ///< Generates the matrix on the Y, X and Z axes by the given angles.
    /* 802f1a40 */ void ZXYrotM(mAng xRot, mAng yRot,
            mAng zRot); ///< Rotates the matrix on the Y, X and Z axes by the given angles.
    /* 802f1aa0 */ void XYZrotS(mAng xRot, mAng yRot,
            mAng zRot); ///< Generates the matrix on the Z, Y and X axes by the given angles.
    /* 802f1b00 */ void XYZrotM(mAng xRot, mAng yRot,
            mAng zRot); ///< Rotates the matrix on the Z, Y and X axes by the given angles.

    /* 802f1b60 */ void toRot(mAng3_c &out) const; ///< Converts the matrix to a rotation vector.

    void multVecZero(nw4r::math::VEC3 &out) const; ///< Converts the matrix to a vector.
    void zero();                                   ///< Zeroes out the matrix.

    /* 802f1c40 */ void rot(int, int); // does some werrd operation to rotate the matrix
    /* 802f1e60 */ bool quatRelated();

public:
    union {
        EGG::Matrix34f mat;
        f32 m[3][4];
        nw4r::math::MTX34 nw4rm;
        struct {
            f32 xx, xy, xz, xw;
            f32 yx, yy, yz, yw;
            f32 zx, zy, zz, zw;
        };
    };

public:
    static mMtx_c Identity;
};

#pragma pop

#endif
