#ifndef EGG_I_SCN_PROC_MODEL_H
#define EGG_I_SCN_PROC_MODEL_H

#include "common.h"
#include "nw4r/math/math_types.h"

namespace EGG {

class IScnProcModel {
private:
    /* 0x00 */ u8 _0x00[0x08];

public:
    IScnProcModel() {}
    virtual ~IScnProcModel() {}                                                           // at 0x8
    virtual void draw(bool, u32) = 0;                                                     // at 0xC
    virtual UNKTYPE VF_0x10(UNKTYPE) = 0;                                                 // at 0x10
    virtual UNKTYPE VF_0x14(UNKTYPE) = 0;                                                 // at 0x14
    virtual void getMinMaxScnProcModel(nw4r::math::VEC3 *, nw4r::math::VEC3 *) const = 0; // at 0x18
};

} // namespace EGG

#endif
