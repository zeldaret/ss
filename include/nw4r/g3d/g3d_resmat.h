#ifndef NW4R_G3D_RESMAT_H
#define NW4R_G3D_RESMAT_H
#include "common.h"
#include "g3d_rescommon.h"
#include "math_types.h"
#include <rvl/GX.h>


namespace nw4r {
namespace g3d {
struct ResMatTexCoordGenData {};

struct ResMatTexCoordGen {
    ResCommon<ResMatTexCoordGenData> mTexGen; // at 0x0

    inline ResMatTexCoordGen(void *vptr) : mTexGen(vptr) {}
    bool IsValid() const {
        return mTexGen.IsValid();
    }

    bool GXGetTexCoordGen2(GXTexCoordID, GXTexGenType *, GXTexGenSrc *, u8 *, u32 *);
    void GXSetTexCoordGen2(GXTexCoordID, GXTexGenType, GXTexGenSrc, u8, u32);
    void DCStore(bool);
};

struct ResTexSrtData {};

struct ResTexSrt {
    ResCommon<ResTexSrtData> mTexSrt; // at 0x0

    inline ResTexSrt(void *vptr) : mTexSrt(vptr) {}
    bool IsValid() const {
        return mTexSrt.IsValid();
    }

    void SetMapMode(u32, u32, int, int);
};

struct ResTexPlttInfoData {
    u32 mNumOffsets; // at 0x0

    struct InfoOffset {
        u32 mResOffset; // at 0x0
        u32 mTexOffset; // at 0x4
    } mOffsets[];       // at 0x4
};

struct ResTexPlttInfo {
    ResCommon<ResTexPlttInfoData> mInfo;

    inline ResTexPlttInfo(void *vptr) : mInfo(vptr) {}
};

struct ResMatDLData {
    char UNK_0x0[0xE0];
    ResMatTexCoordGenData texCoordGenData; // at 0xE0
};

struct ResMatData {
    char UNK_0x0[0x38];
    u32 resMatDLOfs; // at 0x38
    char UNK_0x3C[0x1A4 - 0x3C];
    ResTexSrtData texSrtData; // at 0x1A4
};

struct ResMat {
    ResCommon<ResMatData> mMat;

    inline ResMat(void *vptr) : mMat(vptr) {}
    bool IsValid() const {
        return mMat.IsValid();
    }

    ResTexSrt GetResTexSrt() {
        return ResTexSrt(&mMat.ref().texSrtData);
    }

    ResMatTexCoordGen GetResMatTexCoordGen() {
        ResMatDLData *dlData = mMat.ofs_to_ptr<ResMatDLData>(mMat.ref().resMatDLOfs);
        return ResMatTexCoordGen(&dlData->texCoordGenData);
    }

    bool Bind(ResFile);
    UNKTYPE Release();
    UNKTYPE Init();
};
} // namespace g3d
} // namespace nw4r

#endif
