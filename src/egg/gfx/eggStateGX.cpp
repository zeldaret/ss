#include "egg/gfx/eggStateGX.h"

#include "common.h"
#include "egg/core/eggDisplay.h"
#include "egg/core/eggSystem.h"
#include "nw4r/g3d/g3d_init.h"
#include "nw4r/g3d/platform/g3d_tmem.h"
#include "rvl/GX/GXAttr.h"
#include "rvl/GX/GXBump.h"
#include "rvl/GX/GXFrameBuf.h"
#include "rvl/GX/GXGeometry.h"
#include "rvl/GX/GXPixel.h"
#include "rvl/GX/GXTexture.h"
#include "rvl/GX/GXTransform.h"
#include "rvl/GX/GXTypes.h"

#include <cstring>

namespace {

static GXTexRegion *texRegionCallback(const GXTexObj *pObj, GXTexMapID map) {
    return &EGG::StateGX::s_tmem_layout.mRegions[map % EGG::StateGX::s_tmem_layout.numRegions];
}

static const GXColor white = {0xFF, 0xFF, 0xFF, 0xFF};

static const f32 identity[3][4] = {
    {1.0f, 0.0f, 0.0f, 0.0f},
    {0.0f, 1.0f, 0.0f, 0.0f},
    {0.0f, 0.0f, 1.0f, 0.0f},
};

} // namespace

namespace EGG {

GXTexObj ALIGN_DECL(32) StateGX::sDefaultTexObj;
StateGX::MemLayout StateGX::s_tmem_layout;
StateGX::CacheGX StateGX::s_cacheGX;

u16 StateGX::s_widthEfb;
u16 StateGX::s_heightEfb;
GXPixelFmt StateGX::s_pixFormat;
GXZFmt16 StateGX::s_zFmt16;
GXColor StateGX::s_clearEfb;
u16 StateGX::s_flag;
u16 StateGX::s_commandFlag;

void StateGX::initialize(u16 width, u16 height, GXColor color, GXPixelFmt pixelFmt) {
    s_widthEfb = width;
    s_heightEfb = height;
    s_clearEfb = color;
    s_pixFormat = pixelFmt;

    if (BaseSystem::getDisplay() != nullptr) {
        BaseSystem::getDisplay()->setClearColor(color);
    }

    static u8 sDefaultTexObjData[16] = {0};

    GXInitTexObj(&sDefaultTexObj, sDefaultTexObjData, 4, 4, GX_TF_IA8, GX_CLAMP, GX_CLAMP, 0);
    GXInitTexObjLOD(&sDefaultTexObj, GX_LINEAR, GX_LINEAR, 0.0f, 0.0f, 0.0f, 0, 0, GX_ANISO_1);
    invalidateTexAllGX();
    frameInit();
    setTMemLayout(nw4r::g3d::tmem::TMEM_LAYOUT_1);
}

void StateGX::frameInit() {
    GXSetPixelFmt_(s_pixFormat, s_zFmt16);
    if (BaseSystem::mConfigData->getDisplay() != nullptr) {
        s_clearEfb = BaseSystem::mConfigData->getDisplay()->getClearColor();
    }
    GXInvalidateVtxCache();
    GXSetCurrentMtx(0);
    GXSetCoPlanar(false);
    GXSetClipMode(GX_CLIP_ENABLE);
    resetGX();
    nw4r::g3d::G3dReset();
}

void StateGX::setTMemLayout(int layout) {
    s_tmem_layout.mLayout = layout;
    if (nw4r::g3d::tmem::TMEM_LAYOUT_NONE <= layout && layout <= nw4r::g3d::tmem::TMEM_LAYOUT_3) {
        nw4r::g3d::tmem::SetTMemLayout(static_cast<nw4r::g3d::tmem::TMemLayout>(layout));
        s_tmem_layout.numRegions = 8;
    } else {
        static const u32 numbers[4] = {0x8, 0x4, 0x1, 0x7};
        static const u32 offsets[4] = {0x8000, 0x20000, 0x80000, 0};

        u32 index = layout - 4;
        s_tmem_layout.numRegions = numbers[index];
        for (u32 i = 0; i < 8; i++) {
            std::memset(&s_tmem_layout.mRegions[i], 0, sizeof(GXTexRegion));
        }

        u32 addr = 0;
        for (u32 i = 0; i < numbers[index]; i++) {
            int u1 = layout == 7 ? (0 <= i && i < 3 ? 1 : 0) : index;
            if ((i & 1) != 0) {
                GXInitTexCacheRegion(&s_tmem_layout.mRegions[i], false, addr + 0x80000, u1, addr, u1);
            } else {
                GXInitTexCacheRegion(&s_tmem_layout.mRegions[i], false, addr, u1, addr + 0x80000, u1);
            }
            addr += offsets[u1];
        }
        GXSetTexRegionCallback(texRegionCallback);
    }
}

void StateGX::resetGX() {
    if ((s_flag & 1) == 0) {
        resetVtx();
        resetIndirect();
        resetColorChannel();
        resetTexture();
        resetTexGen();
        resetTev();
        resetPE();
    }
    resetGXCache();
}

void StateGX::resetVtx() {
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_CLR_RGBA, GX_F32, 0);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_NRM, GX_CLR_RGB, GX_F32, 0);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_CLR0, GX_CLR_RGBA, GX_RGBA8, 0);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_CLR1, GX_CLR_RGBA, GX_RGBA8, 0);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_CLR_RGBA, GX_RGBA4, 7);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX1, GX_CLR_RGBA, GX_RGBA4, 7);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX2, GX_CLR_RGBA, GX_RGBA4, 7);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX3, GX_CLR_RGBA, GX_RGBA4, 7);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX4, GX_CLR_RGBA, GX_RGBA4, 7);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX5, GX_CLR_RGBA, GX_RGBA4, 7);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX6, GX_CLR_RGBA, GX_RGBA4, 7);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX7, GX_CLR_RGBA, GX_RGBA4, 7);
}

