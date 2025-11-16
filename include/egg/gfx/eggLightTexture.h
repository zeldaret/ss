#ifndef EGG_LIGHT_TEXTURE_H
#define EGG_LIGHT_TEXTURE_H

#include "common.h"
#include "egg/egg_types.h"
#include "egg/gfx/eggCapTexture.h"
#include "egg/gfx/eggTexture.h"
#include "egg/math/eggVector.h"
#include "egg/prim/eggBinary.h"
#include "rvl/GX/GXTypes.h"

namespace EGG {

class LightTextureManager;

class LightTexture : public CapTexture, public IBinary<LightTexture> {
public:
#pragma pack(push, 1)
    struct SubData {
        /* 0x00 */ f32 mIntensity;
        /* 0x04 */ u8 mGradientUsed;
        /* 0x05 */ u8 field_0x05;
        /* 0x06 */ u8 _0x06[2];
    };

    // Implicit +0x10 from BinHeader
    struct BinData {
        /* 0x00 */ u16 mNumEntries;
        /* 0x02 */ u8 field_0x02;
        /* 0x03 */ u8 field_0x03;
        /* 0x04 */ char mName[32];
        /* 0x24 */ u8 mType;
        /* 0x25 */ u16 field_0x25;
        /* 0x27 */ u8 field_0x27;
        /* 0x28 */ u32 field_0x28;
        /* 0x2C */ f32 field_0x2C;
        /* 0x30 */ u8 _0x30[4];
        /* 0x34 */ char mName2[32];
        /* 0x54 */ f32 field_0x54;
        u8 _0x00[0x20];
        SubData mSubData[1];
    };
#pragma pack(pop)

    LightTexture(const char *name, const LightTextureManager *mgr);
    virtual ~LightTexture();

    virtual void configure() override; // at 0xC

    virtual void SetBinaryInner(const Bin &) override;
    virtual void GetBinaryInner(Bin *) const override;
    virtual size_t GetBinarySize() const override;

    static void initialize(u16 textureSize, Heap *pHeap);
    static void loadTextureFromResTimg(int index, EGG::ResTIMG *img);
    static void initDrawSetting(u16, u16, u16, u16);
    void addLight(const EGG::LightObject &);

    void fn_804AC0E0(int, const GXColor &, bool);
    void draw(int);
    static void beginDebugDraw();
    void debugDraw(int);

    const char *getName() const {
        return mName1;
    }

    f32 getFloat(u16 idx) const {
        return mpFloatData[idx];
    }
    void setFloat(u16 idx, f32 val) {
        mpFloatData[idx] = val;
    }

    u8 getByte1(u16 idx) const {
        return mpByteData1[idx];
    }
    void offByte1(u16 idx) {
        mpByteData1[idx] &= ~1;
    }
    void onByte1(u16 idx) {
        mpByteData1[idx] |= 1;
    }

    u8 getByte2(u16 idx) const {
        return mpByteData2[idx];
    }
    void setByte2(u16 idx, u8 val) const {
        mpByteData2[idx] = val;
    }

    static void getTexDimensions(u16 *, u16 *, u16 *, u16 *, u16);

private:
    static void fn_804AC0A0(int, int *, int *);
    static void loadTextureData(int index, void *data, GXTexFmt fmt);

    // TODO enum
    int GetLightType() const {
        return mLightType;
    }

    /* 0x30 */ const LightTextureManager *mpMgr;
    /* 0x34 */ u8 mNumData;
    /* 0x35 */ u8 field_0x35;
    /* 0x36 */ u8 field_0x36;
    /* 0x37 */ u8 mLightType;
    /* 0x38 */ Vector3f field_0x38;
    /* 0x44 */ f32 *mpFloatData;
    /* 0x48 */ u8 *mpByteData1;
    /* 0x4C */ u8 *mpByteData2;
    /* 0x50 */ f32 field_0x50;
    /* 0x54 */ char mName1[0x20];
    /* 0x74 */ char mName2[0x20];
    /* 0x94 */ s16 field_0x94;
    /* 0x98 */ f32 field_0x98;
    /* 0x9C */ u8 field_0x9C;
    /* 0x9D */ u8 field_0x9D;
    /* 0x9E */ u8 field_0x9E;
    /* 0x9F */ u8 field_0x9F;

    static u16 sTextureSize;
    static s8 sScaleExp;

    static u16 sDrawWidth;
    static u16 sDrawHeight;
    static u16 sDrawPosX;
    static u16 sDrawPosY;
    static u16 sTexWidth;
    static u16 sTexHeight;
    static u16 sDrawNumX;
    static u16 sDrawNumY;
    static CpuTexture *spNormalEnvironment;

    // unsure
    static bool sUseDebug;
};

} // namespace EGG

#endif
