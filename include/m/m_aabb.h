#ifndef M_AABB
#define M_AABB

#include "m_vec.h"

// Complete guess. Could use nw4r/egg or elsewhere
class mAABB_c {
public:
    /* 0x00 */ mVec3_c mMin;
    /* 0x0C */ mVec3_c mMax;
    /* 0x18 vtable */

    virtual ~mAABB_c() {}
    void Set(const mVec3_c *, const mVec3_c *);
    bool CrossY(const mVec3_c *) const;
    bool UnderPlaneYUnder(f32) const;
    bool TopPlaneYUnder(f32) const;
    void ClearForMinMax(void);
    void SetMinMax(const mVec3_c &);
    void SetMinMax(const mAABB_c &);
    void SetMin(const mVec3_c &);
    void SetMax(const mVec3_c &);
    void CalcCenter(mVec3_c *) const;
    void PlusR(f32);

    const mVec3_c *GetMaxP(void) const { return &mMax; }
    const mVec3_c *GetMinP(void) const { return &mMin; }
    const f32 GetMaxX(void) const { return mMax.x; }
    const f32 GetMaxY(void) const { return mMax.y; }
    const f32 GetMaxZ(void) const { return mMax.z; }
    const f32 GetMinX(void) const { return mMin.x; }
    const f32 GetMinY(void) const { return mMin.y; }
    const f32 GetMinZ(void) const { return mMin.z; }
};

#endif
