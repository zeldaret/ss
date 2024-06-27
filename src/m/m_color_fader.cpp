#include <m/m_color_fader.h>
#include <rvl/SC.h>
#include <rvl/VI.h>

mColorFader_c::mColorFader_c(const mColor &color, EStatus status) : mFaderBase_c(color, status) {
    mAspectRatio = SCGetAspectRatio();
}

mColorFader_c::~mColorFader_c() {}

void mColorFader_c::setStatus(EStatus status) {
    if (status == FADED_OUT) {
        mStatus = FADED_OUT;
        mFaderColor.a = 0xff;
    } else if (status == FADED_IN) {
        mStatus = FADED_IN;
        mFaderColor.a = 0;
    }
}

u8 mColorFader_c::calc() {
    u8 result = mFaderBase_c::calc();
    u16 elapsed = mElapsed;
    u16 frame = mFrame;
    if (elapsed > mFrame) {
        elapsed = frame;
    }

    switch (mStatus) {
        case FADED_IN:
            mFaderColor.a = 0;
            break;
        case FADED_OUT:
            mFaderColor.a = 0xff;
            break;
        case FADING_IN:
            mFaderColor.a = 0xff - (elapsed * 0xff / frame);
            break;
        case FADING_OUT:
            mFaderColor.a = elapsed * 0xff / frame;
            break;
    }
    

    return result;
}

#define VI_VIRTUAL_HALF_WIDTH_WIDE (406.0f)
#define VI_VIRTUAL_HALF_WIDTH_STD (304.0f)
#define VI_VIRTUAL_HALF_HEIGHT (228.0f)

void mColorFader_c::draw() {
    f32 h = (mAspectRatio == 1) ? VI_VIRTUAL_HALF_WIDTH_WIDE : VI_VIRTUAL_HALF_WIDTH_STD;

    if (mFaderColor.a == 0) {
        return;
    }

    Mtx44 projMtx;
    C_MTXOrtho(projMtx, -VI_VIRTUAL_HALF_HEIGHT, VI_VIRTUAL_HALF_HEIGHT, -h, h, 0, 1);
    GXSetProjection(projMtx, GX_ORTHOGRAPHIC);

    Mtx posMtx;
    PSMTXIdentity(posMtx);
    GXLoadPosMtxImm(posMtx, 0);
    GXSetCurrentMtx(0);

    GXClearVtxDesc();
    GXInvalidateVtxCache();

    GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_CLR_RGBA, GX_F32, 0);

    GXSetNumChans(1);
    GXSetChanMatColor(GX_COLOR0A0, mFaderColor);
    GXSetChanCtrl(GX_COLOR0A0, 0, GX_SRC_REG, GX_SRC_REG, GX_LIGHT_NULL, GX_DF_NONE, GX_AF_NONE);

    GXSetNumTexGens(0);
    GXSetNumIndStages(0);
    __GXSetIndirectMask(0);

    GXSetNumTevStages(1);
    GXSetTevOp(GX_TEVSTAGE0, 4);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD_NULL, GX_TEXMAP_NULL, GX_COLOR0A0);

    if (mFaderColor.a == 255) {
        GXSetBlendMode(GX_BM_NONE, GX_BL_ONE, GX_BL_ZERO, GX_LO_SET);
    } else {
        GXSetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA, GX_LO_SET);
    }

    GXSetColorUpdate(1);
    GXSetAlphaUpdate(1);
    GXSetZMode(0, GX_NEVER, 0);
    GXSetCullMode(GX_CULL_BACK);

    GXBegin(GX_QUADS, GX_VTXFMT0, 4);

    GXPosition3f32(-h, -VI_VIRTUAL_HALF_HEIGHT, 0);
    GXPosition3f32(h, -VI_VIRTUAL_HALF_HEIGHT, 0);
    GXPosition3f32(h, VI_VIRTUAL_HALF_HEIGHT, 0);
    GXPosition3f32(-h, VI_VIRTUAL_HALF_HEIGHT, 0);

    // GXEnd();
}
