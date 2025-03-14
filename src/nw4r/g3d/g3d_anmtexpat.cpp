#include <nw4r/g3d.h>

#include <nw4r/ut.h>

#include <rvl/GX.h>

namespace nw4r {
namespace g3d {

NW4R_G3D_RTTI_DEF(AnmObjTexPat);
NW4R_G3D_RTTI_DEF(AnmObjTexPatNode);
NW4R_G3D_RTTI_DEF(AnmObjTexPatOverride);
NW4R_G3D_RTTI_DEF(AnmObjTexPatRes);

/******************************************************************************
 *
 * AnmObjTexPat
 *
 ******************************************************************************/
AnmObjTexPat::AnmObjTexPat(MEMAllocator *pAllocator, u16 *pBindingBuf, int numBinding)
    : AnmObj(pAllocator, NULL), mNumBinding(numBinding), mpBinding(pBindingBuf) {
    Release();
}

bool AnmObjTexPat::TestExistence(u32 idx) const {
    return !(mpBinding[idx] & (BINDING_UNDEFINED | BINDING_INVALID));
}

bool AnmObjTexPat::TestDefined(u32 idx) const {
    return !(mpBinding[idx] & BINDING_UNDEFINED);
}

void AnmObjTexPat::Release() {
    for (int i = 0; i < mNumBinding; i++) {
        mpBinding[i] = BINDING_UNDEFINED;
    }

    SetAnmFlag(FLAG_ANM_BOUND, false);
}

AnmObjTexPatRes *AnmObjTexPat::Attach(int idx, AnmObjTexPatRes *pRes) {
#pragma unused(idx)
#pragma unused(pRes)

    return NULL;
}

AnmObjTexPatRes *AnmObjTexPat::Detach(int idx) {
#pragma unused(idx)

    return NULL;
}

void AnmObjTexPat::DetachAll() {}

/******************************************************************************
 *
 * AnmObjTexPatNode
 *
 ******************************************************************************/
AnmObjTexPatNode::AnmObjTexPatNode(
    MEMAllocator *pAllocator, u16 *pBindingBuf, int numBinding, AnmObjTexPatRes **ppChildrenBuf, int numChildren
)
    : AnmObjTexPat(pAllocator, pBindingBuf, numBinding),
      mChildrenArraySize(numChildren),
      mpChildrenArray(ppChildrenBuf) {
    for (int i = 0; i < mChildrenArraySize; i++) {
        mpChildrenArray[i] = NULL;
    }
}

AnmObjTexPatNode::~AnmObjTexPatNode() {
    DetachAll();
}

AnmObjTexPatRes *AnmObjTexPatNode::Attach(int idx, AnmObjTexPatRes *pRes) {
    AnmObjTexPatRes *pOld = Detach(idx);
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

AnmObjTexPatRes *AnmObjTexPatNode::Detach(int idx) {
    AnmObjTexPatRes *pOld = mpChildrenArray[idx];

    if (pOld != NULL) {
        pOld->G3dProc(G3DPROC_DETACH_PARENT, 0, this);
        mpChildrenArray[idx] = NULL;

        bool hasAnm = false;
        for (u32 i = 0; i < mNumBinding; i++) {
            u16 binding = BINDING_UNDEFINED;

            for (int j = 0; j < mChildrenArraySize; j++) {
                AnmObjTexPatRes *pChild = mpChildrenArray[j];

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

void AnmObjTexPatNode::DetachAll() {
    for (int i = 0; i < mChildrenArraySize; i++) {
        Detach(i);
    }
}

void AnmObjTexPatNode::UpdateFrame() {
    for (int i = 0; i < mChildrenArraySize; i++) {
        if (mpChildrenArray[i] != NULL) {
            mpChildrenArray[i]->UpdateFrame();
        }
    }
}

void AnmObjTexPatNode::SetFrame(f32 frame) {
    for (int i = 0; i < mChildrenArraySize; i++) {
        if (mpChildrenArray[i] != NULL) {
            mpChildrenArray[i]->SetFrame(frame);
        }
    }
}

f32 AnmObjTexPatNode::GetFrame() const {
    for (int i = 0; i < mChildrenArraySize; i++) {
        if (mpChildrenArray[i] != NULL) {
            return mpChildrenArray[i]->GetFrame();
        }
    }

    return 0.0f;
}

void AnmObjTexPatNode::SetUpdateRate(f32 rate) {
    for (int i = 0; i < mChildrenArraySize; i++) {
        if (mpChildrenArray[i] != NULL) {
            mpChildrenArray[i]->SetUpdateRate(rate);
        }
    }
}

f32 AnmObjTexPatNode::GetUpdateRate() const {
    for (int i = 0; i < mChildrenArraySize; i++) {
        if (mpChildrenArray[i] != NULL) {
            return mpChildrenArray[i]->GetUpdateRate();
        }
    }

    return 1.0f;
}

bool AnmObjTexPatNode::Bind(const ResMdl mdl) {
    bool success = false;

    for (int i = 0; i < mChildrenArraySize; i++) {
        AnmObjTexPatRes *pChild = mpChildrenArray[i];
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

void AnmObjTexPatNode::Release() {
    for (int i = 0; i < mChildrenArraySize; i++) {
        if (mpChildrenArray[i] != NULL) {
            mpChildrenArray[i]->Release();
        }
    }

    AnmObjTexPat::Release();
}

void AnmObjTexPatNode::G3dProc(u32 task, u32 param, void *pInfo) {
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
 * AnmObjTexPatOverride
 *
 ******************************************************************************/
AnmObjTexPatOverride *
AnmObjTexPatOverride::Construct(MEMAllocator *pAllocator, u32 *pSize, ResMdl mdl, int numChildren) {
    if (!mdl.IsValid()) {
        return NULL;
    }

    int numAnim = mdl.GetResMatNumEntries();
    int bindNum = numAnim;

    u32 objSize = sizeof(AnmObjTexPatOverride);
    u32 bindSize = bindNum * sizeof(u16);
    u32 childrenSize = numChildren * sizeof(AnmObjTexPatRes *);

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
    return new (pBuffer) AnmObjTexPatOverride(
        pAllocator,
        reinterpret_cast<u16*>(pBuffer + bindOfs),
        bindNum,
        reinterpret_cast<AnmObjTexPatRes**>(pBuffer + childrenOfs),
        numChildren);
    // clang-format on
}

const TexPatAnmResult *AnmObjTexPatOverride::GetResult(TexPatAnmResult *pResult, u32 idx) {
    for (int i = mChildrenArraySize - 1; i >= 0; i--) {
        AnmObjTexPatRes *pChild = mpChildrenArray[i];

        if (pChild == NULL || !pChild->TestExistence(idx)) {
            continue;
        }

        const TexPatAnmResult *pChildResult = pChild->GetResult(pResult, idx);

        if (pChildResult->bTexExist != 0 || pChildResult->bPlttExist) {
            return pChildResult;
        }
    }

    pResult->bTexExist = 0;
    pResult->bPlttExist = 0;
    return pResult;
}

/******************************************************************************
 *
 * AnmObjTexPatRes
 *
 ******************************************************************************/
AnmObjTexPatRes *
AnmObjTexPatRes::Construct(MEMAllocator *pAllocator, u32 *pSize, ResAnmTexPat pat, ResMdl mdl, bool cache) {
    if (!pat.IsValid() || !mdl.IsValid()) {
        return NULL;
    }

    int numAnim = pat.GetNumMaterial();
    int numMat = mdl.GetResMatNumEntries();

    int bindNum = numMat;
    int cacheNum = cache ? numAnim : 0;

    u32 bindSize = bindNum * sizeof(u16);
    u32 cacheSize = cacheNum * sizeof(TexPatAnmResult);

    u32 size = bindSize + cacheSize + sizeof(AnmObjTexPatRes);
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

    TexPatAnmResult *pCacheBuf = cache ? reinterpret_cast<TexPatAnmResult *>(pBuffer + sizeof(AnmObjTexPatRes)) : NULL;

    u16 *pBindingBuf = reinterpret_cast<u16 *>(cacheSize + (pBuffer + sizeof(AnmObjTexPatRes)));

    return new (pBuffer) AnmObjTexPatRes(pAllocator, pat, pBindingBuf, bindNum, pCacheBuf);
}

AnmObjTexPatRes::AnmObjTexPatRes(
    MEMAllocator *pAllocator, ResAnmTexPat pat, u16 *pBindingBuf, int numBinding, TexPatAnmResult *pCacheBuf
)
    : AnmObjTexPat(pAllocator, pBindingBuf, numBinding),
      FrameCtrl(0.0f, pat.GetNumFrame(), GetAnmPlayPolicy(pat.GetAnmPolicy())),
      mRes(pat),
      mpResultCache(pCacheBuf) {
    if (mpResultCache != NULL) {
        UpdateCache();
    }
}

void AnmObjTexPatRes::SetFrame(f32 frame) {
    SetFrm(frame);

    if (mpResultCache != NULL) {
        UpdateCache();
    }
}

f32 AnmObjTexPatRes::GetFrame() const {
    return GetFrm();
}

void AnmObjTexPatRes::SetUpdateRate(f32 rate) {
    SetRate(rate);
    if (rate == 0.f && mpResultCache != NULL) {
        UpdateCache();
    }
}

f32 AnmObjTexPatRes::GetUpdateRate() const {
    return GetRate();
}

void AnmObjTexPatRes::UpdateFrame() {
    if (GetRate() != 0.f) {
        UpdateFrm();

        if (mpResultCache != NULL) {
            UpdateCache();
        }
    }
}

bool AnmObjTexPatRes::Bind(const ResMdl mdl) {
    int numAnim = mRes.GetNumMaterial();
    bool success = false;

    for (u16 i = 0; i < numAnim; i++) {
        const ResAnmTexPatMatData *pData = mRes.GetMatAnm(i);

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

const TexPatAnmResult *AnmObjTexPatRes::GetResult(TexPatAnmResult *pResult, u32 i) {
    u32 id = mpBinding[i];

    if (id & (BINDING_UNDEFINED | BINDING_INVALID)) {
        pResult->bTexExist = 0;
        pResult->bPlttExist = 0;
        return pResult;
    }

    if (mpResultCache != NULL) {
        return &mpResultCache[id];
    }

    mRes.GetAnmResult(pResult, id, GetFrm());
    return pResult;
}

void AnmObjTexPatRes::UpdateCache() {
    f32 frame = GetFrm();

    for (u32 i = 0; i < mNumBinding; i++) {
        u16 bind = mpBinding[i];

        if (!(bind & BINDING_UNDEFINED)) {
            u32 id = bind & BINDING_ID_MASK;
            mRes.GetAnmResult(&mpResultCache[id], id, frame);
        }
    }
}

void AnmObjTexPatRes::G3dProc(u32 task, u32 param, void *pInfo) {
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
 * ApplyTexPatAnmResult
 *
 ******************************************************************************/
void ApplyTexPatAnmResult(ResTexObj texObj, ResTlutObj tlutObj, const TexPatAnmResult *pResult) {
    u32 texExist = pResult->bTexExist;
    u32 i;

    for (i = 0; texExist != 0; texExist >>= 1, i++) {
        if (!(texExist & 1)) {
            continue;
        }

        ResTex tex = pResult->tex[i];
        GXTexObj *pGXObj = texObj.GetTexObj(static_cast<GXTexMapID>(i));

        GXTexFilter minFilt, magFilt;
        f32 minLod, maxLod, lodBias;
        GXBool biasClamp, edgeLod, mipmap;
        GXAnisotropy maxAniso;

        void *pTexData;
        u16 width, height;
        GXTexFmt fmt;
        GXCITexFmt cifmt;

        GXGetTexObjLODAll(pGXObj, &minFilt, &magFilt, &minLod, &maxLod, &lodBias, &biasClamp, &edgeLod, &maxAniso);

        GXTexWrapMode wrapS = GXGetTexObjWrapS(pGXObj);
        GXTexWrapMode wrapT = GXGetTexObjWrapT(pGXObj);

        if (tex.IsCIFmt()) {
            tex.GetTexObjCIParam(&pTexData, &width, &height, &cifmt, &minLod, &maxLod, &mipmap);

            GXInitTexObjCI(
                pGXObj, pTexData, width, height, static_cast<GXTexFmt>(cifmt), wrapS, wrapT, mipmap,
                static_cast<GXTlut>(i)
            );
        } else {
            tex.GetTexObjParam(&pTexData, &width, &height, &fmt, &minLod, &maxLod, &mipmap);

            GXInitTexObj(pGXObj, pTexData, width, height, fmt, wrapS, wrapT, mipmap);
        }

        GXInitTexObjLOD(pGXObj, minFilt, magFilt, minLod, maxLod, lodBias, biasClamp, edgeLod, maxAniso);
    }

    u32 plttExist = pResult->bPlttExist;

    for (i = 0; plttExist != 0; plttExist >>= 1, i++) {
        if (!(plttExist & 1)) {
            continue;
        }

        ResPltt pltt = pResult->pltt[i];

        void *pLUT = pltt.GetPlttData();
        GXTlutFmt fmt = pltt.GetFmt();
        u16 numEntries = pltt.GetNumEntries();
        GXTlutObj *pGXObj = tlutObj.GetTlut(static_cast<GXTlut>(i));

        GXInitTlutObj(pGXObj, pLUT, fmt, numEntries);
    }
}

} // namespace g3d
} // namespace nw4r
