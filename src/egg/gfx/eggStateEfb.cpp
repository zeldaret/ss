#include "egg/gfx/eggStateEfb.h"

#include "common.h"
#include "egg/gfx/eggDrawGX.h"
#include "egg/gfx/eggGlobalDrawState.h"
#include "egg/gfx/eggScreen.h"
#include "egg/gfx/eggStateGX.h"
#include "egg/gfx/eggTextureBuffer.h"
#include "nw4r/math/math_types.h"
#include "rvl/GX/GXTypes.h"
#include "rvl/MTX/mtx.h"

namespace EGG {

StateEfb::Buffer StateEfb::spBufferSet[BUFFER_MAX];
f32 StateEfb::sWorkSpaceV[6];
f32 StateEfb::sWorkSpaceHideV[6];
f32 StateEfb::sShiftViewPort[6];

u32 StateEfb::sFlag = 4;
s32 StateEfb::sWorkBuffer = -1;

s32 StateEfb::sPushCount;

void StateEfb::Clean() {
    sFlag &= ~(1 | 2);
    sWorkBuffer = -1;
    sPushCount = 0;

    for (u32 i = 0; i < ARRAY_LENGTH(spBufferSet); i++) {
        spBufferSet[i].field_0x08 = 0;
    }

    for (u32 i = 0; i < ARRAY_LENGTH(sWorkSpaceV); i++) {
        sWorkSpaceV[i] = 0.0f;
        sWorkSpaceHideV[i] = 0.0f;
    }
}

TextureBuffer *StateEfb::captureEfb(BufferType type, bool noTransparencyMaybe, u32 userData) {
    // NONMATCHING - GPR regswaps
    const Screen::DataEfb &efb = GlobalDrawState::getScreen().GetDataEfb();

    if (spBufferSet[type].buf == nullptr) {
        f32 x1 = efb.vp.x1;
        f32 y1 = efb.vp.y1;

        bool doUpscale = false;
        bool doCapture = true;

        switch (type) {
            case BUFFER_0:
            case BUFFER_1: {
                f32 arg = type == BUFFER_0 ? 0.25f : 0.5f;

                sWorkSpaceV[2] = efb.vp.x2 * arg;
                sWorkSpaceV[3] = efb.vp.y2 * arg;

                sWorkSpaceV[2] = (int)sWorkSpaceV[2] + (int)sWorkSpaceV[2] % 2;
                sWorkSpaceV[3] = (int)sWorkSpaceV[3] + (int)sWorkSpaceV[3] % 2;

                if (sWorkSpaceV[2] > sWorkSpaceV[3]) {
                    sWorkSpaceV[3] = sWorkSpaceV[2];
                } else {
                    sWorkSpaceV[2] = sWorkSpaceV[3];
                }

                sWorkSpaceV[0] = 640.0f - sWorkSpaceV[2];
                sWorkSpaceHideV[0] = sWorkSpaceV[0];
                sWorkSpaceV[1] = 528.0f - sWorkSpaceV[3];
                sWorkSpaceHideV[1] = sWorkSpaceV[1];

                sWorkSpaceHideV[2] = sWorkSpaceV[2] - (640 - StateGX::s_widthEfb);
                sWorkSpaceHideV[3] = sWorkSpaceV[3] - (528 - StateGX::s_heightEfb);

                if (sWorkSpaceHideV[2] < 0.0f) {
                    sWorkSpaceHideV[2] = 0.0f;
                }
                if (sWorkSpaceHideV[3] < 0.0f) {
                    sWorkSpaceHideV[3] = 0.0f;
                }

                x1 = sWorkSpaceV[0];
                y1 = sWorkSpaceV[1];

                TextureBuffer *buf = TextureBuffer::alloc(sWorkSpaceHideV[2], sWorkSpaceHideV[3], GX_TF_RGBA8);
                spBufferSet[type].buf = buf;
                if (spBufferSet[type].buf != nullptr) {
                    spBufferSet[type].buf->setPixModeSync(false);
                }
                doCapture = isEnableDirtyBufferMode();
                break;
            }

            case BUFFER_2:
                spBufferSet[type].buf = TextureBuffer::alloc(efb.vp.x2, efb.vp.y2, GX_TF_RGBA8);
                spBufferSet[type].buf->setPixModeSync(false);
                break;
            case BUFFER_3:
                spBufferSet[type].buf = TextureBuffer::alloc(
                    efb.vp.x2 / 2.0f, efb.vp.y2 / 2.0f, GetUseTfRgb565() ? GX_TF_RGB565 : GX_TF_RGBA8
                );
                doUpscale = true;
                break;
            default: break;
        }

        if (spBufferSet[type].buf != nullptr) {
            spBufferSet[type].userData = userData;
            if (noTransparencyMaybe) {
                spBufferSet[type].buf->onCapFlag(0x2);
                spBufferSet[type].buf->setClearColor((GXColor){0, 0, 0, 0});
            }
            if (x1 < 0.0f) {
                x1 = 0.0f;
            }
            if (y1 < 0.0f) {
                y1 = 0.0f;
            }
            if (doCapture) {
                spBufferSet[type].buf->capture(x1, y1, doUpscale, -1);
            }
        }
    }

    return spBufferSet[type].buf;
}

bool StateEfb::releaseEfb(BufferType type, u32 userData) {
    if (spBufferSet[type].buf != nullptr && spBufferSet[type].userData == userData) {
        spBufferSet[type].buf->free();
        spBufferSet[type].buf = nullptr;
        spBufferSet[type].userData = 0;
        spBufferSet[type].field_0x08 = 0;
        return true;
    } else {
        return false;
    }
}

void StateEfb::fn_804B4270(BufferType type, u32 userData) {
    if (type == BUFFER_0) {
        if (sWorkBuffer == 1) {
            StateEfb::releaseEfb(BUFFER_0, spBufferSet[0].userData);
        }
        captureEfb(BUFFER_1, false, userData);
        sWorkBuffer = 0;
    } else if (type == BUFFER_1 && sWorkBuffer != 0) {
        captureEfb(BUFFER_0, false, userData);
        sWorkBuffer = 1;
    }
}

void StateEfb::popWorkBuffer(bool b, u32 userData) {
    if (sWorkBuffer != -1) {
        BufferType i = static_cast<BufferType>(-1);
        if (sWorkBuffer == 0) {
            i = BUFFER_1;
        } else if (sWorkBuffer == 1) {
            i = BUFFER_0;
        }

        if (spBufferSet[i].userData == userData) {
            if (!b) {
                StateGX::ScopedColor colorGuard(true);
                StateGX::ScopedAlpha alphaGuard(true);
                StateGX::ScopedDither ditherGuard(false);

                const Screen &parentScreen = GlobalDrawState::getScreen();
                Screen s(parentScreen);

                s.SetFlag(Screen::FLAG_0x40);
                s.SetCanvasMode(Screen::CANVASMODE_1);
                s.SetProjectionType(Screen::PROJ_ORTHO);
                s.SetNearFar(0.0f, 1.0f);
                s.SetScale(nw4r::math::VEC3(1.0f, 1.0f, 1.0f));
                s.SetOffset(nw4r::math::VEC2(0.0f, 0.0f));

                s.SetProjectionGX();
                StateGX::GXSetViewport_(
                    sWorkSpaceHideV[0], sWorkSpaceHideV[1], sWorkSpaceHideV[2], sWorkSpaceHideV[3], 0.0f, 1.0f
                );
                StateGX::GXSetScissor_(sWorkSpaceHideV[0], sWorkSpaceHideV[1], sWorkSpaceHideV[2], sWorkSpaceHideV[3]);
                StateGX::GXSetScissorBoxOffset_(0, 0);

                nw4r::math::MTX34 mtx;
                PSMTXScale(mtx, parentScreen.GetSize().x, parentScreen.GetSize().y, 1.0f);

                DrawGX::BeginDrawScreen(true, isEnableDirtyBufferMode(), false);
                DrawGX::SetBlendMode(DrawGX::BLEND_14);
                if (isEnableDirtyBufferMode()) {
                    TextureBuffer *buf = spBufferSet[i].buf;
                    buf->setFilt(GX_NEAR, GX_NEAR);
                    buf->load(DrawGX::GetTexMapDefault());
                    DrawGX::DrawDL(DrawGX::DL_16, mtx, DrawGX::WHITE);
                }
            }
            releaseEfb((i), spBufferSet[i].userData);
            sWorkBuffer = -1;
            sPushCount += 1;
        }
    }
}

f32 *StateEfb::fn_804B4550() {
    const Screen::DataEfb &efb = GlobalDrawState::getScreen().GetDataEfb();
    sShiftViewPort[0] = sWorkSpaceV[0];
    sShiftViewPort[1] = sWorkSpaceV[1];
    sShiftViewPort[2] = efb.vp.x2;
    sShiftViewPort[3] = efb.vp.y2;
    sShiftViewPort[4] = efb.vp.z1;
    sShiftViewPort[5] = efb.vp.z2;
    StateGX::GXSetViewport_(
        sShiftViewPort[0], sShiftViewPort[1], sShiftViewPort[2], sShiftViewPort[3], sShiftViewPort[4], sShiftViewPort[5]
    );
    u32 f[4];
    StateGX::GetScissorSafeParam(sShiftViewPort, f);
    StateGX::GXSetScissor_(f[0], f[1], f[2], f[3]);
    StateGX::GXSetScissorBoxOffset_(0, 0);
    return sShiftViewPort;
}

bool StateEfb::isEnableDirtyBufferMode() {
    return ((sFlag >> 2) & 1) != 0;
}

bool StateEfb::GetUseTfRgb565() {
    return (sFlag & 8) != 0;
}

} // namespace EGG
