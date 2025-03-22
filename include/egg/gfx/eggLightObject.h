#ifndef EGG_LIGHT_OBJ_H
#define EGG_LIGHT_OBJ_H

#include "egg/math/eggVector.h"
#include "egg/prim/eggBinary.h"
#include "nw4r/math/math_types.h"
#include "rvl/GX/GXLight.h"
#include "rvl/GX/GXTypes.h"

namespace EGG {

class LightObject : public IBinary<LightObject> {
public:
    struct BinData {};
    LightObject();
    virtual ~LightObject() {}

    virtual void SetBinaryInner(const Bin &) override;
    virtual void GetBinaryInner(Bin *) const override;
    virtual void SetBinaryInner(const Bin &, const Bin &, f32) override;

    void CalcDirDist();
    void Reset();
    void Calc();
    void CalcView(nw4r::math::MTX34 const &);
    void InitGX(GXLightObj *obj) const;

    void CalcAt();
    void SetPos(const nw4r::math::VEC3 &);
    void SetAt(const nw4r::math::VEC3 &);
    void SetPosAt(nw4r::math::VEC3 const &, nw4r::math::VEC3 const &);
    void SetDist(f32);

    int getField0x3C() const {
        return field_0x3C;
    }

    GXSpotFn getSpotFn() const {
        return static_cast<GXSpotFn>(mSpotFn);
    }

    GXDistAttnFn getDistAttnFn() const {
        return static_cast<GXDistAttnFn>(mDistAttnFn);
    }

    f32 getDistance() const {
        return mDist;
    }

private:
    /* 0x04 */ u16 mIndex;
    /* 0x06 */ u16 field_0x06;
    /* 0x08 */ nw4r::math::VEC3 mAt;
    /* 0x14 */ nw4r::math::VEC3 mPos;
    /* 0x20 */ nw4r::math::VEC3 mDir;
    /* 0x2C */ GXColor mWhite;
    /* 0x30 */ f32 field_0x30;
    /* 0x34 */ f32 mDist;
    /* 0x38 */ GXColor mBlack;
    /* 0x3C */ u8 field_0x3C;
    /* 0x3D */ u8 field_0x3D;
    /* 0x3E */ u8 mSpotFn;
    /* 0x3F */ u8 mDistAttnFn;
    /* 0x40 */ f32 field_0x40;
    /* 0x44 */ f32 field_0x44;
    /* 0x48 */ f32 field_0x48;
    /* 0x4C */ nw4r::math::VEC3 field_0x4C;
    /* 0x58 */ nw4r::math::VEC3 field_0x58;
    /* 0x64 */ f32 field_0x64;
    /* 0x68 */ u16 field_0x68;
    /* 0x6C */ nw4r::math::VEC3 field_0x6C;
    /* 0x78 */ GXColor field_0x78;
    /* 0x7C */ nw4r::math::VEC3 field_0x7C;
    /* 0x88 */ nw4r::math::VEC3 field_0x88;
    /* 0x94 */ nw4r::math::VEC3 field_0x94;
    /* 0xA0 */ u16 field_0xA0;
};

} // namespace EGG

#endif
