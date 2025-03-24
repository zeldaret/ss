#ifndef EGG_LIGHT_MANAGER_H
#define EGG_LIGHT_MANAGER_H

#include "egg/egg_types.h"
#include "egg/gfx/eggLightObject.h"
#include "egg/prim/eggBinary.h"
#include "nw4r/g3d/g3d_light.h"
#include "nw4r/g3d/res/g3d_resanmamblight.h"
#include "nw4r/g3d/res/g3d_resanmscn.h"
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
            /* 0x01 */ u8 mNumAmbientObjects;
        };
    };

    struct AmbientObject {
        /* 0x00 */ nw4r::g3d::AmbLightObj mLightObj;
        /* 0x04 */ const char *mLightName;
        /* 0x08 */ u8 field_0x08;
    };

public:
    struct BinAmbient {
        /* 0x00 */ u8 field_0x00;
        /* 0x01 */ u8 field_0x01;
        /* 0x02 */ u8 field_0x02;
        /* 0x03 */ u8 field_0x03;
        /* 0x04 */ u8 field_0x04;
        /* 0x05 */ u8 field_0x05;
        /* 0x06 */ u8 field_0x06;
        /* 0x07 */ u8 field_0x07;
    };

    struct BinData {
        /* 0x00 */ u16 mNumLightObjects;
        /* 0x02 */ u16 mNumAmbientObjects;
        /* 0x04 */ GXColor mColor;
        /* 0x08 */ u8 _0x08[0x18 - 0x08];
        /* 0x18 */ LightObject::Bin mObjData[1];
    };

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

    void LoadLightSet(int, u32 *, u32 *, u32 *, u32 *, GXColor *);

    void LoadScnLightInner(nw4r::g3d::ResAnmScn, f32, s16, u32);

    void AnmScnRes_GetAmbLightColor(AmbientObject *, nw4r::g3d::ResAnmAmbLight, f32) const;

    void CopyToG3D_Ambient(nw4r::g3d::ScnRoot *) const;
    void CopyToG3D_World(nw4r::g3d::ScnRoot *) const;
    void CopyToG3D_View(nw4r::g3d::ScnRoot *, const nw4r::math::MTX34 &) const;

    void BecomeInvalidLight(int);

    LightTextureManager *GetTextureMgr() const {
        return mpTextureMgr;
    }

    static u16 CheckedNumLightObjs(u8 num) {
        return num < 8 ? num : 8;
    }

    const LightObject *GetLightObject(int i) const {
        return &mpLightData[i].mLightObject;
    }

    LightObject *GetLightObject(int i) {
        return &mpLightData[i].mLightObject;
    }

    /* 0x04 */ Counts mCounts;
    /* 0x06 */ Counts mSavedCounts;
    /* 0x08 */ LightData *mpLightData;
    /* 0x0C */ AmbientObject *mpAmbientObjects;
    /* 0x10 */ GXLightObj *mpLightObjs;
    /* 0x14 */ LightTextureManager *mpTextureMgr;
    /* 0x18 */ GXColor mColor;
    /* 0x1C */ u8 mNumGxObjsPerEggObj;
    /* 0x1D */ u8 field_0x1D;
    /* 0x1E */ u16 mFlags;
    /* 0x20 */ nw4r::g3d::ResAnmScn mResAnmScn;
    /* 0x24 */ s16 mRefNumber;
};

} // namespace EGG

#endif
