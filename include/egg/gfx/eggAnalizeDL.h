#ifndef EGG_ANALIZE_DL_H
#define EGG_ANALIZE_DL_H

#include "nw4r/g3d/res/g3d_resshp.h"

namespace EGG {

class AnalizeDL {
public:
    enum Status {
        STATUS_NONE,

        STATUS_VTX = 4,
        STATUS_ERROR = 6,
    };

    struct VtxResult {
        /* 0x00 */ s32 mMtxIdx;
        /* 0x04 */ u8 _0x04[4];
        /* 0x08 */ Vec field_0x08;
    };

private:
    u8 _0x000[0x1E4 - 0x000];

public:
    AnalizeDL(nw4r::g3d::ResShp shp);
    virtual ~AnalizeDL() {}

    Status step();
    const VtxResult &getVtxResult() const;
};

} // namespace EGG

#endif
