#include <m/m3d/m_shadow.h>
#include <nw4r/g3d/g3d_calcview.h>
#include <nw4r/g3d/g3d_draw.h>
#include <nw4r/g3d/g3d_draw1mat1shp.h>
#include <nw4r/g3d/g3d_resmat.h>
#include <nw4r/g3d/g3d_resmdl.h>
#include <nw4r/g3d/g3d_resshp.h>
#include <nw4r/g3d/g3d_scnmdl.h>
#include <nw4r/g3d/g3d_scnmdlsmpl.h>
#include <nw4r/g3d/g3d_state.h>

// All of this is completely made up, as we don't have symbols for this TU
// (contrary to the rest of m3d and most of nw4r::g3d)

#define FRM_HEAP_STATE 0x74616730

namespace m3d {

mShadow_c *mShadow_c::sInstance;

GXTexObj *mShadow_c::sTexObj;
nw4r::g3d::ResShp mShadow_c::sResShp;

mShadow_c::~mShadow_c() {
    remove();
}

void mShadow_c::drawOpa() {
    if (field_0x66) {
        beforeDraw();
        mShadowChild_c *child = static_cast<mShadowChild_c *>(nw4r::ut::List_GetFirst(&mList));
        while (child != nullptr) {
            child->draw();
            child = static_cast<mShadowChild_c *>(nw4r::ut::List_GetNext(&mList, child));
        }
        afterDraw();
    }
}

void mShadow_c::create(int count, int unk1, int unk2, u16 texBufferSize, u32 drawOpaPriority, nw4r::g3d::ResMdl mdl,
        u32 heapSize) {
    // Regswaps
    mAllocator.attach(mpHeap, 0x20);
    proc_c::create(&mAllocator, nullptr);
    u32 bufSize = GXGetTexBufferSize(texBufferSize, texBufferSize, 4, false, 0);
    mTexBufferSize = bufSize;
    u32 b = (unk2 + 2) / 3;
    mpBuf = mpHeap->alloc(bufSize * b, 0x20);

    EGG::FrmHeap **heap = mpFrmHeaps;
    for (int i = 0; i < 2; i++) {
        heap[i] = mHeap::createFrmHeap(heapSize, mpHeap, "ShadowTmp", 0x4, 0);
        heap[i]->recordState(FRM_HEAP_STATE);
    }
    swapHeaps();
    mShadowChild_c *childs = new (mpHeap, 0x04) mShadowChild_c[count]();
    mpChilds = childs;
    for (int i = 0; i < count; i++) {
        childs[i].create(unk1, mpHeap);
    }
    setPriorityDraw(drawOpaPriority, 0);
    setOption(/* DISABLE_DRAW_XLU */ 0x07, 1);
    field_0x61 = count;
    field_0x62 = 0;
    field_0x63 = bufSize * 3;
    field_0x64 = 0;
    field_0x65 = texBufferSize;
    nw4r::g3d::ResMat mat = mdl.GetResMat(0);
    sResShp = mdl.GetResShp(0);
    sTexObj = mat.GetResTexObj().GetTexObj(0);
}

void mShadow_c::remove() {
    if (mpHeap != nullptr) {
        if (mpChilds != nullptr) {
            delete[] mpChilds;
            mpChilds = nullptr;
        }

        EGG::FrmHeap **heap = mpFrmHeaps;
        for (int i = 0; i < 2; i++) {
            if (heap[i] != nullptr) {
                mHeap::destroyFrmHeap(heap[i]);
                heap[i] = nullptr;
            }
        }

        if (mpBuf != nullptr) {
            mpHeap->free(mpBuf);
            mpBuf = nullptr;
        }

        scnLeaf_c::remove();
        mpHeap = nullptr;
    }
}

void mShadow_c::reset() {
    mShadowChild_c *iter = static_cast<mShadowChild_c *>(nw4r::ut::List_GetFirst(&mList));
    while (iter != nullptr) {
        iter->mNumLeaves = 0;
        mShadowCircle_c *circle = iter->mpCircle;
        iter->mpCircle = nullptr;
        circle->mpChild = nullptr;
        iter = static_cast<mShadowChild_c *>(nw4r::ut::List_GetNext(&mList, iter));
    }
    field_0x62 = 0;
    field_0x64 = 0;
    nw4r::ut::List_Init(&mList, 0);
    swapHeaps();
}

bool mShadow_c::addCircle(mShadowCircle_c *circle, u32 priority, u32 unk) {
    if (circle->mpChild != nullptr) {
        // Already linked, OK
        return true;
    }

    mShadowChild_c *childPtr;
    if (field_0x62 < field_0x61 && (unk == 0 || field_0x64 < field_0x63)) {
        // There are free entries in our buffer, so just add a new child
        childPtr = &mpChilds[field_0x62];
        if (field_0x62++ == 0) {
            entry();
        }

        if (unk != 0) {
            field_0x64++;
        }
    } else {
        // There are no free entries in our buffer, so replace an existing entry?
        childPtr = static_cast<mShadowChild_c *>(nw4r::ut::List_GetLast(&mList));
        if (priority >= childPtr->mPriorityMaybe) {
            return false;
        }

        if (unk != 0) {
            if (field_0x64 >= field_0x63) {
                while (childPtr->mNumLeaves == 0) {
                    childPtr = static_cast<mShadowChild_c *>(nw4r::ut::List_GetPrev(&mList, childPtr));
                    if (priority >= childPtr->mPriorityMaybe) {
                        return false;
                    }
                }
            } else {
                field_0x64++;
            }
        }
        childPtr->mNumLeaves = 0;
        childPtr->mpCircle->mpChild = nullptr;
        nw4r::ut::List_Remove(&mList, childPtr);
    }

    circle->mpChild = childPtr;
    childPtr->mpCircle = circle;
    childPtr->mPriorityMaybe = priority;
    mShadowChild_c *iter = static_cast<mShadowChild_c *>(nw4r::ut::List_GetFirst(&mList));
    if (iter == nullptr) {
        nw4r::ut::List_Append(&mList, childPtr);
        return true;
    } else {
        do {
            if (childPtr->mPriorityMaybe < iter->mPriorityMaybe) {
                nw4r::ut::List_Insert(&mList, iter, childPtr);
                return true;
            }
            iter = static_cast<mShadowChild_c *>(nw4r::ut::List_GetNext(&mList, iter));
        } while (iter != nullptr);
        nw4r::ut::List_Append(&mList, childPtr);
    }

    return true;
}

bool mShadow_c::drawMdl(mShadowCircle_c *circle, u32 priority, scnLeaf_c &mdl, const mQuat_c &quat, mVec3_c &pos,
        mColor color, u32 param9, f32 dist) {
    if (!addCircle(circle, priority, 1)) {
        return false;
    }

    mShadowChild_c *child = circle->mpChild;
    child->set(pos, dist, color);
    child->set0x154(param9);
    return child->addMdl(mdl, quat);
}

bool mShadow_c::addMdlToCircle(mShadowCircle_c *circle, scnLeaf_c &mdl, const mQuat_c &quat) {
    if (circle->mpChild == nullptr) {
        return false;
    }
    return circle->mpChild->addMdl(mdl, quat);
}

void mShadow_c::removeCircle(mShadowCircle_c *circle) {
    mShadowChild_c *child = circle->mpChild;
    if (child == nullptr) {
        return;
    }
    circle->mpChild = nullptr;
    child->mpCircle = nullptr;
    nw4r::ut::List_Remove(&mList, child);
}

static f32 sTexMtx[2][4] = {{1.0f, 0.0f, 0.0f, 0.0f}, {0.0f, 1.0f, 0.0f, 0.0f}};

static u32 STEP = 0x2492;

static GXColor sColors[] = {
        {0xFF, 0x00, 0x00, 0x00},
        {0x00, 0xFF, 0x00, 0x00},
        {0x00, 0x00, 0xFF, 0x00},
        {0x00, 0x00, 0x00, 0x00},
};

bool mShadow_c::drawTexObj(mShadowCircle_c *circle, u32 priority, const GXTexObj *texObj, const mMtx_c &mtx,
        const mQuat_c &quat, mVec3_c &pos, mColor color, u32 param9, f32 dist) {
    if (!addCircle(circle, priority, 0)) {
        return false;
    }

    mShadowChild_c *child = circle->mpChild;
    child->set(pos, dist, color);
    child->set0x154(param9);
    return child->setGeom(texObj, mtx, quat);
}

static void drawSub2(void *data, u8 i) {
    // TODO this function has one regswap
    int id;
    u16 wid = i * 2;
    GXSetTexCopySrc(0, 0, wid, wid);
    GXSetTexCopyDst(i, i, GX_TF_RGB565, 1);
    GXCopyTex(data, false);
    GXPixModeSync();
    GXInvalidateTexAll();
    GXTexObj obj;
    GXInitTexObj(&obj, data, i, i, GX_TF_RGB565, GX_CLAMP, GX_CLAMP, 0);
    GXInitTexObjLOD(&obj, GX_LINEAR, GX_LINEAR, 0.0f, 0.0f, 0.0f, 0, 0, GX_ANISO_1);
    GXLoadTexObj(&obj, GX_TEXMAP0);
    GXClearVtxDesc();
    GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
    GXSetVtxDesc(GX_VA_TEX0, GX_DIRECT);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_CLR_RGB, GX_RGBX8, 0);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_CLR_RGBA, GX_RGB565, 0);
    GXSetNumChans(0);
    GXSetNumTexGens(8);
    GXSetNumTevStages(8);
    GXSetTevColor(GX_TEVREG1, nw4r::ut::Color(0, 0, 0, 0x20));
    s16 ang = 0;
    id = GX_TEXCOORD0;
    u32 idx = 0x1E;
    for (; id < GX_MAX_TEXCOORD; id++) {
        GXSetTexCoordGen2((GXTexCoordID)id, GX_TG_MTX2x4, GX_TG_TEX0, idx, false, 0x7D);
        sTexMtx[0][3] = 0.01f * nw4r::math::CosIdx(ang);
        sTexMtx[1][3] = 0.01f * nw4r::math::SinIdx(ang);
        GXLoadTexMtxImm(sTexMtx, idx, GX_MTX_2x4);
        idx += 3;
        ang += STEP;
    }
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR_NULL);
    GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_TEXC, GX_CC_A0, GX_CC_ZERO);

    for (int id = GX_TEVSTAGE1; id < GX_TEVSTAGE8; id++) {
        GXSetTevSwapMode((GXTevStageID)id, GX_TEV_SWAP0, GX_TEV_SWAP0);
        GXSetTevOrder((GXTevStageID)id, (GXTexCoordID)id, GX_TEXMAP0, GX_COLOR_NULL);
        GXSetTevColorIn((GXTevStageID)id, GX_CC_ZERO, GX_CC_TEXC, GX_CC_A1, GX_CC_CPREV);
        GXSetTevColorOp((GXTevStageID)id, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, 1, GX_TEVPREV);
        GXSetTevAlphaIn((GXTevStageID)id, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO);
        GXSetTevAlphaOp((GXTevStageID)id, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, 1, GX_TEVPREV);
    }
    GXSetBlendMode(GX_BM_NONE, GX_BL_ONE, GX_BL_ONE, GX_LO_SET);
    GXLoadPosMtxImm(mMtx_c::Identity.m, 0);
    GXSetCurrentMtx(0);
    GXBegin(GX_QUADS, GX_VTXFMT0, 4);
    GXPosition2u16(0, 0);
    GXPosition2u8(0, 0);
    GXPosition2u16(wid, 0);
    GXPosition2u8(1, 0);
    GXPosition2u16(wid, wid);
    GXPosition2u8(1, 1);
    GXPosition2u16(0, wid);
    GXPosition2u8(0, 1);
    // GXEnd();
}

