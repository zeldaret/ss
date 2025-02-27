#include "common.h"
#include <nw4r/g3d.h>

#include <algorithm>
#include <cstring>

namespace nw4r {
namespace g3d {
namespace detail {
namespace ScnDependentMtxFunc {
/******************************************************************************
 *
 * ScnDependentMtxFunc
 *
 ******************************************************************************/
void EnvironmentMapping(math::MTX34 *pMtx, s8 camRef, s8 lightRef) {
    if (pMtx == NULL) {
        return;
    }

    if (camRef >= 0 && camRef < G3DState::NUM_CAMERA) {
        math::MTX34Mult(pMtx, G3DState::GetCameraMtxPtr(camRef), G3DState::GetInvCameraMtxPtr());

        pMtx->_03 = pMtx->_13 = pMtx->_23 = 0.0f;

        math::MTX34Mult(pMtx, G3DState::GetEnvironmentTexMtxPtr(), pMtx);
        return;
    }

    if (lightRef >= 0 && lightRef < G3DState::NUM_LIGHT && G3DState::GetLightObj(lightRef)->IsEnable()) {
        math::VEC3 look, camUp, right, up;

        const LightObj *pLight = G3DState::GetLightObj(lightRef);
        pLight->GetLightDir(&look);

        const math::MTX34 *pInvCamMtx = G3DState::GetInvCameraMtxPtr();

        if (pLight->IsDiffuseLight() || (look.x == 0.0f && look.y == 0.0f && look.z == 0.0f)) {
            pLight->GetLightPos(&look);
            math::VEC3TransformNormal(&look, pInvCamMtx, &look);

            look = -look;

            if (look.x == 0.0f && look.y == 0.0f && look.z == 0.0f) {
                look.y = -1.0f;
            }
        } else {
            math::VEC3TransformNormal(&look, pInvCamMtx, &look);
        }

        math::VEC3Normalize(&look, &look);

        if (math::FAbs(look.x) < 0.000001f && math::FAbs(look.z) < 0.000001f) {
            camUp.x = camUp.y = 0.0f;

            if (look.y <= 0.0f) {
                camUp.z = -1.0f;
            } else {
                camUp.z = 1.0f;
            }
        } else {
            camUp.x = camUp.z = 0.0f;
            camUp.y = 1.0f;
        }

        math::VEC3Cross(&right, &look, &camUp);
        math::VEC3Normalize(&right, &right);
        math::VEC3Cross(&up, &right, &look);

        pMtx->_00 = right.x;
        pMtx->_01 = right.y;
        pMtx->_02 = right.z;

        pMtx->_10 = up.x;
        pMtx->_11 = up.y;
        pMtx->_12 = up.z;

        pMtx->_20 = -look.x;
        pMtx->_21 = -look.y;
        pMtx->_22 = -look.z;

        math::MTX34Mult(pMtx, pMtx, pInvCamMtx);
        pMtx->_03 = pMtx->_13 = pMtx->_23 = 0.0f;

        static const math::_MTX34 envMtx = {
            // clang-format off
            0.5f,  0.0f,  0.0f,  0.5f,
            0.0f, -0.5f,  0.0f,  0.5f,
            0.0f,  0.0f,  0.0f,  1.0f
            // clang-format on
        };

        math::MTX34Mult(pMtx, static_cast<const math::MTX34 *>(&envMtx), pMtx);
        return;
    }

    math::MTX34Copy(pMtx, G3DState::GetEnvironmentTexMtxPtr());
}

void ProjectionMapping(math::MTX34 *pMtx, s8 camRef, s8 lightRef) {
#pragma unused(lightRef)

    if (pMtx == NULL) {
        return;
    }

    if (camRef >= 0 && camRef < G3DState::NUM_CAMERA) {
        math::MTX34Mult(pMtx, G3DState::GetCameraMtxPtr(camRef), G3DState::GetInvCameraMtxPtr());

        math::MTX34Mult(pMtx, G3DState::GetProjectionTexMtxPtr(camRef), pMtx);
    } else {
        math::MTX34Copy(pMtx, G3DState::GetProjectionTexMtxPtr());
    }
}

void EnvironmentSpecularMapping(math::MTX34 *pMtx, s8 camRef, s8 lightRef) {
    if (pMtx == NULL) {
        return;
    }

    math::VEC3 look, camUp, camLook;

    const math::MTX34 *pCamMtx = G3DState::GetCameraMtxPtr();
    camLook.x = -pCamMtx->_20;
    camLook.y = -pCamMtx->_21;
    camLook.z = -pCamMtx->_22;

    const math::MTX34 *pInvCamMtx = G3DState::GetInvCameraMtxPtr();

    if (lightRef >= 0 && lightRef < G3DState::NUM_LIGHT && G3DState::GetLightObj(lightRef)->IsEnable()) {
        math::VEC3 lightLook;

        const LightObj *pLight = G3DState::GetLightObj(lightRef);
        pLight->GetLightDir(&lightLook);

        if (pLight->IsDiffuseLight() || (lightLook.x == 0.0f && lightLook.y == 0.0f && lightLook.z == 0.0f)) {
            pLight->GetLightPos(&lightLook);
            math::VEC3TransformNormal(&lightLook, pInvCamMtx, &lightLook);

            lightLook = -lightLook;

            if (lightLook.x == 0.0f && lightLook.y == 0.0f && lightLook.z == 0.0f) {
                lightLook.y = -1.0f;
            }
        } else {
            math::VEC3TransformNormal(&lightLook, pInvCamMtx, &lightLook);
        }

        if (!pLight->IsSpecularLight()) {
            C_VECHalfAngle(camLook, lightLook, look);
        } else {
            look = -lightLook;
        }

        if (math::FAbs(look.x) < 0.000001f && math::FAbs(look.z) < 0.000001f) {
            camUp.x = camUp.y = 0.0f;

            if (look.y <= 0.0f) {
                camUp.z = -1.0f;
            } else {
                camUp.z = 1.0f;
            }
        } else {
            camUp.x = camUp.z = 0.0f;
            camUp.y = 1.0f;
        }

    } else if (camRef >= 0 && camRef < G3DState::NUM_CAMERA) {
        const math::MTX34 *pLightCamMtx = G3DState::GetCameraMtxPtr(camRef);
        camUp.x = pLightCamMtx->_10;
        camUp.y = pLightCamMtx->_11;
        camUp.z = pLightCamMtx->_12;

        math::VEC3 lightLook(-pLightCamMtx->_20, -pLightCamMtx->_21, -pLightCamMtx->_22);

        C_VECHalfAngle(camLook, lightLook, look);
    } else {
        math::MTX34Copy(pMtx, G3DState::GetEnvironmentTexMtxPtr());
        return;
    }

    math::VEC3Normalize(&look, &look);

    math::VEC3 right, up;

    math::VEC3Cross(&right, &camUp, &look);
    math::VEC3Normalize(&right, &right);

    math::VEC3Cross(&up, &look, &right);

    pMtx->_00 = right.x;
    pMtx->_01 = right.y;
    pMtx->_02 = right.z;

    pMtx->_10 = up.x;
    pMtx->_11 = up.y;
    pMtx->_12 = up.z;

    pMtx->_20 = look.x;
    pMtx->_21 = look.y;
    pMtx->_22 = look.z;

    math::MTX34Mult(pMtx, pMtx, pInvCamMtx);
    pMtx->_03 = pMtx->_13 = pMtx->_23 = 0.0f;

    math::MTX34Mult(pMtx, G3DState::GetEnvironmentTexMtxPtr(), pMtx);
}

void DefaultMapping(math::MTX34 *pMtx, s8 camRef, s8 lightRef) {
#pragma unused(camRef)
#pragma unused(lightRef)

    if (pMtx == NULL) {
        return;
    }

    math::MTX34Identity(pMtx);
}

} // namespace ScnDependentMtxFunc
} // namespace detail

namespace G3DState {
namespace {
/******************************************************************************
 *
 * Utility functions
 *
 ******************************************************************************/
inline bool IsEqualTexObj(const GXTexObj &rLhs, const GXTexObj &rRhs) {
    return rLhs.dummy[0] == rRhs.dummy[0] && rLhs.dummy[1] == rRhs.dummy[1] && rLhs.dummy[2] == rRhs.dummy[2] &&
           rLhs.dummy[3] == rRhs.dummy[3] && rLhs.dummy[4] == rRhs.dummy[4] && rLhs.dummy[5] == rRhs.dummy[5] &&
           rLhs.dummy[6] == rRhs.dummy[6] && rLhs.dummy[7] == rRhs.dummy[7];
}

inline bool IsEqualTlutObj(const GXTlutObj &rLhs, const GXTlutObj &rRhs) {
    return rLhs.dummy[0] == rRhs.dummy[0] && rLhs.dummy[1] == rRhs.dummy[1] && rLhs.dummy[2] == rRhs.dummy[2];
}

} // namespace

/******************************************************************************
 *
 * IndMtxTexInfo
 *
 ******************************************************************************/
IndTexMtxInfo::IndTexMtxInfo(const ResMatIndMtxAndScale ind) : flag(0) {
    if (ind.GXGetIndTexMtx(GX_ITM_0, &offset_mtx[GX_ITM_0 - 1])) {
        flag |= 1 << GX_ITM_0 - 1;
    }

    if (ind.GXGetIndTexMtx(GX_ITM_1, &offset_mtx[GX_ITM_1 - 1])) {
        flag |= 1 << GX_ITM_1 - 1;
    }

    if (ind.GXGetIndTexMtx(GX_ITM_2, &offset_mtx[GX_ITM_2 - 1])) {
        flag |= 1 << GX_ITM_2 - 1;
    }
}

void IndTexMtxInfo::FifoSend() const {
    if (flag & (1 << GX_ITM_0 - 1)) {
        fifo::GDSetIndTexMtx(GX_PNMTX0, offset_mtx[GX_ITM_0 - 1]);
    }

    if (flag & (1 << GX_ITM_1 - 1)) {
        fifo::GDSetIndTexMtx(GX_PNMTX1, offset_mtx[GX_ITM_1 - 1]);
    }

    if (flag & (1 << GX_ITM_2 - 1)) {
        fifo::GDSetIndTexMtx(GX_PNMTX2, offset_mtx[GX_ITM_2 - 1]);
    }
}

void IndTexMtxInfo::SetMtx(GXIndTexMtxID id, const math::MTX34 &rMtx) {
    if (id == GX_ITM_0) {
        offset_mtx[GX_ITM_0 - 1] = rMtx;
        flag |= 1 << GX_ITM_0 - 1;

    } else if (id == GX_ITM_1) {
        offset_mtx[GX_ITM_1 - 1] = rMtx;
        flag |= 1 << GX_ITM_1 - 1;

    } else if (id == GX_ITM_2) {
        offset_mtx[GX_ITM_2 - 1] = rMtx;
        flag |= 1 << GX_ITM_2 - 1;
    }
}

namespace {
/******************************************************************************
 *
 * SyncGX
 *
 ******************************************************************************/

class SyncGX {
public:
    SyncGX() : bNeedSync(true) {}

