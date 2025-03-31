#ifndef NW4R_G3D_RES_RES_TEX_H
#define NW4R_G3D_RES_RES_TEX_H
#include <nw4r/types_nw4r.h>

#include "nw4r/g3d/res/g3d_rescommon.h"

#include "rvl/GX.h" // IWYU pragma: export

namespace nw4r {
namespace g3d {

struct ResTexData {
    enum Flag {
        FLAG_CIFMT = (1 << 0),
    };

    ResBlockHeaderData header; // at 0x0
    u32 revision;              // at 0x8
    s32 toResFileData;         // at 0xC
    s32 toTexData;             // at 0x10
    s32 name;                  // at 0x14
    u32 flag;                  // at 0x18
    u16 width;                 // at 0x1C
    u16 height;                // at 0x1E
    union {
        GXTexFmt fmt;
        GXCITexFmt cifmt;
    }; // at 0x20
    u32 mipmap_level;  // at 0x24
    f32 min_lod;       // at 0x28
    f32 max_lod;       // at 0x2C
    s32 original_path; // at 0x30
    s32 toResUserData; // at 0x34
};

class ResTex : public ResCommon<ResTexData> {
public:
    static const u32 SIGNATURE = 'TEX0';
    static const int REVISION = 1;

public:
    NW4R_G3D_RESOURCE_FUNC_DEF(ResTex);

    void Init();

    u32 GetRevision() const {
        return ref().revision;
    }

    bool CheckRevision() const {
        return GetRevision() == REVISION;
    }

    bool GetTexObjParam(
        void **ppTexData, u16 *pWidth, u16 *pHeight, GXTexFmt *pFormat, f32 *pMinLod, f32 *pMaxLod, GXBool *pMipMap
    ) const;

    bool GetTexObjCIParam(
        void **ppTexData, u16 *pWidth, u16 *pHeight, GXCITexFmt *pFormatCI, f32 *pMinLod, f32 *pMaxLod, GXBool *pMipMap
    ) const;

    bool IsCIFmt() const {
        return ref().flag & ResTexData::FLAG_CIFMT;
    }

    u16 GetWidth() const {
        return ref().width;
    }
    u16 GetHeight() const {
        return ref().height;
    }

    const void *GetTexData() const {
        return ofs_to_ptr<void>(ref().toTexData);
    }
};

} // namespace g3d
} // namespace nw4r

#endif
