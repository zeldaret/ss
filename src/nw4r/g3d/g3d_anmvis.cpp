#include <nw4r/g3d.h>

#include <nw4r/ut.h>

namespace nw4r {
namespace g3d {

NW4R_G3D_RTTI_DEF(AnmObjVis);
NW4R_G3D_RTTI_DEF(AnmObjVisNode);
NW4R_G3D_RTTI_DEF(AnmObjVisOR);
NW4R_G3D_RTTI_DEF(AnmObjVisRes);

/******************************************************************************
 *
 * AnmObjVis
 *
 ******************************************************************************/
AnmObjVis::AnmObjVis(MEMAllocator* pAllocator, u16* pBindingBuf, int numBinding)
    : AnmObj(pAllocator, NULL),
      mpBinding(pBindingBuf),
      mNumBinding(numBinding) {

    Release();
}

bool AnmObjVis::TestExistence(u32 idx) const {
    return !(mpBinding[idx] & (BINDING_UNDEFINED | BINDING_INVALID));
}

bool AnmObjVis::TestDefined(u32 idx) const {
    return !(mpBinding[idx] & BINDING_UNDEFINED);
}

void AnmObjVis::Release() {
    for (int i = 0; i < mNumBinding; i++) {
        mpBinding[i] = BINDING_UNDEFINED;
    }

    SetAnmFlag(FLAG_ANM_BOUND, false);
}

AnmObjVisRes* AnmObjVis::Attach(int idx, AnmObjVisRes* pRes) {
#pragma unused(idx)
#pragma unused(pRes)

    return NULL;
}

AnmObjVisRes* AnmObjVis::Detach(int idx) {
#pragma unused(idx)

    return NULL;
}

void AnmObjVis::DetachAll() {
    for (int i = 0; i < MAX_CHILD; i++) {
        Detach(i);
    }
}

void AnmObjVis::G3dProc(u32 task, u32 param, void* pInfo) {
#pragma unused(param)

    switch (task) {
    case G3DPROC_UPDATEFRAME: {
        SetAnmFlag(FLAG_CACHE_OBSOLETE, true);
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
 * AnmObjVisNode
 *
 ******************************************************************************/
AnmObjVisNode::AnmObjVisNode(MEMAllocator* pAllocator, u16* pBindingBuf,
                             int numBinding)
    : AnmObjVis(pAllocator, pBindingBuf, numBinding) {

    for (int i = 0; i < MAX_CHILD; i++) {
        mpChildren[i] = NULL;
    }
}

AnmObjVisNode::~AnmObjVisNode() {
    DetachAll();
}

AnmObjVisRes* AnmObjVisNode::Attach(int idx, AnmObjVisRes* pRes) {
    AnmObjVisRes* pOld = Detach(idx);
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

    mpChildren[idx] = pRes;
    pRes->G3dProc(G3DPROC_ATTACH_PARENT, 0, this);
    return pOld;
}

AnmObjVisRes* AnmObjVisNode::Detach(int idx) {
    AnmObjVisRes* pOld = mpChildren[idx];

    if (pOld != NULL) {
        pOld->G3dProc(G3DPROC_DETACH_PARENT, 0, this);
        mpChildren[idx] = NULL;

        bool hasAnm = false;
        for (u32 i = 0; i < mNumBinding; i++) {
            u16 binding = BINDING_UNDEFINED;

            for (int j = 0; j < MAX_CHILD; j++) {
                AnmObjVisRes* pChild = mpChildren[j];

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

void AnmObjVisNode::UpdateFrame() {
    for (int i = 0; i < MAX_CHILD; i++) {
        if (mpChildren[i] != NULL) {
            mpChildren[i]->UpdateFrame();
        }
    }
}

void AnmObjVisNode::SetFrame(f32 frame) {
    for (int i = 0; i < MAX_CHILD; i++) {
        if (mpChildren[i] != NULL) {
            mpChildren[i]->SetFrame(frame);
        }
    }
}

f32 AnmObjVisNode::GetFrame() const {
    for (int i = 0; i < MAX_CHILD; i++) {
        if (mpChildren[i] != NULL) {
            return mpChildren[i]->GetFrame();
        }
    }

    return 0.0f;
}

void AnmObjVisNode::SetUpdateRate(f32 rate) {
    for (int i = 0; i < MAX_CHILD; i++) {
        if (mpChildren[i] != NULL) {
            mpChildren[i]->SetUpdateRate(rate);
        }
    }
}

f32 AnmObjVisNode::GetUpdateRate() const {
    for (int i = 0; i < MAX_CHILD; i++) {
        if (mpChildren[i] != NULL) {
            return mpChildren[i]->GetUpdateRate();
        }
    }

    return 1.0f;
}

bool AnmObjVisNode::Bind(ResMdl mdl) {
    bool success = false;

    for (int i = 0; i < MAX_CHILD; i++) {
        AnmObjVisRes* pChild = mpChildren[i];
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

void AnmObjVisNode::Release() {
    for (int i = 0; i < MAX_CHILD; i++) {
        if (mpChildren[i] != NULL) {
            mpChildren[i]->Release();
        }
    }

    AnmObjVis::Release();
}

void AnmObjVisNode::G3dProc(u32 task, u32 param, void* pInfo) {
#pragma unused(param)

    switch (task) {
    case G3DPROC_CHILD_DETACHED: {
        for (int i = 0; i < MAX_CHILD; i++) {
            if (mpChildren[i] == pInfo) {
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
 * AnmObjVisOR
 *
 ******************************************************************************/
AnmObjVisOR* AnmObjVisOR::Construct(MEMAllocator* pAllocator, u32* pSize,
                                    ResMdl mdl) {
    if (!mdl.IsValid()) {
        return NULL;
    }

    u32 numAnim = mdl.GetResNodeNumEntries();

    int bindNum = numAnim;
    u32 bindSize = bindNum * sizeof(u16);

    u32 size = bindSize + sizeof(AnmObjVisOR);
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

    u16* pBindingBuf = reinterpret_cast<u16*>(pBuffer + sizeof(AnmObjVisOR));

    AnmObjVisOR* pObjVis =
        new (pBuffer) AnmObjVisOR(pAllocator, pBindingBuf, bindNum);

    return pObjVis;
}

bool AnmObjVisOR::GetResult(u32 idx) {
    for (int i = 0; i < MAX_CHILD; i++) {
        AnmObjVisRes* pChild = mpChildren[i];

        if (pChild == NULL || !pChild->TestExistence(idx)) {
            continue;
        }

        if (!pChild->GetResult(idx)) {
            return false;
        }
    }

    return true;
}

/******************************************************************************
 *
 * AnmObjVisRes
 *
 ******************************************************************************/
AnmObjVisRes* AnmObjVisRes::Construct(MEMAllocator* pAllocator, u32* pSize,
                                      ResAnmVis vis, ResMdl mdl) {
    if (!vis.IsValid() || !mdl.IsValid()) {
        return NULL;
    }

    u32 numAnim = mdl.GetResNodeNumEntries();

    int bindNum = numAnim;
    u32 bindSize = bindNum * sizeof(u16);

    u32 size = bindSize + sizeof(AnmObjVisRes);
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

    u16* pBindingBuf = reinterpret_cast<u16*>(pBuffer + sizeof(AnmObjVisRes));

    AnmObjVisRes* pObjVis =
        new (pBuffer) AnmObjVisRes(pAllocator, vis, pBindingBuf, bindNum);

    return pObjVis;
}

AnmObjVisRes::AnmObjVisRes(MEMAllocator* pAllocator, ResAnmVis vis,
                           u16* pBindingBuf, int numBinding)
    : AnmObjVis(pAllocator, pBindingBuf, numBinding),
      FrameCtrl(0.0f, vis.GetNumFrame(), GetAnmPlayPolicy(vis.GetAnmPolicy())),
      mRes(vis) {}

void AnmObjVisRes::SetFrame(f32 frame) {
    SetFrm(frame);
    G3dProc(G3DPROC_UPDATEFRAME, 0, NULL);
}

f32 AnmObjVisRes::GetFrame() const {
    return GetFrm();
}

void AnmObjVisRes::SetUpdateRate(f32 rate) {
    SetRate(rate);
}

f32 AnmObjVisRes::GetUpdateRate() const {
    return GetRate();
}

void AnmObjVisRes::UpdateFrame() {
    UpdateFrm();
    G3dProc(G3DPROC_UPDATEFRAME, 0, NULL);
}

bool AnmObjVisRes::Bind(ResMdl mdl) {
    int numAnim = mRes.GetNumNode();
    bool success = false;

    for (u16 i = 0; i < numAnim; i++) {
        const ResAnmVisAnmData* pData = mRes.GetNodeAnm(i);

        // Seek back from name string to start of ResName
        ResName name(ut::AddOffsetToPtr(pData, pData->name - 4));

        ResNode node = mdl.GetResNode(name);
        if (!node.IsValid()) {
            continue;
        }

        mpBinding[node.GetID()] = i;
        success = true;
    }

    SetAnmFlag(FLAG_ANM_BOUND, true);
    return success;
}

bool AnmObjVisRes::GetResult(u32 idx) {
    u32 id = mpBinding[idx];

    if ((id & BINDING_INVALID) || (id & BINDING_UNDEFINED)) {
        return true;
    }

    return mRes.GetAnmResult(id, GetFrm());
}

/******************************************************************************
 *
 * ApplyVisAnmResult
 *
 ******************************************************************************/
void ApplyVisAnmResult(ResMdl mdl, AnmObjVis* pObj) {
    u32 numNode = mdl.GetResNodeNumEntries();

    for (u32 i = 0; i < numNode; i++) {
        if (!pObj->TestExistence(i)) {
            continue;
        }

        ResNode node = mdl.GetResNode(i);
        node.SetVisibility(pObj->GetResult(i));
    }
}

void ApplyVisAnmResult(u8* byteVec, ResMdl mdl, AnmObjVis* pObj) {
    u32 numNode = mdl.GetResNodeNumEntries();

    for (u32 i = 0; i < numNode; i++) {
        if (!pObj->TestExistence(i)) {
            continue;
        }

        byteVec[i] = pObj->GetResult(i);
    }
}

} // namespace g3d
} // namespace nw4r
