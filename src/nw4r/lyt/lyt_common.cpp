#include "nw4r/lyt/lyt_common.h"

#include "nw4r/lyt/lyt_layout.h"
#include "nw4r/ut/ut_Color.h"
#include "nw4r/ut/ut_algorithm.h"


namespace nw4r {
namespace lyt {
using namespace math;
using namespace ut;

// EqualsResName__Q34nw4r3lyt6detailFPCcPCc
bool detail::EqualsResName(const char *name1, const char *name2) {
    return strncmp(name1, name2, NW4R_RES_NAME_SIZE) == 0;
}

// EqualsMaterialName__Q34nw4r3lyt6detailFPCcPCc
bool detail::EqualsMaterialName(const char *name1, const char *name2) {
    return strncmp(name1, name2, NW4R_MAT_NAME_SIZE) == 0;
}

// TestFileHeader__Q34nw4r3lyt6detailFRCQ44nw4r3lyt3res16BinaryFileHeaderUl
bool detail::TestFileHeader(const res::BinaryFileHeader &fileHeader, u32 testSig) {
    return ((testSig == detail::GetSignatureInt(fileHeader.signature)) && (fileHeader.byteOrder == NW4R_BYTEORDER_BIG));
}

namespace detail {
// __ct__Q44nw4r3lyt6detail11TexCoordAryFv
TexCoordAry::TexCoordAry() : mpData(nullptr), mCap(0), mNum(0) {}

// Free__Q44nw4r3lyt6detail11TexCoordAryFv
void TexCoordAry::Free() {
    if (this->mpData) {
        Layout::DeleteArray<VEC2>(*this->mpData, this->mNum * TEXCOORD_VTX_COUNT);
        this->mpData = nullptr;
        this->mCap = 0;
        this->mNum = 0;
    }
}

// Reserve__Q44nw4r3lyt6detail11TexCoordAryFUc
void TexCoordAry::Reserve(u8 num) {
    if (mCap < num) {
        Free();
        TexCoordData *pData = (TexCoordData *)Layout::NewArray<VEC2>(num * TEXCOORD_VTX_COUNT);
        this->mpData = pData;
        if (mpData != NULL) {
            mCap = num;
        }
    }
}

// SetSize__Q44nw4r3lyt6detail11TexCoordAryFUc
void TexCoordAry::SetSize(u8 num) {
    if (this->mpData != nullptr && num <= this->mCap) {
        static TexCoordData texCoords = {VEC2(0.0f, 0.0f), VEC2(1.0f, 0.0f), VEC2(0.0f, 1.0f), VEC2(1.0f, 1.0f)};

        for (int i = mNum; i < num; i++) {
            for (int j = 0; j < TEXCOORD_VTX_COUNT; j++) {
                this->mpData[i][j] = texCoords[j];
            }
        }
        this->mNum = num;
    }
}

// Copy__Q44nw4r3lyt6detail11TexCoordAryFPCvUc
void TexCoordAry::Copy(const void *pResTexCoord, u8 texCoordNum) {
    mNum = Max<u8>(mNum, texCoordNum);

    const TexCoordData *src = static_cast<const TexCoordData *>(pResTexCoord);
    for (int i = 0; i < texCoordNum; i++) {
        for (int j = 0; j < TEXCOORD_VTX_COUNT; j++) {
            mpData[i][j] = src[i][j];
        }
    }
}

} // namespace detail

// IsModulateVertexColor__Q34nw4r3lyt6detailFPQ34nw4r2ut5ColorUc
bool detail::IsModulateVertexColor(ut::Color *vtxColors, u8 glbAlpha) {
    if (glbAlpha != 0xFF) {
        return true;
    }

    if ((vtxColors != NULL) && ((vtxColors[0] != 0xFFFFFFFF) || (vtxColors[1] != 0xFFFFFFFF) ||
                                (vtxColors[2] != 0xFFFFFFFF) || (vtxColors[3] != 0xFFFFFFFF))) {
        return true;
    } else {
        return false;
    }
}

// MultipleAlpha__Q34nw4r3lyt6detailFQ34nw4r2ut5ColorUc
ut::Color detail::MultipleAlpha(ut::Color col, u8 alpha) {
    ut::Color ret = col;
    if (alpha != 0xFF) {
        ret.a = (col.a * alpha) / 0xFF;
    }
    return ret;
}

// MultipleAlpha__Q34nw4r3lyt6detailFPQ34nw4r2ut5ColorPCQ34nw4r2ut5ColorUc
void detail::MultipleAlpha(ut::Color *dst, const ut::Color *src, u8 alpha) {
    for (int i = 0; i < TEXCOORD_VTX_COUNT; i++) {
        dst[i] = MultipleAlpha(src[i], alpha);
    }
}

// SetVertexFormat__Q34nw4r3lyt6detailFbUc
void detail::SetVertexFormat(bool bModulate, u8 textCoordNum) {
    GXClearVtxDesc();

    GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
    if (bModulate) {
        GXSetVtxDesc(GX_VA_CLR0, GX_DIRECT);
    }

    for (int i = 0; i < textCoordNum; i++) {
        GXSetVtxDesc((GXAttr)(GX_VA_TEX0 + i), GX_DIRECT);
    }

    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XY, GX_F32, 0);
    if (bModulate) {
        GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_CLR0, GX_CLR_RGBA, GX_RGBA8, 0);
    }