void StateGX::resetColorChannel() {
    GXSetNumChans(1);
    GXSetChanAmbColor(GX_COLOR0A0, white);
    GXSetChanAmbColor(GX_COLOR1A1, white);

    GXSetChanMatColor(GX_COLOR0A0, white);

    GXSetChanMatColor(GX_COLOR1A1, white);
    GXSetCullMode(GX_CULL_BACK);
    GXSetChanCtrl(GX_COLOR0A0, false, GX_SRC_REG, GX_SRC_REG, GX_LIGHT_NULL, GX_DF_CLAMP, GX_AF_NONE);
    GXSetChanCtrl(GX_COLOR1A1, false, GX_SRC_REG, GX_SRC_REG, GX_LIGHT_NULL, GX_DF_CLAMP, GX_AF_NONE);
}

void StateGX::resetIndirect() {
    GXSetNumIndStages(0);
    static const f32 ind[2][3] = {
        {0.5f, 0.0f, 0.0f},
        {0.0f, 0.5f, 0.0f},
    };
    for (u32 i = 0; i < 3; i++) {
        GXSetIndTexMtx(static_cast<GXIndTexMtxID>(i + GX_ITM_0), ind, 1);
    }
    for (u32 i = 0; i < 4; i++) {
        GXSetIndTexCoordScale(static_cast<GXIndTexStageID>(i + GX_INDTEXSTAGE0), GX_ITS_1, GX_ITS_1);
    }
}

void StateGX::resetTexture() {
    for (u32 i = 0; i < s_tmem_layout.numRegions; i++) {
        GXLoadTexObj(&sDefaultTexObj, static_cast<GXTexMapID>(i + GX_TEXMAP0));
    }
}

void StateGX::resetTexGen() {
    GXSetNumTexGens(1);

    for (u32 i = 0; i < 10; i++) {
        GXLoadTexMtxImm(identity, i * 3 + 0x1E, GX_MTX3x4);
    }

    for (u32 i = 0; i < GX_MAX_TEXCOORD; i++) {
        GXTexCoordID id = static_cast<GXTexCoordID>(i);
        GXSetTexCoordGen2(id, GX_TG_MTX2x4, GX_TG_TEX0, 0x3c, 0, 0x7d);
        GXSetTexCoordScaleManually(id, false, 0, 0);
        GXSetTexCoordBias(id, false, false);
    }
}

void StateGX::resetTev() {
    GXSetNumTevStages(1);
    for (u32 i = 0; i < GX_MAX_TEVSTAGE; i++) {
        GXTevStageID id = static_cast<GXTevStageID>(i);
        GXSetTevDirect(id);
        GXSetTevOrder(id, GX_TEXCOORD_NULL, GX_TEXMAP_NULL, GX_COLOR_NULL);
        GXSetTevColorIn(id, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO);
        GXSetTevColorOp(id, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, 1, GX_TEVPREV);
        GXSetTevAlphaIn(id, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO);
        GXSetTevAlphaOp(id, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, true, GX_TEVPREV);
        GXSetTevSwapMode(id, GX_TEV_SWAP0, GX_TEV_SWAP0);
    }

    for (u32 i = 0; i < GX_MAX_KCOLOR; i++) {
        GXTevKColorID id = static_cast<GXTevKColorID>(i);
        GXSetTevKColor(id, white);
    }

    for (u32 i = 0; i < GX_MAX_TEVREG; i++) {
        GXTevRegID id = static_cast<GXTevRegID>(i);
        GXSetTevColor(id, white);
    }

    GXSetTevSwapModeTable(GX_TEV_SWAP0, GX_CH_RED, GX_CH_GREEN, GX_CH_BLUE, GX_CH_ALPHA);
    GXSetTevSwapModeTable(GX_TEV_SWAP1, GX_CH_RED, GX_CH_RED, GX_CH_RED, GX_CH_ALPHA);
    GXSetTevSwapModeTable(GX_TEV_SWAP2, GX_CH_GREEN, GX_CH_GREEN, GX_CH_GREEN, GX_CH_ALPHA);
    GXSetTevSwapModeTable(GX_TEV_SWAP3, GX_CH_BLUE, GX_CH_BLUE, GX_CH_BLUE, GX_CH_ALPHA);
}