static void drawSub1(void *data, u8 i) {
    Mtx44 mtx;
    u16 wid = i * 2;
    f32 wid2 = i * 2;
    C_MTXOrtho(mtx, 0.0f, wid2, 0.0f, wid2, 0.0f, 1.0f);
    GXSetProjection(mtx, GX_ORTHOGRAPHIC);
    drawSub2(data, i);
    GXSetNumChans(1);
    GXSetNumTexGens(0);
    GXSetNumTevStages(1);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD_NULL, GX_TEXMAP_NULL, GX_COLOR0A0);
    GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO);
    GXClearVtxDesc();
    GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
    GXBegin(GX_LINESTRIP, GX_VTXFMT0, 5);

    GXPosition2u16(0, 0);
    GXPosition2u16(wid, 0);
    GXPosition2u16(wid, wid);
    GXPosition2u16(0, wid);
    GXPosition2u16(0, 0);
    // GXEnd();

    GXSetZMode(true, GX_ALWAYS, true);
    GXSetTexCopySrc(0, 0, wid, wid);
    GXSetTexCopyDst(i, i, GX_TF_RGB565, 1);
    GXCopyTex(data, true);
    GXPixModeSync();
    GXInvalidateTexAll();
}

void mShadow_c::drawAllShadows() {
    mShadowChild_c *iter = static_cast<mShadowChild_c *>(nw4r::ut::List_GetFirst(&mList));
    if (iter != nullptr) {
        f32 wid = field_0x65 * 2;
        u16 wid2 = field_0x65 * 2;
        GXSetViewport(0.0f, 0.0f, wid, wid, 0.0f, 1.0f);
        GXSetScissor(0, 0, wid2, wid2);
        GXSetScissorBoxOffset(0, 0);
        GXLoadTexObj(mShadow_c::sTexObj, GX_TEXMAP1);
        GXSetNumChans(1);
        GXSetChanCtrl(GX_COLOR0A0, false, GX_SRC_REG, GX_SRC_REG, GX_LIGHT_NULL, GX_DF_NONE, GX_AF_NONE);
        GXSetNumTexGens(0);
        GXSetNumTevStages(1);
        GXSetNumIndStages(0);
        GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD_NULL, GX_TEXMAP_NULL, GX_COLOR0A0);
        GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO);
        GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, 1, GX_TEVPREV);
        GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO);
        GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, 1, GX_TEVPREV);
        GXSetZCompLoc(1);
        GXSetBlendMode(GX_BM_NONE, GX_BL_ONE, GX_BL_ONE, GX_LO_SET);
        GXSetZMode(false, GX_ALWAYS, false);
        GXSetAlphaCompare(GX_ALWAYS, 0, GX_AOP_OR, GX_ALWAYS, 0);
        GXColor clr = {0};
        GXSetFog(GX_FOG_NONE, clr, 0.0f, 0.0f, 0.0f, 0.0f);
        GXSetFogRangeAdj(0, 0, 0);
        GXSetCullMode(GX_CULL_BACK);
        GXSetDither(0);
        GXSetClipMode(GX_CLIP_DISABLE);
        GXLoadPosMtxImm(mMtx_c::Identity.m, 0);
        GXSetCurrentMtx(0);
        Mtx44 mtx;
        C_MTXOrtho(mtx, 0.0f, wid, 0.0f, wid, 0.0f, 1.0f);
        GXSetProjection(mtx, GX_ORTHOGRAPHIC);
        GXSetLineWidth(0x18, 0);
        int i = 0;
        void *imgData = mpBuf;
        do {
            iter->updateMtx();
            if (iter->mNumLeaves != 0) {
                if (i == 0) {
                    GXClearVtxDesc();
                    GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
                    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_CLR_RGB, GX_RGBX8, 0);
                    GXBegin(GX_QUADS, GX_VTXFMT0, 4);
                    GXPosition2u16(0, 0);
                    GXPosition2u16(wid2, 0);
                    GXPosition2u16(wid2, wid2);
                    GXPosition2u16(0, wid2);
                    // GXEnd();
                    GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO, GX_CC_C0);
                    GXSetBlendMode(GX_BM_LOGIC, GX_BL_ONE, GX_BL_ONE, GX_LO_OR);
                }
                iter->field_0x15A = i;
                GXInitTexObj(&iter->mTexObj, imgData, field_0x65, field_0x65, GX_TF_RGB565, GX_CLAMP, GX_CLAMP, false);
                GXInitTexObjLOD(&iter->mTexObj, GX_LINEAR, GX_LINEAR, 0.0f, 0.0f, 0.0f, 0, 0, GX_ANISO_1);
                iter->drawMdl();
                i = (i + 1) % 3;
                if (i == 0) {
                    drawSub1(imgData, field_0x65);
                    imgData = (void *)((u8 *)imgData + mTexBufferSize);
                }
            }
            iter = static_cast<mShadowChild_c *>(nw4r::ut::List_GetNext(&mList, iter));
        } while (iter != nullptr);

        if (i != 0) {
            drawSub1(imgData, field_0x65);
        }

        GXSetClipMode(GX_CLIP_ENABLE);
        GXSetDither(true);
        nw4r::g3d::G3DState::Invalidate(0x7FF);
    }
}

