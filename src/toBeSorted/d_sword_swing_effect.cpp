#include "toBeSorted/d_sword_swing_effect.h"

#include "d/d_stage_mgr.h"
#include "egg/gfx/eggDrawGX.h"
#include "m/m3d/m3d.h"
#include "m/m3d/m_proc.h"
#include "m/m3d/m_scnleaf.h"
#include "nw4r/g3d/g3d_scnobj.h"
#include "nw4r/math/math_types.h"

void dSwordSwingEffectProcBase_c::remove() {
    if (mpEntries != nullptr) {
        delete[] mpEntries;
        mpEntries = nullptr;
    }
    m3d::scnLeaf_c::remove();
}

void dSwordSwingEffectProcBase_c::drawXlu() {
    m3d::resetMaterial();
    GXSetNumChans(1);
    GXSetChanCtrl(GX_COLOR0A0, GX_FALSE, GX_SRC_REG, GX_SRC_VTX, GX_LIGHT_NULL, GX_DF_NONE, GX_AF_NONE);
    GXSetNumTexGens(0);
    GXSetNumTevStages(1);
    GXSetNumIndStages(0);
    GXSetTevSwapMode(GX_TEVSTAGE0, GX_TEV_SWAP0, GX_TEV_SWAP0);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD_NULL, GX_TEXMAP_NULL, GX_COLOR0A0);
    GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO, GX_CC_RASC);
    GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, 1, GX_TEVPREV);
    GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_CA_RASA);
    GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetZCompLoc(1);
    GXSetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA, GX_LO_OR);
    GXSetZMode(GX_TRUE, GX_LEQUAL, GX_FALSE);
    GXSetAlphaCompare(GX_GREATER, 0, GX_AOP_OR, GX_GREATER, 0);
    GXSetFog(GX_FOG_NONE, mColor(0), 0.0, 0.0, 0.0, 0.0);
    GXSetFogRangeAdj(0, 0, 0);
    GXSetCullMode(GX_CULL_NONE);
    nw4r::math::MTX34 mtx;
    nw4r::g3d::Camera cam = m3d::getCurrentCamera();
    cam.GetCameraMtx(&mtx);
    GXLoadPosMtxImm(mtx, 0);
    GXSetCurrentMtx(0);
    GXClearVtxDesc();
    GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
    GXSetVtxDesc(GX_VA_CLR0, GX_DIRECT);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_F32, 0);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_CLR0, GX_CLR_RGBA, GX_RGBA8, 0);
    GXBegin(GX_TRIANGLESTRIP, GX_VTXFMT0, mCount * 2);

    swing_entry *entry = &mpEntries[mHead];
    for (u16 i = 0; i < mCount; i++) {
        if (entry == &mpEntries[0]) {
            entry = &mpEntries[mNum];
        }
        entry--;

        GXPosition3f32(entry->mv1.x, entry->mv1.y, entry->mv1.z);
        GXColor4u8(entry->mc1.r, entry->mc1.g, entry->mc1.b, entry->mc1.a * entry->mLifetime);
        GXPosition3f32(entry->mv2.x, entry->mv2.y, entry->mv2.z);
        GXColor4u8(entry->mc2.r, entry->mc2.g, entry->mc2.b, entry->mc2.a * entry->mLifetime);
    }

    GXEnd();
}

bool dSwordSwingEffectProcBase_c::hasSwings() const {
    return mCount != 0;
}

int dSwordSwingEffectProcBase_c::entry() {
    if (hasSwings()) {
        return m3d::pushBack(mpScnLeaf);
    }
    return 0;
}

bool dSwordSwingEffectProcBase_c::create(s32 num, mAllocator_c *alloc) {
    if (!m3d::proc_c::create(alloc, nullptr)) {
        return false;
    }

    mpEntries = new (alloc->getHeap(), 4) swing_entry[num]();
    if (mpEntries == nullptr) {
        remove();
        return false;
    }

    mNum = num;
    setOption(nw4r::g3d::ScnObj::OPTION_DISABLE_DRAW_OPA, true);
    return true;
}

void dSwordSwingEffectProcBase_c::addSwing(
    const mVec3_c &pos1, mColor c1, const mVec3_c &pos2, mColor c2
) {
    mpEntries[mHead].set(pos1, c1, pos2, c2);
    mHead++;
    if (mHead >= mNum) {
        mHead = 0;
    }
    if (mCount < mNum) {
        mCount++;
    }
}

