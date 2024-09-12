#ifndef NW4R_G3D_RESTEX_H
#define NW4R_G3D_RESTEX_H
#include "common.h"
#include "nw4r/g3d/g3d_rescommon.h"
#include <rvl/GX.h>


namespace nw4r {
namespace g3d {
struct ResTexData {
    char mMagic[4]; // "TEX0"; at 0x0
    u32 mLength;    // at 0x4
    u32 mRevision;  // at 0x8
    s16 SHORT_0xC;
    u16 SHORT_0xE;
    u32 INT_0x10;
    UNKWORD WORD_0x14;
    u32 mFlags; // at 0x18
    u16 SHORT_0x1C;
    u16 SHORT_0x1E;
    union // at 0x20
    {
        GXTexFmt mFormat;
        GXCITexFmt mCiFormat;
    };
    u32 WORD_0x24;
    f32 FLOAT_0x28;
    f32 FLOAT_0x2C;
};

struct ResPlttData {
    char mMagic[4]; // "PLT0"; at 0x0
    u32 mLength;    // at 0x4
    u32 mRevision;  // at 0x8
};

struct ResTex {
    enum { REVISION = 1 };

    ResCommon<ResTexData> mTex; // at 0x0

    inline ResTex(void *vptr) : mTex(vptr) {}

    inline bool CheckRevision() const {
        return mTex.ref().mRevision == REVISION;
    }

    bool GetTexObjParam(void **, u16 *, u16 *, GXTexFmt *, f32 *, f32 *, u8 *) const;
    bool GetTexObjCIParam(void **, u16 *, u16 *, GXCITexFmt *, f32 *, f32 *, u8 *) const;
    void Init();

    bool IsValid() const {
        return mTex.IsValid();
    }
};

struct ResPltt {
    enum { REVISION = 1 };

    ResCommon<ResPlttData> mPltt; // at 0x0

    inline ResPltt(void *vptr) : mPltt(vptr) {}

    inline bool CheckRevision() const {
        return mPltt.ref().mRevision == REVISION;
    }

    void DCStore(bool);
};
} // namespace g3d
} // namespace nw4r

#endif
