#include <nw4r/g3d.h>

namespace nw4r {
namespace g3d {

void ResPltt::DCStore(bool sync) {
    void* pBase = &ref();
    u32 size = ref().header.size;

    if (sync) {
        DC::StoreRange(pBase, size);
    } else {
        DC::StoreRangeNoSync(pBase, size);
    }
}

bool ResTex::GetTexObjParam(void** ppTexData, u16* pWidth, u16* pHeight,
                            GXTexFmt* pFormat, f32* pMinLod, f32* pMaxLod,
                            GXBool* pMipMap) const {
    const ResTexData& r = ref();

    if (IsCIFmt()) {
        return false;
    }

    if (ppTexData != NULL) {
        *ppTexData = const_cast<void*>(GetTexData());
    }

    if (pWidth != NULL) {
        *pWidth = GetWidth();
    }

    if (pHeight != NULL) {
        *pHeight = GetHeight();
    }

    if (pFormat != NULL) {
        *pFormat = r.fmt;
    }

    if (pMinLod != NULL) {
        *pMinLod = r.min_lod;
    }

    if (pMaxLod != NULL) {
        *pMaxLod = r.max_lod;
    }

    if (pMipMap != NULL) {
        *pMipMap = r.mipmap_level > 1;
    }

    return true;
}

bool ResTex::GetTexObjCIParam(void** ppTexData, u16* pWidth, u16* pHeight,
                              GXCITexFmt* pFormatCI, f32* pMinLod, f32* pMaxLod,
                              GXBool* pMipMap) const {
    const ResTexData& r = ref();

    if (!IsCIFmt()) {
        return false;
    }

    if (ppTexData != NULL) {
        *ppTexData = const_cast<void*>(GetTexData());
    }

    if (pWidth != NULL) {
        *pWidth = GetWidth();
    }

    if (pHeight != NULL) {
        *pHeight = GetHeight();
    }

    if (pFormatCI != NULL) {
        *pFormatCI = r.cifmt;
    }

    if (pMinLod != NULL) {
        *pMinLod = r.min_lod;
    }

    if (pMaxLod != NULL) {
        *pMaxLod = r.max_lod;
    }

    if (pMipMap != NULL) {
        *pMipMap = r.mipmap_level > 1;
    }

    return true;
}

void ResTex::Init() {
    ResTexData& r = ref();
    DC::FlushRangeNoSync(&r, r.header.size);
}

} // namespace g3d
} // namespace nw4r