    void Set();
    void Unset() {
        bNeedSync = false;
    }

    bool Get() {
        bool result = bNeedSync;
        Unset();
        return result;
    }

    void Invalidate() {
        bNeedSync = true;
    }

private:
    bool bNeedSync; // at 0x0
};

SyncGX sSyncGX;

void SyncGX::Set() {
    bNeedSync = true;
}

/******************************************************************************
 *
 * GenMode2
 *
 ******************************************************************************/
class GenMode2 {
public:
    GenMode2() : mNumTexGens(1), mNumChans(0), mNumTevs(1), mNumInds(0), mCullMode(GX_CULL_BACK), flag(0) {}

    void Invalidate() {
        mNumTexGens = 1;
        mNumChans = 0;
        mNumTevs = 1;
        mNumInds = 0;
        mCullMode = GX_CULL_BACK;
        flag = 0;
    }

    void LoadBP() {
        if ((flag & FLAG_FIRST_SET) && (flag & FLAG_ALL_LOADED) == FLAG_XF_LOADED) {
            fifo::GDSetGenMode2Ex_BP(mNumTexGens, mNumChans, mNumTevs, mNumInds, mCullMode);

            flag |= FLAG_BP_LOADED;
        }
    }

    void LoadXF() {
        if ((flag & FLAG_FIRST_SET) && (flag & FLAG_ALL_LOADED) != FLAG_ALL_LOADED) {
            fifo::GDSetGenMode2(mNumTexGens, mNumChans, mNumTevs, mNumInds, mCullMode);

            flag |= FLAG_BP_LOADED | FLAG_XF_LOADED;
        }
    }

    void SetResGenMode(const ResGenMode res) {
        if (mNumTexGens != res.GXGetNumTexGens()) {
            mNumTexGens = res.GXGetNumTexGens();
            flag &= ~FLAG_ALL_LOADED;
        }

        if (mNumChans != res.GXGetNumChans()) {
            mNumChans = res.GXGetNumChans();
            flag &= ~FLAG_ALL_LOADED;
        }

        if (mNumTevs != res.GXGetNumTevStages()) {
            mNumTevs = res.GXGetNumTevStages();
            flag &= ~FLAG_BP_LOADED;
        }

        if (mNumInds != res.GXGetNumIndStages()) {
            mNumInds = res.GXGetNumIndStages();
            flag &= ~FLAG_BP_LOADED;
        }

        if (mCullMode != res.GXGetCullMode()) {
            mCullMode = res.GXGetCullMode();
            flag &= ~FLAG_BP_LOADED;
        }

        if (!(flag & FLAG_FIRST_SET)) {
            flag &= ~FLAG_ALL_LOADED;
            flag |= FLAG_FIRST_SET;
        }
    }

    u8 GXGetNumTexGens() const {
        return mNumTexGens;
    }

    u8 GXGetNumChans() const {
        return mNumChans;
    }

    u8 GXGetNumIndStages() const {
        return mNumInds;
    }

private:
    enum Flag {
        FLAG_BP_LOADED = (1 << 0),
        FLAG_XF_LOADED = (1 << 1),
        FLAG_FIRST_SET = (1 << 2),

        FLAG_ALL_LOADED = FLAG_BP_LOADED | FLAG_XF_LOADED
    };

private:
    u8 mNumTexGens;       // at 0x0
    u8 mNumChans;         // at 0x1
    u8 mNumTevs;          // at 0x2
    u8 mNumInds;          // at 0x3
    GXCullMode mCullMode; // at 0x4
    u32 flag;             // at 0x8
};

GenMode2 sGenMode2;

/******************************************************************************
 *
 * TexCoordScaleState
 *
 ******************************************************************************/
struct TexMapSize {
    u16 width;  // at 0x0
    u16 height; // at 0x2
};

struct TexCoordScale {
    u16 width;  // at 0x0
    u16 height; // at 0x2
    u32 opt;    // at 0x4
};

class TexCoordScaleState {
public:
    TexCoordScaleState() {
        std::memset(this, 0, sizeof(TexCoordScaleState));
    }

    void Invalidate() {
        mUpToDate = 0;
    }

    void UpdateTexMapSize(GXTexMapID map, u16 width, u16 height) {
        wh[map].width = width;
        wh[map].height = height;

        mUpToDate |= FLAG_TEXMAP;
        mUpToDate &= ~FLAG_TEXCOORD;
    }

    void UpdateTable(const ResTev tev) {
        const u32 *pTable = reinterpret_cast<const u32 *>(tev.ref().texCoordToTexMapID);

        if (!(mUpToDate & FLAG_TEXMAP) || pTable[0] != table_data[0] || pTable[1] != table_data[1]) {
            table_data[0] = pTable[0];
            table_data[1] = pTable[1];

            mUpToDate |= FLAG_TEXMAP;
            mUpToDate &= ~FLAG_TEXCOORD;
        }
    }

    void LoadTexCoordScale(u8 texGens) {
        if ((mUpToDate & FLAG_TEXMAP) && !(mUpToDate & FLAG_TEXCOORD) && texGens != 0) {
            LoadTexCoordScale_(texGens);
        }
    }

private:
    enum UpToDateFlag {
        FLAG_TEXCOORD = (1 << 0),
        FLAG_TEXMAP = (1 << 1),
    };

private:
    void LoadTexCoordScale_(u8 texGens);

private:
    u32 mUpToDate;                        // at 0x0
    TexMapSize wh[GX_MAX_TEXMAP];         // at 0x4
    TexCoordScale scale[GX_MAX_TEXCOORD]; // at 0x24