    for (int i = 0; i < textCoordNum; i++) {
        GXSetVtxAttrFmt(GX_VTXFMT0, (GXAttr)(GX_VA_TEX0 + i), GX_TEX_ST, GX_F32, 0);
    }
}

// DrawQuad__Q34nw4r3lyt6detailFRCQ34nw4r4math4VEC2RCQ34nw4r3lyt4SizeUcPA4_CQ34nw4r4math4VEC2PCQ34nw4r2ut5Color
void detail::DrawQuad(
    const math::VEC2 &basePt, const Size &size, u8 texCoordNum, const TexCoordData *texCoords,
    const ut::Color *vtxColors
) {
    GXBegin(GX_QUADS, GX_VTXFMT0, 4);

    GXPosition2f32(basePt.x, basePt.y);
    if (vtxColors != NULL) {
        GXColor1u32(vtxColors[0]);
    }
    for (int i = 0; i < texCoordNum; i++) {
        GXTexCoord2f32(texCoords[i][0].x, texCoords[i][0].y);
    }

    GXPosition2f32(basePt.x + size.width, basePt.y);
    if (vtxColors != NULL) {
        GXColor1u32(vtxColors[1]);
    }
    for (int i = 0; i < texCoordNum; i++) {
        GXTexCoord2f32(texCoords[i][1].x, texCoords[i][1].y);
    }

    GXPosition2f32(basePt.x + size.width, basePt.y - size.height);
    if (vtxColors != NULL) {
        GXColor1u32(vtxColors[3]);
    }
    for (int i = 0; i < texCoordNum; i++) {
        GXTexCoord2f32(texCoords[i][3].x, texCoords[i][3].y);
    }

    GXPosition2f32(basePt.x, basePt.y - size.height);
    if (vtxColors != NULL) {
        GXColor1u32(vtxColors[2]);
    }
    for (int i = 0; i < texCoordNum; i++) {
        GXTexCoord2f32(texCoords[i][2].x, texCoords[i][2].y);
    }
}

// DrawQuad__Q34nw4r3lyt6detailFRCQ34nw4r4math4VEC2RCQ34nw4r3lyt4SizeUcPA4_CQ34nw4r4math4VEC2PCQ34nw4r2ut5ColorUc
void detail::DrawQuad(
    const math::VEC2 &basePt, const Size &size, u8 texCoordNum, const TexCoordData *texCoords,
    const ut::Color *vtxColors, u8 alpha
) {
    ut::Color wkVtxColors[4];
    if (vtxColors) {
        MultipleAlpha(wkVtxColors, vtxColors, alpha);
    }
    DrawQuad(basePt, size, texCoordNum, texCoords, vtxColors ? wkVtxColors : nullptr);
}

// DrawLine__Q34nw4r3lyt6detailFRCQ34nw4r4math4VEC2RCQ34nw4r3lyt4SizeQ34nw4r2ut5Color
void detail::DrawLine(const math::VEC2 &pos, const Size &size, ut::Color color) {
    // Not Impl in SS
}
} // namespace lyt
} // namespace nw4r