void mShadow_c::create(const mShadowCircleConfig *config, nw4r::g3d::ResMdl mdl, EGG::Heap *heap) {
    mShadow_c::sInstance = new (heap, 0x04) mShadow_c(heap);
    mShadow_c::sInstance->create(config->count, config->unk1, config->unk2, config->texBufferSize,
            config->drawOpaPriority, mdl, config->heapSize);
}

void mShadow_c::beforeDraw() {
    if (mpCallback != nullptr) {
        mpCallback->beforeDraw();
    }
}

void mShadow_c::draw(const mMtx_c &mtx, u32 unk) {
    if (mpCallback != nullptr) {
        mpCallback->draw(mtx, unk);
    }
}

void mShadow_c::afterDraw() {
    if (mpCallback != nullptr) {
        mpCallback->afterDraw();
    }
}

void mShadow_c::swapHeaps() {
    // TODO I don't know how to generate this assembly
    u8 newIndex = ((u32)mCurrentHeapIdx + 1) & 1;
    mCurrentHeapIdx = newIndex;
    mpCurrentHeap = mpFrmHeaps[newIndex];
    mpCurrentHeap->freeState(FRM_HEAP_STATE);
    mpCurrentHeap->recordState(FRM_HEAP_STATE);
}

void mShadow_c::destroy() {
    if (mShadow_c::sInstance != nullptr) {
        mShadow_c::sInstance->remove();
        delete mShadow_c::sInstance;
        mShadow_c::sInstance = nullptr;
    }
}

