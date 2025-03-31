#include "nw4r/g3d.h" // IWYU pragma: export

#include "rvl/GX.h" // IWYU pragma: export

namespace nw4r {
namespace g3d {

Fog::Fog(FogData *pData) : ResCommon(pData) {}

void Fog::Init() {
    if (!IsValid()) {
        return;
    }

    FogData &r = ref();

    r.type = GX_FOG_NONE;

    r.startz = 0.0f;
    r.endz = 0.0f;
    r.nearz = 0.0f;
    r.farz = 0.0f;

    r.color.r = r.color.g = r.color.b = r.color.a = 0;

    r.adjEnable = FALSE;
    r.PADDING_0x19 = 0;

    r.adjCenter = 0;
    for (int i = 0; i < GX_FOG_ADJ_TABLE_SZ; i++) {
        r.adjTable.r[i] = 0;
    }
}

Fog Fog::CopyTo(register void *pDst) const {
    if (pDst != NULL && IsValid()) {
        register const FogData *pSrc = ptr();
        register f64 work0, work1, work2, work3, work4, work5;

        // clang-format off
        asm {
            lfd  work0, 0(pSrc)
            stfd work0, 0(pDst)

            lfd  work1, 8(pSrc)
            stfd work1, 8(pDst)

            lfd  work2, 16(pSrc)
            stfd work2, 16(pDst)

            lfd  work3, 24(pSrc)
            stfd work3, 24(pDst)

            lfd  work4, 32(pSrc)
            stfd work4, 32(pDst)

            lfd  work5, 40(pSrc)
            stfd work5, 40(pDst)
        }
        // clang-format on

        return Fog(static_cast<FogData *>(pDst));
    }

    return Fog(NULL);
}

void Fog::SetFogRangeAdjParam(u16 width, u16 center, const math::MTX44 &rProjMtx) {
    if (!IsValid()) {
        return;
    }

    FogData &r = ref();

    r.adjCenter = center;
    GXInitFogAdjTable(&r.adjTable, width, rProjMtx);
}

void Fog::SetGP() const {
    if (!IsValid()) {
        return;
    }

    const FogData &r = ref();

    if (r.type != GX_FOG_NONE) {
        GXSetFogRangeAdj(r.adjEnable, r.adjCenter, &r.adjTable);
    }

    GXSetFog(r.type, r.color, r.startz, r.endz, r.nearz, r.farz);
}

} // namespace g3d
} // namespace nw4r
