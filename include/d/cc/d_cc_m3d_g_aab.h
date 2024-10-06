#ifndef D_CC_M3D_G_AAB_H
#define D_CC_M3D_G_AAB_H

#include "d/cc/d_cc_m3d.h"
#include "d/cc/d_cc_m3d_g_lin.h"

#include <common.h>

// Axis aligned bounding box
class cM3dGAab {
private:
public:
    /* 0x00 */ mVec3_c mMin;
    /* 0x0C */ mVec3_c mMax;

    void Set(const mVec3_c &min, const mVec3_c &max) {
        mMin = min;
        mMax = max;
    }
    bool CrossY(const mVec3_c &) const;
    bool UnderPlaneYUnder(f32) const;
    bool TopPlaneYUnder(f32) const;
    void ClearForMinMax(void);
    void SetMinMax(const mVec3_c &);
    void SetMinMax(const cM3dGAab &);
    void SetMin(const mVec3_c &min) {
        mMin = min;
    }
    void SetMax(const mVec3_c &max) {
        mMax = max;
    }
    void CalcCenter(mVec3_c *) const;
    void PlusR(f32);
    const mVec3_c *GetMaxP(void) const {
        return &mMax;
    }
    const mVec3_c *GetMinP(void) const {
        return &mMin;
    }
    const f32 GetMaxX(void) const {
        return mMax.x;
    }
    const f32 GetMaxY(void) const {
        return mMax.y;
    }
    const f32 GetMaxZ(void) const {
        return mMax.z;
    }
    const f32 GetMinX(void) const {
        return mMin.x;
    }
    const f32 GetMinY(void) const {
        return mMin.y;
    }
    const f32 GetMinZ(void) const {
        return mMin.z;
    }
    // bool Cross(const cM3dGLin *param_1) {
    //     return cM3d_Cross_MinMaxBoxLine(GetMinP(), GetMaxP(), (Vec *)&param_1->GetStartP(), (Vec
    //     *)&param_1->GetEndP());
    // }
    // bool Cross(const cM3dGAab *param_1) {
    //     return cM3d_Cross_AabAab(this, param_1);
    // }
    // bool Cross(const cM3dGCyl *param_1) {
    //     return cM3d_Cross_AabCyl(this, param_1);
    // }
    // bool Cross(const cM3dGSph *param_1) {
    //     return cM3d_Cross_AabSph(this, param_1);
    // }
}; // Size = 0x1C

#endif /* C_M3D_G_AAB_H */
