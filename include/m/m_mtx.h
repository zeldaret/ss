#ifndef M_MTX_H
#define M_MTX_H

// NOTE:: Comments about functions pulled from NSMBW

#include "common.h"
#include "egg/math/eggMatrix.h"
#include "m/m_angle.h"
#include "m/m_vec.h"
#include "nw4r/types_nw4r.h"

class mMtx_c : public EGG::Matrix34f {
    typedef f32 (*MtxRef)[4];
    typedef const f32 (*MtxRefConst)[4];

public:
    mMtx_c(){};
    mMtx_c(f32 xx, f32 xy, f32 xz, f32 xw, f32 yx, f32 yy, f32 yz, f32 yw, f32 zx, f32 zy, f32 zz, f32 zw);

    operator MtxRef() {
        return (MtxRef)(this);
    }
    operator MtxRefConst() const {
        return (MtxRefConst)(this);
    }
    operator nw4r::math::MTX34 *() {
        return (nw4r::math::MTX34 *)(this);
    }
    operator nw4r::math::MTX34 &() {
        return *(nw4r::math::MTX34 *)(this);
    }

    void XrotS(const mAng &angle); ///< Generates a rotation matrix for the X axis with the given angle.
    void XrotM(const mAng &angle); ///< Rotates the matrix on the X axis by the given angle.
    void YrotS(const mAng &angle); ///< Generates a rotation matrix for the Y axis with the given angle.
    void YrotM(const mAng &angle); ///< Rotates the matrix on the Y axis by the given angle.
    void ZrotS(const mAng &angle); ///< Generates a rotation matrix for the Z axis with the given angle.
    void ZrotM(const mAng &angle); ///< Rotates the matrix on the Z axis by the given angle.

    void ZXYrotS(mAng xRot, mAng yRot,
                 mAng zRot); ///< Generates the matrix on the Y, X and Z axes by the given angles.
    void ZXYrotM(mAng xRot, mAng yRot,
                 mAng zRot); ///< Rotates the matrix on the Y, X and Z axes by the given angles.
    void XYZrotS(mAng xRot, mAng yRot,
                 mAng zRot); ///< Generates the matrix on the Z, Y and X axes by the given angles.

    void XYZrotM(mAng xRot, mAng yRot,
                 mAng zRot); ///< Rotates the matrix on the Z, Y and X axes by the given angles.

    void toRot(mAng3_c &out) const; ///< Converts the matrix to a rotation vector.

    void multVecZero(nw4r::math::VEC3 &out) const; ///< Converts the matrix to a vector.
    void zero();                                   ///< Zeroes out the matrix.

    void rot(int, int); // does some werrd operation to rotate the matrix
    bool quatRelated();

public:
    static mMtx_c Identity;
};

#endif
