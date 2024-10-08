#ifndef C_M3D_G_LIN_H
#define C_M3D_G_LIN_H

#include <common.h>
#include <m/m_vec.h>

class cM3dGLin {
public:
    typedef nw4r::math::VEC3 VEC3;
    typedef EGG::Vector3f Vector3f;

    /* 0x00 */ mVec3_c mStart;
    /* 0x0C */ mVec3_c mEnd;

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

    nw4r::math::VEC3 &GetStart() {
        return mStart;
    }

    const nw4r::math::VEC3 &GetStart() const {
        return mStart;
    }

    nw4r::math::VEC3 *GetStartP(void) {
        return &mStart;
    }
    const nw4r::math::VEC3 *GetStartP(void) const {
        return &mStart;
    }

    nw4r::math::VEC3 &GetEnd() {
        return mEnd;
    }

    const nw4r::math::VEC3 &GetEnd() const {
        return mEnd;
    }

    nw4r::math::VEC3 *GetEndP(void) {
        return &mEnd;
    }

    const nw4r::math::VEC3 *GetEndP(void) const {
        return &mEnd;
    }
};

#endif /* C_M3D_G_LIN_H */
