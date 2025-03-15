#ifndef EGG_LIGHT_H
#define EGG_LIGHT_H

#include "egg/core/eggHeap.h"
#include "egg/prim/eggBinary.h"
#include "nw4r/types_nw4r.h"

namespace EGG {

// TODO: Fill out more
class LightTexture {
public:
    static void initialize(u16 textureSize, Heap *pHeap);
};

// TODO: Fill out more
class LightTextureManager : public IBinary<LightTextureManager> {
public:
    struct BinData {};
    virtual ~LightTextureManager();
    virtual void SetBinaryInner(const Bin &) override;
    virtual void GetBinaryInner(Bin *) const override;
    virtual size_t GetBinarySize() const override;
    virtual void SetBinaryInner(const Bin &, const Bin &, f32) override;
    void replaceModelTextures(nw4r::g3d::ResMdl) const;

    void drawAndCaptureTexture(f32, f32, f32, f32);
};

// TODO: Fill out more
class LightManager : public IBinary<LightManager> {
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

    LightTextureManager *GetTextureMgr() const {
        return mTextureMgr;
    }

    u8 TODO_0x04[0x14 - 0x04];
    LightTextureManager *mTextureMgr;
    u8 TODO_0x18[0x28 - 0x18];
};

} // namespace EGG

#endif
