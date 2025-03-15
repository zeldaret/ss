#include "nw4r/g3d/g3d_scnroot.h"

#include "nw4r/g3d.h" // IWYU pragma: export
#include "nw4r/g3d/g3d_scnobj.h"

#include "rvl/GX.h" // IWYU pragma: export

#include <algorithm>
#include <new>

namespace nw4r {
namespace g3d {

NW4R_G3D_RTTI_DEF(ScnRoot);

/******************************************************************************
 *
 * ScnRoot
 *
 ******************************************************************************/
ScnRoot *ScnRoot::Construct(
    MEMAllocator *pAllocator, u32 *pSize, u32 maxChildren, u32 maxScnObj, u32 numLightObj, u32 numLightSet
) {
    ScnRoot *pScnRoot = NULL;
    u32 scnRootSize = sizeof(ScnRoot);
    u32 scnObjGatherSize = sizeof(ScnObjGather);

    u32 childrenSize = maxChildren * sizeof(ScnObj *);
    u32 scnObjBufSize = maxScnObj * sizeof(ScnObj *) + maxScnObj * sizeof(ScnObj *);

    u32 lightObjSize = numLightObj * sizeof(LightObj);
    u32 ambLightObjSize = numLightObj * sizeof(AmbLightObj);
    u32 lightSetDataSize = numLightSet * sizeof(LightSetData);

    u32 scnObjGatherOfs = align4(scnRootSize);
    u32 childrenOfs = align4(scnObjGatherOfs + scnObjGatherSize);
    u32 scnObjBufOfs = align4(childrenOfs + childrenSize);
    u32 lightObjOfs = align4(scnObjBufOfs + scnObjBufSize);
    u32 ambLightOfs = align4(lightObjOfs + lightObjSize);
    u32 lightSetDataOfs = align4(ambLightOfs + ambLightObjSize);

    u32 size = align4(lightSetDataOfs + lightSetDataSize);
    if (pSize != NULL) {
        *pSize = size;
    }

    if (pAllocator != NULL) {
        u8 *pBuffer = reinterpret_cast<u8 *>(Alloc(pAllocator, size));
        if (pBuffer == NULL) {
            return NULL;
        }

        ScnObj **ppScnObjBuf = reinterpret_cast<ScnObj **>(pBuffer + scnObjBufOfs);

        IScnObjGather *pGather =
            new (pBuffer + scnObjGatherOfs) ScnObjGather(ppScnObjBuf, ppScnObjBuf + maxScnObj, maxScnObj);

        LightObj *pLightObjBuf = reinterpret_cast<LightObj *>(pBuffer + lightObjOfs);

        AmbLightObj *pAmbObjBuf = reinterpret_cast<AmbLightObj *>(pBuffer + ambLightOfs);

        LightSetData *pLightSetBuf = reinterpret_cast<LightSetData *>(pBuffer + lightSetDataOfs);

        // clang-format off
        pScnRoot = new (pBuffer) ScnRoot(
            pAllocator,
            pGather,
            reinterpret_cast<ScnObj**>(pBuffer + childrenOfs),
            maxChildren,
            numLightObj,
            numLightSet,
            pLightObjBuf,
            pAmbObjBuf,
            pLightSetBuf);
        // clang-format on
    }

    return pScnRoot;
}

void ScnRoot::G3dProc(u32 task, u32 param, void *pInfo) {
    if (IsG3dProcDisabled(task)) {
        return;
    }

    switch (task) {
        case G3DPROC_CHILD_DETACHED: {
            if (mpAnmScn == pInfo) {
                mpAnmScn->G3dProc(G3DPROC_DETACH_PARENT, 0, this);
                mpAnmScn = NULL;
                break;
            }

            // FALLTHROUGH, detach may be for ScnGroup
        }

        default: {
            DefG3dProcScnGroup(task, param, pInfo);
            break;
        }
    }
}

Camera ScnRoot::GetCamera(int idx) {
    if (0 <= idx && idx < G3DState::NUM_CAMERA) {
        return Camera(&mCamera[idx]);
    }

    return Camera(NULL);
}

Camera ScnRoot::GetCurrentCamera() {
    return Camera(&mCamera[mCurrentCameraID]);
}

void ScnRoot::SetCurrentCamera(int idx) {
    mCurrentCameraID = static_cast<u8>(idx);
}

Fog ScnRoot::GetFog(int idx) {
    if (0 <= idx && idx < G3DState::NUM_FOG) {
        return Fog(&mFog[idx]);
    }

    return Fog(NULL);
}

LightSet ScnRoot::GetLightSet(int idx) {
    return mLightSetting.GetLightSet(idx);
}

void ScnRoot::UpdateFrame() {
    if (mpAnmScn != NULL) {
        mpAnmScn->UpdateFrame();
    }

    G3dProc(G3DPROC_UPDATEFRAME, 0, NULL);
}

void ScnRoot::SetGlbSettings() {
    int i;

    Camera cam = GetCurrentCamera();
    cam.GXSetViewport();
    cam.GXSetProjection();
    cam.GXSetScissor();
    cam.GXSetScissorBoxOffset();

    for (i = 0; i < G3DState::NUM_CAMERA; i++) {
        G3DState::SetCameraProjMtx(Camera(&mCamera[i]), i, i == mCurrentCameraID);
    }

    bool persp = cam.GetProjectionType() != GX_ORTHOGRAPHIC;
    u32 projOrthoBit = persp ? 0 : GX_ORTHOGRAPHIC << 3;

    f32 near = cam.ref().projNear;
    f32 far = cam.ref().projFar;

    u16 width = 0;
    u16 center = 0;

    math::MTX44 proj;
    bool fogRangeAdj = false;

    for (i = 0; i < G3DState::NUM_FOG; i++) {
        Fog fog(&mFog[i]);
        fog.SetNearFar(near, far);

        // Enum Comparison mismatch on purpose
        if (fog.ref().type != GX_PERSPECTIVE) {
            fog.ref().type = static_cast<GXFogType>((fog.ref().type & ~(GX_ORTHOGRAPHIC << 3)) | projOrthoBit);
        }

        if (fog.IsFogRangeAdjEnable()) {
            if (!fogRangeAdj) {
                cam.GetProjectionMtx(&proj);

                f32 f_width, x;
                cam.GetViewport(&x, NULL, &f_width, NULL, NULL, NULL);

                width = math::F32ToU16(f_width);
                center = math::F32ToU16(x + f_width * 0.5f);
                fogRangeAdj = true;
            }

            fog.SetFogRangeAdjParam(width, center, proj);
        }

        G3DState::SetFog(fog, i);
    }

    G3DState::SetLightSetting(mLightSetting);
}

void ScnRoot::CalcAnmScn() {
    if (mpAnmScn == NULL) {
        return;
    }

    const u32 numLightSet = mpAnmScn->GetLightSetMaxRefNumber();
    const u32 numFog = mpAnmScn->GetFogMaxRefNumber();
    const u32 numCamera = mpAnmScn->GetCameraMaxRefNumber();

    if (numLightSet > 0) {
        mpAnmScn->GetLightSetting(&mLightSetting);
    }

    if (numFog > 0) {
        const u32 numLoadableFog = std::min<u32>(G3DState::NUM_FOG, numFog);

        for (u32 i = 0; i < numLoadableFog; i++) {
            Fog fog(&mFog[i]);
            mpAnmScn->GetFog(fog, i);
        }
    }

    if (numCamera > 0) {
        const u32 numLoadableCamera = std::min<u32>(G3DState::NUM_CAMERA, numCamera);

        for (u32 i = 0; i < numLoadableCamera; i++) {
            Camera cam(&mCamera[i]);
            mpAnmScn->GetCamera(cam, i);
        }
    }
}

void ScnRoot::CalcWorld() {
    CalcAnmScn();
    G3dProc(G3DPROC_CALC_WORLD, 0, NULL);
}

void ScnRoot::CalcMaterial() {
    G3dProc(G3DPROC_CALC_MAT, 0, NULL);
}

void ScnRoot::CalcVtx() {
    G3dProc(G3DPROC_CALC_VTX, 0, NULL);
}

void ScnRoot::CalcView() {
    math::MTX34 cam;
    GXInvalidateVtxCache();
    GetCurrentCamera().GetCameraMtx(&cam);
    G3dProc(G3DPROC_CALC_VIEW, 0, &cam);
}

void ScnRoot::GatherDrawScnObj() {
    mpCollection->Clear();

    math::FRUSTUM frustum;
    Camera cam = GetCurrentCamera();

    math::MTX34 mtx;
    cam.GetCameraMtx(&mtx);

    if (cam.ref().flags & CameraData::FLAG_PROJ_PERSP) {
        frustum.Set(cam.ref().projFovy, cam.ref().projAspect, cam.ref().projNear, cam.ref().projFar, mtx);
    } else {
        frustum.Set(
            cam.ref().projTop, cam.ref().projBottom, cam.ref().projLeft, cam.ref().projRight, cam.ref().projNear,
            cam.ref().projFar, mtx
        );
    }

    gpCullingFrustum = &frustum;
    G3dProc(G3DPROC_GATHER_SCNOBJ, 0, mpCollection);
    gpCullingFrustum = NULL;
}

void ScnRoot::ZSort() {
    mpCollection->ZSort();
    G3dProc(G3DPROC_ZSORT, 0, mpCollection);
}

void ScnRoot::DrawOpa() {
    SetGlbSettings();

    if (TestScnRootFlag(SCNROOTFLAG_FORCE_RESMDLDRAWMODE)) {
        mpCollection->DrawOpa(&mDrawMode);
    } else {
        mpCollection->DrawOpa(NULL);
    }

    G3DState::Invalidate(G3DState::INVALIDATE_TEV);
}

void ScnRoot::DrawXlu() {
    SetGlbSettings();

    if (TestScnRootFlag(SCNROOTFLAG_FORCE_RESMDLDRAWMODE)) {
        mpCollection->DrawXlu(&mDrawMode);
    } else {
        mpCollection->DrawXlu(NULL);
    }

    G3DState::Invalidate(G3DState::INVALIDATE_TEV);
}

ScnRoot::ScnRoot(
    MEMAllocator *pAllocator, IScnObjGather *pGather, ScnObj **ppChildrenBuf, u32 maxChildren, u32 numLight,
    u32 numLightSet, LightObj *pLightObjBuf, AmbLightObj *pAmbObjBuf, LightSetData *pLightSetBuf
)
    : ScnGroup(pAllocator, ppChildrenBuf, maxChildren),
      mpCollection(pGather),
      mDrawMode(RESMDL_DRAWMODE_DEFAULT),
      mScnRootFlags(0),
      mCurrentCameraID(0),
      mLightSetting(pLightObjBuf, pAmbObjBuf, numLight, pLightSetBuf, numLightSet),
      mpAnmScn(NULL) {
    for (u32 i = 0; i < G3DState::NUM_CAMERA; i++) {
        Camera res(&mCamera[i]);
        res.Init();
    }

    for (u32 i = 0; i < G3DState::NUM_FOG; i++) {
        Fog res(&mFog[i]);
        res.Init();
    }
}

ScnRoot::~ScnRoot() {
    if (mpAnmScn != NULL) {
        mpAnmScn->G3dProc(G3DPROC_DETACH_PARENT, 0, this);
    }
}

/******************************************************************************
 *
 * Sorting functions
 *
 ******************************************************************************/
namespace {

inline bool LessZSortOpa(const ScnObj *pLhs, const ScnObj *pRhs) {
    int lhsPrio = pLhs->GetPriorityDrawOpa();
    int rhsPrio = pRhs->GetPriorityDrawOpa();

    if (lhsPrio == rhsPrio) {
        return pLhs->GetMtxPtr(ScnObj::MTX_VIEW)->_23 > pRhs->GetMtxPtr(ScnObj::MTX_VIEW)->_23;
    }

    return lhsPrio < rhsPrio;
}

inline bool LessZSortXlu(const ScnObj *pLhs, const ScnObj *pRhs) {
    int lhsPrio = pLhs->GetPriorityDrawXlu();
    int rhsPrio = pRhs->GetPriorityDrawXlu();

    if (lhsPrio == rhsPrio) {
        return pLhs->GetMtxPtr(ScnObj::MTX_VIEW)->_23 < pRhs->GetMtxPtr(ScnObj::MTX_VIEW)->_23;
    }

    return lhsPrio < rhsPrio;
}

inline bool LessByGetValueForSortOpa(const ScnObj *pLhs, const ScnObj *pRhs) {
    int lhsPrio = pLhs->GetPriorityDrawOpa();
    int rhsPrio = pRhs->GetPriorityDrawOpa();

    if (lhsPrio == rhsPrio) {
        return pLhs->GetValueForSortOpa() < pRhs->GetValueForSortOpa();
    }

    return lhsPrio < rhsPrio;
}

inline bool LessByGetValueForSortXlu(const ScnObj *pLhs, const ScnObj *pRhs) {
    int lhsPrio = pLhs->GetPriorityDrawXlu();
    int rhsPrio = pRhs->GetPriorityDrawXlu();

    if (lhsPrio == rhsPrio) {
        return pLhs->GetValueForSortXlu() < pRhs->GetValueForSortXlu();
    }

    return lhsPrio < rhsPrio;
}

} // namespace

/******************************************************************************
 *
 * ScnObjGather
 *
 ******************************************************************************/
IScnObjGather::CullingStatus ScnObjGather::Add(ScnObj *pObj, bool opa, bool xlu) {
    IScnObjGather::CullingStatus status = IScnObjGather::CULLINGSTATUS_INTERSECT;

    math::IntersectionResult ixResult = math::INTERSECTION_INTERSECT;

    if (gpCullingFrustum != NULL) {
        u32 value;
        pObj->GetScnObjOption(ScnObj::OPTION_ENABLE_CULLING, &value);

        if (value) {
            math::AABB aabb;
            pObj->GetBoundingVolume(ScnObj::BOUNDINGVOLUME_AABB_WORLD, &aabb);
            ixResult = gpCullingFrustum->IntersectAABB_Ex(&aabb);

            if (ixResult == math::INTERSECTION_NONE) {
                return IScnObjGather::CULLINGSTATUS_OUTSIDE;
            } else if (ixResult == math::INTERSECTION_INSIDE) {
                status = IScnObjGather::CULLINGSTATUS_INSIDE;
            }
        }
    }

    if (opa) {
        if (mNumScnObjOpa < mSizeScnObj) {
            mpArrayOpa[mNumScnObjOpa++] = pObj;
        } else {
            return status;
        }
    }

    if (xlu) {
        if (mNumScnObjXlu < mSizeScnObj) {
            mpArrayXlu[mNumScnObjXlu++] = pObj;
        } else {
            return status;
        }
    }

    return status;
}

void ScnObjGather::ZSort() {
    std::sort(mpArrayOpa, mpArrayOpa + mNumScnObjOpa, LessZSortOpa);
    std::sort(mpArrayXlu, mpArrayXlu + mNumScnObjXlu, LessZSortXlu);
}

void ScnObjGather::Sort() {
    std::sort(mpArrayOpa, mpArrayOpa + mNumScnObjOpa, LessByGetValueForSortOpa);
    std::sort(mpArrayXlu, mpArrayXlu + mNumScnObjXlu, LessByGetValueForSortXlu);
}

void ScnObjGather::Sort(LessThanFunc pOpaFunc, LessThanFunc pXluFunc) {
    std::sort(mpArrayOpa, mpArrayOpa + mNumScnObjOpa, pOpaFunc);
    std::sort(mpArrayXlu, mpArrayXlu + mNumScnObjXlu, pXluFunc);
}

void ScnObjGather::DrawOpa(ResMdlDrawMode *pForceMode) {
    for (u32 i = 0; i != mNumScnObjOpa;) {
        mpArrayOpa[i++]->G3dProc(G3dObj::G3DPROC_DRAW_OPA, 0, pForceMode);
    }
}

void ScnObjGather::DrawXlu(ResMdlDrawMode *pForceMode) {
    for (u32 i = 0; i != mNumScnObjXlu;) {
        mpArrayXlu[i++]->G3dProc(G3dObj::G3DPROC_DRAW_XLU, 0, pForceMode);
    }
}

ScnObjGather::CheckStatus ScnObjGather::CheckScnObj(ScnObj *) {
    return ScnObjGather::CHECKSTATUS_GATHER_SCNOBJ;
}

ScnObjGather::ScnObjGather(ScnObj **ppBufOpa, ScnObj **ppBufXlu, u32 capacity)
    : mpArrayOpa(ppBufOpa), mpArrayXlu(ppBufXlu), mSizeScnObj(capacity), mNumScnObjOpa(0), mNumScnObjXlu(0) {}

} // namespace g3d
} // namespace nw4r