    union {
        u8 table[GX_MAX_TEXCOORD];
        u32 table_data[GX_MAX_TEXCOORD / sizeof(u32)];
    }; // at 0x64
};

TexCoordScaleState sTexCoordScale;

void TexCoordScaleState::LoadTexCoordScale_(u8 texGens) {
    for (u8 i = 0; i < texGens; i++) {
        if (table[i] == GX_TEXMAP_NULL) {
            continue;
        }

        scale[i].width = wh[table[i]].width;
        scale[i].height = wh[table[i]].height;

        fifo::GDSetTexCoordScale2(
            static_cast<GXTexCoordID>(i), scale[i].width, false, false, scale[i].height, false, false
        );
    }

    mUpToDate |= FLAG_TEXCOORD;
}

/******************************************************************************
 *
 * TexState
 *
 ******************************************************************************/
class TexState {
public:
    TexState() : mValidFlag(0) {}

    void Invalidate() {
        mValidFlag = 0;
    }

    void Invalidate(GXTexMapID map) {
        mValidFlag &= ~(1 << map);
    }

    void LoadResTexObj(const ResTexObj texObj) {
        for (u32 i = 0; i < GX_MAX_TEXMAP; i++) {
            if (!texObj.IsValidTexObj(static_cast<GXTexMapID>(i))) {
                continue;
            }

            GXTexMapID id = static_cast<GXTexMapID>(i);
            const GXTexObj *pGXObj = texObj.GetTexObj(id);

            u8 mask = 1 << id;

            if (!(mValidFlag & mask) || !IsEqualTexObj(*pGXObj, mTexObj[id])) {
                mValidFlag |= mask;
                mTexObj[id] = *pGXObj;

                GXLoadTexObj(pGXObj, id);

                sTexCoordScale.UpdateTexMapSize(id, GXGetTexObjWidth(pGXObj), GXGetTexObjHeight(pGXObj));
            }
        }
    }

private:
    GXTexObj mTexObj[GX_MAX_TEXMAP]; // at 0x0
    u8 mValidFlag;                   // at 0x100
    u8 PADDING_0x101;                // at 0x101
    u8 PADDING_0x102;                // at 0x102
    u8 PADDING_0x103;                // at 0x103
};

TexState sTex ALIGN_DECL(32);

/******************************************************************************
 *
 * TlutState
 *
 ******************************************************************************/
class TlutState {
public:
    TlutState() : mFlag(0) {}

    void Invalidate() {
        mFlag = 0;
    }

    void LoadResTlutObj(const ResTlutObj tlutObj) {
        for (u32 i = 0; i < GX_TLUT8; i++) {
            if (!tlutObj.IsValidTlut(static_cast<GXTlut>(i))) {
                continue;
            }

            GXTlut id = static_cast<GXTlut>(i);
            const GXTlutObj *pGXObj = tlutObj.GetTlut(id);

            u16 mask = 1 << id;

            if (!(mFlag & mask) || !IsEqualTlutObj(*pGXObj, mTlutObj[id])) {
                mFlag |= mask;
                mTlutObj[id] = *pGXObj;

                GXLoadTlut(const_cast<GXTlutObj *>(pGXObj), id);
                sTex.Invalidate(static_cast<GXTexMapID>(id));
            }
        }
    }

private:
    GXTlutObj mTlutObj[GX_TLUT7 - GX_TLUT0 + 1]; // at 0x0
    u16 mFlag;                                   // at 0x60
    u16 PADDING_0x62;                            // at 0x62
};

TlutState sTlut ALIGN_DECL(32);

/******************************************************************************
 *
 * ShpState
 *
 ******************************************************************************/
class ShpState {
public:
    ShpState() {
        mCache.Clear();
    }

    void Invalidate() {
        mCache.Clear();
    }

    bool IsCacheEqual(const ResCacheVtxDescv &rOther) {
        if (rOther == mCache) {
            return true;
        }

        mCache = rOther;
        return false;
    }

private:
    ResCacheVtxDescv mCache; // at 0x0
};

ShpState sShp;

/******************************************************************************
 *
 * CurrMtxState
 *
 ******************************************************************************/
class CurrMtxState {
public:
    CurrMtxState() : mIsUpToDate(false) {}

    void Invalidate() {
        mIsUpToDate = false;
    }

    void SetCurrMtx() {
        if (mIsUpToDate) {
            return;
        }

        fifo::GDResetCurrentMtx();
        mIsUpToDate = true;
    }

private:
    bool mIsUpToDate; // at 0x0
};

CurrMtxState sCurrMtx;

/******************************************************************************
 *
 * PostTexMtxState
 *
 ******************************************************************************/
class PostTexMtxState {
public:
    PostTexMtxState() : mFlagIdentity(0) {}

    void Invalidate() {
        mFlagIdentity = 0;
    }

    bool IsIdentity(u32 id) const {
        return mFlagIdentity & (1 << id);
    }

    void SetIdentity(u32 id) {
        mFlagIdentity |= (1 << id);
    }

    void ResetIdentity(u32 id) {
        mFlagIdentity &= ~(1 << id);
    }

private:
    u32 mFlagIdentity; // at 0x0
};

PostTexMtxState sPostTexMtx;

/******************************************************************************
 *
 * PreTexMtxState
 *
 ******************************************************************************/
class PreTexMtxState {
public:
    PreTexMtxState() {
        Invalidate();
    }

    void Invalidate() {
        mType[0] = mType[1] = mType[2] = mType[3] = mType[4] = mType[5] = mType[6] = mType[7] =
            SCNDEPENDENT_TEXMTX_FUNCTYPE_TEXMTX_NOT_EXIST;
    }

    void SetFuncType(u32 id, ScnDependentTexMtxFuncType type) {
        mType[id] = type;
    }

    void SetCurrentMtx(int id) const {
        bool allIdent = true;
        u32 idArray[NUM_TEX_MTX];

        if (id < 0) {
            return;
        }

        for (u32 i = 0; i < NUM_TEX_MTX; i++) {
            if (mType[i] == SCNDEPENDENT_TEXMTX_FUNCTYPE_SRC_NRM) {
                idArray[i] = i * 3 + GX_TEXMTX0;
                fifo::GDLoadTexMtxImm3x3(*GetViewNrmMtxPtr(static_cast<u16>(id)), idArray[i]);
                allIdent = false;
            } else if (mType[i] == SCNDEPENDENT_TEXMTX_FUNCTYPE_SRC_POS) {
                idArray[i] = GX_PNMTX0;
                allIdent = false;
            } else {
                idArray[i] = GX_TEXMTX_IDENT;
            }
        }

        if (allIdent) {
            sCurrMtx.SetCurrMtx();
        } else {
            fifo::GDSetCurrentMtx(idArray);
            sCurrMtx.Invalidate();
        }
    }

private:
    static const int NUM_TEX_MTX = (GX_TEXMTX8 - GX_TEXMTX0) / 3;

private:
    ScnDependentTexMtxFuncType mType[NUM_TEX_MTX]; // at 0x0
};

PreTexMtxState sPreTexMtxState;

/******************************************************************************
 *
 * TevState
 *
 ******************************************************************************/
class TevState {
public:
    TevState() : mpTevData(NULL) {}

    void Invalidate() {
        mpTevData = NULL;
    }

    bool IsTevEqual(const ResTev tev) {
        if (tev.ptr() == mpTevData) {
            return true;
        }

        mpTevData = tev.ptr();
        return false;
    }

private:
    const ResTevData *mpTevData; // at 0x0
};

TevState sTev;

/******************************************************************************
 *
 * PosNrmMtxArrayState
 *
 ******************************************************************************/
class PosNrmMtxArrayState {
public:
    PosNrmMtxArrayState() : mpViewPosMtxArray(NULL), mpViewNrmMtxArray(NULL), mpViewEnvTexMtxArray(NULL) {}

    void Invalidate() {
        mpViewPosMtxArray = NULL;
        mpViewNrmMtxArray = NULL;
        mpViewEnvTexMtxArray = NULL;
    }