mShadowChild_c::~mShadowChild_c() {
    if (mpCircle != nullptr) {
        mpCircle->mpChild = nullptr;
        mpCircle = nullptr;
    }

    if (mpLeaves != nullptr) {
        mpHeap->free(mpLeaves);
        mpLeaves = nullptr;
    }
}

bool mShadowChild_c::create(u8 maxNum, EGG::Heap *heap) {
    if ((mpLeaves = static_cast<scnLeaf_c **>(heap->alloc(maxNum * sizeof(scnLeaf_c *), 0x04))) == nullptr) {
        return false;
    }
    mpHeap = heap;
    mMaxNumLeaves = maxNum;
    return true;
}

void mShadowChild_c::set(const mVec3_c &pos, f32 dist, mColor color) {
    mPositionMaybe = pos;
    mOffsetMaybe = dist;
    mShadowColor = color;
}

bool mShadowChild_c::addMdl(scnLeaf_c &mdl, const mQuat_c &quat) {
    if (!(mNumLeaves < mMaxNumLeaves)) {
        return false;
    }

    mMtx_c mtx;
    if (mdl.getType() == 0) {
        mdl.getLocalMtx(mtx);
    } else {
        mtx = static_cast<mCustomShadow_c &>(mdl).mMtx;
    }

    // TODO this copy is a bit weird (reads members in a different order)
    mQuat_c q = quat;
    PSMTXMultVec(mtx.m, q, q);
    if (mNumLeaves == 0) {
        mQuat = q;
    } else {
        mQuat.fn_802F2780(q);
    }
    mpLeaves[mNumLeaves++] = &mdl;
    return true;
}

