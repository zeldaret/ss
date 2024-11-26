#ifndef D_ROPE_MODEL_H
#define D_ROPE_MODEL_H

#include "m/m3d/m_bline.h"

//
// The names in this file are made up based on the usage in setupGX
//

class dMultiMat_c : public m3d::blineMat_c {
public:
    dMultiMat_c() {}
    virtual void setupGX(bool bTransparent) override;
};

class dSkinMat_c : public m3d::blineMat_c {
public:
    dSkinMat_c() : mField_0x4C(-1), mField_0x50(0xFF), mField_0x54(0xFF) {}
    virtual void setupGX(bool bTransparent) override;

    /* 0x4C */ s32 mField_0x4C;
    /* 0x50 */ s32 mField_0x50;
    /* 0x54 */ s32 mField_0x54;
};

#endif
