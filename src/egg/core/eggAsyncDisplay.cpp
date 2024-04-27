#include <egg/core/eggAsyncDisplay.h>
#include <egg/math/eggMatrix.h>
#include <egg/core/eggXfbManager.h>
#include <rvl/GX/GXTexture.h>
#include <rvl/GX/GXHardware.h>

static EGG::AsyncDisplay *lbl_80576790[2];

// Hacks for float ordering
static f32 one() {
    return 1.0f;
}

static f64 cast() {
    return 4503599627370496.0;
}

static f32 zero() {
    return 0.0f;
}

namespace EGG {

AsyncDisplay::AsyncDisplay(u8 maxRetrace): Display(maxRetrace) {
    this->field_0x98 = 0;
    this->field_0x9C = 1.0f;
    this->field_0xA4 = 0;
    this->field_0xA8 = 0;
    this->field_0xAC = 0;
    this->field_0xB8 = 0;
    lbl_80576790[0] = this;
    OSInitThreadQueue(&this->mThreadQueue);
    OSInitMessageQueue(&this->mMesgQueue, this->mMesgBuf, 4);
}

f32 AsyncDisplay::calcS() {
    f32 tmp = this->field_0x9C + this->field_0xA0;
    this->field_0x9C = tmp;
    if (!(tmp > 1.0f)) {
        return tmp;
    }

    if (this->mScreenStateFlag.onBit(4)) {
        this->field_0x9C = tmp - 1.0f;
        this->mScreenStateFlag.resetBit(4);
    } else {
        this->field_0x9C = tmp - this->field_0xA0;
        this->mScreenStateFlag.setBit(4);
    }
    return tmp;
}

inline void AsyncDisplay::waitForMsg(bool b) {
    // This is really ugly but I couldn't find a different way
    // to make the compiler use the registers
    XfbManager *xfb;
    while (!((xfb = BaseSystem::mConfigData->getXfbMgr(), 1 < xfb->mNumXfbs - this->field_0xAC) && (BaseSystem::mConfigData->getXfbMgr()->mToCopyXfb != nullptr || b))) {

        OSReceiveMessage(&this->mMesgQueue, this->mMesgBuf, 1);

        while (OSReceiveMessage(&this->mMesgQueue, this->mMesgBuf, 0)) {}
    }
}

void AsyncDisplay::beginFrame() {
    BaseSystem::mConfigData->getXfbMgr();
    GXDrawDone();

    if (this->mFlag.onBit(1)) {
        OSSleepThread(&this->mThreadQueue);
    }
    bool b = false;
    if (BaseSystem::mConfigData->getXfbMgr()->mNumXfbs == 1) {
        s32 tick = OSGetTick();
        s32 lastTick = this->mLastTick;
        if (tick - lastTick > Display::sTickPeriod) {
            VIWaitForRetrace();
        }
    } else {
        b = false;
        if (this->field_0x98 == 1 && this->mScreenStateFlag.onBit(6)) {
            b = true;
        }

        this->waitForMsg(b);
    }

    if (b) {
        this->clearEFB();
        this->mScreenStateFlag.resetBit(6);
        GXDrawDone();
    } else {
        this->copyEFBtoXFB();
    }

    if (this->field_0x98 == 1) {
        this->calcS();
    }
    this->field_0xAC++;
    this->mFrameCount++;
    this->calcFrequency();
}

void AsyncDisplay::endFrame() {
    if (this->mScreenStateFlag.onBit(4)) {
        this->mScreenStateFlag.setBit(5);
    }
}

void AsyncDisplay::endRender() {
    if (this->mScreenStateFlag.onBit(5)) {
        this->mScreenStateFlag.setBit(6);
        this->mScreenStateFlag.resetBit(5);
    }
}

void AsyncDisplay::preVRetrace() {
    s32 tick = OSGetTick();
    this->mLastTick = tick;
    if (this->field_0x98 == 0) {
        u32 a8 = ++this->field_0xA8;
        if (a8 - this->field_0xA4 >= this->mMaxRetraces) {
            this->field_0xA4 = a8;
            this->mScreenStateFlag.setBit(3);
            OSSendMessage(&this->mMesgQueue, this->mMesgBuf, 0);
            if (this->field_0xAC > 0) {
                this->field_0xAC--;
            }
        }
        OSWakeupThread(&this->mThreadQueue);
    }

    if (this->mScreenStateFlag.onBit(3)) {
        s32 count = VIGetRetraceCount();
        if (count - this->mRetraceCount >= this->mMaxRetraces) {
            if (this->mScreenStateFlag.onBit(0)) {
                Video *video = BaseSystem::mConfigData->getVideo();
                VISetBlack(video->mFlag.offBit(0));
                if (video->mFlag.offBit(0)) {
                    video->mFlag.setBit(0);
                } else {
                    video->mFlag.resetBit(0);
                }
                this->mScreenStateFlag.resetBit(0);
            }
            XfbManager *xfb = BaseSystem::mConfigData->getXfbMgr();
            xfb->setNextFrameBuffer();
            s32 newCount = VIGetRetraceCount();
            this->mRetraceCount = newCount;
            this->mScreenStateFlag.resetBit(3);
        }
    }
}

void AsyncDisplay::clearEFB() {
    Video *video = BaseSystem::mConfigData->getVideo();
    GXRenderModeObj const *renderMode = video->pRenderMode;
    this->clearEFB(renderMode->fbWidth, renderMode->efbHeight, 0, 0, renderMode->fbWidth, renderMode->efbHeight, this->mClearColor);
}

u32 AsyncDisplay::getTickPerFrame() {
    if (this->field_0x98 == 1) {
        return Video::getTickPerVRetrace(0);
    } else {
        return Video::getTickPerVRetrace();
    }
}

// TODO move
extern "C" {
    extern GXTexObj lbl_80673B20, lbl_8056EB20;
    extern Matrix34f lbl_80674C00;
}

void AsyncDisplay::clearEFB(u16 fbWidth, u16 fbHeight, u16 x, u16 y, u16 width, u16 height, nw4r::ut::Color color) {

    GXInitTexObj(&lbl_80673B20, &lbl_8056EB20, 4, 4, GX_TF_Z24X8, GX_REPEAT, GX_REPEAT, 0);
    GXInitTexObjLOD(&lbl_80673B20, GX_NEAR, GX_NEAR, 0.0, 0.0, 0.0, 0, 0, GX_ANISO_1);
    Mtx44 mat;
    C_MTXOrtho(mat, 0.0, fbHeight, 0.0, fbWidth, 0.0, 1.0f);
    GXSetProjection(mat, GX_ORTHOGRAPHIC);
    GXSetViewport(0.0, 0.0, fbWidth, fbHeight, 0.0, 1.0f);
    GXSetScissor(0, 0, fbWidth, fbHeight);
    lbl_80674C00.loadPosMtx(0);
    GXSetCurrentMtx(0);
    GXClearVtxDesc();
    GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
    GXSetVtxDesc(GX_VA_TEX0, GX_DIRECT);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_CLR_RGB, GX_RGBX8, 0);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_CLR_RGBA, GX_RGB565, 0);
    GXSetNumChans(0);
    GXSetChanCtrl(GX_COLOR0A0, false, GX_SRC_REG, GX_SRC_REG, GX_LIGHT_NULL, GX_DF_NONE, GX_AF_NONE);
    GXSetChanCtrl(GX_COLOR1A1, false, GX_SRC_REG, GX_SRC_REG, GX_LIGHT_NULL, GX_DF_NONE, GX_AF_NONE);
    GXSetNumTexGens(1);
    GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, 0x3c, false, 0x7d);
    GXLoadTexObj(&lbl_80673B20, GX_TEXMAP0);
    GXSetNumTevStages(1);
    GXSetTevColor(GX_TEVREG0, color);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR_NULL);
    GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO, GX_CC_C0);
    GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_TEV_SCALE_0, 1, GX_TEVPREV);
    GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_CA_A0);
    GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_TEV_SCALE_0, 1, GX_TEVPREV);
    GXSetAlphaCompare(GX_ALWAYS, 0, GX_AOP_OR, GX_ALWAYS, 0);
    GXSetZTexture(2, GX_TF_Z24X8, 0);
    GXSetZCompLoc(false);
    GXSetBlendMode(GX_BM_NONE, GX_BL_ZERO, GX_BL_ZERO, GX_LO_NOOP);
    GXSetAlphaUpdate(true);
    GXSetDstAlpha(true, color.a);
    GXSetZMode(true, GX_ALWAYS, true);
    GXSetCullMode(GX_CULL_BACK);
    GXBegin(GX_QUADS, GX_VTXFMT0, 4);

    GXPosition2u16(x, y);
    GXPosition2u8(0, 0);
    u16 tmp = x + width;
    GXPosition2u16(tmp, y);
    GXPosition2u8(1, 0);
    u16 tmp2 = y + height;
    GXPosition2u16(tmp, tmp2);
    GXPosition2u8(1, 1);
    GXPosition2u16(x, tmp2);
    GXPosition2u8(0, 1);

    // GXEnd();

    GXSetZTexture(0, GX_TF_Z24X8, 0);
    GXSetZCompLoc(true);
    GXSetDstAlpha(false, color.a);
}

} // namespace EGG
