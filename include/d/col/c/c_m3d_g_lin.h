#ifndef C_M3D_G_LIN_H
#define C_M3D_G_LIN_H

#include <common.h>
#include <m/m_vec.h>

// Line
class cM3dGLin {
    // private:
public:
    /* 0x00 */ mVec3_c mStart;
    /* 0x0C */ mVec3_c mEnd;
    /* 0x18 vtable */

    cM3dGLin() {}
    cM3dGLin(const mVec3_c &, const mVec3_c &);
    virtual ~cM3dGLin() {}
    void SetStartEnd(const mVec3_c &, const mVec3_c &);
    void SetStartEnd(const Vec &, const Vec &);
    void CalcPos(Vec *, f32) const;
    void CalcVec(Vec *pOut) const {
        PSVECSubtract(mEnd, mStart, pOut);
    }
    void SetEnd(const mVec3_c &);

    void set(const Vec &i_start, const Vec &i_end) {
        SetStartEnd(i_start, i_end);
    }
    const mVec3_c &GetStartP(void) const {
        return mStart;
    }
    mVec3_c &GetStartP(void) {
        return mStart;
    }
    mVec3_c &GetStart() {
        return mStart;
    }
    const mVec3_c &GetStart() const {
        return mStart;
    }

    const mVec3_c &GetEndP(void) const {
        return mEnd;
    }
    mVec3_c &GetEndP(void) {
        return mEnd;
    }
    mVec3_c &GetEnd() {
        return mEnd;
    }
    const mVec3_c &GetEnd() const {
        return mEnd;
    }
}; // Size = 0x1C

#endif /* C_M3D_G_LIN_H */