    bool IsValid() const {
        return mpViewPosMtxArray != NULL;
    }

    void SetViewPosNrmMtxArray(
        const math::MTX34 *pViewPosMtxArray, const math::MTX33 *pViewNrmMtxArray, const math::MTX34 *pViewEnvTexMtxArray
    ) {
        mpViewPosMtxArray = pViewPosMtxArray;
        mpViewNrmMtxArray = pViewNrmMtxArray;
        mpViewEnvTexMtxArray = pViewEnvTexMtxArray;

        if (mpViewPosMtxArray != NULL) {
            GXSetArray(GX_POS_MTX_ARRAY, mpViewPosMtxArray, sizeof(math::MTX34));
        }

        if (mpViewNrmMtxArray != NULL) {
            GXSetArray(GX_NRM_MTX_ARRAY, mpViewNrmMtxArray, sizeof(math::MTX33));
        }

        if (mpViewEnvTexMtxArray != NULL) {
            GXSetArray(GX_TEX_MTX_ARRAY, mpViewEnvTexMtxArray, sizeof(math::MTX34));
        }
    }

    const math::MTX34 *GetViewPosMtxPtr(u32 id) const {
        if (mpViewPosMtxArray != NULL) {
            return &mpViewPosMtxArray[id];
        }

        return NULL;
    }

    const math::MTX33 *GetViewNrmMtxPtr(u32 id) const {
        if (mpViewNrmMtxArray != NULL) {
            return &mpViewNrmMtxArray[id];
        }

        static math::MTX33 m;

        if (!math::MTX34InvTranspose(&m, GetViewPosMtxPtr(id))) {
            m._00 = m._11 = m._22 = 1.0f;
            m._20 = m._21 = 0.0f;
            m._10 = m._12 = 0.0f;
            m._01 = m._02 = 0.0f;
        }

        return &m;
    }

private:
    const math::MTX34 *mpViewPosMtxArray;    // at 0x0
    const math::MTX33 *mpViewNrmMtxArray;    // at 0x4
    const math::MTX34 *mpViewEnvTexMtxArray; // at 0x8
};

PosNrmMtxArrayState sPosNrmArrayState;

/******************************************************************************
 *
 * ScnDependentTexMtxFuncTable
 *
 ******************************************************************************/
class ScnDependentTexMtxFuncTable {
public:
    ScnDependentTexMtxFuncTable() {
        mTable[0].func = detail::ScnDependentMtxFunc::DefaultMapping;
        mTable[0].type = SCNDEPENDENT_TEXMTX_FUNCTYPE_SRC_TEXCOORD;

        mTable[1].func = detail::ScnDependentMtxFunc::EnvironmentMapping;
        mTable[1].type = SCNDEPENDENT_TEXMTX_FUNCTYPE_SRC_NRM;

        mTable[2].func = detail::ScnDependentMtxFunc::ProjectionMapping;
        mTable[2].type = SCNDEPENDENT_TEXMTX_FUNCTYPE_SRC_POS;

        mTable[3].func = detail::ScnDependentMtxFunc::EnvironmentMapping;
        mTable[3].type = SCNDEPENDENT_TEXMTX_FUNCTYPE_SRC_NRM;

        mTable[4].func = detail::ScnDependentMtxFunc::EnvironmentSpecularMapping;
        mTable[4].type = SCNDEPENDENT_TEXMTX_FUNCTYPE_SRC_NRM;

        for (u32 i = 5; i < NUM_SCNDEPENDENT_TEXMTX_FUNCTYPE; i++) {
            mTable[i].func = detail::ScnDependentMtxFunc::DefaultMapping;
            mTable[i].type = SCNDEPENDENT_TEXMTX_FUNCTYPE_SRC_TEXCOORD;
        }
    }

    void SetFunc(u32 id, ScnDependentTexMtxFuncPtr func, ScnDependentTexMtxFuncType type) {
        if (id <= 4 || id >= NUM_SCNDEPENDENT_TEXMTX_FUNCTYPE) {
            return;
        }

        if (func != NULL) {
            if (type >= MAX_SCNDEPENDENT_TEXMTX_FUNCTYPE) {
                type = SCNDEPENDENT_TEXMTX_FUNCTYPE_SRC_TEXCOORD;
            }

            mTable[id].func = func;
            mTable[id].type = type;
        } else {
            mTable[id].func = detail::ScnDependentMtxFunc::DefaultMapping;
            mTable[id].type = SCNDEPENDENT_TEXMTX_FUNCTYPE_SRC_TEXCOORD;
        }
    }

    bool GetFunc(u32 id, ScnDependentTexMtxFuncPtr *pFunc, ScnDependentTexMtxFuncType *pType) {
        if (id < NUM_SCNDEPENDENT_TEXMTX_FUNCTYPE) {
            if (pFunc != NULL) {
                *pFunc = mTable[id].func;
            }

            if (pType != NULL) {
                *pType = mTable[id].type;
            }

            return true;
        }

        return false;
    }

    ScnDependentTexMtxFuncType GetFuncType(u32 id) {
        if (id >= NUM_SCNDEPENDENT_TEXMTX_FUNCTYPE) {
            id = 0;
        }

        return mTable[id].type;
    }

    void Calc(u32 id, math::MTX34 *pMtx, s8 camRef, s8 lightRef) {
        if (id >= NUM_SCNDEPENDENT_TEXMTX_FUNCTYPE) {
            id = 0;
        }

        if (camRef >= NUM_CAMERA) {
            camRef = -1;
        }

        if (lightRef >= NUM_LIGHT) {
            lightRef = -1;
        }

        mTable[id].func(pMtx, camRef, lightRef);
    }

private:
    struct {
        ScnDependentTexMtxFuncPtr func;         // at 0x0
        ScnDependentTexMtxFuncType type;        // at 0x4
    } mTable[NUM_SCNDEPENDENT_TEXMTX_FUNCTYPE]; // at 0x0
};

ScnDependentTexMtxFuncTable sScnDependentTexMtxFuncTable;

/******************************************************************************
 *
 * FogState
 *
 ******************************************************************************/
class FogState {
public:
    FogState() : mFlag(0), mCurrentFogIdx(-1) {
        for (u32 i = 0; i < NUM_FOG; i++) {
            Fog(&mFogData[i]).Init();
        }
    }

    void Invalidate() {
        mFlag = 0;
    }

    void SetFog(const Fog fog, int id) {
        if (id < 0 || id >= NUM_FOG || !fog.IsValid()) {
            return;
        }

        if (mCurrentFogIdx == id) {
            if (std::memcmp(&fog.ref(), &mFogData[id], sizeof(FogData)) == 0) {
                return;
            }

            Invalidate();
        }

        fog.CopyTo(&mFogData[id]);
    }

    void LoadFog(int id) {
        if (!(mFlag & FLAG_0) || !(mFlag & FLAG_1) || id != mCurrentFogIdx) {
            if (id < 0 || id >= NUM_FOG) {
                GXColor color = {0, 0, 0, 0};
                GXSetFog(GX_FOG_NONE, color, 0.0f, 0.0f, 0.0f, 0.0f);
            } else {
                Fog(&mFogData[id]).SetGP();
            }

            mFlag |= FLAG_0 | FLAG_1;
            mCurrentFogIdx = id;
        }
    }

private:
    enum Flag {
        FLAG_0 = (1 << 0),
        FLAG_1 = (1 << 1),
    };

private:
    u32 mFlag;                 // at 0x0
    int mCurrentFogIdx;        // at 0x4
    FogData mFogData[NUM_FOG]; // at 0x8
};

FogState sFogState;

/******************************************************************************
 *
 * LightState
 *
 ******************************************************************************/
class LightState {
public:
    LightState();

    void Invalidate();

