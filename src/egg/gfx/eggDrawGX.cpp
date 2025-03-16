#include "egg/gfx/eggDrawGX.h"

#include "common.h"
#include "math.h"
#include "nw4r/math/math_triangular.h"
#include "nw4r/math/math_types.h"
#include "rvl/GX/GXDisplayList.h"
#include "rvl/GX/GXDraw.h"
#include "rvl/GX/GXGeometry.h"
#include "rvl/GX/GXLight.h"
#include "rvl/GX/GXTransform.h"
#include "rvl/GX/GXTypes.h"
#include "rvl/GX/GXVert.h"
#include "rvl/MTX/mtx.h"
#include "rvl/OS/OSCache.h"

namespace {

static void DrawQuadNormal(u8, u8, u8, u8, u8) {}
static void DrawQuadLineStripNormal(u8, u8, u8, u8, u8) {}
static void DrawCircleYPolygonFan(const nw4r::math::MTX34 &mtx, f32, u16) {}

} // namespace

namespace EGG {

u8 sDetailLevels[] = {10, 20};

void DrawGX::CreateDisplayList(EGG::Heap *pHeap) {
    u8 ALIGN_DECL(32) tmpDisplayList[16 * 1024];
    
    OSInitFastCast();

    for (int i = 0; i < DL_MAX; i++) {
        DCInvalidateRange(tmpDisplayList, sizeof(tmpDisplayList));
        GXBeginDisplayList(tmpDisplayList, sizeof(tmpDisplayList));
        switch (static_cast<DL>(i)) {
            case DL_0:
                GXSetChanMatColor(GX_COLOR0A0, RED);
                GXBegin(GX_LINES, GX_VTXFMT0, 2);
                GXPosition3f32(0.0f, 0.0f, 0.0f);
                GXPosition3f32(1.0f, 0.0f, 0.0f);
                GXSetChanMatColor(GX_COLOR0A0, GREEN);
                GXBegin(GX_LINES, GX_VTXFMT0, 2);
                GXPosition3f32(0.0f, 0.0f, 0.0f);
                GXPosition3f32(0.0f, 1.0f, 0.0f);
                GXSetChanMatColor(GX_COLOR0A0, BLUE);
                GXBegin(GX_LINES, GX_VTXFMT0, 2);
                GXPosition3f32(0.0f, 0.0f, 0.0f);
                GXPosition3f32(0.0f, 0.0f, 1.0f);
                break;
            case DL_1:
                GXBegin(GX_QUADS, GX_VTXFMT0, 6 * 4);
                DrawQuadNormal(1, 5, 4, 0, 0);
                DrawQuadNormal(3, 7, 6, 2, 1);
                DrawQuadNormal(5, 6, 7, 4, 2);
                DrawQuadNormal(3, 2, 1, 0, 3);
                DrawQuadNormal(0, 4, 7, 3, 4);
                DrawQuadNormal(2, 6, 5, 1, 5);
                break;
            case DL_2:
                DrawQuadLineStripNormal(1, 5, 4, 0, 0);
                DrawQuadLineStripNormal(3, 7, 6, 2, 1);
                DrawQuadLineStripNormal(5, 6, 7, 4, 2);
                DrawQuadLineStripNormal(3, 2, 1, 0, 3);
                break;
            case DL_3: GXDrawSphere(4, 8); break;
            case DL_4: GXDrawSphere(8, 16); break;
            case DL_5:
            case DL_6: {
                u16 numSegments = sDetailLevels[i - DL_5] + 1;
                f32 radPerSegment = 2.0f * M_PI / (numSegments - 1);
                GXBegin(GX_LINESTRIP, GX_VTXFMT0, numSegments);
                for (int i = 0; i < numSegments; i++) {
                    GXPosition3f32(
                        0.5f * nw4r::math::CosRad(radPerSegment * i), 0.5f * nw4r::math::SinRad(radPerSegment * i), 0.0f
                    );
                }
                break;
            }
            case DL_9:
            case DL_10: {
                u16 numSegments = sDetailLevels[i - DL_9] + 1;
                f32 radPerSegment = 2.0f * M_PI / (numSegments - 1);
                GXBegin(GX_TRIANGLESTRIP, GX_VTXFMT0, numSegments * 2);
                for (int i = 0; i < numSegments; i++) {
                    f32 x = 0.5f * nw4r::math::SinRad(radPerSegment * i);
                    f32 z = 0.5f * nw4r::math::CosRad(radPerSegment * i);
                    GXPosition3f32(x, -0.5f, z);
                    GXPosition3f32(2.0f * x, 0.0f, 2.0f * z);
                    GXPosition3f32(x, 0.5f, z);
                    GXPosition3f32(2.0f * x, 0.0f, 2.0f * z);
                }
                nw4r::math::MTX34 mtx;
                PSMTXTrans(mtx, 0.0f, 0.5f, 0.0f);
                DrawCircleYPolygonFan(mtx, 0.0f, sDetailLevels[i - DL_9]);

                PSMTXIdentity(mtx);
                PSMTXRotRad(mtx, M_PI, 0x7A);
                PSMTXTransApply(mtx, mtx, 0.0f, -0.5f, 0.0f);
                DrawCircleYPolygonFan(mtx, 0.0f, sDetailLevels[i - DL_9]);
                break;
            }
            case DL_11:
            case DL_12: {
                nw4r::math::MTX34 mtx;
                PSMTXIdentity(mtx);
                DrawCircleYPolygonFan(mtx, 1.0f, sDetailLevels[i - DL_11]);
                PSMTXRotRad(mtx, M_PI, 0x7A);
                DrawCircleYPolygonFan(mtx, 0.0f, sDetailLevels[i - DL_11]);
                break;
            }
            case DL_7:
            case DL_8: {
                nw4r::math::MTX34 mtx;
                PSMTXIdentity(mtx);
                PSMTXRotRad(mtx, M_PI / 2.0f, 0x78);
                DrawCircleYPolygonFan(mtx, 0.0f, sDetailLevels[i - DL_7]);
                break;
            }

            case DL_13:
            case DL_14: {
                GXBegin(GX_QUADS, GX_VTXFMT0, 4);
                GXPosition2u8(0, 0);
                if (i == DL_13) {
                    GXPosition1x8(0);
                }
                GXPosition2u8(1, 0);
                if (i == DL_13) {
                    GXPosition1x8(1);
                }
                GXPosition2u8(2, 0);
                if (i == DL_13) {
                    GXPosition1x8(2);
                }
                GXPosition2u8(3, 0);
                if (i == DL_13) {
                    GXPosition1x8(3);
                }
                break;
            }
            case DL_15:
                GXBegin(GX_LINESTRIP, GX_VTXFMT0, 5);
                GXPosition3f32(-0.5f, 0.5f, 0.0f);
                GXPosition3f32(0.5f, 0.5f, 0.0f);
                GXPosition3f32(0.5f, -0.5f, 0.0f);
                GXPosition3f32(-0.5f, -0.5f, 0.0f);
                GXPosition3f32(-0.5f, 0.5f, 0.0f);
                break;
            case DL_16:
            case DL_17:
                GXBegin(GX_QUADS, GX_VTXFMT0, 4);
                GXPosition1x8(0);
                if (i == DL_16) {
                    GXPosition1x8(0);
                }
                GXPosition1x8(1);
                if (i == DL_16) {
                    GXPosition1x8(1);
                }
                GXPosition1x8(2);
                if (i == DL_16) {
                    GXPosition1x8(2);
                }
                GXPosition1x8(3);
                if (i == DL_16) {
                    GXPosition1x8(3);
                }
                break;
            default: break;
        }

        u32 size = GXEndDisplayList();
        s_DL[i].mLen = size;
        u8 *data = new (pHeap, 0x20) u8[size];
        s_DL[i].mpList = data;
        for (int j = 0; j < size; j++) {
            data[j] = tmpDisplayList[j];
        }
        DCFlushRange(data, size);
    }
    s_flag |= 1;
}

void DrawGX::DrawDL(enum DL dl, const nw4r::math::MTX34 &mtx, GXColor clr) {
    GXLoadPosMtxImm(mtx, 0);
    GXLoadNrmMtxImm(mtx, 0);
    GXSetChanMatColor(GX_COLOR0A0, clr);
    GXCallDisplayList(s_DL[dl].mpList, s_DL[dl].mLen);
}

} // namespace EGG
