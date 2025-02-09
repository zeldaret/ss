#include <nw4r/g3d.h>

#include <nw4r/ut.h>

namespace nw4r {
namespace g3d {

NW4R_G3D_RTTI_DEF(ScnMdlSimple);

ScnMdlSimple* ScnMdlSimple::Construct(MEMAllocator* pAllocator, u32* pSize,
                                      ResMdl mdl, int numView) {
    if (!mdl.IsValid()) {
        return NULL;
    }

    if (numView == 0) {
        numView = 1;
    } else if (numView > VIEW_MAX) {
        numView = VIEW_MAX;
    }

    ScnMdlSimple* pSimple = NULL;
    u32 simpleSize = sizeof(ScnMdlSimple);

    u32 posNrmMtxNum = mdl.GetResMdlInfo().GetNumPosNrmMtx();
    u32 viewMtxNum = mdl.GetResMdlInfo().GetNumViewMtx();

    u32 worldMtxSize = posNrmMtxNum * sizeof(math::MTX34);
    u32 worldAttrSize = posNrmMtxNum * sizeof(u32);

    u32 viewPosTexMtxSizeUnit = viewMtxNum * (sizeof(math::MTX34));
    u32 viewNrmMtxSizeUnit = viewMtxNum * (sizeof(math::MTX33));

    u32 viewPosMtxSize = numView * align32(viewPosTexMtxSizeUnit);

    u32 viewNrmMtxSize = mdl.GetResMdlInfo().ref().need_nrm_mtx_array
                             ? numView * align32(viewNrmMtxSizeUnit)
                             : 0;

    // TODO: Fakematch
    u32 viewTexMtxSize = mdl.ref().info.need_tex_mtx_array
                             ? numView * align32(viewPosTexMtxSizeUnit)
                             : 0;

    u32 worldMtxOfs = align32(simpleSize);
    u32 worldAttrOfs = align32(worldMtxOfs + worldMtxSize);
    u32 viewPosMtxOfs = align32(worldAttrOfs + worldAttrSize);
    u32 viewNrmMtxOfs = align32(viewPosMtxOfs + viewPosMtxSize);
    u32 viewTexMtxOfs = align32(viewNrmMtxOfs + viewNrmMtxSize);

    u32 size = align32(viewTexMtxOfs + viewTexMtxSize);
    if (pSize != NULL) {
        *pSize = size;
    }

    if (pAllocator != NULL) {
        u8* pBuffer = reinterpret_cast<u8*>(Alloc(pAllocator, size));
        if (pBuffer == NULL) {
            return NULL;
        }

        // clang-format off
        pSimple = new (pBuffer) ScnMdlSimple(
            pAllocator,
            mdl,
            reinterpret_cast<math::MTX34*>(pBuffer + worldMtxOfs),
            reinterpret_cast<u32*>(pBuffer + worldAttrOfs),
            reinterpret_cast<math::MTX34*>(pBuffer + viewPosMtxOfs),
            viewNrmMtxSize != 0 ? reinterpret_cast<math::MTX33*>(pBuffer + viewNrmMtxOfs) : NULL,
            viewTexMtxSize != 0 ? reinterpret_cast<math::MTX34*>(pBuffer + viewTexMtxOfs) : NULL,
            numView,
            viewMtxNum);
        // clang-format on
    }

    return pSimple;
}

void ScnMdlSimple::ScnMdlSmpl_CalcPosture(u32 param,
                                          const math::MTX34* pParent) {
    CheckCallback_CALC_WORLD(CALLBACK_TIMING_A, param,
                             const_cast<math::MTX34*>(pParent));

    CalcWorldMtx(pParent, &param);

    CheckCallback_CALC_WORLD(CALLBACK_TIMING_B, param,
                             const_cast<math::MTX34*>(pParent));

    u32 mtxNum = GetResMdl().GetResMdlInfo().GetNumPosNrmMtx();
    math::MTX34* pWorldMtxArray;
    bool locked = false;

    if (mtxNum > MTX_CACHE_MIN && mtxNum < MTX_CACHE_MAX &&
        (locked = ut::LC::Lock()) == true) {

        mFlagScnMdlSimple |= SCNMDLSMPLFLAG_LC_DMA;
        DC::InvalidateRange(GetWldMtxArray(), mtxNum * sizeof(math::MTX34));
        pWorldMtxArray = static_cast<math::MTX34*>(ut::LC::GetBase());
    } else {
        mFlagScnMdlSimple &= ~SCNMDLSMPLFLAG_LC_DMA;
        pWorldMtxArray = GetWldMtxArray();
    }

    ScaleProperty property = GetScaleProperty();
    u32 rootAttr = detail::WorldMtxAttr::GetRootMtxAttr();

    u32 ignoreTrans;
    GetScnObjOption(OPTION_IGNORE_ANMCHR_TRANS, &ignoreTrans);

    if (property == UNIFORM_SCALED) {
        rootAttr = detail::WorldMtxAttr::AnmNotScaleOne(rootAttr);
    } else if (property == NONUNIFORM_SCALED) {
        rootAttr = detail::WorldMtxAttr::AnmNotScaleUniform(rootAttr);
    }

    if (ignoreTrans) {
        rootAttr = detail::WorldMtxAttr::AnmIgnoreTrans(rootAttr);
    }

    if (GetScnMdlCallback() != NULL) {
        FuncObjCalcWorld funcObj(GetScnMdlCallback(), GetScnMdlCallbackTiming(),
                                 GetScnMdlCallbackNodeID());

        CalcWorld(pWorldMtxArray, GetWldMtxAttribArray(), GetByteCodeCalc(),
                  GetMtxPtr(MTX_WORLD), GetResMdl(), GetAnmObjChr(), &funcObj,
                  rootAttr);
    } else {
        CalcWorld(pWorldMtxArray, GetWldMtxAttribArray(), GetByteCodeCalc(),
                  GetMtxPtr(MTX_WORLD), GetResMdl(), GetAnmObjChr(), NULL,
                  rootAttr);
    }

    if (GetByteCodeMix() != NULL) {
        CalcSkinning(pWorldMtxArray, GetWldMtxAttribArray(), GetResMdl(),
                     GetByteCodeMix());
    }

    if (locked) {
        ut::LC::StoreData(GetWldMtxArray(), ut::LC::GetBase(),
                          mtxNum * sizeof(math::MTX34));

        ut::LC::Unlock();
    }
}

void ScnMdlSimple::ScnMdlSmpl_G3DPROC_GATHER_SCNOBJ(
    u32 param, IScnObjGather* pCollection) {
#pragma unused(param)

    pCollection->Add(this, !TestScnObjFlag(SCNOBJFLAG_NOT_GATHER_DRAW_OPA),
                     !TestScnObjFlag(SCNOBJFLAG_NOT_GATHER_DRAW_XLU));
}

void ScnMdlSimple::ScnMdlSmpl_G3DPROC_CALC_WORLD(u32 param,
                                                 const math::MTX34* pParent) {
    ScnMdlSmpl_CalcPosture(param, pParent);

    if (GetAnmObjVis() != NULL) {
        ApplyVisAnmResult(GetResMdl(), GetAnmObjVis());
    }

    CheckCallback_CALC_WORLD(CALLBACK_TIMING_C, param,
                             const_cast<math::MTX34*>(pParent));
}

void ScnMdlSimple::ScnMdlSmpl_G3DPROC_CALC_MAT(u32 param, void* pInfo) {
    CheckCallback_CALC_MAT(CALLBACK_TIMING_A, param, pInfo);

    if (GetAnmObjTexPat() != NULL || GetAnmObjTexSrt() != NULL ||
        GetAnmObjMatClr() != NULL) {

        CalcMaterialDirectly(GetResMdl(), GetAnmObjTexPat(), GetAnmObjTexSrt(),
                             GetAnmObjMatClr());
    }

    CheckCallback_CALC_MAT(CALLBACK_TIMING_C, param, pInfo);
}

void ScnMdlSimple::ScnMdlSmpl_G3DPROC_CALC_VIEW(u32 param,
                                                const math::MTX34* pCamera) {
    mCurView = (mCurView + 1) % mNumView;

    CheckCallback_CALC_VIEW(CALLBACK_TIMING_A, param,
                            const_cast<math::MTX34*>(pCamera));

    CalcViewMtx(pCamera);

    CheckCallback_CALC_VIEW(CALLBACK_TIMING_B, param,
                            const_cast<math::MTX34*>(pCamera));

    if (ut::LC::Lock()) {
        if (mFlagScnMdlSimple & SCNMDLSMPLFLAG_LC_DMA) {
            DC::StoreRange(GetWldMtxArray(),
                           GetNumViewMtx() * sizeof(math::MTX34));

            CalcView_LC_DMA_ModelMtx(GetViewPosMtxArray(), GetViewNrmMtxArray(),
                                     GetWldMtxArray(), GetWldMtxAttribArray(),
                                     GetNumViewMtx(), pCamera, GetResMdl(),
                                     GetViewTexMtxArray());
        } else {
            CalcView_LC(GetViewPosMtxArray(), GetViewNrmMtxArray(),
                        GetWldMtxArray(), GetWldMtxAttribArray(),
                        GetNumViewMtx(), pCamera, GetResMdl(),
                        GetViewTexMtxArray());
        }

        ut::LC::Unlock();
    } else {
        CalcView(GetViewPosMtxArray(), GetViewNrmMtxArray(), GetWldMtxArray(),
                 GetWldMtxAttribArray(), GetNumViewMtx(), pCamera, GetResMdl(),
                 GetViewTexMtxArray());
    }

    CheckCallback_CALC_VIEW(CALLBACK_TIMING_C, param,
                            const_cast<math::MTX34*>(pCamera));
}

void ScnMdlSimple::ScnMdlSmpl_G3DPROC_DRAW_OPA(u32 param, void* pInfo) {
    CheckCallback_DRAW_OPA(CALLBACK_TIMING_A, param, pInfo);

    u32 drawMode = pInfo != NULL ? *static_cast<u32*>(pInfo) : GetDrawMode();

    DrawResMdlDirectly(GetResMdl(), GetViewPosMtxArray(), GetViewNrmMtxArray(),
                       GetViewTexMtxArray(), GetByteCodeDrawOpa(), NULL, NULL,
                       drawMode);

    CheckCallback_DRAW_OPA(CALLBACK_TIMING_C, param, pInfo);
}

void ScnMdlSimple::ScnMdlSmpl_G3DPROC_DRAW_XLU(u32 param, void* pInfo) {
    CheckCallback_DRAW_XLU(CALLBACK_TIMING_A, param, pInfo);

    u32 drawMode = pInfo != NULL ? *static_cast<u32*>(pInfo) : GetDrawMode();

    DrawResMdlDirectly(GetResMdl(), GetViewPosMtxArray(), GetViewNrmMtxArray(),
                       GetViewTexMtxArray(), NULL, GetByteCodeDrawXlu(), NULL,
                       drawMode);

    CheckCallback_DRAW_XLU(CALLBACK_TIMING_C, param, pInfo);
}

void ScnMdlSimple::G3dProc(u32 task, u32 param, void* pInfo) {
    if (IsG3dProcDisabled(task)) {
        return;
    }

    switch (task) {
    case G3DPROC_GATHER_SCNOBJ: {
        ScnMdlSmpl_G3DPROC_GATHER_SCNOBJ(param,
                                         static_cast<IScnObjGather*>(pInfo));
        break;
    }

    case G3DPROC_CALC_WORLD: {
        ScnMdlSmpl_G3DPROC_CALC_WORLD(param, static_cast<math::MTX34*>(pInfo));
        break;
    }

    case G3DPROC_CALC_MAT: {
        ScnMdlSmpl_G3DPROC_CALC_MAT(param, pInfo);
        break;
    }

    case G3DPROC_CALC_VIEW: {
        ScnMdlSmpl_G3DPROC_CALC_VIEW(param, static_cast<math::MTX34*>(pInfo));
        break;
    }

    case G3DPROC_DRAW_OPA: {
        ScnMdlSmpl_G3DPROC_DRAW_OPA(param, pInfo);
        break;
    }

    case G3DPROC_DRAW_XLU: {
        ScnMdlSmpl_G3DPROC_DRAW_XLU(param, pInfo);
        break;
    }

    case G3DPROC_UPDATEFRAME: {
        ScnMdlSmpl_G3DPROC_UPDATEFRAME(param, pInfo);
        break;
    }

    case G3DPROC_CHILD_DETACHED: {
        RemoveAnmObj(static_cast<AnmObj*>(pInfo));
        break;
    }

    default: {
        DefG3dProcScnLeaf(task, param, pInfo);
        break;
    }
    }
}

bool ScnMdlSimple::SetScnObjOption(u32 option, u32 value) {
    switch (option) {
    case OPTION_IGNORE_ANMCHR_TRANS: {
        SetScnObjFlag(SCNOBJFLAG_IGNORE_ANMCHR_TRANS, value);
        break;
    }

    default: {
        return ScnLeaf::SetScnObjOption(option, value);
    }
    }

    return true;
}

bool ScnMdlSimple::GetScnObjOption(u32 option, u32* pValue) const {
    if (pValue == NULL) {
        return false;
    }

    switch (option) {
    case OPTION_IGNORE_ANMCHR_TRANS: {
        *pValue = TestScnObjFlag(SCNOBJFLAG_IGNORE_ANMCHR_TRANS);
        break;
    }

    default: {
        return ScnLeaf::GetScnObjOption(option, pValue);
    }
    }

    return true;
}

bool ScnMdlSimple::GetScnMtxPos(math::MTX34* pMtx, ScnObjMtxType type,
                                u32 idx) const {
    if (pMtx != NULL) {
        ResNode node = GetResMdl().GetResNode(idx);

        if (node.IsValid()) {
            s32 id = node.GetMtxID();

            switch (type) {
            case MTX_WORLD: {
                math::MTX34Copy(pMtx, &mpWorldMtxArray[id]);
                return true;
            }

            case MTX_VIEW: {
                if (id < GetResMdl().GetResMdlInfo().GetNumViewMtx()) {
                    math::MTX34Copy(pMtx, &GetViewPosMtxArray()[id]);
                    return true;
                }

                const math::MTX34* pWorld = GetMtxPtr(MTX_WORLD);
                const math::MTX34* pView = GetMtxPtr(MTX_VIEW);

                math::MTX34 work0;
                math::MTX34 work1;

                if (math::MTX34Inv(&work0, pWorld) != FALSE) {
                    math::MTX34Mult(&work1, pView, &work0);
                    math::MTX34Mult(pMtx, &work1, &mpWorldMtxArray[id]);
                    return true;
                }
            }

            default: {
                break;
            }
            }
        }
    }

    return false;
}

bool ScnMdlSimple::SetAnmObj(AnmObj* pObj, AnmObjType type) {
    if (pObj == NULL || pObj->GetParent() != NULL) {
        return false;
    }

    AnmObjChr* pChr = NULL;
    AnmObjVis* pVis = NULL;
    AnmObjMatClr* pClr = NULL;
    AnmObjTexPat* pPat = NULL;
    AnmObjTexSrt* pSrt = NULL;

    switch (type) {
    case ANMOBJTYPE_CHR: {
        if ((pChr = DynamicCast<AnmObjChr>(pObj)) != NULL) {
            goto _type_chr;
        }

        goto _bad_cast;
    }

    case ANMOBJTYPE_VIS: {
        if ((pVis = DynamicCast<AnmObjVis>(pObj)) != NULL) {
            goto _type_vis;
        }

        goto _bad_cast;
    }

    case ANMOBJTYPE_MATCLR: {
        if ((pClr = DynamicCast<AnmObjMatClr>(pObj)) != NULL) {
            goto _type_clr;
        }

        goto _bad_cast;
    }

    case ANMOBJTYPE_TEXPAT: {
        if ((pPat = DynamicCast<AnmObjTexPat>(pObj)) != NULL) {
            goto _type_pat;
        }

        goto _bad_cast;
    }

    case ANMOBJTYPE_TEXSRT: {
        if ((pSrt = DynamicCast<AnmObjTexSrt>(pObj)) != NULL) {
            goto _type_srt;
        }

        goto _bad_cast;
    }

    case ANMOBJTYPE_NOT_SPECIFIED: {
        if ((pChr = DynamicCast<AnmObjChr>(pObj)) != NULL) {
            goto _type_chr;
        }
        if ((pVis = DynamicCast<AnmObjVis>(pObj)) != NULL) {
            goto _type_vis;
        }
        if ((pClr = DynamicCast<AnmObjMatClr>(pObj)) != NULL) {
            goto _type_clr;
        }
        if ((pPat = DynamicCast<AnmObjTexPat>(pObj)) != NULL) {
            goto _type_pat;
        }
        if ((pSrt = DynamicCast<AnmObjTexSrt>(pObj)) != NULL) {
            goto _type_srt;
        }

        // FALLTHROUGH
    }

    _bad_cast:
    case ANMOBJTYPE_SHP:
    default: {
        return false;
    }
    }

_type_chr:
    if (!pChr->IsBound()) {
        return false;
    }

    if (mpAnmObjChr != NULL) {
        RemoveAnmObj(mpAnmObjChr);
    }

    mpAnmObjChr = pChr;
    pChr->G3dProc(G3DPROC_ATTACH_PARENT, 0, this);
    return true;

_type_vis:
    if (!pVis->IsBound()) {
        return false;
    }

    if (mpAnmObjVis != NULL) {
        RemoveAnmObj(mpAnmObjVis);
    }

    mpAnmObjVis = pVis;
    pVis->G3dProc(G3DPROC_ATTACH_PARENT, 0, this);
    return true;

_type_clr:
    if (!pClr->IsBound()) {
        return false;
    }

    if (mpAnmObjMatClr != NULL) {
        RemoveAnmObj(mpAnmObjMatClr);
    }

    mpAnmObjMatClr = pClr;
    pClr->G3dProc(G3DPROC_ATTACH_PARENT, 0, this);
    return true;

_type_pat:
    if (!pPat->IsBound()) {
        return false;
    }

    if (mpAnmObjTexPat != NULL) {
        RemoveAnmObj(mpAnmObjTexPat);
    }

    mpAnmObjTexPat = pPat;
    pPat->G3dProc(G3DPROC_ATTACH_PARENT, 0, this);
    return true;

_type_srt:
    if (!pSrt->IsBound()) {
        return false;
    }

    if (mpAnmObjTexSrt != NULL) {
        RemoveAnmObj(mpAnmObjTexSrt);
    }

    mpAnmObjTexSrt = pSrt;
    pSrt->G3dProc(G3DPROC_ATTACH_PARENT, 0, this);
    return true;
}

bool ScnMdlSimple::RemoveAnmObj(AnmObj* pObj) {
    if (pObj == NULL) {
        return false;
    }

    if (pObj == mpAnmObjChr) {
        mpAnmObjChr->G3dProc(G3DPROC_DETACH_PARENT, 0, this);
        mpAnmObjChr = NULL;
        return true;
    }

    if (pObj == mpAnmObjVis) {
        mpAnmObjVis->G3dProc(G3DPROC_DETACH_PARENT, 0, this);
        mpAnmObjVis = NULL;
        return true;
    }

    if (pObj == mpAnmObjMatClr) {
        mpAnmObjMatClr->G3dProc(G3DPROC_DETACH_PARENT, 0, this);
        mpAnmObjMatClr = NULL;
        return true;
    }

    if (pObj == mpAnmObjTexPat) {
        mpAnmObjTexPat->G3dProc(G3DPROC_DETACH_PARENT, 0, this);
        mpAnmObjTexPat = NULL;
        return true;
    }

    if (pObj == mpAnmObjTexSrt) {
        mpAnmObjTexSrt->G3dProc(G3DPROC_DETACH_PARENT, 0, this);
        mpAnmObjTexSrt = NULL;
        return true;
    }

    return false;
}

AnmObj* ScnMdlSimple::RemoveAnmObj(AnmObjType type) {
    AnmObj* pObj = NULL;

    switch (type) {
    case ANMOBJTYPE_CHR: {
        pObj = mpAnmObjChr;
        RemoveAnmObj(mpAnmObjChr);
        break;
    }

    case ANMOBJTYPE_VIS: {
        pObj = mpAnmObjVis;
        RemoveAnmObj(mpAnmObjVis);
        break;
    }

    case ANMOBJTYPE_MATCLR: {
        pObj = mpAnmObjMatClr;
        RemoveAnmObj(mpAnmObjMatClr);
        break;
    }

    case ANMOBJTYPE_TEXPAT: {
        pObj = mpAnmObjTexPat;
        RemoveAnmObj(mpAnmObjTexPat);
        break;
    }

    case ANMOBJTYPE_TEXSRT: {
        pObj = mpAnmObjTexSrt;
        RemoveAnmObj(mpAnmObjTexSrt);
        break;
    }

    case ANMOBJTYPE_SHP:
    case ANMOBJTYPE_NOT_SPECIFIED:
    default: {
        break;
    }
    }

    return pObj;
}

AnmObj* ScnMdlSimple::GetAnmObj(AnmObjType type) {
    switch (type) {
    case ANMOBJTYPE_CHR: {
        return mpAnmObjChr;
    }

    case ANMOBJTYPE_VIS: {
        return mpAnmObjVis;
    }

    case ANMOBJTYPE_MATCLR: {
        return mpAnmObjMatClr;
    }

    case ANMOBJTYPE_TEXPAT: {
        return mpAnmObjTexPat;
    }

    case ANMOBJTYPE_TEXSRT: {
        return mpAnmObjTexSrt;
    }

    case ANMOBJTYPE_SHP:
    case ANMOBJTYPE_NOT_SPECIFIED:
    default: {
        return NULL;
    }
    }
}

const AnmObj* ScnMdlSimple::GetAnmObj(AnmObjType type) const {
    switch (type) {
    case ANMOBJTYPE_CHR: {
        return mpAnmObjChr;
    }

    case ANMOBJTYPE_VIS: {
        return mpAnmObjVis;
    }

    case ANMOBJTYPE_MATCLR: {
        return mpAnmObjMatClr;
    }

    case ANMOBJTYPE_TEXPAT: {
        return mpAnmObjTexPat;
    }

    case ANMOBJTYPE_TEXSRT: {
        return mpAnmObjTexSrt;
    }

    case ANMOBJTYPE_SHP:
    case ANMOBJTYPE_NOT_SPECIFIED:
    default: {
        return NULL;
    }
    }
}

void ScnMdlSimple::UpdateFrame() {
    if (mpAnmObjChr != NULL) {
        mpAnmObjChr->UpdateFrame();
    }

    if (mpAnmObjVis != NULL) {
        mpAnmObjVis->UpdateFrame();
    }

    if (mpAnmObjMatClr != NULL) {
        mpAnmObjMatClr->UpdateFrame();
    }

    if (mpAnmObjTexPat != NULL) {
        mpAnmObjTexPat->UpdateFrame();
    }

    if (mpAnmObjTexSrt != NULL) {
        mpAnmObjTexSrt->UpdateFrame();
    }
}

void ScnMdlSimple::EnableScnMdlCallbackTiming(Timing timing) {
    if (timing & CALLBACK_TIMING_A) {
        mCwcbTiming |= CALLBACK_TIMING_A;
    }

    if (timing & CALLBACK_TIMING_B) {
        mCwcbTiming |= CALLBACK_TIMING_B;
    }

    if (timing & CALLBACK_TIMING_C) {
        mCwcbTiming |= CALLBACK_TIMING_C;
    }
}

void ScnMdlSimple::DisableScnMdlCallbackTiming(Timing timing) {
    if (timing & CALLBACK_TIMING_A) {
        mCwcbTiming &= ~CALLBACK_TIMING_A;
    }

    if (timing & CALLBACK_TIMING_B) {
        mCwcbTiming &= ~CALLBACK_TIMING_B;
    }

    if (timing & CALLBACK_TIMING_C) {
        mCwcbTiming &= ~CALLBACK_TIMING_C;
    }
}

math::MTX34* ScnMdlSimple::GetViewPosMtxArray() {
    u8* pBase = reinterpret_cast<u8*>(mpViewPosMtxArray);

    return reinterpret_cast<math::MTX34*>(
        pBase + mCurView * align32(mNumViewMtx * sizeof(math::MTX34)));
}

const math::MTX34* ScnMdlSimple::GetViewPosMtxArray() const {
    u8* pBase = reinterpret_cast<u8*>(mpViewPosMtxArray);

    return reinterpret_cast<math::MTX34*>(
        pBase + mCurView * align32(mNumViewMtx * sizeof(math::MTX34)));
}

math::MTX33* ScnMdlSimple::GetViewNrmMtxArray() {
    if (mpViewNrmMtxArray != NULL) {
        u8* pBase = reinterpret_cast<u8*>(mpViewNrmMtxArray);

        return reinterpret_cast<math::MTX33*>(
            pBase + mCurView * align32(mNumViewMtx * sizeof(math::MTX33)));
    }

    return NULL;
}

const math::MTX33* ScnMdlSimple::GetViewNrmMtxArray() const {
    if (mpViewNrmMtxArray != NULL) {
        u8* pBase = reinterpret_cast<u8*>(mpViewNrmMtxArray);

        return reinterpret_cast<math::MTX33*>(
            pBase + mCurView * align32(mNumViewMtx * sizeof(math::MTX33)));
    }

    return NULL;
}

math::MTX34* ScnMdlSimple::GetViewTexMtxArray() {
    if (mpViewTexMtxArray != NULL) {
        u8* pBase = reinterpret_cast<u8*>(mpViewTexMtxArray);

        return reinterpret_cast<math::MTX34*>(
            pBase + mCurView * align32(mNumViewMtx * sizeof(math::MTX34)));
    }

    return NULL;
}

const math::MTX34* ScnMdlSimple::GetViewTexMtxArray() const {
    if (mpViewTexMtxArray != NULL) {
        u8* pBase = reinterpret_cast<u8*>(mpViewTexMtxArray);

        return reinterpret_cast<math::MTX34*>(
            pBase + mCurView * align32(mNumViewMtx * sizeof(math::MTX34)));
    }

    return NULL;
}

const u8* ScnMdlSimple::GetByteCode(ByteCodeType type) const {
    switch (type) {
    case BYTE_CODE_CALC: {
        return mpByteCodeCalc;
    }

    case BYTE_CODE_MIX: {
        return mpByteCodeMix;
    }

    case BYTE_CODE_DRAW_OPA: {
        return mpByteCodeDrawOpa;
    }

    case BYTE_CODE_DRAW_XLU: {
        return mpByteCodeDrawXlu;
    }

    default: {
        return NULL;
    }
    }
}

ScnMdlSimple::ScnMdlSimple(MEMAllocator* pAllocator, ResMdl mdl,
                           math::MTX34* pWorldMtxArray,
                           u32* pWorldMtxAttribArray,
                           math::MTX34* pViewPosMtxArray,
                           math::MTX33* pViewNrmMtxArray,
                           math::MTX34* pViewTexMtxArray, int numView,
                           int numViewMtx)
    : ScnLeaf(pAllocator),
      mResMdl(mdl),
      mpWorldMtxArray(pWorldMtxArray),
      mpWorldMtxAttribArray(pWorldMtxAttribArray),
      mpViewPosMtxArray(pViewPosMtxArray),
      mpViewNrmMtxArray(pViewNrmMtxArray),
      mpViewTexMtxArray(pViewTexMtxArray),
      mNumView(numView),
      mCurView(0),
      mNumViewMtx(numViewMtx),
      mFlagScnMdlSimple(0),
      mpByteCodeCalc(mdl.GetResByteCode("NodeTree")),
      mpByteCodeMix(mdl.GetResByteCode("NodeMix")),
      mpByteCodeDrawOpa(mdl.GetResByteCode("DrawOpa")),
      mpByteCodeDrawXlu(mdl.GetResByteCode("DrawXlu")),
      mDrawMode(RESMDL_DRAWMODE_DEFAULT),
      mpCalcWorldCallback(NULL),
      mCwcbTiming(0),
      mCwcbDeleteOption(FALSE),
      mCwcbNodeID(0),
      mpAnmObjChr(NULL),
      mpAnmObjVis(NULL),
      mpAnmObjMatClr(NULL),
      mpAnmObjTexPat(NULL),
      mpAnmObjTexSrt(NULL) {

    if (mpByteCodeDrawOpa != NULL) {
        SetScnObjFlag(SCNOBJFLAG_NOT_GATHER_DRAW_OPA, false);
    } else {
        SetScnObjFlag(SCNOBJFLAG_NOT_GATHER_DRAW_OPA, true);
    }

    if (mpByteCodeDrawXlu != NULL) {
        SetScnObjFlag(SCNOBJFLAG_NOT_GATHER_DRAW_XLU, false);
    } else {
        SetScnObjFlag(SCNOBJFLAG_NOT_GATHER_DRAW_XLU, true);
    }

    if (mpViewPosMtxArray != NULL) {
        DC::InvalidateRange(mpViewPosMtxArray,
                            numView *
                                align32(numViewMtx * sizeof(math::MTX34)));
    }

    if (mpViewNrmMtxArray != NULL) {
        DC::InvalidateRange(mpViewNrmMtxArray,
                            numView *
                                align32(numViewMtx * sizeof(math::MTX33)));
    }

    if (mpViewTexMtxArray != NULL) {
        DC::InvalidateRange(mpViewTexMtxArray,
                            numView *
                                align32(numViewMtx * sizeof(math::MTX34)));
    }

    if (mdl.GetResMdlInfo().ref().is_valid_volume) {
        const math::_VEC3& rMin = mdl.GetResMdlInfo().ref().volume_min;
        const math::_VEC3& rMax = mdl.GetResMdlInfo().ref().volume_max;

        math::AABB box;
        box.min = static_cast<const math::VEC3&>(rMin);
        box.max = static_cast<const math::VEC3&>(rMax);

        SetBoundingVolume(&box);
    }
}

ScnMdlSimple::~ScnMdlSimple() {
    switch (mCwcbDeleteOption) {
    case TRUE: {
        delete mpCalcWorldCallback;
        break;
    }
    }

    if (mpAnmObjChr != NULL) {
        RemoveAnmObj(mpAnmObjChr);
    }

    if (mpAnmObjVis != NULL) {
        RemoveAnmObj(mpAnmObjVis);
    }

    if (mpAnmObjMatClr != NULL) {
        RemoveAnmObj(mpAnmObjMatClr);
    }

    if (mpAnmObjTexPat != NULL) {
        RemoveAnmObj(mpAnmObjTexPat);
    }

    if (mpAnmObjTexSrt != NULL) {
        RemoveAnmObj(mpAnmObjTexSrt);
    }
}

} // namespace g3d
} // namespace nw4r
