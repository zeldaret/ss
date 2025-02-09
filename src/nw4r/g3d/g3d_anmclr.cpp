#include <nw4r/g3d.h>

#include <nw4r/ut.h>

namespace nw4r {
namespace g3d {

NW4R_G3D_RTTI_DEF(AnmObjMatClr);
NW4R_G3D_RTTI_DEF(AnmObjMatClrNode);
NW4R_G3D_RTTI_DEF(AnmObjMatClrOverride);
NW4R_G3D_RTTI_DEF(AnmObjMatClrRes);

/******************************************************************************
 *
 * AnmObjMatClr
 *
 ******************************************************************************/
AnmObjMatClr::AnmObjMatClr(MEMAllocator* pAllocator, u16* pBindingBuf,
                           int numBinding)
    : AnmObj(pAllocator, NULL),
      mNumBinding(numBinding),
      mpBinding(pBindingBuf) {

    Release();
}

bool AnmObjMatClr::TestExistence(u32 idx) const {
    return !(mpBinding[idx] & (BINDING_UNDEFINED | BINDING_INVALID));
}

bool AnmObjMatClr::TestDefined(u32 idx) const {
    return !(mpBinding[idx] & BINDING_UNDEFINED);
}

void AnmObjMatClr::Release() {
    for (int i = 0; i < mNumBinding; i++) {
        mpBinding[i] = BINDING_UNDEFINED;
    }

    SetAnmFlag(FLAG_ANM_BOUND, false);
}

AnmObjMatClrRes* AnmObjMatClr::Attach(int idx, AnmObjMatClrRes* pRes) {
#pragma unused(idx)
#pragma unused(pRes)

    return NULL;
}

AnmObjMatClrRes* AnmObjMatClr::Detach(int idx) {
#pragma unused(idx)

    return NULL;
}

void AnmObjMatClr::DetachAll() {}

/******************************************************************************
 *
 * AnmObjMatClrNode
 *
 ******************************************************************************/
AnmObjMatClrNode::AnmObjMatClrNode(MEMAllocator* pAllocator, u16* pBindingBuf,
                                   int numBinding,
                                   AnmObjMatClrRes** ppChildrenBuf,
                                   int numChildren)
    : AnmObjMatClr(pAllocator, pBindingBuf, numBinding),
      mChildrenArraySize(numChildren),
      mpChildrenArray(ppChildrenBuf) {

    for (int i = 0; i < mChildrenArraySize; i++) {
        mpChildrenArray[i] = NULL;
    }
}

AnmObjMatClrNode::~AnmObjMatClrNode() {
    DetachAll();
}

AnmObjMatClrRes* AnmObjMatClrNode::Attach(int idx, AnmObjMatClrRes* pRes) {
    AnmObjMatClrRes* pOld = Detach(idx);
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

AnmObjMatClrRes* AnmObjMatClrNode::Detach(int idx) {
    AnmObjMatClrRes* pOld = mpChildrenArray[idx];

    if (pOld != NULL) {
        pOld->G3dProc(G3DPROC_DETACH_PARENT, 0, this);
        mpChildrenArray[idx] = NULL;

        bool hasAnm = false;
        for (u32 i = 0; i < mNumBinding; i++) {
            u16 binding = BINDING_UNDEFINED;

            for (int j = 0; j < mChildrenArraySize; j++) {
                AnmObjMatClrRes* pChild = mpChildrenArray[j];

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

void AnmObjMatClrNode::DetachAll() {
    for (int i = 0; i < mChildrenArraySize; i++) {
        Detach(i);
    }
}

void AnmObjMatClrNode::UpdateFrame() {
    for (int i = 0; i < mChildrenArraySize; i++) {
        if (mpChildrenArray[i] != NULL) {
            mpChildrenArray[i]->UpdateFrame();
        }
    }
}

void AnmObjMatClrNode::SetFrame(f32 frame) {
    for (int i = 0; i < mChildrenArraySize; i++) {
        if (mpChildrenArray[i] != NULL) {
            mpChildrenArray[i]->SetFrame(frame);
        }
    }
}

f32 AnmObjMatClrNode::GetFrame() const {
    for (int i = 0; i < mChildrenArraySize; i++) {
        if (mpChildrenArray[i] != NULL) {
            return mpChildrenArray[i]->GetFrame();
        }
    }

    return 0.0f;
}

void AnmObjMatClrNode::SetUpdateRate(f32 rate) {
    for (int i = 0; i < mChildrenArraySize; i++) {
        if (mpChildrenArray[i] != NULL) {
            mpChildrenArray[i]->SetUpdateRate(rate);
        }
    }
}

f32 AnmObjMatClrNode::GetUpdateRate() const {
    for (int i = 0; i < mChildrenArraySize; i++) {
        if (mpChildrenArray[i] != NULL) {
            return mpChildrenArray[i]->GetUpdateRate();
        }
    }

    return 1.0f;
}

bool AnmObjMatClrNode::Bind(const ResMdl mdl) {
    bool success = false;

    for (int i = 0; i < mChildrenArraySize; i++) {
        AnmObjMatClrRes* pChild = mpChildrenArray[i];
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

void AnmObjMatClrNode::Release() {
    for (int i = 0; i < mChildrenArraySize; i++) {
        if (mpChildrenArray[i] != NULL) {
            mpChildrenArray[i]->Release();
        }
    }

    AnmObjMatClr::Release();
}

void AnmObjMatClrNode::G3dProc(u32 task, u32 param, void* pInfo) {
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
        SetParent(static_cast<G3dObj*>(pInfo));
        break;
    }
    }
}

/******************************************************************************
 *
 * AnmObjMatClrOverride
 *
 ******************************************************************************/
AnmObjMatClrOverride* AnmObjMatClrOverride::Construct(MEMAllocator* pAllocator,
                                                      u32* pSize, ResMdl mdl,
                                                      int numChildren) {
    if (!mdl.IsValid()) {
        return NULL;
    }

    int numAnim = mdl.GetResMatNumEntries();
    int bindNum = numAnim;

    u32 objSize = sizeof(AnmObjMatClrOverride);
    u32 bindSize = bindNum * sizeof(u16);
    u32 childrenSize = numChildren * sizeof(AnmObjMatClrRes*);

    u32 bindOfs = align4(objSize);
    u32 childrenOfs = align4(bindOfs + bindSize);

    u32 size = align4(childrenOfs + childrenSize);
    if (pSize != NULL) {
        *pSize = size;
    }

    if (pAllocator == NULL) {
        return NULL;
    }

    u8* pBuffer = reinterpret_cast<u8*>(Alloc(pAllocator, size));
    if (pBuffer == NULL) {
        return NULL;
    }

    // clang-format off
    return new (pBuffer) AnmObjMatClrOverride(
        pAllocator,
        reinterpret_cast<u16*>(pBuffer + bindOfs),
        bindNum,
        reinterpret_cast<AnmObjMatClrRes**>(pBuffer + childrenOfs),
        numChildren);
    // clang-format on
}

const ClrAnmResult* AnmObjMatClrOverride::GetResult(ClrAnmResult* pResult,
                                                    u32 idx) {
    for (int i = mChildrenArraySize - 1; i >= 0; i--) {
        AnmObjMatClrRes* pChild = mpChildrenArray[i];

        if (pChild == NULL || !pChild->TestExistence(idx)) {
            continue;
        }

        const ClrAnmResult* pChildResult = pChild->GetResult(pResult, idx);

        if (pChildResult->bRgbaExist != 0) {
            return pChildResult;
        }
    }

    pResult->bRgbaExist = 0;
    return pResult;
}

/******************************************************************************
 *
 * AnmObjMatClrRes
 *
 ******************************************************************************/
AnmObjMatClrRes* AnmObjMatClrRes::Construct(MEMAllocator* pAllocator,
                                            u32* pSize, ResAnmClr clr,
                                            ResMdl mdl, bool cache) {
    if (!clr.IsValid() || !mdl.IsValid()) {
        return NULL;
    }

    int numAnim = clr.GetNumMaterial();
    int numMat = mdl.GetResMatNumEntries();

    int bindNum = numMat;
    int cacheNum = cache ? numAnim : 0;

    u32 bindSize = bindNum * sizeof(u16);
    u32 cacheSize = cacheNum * sizeof(ClrAnmResult);

    u32 size = bindSize + cacheSize + sizeof(AnmObjMatClrRes);
    if (pSize != NULL) {
        *pSize = size;
    }

    if (pAllocator == NULL) {
        return NULL;
    }

    u8* pBuffer = reinterpret_cast<u8*>(Alloc(pAllocator, size));
    if (pBuffer == NULL) {
        return NULL;
    }

    ClrAnmResult* pCacheBuf =
        cache
            ? reinterpret_cast<ClrAnmResult*>(pBuffer + sizeof(AnmObjMatClrRes))
            : NULL;

    u16* pBindingBuf =
        reinterpret_cast<u16*>(cacheSize + (pBuffer + sizeof(AnmObjMatClrRes)));

    return new (pBuffer)
        AnmObjMatClrRes(pAllocator, clr, pBindingBuf, bindNum, pCacheBuf);
}

AnmObjMatClrRes::AnmObjMatClrRes(MEMAllocator* pAllocator, ResAnmClr clr,
                                 u16* pBindingBuf, int numBinding,
                                 ClrAnmResult* pCacheBuf)
    : AnmObjMatClr(pAllocator, pBindingBuf, numBinding),
      FrameCtrl(0.0f, clr.GetNumFrame(), GetAnmPlayPolicy(clr.GetAnmPolicy())),
      mRes(clr),
      mpResultCache(pCacheBuf) {

    if (mpResultCache != NULL) {
        UpdateCache();
    }
}

void AnmObjMatClrRes::SetFrame(f32 frame) {
    SetFrm(frame);

    if (mpResultCache != NULL) {
        UpdateCache();
    }
}

f32 AnmObjMatClrRes::GetFrame() const {
    return GetFrm();
}

void AnmObjMatClrRes::SetUpdateRate(f32 rate) {
    SetRate(rate);
}

f32 AnmObjMatClrRes::GetUpdateRate() const {
    return GetRate();
}

void AnmObjMatClrRes::UpdateFrame() {
    UpdateFrm();

    if (mpResultCache != NULL) {
        UpdateCache();
    }
}

bool AnmObjMatClrRes::Bind(const ResMdl mdl) {
    int numAnim = mRes.GetNumMaterial();
    bool success = false;

    for (u16 i = 0; i < numAnim; i++) {
        const ResAnmClrMatData* pData = mRes.GetMatAnm(i);

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

const ClrAnmResult* AnmObjMatClrRes::GetResult(ClrAnmResult* pResult, u32 idx) {
    u32 id = mpBinding[idx];

    if (id & (BINDING_UNDEFINED | BINDING_INVALID)) {
        pResult->bRgbaExist = 0;
        return pResult;
    }

    if (mpResultCache != NULL) {
        return &mpResultCache[id];
    }

    mRes.GetAnmResult(pResult, id, GetFrm());
    return pResult;
}

void AnmObjMatClrRes::UpdateCache() {
    f32 frame = GetFrm();

    for (u32 i = 0; i < mNumBinding; i++) {
        u16 bind = mpBinding[i];

        if (!(bind & BINDING_UNDEFINED)) {
            u32 id = bind & BINDING_ID_MASK;
            mRes.GetAnmResult(&mpResultCache[id], id, frame);
        }
    }
}

void AnmObjMatClrRes::G3dProc(u32 task, u32 param, void* pInfo) {
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
        SetParent(static_cast<G3dObj*>(pInfo));
        break;
    }
    }
}

/******************************************************************************
 *
 * ApplyClrAnmResult
 *
 ******************************************************************************/
void ApplyClrAnmResult(ResMatChan chan, ResMatTevColor tev,
                       const ClrAnmResult* pResult) {
    ut::Color c;

    if (pResult->bRgbaExist &
        (1 << ClrAnmResult::CLA_CLR0 | 1 << ClrAnmResult::CLA_CLR1 |
         1 << ClrAnmResult::CLA_AMB0 | 1 << ClrAnmResult::CLA_AMB1)) {

        if (pResult->bRgbaExist & (1 << ClrAnmResult::CLA_CLR0)) {
            GXColor& rMatColor = chan.ref().chan[0].matColor;
            c = rMatColor;

            rMatColor = (c & pResult->rgbaMask[ClrAnmResult::CLA_CLR0]) |
                        pResult->rgba[ClrAnmResult::CLA_CLR0];
        }

        if (pResult->bRgbaExist & (1 << ClrAnmResult::CLA_AMB0)) {
            GXColor& rMatColor = chan.ref().chan[0].ambColor;
            c = rMatColor;

            rMatColor = (c & pResult->rgbaMask[ClrAnmResult::CLA_AMB0]) |
                        pResult->rgba[ClrAnmResult::CLA_AMB0];
        }

        if (pResult->bRgbaExist & (1 << ClrAnmResult::CLA_CLR1)) {
            GXColor& rMatColor = chan.ref().chan[1].matColor;
            c = rMatColor;

            rMatColor = (c & pResult->rgbaMask[ClrAnmResult::CLA_CLR1]) |
                        pResult->rgba[ClrAnmResult::CLA_CLR1];
        }

        if (pResult->bRgbaExist & (1 << ClrAnmResult::CLA_AMB1)) {
            GXColor& rMatColor = chan.ref().chan[1].ambColor;
            c = rMatColor;

            rMatColor = (c & pResult->rgbaMask[ClrAnmResult::CLA_AMB1]) |
                        pResult->rgba[ClrAnmResult::CLA_AMB1];
        }
    }

    if (pResult->bRgbaExist &
        (1 << ClrAnmResult::CLA_TEV0 | 1 << ClrAnmResult::CLA_TEV1 |
         1 << ClrAnmResult::CLA_TEV2 | 1 << ClrAnmResult::CLA_TEVK0 |
         1 << ClrAnmResult::CLA_TEVK1 | 1 << ClrAnmResult::CLA_TEVK2 |
         1 << ClrAnmResult::CLA_TEVK3)) {

        if (pResult->bRgbaExist & (1 << ClrAnmResult::CLA_TEV0)) {
            if (!tev.GXGetTevColor(GX_TEVREG0, &c)) {
                c = ut::Color::BLACK;
            }

            tev.GXSetTevColor(GX_TEVREG0,
                              c & pResult->rgbaMask[ClrAnmResult::CLA_TEV0] |
                                  pResult->rgba[ClrAnmResult::CLA_TEV0]);
        }

        if (pResult->bRgbaExist & (1 << ClrAnmResult::CLA_TEV1)) {
            if (!tev.GXGetTevColor(GX_TEVREG1, &c)) {
                c = ut::Color::BLACK;
            }

            tev.GXSetTevColor(GX_TEVREG1,
                              c & pResult->rgbaMask[ClrAnmResult::CLA_TEV1] |
                                  pResult->rgba[ClrAnmResult::CLA_TEV1]);
        }

        if (pResult->bRgbaExist & (1 << ClrAnmResult::CLA_TEV2)) {
            if (!tev.GXGetTevColor(GX_TEVREG2, &c)) {
                c = ut::Color::BLACK;
            }

            tev.GXSetTevColor(GX_TEVREG2,
                              c & pResult->rgbaMask[ClrAnmResult::CLA_TEV2] |
                                  pResult->rgba[ClrAnmResult::CLA_TEV2]);
        }

        if (pResult->bRgbaExist & (1 << ClrAnmResult::CLA_TEVK0)) {
            if (!tev.GXGetTevKColor(GX_KCOLOR0, &c)) {
                c = ut::Color::BLACK;
            }

            tev.GXSetTevKColor(GX_KCOLOR0,
                               c & pResult->rgbaMask[ClrAnmResult::CLA_TEVK0] |
                                   pResult->rgba[ClrAnmResult::CLA_TEVK0]);
        }

        if (pResult->bRgbaExist & (1 << ClrAnmResult::CLA_TEVK1)) {
            if (!tev.GXGetTevKColor(GX_KCOLOR1, &c)) {
                c = ut::Color::BLACK;
            }

            tev.GXSetTevKColor(GX_KCOLOR1,
                               c & pResult->rgbaMask[ClrAnmResult::CLA_TEVK1] |
                                   pResult->rgba[ClrAnmResult::CLA_TEVK1]);
        }

        if (pResult->bRgbaExist & (1 << ClrAnmResult::CLA_TEVK2)) {
            if (!tev.GXGetTevKColor(GX_KCOLOR2, &c)) {
                c = ut::Color::BLACK;
            }

            tev.GXSetTevKColor(GX_KCOLOR2,
                               c & pResult->rgbaMask[ClrAnmResult::CLA_TEVK2] |
                                   pResult->rgba[ClrAnmResult::CLA_TEVK2]);
        }

        if (pResult->bRgbaExist & (1 << ClrAnmResult::CLA_TEVK3)) {
            if (!tev.GXGetTevKColor(GX_KCOLOR3, &c)) {
                c = ut::Color::BLACK;
            }

            tev.GXSetTevKColor(GX_KCOLOR3,
                               c & pResult->rgbaMask[ClrAnmResult::CLA_TEVK3] |
                                   pResult->rgba[ClrAnmResult::CLA_TEVK3]);
        }

        tev.DCStore(false);
    }
}

} // namespace g3d
} // namespace nw4r
