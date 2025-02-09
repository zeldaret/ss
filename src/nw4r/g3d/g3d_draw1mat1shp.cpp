#include <nw4r/g3d.h>

#include <nw4r/ut.h>

#include <rvl/BASE.h>
#include <rvl/GX.h>

namespace nw4r {
namespace g3d {

void Draw1Mat1ShpDirectly(
    ResMat mat, ResShp shp, const math::MTX34 *pViewPos, const math::MTX34 *pViewNrm, u32 ctrl, Draw1Mat1ShpSwap *pSwap,
    G3DState::IndMtxOp *pIndMtxOp
) {
    if (!(ctrl & DRAW1MAT1SHP_CTRL_NOPPCSYNC)) {
        ut::LC::QueueWait(0);
        PPCSync();
    }

    if (mat.IsValid()) {
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

    if (shp.IsValid() && shp.IsVisible()) {
        if (!(ctrl & DRAW1MAT1SHP_CTRL_NOSWAPSHP)) {
            G3DState::LoadResShpPrePrimitive(shp);

            if (pSwap != NULL) {
                if (pSwap->vtxPosTable != NULL) {
                    ResVtxPos(pSwap->vtxPosTable[shp.ref().idVtxPosition]).SetArray();
                }

                if (pSwap->vtxNrmTable != NULL) {
                    int nrmID = shp.ref().idVtxNormal;

                    if (nrmID >= 0) {
                        ResVtxNrm(pSwap->vtxNrmTable[nrmID]).SetArray();
                    }
                }

                if (pSwap->vtxClrTable != NULL) {
                    int clrID = shp.ref().idVtxColor[0];

                    if (clrID >= 0) {
                        ResVtxClr(pSwap->vtxClrTable[clrID]).SetArray(GX_VA_CLR0);
                    }

                    clrID = shp.ref().idVtxColor[1];

                    if (clrID >= 0) {
                        ResVtxClr(pSwap->vtxClrTable[clrID]).SetArray(GX_VA_CLR1);
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

} // namespace g3d
} // namespace nw4r
