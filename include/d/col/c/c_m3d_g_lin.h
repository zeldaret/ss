#ifndef C_M3D_G_LIN_H
#define C_M3D_G_LIN_H

#include <common.h>
#include <m/m_vec.h>

// Line
class cM3dGLin {
public:
    /* 0x00 */ mVec3_c mStart;
    /* 0x0C */ mVec3_c mEnd;

    cM3dGLin() {}
    cM3dGLin(const mVec3_c &, const mVec3_c &);

    void SetStartEnd(const mVec3_c &start, const mVec3_c &end) {
        mStart = start;
        mEnd = end;
    }
    void SetStartEnd(const mVec3_c *, const mVec3_c *);
    void CalcPos(mVec3_c *, f32) const;

    void CalcVec(Vec *pOut) const {
        PSVECSubtract(mEnd, mStart, pOut);
    }

    void SetStart(const mVec3_c &);
    void SetEnd(const mVec3_c &);

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
