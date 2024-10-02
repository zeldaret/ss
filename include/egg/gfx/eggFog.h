#ifndef EGG_FOG_H
#define EGG_FOG_H

#include <egg/prim/eggBinary.h>
#include <nw4r/types_nw4r.h>

namespace EGG {

// TODO: Add members
class FogManager : IBinary<FogManager> {
public:
    FogManager(u16);
    virtual ~FogManager();
    virtual void SetBinaryInner(Bin &) override;
    virtual void GetBinaryInner(Bin *) const override;
    virtual size_t GetBinarySize() override;
    virtual void SetBinaryInner(const Bin &, const Bin &, f32) override;

    void Calc();
    void CopyToG3D(nw4r::g3d::ScnRoot *) const;

    /* 0x04 */ u8 mFlag;
    /* 0x05 */ u8 TODO[0x10 - 0x05];
};

} // namespace EGG

#endif
