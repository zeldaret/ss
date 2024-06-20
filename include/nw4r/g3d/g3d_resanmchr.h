#ifndef NW4R_G3D_RESANMCHR_H
#define NW4R_G3D_RESANMCHR_H
#include "common.h"
#include "nw4r/g3d/g3d_rescommon.h"

namespace nw4r {
namespace g3d {
struct ResAnmChrData {
    char UNK_0x0[0x8];
    u32 mRevision; // at 0x8
};

struct ResAnmChr {
    enum { REVISION = 4 };

    ResCommon<ResAnmChrData> mAnmChr;

    inline ResAnmChr(void *vptr) : mAnmChr(vptr) {}

    inline bool CheckRevision() const {
        return mAnmChr.ref().mRevision == REVISION;
    }
};
} // namespace g3d
} // namespace nw4r

#endif
