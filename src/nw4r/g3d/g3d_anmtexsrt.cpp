#include <nw4r/g3d.h>

#include <nw4r/ut.h>

#include <rvl/GX.h>

#include <cmath>

namespace nw4r {
namespace g3d {

NW4R_G3D_RTTI_DEF(AnmObjTexSrt);
NW4R_G3D_RTTI_DEF(AnmObjTexSrtNode);
NW4R_G3D_RTTI_DEF(AnmObjTexSrtOverride);
NW4R_G3D_RTTI_DEF(AnmObjTexSrtRes);

/******************************************************************************
 *
 * AnmObjTexSrt
 *
 ******************************************************************************/
AnmObjTexSrt::AnmObjTexSrt(MEMAllocator *pAllocator, u16 *pBindingBuf, int numBinding)
    : AnmObj(pAllocator, NULL), mNumBinding(numBinding), mpBinding(pBindingBuf) {
    Release();
}

bool AnmObjTexSrt::TestExistence(u32 idx) const {
    return !(mpBinding[idx] & (BINDING_UNDEFINED | BINDING_INVALID));
}

bool AnmObjTexSrt::TestDefined(u32 idx) const {
    return !(mpBinding[idx] & BINDING_UNDEFINED);
}

void AnmObjTexSrt::Release() {
    for (int i = 0; i < mNumBinding; i++) {
        mpBinding[i] = BINDING_UNDEFINED;
    }

    SetAnmFlag(FLAG_ANM_BOUND, false);
}

AnmObjTexSrtRes *AnmObjTexSrt::Attach(int idx, AnmObjTexSrtRes *pRes) {
#pragma unused(idx)
#pragma unused(pRes)

    return NULL;
}

AnmObjTexSrtRes *AnmObjTexSrt::Detach(int idx) {
#pragma unused(idx)

    return NULL;
}

void AnmObjTexSrt::DetachAll() {}

/******************************************************************************
 *
 * AnmObjTexSrtNode
 *
 ******************************************************************************/
AnmObjTexSrtNode::AnmObjTexSrtNode(
    MEMAllocator *pAllocator, u16 *pBindingBuf, int numBinding, AnmObjTexSrtRes **ppChildrenBuf, int numChildren
)
    : AnmObjTexSrt(pAllocator, pBindingBuf, numBinding),
      mChildrenArraySize(numChildren),
      mpChildrenArray(ppChildrenBuf) {
    for (int i = 0; i < mChildrenArraySize; i++) {
        mpChildrenArray[i] = NULL;
    }
}

AnmObjTexSrtNode::~AnmObjTexSrtNode() {
    DetachAll();
}

AnmObjTexSrtRes *AnmObjTexSrtNode::Attach(int idx, AnmObjTexSrtRes *pRes) {
    AnmObjTexSrtRes *pOld = Detach(idx);
    bool hasAnm = false;

    for (u32 i = 0; i < mNumBinding; i++) {
        if (!pRes->TestDefined(i)) {
            continue;
        }

        hasAnm = true;
        mpBinding[i] = 0;
    }

    if (hasAnm) {
        SetAnmFlag(FLAG_ANM_BOUND, true);
    }

    mpChildrenArray[idx] = pRes;
    pRes->G3dProc(G3DPROC_ATTACH_PARENT, 0, this);
    return pOld;
}

AnmObjTexSrtRes *AnmObjTexSrtNode::Detach(int idx) {
    AnmObjTexSrtRes *pOld = mpChildrenArray[idx];

    if (pOld != NULL) {
        pOld->G3dProc(G3DPROC_DETACH_PARENT, 0, this);
        mpChildrenArray[idx] = NULL;

        bool hasAnm = false;
        for (u32 i = 0; i < mNumBinding; i++) {
            u16 binding = BINDING_UNDEFINED;

            for (int j = 0; j < mChildrenArraySize; j++) {
                AnmObjTexSrtRes *pChild = mpChildrenArray[j];

                if (pChild == NULL || !pChild->TestDefined(i)) {
                    continue;
                }

                hasAnm = true;
                binding = 0;
                break;
            }

            mpBinding[i] = binding;
        }

        if (!hasAnm) {
            SetAnmFlag(FLAG_ANM_BOUND, false);
        }
    }

    return pOld;
}

void AnmObjTexSrtNode::DetachAll() {
    for (int i = 0; i < mChildrenArraySize; i++) {
        Detach(i);
    }
}

void AnmObjTexSrtNode::UpdateFrame() {
    for (int i = 0; i < mChildrenArraySize; i++) {
        if (mpChildrenArray[i] != NULL) {
            mpChildrenArray[i]->UpdateFrame();
        }
    }
}

void AnmObjTexSrtNode::SetFrame(f32 frame) {
    for (int i = 0; i < mChildrenArraySize; i++) {
        if (mpChildrenArray[i] != NULL) {
            mpChildrenArray[i]->SetFrame(frame);
        }
    }
}

f32 AnmObjTexSrtNode::GetFrame() const {
    for (int i = 0; i < mChildrenArraySize; i++) {
        if (mpChildrenArray[i] != NULL) {
            return mpChildrenArray[i]->GetFrame();
        }
    }

    return 0.0f;
}

void AnmObjTexSrtNode::SetUpdateRate(f32 rate) {
    for (int i = 0; i < mChildrenArraySize; i++) {
        if (mpChildrenArray[i] != NULL) {
            mpChildrenArray[i]->SetUpdateRate(rate);
        }
    }
}

f32 AnmObjTexSrtNode::GetUpdateRate() const {
    for (int i = 0; i < mChildrenArraySize; i++) {
        if (mpChildrenArray[i] != NULL) {
            return mpChildrenArray[i]->GetUpdateRate();
        }
    }

    return 1.0f;
}

bool AnmObjTexSrtNode::Bind(const ResMdl mdl) {
    bool success = false;

    for (int i = 0; i < mChildrenArraySize; i++) {
        AnmObjTexSrtRes *pChild = mpChildrenArray[i];
        if (pChild == NULL) {
            continue;
        }

        bool childSuccess = pChild->Bind(mdl);
        success = success || childSuccess;

        for (u32 j = 0; j < mNumBinding; j++) {
            if (pChild->TestDefined(j)) {
                mpBinding[j] = 0;
            }
        }
    }

    SetAnmFlag(FLAG_ANM_BOUND, true);
    return success;
}

void AnmObjTexSrtNode::Release() {
    for (int i = 0; i < mChildrenArraySize; i++) {
        if (mpChildrenArray[i] != NULL) {
            mpChildrenArray[i]->Release();
        }
    }

    AnmObjTexSrt::Release();
}

void AnmObjTexSrtNode::G3dProc(u32 task, u32 param, void *pInfo) {
#pragma unused(param)

    switch (task) {
        case G3DPROC_CHILD_DETACHED: {
            for (int i = 0; i < mChildrenArraySize; i++) {
                if (mpChildrenArray[i] == pInfo) {
                    Detach(i);
                    return;
                }
            }

            break;
        }

        case G3DPROC_DETACH_PARENT: {
            SetParent(NULL);
            break;
        }

        case G3DPROC_ATTACH_PARENT: {
            SetParent(static_cast<G3dObj *>(pInfo));
            break;
        }
    }
}

/******************************************************************************
 *
 * AnmObjTexSrtOverride
 *
 ******************************************************************************/
AnmObjTexSrtOverride *
AnmObjTexSrtOverride::Construct(MEMAllocator *pAllocator, u32 *pSize, ResMdl mdl, int numChildren) {
    if (!mdl.IsValid()) {
        return NULL;
    }

    int numAnim = mdl.GetResMatNumEntries();
    int bindNum = numAnim;

    u32 objSize = sizeof(AnmObjTexSrtOverride);
    u32 bindSize = bindNum * sizeof(u16);
    u32 childrenSize = numChildren * sizeof(AnmObjTexSrtRes *);

    u32 bindOfs = align4(objSize);
    u32 childrenOfs = align4(bindOfs + bindSize);

    u32 size = align4(childrenOfs + childrenSize);
    if (pSize != NULL) {
        *pSize = size;
    }

    if (pAllocator == NULL) {
        return NULL;
    }

    u8 *pBuffer = reinterpret_cast<u8 *>(Alloc(pAllocator, size));
    if (pBuffer == NULL) {
        return NULL;
    }

    // clang-format off
    return new (pBuffer) AnmObjTexSrtOverride(
        pAllocator,
        reinterpret_cast<u16*>(pBuffer + bindOfs),
        bindNum,
        reinterpret_cast<AnmObjTexSrtRes**>(pBuffer + childrenOfs),
        numChildren);
    // clang-format on
}

const TexSrtAnmResult *AnmObjTexSrtOverride::GetResult(TexSrtAnmResult *pResult, u32 idx) {
    for (int i = mChildrenArraySize - 1; i >= 0; i--) {
        AnmObjTexSrtRes *pChild = mpChildrenArray[i];

        if (pChild == NULL || !pChild->TestExistence(idx)) {
            continue;
        }

        const TexSrtAnmResult *pChildResult = pChild->GetResult(pResult, idx);

        if (pChildResult->flags != 0) {
            return pChildResult;
        }
    }

    pResult->flags = 0;
    return pResult;
}

/******************************************************************************
 *
 * AnmObjTexSrtRes
 *
 ******************************************************************************/
AnmObjTexSrtRes *
AnmObjTexSrtRes::Construct(MEMAllocator *pAllocator, u32 *pSize, ResAnmTexSrt srt, ResMdl mdl, bool cache) {
    if (!srt.IsValid() || !mdl.IsValid()) {
        return NULL;
    }

    int numAnim = srt.GetNumMaterial();
    int numMat = mdl.GetResMatNumEntries();

    int bindNum = numMat;
    int cacheNum = cache ? numAnim : 0;

    u32 bindSize = bindNum * sizeof(u16);
    u32 cacheSize = cacheNum * sizeof(TexSrtAnmResult);

    u32 size = bindSize + cacheSize + sizeof(AnmObjTexSrtRes);
    if (pSize != NULL) {
        *pSize = size;
    }

    if (pAllocator == NULL) {
        return NULL;
    }

    u8 *pBuffer = reinterpret_cast<u8 *>(Alloc(pAllocator, size));
    if (pBuffer == NULL) {
        return NULL;
    }

    TexSrtAnmResult *pCacheBuf = cache ? reinterpret_cast<TexSrtAnmResult *>(pBuffer + sizeof(AnmObjTexSrtRes)) : NULL;

    u16 *pBindingBuf = reinterpret_cast<u16 *>(cacheSize + (pBuffer + sizeof(AnmObjTexSrtRes)));

    return new (pBuffer) AnmObjTexSrtRes(pAllocator, srt, pBindingBuf, bindNum, pCacheBuf);
}

AnmObjTexSrtRes::AnmObjTexSrtRes(
    MEMAllocator *pAllocator, ResAnmTexSrt srt, u16 *pBindingBuf, int numBinding, TexSrtAnmResult *pCacheBuf
)
    : AnmObjTexSrt(pAllocator, pBindingBuf, numBinding),
      FrameCtrl(0.0f, srt.GetNumFrame(), GetAnmPlayPolicy(srt.GetAnmPolicy())),
      mRes(srt),
      mpResultCache(pCacheBuf) {
    if (mpResultCache != NULL) {
        UpdateCache();
    }
}

void AnmObjTexSrtRes::SetFrame(f32 frame) {
    SetFrm(frame);

    if (mpResultCache != NULL) {
        UpdateCache();
    }
}

f32 AnmObjTexSrtRes::GetFrame() const {
    return GetFrm();
}

void AnmObjTexSrtRes::SetUpdateRate(f32 rate) {
    SetRate(rate);
}

f32 AnmObjTexSrtRes::GetUpdateRate() const {
    return GetRate();
}

void AnmObjTexSrtRes::UpdateFrame() {
    UpdateFrm();

    if (mpResultCache != NULL) {
        UpdateCache();
    }
}

bool AnmObjTexSrtRes::Bind(const ResMdl mdl) {
    int numAnim = mRes.GetNumMaterial();
    bool success = false;

    for (u16 i = 0; i < numAnim; i++) {
        const ResAnmTexSrtMatData *pData = mRes.GetMatAnm(i);

        // Seek back from name string to start of ResName
        ResName name(ut::AddOffsetToPtr(pData, pData->name - 4));

        ResMat mat = mdl.GetResMat(name);
        if (!mat.IsValid()) {
            continue;
        }

        mpBinding[mat.GetID()] = i;
        success = true;
    }

    SetAnmFlag(FLAG_ANM_BOUND, true);
    return success;
}

const TexSrtAnmResult *AnmObjTexSrtRes::GetResult(TexSrtAnmResult *pResult, u32 idx) {
    u32 id = mpBinding[idx];

    if (id & (BINDING_UNDEFINED | BINDING_INVALID)) {
        pResult->flags = 0;
        return pResult;
    }

    if (mpResultCache != NULL) {
        return &mpResultCache[id];
    }

    mRes.GetAnmResult(pResult, id, GetFrm());
    return pResult;
}

void AnmObjTexSrtRes::UpdateCache() {
    f32 frame = GetFrm();

    for (u32 i = 0; i < mNumBinding; i++) {
        u16 bind = mpBinding[i];

        if (!(bind & BINDING_UNDEFINED)) {
            u32 id = bind & BINDING_ID_MASK;
            mRes.GetAnmResult(&mpResultCache[id], id, frame);
        }
    }
}

void AnmObjTexSrtRes::G3dProc(u32 task, u32 param, void *pInfo) {
#pragma unused(param)

    switch (task) {
        case G3DPROC_UPDATEFRAME: {
            UpdateFrame();
            break;
        }

        case G3DPROC_DETACH_PARENT: {
            SetParent(NULL);
            break;
        }

        case G3DPROC_ATTACH_PARENT: {
            SetParent(static_cast<G3dObj *>(pInfo));
            break;
        }
    }
}

/******************************************************************************
 *
 * ApplyTexSrtAnmResult
 *
 ******************************************************************************/
void ApplyTexSrtAnmResult(ResTexSrt srt, const TexSrtAnmResult *pResult) {
    ResTexSrtData &r = srt.ref();

    u32 flags = pResult->flags;
    u32 mask = 0x0F;

    for (int i = 0; flags != 0; flags >>= TexSrt::NUM_OF_FLAGS, mask <<= 4, i++) {
        if (!(flags & 1)) {
            continue;
        }

        r.texMtxMode = pResult->texMtxMode;
        r.texSrt[i] = pResult->srt[i];
        r.flag = (r.flag & ~mask) | (pResult->flags & mask);
    }
}

void ApplyTexSrtAnmResult(ResTexSrt srt, ResMatIndMtxAndScale ind, const TexSrtAnmResult *pResult) {
    ApplyTexSrtAnmResult(srt, pResult);

    u32 flags = pResult->indFlags;

    for (int i = TexSrtAnmResult::NUM_OF_MAT_TEX_MTX; flags != 0; i++, flags >>= TexSrt::NUM_OF_FLAGS) {
        if (!(flags & 1)) {
            continue;
        }

        math::MTX34 mtx;

        CalcTexMtx(
            &mtx, true, pResult->srt[i], static_cast<TexSrt::Flag>(flags & TexSrt::FLAGSET_IDENTITY),
            TexSrtTypedef::TEXMATRIXMODE_MAYA
        );

        s8 scaleExp;

        f32 maxElem = 0.000000000000000001f;
        maxElem = ut::Max(maxElem, math::FAbs(mtx._00));
        maxElem = ut::Max(maxElem, math::FAbs(mtx._01));
        maxElem = ut::Max(maxElem, math::FAbs(mtx._03));
        maxElem = ut::Max(maxElem, math::FAbs(mtx._10));
        maxElem = ut::Max(maxElem, math::FAbs(mtx._11));
        maxElem = ut::Max(maxElem, math::FAbs(mtx._13));

        scaleExp = static_cast<s8>(math::FGetExpPart(maxElem) + 1);
        f32 invScale = 0.f; // TODO: ldexpf(1.0f, -scaleExp);

        mtx._00 *= invScale;
        mtx._01 *= invScale;
        mtx._02 = mtx._03 * invScale;
        mtx._10 *= invScale;
        mtx._11 *= invScale;
        mtx._12 = mtx._13 * invScale;

        GXIndTexMtxID id = static_cast<GXIndTexMtxID>(i - TexSrtAnmResult::NUM_OF_MAT_TEX_MTX + 1);

        ind.GXSetIndTexMtx(id, mtx, scaleExp);
    }
}

} // namespace g3d
} // namespace nw4r
