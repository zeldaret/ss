#ifndef NW4R_G3D_RESANMSCN_H
#define NW4R_G3D_RESANMSCN_H
#include "common.h"
#include "nw4r/g3d/g3d_resanmcamera.h"
#include "nw4r/g3d/g3d_resanmfog.h"
#include "nw4r/g3d/g3d_rescommon.h"

namespace nw4r {
namespace g3d {
struct ResAnmScnData {
    char UNK_0x0[0x8];
    u32 mRevision; // at 0x8
    char UNK_0xC[0x3E - 0xC];
    u16 mFogMaxRefNum; // at 0x3E
};

struct ResAnmScn {
    enum { REVISION = 4 };

    ResCommon<ResAnmScnData> mAnmScn;

    ResAnmFog GetResAnmFogByRefNumber(u32) const;
    ResAnmCamera GetResAnmCameraByRefNumber(u32) const;

    inline ResAnmScn(void *vptr) : mAnmScn(vptr) {}

    inline bool IsValid() const {
        return mAnmScn.IsValid();
    }
    inline bool CheckRevision() const {
        return mAnmScn.ref().mRevision == REVISION;
    }

    u16 GetResAnmFogMaxRefNumber() const {
        return mAnmScn.ref().mFogMaxRefNum;
    }
};
} // namespace g3d
} // namespace nw4r

#endif
