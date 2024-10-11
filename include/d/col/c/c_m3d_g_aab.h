#ifndef C_M3D_G_AAB_H
#define C_M3D_G_AAB_H

#include "d/col/c/c_m3d.h"
#include "d/col/c/c_m3d_g_lin.h"
#include "d/col/c/c_m3d_g_sph.h"

#include <common.h>

// Axis aligned bounding box
class cM3dGAab {
private:
public:
    /* 0x00 */ mVec3_c mMin;
    /* 0x0C */ mVec3_c mMax;

    cM3dGAab() {}
    cM3dGAab(const mVec3_c &, const mVec3_c &);

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
    void SetMin(const mVec3_c &min);
    void SetMax(const mVec3_c &max);
    void CalcCenter(mVec3_c &) const;
    void PlusR(f32);
    void Clamp(const mVec3_c &, mVec3_c &) const;
    void Set(const cM3dGSph &);
    bool Cross(const mVec3_c &) const;

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
};

#endif /* C_M3D_G_AAB_H */