void dSwordSwingEffectProcBase_c::calc(f32 f) {
    s32 count = mCount;
    if (count == 0) {
        return;
    }

    swing_entry *entry = &mpEntries[mHead];
    for (u16 i = 0; i < mCount; i++) {
        if (entry == &mpEntries[0]) {
            entry = &mpEntries[mNum];
        }
        entry--;
        entry->mLifetime -= f;
        if (entry->mLifetime <= 0.0f) {
            entry->mLifetime = 0.0f;
            count--;
        }
    }
    if (mCount != 0 && count == 0) {
        mHead = 0;
    }
    mCount = count;
}

void dSwordSwingEffectProc_c::drawXlu() {
    if (mpImg == nullptr) {
        dSwordSwingEffectProcBase_c::drawXlu();
    } else {
        EGG::DrawGX::LoadTexture(mpImg, GX_TEXMAP0);
        GXSetNumChans(1);
        GXSetChanCtrl(GX_COLOR0A0, false, GX_SRC_REG, GX_SRC_VTX, GX_LIGHT_NULL, GX_DF_NONE, GX_AF_NONE);
        GXSetNumTexGens(1);
        GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, 0x3c, 0, 0x7d);
        GXSetNumTevStages(1);
        GXSetNumIndStages(0);
        GXSetTevSwapMode(GX_TEVSTAGE0, GX_TEV_SWAP0, GX_TEV_SWAP0);
        GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR0A0);
        GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_RASC, GX_CC_TEXC, GX_CC_ZERO);
        GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, 1, GX_TEVPREV);
        GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_RASA, GX_CA_TEXA, GX_CA_ZERO);
        GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, true, GX_TEVPREV);
        GXSetZCompLoc(0);
        GXSetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA, GX_LO_OR);
        GXSetZMode(true, GX_LEQUAL, false);
        GXSetAlphaCompare(GX_GREATER, 0, GX_AOP_OR, GX_GREATER, 0);
        GXSetFog(GX_FOG_NONE, mColor(0), 0.0, 0.0, 0.0, 0.0);
        GXSetFogRangeAdj(0, 0, 0);
        GXSetCullMode(GX_CULL_NONE);
        nw4r::math::MTX34 mtx;
        nw4r::g3d::Camera cam = m3d::getCurrentCamera();
        cam.GetCameraMtx(&mtx);
        GXLoadPosMtxImm(mtx, 0);
        GXSetCurrentMtx(0);
        GXClearVtxDesc();
        GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
        GXSetVtxDesc(GX_VA_CLR0, GX_DIRECT);
        GXSetVtxDesc(GX_VA_TEX0, GX_DIRECT);
        GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_F32, 0);
        GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_CLR0, GX_CLR_RGBA, GX_RGBA8, 0);
        GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_TEX_ST, GX_F32, 0);
        GXBegin(GX_TRIANGLESTRIP, GX_VTXFMT0, mCount * 2);

        u8 alpha = dStageMgr_c::GetInstance()->getGlobalAlpha();
        f32 fAlpha = alpha * (1.0f / 255.0f);

        s32 idx = mHead - mCount;
        if (idx < 0) {
            idx += mNum;
        }

        f32 texCoordOffset = mTexScale * idx;

        swing_entry *entry = &mpEntries[idx];
        swing_entry *last = &mpEntries[mNum];

        for (u16 i = 0; i < mCount; i++) {
            GXPosition3f32(entry->mv1.x, entry->mv1.y, entry->mv1.z);
            GXColor4u8(entry->mc1.r, entry->mc1.g, entry->mc1.b, entry->mc1.a * entry->mLifetime * fAlpha);
            GXTexCoord2f32(texCoordOffset, 0.0f);

            GXPosition3f32(entry->mv2.x, entry->mv2.y, entry->mv2.z);
            GXColor4u8(entry->mc2.r, entry->mc2.g, entry->mc2.b, entry->mc2.a * entry->mLifetime * fAlpha);
            GXTexCoord2f32(texCoordOffset, 1.0f);

            texCoordOffset += mTexScale;
            entry++;
            if (entry == last) {
                entry = &mpEntries[0];
            }
        }
    }
}

bool dSwordSwingEffectProc_c::create(s32 num, mAllocator_c *alloc, EGG::ResTIMG *pImg, f32 scale) {
    if (!dSwordSwingEffectProcBase_c::create(num, alloc)) {
        return false;
    }

    mpImg = pImg;
    mTexScale = scale;
    return true;
}

void dSwordSwingEffectProcBase_c::swing_entry::set(
    const mVec3_c &pos1, const mColor &c1, const mVec3_c &pos2, const mColor &c2
) {
    mLifetime = 1.0f;
    mv1 = pos1;
    mv2 = pos2;
    mc1 = c1;
    mc2 = c2;
}
