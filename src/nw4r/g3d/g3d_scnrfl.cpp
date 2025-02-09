#include <nw4r/g3d.h>

namespace nw4r {
namespace g3d {

NW4R_G3D_RTTI_DEF(ScnRfl);

void ScnRfl::SetLightSetIdx(int idx) {
    if (idx < 0 || idx > G3DState::NUM_LIGHT_SET - 1) {
        mIdxLightSet = -1;
        return;
    }

    mIdxLightSet = idx;
}

void ScnRfl::SetFogIdx(int idx) {
    // @bug G3DState only allows 32 fog entries
    if (idx < 0 || idx > 127) {
        mIdxFog = -1;
        return;
    }

    mIdxFog = idx;
}

bool ScnRfl::SetExpression(RFLExpression expression) {
    if (IsCharModelReady()) {
        RFLSetExpression(&mCharModel, expression);
        return true;
    }

    return false;
}

bool ScnRfl::GetExpression(RFLExpression* pExpression) {
    if (pExpression != NULL && IsCharModelReady()) {
        *pExpression = RFLGetExpression(&mCharModel);
        return true;
    }

    return false;
}

bool ScnRfl::SetupCharModel(RFLDataSource src, u16 index,
                            RFLMiddleDB* pMiddleDB) {
    ReleaseCharModel();

    mRFLErrCode = RFLInitCharModel(&mCharModel, src, pMiddleDB, index,
                                   mpNglBuffer, mResolution, mExpressionFlag);

    if (mRFLErrCode != RFLErrcode_Success) {
        return false;
    }

    mCharModelReady = TRUE;
    return true;
}

void ScnRfl::ReleaseCharModel() {
    if (IsCharModelReady()) {
        mCharModelReady = FALSE;
    }
}

ScnRfl* ScnRfl::Construct(MEMAllocator* pAllocator, u32* pSize,
                          RFLResolution resolution, u32 exprFlags,
                          u32 userDataSize) {
    ScnRfl* pScnRfl = NULL;

    u32 scnRflSize = sizeof(ScnRfl);
    u32 nglSize = RFLGetModelBufferSize(resolution, exprFlags);

    u32 userOffset = align4(scnRflSize);
    u32 nglOffset = align32(userOffset + userDataSize);

    u32 size = align32(nglOffset + nglSize);
    if (pSize != NULL) {
        *pSize = size;
    }

    if (pAllocator != NULL) {
        u8* pBuffer = reinterpret_cast<u8*>(Alloc(pAllocator, size));
        if (pBuffer == NULL) {
            return NULL;
        }

        // clang-format off
        pScnRfl = new (pBuffer) ScnRfl(
            pAllocator,
            pBuffer + nglOffset,
            userDataSize != 0 ? pBuffer + userOffset : NULL,
            resolution,
            exprFlags);
        // clang-format on
    }

    return pScnRfl;
}

void ScnRfl::G3dProc(u32 task, u32 param, void* pInfo) {
    if (IsG3dProcDisabled(task)) {
        return;
    }

    switch (task) {
    case G3DPROC_GATHER_SCNOBJ: {
        IScnObjGather* pCollection = static_cast<IScnObjGather*>(pInfo);
        pCollection->Add(this, true, true);
        break;
    }

    case G3DPROC_CALC_WORLD: {
        CheckCallback_CALC_WORLD(CALLBACK_TIMING_A, param, pInfo);
        CalcWorldMtx(static_cast<math::MTX34*>(pInfo), &param);
        CheckCallback_CALC_WORLD(CALLBACK_TIMING_B, param, pInfo);
        CheckCallback_CALC_WORLD(CALLBACK_TIMING_C, param, pInfo);
        break;
    }

    case G3DPROC_CALC_VIEW: {
        CheckCallback_CALC_VIEW(CALLBACK_TIMING_A, param, pInfo);
        CalcViewMtx(static_cast<math::MTX34*>(pInfo));
        CheckCallback_CALC_VIEW(CALLBACK_TIMING_B, param, pInfo);

        math::MTX34 viewMtx;
        GetMtx(MTX_VIEW, &viewMtx);
        RFLSetMtx(&mCharModel, viewMtx);

        CheckCallback_CALC_VIEW(CALLBACK_TIMING_C, param, pInfo);
        break;
    }

    case G3DPROC_DRAW_OPA: {
        if (!IsCharModelReady()) {
            break;
        }

        if (mpDrawProc == NULL) {
            LoadDrawSetting();
            RFLDrawOpa(&mCharModel);
        } else {
            CallDrawProc(true);
        }
        break;
    }

    case G3DPROC_DRAW_XLU: {
        if (!IsCharModelReady()) {
            break;
        }

        if (mpDrawProc == NULL) {
            LoadDrawSetting();
            RFLDrawXlu(&mCharModel);
        } else {
            CallDrawProc(false);
        }
        break;
    }

    default: {
        DefG3dProcScnLeaf(task, param, pInfo);
        break;
    }
    }
}

void ScnRfl::CallDrawProc(bool opa) {
    G3DState::LoadFog(GetFogIdx());

    u32 diffMask, specMask;
    AmbLightObj lobj;
    G3DState::LoadLightSet(GetLightSetIdx(), &diffMask, &specMask, &lobj);

    GXColor ambColor;
    ambColor.r = 0.5f + (lobj.r * mAmbientColor.r) * (1.0f / 255.0f);
    ambColor.g = 0.5f + (lobj.g * mAmbientColor.g) * (1.0f / 255.0f);
    ambColor.b = 0.5f + (lobj.b * mAmbientColor.b) * (1.0f / 255.0f);
    ambColor.a = 0.5f + (lobj.a * mAmbientColor.a) * (1.0f / 255.0f);

    G3DState::Invalidate(
        G3DState::INVALIDATE_ALL &
        ~(G3DState::INVALIDATE_FOG | G3DState::INVALIDATE_LIGHT));

    mpDrawProc(this, &mCharModel, diffMask, specMask, ambColor, opa);
}

void ScnRfl::LoadDrawSetting() {
    RFLDrawSetting setting;

    G3DState::LoadFog(GetFogIdx());

    u32 diffMask, specMask;
    AmbLightObj lobj;
    G3DState::LoadLightSet(GetLightSetIdx(), &diffMask, &specMask, &lobj);

    setting.lightEnable = GetLightSetIdx() >= 0 ? TRUE : FALSE;
    setting.lightMask = static_cast<GXLightID>(diffMask);
    setting.diffuse = GetDiffuseFunc();
    setting.attn = GetAttnFunc();
    setting.compLoc = GXGetZCompLoc();

    setting.ambColor.r = 0.5f + ((lobj.r * mAmbientColor.r) * (1.0f / 255.0f));
    setting.ambColor.g = 0.5f + ((lobj.g * mAmbientColor.g) * (1.0f / 255.0f));
    setting.ambColor.b = 0.5f + ((lobj.b * mAmbientColor.b) * (1.0f / 255.0f));
    setting.ambColor.a = 0.5f + ((lobj.a * mAmbientColor.a) * (1.0f / 255.0f));

    G3DState::Invalidate(
        G3DState::INVALIDATE_ALL &
        ~(G3DState::INVALIDATE_FOG | G3DState::INVALIDATE_LIGHT));

    RFLLoadDrawSetting(&setting);
}

ScnRfl::ScnRfl(MEMAllocator* pAllocator, void* pNglBuffer, void* pUserData,
               RFLResolution resolution, u32 exprFlag)
    : ScnLeaf(pAllocator),
      mResolution(resolution),
      mExpressionFlag(exprFlag),
      mIdxLightSet(-1),
      mIdxFog(-1),
      mZCompLoc(TRUE),
      mCharModelReady(FALSE),
      mDiffuseFn(GX_DF_CLAMP),
      mAttnFn(GX_AF_SPOT),
      mpNglBuffer(pNglBuffer),
      mRFLErrCode(RFLErrcode_Success),
      mpDrawProc(NULL),
      mpUserData(pUserData) {

    mAmbientColor.r = mAmbientColor.g = mAmbientColor.b = 64;
    mAmbientColor.a = 255;
}

} // namespace g3d
} // namespace nw4r
