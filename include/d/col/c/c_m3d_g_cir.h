#ifndef C_M3D_G_CIR_H
#define C_M3D_G_CIR_H

#include "common.h"

class cM2dGCir {
public:
    f32 mPosX;
    f32 mPosY;
    f32 mRadius;

    f32 GetCx() const {
        return mPosX;
    }
    f32 GetCy() const {
        return mPosY;
    }
    f32 GetR() const {
        return mRadius;
    }

    cM2dGCir() {}
    virtual ~cM2dGCir() {}
};

class cM3dGCir : public cM2dGCir {
    f32 mPosZ;

public:
    cM3dGCir(void);
    virtual ~cM3dGCir();
    void Set(f32, f32, f32, f32);
};

#endif