    void SetLightSetting(const LightSetting &rSetting);
    const LightObj *GetLightObj(int id) const;
    void LoadLightSet(
        int id, u32 *pDiffColorMask, u32 *pDiffAlphaMask, u32 *pSpecColorMask, u32 *pSpecAlphaMask, AmbLightObj *pAmb
    );

private:
    LightSetting mLightSetting;                 // at 0x0
    int mCurrentLightSetIdx;                    // at 0x10
    u32 mCurrentMaskDiffColor;                  // at 0x14
    u32 mCurrentMaskDiffAlpha;                  // at 0x18
    u32 mCurrentMaskSpecColor;                  // at 0x1C
    u32 mCurrentMaskSpecAlpha;                  // at 0x20
    s8 mLoadedLightIdx[NUM_LIGHT_IN_LIGHT_SET]; // at 0x24
    LightObj mLightObj[NUM_LIGHT];              // at 0x2C
    AmbLightObj mAmbLightObj[NUM_LIGHT];        // at 0x222C
    LightSetData mLightSetData[NUM_LIGHT_SET];  // at 0x242C
};

LightState sLightState;

LightState::LightState() : mLightSetting(mLightObj, mAmbLightObj, NUM_LIGHT, mLightSetData, NUM_LIGHT_SET) {
    Invalidate();
}

void LightState::Invalidate() {
    mCurrentLightSetIdx = -1;

    mCurrentMaskDiffColor = 0;
    mCurrentMaskDiffAlpha = 0;

    mCurrentMaskSpecColor = 0;
    mCurrentMaskSpecAlpha = 0;

    mLoadedLightIdx[0] = mLoadedLightIdx[1] = mLoadedLightIdx[2] = mLoadedLightIdx[3] = mLoadedLightIdx[4] =
        mLoadedLightIdx[5] = mLoadedLightIdx[6] = mLoadedLightIdx[7] = -1;
}

void LightState::SetLightSetting(const LightSetting &rSetting) {
    mLightSetting.Import(rSetting);

    const math::MTX34 *pCamMtx = GetCameraMtxPtr();
    mLightSetting.ApplyViewMtx(*pCamMtx, rSetting.GetNumLightObj());
    Invalidate();

    u32 lightNum = mLightSetting.GetNumLightObj();

    u32 i = 0;
    u32 idx = 0;

    for (; i < G3DState::NUM_LIGHT_IN_LIGHT_SET && idx < lightNum; idx++) {
        const LightObj &rLight = mLightSetting.GetLightObjArray()[idx];
        if (!rLight.IsEnable()) {
            continue;
        }

        mLoadedLightIdx[i] = idx;

        GXLightID id = static_cast<GXLightID>(1 << i);
        GXLoadLightObjImm(rLight, id);

        i++;
    }
}

const LightObj *LightState::GetLightObj(int id) const {
    if (id >= 0 && id < NUM_LIGHT) {
        return &mLightObj[id];
    }

    return NULL;
}

void LightState::LoadLightSet(
    int id, u32 *pDiffColorMask, u32 *pDiffAlphaMask, u32 *pSpecColorMask, u32 *pSpecAlphaMask, AmbLightObj *pAmb
) {
    if (id < 0 || id >= NUM_LIGHT_SET) {
        if (pDiffColorMask != NULL) {
            *pDiffColorMask = 0;
        }

        if (pDiffAlphaMask != NULL) {
            *pDiffAlphaMask = 0;
        }

        if (pSpecColorMask != NULL) {
            *pSpecColorMask = 0;
        }

        if (pSpecAlphaMask != NULL) {
            *pSpecAlphaMask = 0;
        }

        if (pAmb != NULL) {
            pAmb->r = pAmb->g = pAmb->b = pAmb->a = 255;
        }

        return;
    }

    if (pAmb != NULL) {
        int ambId = mLightSetData[id].idxAmbLight;

        if (ambId < 0) {
            pAmb->r = pAmb->g = pAmb->b = pAmb->a = 255;
        } else {
            *pAmb = mAmbLightObj[ambId];
        }
    }

    if (mCurrentLightSetIdx == id) {
        if (pDiffColorMask != NULL) {
            *pDiffColorMask = mCurrentMaskDiffColor;
        }

        if (pDiffAlphaMask != NULL) {
            *pDiffAlphaMask = mCurrentMaskDiffAlpha;
        }

        if (pSpecColorMask != NULL) {
            *pSpecColorMask = mCurrentMaskSpecColor;
        }

        if (pSpecAlphaMask != NULL) {
            *pSpecAlphaMask = mCurrentMaskSpecAlpha;
        }

        return;
    }

    LightSetData &rData = mLightSetData[id];

    mCurrentMaskDiffColor = 0;
    mCurrentMaskDiffAlpha = 0;

    mCurrentMaskSpecColor = 0;
    mCurrentMaskSpecAlpha = 0;

    mCurrentLightSetIdx = id;

    for (u32 i = 0; i < NUM_LIGHT_IN_LIGHT_SET; i++) {
        s8 lightId = rData.idxLight[i];
        if (lightId < 0) {
            continue;
        }

        if (!mLightObj[lightId].IsEnable()) {
            continue;
        }

        s8 *pLoaded = std::find(mLoadedLightIdx, mLoadedLightIdx + NUM_LIGHT_IN_LIGHT_SET, lightId);

        GXLightID idFlag;

        if (pLoaded == mLoadedLightIdx + NUM_LIGHT_IN_LIGHT_SET) {
            pLoaded = std::find(mLoadedLightIdx, mLoadedLightIdx + NUM_LIGHT_IN_LIGHT_SET, -1);

            if (pLoaded != mLoadedLightIdx + NUM_LIGHT_IN_LIGHT_SET) {
                idFlag = static_cast<GXLightID>(1 << std::distance(mLoadedLightIdx, pLoaded));

                GXLoadLightObjImm(mLightObj[lightId], idFlag);
                *pLoaded = lightId;
            } else {
                u32 j;

                for (j = 0; j < NUM_LIGHT_IN_LIGHT_SET; j++) {
                    s8 *pResult =
                        std::find(rData.idxLight, rData.idxLight + NUM_LIGHT_IN_LIGHT_SET, mLoadedLightIdx[j]);

                    if (pResult == rData.idxLight + NUM_LIGHT_IN_LIGHT_SET) {
                        break;
                    }
                }

                idFlag = static_cast<GXLightID>(1 << j);

                GXLoadLightObjImm(mLightObj[lightId], idFlag);
                mLoadedLightIdx[j] = lightId;
            }

        } else {
            idFlag = static_cast<GXLightID>(1 << std::distance(mLoadedLightIdx, pLoaded));
        }

        if (mLightObj[lightId].IsSpecularLight()) {
            if (mLightObj[lightId].IsColorEnable()) {
                mCurrentMaskSpecColor |= idFlag;
            }

            if (mLightObj[lightId].IsAlphaEnable()) {
                mCurrentMaskSpecAlpha |= idFlag;
            }
        } else {
            if (mLightObj[lightId].IsColorEnable()) {
                mCurrentMaskDiffColor |= idFlag;
            }

            if (mLightObj[lightId].IsAlphaEnable()) {
                mCurrentMaskDiffAlpha |= idFlag;
            }
        }
    }

    if (pDiffColorMask != NULL) {
        *pDiffColorMask = mCurrentMaskDiffColor;
    }

    if (pDiffAlphaMask != NULL) {
        *pDiffAlphaMask = mCurrentMaskDiffAlpha;
    }

    if (pSpecColorMask != NULL) {
        *pSpecColorMask = mCurrentMaskSpecColor;
    }

    if (pSpecAlphaMask != NULL) {
        *pSpecAlphaMask = mCurrentMaskSpecAlpha;
    }
}

/******************************************************************************
 *
 * CameraMtxState
 *
 ******************************************************************************/
class CameraMtxState {
public:
    CameraMtxState();

    void SetCameraProjMtx(const Camera &rCam, int id, bool view);

    const math::MTX34 *GetInvCameraMtxPtr() const {
        if (!(mFlag & FLAG_INV_MTX_READY)) {
            math::MTX34Inv(&mInvCurrentCameraMtx, GetCameraMtxPtr());
            mFlag |= FLAG_INV_MTX_READY;
        }

        return &mInvCurrentCameraMtx;
    }

    const math::MTX34 *GetCameraMtxPtr() const {
        return &mCameraMtx[mViewIdx];
    }
    const math::MTX34 *GetCameraMtxPtr(int id) const {
        if (id < NUM_CAMERA && id >= 0) {
            return &mCameraMtx[id];
        }

        return NULL;
    }