bool mShadowChild_c::setGeom(const GXTexObj *texObj, const mMtx_c &mtx, const mQuat_c &quat) {
    mQuat = quat;
    PSMTXMultVec(mtx.m, mQuat, mQuat);
    if (texObj == nullptr) {
        mTexObj = *mShadow_c::sTexObj;
    } else {
        mTexObj = *texObj;
    }
    return true;
}

void mShadowChild_c::updateMtx() {
    // TODO all of this is broken
    mVec3_c a = *(mVec3_c *)(&mQuat) + mPositionMaybe * mOffsetMaybe;
    mVec3_c b = *(mVec3_c *)(&mQuat) - mPositionMaybe * field_0x13C;
    mMtx_c mtx;
    C_MTXLookAt(mtx.m, b,
            *(fabsf((a.x - b.x) * (a.x - b.x) + (a.z - b.z) * (a.z - b.z)) <= FLT_EPSILON ? &mVec3_c::Ez :
                                                                                            &mVec3_c::Ey),
            a);
    f32 f = field_0x13C;
    mFrustum.set(f, -f, -f, f, f, f + mOffsetMaybe, mtx, true);
}

void mShadowChild_c::drawMdl() {
    // TODO maybe roughly equivalent
    mMtx_c mtx;
    GXSetTevColor(GX_TEVREG0, sColors[field_0x15A]);
    C_MTXOrtho(mtx.m, field_0x13C, -field_0x13C, -field_0x13C, field_0x13C, 0.0f, 100.0f + (-mFrustum.mFar));
    GXSetProjection(mtx.m, GX_ORTHOGRAPHIC);
    nw4r::g3d::G3DState::Invalidate(0x7FF);
    for (scnLeaf_c **leaf = &mpLeaves[mNumLeaves - 1]; leaf >= &mpLeaves[0]; leaf--) {
        scnLeaf_c *lf = *leaf;
        if (lf->getType() == 0) {
            nw4r::g3d::ScnMdlSimple *mdl = nw4r::g3d::G3dObj::DynamicCast<nw4r::g3d::ScnMdlSimple>(lf->getG3dObject());

            u32 bufSize = mdl->GetNumViewMtx() * sizeof(nw4r::math::MTX34);
            nw4r::math::MTX34 *buf =
                    static_cast<nw4r::math::MTX34 *>(mShadow_c::sInstance->mpHeap->alloc(bufSize, 0x20));
            nw4r::g3d::CalcView(buf, nullptr, mdl->GetWldMtxArray(), mdl->GetWldMtxAttribArray(), mdl->GetNumViewMtx(),
                    mFrustum.mMtx, mdl->GetResMdl(), nullptr);
            DCStoreRange(buf, bufSize);

            nw4r::g3d::ScnMdl *mdl2 = nw4r::g3d::G3dObj::DynamicCast<nw4r::g3d::ScnMdl>(lf->getG3dObject());
            nw4r::g3d::DrawResMdlReplacement *repl;
            if (mdl2 != nullptr) {
                repl = mdl2->GetDrawResMdlReplacement();
            } else {
                repl = nullptr;
            }
            nw4r::g3d::DrawResMdlDirectly(mdl->GetResMdl(), buf, nullptr, nullptr,
                    mdl2->GetByteCode(nw4r::g3d::ScnMdlSimple::BYTE_CODE_DRAW_OPA), nullptr, repl, 0x0C);
            GXInvalidateVtxCache();
        } else {
            // this happens with bomb bag, and goes to 0x802EDC90 (mCustomShadow_c::draw)
            static_cast<mCustomShadow_c *>(lf)->draw(mFrustum.mMtx);
        }
    }
}

