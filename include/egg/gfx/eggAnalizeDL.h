#ifndef EGG_ANALIZE_DL_H
#define EGG_ANALIZE_DL_H

#include "nw4r/g3d/res/g3d_resshp.h"
#include "rvl/GX/GXAttr.h"

namespace EGG {

class AnalizeDL {
public:
    enum Status {
        STATUS_NONE,
        STATUS_1,
        STATUS_2,
        STATUS_3,
        STATUS_VTX = 4,
        STATUS_5,
        STATUS_END = 6,
    };

    struct VtxResult {
        /* 0x00 */ s32 mMtxIdx;
        /* 0x04 */ s32 field_0x04;
        /* 0x08 */ Vec field_0x08;
    };

private:
    /* 0x000 */ u8 *mpData;
    /* 0x004 */ u32 mBufSize;
    /* 0x008 */ u32 mCursor;
    /* 0x00C */ u8 field_0x00C;
    /* 0x00D */ u8 field_0x00D;
    /* 0x00E */ u8 field_0x00E;
    /* 0x00F */ u8 field_0x00F;
    /* 0x010 */ u16 field_0x010;
    /* 0x014 */ GXVtxDescList mVtxDescList[22];
    /* 0x0C4 */ GXVtxAttrFmtList mAttrFmtList[13];
    /* 0x194 */ u8 mArrStride;
    /* 0x198 */ const void *mpArr;
    /* 0x19C */ Status mStatus;
    /* 0x1A0 */ s16 field_0x1A0[10];
    /* 0x1B4 */ s16 field_0x1B4[10];
    /* 0x1C8 */ u8 field_0x1C8;
    /* 0x1CA */ u16 field_0x1CA;
    /* 0x1CC */ u16 field_0x1CC;
    /* 0x1D0 */ VtxResult mVtxResult;

    void init();

public:
    AnalizeDL(nw4r::g3d::ResShp shp);
    virtual ~AnalizeDL() {}

    Status step();
    const VtxResult &getVtxResult() const;
};

} // namespace EGG

#endif
