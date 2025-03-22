#ifndef EGG_FOG_MANAGER_H
#define EGG_FOG_MANAGER_H

#include "common.h"
#include "egg/gfx/eggFog.h"
#include "egg/prim/eggBinary.h"
#include "nw4r/types_nw4r.h"

namespace EGG {

// TODO: Add members
class FogManager : IBinary<FogManager> {
public:
    struct BinData {
        /* 0x10 */ u16 mCount;
        /* 0x12 */ u8 _0x12[2];
        /* 0x14 */ IBinary<Fog>::Bin mFogData[1];
    };
    FogManager(u16);
    virtual ~FogManager();
    virtual void SetBinaryInner(const Bin &) override;
    virtual void GetBinaryInner(Bin *) const override;
    virtual size_t GetBinarySize() const override;
    virtual void SetBinaryInner(const Bin &, const Bin &, f32) override;

    void ResetFog();
    void Calc();
    void CopyToG3D(nw4r::g3d::ScnRoot *) const;

    void DoneDraw() {
        mFlag &= ~4;
    }

private:
    /* 0x04 */ u8 mFlag;
    /* 0x05 */ u8 TODO[0x08 - 0x05];
    /* 0x08 */ u16 mCount;
    /* 0x0A */ u16 mCount2;
    /* 0x0C */ Fog *mpFog;
};

} // namespace EGG

#endif
