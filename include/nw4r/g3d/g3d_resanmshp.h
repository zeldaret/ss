#ifndef NW4R_G3D_RESANMSHP_H
#define NW4R_G3D_RESANMSHP_H
#include "common.h"
#include "nw4r/g3d/g3d_rescommon.h"

namespace nw4r {
namespace g3d {
struct ResAnmShpData {
    char UNK_0x0[0x8];
    u32 mRevision; // at 0x8
};

struct ResAnmShp {
    enum { REVISION = 3 };

    ResCommon<ResAnmShpData> mAnmShp;

    inline ResAnmShp(void *vptr) : mAnmShp(vptr) {}

    inline bool CheckRevision() const {
        return mAnmShp.ref().mRevision == REVISION;
    }
};
} // namespace g3d
} // namespace nw4r

#endif