void StateGX::resetPE() {
    GXSetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA, GX_LO_NOOP);
    GXSetAlphaCompare(GX_ALWAYS, 0, GX_AOP_AND, GX_ALWAYS, 0);
    GXSetZTexture(0, GX_TF_Z24X8, 0);
    GXSetZMode(true, GX_LEQUAL, true);
    GXSetFog(GX_FOG_NONE, white, 0.0f, 1.0f, 0.1f, 1.0f);
    GXSetFogRangeAdj(false, 0, nullptr);
    GXSetZCompLoc(true);
    GXSetDstAlpha(false, 0);
}

void StateGX::resetGXCache() {
    GXSetColorUpdate_(true);
    GXSetAlphaUpdate_(true);
    GXSetDither_(true);
    GXSetDstAlpha_(false, 0);
}

void StateGX::GXSetPixelFmt_(GXPixelFmt pixelFmt, GXZFmt16 zFmt) {
    GXPixelFmt oldPixelFmt;
    GXZFmt16 oldZFmt;

    GXGetPixelFmt(&oldPixelFmt, &oldZFmt);
    if (pixelFmt != oldPixelFmt || zFmt != oldZFmt) {
        GXBool oldColorUpdate;
        GXGetColorUpdate(&oldColorUpdate);
        bool bOldColorUpdate = oldColorUpdate;
        GXBool oldAlphaUpdate;
        GXGetAlphaUpdate(&oldAlphaUpdate);
        bool bOldAlphaUpdate = oldAlphaUpdate;

        GXSetPixelFmt(pixelFmt, zFmt);
        GXSetColorUpdate(bOldColorUpdate);
        GXSetAlphaUpdate(bOldAlphaUpdate);
    }
}

void StateGX::invalidateTexAllGX() {
    GXInvalidateTexAll();
}

bool StateGX::GXSetColorUpdate_(bool enable) {
    GXBool oldEnable;
    GXGetColorUpdate(&oldEnable);
    bool bOldEnable = oldEnable;
    GXSetColorUpdate(enable);
    return bOldEnable;
}

bool StateGX::GXSetAlphaUpdate_(bool enable) {
    GXBool oldEnable;
    GXGetAlphaUpdate(&oldEnable);
    bool bOldEnable = oldEnable;
    GXSetAlphaUpdate(enable);
    return bOldEnable;
}

bool StateGX::GXSetDither_(bool enable) {
    GXBool oldEnable;
    GXGetDither(&oldEnable);
    bool bOldEnable = oldEnable;
    GXSetDither(enable);
    return bOldEnable;
}

void StateGX::GXCopyTex_(void *data, bool bUpdate) {
    GXCopyTex(data, bUpdate);
}

StateGX::CacheGX &StateGX::GXSetDstAlpha_(bool enable, u8 alpha) {
    GXSetDstAlpha(enable, alpha);
    s_cacheGX.mDstAlphaEnable = enable;
    s_cacheGX.mDstAlpha = alpha;
    return s_cacheGX;
}

void StateGX::CalculateScreenScissor(const f32 src[4], u32 dst[4]) {
    dst[0] = src[0] >= 0.0f ? src[0] : 0.0f;
    dst[1] = src[1] >= 0.0f ? src[1] : 0.0f;
    dst[2] = dst[0] + src[2] > 640.0f ? 640 - dst[0] : src[2];
    dst[3] = dst[1] + src[3] > 528.0f ? 528 - dst[1] : src[3];
}

void StateGX::GXSetProjection_(Mtx44 proj, GXProjectionType type) {
    GXSetProjection(proj, type);
    s_commandFlag |= 1;
}

void StateGX::GXSetProjectionv_(const f32 *proj) {
    GXSetProjectionv(proj);
    s_commandFlag |= 1;
}

void StateGX::GXSetViewport_(f32 ox, f32 oy, f32 sx, f32 sy, f32 near, f32 far) {
    GXSetViewport(ox, oy, sx, sy, near, far);
    s_commandFlag |= 2;
}

void StateGX::GXSetScissor_(u32 x, u32 y, u32 w, u32 h) {
    GXSetScissor(x, y, w, h);
    s_commandFlag |= 4;
}

void StateGX::GXSetScissorBoxOffset_(s32 ox, s32 oy) {
    GXSetScissorBoxOffset(ox, oy);
    s_commandFlag |= 8;
}

} // namespace EGG