    const math::MTX44 *GetProjectionMtxPtr() const {
        return &mProjMtx[mViewIdx];
    }
    const math::MTX44 *GetProjectionMtxPtr(int id) const {
        if (id < NUM_CAMERA && id >= 0) {
            return &mProjMtx[id];
        }

        return NULL;
    }

    const math::MTX34 *GetProjectionTexMtxPtr() const {
        return &mProjTexMtx[mViewIdx];
    }
    const math::MTX34 *GetProjectionTexMtxPtr(int id) const {
        if (id < NUM_CAMERA && id >= 0) {
            return &mProjTexMtx[id];
        }

        return NULL;
    }

    const math::MTX34 *GetEnvironmentTexMtxPtr() const {
        return &mEnvTexMtx[mViewIdx];
    }

private:
    enum Flag {
        FLAG_INV_MTX_READY = (1 << 0),
    };

private:
    mutable u16 mFlag;                        // at 0x0
    u16 mViewIdx;                             // at 0x2
    mutable math::MTX34 mInvCurrentCameraMtx; // at 0x4
    math::MTX34 mCameraMtx[NUM_CAMERA];       // at 0x34
    math::MTX44 mProjMtx[NUM_CAMERA];         // at 0x634
    math::MTX34 mProjTexMtx[NUM_CAMERA];      // at 0xE34
    math::MTX34 mEnvTexMtx[NUM_CAMERA];       // at 0x1434
};

CameraMtxState sCameraMtxState;

CameraMtxState::CameraMtxState() : mFlag(0), mViewIdx(0) {
    math::MTX34Identity(&mInvCurrentCameraMtx);

    for (u32 i = 0; i < NUM_CAMERA; i++) {
        math::MTX34Identity(&mCameraMtx[i]);
        math::MTX44Identity(&mProjMtx[i]);
        math::MTX34Identity(&mProjTexMtx[i]);
        math::MTX34Identity(&mEnvTexMtx[i]);
    }
}

void CameraMtxState::SetCameraProjMtx(const Camera &rCam, int id, bool view) {
    if (id >= NUM_CAMERA || id < 0) {
        return;
    }

    rCam.GetCameraMtx(&mCameraMtx[id]);
    rCam.GetProjectionMtx(&mProjMtx[id]);
    rCam.GetProjectionTexMtx(&mProjTexMtx[id]);
    rCam.GetEnvironmentTexMtx(&mEnvTexMtx[id]);

    if (view) {
        mViewIdx = static_cast<u16>(id);
        mFlag &= ~FLAG_INV_MTX_READY;
    }
}

/******************************************************************************
 *
 * Render mode
 *
 ******************************************************************************/
GXRenderModeObj sRenderMode;

/******************************************************************************
 *
 * Misc
 *
 ******************************************************************************/
class Misc {
public:
    Misc() : mValidFlag(0), mbZCompLoc(FALSE) {}

    void Invalidate() {
        InvalidateZCompLoc();
    }

    void InvalidateZCompLoc() {
        mValidFlag &= ~FLAG_ZCOMPLOC;
    }

