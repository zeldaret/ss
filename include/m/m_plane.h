#ifndef M_PLANE_H
#define M_PLANE_H

#include "m_vec.h"

/// @brief Plane with a normal. Class made up, file is a guess
class mPlane_c {
public:
    /* 0x00 */ mVec3_c mNormal;
    /* 0x0C */ f32 mD;
    /* 0x10   vtable */

    mPlane_c() {}
    mPlane_c(const mVec3_c *, f32);
    bool crossInfLin(const mVec3_c &, const mVec3_c &, mVec3_c &) const;
    void SetupNP0(const mVec3_c &, const mVec3_c &);
    void SetupNP(const mVec3_c &, const mVec3_c &);
    bool getCrossY(const mVec3_c &, f32 *) const;
    bool getCrossYLessD(const mVec3_c &, f32 *) const;
    void Set(const mPlane_c *);

    virtual ~mPlane_c() {}
};

#endif
