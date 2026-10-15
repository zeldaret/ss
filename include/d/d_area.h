#ifndef D_AREA_H
#define D_AREA_H

#include "m/m_angle.h"
#include "m/m_mtx.h"
#include "m/m_vec.h"

class dArea_c {
public:
    void set(const mVec3_c &, const mAng, const mVec3_c &, mMtx_c *, f32);
    bool checkAreaBox(const mVec3_c &);
    bool checkAreaCyl(const mVec3_c &);

private:
    mMtx_c mArea;
};

#endif