    void GXSetZCompLoc(GXBool beforeTex) {
        if (!(mValidFlag & FLAG_ZCOMPLOC) || mbZCompLoc != beforeTex) {
            mValidFlag |= FLAG_ZCOMPLOC;
            mbZCompLoc = beforeTex;

            ::GXSetZCompLoc(beforeTex);
            sSyncGX.Set();
        }
    }

private:
    enum ValidFlag {
        FLAG_ZCOMPLOC = (1 << 0),
    };

private:
    u32 mValidFlag;    // at 0x0
    GXBool mbZCompLoc; // at 0x4
};

Misc sMisc;

} // namespace

/******************************************************************************
 *
 * G3DState
 *
 ******************************************************************************/
namespace {

IndMtxOpStd IndMtxOpDefault;
IndMtxOp *pG3DStateIndMtxOp = &IndMtxOpDefault;

} // namespace

void LoadResMatMisc(const ResMatMisc misc) {
    if (!misc.IsValid()) {
        return;
    }

    sMisc.GXSetZCompLoc(misc.GXGetZCompLoc());
}

void LoadResTexObj(const ResTexObj texObj) {
    if (!texObj.IsValid()) {
        return;
    }

    sTex.LoadResTexObj(texObj);
    sSyncGX.Set();
}

void LoadResTlutObj(const ResTlutObj tlutObj) {
    if (!tlutObj.IsValid()) {
        return;
    }

    sTlut.LoadResTlutObj(tlutObj);
    sSyncGX.Set();
}

void LoadResGenMode(const ResGenMode mode) {
    if (!mode.IsValid()) {
        return;
    }

    sGenMode2.SetResGenMode(mode);
}

void LoadResTev(const ResTev tev) {
    if (!tev.IsValid()) {
        return;
    }

    if (sTev.IsTevEqual(tev)) {
        return;
    }

    sGenMode2.LoadBP();
    tev.CallDisplayList(sSyncGX.Get());

    sTexCoordScale.UpdateTable(tev);
    sTexCoordScale.LoadTexCoordScale(sGenMode2.GXGetNumTexGens());
}

void LoadResMatPix(const ResMatPix pix) {
    if (!pix.IsValid()) {
        return;
    }

    sGenMode2.LoadBP();
    pix.CallDisplayList(sSyncGX.Get());
}

void LoadResMatTevColor(const ResMatTevColor color) {
    if (!color.IsValid()) {
        return;
    }

    sGenMode2.LoadBP();
    color.CallDisplayList(sSyncGX.Get());
}

void LoadResMatIndMtxAndScale(const ResMatIndMtxAndScale ind) {
    if (!ind.IsValid()) {
        return;
    }

    sGenMode2.LoadBP();
    ind.CallDisplayList(sGenMode2.GXGetNumIndStages(), sSyncGX.Get());
}

void LoadResMatIndMtxAndScale(const ResMatIndMtxAndScale ind, IndMtxOp &rOp) {
    if (!ind.IsValid()) {
        return;
    }

    LoadResMatIndMtxAndScale(ind);

    IndTexMtxInfo info(ind);
    rOp(&info);
    info.FifoSend();
}

void LoadResMatChan(
    const ResMatChan chan, u32 maskDiffColor, u32 maskDiffAlpha, u32 maskSpecColor, u32 maskSpecAlpha, GXColor amb,
    bool lightOff
) {
    if (!chan.IsValid()) {
        return;
    }

    {
        const Chan &r = chan.ref().chan[0];

        if (r.flag & Chan::FLAG_MAT_COLOR_ENABLE) {
            if (r.flag & Chan::FLAG_MAT_ALPHA_ENABLE) {
                // @bug Color+Alpha channel is GX_COLOR0A0
                fifo::GDSetChanMatColor(GX_COLOR0, r.matColor);
            } else {
                fifo::GDSetChanMatColor(GX_COLOR0, r.matColor);
            }
        } else if (r.flag & Chan::FLAG_MAT_ALPHA_ENABLE) {
            // @bug Alpha channel is GX_ALPHA0
            fifo::GDSetChanMatColor(GX_COLOR0, r.matColor);
        }

        GXColor ambColor;
        ambColor.r = 0.5f + (amb.r * r.ambColor.r) * (1.0f / 255.0f);
        ambColor.g = 0.5f + (amb.g * r.ambColor.g) * (1.0f / 255.0f);
        ambColor.b = 0.5f + (amb.b * r.ambColor.b) * (1.0f / 255.0f);
        ambColor.a = 0.5f + (amb.a * r.ambColor.a) * (1.0f / 255.0f);

        if (r.flag & Chan::FLAG_AMB_COLOR_ENABLE) {
            if (r.flag & Chan::FLAG_AMB_ALPHA_ENABLE) {
                // @bug Color+Alpha channel is GX_COLOR0A0
                fifo::GDSetChanAmbColor(GX_COLOR0, ambColor);
            } else {
                fifo::GDSetChanAmbColor(GX_COLOR0, ambColor);
            }
        } else if (r.flag & Chan::FLAG_AMB_ALPHA_ENABLE) {
            // @bug Alpha channel is GX_ALPHA0
            fifo::GDSetChanAmbColor(GX_COLOR0, ambColor);
        }

        if (lightOff) {
            if (r.flag & Chan::FLAG_CTRL_COLOR_ENABLE) {
                fifo::GDSetChanCtrlLightOff(GX_COLOR0, r.paramChanCtrlC, maskDiffColor);
            }

            if (r.flag & Chan::FLAG_CTRL_ALPHA_ENABLE) {
                fifo::GDSetChanCtrlLightOff(GX_ALPHA0, r.paramChanCtrlA, maskDiffAlpha);
            }
        } else {
            if (r.flag & Chan::FLAG_CTRL_COLOR_ENABLE) {
                fifo::GDSetChanCtrl(GX_COLOR0, r.paramChanCtrlC, maskDiffColor);
            }

            if (r.flag & Chan::FLAG_CTRL_ALPHA_ENABLE) {
                fifo::GDSetChanCtrl(GX_ALPHA0, r.paramChanCtrlA, maskDiffAlpha);
            }
        }
    }

    {
        if (sGenMode2.GXGetNumChans() == 2) {
            const Chan &r = chan.ref().chan[1];

            if (r.flag & Chan::FLAG_MAT_COLOR_ENABLE) {
                if (r.flag & Chan::FLAG_MAT_ALPHA_ENABLE) {
                    // @bug Color+Alpha channel is GX_COLOR1A1
                    fifo::GDSetChanMatColor(GX_COLOR1, r.matColor);
                } else {
                    fifo::GDSetChanMatColor(GX_COLOR1, r.matColor);
                }
            } else if (r.flag & Chan::FLAG_MAT_ALPHA_ENABLE) {
                // @bug Alpha channel is GX_ALPHA1
                fifo::GDSetChanMatColor(GX_COLOR1, r.matColor);
            }

            if (r.flag & Chan::FLAG_AMB_COLOR_ENABLE) {
                if (r.flag & Chan::FLAG_AMB_ALPHA_ENABLE) {
                    // @bug Color+Alpha channel is GX_COLOR1A1
                    fifo::GDSetChanAmbColor(GX_COLOR1, r.ambColor);
                } else {
                    fifo::GDSetChanAmbColor(GX_COLOR1, r.ambColor);
                }
            } else if (r.flag & Chan::FLAG_AMB_ALPHA_ENABLE) {
                // @bug Alpha channel is GX_ALPHA1
                fifo::GDSetChanAmbColor(GX_COLOR1, r.ambColor);
            }

            if (lightOff) {
                if (r.flag & Chan::FLAG_CTRL_COLOR_ENABLE) {
                    fifo::GDSetChanCtrlLightOff(GX_COLOR1, r.paramChanCtrlC, maskSpecColor);
                }

                if (r.flag & Chan::FLAG_CTRL_ALPHA_ENABLE) {
                    fifo::GDSetChanCtrlLightOff(GX_ALPHA1, r.paramChanCtrlA, maskSpecAlpha);
                }
            } else {
                if (r.flag & Chan::FLAG_CTRL_COLOR_ENABLE) {
                    fifo::GDSetChanCtrl(GX_COLOR1, r.paramChanCtrlC, maskSpecColor);
                }

                if (r.flag & Chan::FLAG_CTRL_ALPHA_ENABLE) {
                    fifo::GDSetChanCtrl(GX_ALPHA1, r.paramChanCtrlA, maskSpecAlpha);
                }
            }
        } else {
            fifo::GDSetChanCtrl(GX_COLOR1, 0, 0);
            fifo::GDSetChanCtrl(GX_ALPHA1, 0, 0);
        }
    }

    sGenMode2.LoadXF();
}

void LoadResMatTexCoordGen(const ResMatTexCoordGen gen) {
    if (!gen.IsValid()) {
        return;
    }

    gen.CallDisplayList(sGenMode2.GXGetNumTexGens(), sSyncGX.Get());
    sGenMode2.LoadXF();
}

void LoadResTexSrt(const ResTexSrt srt) {
    math::MTX34 mtx;

    for (u32 i = 0; i < ResTexSrtData::NUM_OF_TEXTURE; i++) {
        if (srt.IsExist(i)) {
            bool ident = true;
            const TexMtxEffect &rEffect = srt.ref().effect[i];

            math::MTX34Identity(&mtx);

            if (rEffect.map_mode != 0) {
                sScnDependentTexMtxFuncTable.Calc(rEffect.map_mode, &mtx, rEffect.ref_camera, rEffect.ref_light);

                sPreTexMtxState.SetFuncType(i, sScnDependentTexMtxFuncTable.GetFuncType(rEffect.map_mode));

                ident = false;
            } else {
                sPreTexMtxState.SetFuncType(i, SCNDEPENDENT_TEXMTX_FUNCTYPE_TEXMTX_NOT_EXIST);
            }

            if (!srt.IsIdentity(i)) {
                if (ident) {
                    if (!(rEffect.misc_flag & TexMtxEffect::FLAG_IDENT)) {
                        ident = false;

                        math::MTX34Copy(&mtx, static_cast<const math::MTX34 *>(&rEffect.effectMtx));
                    }
                } else {
                    math::MTX34Mult(&mtx, static_cast<const math::MTX34 *>(&rEffect.effectMtx), &mtx);
                }

                if (rEffect.map_mode == 0) {
                    CalcTexMtx(&mtx, ident, srt.ref().texSrt[i], srt.GetTexSrtFlag(i), srt.GetTexMtxMode());
                } else {
                    math::MTX34 srtMtx;
                    CalcTexMtx(&srtMtx, true, srt.ref().texSrt[i], srt.GetTexSrtFlag(i), srt.GetTexMtxMode());

                    std::swap(srtMtx._02, srtMtx._03);
                    std::swap(srtMtx._12, srtMtx._13);

                    math::MTX34Mult(&mtx, &srtMtx, &mtx);
                }

                ident = false;
            }

            if (ident) {
                if (!sPostTexMtx.IsIdentity(i)) {
                    math::MTX34 identMtx;

                    sPostTexMtx.SetIdentity(i);
                    math::MTX34Identity(&identMtx);

                    GXLoadTexMtxImm(identMtx, i * 3 + GX_DUALMTX0, GX_MTX_3x4);
                }
            } else {
                sPostTexMtx.ResetIdentity(i);
                GXLoadTexMtxImm(mtx, i * 3 + GX_DUALMTX0, GX_MTX_3x4);
            }

        } else {
            sPreTexMtxState.SetFuncType(i, SCNDEPENDENT_TEXMTX_FUNCTYPE_TEXMTX_NOT_EXIST);
        }
    }
}

void LoadResShpPrePrimitive(const ResShp shp) {
    if (!shp.IsValid()) {
        return;
    }

    sTexCoordScale.LoadTexCoordScale(sGenMode2.GXGetNumTexGens());
    sGenMode2.LoadXF();
    shp.CallPrePrimitiveDisplayList(sSyncGX.Get(), sShp.IsCacheEqual(shp.ref().cache));
}

void LoadResShpPrimitive(const ResShp shp, const math::MTX34 *pViewPos, const math::MTX34 *pViewNrm) {
    if (!shp.IsValid()) {
        return;
    }

    sTexCoordScale.LoadTexCoordScale(sGenMode2.GXGetNumTexGens());

    int id = shp.ref().curMtxIdx;
    bool envelope = (id & ResShpData::ID_FLAG_ENVELOPE) != 0;

    if (!envelope) {
        if (pViewPos != NULL) {
            GXLoadPosMtxImm(*pViewPos, GX_PNMTX0);

            if (shp.ExistVtxDesc(GX_VA_NRM)) {
                if (pViewNrm != NULL) {
                    GXLoadNrmMtxImm(*pViewNrm, GX_PNMTX0);
                } else {
                    GXLoadNrmMtxImm(*pViewPos, GX_PNMTX0);
                }
            }
        } else {
            GXLoadPosMtxIndx(id, GX_PNMTX0);

            if (shp.ExistVtxDesc(GX_VA_NRM)) {
                GXLoadNrmMtxIndx3x3(id, GX_PNMTX0);
            }
        }

        sPreTexMtxState.SetCurrentMtx(id);
    } else {
        sCurrMtx.SetCurrMtx();
    }

    sGenMode2.LoadXF();
    shp.CallPrimitiveDisplayList(sSyncGX.Get());

    if (envelope) {
        sCurrMtx.Invalidate();
    }
}

void SetViewPosNrmMtxArray(
    const math::MTX34 *pViewPosMtxArray, const math::MTX33 *pViewNrmMtxArray, const math::MTX34 *pViewEnvTexMtxArray
) {
    sPosNrmArrayState.SetViewPosNrmMtxArray(pViewPosMtxArray, pViewNrmMtxArray, pViewEnvTexMtxArray);
}

const math::MTX33 *GetViewNrmMtxPtr(u32 id) {
    return sPosNrmArrayState.GetViewNrmMtxPtr(id);
}

void SetScnDependentTexMtxFunc(u32 id, ScnDependentTexMtxFuncPtr func, ScnDependentTexMtxFuncType type) {
    sScnDependentTexMtxFuncTable.SetFunc(id, func, type);
}

bool GetScnDependentTexMtxFunc(u32 id, ScnDependentTexMtxFuncPtr *pFunc, ScnDependentTexMtxFuncType *pType) {
    return sScnDependentTexMtxFuncTable.GetFunc(id, pFunc, pType);
}

IndMtxOp *GetIndMtxOp() {
    return pG3DStateIndMtxOp;
}

void SetFog(const Fog fog, int id) {
    sFogState.SetFog(fog, id);
}

void LoadFog(int id) {
    sFogState.LoadFog(id);
}

void SetLightSetting(const LightSetting &rSetting) {
    sLightState.SetLightSetting(rSetting);
}

const LightObj *GetLightObj(int id) {
    return sLightState.GetLightObj(id);
}

void LoadLightSet(
    int id, u32 *pDiffColorMask, u32 *pDiffAlphaMask, u32 *pSpecColorMask, u32 *pSpecAlphaMask, AmbLightObj *pAmb
) {
    sLightState.LoadLightSet(id, pDiffColorMask, pDiffAlphaMask, pSpecColorMask, pSpecAlphaMask, pAmb);
}

void LoadLightSet(int id, u32 *pDiffMask, u32 *pSpecMask, AmbLightObj *pAmb) {
    u32 diffAlphaMask, specAlphaMask;

    sLightState.LoadLightSet(id, pDiffMask, &diffAlphaMask, pSpecMask, &specAlphaMask, pAmb);

    if (pDiffMask != NULL) {
        *pDiffMask |= diffAlphaMask;
    }

    if (pSpecMask != NULL) {
        *pSpecMask |= specAlphaMask;
    }
}

void SetCameraProjMtx(const Camera &rCam, int id, bool view) {
    sCameraMtxState.SetCameraProjMtx(rCam, id, view);
}

const math::MTX34 *GetCameraMtxPtr() {
    return sCameraMtxState.GetCameraMtxPtr();
}

const math::MTX34 *GetInvCameraMtxPtr() {
    return sCameraMtxState.GetInvCameraMtxPtr();
}

const math::MTX34 *GetCameraMtxPtr(int id) {
    return sCameraMtxState.GetCameraMtxPtr(id);
}

const math::MTX34 *GetProjectionTexMtxPtr() {
    return sCameraMtxState.GetProjectionTexMtxPtr();
}

const math::MTX34 *GetProjectionTexMtxPtr(int id) {
    return sCameraMtxState.GetProjectionTexMtxPtr(id);
}

const math::MTX34 *GetEnvironmentTexMtxPtr() {
    return sCameraMtxState.GetEnvironmentTexMtxPtr();
}

void SetRenderModeObj(const GXRenderModeObj &rObj) {
    sRenderMode = rObj;
}

const GXRenderModeObj *GetRenderModeObj() {
    return &sRenderMode;
}

void Invalidate(u32 flag) {
    if (flag & INVALIDATE_TEX) {
        sTex.Invalidate();
        sTexCoordScale.Invalidate();
    }

    if (flag & INVALIDATE_TLUT) {
        sTlut.Invalidate();
    }

    if (flag & INVALIDATE_TEV) {
        sTexCoordScale.Invalidate();
        sTev.Invalidate();
    }

    if (flag & INVALIDATE_GENMODE) {
        sGenMode2.Invalidate();
    }

    if (flag & INVALIDATE_SHP) {
        sShp.Invalidate();
    }

    if (flag & INVALIDATE_CURRMTX) {
        sCurrMtx.Invalidate();
    }

    if (flag & INVALIDATE_TEXMTX) {
        // @bug Missing PreTexMtxState::Invalidate
        sPostTexMtx.Invalidate();
    }

    if (flag & INVALIDATE_MISC) {
        sMisc.Invalidate();
    }

    if (flag & INVALIDATE_FOG) {
        sFogState.Invalidate();
    }

    if (flag & INVALIDATE_LIGHT) {
        sLightState.Invalidate();
    }

    if (flag & INVALIDATE_POSMTX) {
        sPosNrmArrayState.Invalidate();
    }

    sSyncGX.Invalidate();
}

/******************************************************************************
 *
 * IndMtxOpStd
 *
 ******************************************************************************/
IndMtxOpStd::IndMtxOpStd() {
    math::MTX34Identity(&mIndMtx[GX_ITM_0 - 1]);
    math::MTX34Identity(&mIndMtx[GX_ITM_1 - 1]);
    math::MTX34Identity(&mIndMtx[GX_ITM_2 - 1]);

    mIsValidMtx[GX_ITM_0 - 1] = mIsValidMtx[GX_ITM_1 - 1] = mIsValidMtx[GX_ITM_2 - 1] = false;

    PADDING_0x7 = 0;
}

void IndMtxOpStd::SetNrmMapMtx(
    GXIndTexMtxID id, const math::VEC3 *pLightVec, const math::MTX34 *pNrmMtx, ResMatMiscData::IndirectMethod method
) {
    if (id >= GX_ITM_0 && id <= GX_ITM_2 && method != ResMatMiscData::WARP) {
        u32 i = id - GX_ITM_0;
        mIsValidMtx[i] = true;

        if (pLightVec != NULL) {
            mIndMtx[i]._00 = 0.5f * -pLightVec->x;
            mIndMtx[i]._01 = 0.5f * -pLightVec->y;
            mIndMtx[i]._02 = 0.5f * -pLightVec->z;

            if (method == ResMatMiscData::NORMAL_MAP_SPECULAR) {
                math::VEC3 H(pLightVec->x, pLightVec->y, pLightVec->z - 1.0f);
                math::VEC3Normalize(&H, &H);

                mIndMtx[i]._10 = 0.5f * -H.x;
                mIndMtx[i]._11 = 0.5f * -H.y;
                mIndMtx[i]._12 = 0.5f * -H.z;
            } else {
                mIndMtx[i]._10 = mIndMtx[i]._11 = mIndMtx[i]._12 = 0.0f;
            }

            math::MTX34Mult(&mIndMtx[i], &mIndMtx[i], pNrmMtx);
        } else {
            math::MTX34Zero(&mIndMtx[i]);
        }
    }
}

void IndMtxOpStd::Reset() {
    math::MTX34Identity(&mIndMtx[GX_ITM_0 - 1]);
    math::MTX34Identity(&mIndMtx[GX_ITM_1 - 1]);
    math::MTX34Identity(&mIndMtx[GX_ITM_2 - 1]);

    mIsValidMtx[GX_ITM_0 - 1] = mIsValidMtx[GX_ITM_1 - 1] = mIsValidMtx[GX_ITM_2 - 1] = false;
}

void IndMtxOpStd::operator()(IndTexMtxInfo *pInfo) {
    if (mIsValidMtx[GX_ITM_0 - 1]) {
        pInfo->SetMtx(GX_ITM_0, mIndMtx[GX_ITM_0 - 1]);
    }

    if (mIsValidMtx[GX_ITM_1 - 1]) {
        pInfo->SetMtx(GX_ITM_1, mIndMtx[GX_ITM_1 - 1]);
    }

    if (mIsValidMtx[GX_ITM_2 - 1]) {
        pInfo->SetMtx(GX_ITM_2, mIndMtx[GX_ITM_2 - 1]);
    }
}

} // namespace G3DState
} // namespace g3d
} // namespace nw4r
