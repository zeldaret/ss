#ifndef NW4R_G3D_CALC_WORLD_H
#define NW4R_G3D_CALC_WORLD_H
#include "common.h"
#include "nw4r/math.h"

namespace nw4r {
namespace g3d {

struct FuncObjCalcWorld {
private:
    u8 UNK_0x00[0x06];
    /** 0x06 */ u16 mNodeId;

public:
    u32 GetNodeId() const {
        return mNodeId;
    }

    void SetNodeId(u32 n) {
        mNodeId = n;
    }
};

class WorldMtxManip {};

void CalcWorld(math::MTX34 *, u32 *, const u8 *, const math::MTX34 *, ResMdl, AnmObjChr *, FuncObjCalcWorld *, u32);

void CalcWorld(math::MTX34 *, u32 *, const u8 *, const math::MTX34 *, ResMdl, AnmObjChr *, FuncObjCalcWorld *);

void CalcSkinning(math::MTX34 *, u32 *, ResMdl, const u8 *);
} // namespace g3d
} // namespace nw4r

#endif
