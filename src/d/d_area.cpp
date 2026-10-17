#include "d/d_area.h"

#include "common.h"
#include "m/m_mtx.h"
#include "m/m_vec.h"
#include "rvl/MTX/mtx.h"

void dArea_c::set(const mVec3_c &position, const mAng rotY, const mVec3_c &scale, mMtx_c *pSrc, f32 scaleTerm) {
    mMtx_c src;
    if (pSrc == nullptr) {
        pSrc = &src;
    }
    pSrc->transS(position.x, position.y + scale.y * 0.5f, position.z);
    pSrc->YrotM(rotY);
    pSrc->scaleM(scale.x + scaleTerm, scale.y + scaleTerm, scale.z + scaleTerm);
    mArea.copyFrom(*pSrc);
    MTXInverse(mArea, mArea);
}

bool dArea_c::checkAreaBox(const mVec3_c &position) {
    mVec3_c pos;
    mArea.multVec(position, pos);
    return (-0.5f <= pos.x && pos.x <= 0.5f) && //
           (-0.5f <= pos.y && pos.y <= 0.5f) && //
           (-0.5f <= pos.z && pos.z <= 0.5f);
}

bool dArea_c::checkAreaCyl(const mVec3_c &position) {
    mVec3_c pos;
    mArea.multVec(position, pos);
    return (-0.5f <= pos.y && pos.y <= 0.5f) && //
           pos.squareMagXZ() <= 1.0f;
}