static GXTevColorChan sChans[] = {GX_CH_RED, GX_CH_GREEN, GX_CH_BLUE, GX_CH_ALPHA};

void mShadowChild_c::draw() {
    if (mNumLeaves != 0) {
        GXTevColorChan chan = sChans[field_0x15A];
        GXSetTevSwapModeTable(GX_TEV_SWAP0, chan, chan, chan, chan);
    } else {
        GXSetTevSwapModeTable(GX_TEV_SWAP0, GX_CH_RED, GX_CH_GREEN, GX_CH_BLUE, GX_CH_ALPHA);
    }
    GXLoadTexObj(&mTexObj, GX_TEXMAP0);
    GXSetTevColor(GX_TEVREG0, mShadowColor);
    Mtx mtx;
    C_MTXLightOrtho(mtx, field_0x13C, -field_0x13C, -field_0x13C, field_0x13C, 0.5f, -0.5f, 0.5f, 0.5f);
    PSMTXConcat(mtx, mFrustum.mMtx.m, mtx);
    GXLoadTexMtxImm(mtx, 0x1E, GX_MTX_3x4);
    mShadow_c::sInstance->draw(mFrustum.mMtx, field_0x154);
    GXSetTevSwapModeTable(GX_TEV_SWAP0, GX_CH_RED, GX_CH_GREEN, GX_CH_BLUE, GX_CH_ALPHA);
}

