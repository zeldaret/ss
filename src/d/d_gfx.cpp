#include "d/d_gfx.h"

#include "common.h"
#include "d/d_d2d.h"
#include "d/d_heap.h"
#include "d/d_sys.h"
#include "egg/gfx/eggDrawGX.h"
#include "egg/gfx/eggGfxEngine.h"
#include "egg/gfx/eggScreen.h"
#include "egg/gfx/eggTextureBuffer.h"
#include "m/m2d.h"
#include "m/m3d/m3d.h"
#include "m/m_heap.h"
#include "m/m_mtx.h"
#include "m/m_video.h"
#include "rvl/GX/GXTexture.h"
#include "rvl/GX/GXTransform.h"
#include "rvl/GX/GXTypes.h"
#include "rvl/GX/GXVert.h"
#include "rvl/MTX/mtx.h"
#include "rvl/MTX/mtx44.h"
#include "toBeSorted/d_d3d.h"

dGfx_c *dGfx_c::sInstance;

dGfx_c::dGfx_c() : mpTextureBuffer(nullptr), mBufRefcount(0), mLetterboxEnabled(false) {
    mDrawCallback = nullptr;

    d3d::create(mHeap::g_gameHeaps[0]);
    initGfxConstants();
    EGG::GfxEngine::Configuration cfg;
    u32 texBufSize = cfg.bufferSize;
    texBufSize += GXGetTexBufferSize(
        mVideo::m_video->pRenderMode->fbWidth, mVideo::m_video->pRenderMode->efbHeight, GX_TF_RGB5A3, GX_FALSE, 1
    );
    EGG::TextureBuffer::setIncludesHeader(true);
    {
        mHeap guard(mHeap::g_gameHeaps[0]);
        EGG::TextureBuffer::initialize(texBufSize, dSys_c::ms_RootHeapMem2);
    }
    m2d::create(dHeap::layoutHeap.heap, 0);
    d2d::create(dHeap::layoutExHeap.heap);
    sInstance = this;
}

EGG::TextureBuffer *dGfx_c::getTextureBuffer() {
    EGG::TextureBuffer *ret = nullptr;
    if (mpTextureBuffer == nullptr) {
        mpTextureBuffer = EGG::TextureBuffer::alloc(
            mVideo::m_video->pRenderMode->fbWidth, mVideo::m_video->pRenderMode->efbHeight, GX_TF_RGBA8
        );
        mpTextureBuffer->onCapFlag(0x20); // TODO
        ret = mpTextureBuffer;
    }
    mBufRefcount++;
    return ret;
}

void dGfx_c::releaseTextureBuffer() {
    mBufRefcount--;
    if (mBufRefcount == 0 && mpTextureBuffer != nullptr) {
        mpTextureBuffer->free();
        mpTextureBuffer = nullptr;
    }
}

void dGfx_c::drawBefore() {
    if (mDrawCallback != nullptr) {
        (mDrawCallback)();
    }
    d2d::drawBefore();
    m3d::clear();
    m2d::reset();
}

void dGfx_c::drawLetterbox() {
    if (mLetterboxEnabled && isTvMode4To3()) {
        EGG::Screen screen(*EGG::Screen::GetRoot());
        screen.SetNearFar(-1.0f, 1.0f);
        screen.SetProjectionGX();
        EGG::DrawGX::BeginDrawScreen(true, false, true);

        mMtx_c mtx1;
        mMtx_c mtx2;
        mMtx_c mtx3;

        mtx1.transS(0.0f, 0.0f, 0.0f);
        MTXScale(mtx2, getCurrentScreenWidthF(), getEFBHeightDifferenceF(), 0.0f);
        MTXConcat(mtx1, mtx2, mtx1);
        EGG::DrawGX::DrawDL17(mtx1, EGG::DrawGX::BLACK);

        mtx1.transS(0.0f, getCurrentScreenHeightF() - getEFBHeightDifferenceF() - 1.0f, 0.0f);
        MTXScale(mtx3, getCurrentScreenWidthF(), getEFBHeightDifferenceF() + 1.0f, 0.0f);
        MTXConcat(mtx1, mtx3, mtx1);
        EGG::DrawGX::DrawDL17(mtx1, EGG::DrawGX::BLACK);
    }
}

void dGfx_c::create(EGG::Heap *heap) {
    new (heap) dGfx_c();
}

void dGfx_c::drawCapTexture(EGG::TextureBuffer *buf, nw4r::ut::Color *clr) {
    if (buf == nullptr) {
        return;
    }

    s32 width = mVideo::m_video->pRenderMode->fbWidth;
    s32 height = mVideo::m_video->pRenderMode->efbHeight;
    s32 offsetY = 0;

    if (buf->getHeight() == getEFBHeightLimit()) {
        height = getEFBHeight();
        offsetY = getLetterboxAmount();
    }
    GXSetViewport(0.0f, offsetY, width, height, 0.0f, 1.0f);
    GXSetScissor(0, offsetY, width, height);
    GXSetScissorBoxOffset(0, 0);

    Mtx44 mtx;
    MTXOrtho(mtx, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f);
    GXSetProjection(mtx, GX_ORTHOGRAPHIC);
    buf->load(GX_TEXMAP0);
    GXSetNumChans(0);
    GXSetNumTexGens(1);
    GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, GX_IDENTITY, GX_FALSE, GX_DUALMTX_IDENT);
    GXSetNumIndStages(0);
    GXSetNumTevStages(1);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR_NULL);
    GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_C0, GX_CC_TEXC, GX_CC_ZERO);
    GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, 1, GX_TEVPREV);
    GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO);
    GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetZCompLoc(1);
    GXSetBlendMode(GX_BM_NONE, GX_BL_ONE, GX_BL_ONE, GX_LO_SET);
    GXSetZMode(GX_FALSE, GX_ALWAYS, GX_FALSE);
    GXSetAlphaCompare(GX_ALWAYS, 0, GX_AOP_OR, GX_ALWAYS, 0);

    GXSetFog(GX_FOG_NONE, (GXColor){0}, 0.0, 0.0, 0.0, 0.0);
    GXSetFogRangeAdj(GX_FALSE, 0, 0);
    GXSetCullMode(GX_CULL_NONE);
    GXSetTevColor(GX_TEVREG0, *clr);
    GXLoadPosMtxImm(mMtx_c::Identity, 0);
    GXSetCurrentMtx(0);
    GXClearVtxDesc();
    GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
    GXSetVtxDesc(GX_VA_TEX0, GX_DIRECT);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XY, GX_U8, 0);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_TEX_ST, GX_U8, 0);

    GXBegin(GX_QUADS, GX_VTXFMT0, 4);

    GXPosition2u8(0, 0);
    GXPosition2u8(0, 0);
    GXPosition2u8(1, 0);
    GXPosition2u8(1, 0);
    GXPosition2u8(1, 1);
    GXPosition2u8(1, 1);
    GXPosition2u8(0, 1);
    GXPosition2u8(0, 1);

    GXEnd();
}
