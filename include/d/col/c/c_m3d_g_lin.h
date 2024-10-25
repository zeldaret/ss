#ifndef C_M3D_G_LIN_H
#define C_M3D_G_LIN_H

#include "common.h"
#include "m/m_vec.h"
#include "nw4r/types_nw4r.h"

class cM3dGLin {
public:
    typedef nw4r::math::VEC3 VEC3;
    typedef EGG::Vector3f Vector3f;

    // Needs to be mutable for cCcD_CpsAttr::GetPos and cCcD_CpsAttr::GetShapeAccess
    /* 0x00 */ mutable mVec3_c mStart;
    /* 0x0C */ mutable mVec3_c mEnd;

    cM3dGLin() {}

    void SetStartEnd(const VEC3 &, const VEC3 &);
    void SetStartEnd(const mVec3_c *, const mVec3_c *);

    void Set(const mVec3_c &start, const mVec3_c &end) {
        mStart = start;
        mEnd = end;
    }
    void CalcPos(mVec3_c *, f32) const;

    void CalcVec(Vec *pOut) const {
        PSVECSubtract(mEnd, mStart, pOut);
    }

    void SetStart(const mVec3_c &);
    void SetEnd(const mVec3_c *pEnd) {
        mEnd = *pEnd;
    }

    mVec3_c &GetStart() {
        return mStart;
    }

    const mVec3_c &GetStart() const {
        return mStart;
    }

    mVec3_c *GetStartP(void) {
        return &mStart;
    }
    const mVec3_c *GetStartP(void) const {
        return &mStart;
    }

    mVec3_c &GetEnd() {
        return mEnd;
    }

    const mVec3_c &GetEnd() const {
        return mEnd;
    }

    mVec3_c *GetEndP(void) {
        return &mEnd;
    }

    const mVec3_c *GetEndP(void) const {
        return &mEnd;
    }
};

#endif /* C_M3D_G_LIN_H */