mShadowCircle_c::~mShadowCircle_c() {
    mShadow_c::sInstance->removeCircle(this);
}

mCustomShadow_c::~mCustomShadow_c() {}

int mCustomShadow_c::getType() const {
    return 0x2;
}

void mCustomShadow_c::draw(const mMtx_c &arg) {
    nw4r::g3d::G3DState::Invalidate(0x7FF);
    GXSetNumTexGens(1);
    GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, 0x3C, false, 0x7D);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP1, GX_COLOR_NULL);
    GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_C0, GX_CC_TEXC, GX_CC_ZERO);

    mMtx_c result;
    calc(arg, result);
    static nw4r::g3d::ResMat nullMat(nullptr);

    nw4r::g3d::Draw1Mat1ShpDirectly(nullMat, mShadow_c::sResShp, result, result, 0x18, nullptr, nullptr);
    GXSetNumTexGens(0);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD_NULL, GX_TEXMAP_NULL, GX_COLOR0A0);
    GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO, GX_CC_C0);
}

void mCustomShadow_c::calc(mMtx_c mtx, mMtx_c &mtx2) {
    // TODO some shuffles
    mtx2 = mMtx;
    mVec3_c v(0.0f, field_0x48, 0.0f);
    mVec3_c v2;
    PSMTXMultVec(mtx2.m, v, v2);
    PSMTXMultVec(mtx.m, v2, v);
    PSMTXTrans(mtx2.m, v2.x, v2.y, v2.z);
    mMtx_c scaleMtx;
    PSMTXScale(scaleMtx.m, field_0x4C, field_0x4C, field_0x4C);
    PSMTXConcat(mtx2.m, scaleMtx.m, mtx2.m);
}

} // namespace m3d
