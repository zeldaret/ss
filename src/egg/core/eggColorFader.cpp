#include <egg/core/eggColorFader.h>
#include <rvl/GX/GXTexture.h>
#include <rvl/GX/GXHardware.h>

// This file basically matches, but it pulls in some weak symbols that
// we still need to identify later.

/** 80497930 */
EGG::ColorFader::ColorFader(float startX, float startY, float lengthX, float lengthY, nw4r::ut::Color color, Fader::EStatus initialStatus)
    : mStartX(startX), mStartY(startY), mEndX(startX+lengthX), mEndY(startY+lengthY), mFrame(0x14), mFadeTimer(0) {
    setColor(color);
    setStatus(initialStatus);
    mFlags.setBit(1);
}

void EGG::ColorFader::setFrame(u16 frames) {
    mFrame = frames;
}

void EGG::ColorFader::setColor(nw4r::ut::Color color) {
    mColor.r = color.r;
    mColor.g = color.g;
    mColor.b = color.b;
    // Don't set alpha
}

void EGG::ColorFader::setStatus(Fader::EStatus status) {
    if (status == STATUS_PREPARE_IN) {
        mStatus = STATUS_PREPARE_IN;
        mColor.a = 255;
    } else if (status == STATUS_PREPARE_OUT) {
        mStatus = STATUS_PREPARE_OUT;
        mColor.a = 0;
    }
}

bool EGG::ColorFader::fadeIn() {
    bool doFadeIn = mStatus == STATUS_PREPARE_IN;
    if (doFadeIn) {
        mStatus = STATUS_FADE_IN;
        mFadeTimer = 0;
    }

    return doFadeIn;
}

bool EGG::ColorFader::fadeOut() {
    bool doFadeOut = mStatus == STATUS_PREPARE_OUT;
    if (doFadeOut) {
        mStatus = STATUS_FADE_OUT;
        mFadeTimer = 0;
    }

    return doFadeOut;
}

bool EGG::ColorFader::calc() {
    bool result = false;

    if (mStatus == STATUS_PREPARE_OUT) {
        mColor.a = 0;
    } else if (mStatus == STATUS_PREPARE_IN) {
        mColor.a = 255;
    } else if (mStatus == STATUS_FADE_IN) {
        u16 endFrame = mFrame;
        u16 currFrame = mFadeTimer++;
        if (currFrame > endFrame) {
            mStatus = STATUS_PREPARE_OUT;
            result = mFlags.onBit(0);
            currFrame = mFrame;
        }
        mColor.a = 255 - (currFrame * 255 / mFrame);
    } else if (mStatus == STATUS_FADE_OUT) {
        u16 endFrame = mFrame;
        u16 currFrame = mFadeTimer++;
        if (currFrame > endFrame) {
            if (currFrame > endFrame + 1) {
                mStatus = STATUS_PREPARE_IN;
                result = mFlags.onBit(1);
            }
            endFrame = mFrame;
            currFrame = endFrame;
        }
        mColor.a = currFrame * 255 / endFrame;
    }

    return result;
}

void EGG::ColorFader::draw() {
    if (mColor.a == 0) {
        return;
    }

    Mtx44 projMtx;
    C_MTXOrtho(projMtx, mStartY, mEndY, mStartX, mEndX, 0.0f, 1.0f);
    GXSetProjection(projMtx, GX_ORTHOGRAPHIC);

    GXSetViewport(mStartX, mStartY, getWidth(), getHeight(), 0.0f, 1.0f);
    GXSetScissor(mStartX, mStartY, getWidth(), getHeight());

    Mtx posMtx;
    PSMTXIdentity(posMtx);
    GXLoadPosMtxImm(posMtx, 0);
    GXSetCurrentMtx(0);

    GXClearVtxDesc();
    GXInvalidateVtxCache();

    GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_CLR_RGBA, GX_F32, 0);

    GXSetNumChans(1);
    GXSetChanMatColor(GX_COLOR0A0, mColor);
    GXSetChanCtrl(GX_COLOR0A0, 0, GX_SRC_REG, GX_SRC_REG, GX_LIGHT_NULL, GX_DF_NONE, GX_AF_NONE);

    GXSetNumTexGens(0);
    GXSetNumIndStages(0);
    __GXSetIndirectMask(0);

    GXSetNumTevStages(1);
    GXSetTevOp(GX_TEVSTAGE0, 4);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD_NULL, GX_TEXMAP_NULL, GX_COLOR0A0);

    if (mColor.a == 255) {
        GXSetBlendMode(GX_BM_NONE, GX_BL_ONE, GX_BL_ZERO, GX_LO_SET);
    } else {
        GXSetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA, GX_LO_SET);
    }

    GXSetColorUpdate(1);
    GXSetAlphaUpdate(1);
    GXSetZMode(0, GX_NEVER, 0);
    GXSetCullMode(GX_CULL_BACK);

    GXBegin(GX_QUADS, GX_VTXFMT0, 4);

    GXPosition3f32(mStartX, mStartY, 0.0f);
    GXPosition3f32(mEndX, mStartY, 0.0f);
    GXPosition3f32(mEndX, mEndY, 0.0f);
    GXPosition3f32(mStartX, mEndY, 0.0f);

    // GXEnd();
}
