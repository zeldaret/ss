#ifndef EGG_FOG_H
#define EGG_FOG_H

#include "common.h"
#include "egg/prim/eggBinary.h"
#include "nw4r/g3d/g3d_fog.h"
#include "nw4r/types_nw4r.h"
#include "rvl/GX/GXTypes.h"

namespace EGG {

class Fog : public IBinary<Fog> {
public:
    struct BinData {
        /* 0x10 */ f32 mStartZ;
        /* 0x14 */ f32 mEndZ;
        /* 0x18 */ f32 mNearZ;
        /* 0x1C */ f32 mFarZ;
        /* 0x20 */ GXColor mColor;
        /* 0x24 */ u8 field_0x24;
        /* 0x25 */ u8 mFlag2;
        /* 0x26 */ u16 field_0x26;
        /* 0x28 */ UNKWORD field_0x28;
        /* 0x2C */ UNKWORD field_0x2C;
    };
    Fog();
    virtual ~Fog();
    virtual void SetBinaryInner(const Bin &) override;
    virtual void GetBinaryInner(Bin *) const override;
    virtual void SetBinaryInner(const Bin &, const Bin &, f32) override;

    void CopyToG3D(nw4r::g3d::Fog fof) const;
    void Reset();

private:

    /* 0x04 */ GXColor mColor;
    /* 0x08 */ f32 mStartZ;
    /* 0x0C */ f32 mEndZ;
    /* 0x10 */ f32 mNearZ;
    /* 0x14 */ f32 mFarZ;
    /* 0x18 */ GXFogType mFogType;
    /* 0x1C */ u8 mFlags;
};

} // namespace EGG

#endif
