#ifndef EGG_LIGHT_MANAGER_H
#define EGG_LIGHT_MANAGER_H

#include "egg/egg_types.h"
#include "egg/gfx/eggLightObject.h"
#include "egg/prim/eggBinary.h"
#include "nw4r/types_nw4r.h"
#include "rvl/GX/GXLight.h"
#include "rvl/GX/GXTypes.h"

namespace EGG {

// TODO: Fill out more
class LightManager : public IBinary<LightManager> {
    struct LightData {
        /* 0x00 */ LightObject mLightObject;
    };

    // ???
    union Counts {
        struct {
            /* 0x00 */ u8 mNumEggLightObjects;
            /* 0x01 */ u8 mNumUnks;
        };
    };

    struct Unk1 {
        /* 0x00 */ GXColor mColor;
        /* 0x04 */ const char *mStr;
        /* 0x08 */ u8 field_0x08;
    };

public:
    struct BinData {};
    // vt at 0x00
    LightManager(u32, u32, u8);
    virtual ~LightManager();
    virtual void SetBinaryInner(const Bin &) override;
    virtual void GetBinaryInner(Bin *) const override;
    virtual size_t GetBinarySize() const override;
    virtual void SetBinaryInner(const Bin &, const Bin &, f32) override;
    virtual void Reset();
    virtual void Calc(nw4r::g3d::ScnRoot *);
    virtual void CalcView(const nw4r::math::MTX34 &, u8, nw4r::g3d::ScnRoot *);
    virtual void DoneDraw();

    void BecomeInvalidLight(int);

    LightTextureManager *GetTextureMgr() const {
        return mpTextureMgr;
    }

    LightObject *GetLightObject(int i) {
        return &mpLightData[i].mLightObject;
    }

    /* 0x04 */ Counts mCounts0x04;
    /* 0x06 */ Counts mCounts0x06;
    /* 0x08 */ LightData *mpLightData;
    /* 0x0C */ Unk1 *mpUnk1;
    /* 0x10 */ GXLightObj *mpLightObjs;
    /* 0x14 */ LightTextureManager *mpTextureMgr;
    /* 0x18 */ GXColor mColor;
    /* 0x1C */ u8 field_0x1C;
    /* 0x1D */ u8 field_0x1D;
    /* 0x1E */ u16 mFlags;
    /* 0x20 */ u32 field_0x20;
    /* 0x24 */ s16 field_0x24;
};

} // namespace EGG

#endif
