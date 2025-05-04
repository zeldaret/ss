
#include "toBeSorted/d_unk_mdl_stuff_2.h"

#include "common.h"
#include "d/col/bg/d_bg_s.h"
#include "egg/gfx/eggLightManager.h"
#include "m/m3d/m3d.h"
#include "m/m3d/m_proc.h"
#include "m/m_mtx.h"
#include "nw4r/g3d/g3d_draw1mat1shp.h"
#include "nw4r/g3d/g3d_scnobj.h"
#include "nw4r/g3d/g3d_state.h"
#include "nw4r/g3d/res/g3d_resmat.h"
#include "nw4r/g3d/res/g3d_resshp.h"
#include "nw4r/math/math_types.h"
#include "rvl/GX/GXLight.h"
#include "rvl/GX/GXTev.h"
#include "rvl/GX/GXTypes.h"
#include "rvl/MTX/mtx.h"
#include "toBeSorted/blur_and_palette_manager.h"

bool dShpProcBase_c::init(nw4r::g3d::ResMat mat, nw4r::g3d::ResShp shp, s32 count, bool xlu, u32 *pSize) {
    mCount = count;
    mpFlags = new bool[count];
    if (mpFlags == nullptr) {
        return false;
    }

    if (pSize != nullptr) {
        *pSize += count * sizeof(bool);
    }
    for (s32 i = 0; i < mCount; i++) {
        mpFlags[i] = 0;
    }
    mMat = mat;
    mShp = shp;
    if (xlu) {
        setOption(nw4r::g3d::ScnObj::OPTION_DISABLE_DRAW_OPA, true);
    } else {
        setOption(nw4r::g3d::ScnObj::OPTION_DISABLE_DRAW_XLU, true);
    }
    return true;
}

void dShpProcBase_c::setupLight() {
    u32 pDiffColorMask;
    u32 pDiffAlphaMask;
    u32 pSpecColorMask;
    u32 pSpecAlphaMask;
    GXColor pColor;
    m3d::getLightMgr(0)->LoadLightSet(
        mLightSetId, &pDiffColorMask, &pDiffAlphaMask, &pSpecColorMask, &pSpecAlphaMask, &pColor
    );
    GXSetChanCtrl(GX_COLOR0, GX_TRUE, GX_SRC_REG, GX_SRC_REG, (GXLightID)pDiffColorMask, GX_DF_CLAMP, GX_AF_SPOT);
    GXSetChanCtrl(GX_ALPHA0, GX_FALSE, GX_SRC_REG, GX_SRC_REG, GX_LIGHT_NULL, GX_DF_NONE, GX_AF_NONE);
    GXSetChanAmbColor(GX_COLOR0, pColor);
}

void dShpProcBase_c::draw(mVec3_c *pos) {
    nw4r::g3d::Draw1Mat1ShpDirectly(
        mMat, nw4r::g3d::ResShp(nullptr), mMtx_c::Identity, mMtx_c::Identity, 0, nullptr, nullptr
    );
    setupLight();
    if (!field_0x2C) {
        BlurAndPaletteManager &mgr = BlurAndPaletteManager::GetInstance();
        u32 code = 0xFF;
        if (pos != nullptr) {
            pos->y += 100.0f;
            code = dBgS::GetInstance()->GetLightingCode(pos);
        }
        if (code != 0) {
            GXSetTevColor(GX_TEVREG0, BlurAndPaletteManager::getLightColor1());
            GXSetTevKColor(GX_KCOLOR0, BlurAndPaletteManager::getLightColor2());
        } else {
            GXSetTevColor(GX_TEVREG0, mgr.GetCurrentSph().white2_ffffff);
            GXSetTevKColor(GX_KCOLOR0, mgr.GetCurrentSph().grey2_aaaaaa);
        }
    }
}

void dShpProcBase_c::setResMat(nw4r::g3d::ResMat mat) {
    mMat = mat;
}

void dShpProcBase_c::setAllFlags() {
    for (s32 idx = 0; idx < mCount; idx++) {
        mpFlags[idx] = 1;
    }
}

void dShpProcBase_c::doEntry(s32 lightSetId) {
    mLightSetId = lightSetId;
    entry();
}


bool dShpProc1_c::create(nw4r::g3d::ResMat mat, nw4r::g3d::ResShp shp, s32 count, mHeapAllocator_c *alloc, bool xlu, u32 *pSize) {
    if (!m3d::proc_c::create(alloc, pSize)) {
        return false;
    }
    if (!init(mat, shp, count, xlu, pSize)) {
        return false;
    }
    mpTransforms = new mMtx_c[count];
    if (mpTransforms == nullptr) {
        return false;
    }
    if (pSize != nullptr) {
        *pSize += count * sizeof(mMtx_c);
    }
    for (s32 idx = 0; idx < count; idx++) {
        MTXIdentity(mpTransforms[idx]);
    }
    return true;
}

void dShpProc1_c::draw() {
    mMtx_c viewMtx;
    getViewMtx(viewMtx);
    s32 ctrl = 0;
    mMtx_c *pTransforms = mpTransforms;
    bool *pFlags = mpFlags;
    dShpProcBase_c::draw(nullptr);
    for (s32 idx = 0; idx < mCount;) {
        if (*pFlags == false) {
            mMtx_c mtx;
            mtx.copyFrom(*pTransforms);
            MTXConcat(viewMtx, mtx, mtx);
            mMtx_c mtx2;
            mtx2.copyFrom(mtx);
            mtx2.fn_802F1C40(0, 1);
            nw4r::math::MTX33 mtx3;
            for (int a = 0; a < 3; a++) {
                for (int b = 0; b < 3; b++) {
                    mtx3.m[a][b] = mtx2.m[a][b];
                }
            }
            nw4r::g3d::G3DState::SetViewPosNrmMtxArray(mtx, &mtx3, nullptr);
            nw4r::g3d::Draw1Mat1ShpDirectly(nw4r::g3d::ResMat(nullptr), mShp, mtx, mtx2, ctrl, nullptr, nullptr);
            if (ctrl == 0) {
                ctrl = 2;
            }
        }
        idx++;
        pTransforms++;
        pFlags++;
    }
}
