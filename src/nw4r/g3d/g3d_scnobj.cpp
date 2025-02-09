#include <nw4r/g3d.h>

#include <nw4r/math.h>

#include <algorithm>

namespace nw4r {
namespace g3d {

NW4R_G3D_RTTI_DEF(ScnObj);
NW4R_G3D_RTTI_DEF(ScnLeaf);
NW4R_G3D_RTTI_DEF(ScnGroup);

const math::FRUSTUM* gpCullingFrustum = NULL;

/******************************************************************************
 *
 * ScnObj
 *
 ******************************************************************************/
void ScnObj::CalcWorldMtx(const math::MTX34* pParent, u32* pParam) {
    if (pParam != NULL && (*pParam & SCNOBJFLAG_DISABLE_CALC_WORLD)) {
        *pParam &= ~SCNOBJFLAG_DISABLE_CALC_WORLD;
        return;
    }

    if (pParent != NULL) {
        if (TestScnObjFlag(SCNOBJFLAG_MTX_LOCAL_IDENTITY)) {
            math::MTX34Copy(&mMtxArray[MTX_WORLD], pParent);
        } else {
            math::MTX34Mult(&mMtxArray[MTX_WORLD], pParent,
                            &mMtxArray[MTX_LOCAL]);
        }
    } else {
        math::MTX34Copy(&mMtxArray[MTX_WORLD], &mMtxArray[MTX_LOCAL]);
    }

    if (TestScnObjFlag(SCNOBJFLAG_ENABLE_CULLING)) {
        mAABB[BOUNDINGVOLUME_AABB_WORLD].Set(&mAABB[BOUNDINGVOLUME_AABB_LOCAL],
                                             &mMtxArray[MTX_WORLD]);
    }
}

void ScnObj::CalcViewMtx(const math::MTX34* pCamera) {
    math::MTX34Mult(&mMtxArray[MTX_VIEW], pCamera, &mMtxArray[MTX_WORLD]);
}

ScnObj::ScnObj(MEMAllocator* pAllocator)
    : G3dObj(pAllocator, NULL),
      mScnObjFlags(0),
      mPriorityDrawOpa(128),
      mPriorityDrawXlu(128),
      PADDING_0xD2(0),
      PADDING_0xD3(0),
      mpFuncObjExec(NULL),
      mCallbackTiming(0),
      mCallbackDeleteOption(FALSE),
      mCallbackExecOpMask(0) {

    SetScnObjFlag(SCNOBJFLAG_MTX_LOCAL_IDENTITY, TRUE);

    math::MTX34Identity(&mMtxArray[MTX_LOCAL]);
    math::MTX34Identity(&mMtxArray[MTX_WORLD]);
    math::MTX34Identity(&mMtxArray[MTX_VIEW]);

    mAABB[BOUNDINGVOLUME_AABB_LOCAL].min = math::VEC3(0.0f, 0.0f, 0.0f);
    mAABB[BOUNDINGVOLUME_AABB_LOCAL].max = math::VEC3(0.0f, 0.0f, 0.0f);
    mAABB[BOUNDINGVOLUME_AABB_WORLD].min = math::VEC3(0.0f, 0.0f, 0.0f);
    mAABB[BOUNDINGVOLUME_AABB_WORLD].max = math::VEC3(0.0f, 0.0f, 0.0f);
}

ScnObj::~ScnObj() {
    if (mpFuncObjExec == NULL) {
        return;
    }

    switch (mCallbackDeleteOption) {
    case TRUE: {
        delete mpFuncObjExec;
        break;
    }
    }
}

bool ScnObj::SetScnObjOption(u32 option, u32 value) {
    switch (option) {
    case OPTION_DISABLE_GATHER_SCNOBJ: {
        SetScnObjFlag(SCNOBJFLAG_DISABLE_GATHER_SCNOBJ, value);
        break;
    }

    case OPTION_DISABLE_CALC_WORLD: {
        SetScnObjFlag(SCNOBJFLAG_DISABLE_CALC_WORLD, value);
        break;
    }

    case OPTION_DISABLE_CALC_MAT: {
        SetScnObjFlag(SCNOBJFLAG_DISABLE_CALC_MAT, value);
        break;
    }

    case OPTION_DISABLE_CALC_VTX: {
        SetScnObjFlag(SCNOBJFLAG_DISABLE_CALC_VTX, value);
        break;
    }

    case OPTION_DISABLE_CALC_VIEW: {
        SetScnObjFlag(SCNOBJFLAG_DISABLE_CALC_VIEW, value);
        break;
    }

    case OPTION_DISABLE_DRAW_OPA: {
        SetScnObjFlag(SCNOBJFLAG_DISABLE_DRAW_OPA, value);
        break;
    }

    case OPTION_DISABLE_DRAW_XLU: {
        SetScnObjFlag(SCNOBJFLAG_DISABLE_DRAW_XLU, value);
        break;
    }

    case OPTION_DISABLE_UPDATEFRAME: {
        SetScnObjFlag(SCNOBJFLAG_DISABLE_UPDATEFRAME, value);
        break;
    }

    case OPTION_ENABLE_CULLING: {
        SetScnObjFlag(SCNOBJFLAG_ENABLE_CULLING, value);
        break;
    }

    default: {
        return false;
    }
    }

    return true;
}

bool ScnObj::GetScnObjOption(u32 option, u32* pValue) const {
    if (pValue == NULL) {
        return false;
    }

    switch (option) {
    case OPTION_DISABLE_GATHER_SCNOBJ: {
        *pValue = TestScnObjFlag(SCNOBJFLAG_DISABLE_GATHER_SCNOBJ);
        break;
    }

    case OPTION_DISABLE_CALC_WORLD: {
        *pValue = TestScnObjFlag(SCNOBJFLAG_DISABLE_CALC_WORLD);
        break;
    }

    case OPTION_DISABLE_CALC_MAT: {
        *pValue = TestScnObjFlag(SCNOBJFLAG_DISABLE_CALC_MAT);
        break;
    }

    case OPTION_DISABLE_CALC_VTX: {
        *pValue = TestScnObjFlag(SCNOBJFLAG_DISABLE_CALC_VTX);
        break;
    }

    case OPTION_DISABLE_CALC_VIEW: {
        *pValue = TestScnObjFlag(SCNOBJFLAG_DISABLE_CALC_VIEW);
        break;
    }

    case OPTION_DISABLE_DRAW_OPA: {
        *pValue = TestScnObjFlag(SCNOBJFLAG_DISABLE_DRAW_OPA);
        break;
    }

    case OPTION_DISABLE_DRAW_XLU: {
        *pValue = TestScnObjFlag(SCNOBJFLAG_DISABLE_DRAW_XLU);
        break;
    }

    case OPTION_DISABLE_UPDATEFRAME: {
        *pValue = TestScnObjFlag(SCNOBJFLAG_DISABLE_UPDATEFRAME);
        break;
    }

    case OPTION_ENABLE_CULLING: {
        *pValue = TestScnObjFlag(SCNOBJFLAG_ENABLE_CULLING);
        break;
    }

    default: {
        return false;
    }
    }

    return true;
}

bool ScnObj::SetMtx(ScnObjMtxType type, const math::MTX34* pMtx) {
    if (static_cast<u32>(type) < MTX_TYPE_MAX) {
        if (pMtx != NULL) {
            if (type == MTX_LOCAL) {
                SetScnObjFlag(SCNOBJFLAG_MTX_LOCAL_IDENTITY, FALSE);
            }

            math::MTX34Copy(&mMtxArray[type], pMtx);
        } else {
            if (type == MTX_LOCAL) {
                SetScnObjFlag(SCNOBJFLAG_MTX_LOCAL_IDENTITY, TRUE);
            }

            math::MTX34Identity(&mMtxArray[type]);
        }

        return true;
    }

    return false;
}

bool ScnObj::GetMtx(ScnObjMtxType type, math::MTX34* pMtx) const {
    if (pMtx != NULL && static_cast<u32>(type) < MTX_TYPE_MAX) {
        math::MTX34Copy(pMtx, &mMtxArray[type]);
        return true;
    }

    return false;
}

f32 ScnObj::GetValueForSortOpa() const {
    return -mMtxArray[MTX_VIEW]._23;
}

f32 ScnObj::GetValueForSortXlu() const {
    return mMtxArray[MTX_VIEW]._23;
}

void ScnObj::SetPriorityDrawOpa(int prio) {
    if (prio < 0) {
        prio = 0;
    } else if (prio > 255) {
        prio = 255;
    }

    mPriorityDrawOpa = prio;
}

void ScnObj::SetPriorityDrawXlu(int prio) {
    if (prio < 0) {
        prio = 0;
    } else if (prio > 255) {
        prio = 255;
    }

    mPriorityDrawXlu = prio;
}

void ScnObj::EnableScnObjCallbackTiming(Timing timing) {
    if (timing & CALLBACK_TIMING_A) {
        mCallbackTiming |= CALLBACK_TIMING_A;
    }

    if (timing & CALLBACK_TIMING_B) {
        mCallbackTiming |= CALLBACK_TIMING_B;
    }

    if (timing & CALLBACK_TIMING_C) {
        mCallbackTiming |= CALLBACK_TIMING_C;
    }
}

void ScnObj::EnableScnObjCallbackExecOp(ExecOp op) {
    mCallbackExecOpMask |= static_cast<u16>(op);
}

bool ScnObj::SetBoundingVolume(ScnObjBoundingVolumeType type,
                               const math::AABB* pAABB) {
    if (pAABB != NULL) {
        if (type < BOUNDINGVOLUME_MAX) {
            mAABB[type] = *pAABB;
            return SetScnObjOption(OPTION_ENABLE_CULLING, TRUE);
        }

        return false;
    }

    return SetScnObjOption(OPTION_ENABLE_CULLING, FALSE);
}

bool ScnObj::GetBoundingVolume(ScnObjBoundingVolumeType type,
                               math::AABB* pAABB) const {
    if (pAABB != NULL) {
        if (type < BOUNDINGVOLUME_MAX) {
            *pAABB = mAABB[type];
            return true;
        }

        return false;
    }

    return false;
}

/******************************************************************************
 *
 * ScnLeaf
 *
 ******************************************************************************/
ScnObj::ForEachResult ScnLeaf::ForEach(ForEachFunc pFunc, void* pInfo,
                                       bool postOrder) {
#pragma unused(postOrder)

    return pFunc(this, pInfo) != FOREACHRESULT_CONTINUE
               ? FOREACHRESULT_OK
               : FOREACHRESULT_CONTINUE;
}

bool ScnLeaf::SetScnObjOption(u32 option, u32 value) {
    switch (option) {
    case OPTION_DISABLE_DRAW_ALL: {
        SetScnObjFlag(SCNOBJFLAG_DISABLE_DRAW, value);
        break;
    }

    default: {
        return ScnObj::SetScnObjOption(option, value);
    }
    }

    return true;
}

bool ScnLeaf::GetScnObjOption(u32 option, u32* pValue) const {
    if (pValue == NULL) {
        return false;
    }

    switch (option) {
    case OPTION_DISABLE_DRAW_ALL: {
        *pValue = TestScnObjFlag(SCNOBJFLAG_DISABLE_DRAW);
        break;
    }

    default: {
        return ScnObj::GetScnObjOption(option, pValue);
    }
    }

    return true;
}

void ScnLeaf::CalcWorldMtx(const math::MTX34* pParent, u32* pParam) {
    if (pParam != NULL && (*pParam & SCNOBJFLAG_DISABLE_CALC_WORLD)) {
        *pParam &= ~SCNOBJFLAG_DISABLE_CALC_WORLD;
        return;
    }

    ScnObj::CalcWorldMtx(pParent, pParam);
    math::MTX34Scale(&mMtxArray[MTX_WORLD], &mMtxArray[MTX_WORLD], &mScale);

    if (TestScnObjFlag(SCNOBJFLAG_ENABLE_CULLING)) {
        mAABB[BOUNDINGVOLUME_AABB_WORLD].Set(&mAABB[BOUNDINGVOLUME_AABB_LOCAL],
                                             &mMtxArray[MTX_WORLD]);
    }
}

ScnLeaf::ScaleProperty ScnLeaf::GetScaleProperty() const {
    if (mScale.x == mScale.y) {
        if (mScale.y == mScale.z) {
            if (mScale.x == 1.0f) {
                return NOT_SCALED;
            }

            return UNIFORM_SCALED;
        }
    }

    return NONUNIFORM_SCALED;
}

void ScnLeaf::DefG3dProcScnLeaf(u32 task, u32 param, void* pInfo) {
    switch (task) {
    case G3DPROC_CALC_WORLD: {
        CheckCallback_CALC_WORLD(CALLBACK_TIMING_A, param, pInfo);
        CalcWorldMtx(static_cast<math::MTX34*>(pInfo), &param);
        CheckCallback_CALC_WORLD(CALLBACK_TIMING_B, param, pInfo);
        CheckCallback_CALC_WORLD(CALLBACK_TIMING_C, param, pInfo);
        break;
    }

    case G3DPROC_CALC_MAT: {
        CheckCallback_CALC_MAT(CALLBACK_TIMING_A, param, pInfo);
        CheckCallback_CALC_MAT(CALLBACK_TIMING_C, param, pInfo);
        break;
    }

    case G3DPROC_CALC_VIEW: {
        CheckCallback_CALC_VIEW(CALLBACK_TIMING_A, param, pInfo);
        CalcViewMtx(static_cast<math::MTX34*>(pInfo));
        CheckCallback_CALC_VIEW(CALLBACK_TIMING_B, param, pInfo);
        CheckCallback_CALC_VIEW(CALLBACK_TIMING_C, param, pInfo);
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

    case G3DPROC_GATHER_SCNOBJ:
    case G3DPROC_DRAW_OPA:
    case G3DPROC_DRAW_XLU:
    case G3DPROC_UPDATEFRAME:
    case G3DPROC_CHILD_DETACHED:
    case G3DPROC_ZSORT: {
        break;
    }
    }
}

/******************************************************************************
 *
 * ScnGroup
 *
 ******************************************************************************/
ScnObj::ForEachResult ScnGroup::ForEach(ForEachFunc pFunc, void* pInfo,
                                        bool postOrder) {
    ForEachResult result;

    if (postOrder) {
        for (u32 i = 0; i < Size(); i++) {
            result = mpScnObjArray[i]->ForEach(pFunc, pInfo, false);

            if (result == FOREACHRESULT_CONTINUE) {
                return FOREACHRESULT_CONTINUE;
            }
        }

        result = pFunc(this, pInfo);
        return result == FOREACHRESULT_CONTINUE ? FOREACHRESULT_CONTINUE
                                                : FOREACHRESULT_OK;
    } else {
        result = pFunc(this, pInfo);

        if (result == FOREACHRESULT_OK) {
            for (u32 i = 0; i < Size(); i++) {
                result = mpScnObjArray[i]->ForEach(pFunc, pInfo, false);

                if (result == FOREACHRESULT_CONTINUE) {
                    return FOREACHRESULT_CONTINUE;
                }
            }

            return FOREACHRESULT_OK;
        }

        return result == FOREACHRESULT_CONTINUE ? FOREACHRESULT_CONTINUE
                                                : FOREACHRESULT_OK;
    }
}

void ScnGroup::ScnGroup_G3DPROC_GATHER_SCNOBJ(u32 param,
                                              IScnObjGather* pCollection) {
    IScnObjGather::CullingStatus status =
        pCollection->Add(this, !TestScnObjFlag(SCNOBJFLAG_NOT_GATHER_DRAW_OPA),
                         !TestScnObjFlag(SCNOBJFLAG_NOT_GATHER_DRAW_XLU));

    if (status == IScnObjGather::CULLINGSTATUS_INTERSECT) {
        for (u32 i = 0; i < mNumScnObj; i++) {
            mpScnObjArray[i]->G3dProc(G3DPROC_GATHER_SCNOBJ, param,
                                      pCollection);
        }
    } else if (status == IScnObjGather::CULLINGSTATUS_INSIDE) {
        const math::FRUSTUM* pTemp = gpCullingFrustum;
        gpCullingFrustum = NULL;
        {
            for (u32 i = 0; i < mNumScnObj; i++) {
                mpScnObjArray[i]->G3dProc(G3DPROC_GATHER_SCNOBJ, param,
                                          pCollection);
            }
        }
        gpCullingFrustum = pTemp;
    }
}

void ScnGroup::ScnGroup_G3DPROC_CALC_WORLD(u32 param,
                                           const math::MTX34* pParent) {
    CheckCallback_CALC_WORLD(CALLBACK_TIMING_A, param,
                             const_cast<math::MTX34*>(pParent));

    CalcWorldMtx(pParent, &param);

    CheckCallback_CALC_WORLD(CALLBACK_TIMING_B, param,
                             const_cast<math::MTX34*>(pParent));

    math::MTX34* pWorldMtx = const_cast<math::MTX34*>(GetMtxPtr(MTX_WORLD));

    for (u32 i = 0; i < mNumScnObj; i++) {
        mpScnObjArray[i]->G3dProc(G3DPROC_CALC_WORLD, param, pWorldMtx);
    }

    CheckCallback_CALC_WORLD(CALLBACK_TIMING_C, param,
                             const_cast<math::MTX34*>(pParent));
}

void ScnGroup::ScnGroup_G3DPROC_CALC_MAT(u32 param, void* pInfo) {
    CheckCallback_CALC_MAT(CALLBACK_TIMING_A, param, pInfo);

    for (u32 i = 0; i < mNumScnObj; i++) {
        mpScnObjArray[i]->G3dProc(G3DPROC_CALC_MAT, param, pInfo);
    }

    CheckCallback_CALC_MAT(CALLBACK_TIMING_C, param, pInfo);
}

void ScnGroup::ScnGroup_G3DPROC_CALC_VIEW(u32 param,
                                          const math::MTX34* pCamera) {
    CheckCallback_CALC_VIEW(CALLBACK_TIMING_A, param,
                            const_cast<math::MTX34*>(pCamera));

    CalcViewMtx(pCamera);

    CheckCallback_CALC_VIEW(CALLBACK_TIMING_B, param,
                            const_cast<math::MTX34*>(pCamera));

    for (u32 i = 0; i < mNumScnObj; i++) {
        mpScnObjArray[i]->G3dProc(G3DPROC_CALC_VIEW, param,
                                  const_cast<math::MTX34*>(pCamera));
    }

    CheckCallback_CALC_VIEW(CALLBACK_TIMING_C, param,
                            const_cast<math::MTX34*>(pCamera));
}

void ScnGroup::G3dProc(u32 task, u32 param, void* pInfo) {
    if (IsG3dProcDisabled(task)) {
        return;
    }

    DefG3dProcScnGroup(task, param, pInfo);
}

void ScnGroup::DefG3dProcScnGroup(u32 task, u32 param, void* pInfo) {
    switch (task) {
    case G3DPROC_GATHER_SCNOBJ: {
        ScnGroup_G3DPROC_GATHER_SCNOBJ(param,
                                       static_cast<IScnObjGather*>(pInfo));
        break;
    }

    case G3DPROC_CALC_WORLD: {
        ScnGroup_G3DPROC_CALC_WORLD(param, static_cast<math::MTX34*>(pInfo));
        break;
    }

    case G3DPROC_CALC_MAT: {
        ScnGroup_G3DPROC_CALC_MAT(param, pInfo);
        break;
    }

    case G3DPROC_CALC_VIEW: {
        ScnGroup_G3DPROC_CALC_VIEW(param, static_cast<math::MTX34*>(pInfo));
        break;
    }

    case G3DPROC_DRAW_OPA:
    case G3DPROC_DRAW_XLU: {
        return;
    }

    case G3DPROC_CHILD_DETACHED: {
        Remove(static_cast<ScnObj*>(pInfo));
        break;
    }

    case G3DPROC_DETACH_PARENT: {
        SetParent(NULL);
        break;
    }

    case G3DPROC_ATTACH_PARENT: {
        SetParent(static_cast<ScnObj*>(pInfo));
        break;
    }

    case G3DPROC_UPDATEFRAME:
    case G3DPROC_ZSORT:
    default: {
        for (u32 i = 0; i < mNumScnObj; i++) {
            mpScnObjArray[i]->G3dProc(task, param, pInfo);
        }
        break;
    }
    }
}

bool ScnGroup::Insert(u32 idx, ScnObj* pObj) {
    if (idx <= mNumScnObj && mNumScnObj < mSizeScnObj && pObj != NULL &&
        pObj->GetParent() == NULL) {

        ScnObj** ppObj =
            std::find(mpScnObjArray, mpScnObjArray + mNumScnObj, pObj);

        if (ppObj == mpScnObjArray + mNumScnObj) {
            for (u32 i = mNumScnObj; i > idx; i--) {
                mpScnObjArray[i] = mpScnObjArray[i - 1];
            }

            mpScnObjArray[idx] = pObj;
            pObj->G3dProc(G3DPROC_ATTACH_PARENT, 0, this);

            mNumScnObj++;
            return true;
        }
    }

    return false;
}

ScnObj* ScnGroup::Remove(u32 idx) {
    if (idx < mNumScnObj) {
        ScnObj* pObj = mpScnObjArray[idx];
        pObj->G3dProc(G3DPROC_DETACH_PARENT, 0, this);

        for (u32 i = idx; i < mNumScnObj - 1; i++) {
            mpScnObjArray[i] = mpScnObjArray[i + 1];
        }

        mNumScnObj--;
        return pObj;
    }

    return NULL;
}

bool ScnGroup::Remove(ScnObj* pObj) {
    ScnObj** ppObj = std::find(mpScnObjArray, mpScnObjArray + mNumScnObj, pObj);

    if (ppObj != mpScnObjArray + mNumScnObj) {
        return Remove(std::distance(mpScnObjArray, ppObj)) != NULL;
    }

    return false;
}

ScnGroup::ScnGroup(MEMAllocator* pAllocator, ScnObj** ppObj, u32 capacity)
    : ScnObj(pAllocator),
      mpScnObjArray(ppObj),
      mSizeScnObj(capacity),
      mNumScnObj(0) {

    SetScnObjFlag(SCNOBJFLAG_NOT_GATHER_DRAW_OPA, TRUE);
    SetScnObjFlag(SCNOBJFLAG_NOT_GATHER_DRAW_XLU, TRUE);
}

ScnGroup::~ScnGroup() {
    Clear();
}

} // namespace g3d
} // namespace nw4r
