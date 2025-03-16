#include "nw4r/g3d.h" // IWYU pragma: export

#include "nw4r/ut.h" // IWYU pragma: export

#include "rvl/BASE.h" // IWYU pragma: export
#include "rvl/GX.h"   // IWYU pragma: export

#include <cmath>

namespace nw4r {
namespace g3d {
namespace detail {

void LoadMaterial(ResMat mat, u32 ctrl, Draw1Mat1ShpSwap *pSwap, G3DState::IndMtxOp *pIndMtxOp, bool bIgnoreMaterial) {
    if (!bIgnoreMaterial) {
        ResMatMisc misc;

        if (pSwap == NULL || !pSwap->misc.IsValid()) {
            misc = mat.GetResMatMisc();
        } else {
            misc = pSwap->misc;
        }

        int fogID = misc.GetFogIdx();
        G3DState::LoadFog(fogID);
        G3DState::LoadResMatMisc(misc);

        if (pSwap == NULL || !pSwap->tlutObj.IsValid()) {
            G3DState::LoadResTlutObj(mat.GetResTlutObj());
        } else {
            G3DState::LoadResTlutObj(pSwap->tlutObj);
        }

        if (pSwap == NULL || !pSwap->texObj.IsValid()) {
            G3DState::LoadResTexObj(mat.GetResTexObj());
        } else {
            G3DState::LoadResTexObj(pSwap->texObj);
        }

        if (pSwap == NULL || !pSwap->genMode.IsValid()) {
            G3DState::LoadResGenMode(mat.GetResGenMode());
        } else {
            G3DState::LoadResGenMode(pSwap->genMode);
        }

        if (pSwap == NULL || !pSwap->tev.IsValid()) {
            G3DState::LoadResTev(mat.GetResTev());
        } else {
            G3DState::LoadResTev(pSwap->tev);
        }

        if (pSwap == NULL || !pSwap->pix.IsValid()) {
            G3DState::LoadResMatPix(mat.GetResMatPix());
        } else {
            G3DState::LoadResMatPix(pSwap->pix);
        }

        if (pSwap == NULL || !pSwap->tevColor.IsValid()) {
            G3DState::LoadResMatTevColor(mat.GetResMatTevColor());
        } else {
            G3DState::LoadResMatTevColor(pSwap->tevColor);
        }

        if (pSwap == NULL || !pSwap->indMtxAndScale.IsValid()) {
            if (pIndMtxOp != NULL) {
                G3DState::LoadResMatIndMtxAndScale(mat.GetResMatIndMtxAndScale(), *pIndMtxOp);
            } else {
                G3DState::LoadResMatIndMtxAndScale(mat.GetResMatIndMtxAndScale());
            }
        } else {
            if (pIndMtxOp != NULL) {
                G3DState::LoadResMatIndMtxAndScale(pSwap->indMtxAndScale, *pIndMtxOp);
            } else {
                G3DState::LoadResMatIndMtxAndScale(pSwap->indMtxAndScale);
            }
        }

        u32 diffColorMask, diffAlphaMask;
        u32 specColorMask, specAlphaMask;

        GXColor ambColor;
        AmbLightObj ambLight;

        G3DState::LoadLightSet(
            misc.GetLightSetIdx(), &diffColorMask, &diffAlphaMask, &specColorMask, &specAlphaMask, &ambLight
        );

        ambColor.r = ambLight.r;
        ambColor.g = ambLight.g;
        ambColor.b = ambLight.b;
        ambColor.a = ambLight.a;

        if (pSwap == NULL || !pSwap->chan.IsValid()) {
            G3DState::LoadResMatChan(
                mat.GetResMatChan(), diffColorMask, diffAlphaMask, specColorMask, specAlphaMask, ambColor,
                (ctrl & DRAW1MAT1SHP_CTRL_FORCE_LIGHTOFF) ? true : false
            );
        } else {
            G3DState::LoadResMatChan(
                pSwap->chan, diffColorMask, diffAlphaMask, specColorMask, specAlphaMask, ambColor,
                (ctrl & DRAW1MAT1SHP_CTRL_FORCE_LIGHTOFF) ? true : false
            );
        }

        if (pSwap == NULL || !pSwap->texCoordGen.IsValid()) {
            G3DState::LoadResMatTexCoordGen(mat.GetResMatTexCoordGen());
        } else {
            G3DState::LoadResMatTexCoordGen(pSwap->texCoordGen);
        }

        if (pSwap == NULL || !pSwap->texSrt.IsValid()) {
            G3DState::LoadResTexSrt(mat.GetResTexSrt());
        } else {
            G3DState::LoadResTexSrt(pSwap->texSrt);
        }
    } else if (pIndMtxOp != NULL) {
        if (pSwap == NULL || !pSwap->indMtxAndScale.IsValid()) {
            G3DState::IndTexMtxInfo info;
            (*pIndMtxOp)(&info);
            info.FifoSend();
        } else {
            G3DState::LoadResMatIndMtxAndScale(pSwap->indMtxAndScale, *pIndMtxOp);
        }
    }
}

} // namespace detail

void Draw1Mat1ShpDirectly(
    ResMat mat, ResShp shp, const math::MTX34 *pViewPos, const math::MTX34 *pViewNrm, u32 ctrl, Draw1Mat1ShpSwap *pSwap,
    G3DState::IndMtxOp *pIndMtxOp
) {
    if (!(ctrl & DRAW1MAT1SHP_CTRL_NOPPCSYNC)) {
        ut::LC::QueueWait(0);
        PPCSync();
    }

    bool bIgnoreMaterial = false;
    bool bFur = false;
    ResMatFur fur;
    if (mat.IsValid()) {
        if (ctrl & DRAW1MAT1SHP_CTRL_IGNORE_MAT) {
            bIgnoreMaterial = true;
        }
        fur = mat.GetResMatFur();
        if (fur.IsValid()) {
            bFur = true;
        }
    } else {
        bIgnoreMaterial = true;
    }

    detail::LoadMaterial(mat, ctrl, pSwap, pIndMtxOp, bIgnoreMaterial);

    if (shp.IsValid() && shp.IsVisible()) {
        if (bFur) {
            f32 rateBright, rateAlpha, rateSpec;
            f32 alphaCurve, specCurve;
            f32 rate, rateInv;

            GXColor rootColor, tipColor, kcolor2;

            u8 brightness;
            GXColorSrc matSrc[4];
            GXAttnFn attnFunc[4];
            GXColor matColor[2];

            bool bIndTexMtx[3];
            math::MTX34 itm[3];
            s8 scale_exp[3];

            const GXChannelID targetChan[4] = {GX_COLOR0, GX_ALPHA0, GX_COLOR1, GX_ALPHA1};

            alphaCurve = fur.ref().alphaCurve;
            specCurve = fur.ref().specCurve;

            const ResMatTevColor tevColor =
                (!pSwap || !pSwap->tevColor.IsValid()) ? mat.GetResMatTevColor() : pSwap->tevColor;

            tevColor.GXGetTevKColor(GX_KCOLOR0, &rootColor);
            tevColor.GXGetTevKColor(GX_KCOLOR1, &tipColor);
            tevColor.GXGetTevKColor(GX_KCOLOR2, &kcolor2);

            brightness = kcolor2.g;

            const ResMatChan chan = (!pSwap || !pSwap->chan.IsValid()) ? mat.GetResMatChan() : pSwap->chan;

            chan.GXGetChanMatColor(GX_COLOR0, &matColor[0]);
            chan.GXGetChanMatColor(GX_COLOR1, &matColor[1]);
            for (u8 chanIdx = 0; chanIdx < 4; chanIdx++) {
                chan.GXGetChanCtrl(targetChan[chanIdx], NULL, NULL, &matSrc[chanIdx], NULL, NULL, &attnFunc[chanIdx]);
            }

            ResMatMisc misc = (!pSwap || !pSwap->misc.IsValid()) ? mat.GetResMatMisc() : pSwap->misc;
            ResMatIndMtxAndScale indMtxAndScale =
                (!pSwap || !pSwap->indMtxAndScale.IsValid()) ? mat.GetResMatIndMtxAndScale() : pSwap->indMtxAndScale;

            for (u32 itmIdx = 0; itmIdx < 3; itmIdx++) {
                ResMatMiscData::IndirectMethod method;
                s8 ref_light;
                const GXIndTexMtxID id = static_cast<GXIndTexMtxID>(itmIdx + GX_ITM_0);
                misc.GetIndirectTexMtxCalcMethod(id, &method, &ref_light);
                if (method == ResMatMiscData::FUR) {
                    bIndTexMtx[itmIdx] = true;
                    indMtxAndScale.GXGetIndTexMtx(id, &itm[itmIdx], &scale_exp[itmIdx]);
                } else {
                    bIndTexMtx[itmIdx] = false;
                }
            }

            G3DState::LoadResShpPrePrimitive(shp);
            if (ctrl & DRAW1MAT1SHP_CTRL_CULL_FRONT) {
                fifo::GDSetCullMode(GX_CULL_FRONT);
                G3DState::Invalidate(G3DState::INVALIDATE_SHP);
            }

            ResVtxFurPos furPos = shp.GetResVtxFurPos();
            const u32 numLayer = furPos.ref().numLayer;
            for (u16 i = 0; i < numLayer; i++) {
                rate = fur.GetLyrRate(i);
                rateInv = 1.f - rate;
                rateAlpha = pow(rate, alphaCurve);
                rateSpec = pow(rate, specCurve);
                rateBright = pow(rate, 4.f);

                kcolor2.r = 255 * rate;
                kcolor2.g = brightness * rateBright;
                GXSetTevKColor(GX_KCOLOR2, kcolor2);

                for (u8 itmIdx = 0; itmIdx < 3; itmIdx++) {
                    if (bIndTexMtx[itmIdx]) {
                        f32 newItm[2][3] = {
                            {
                             itm[itmIdx]._00 * rate,
                             itm[itmIdx]._01 * rate,
                             itm[itmIdx]._02,
                             },
                            {
                             itm[itmIdx]._10 * rate,
                             itm[itmIdx]._11 * rate,
                             itm[itmIdx]._12,
                             }
                        };
                        GXSetIndTexMtx(static_cast<GXIndTexMtxID>(itmIdx + GX_ITM_0), newItm, scale_exp[itmIdx]);
                    }
                }

                GXColor diffColor = {
                    rootColor.r * rateInv + tipColor.r * rate, rootColor.g * rateInv + tipColor.g * rate,
                    rootColor.b * rateInv + tipColor.b * rate, rootColor.a * (1.f - rateAlpha) + tipColor.a * rateAlpha
                };

                for (u8 colorIdx = 0; colorIdx < 2; colorIdx++) {
                    u8 chanIdx = colorIdx * 2;
                    GXColor newMatColor;

                    if (matSrc[chanIdx] == GX_SRC_REG || matSrc[chanIdx + 1] == GX_SRC_REG) {
                        if (attnFunc[chanIdx] == GX_AF_SPEC) {
                            newMatColor.r = matColor[colorIdx].r * rateSpec;
                            newMatColor.g = matColor[colorIdx].g * rateSpec;
                            newMatColor.b = matColor[colorIdx].b * rateSpec;
                        } else {
                            newMatColor.r = diffColor.r;
                            newMatColor.g = diffColor.g;
                            newMatColor.b = diffColor.b;
                        }
                        newMatColor.a = attnFunc[chanIdx + 1] == GX_AF_SPEC ? matColor[colorIdx].a : diffColor.a;

                        fifo::GDSetChanMatColor(targetChan[chanIdx], newMatColor);
                    }
                }
                furPos.SetArray(i);
                G3DState::LoadResShpPrimitive(shp, pViewPos, pViewNrm);
            }

        } else {
            if (!(ctrl & DRAW1MAT1SHP_CTRL_NOSWAPSHP)) {
                G3DState::LoadResShpPrePrimitive(shp);

                if (pSwap != NULL) {
                    if (pSwap->vtxPosTable != NULL) {
                        ResVtxPos(pSwap->vtxPosTable[shp.ref().idVtxPosition]).SetArray();
                    }

                    if (pSwap->vtxNrmTable != NULL) {
                        int idVtxNormal = shp.ref().idVtxNormal;

                        if (idVtxNormal >= 0) {
                            ResVtxNrm(pSwap->vtxNrmTable[idVtxNormal]).SetArray();
                        }
                    }

                    if (pSwap->vtxClrTable != NULL) {
                        int idVtxColor = shp.ref().idVtxColor[0];

                        if (idVtxColor >= 0) {
                            ResVtxClr(pSwap->vtxClrTable[idVtxColor]).SetArray(GX_VA_CLR0);
                        }

                        idVtxColor = shp.ref().idVtxColor[1];

                        if (idVtxColor >= 0) {
                            ResVtxClr(pSwap->vtxClrTable[idVtxColor]).SetArray(GX_VA_CLR1);
                        }
                    }
                }
            }
            if (ctrl & DRAW1MAT1SHP_CTRL_CULL_FRONT) {
                fifo::GDSetCullMode(GX_CULL_FRONT);
                G3DState::Invalidate(G3DState::INVALIDATE_SHP);
            }

            G3DState::LoadResShpPrimitive(shp, pViewPos, pViewNrm);
        }
    }
}

} // namespace g3d
} // namespace nw4r
