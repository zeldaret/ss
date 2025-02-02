#ifndef NW4R_G3D_STATE_H
#define NW4R_G3D_STATE_H
#include <nw4r/types_nw4r.h>

#include <nw4r/g3d/g3d_camera.h>
#include <nw4r/g3d/g3d_fog.h>
#include <nw4r/g3d/res/g3d_resmat.h>

#include <nw4r/math.h>

#include <rvl/GX.h>

namespace nw4r {
namespace g3d {
namespace detail {

/******************************************************************************
 *
 * ScnDependentMtxFunc
 *
 ******************************************************************************/
namespace ScnDependentMtxFunc {

void EnvironmentMapping(math::MTX34 *pMtx, s8 camRef, s8 lightRef);
void ProjectionMapping(math::MTX34 *pMtx, s8 camRef, s8 lightRef);
void EnvironmentSpecularMapping(math::MTX34 *pMtx, s8 camRef, s8 lightRef);
void DefaultMapping(math::MTX34 *pMtx, s8 camRef, s8 lightRef);

} // namespace ScnDependentMtxFunc
} // namespace detail

/******************************************************************************
 *
 * G3DState
 *
 ******************************************************************************/
namespace G3DState {

static const int NUM_LIGHT = 128;
static const int NUM_LIGHT_SET = 128;

static const int NUM_LIGHT_IN_LIGHT_SET = 8;

static const int NUM_CAMERA = 32;
static const int NUM_FOG = 32;

static const int NUM_SCNDEPENDENT_TEXMTX_FUNCTYPE = 256;

enum InvalidateFlag {
    INVALIDATE_TEX = (1 << 0),
    INVALIDATE_TLUT = (1 << 1),
    INVALIDATE_TEV = (1 << 2),
    INVALIDATE_GENMODE = (1 << 3),
    INVALIDATE_SHP = (1 << 4),
    INVALIDATE_CURRMTX = (1 << 5),
    INVALIDATE_TEXMTX = (1 << 6),
    INVALIDATE_MISC = (1 << 7),
    INVALIDATE_FOG = (1 << 8),
    INVALIDATE_LIGHT = (1 << 9),
    INVALIDATE_POSMTX = (1 << 10),

    INVALIDATE_ALL = INVALIDATE_TEX | INVALIDATE_TLUT | INVALIDATE_TEV | INVALIDATE_GENMODE | INVALIDATE_SHP |
                     INVALIDATE_CURRMTX | INVALIDATE_TEXMTX | INVALIDATE_MISC | INVALIDATE_FOG | INVALIDATE_LIGHT |
                     INVALIDATE_POSMTX
};

enum ScnDependentTexMtxFuncType {
    SCNDEPENDENT_TEXMTX_FUNCTYPE_SRC_TEXCOORD,
    SCNDEPENDENT_TEXMTX_FUNCTYPE_SRC_NRM,
    SCNDEPENDENT_TEXMTX_FUNCTYPE_SRC_POS,
    SCNDEPENDENT_TEXMTX_FUNCTYPE_SRC_COLOR,

    MAX_SCNDEPENDENT_TEXMTX_FUNCTYPE,

    SCNDEPENDENT_TEXMTX_FUNCTYPE_TEXMTX_NOT_EXIST = SCNDEPENDENT_TEXMTX_FUNCTYPE_SRC_TEXCOORD,
};

typedef void (*ScnDependentTexMtxFuncPtr)(math::MTX34 *pMtx, s8 camRef, s8 lightRef);

/******************************************************************************
 *
 * IndTexMtxInfo
 *
 ******************************************************************************/
struct IndTexMtxInfo {
    u32 flag;                                        // at 0x0
    math::MTX34 offset_mtx[GX_ITM_2 - GX_ITM_0 + 1]; // at 0x4

    IndTexMtxInfo() : flag(0) {}
    explicit IndTexMtxInfo(const ResMatIndMtxAndScale ind);

    void FifoSend() const;
    void SetMtx(GXIndTexMtxID id, const math::MTX34 &rMtx);
};

/******************************************************************************
 *
 * IndMtxOp
 *
 ******************************************************************************/
class IndMtxOp {
public:
    virtual void operator()(IndTexMtxInfo *pInfo) = 0; // at 0x8

    virtual ~IndMtxOp() {}    // at 0xC
    virtual void Reset() = 0; // at 0x10

    virtual void SetNrmMapMtx(
        GXIndTexMtxID id, const math::VEC3 *pLightVec, const math::MTX34 *pNrmMtx,
        ResMatMiscData::IndirectMethod method
    ) = 0; // at 0x14
};

/******************************************************************************
 *
 * IndMtxOpStd
 *
 ******************************************************************************/
class IndMtxOpStd : public IndMtxOp {
public:
    IndMtxOpStd();

    virtual void operator()(IndTexMtxInfo *pInfo); // at 0x8

    virtual ~IndMtxOpStd() {} // at 0xC
    virtual void Reset();     // at 0x10

    virtual void SetNrmMapMtx(
        GXIndTexMtxID id, const math::VEC3 *pLightVec, const math::MTX34 *pNrmMtx,
        ResMatMiscData::IndirectMethod method
    ); // at 0x14

private:
    bool mIsValidMtx[GX_ITM_2 - GX_ITM_0 + 1];    // at 0x4
    u8 PADDING_0x7;                               // at 0x7
    math::MTX34 mIndMtx[GX_ITM_2 - GX_ITM_0 + 1]; // at 0x8
};

/******************************************************************************
 *
 * Functions
 *
 ******************************************************************************/
void LoadResMatMisc(const ResMatMisc misc);
void LoadResTexObj(const ResTexObj texObj);
void LoadResTlutObj(const ResTlutObj tlutObj);
void LoadResGenMode(const ResGenMode mode);
void LoadResTev(const ResTev tev);
void LoadResMatPix(const ResMatPix pix);
void LoadResMatTevColor(const ResMatTevColor color);
void LoadResMatIndMtxAndScale(const ResMatIndMtxAndScale ind);
void LoadResMatIndMtxAndScale(const ResMatIndMtxAndScale ind, IndMtxOp &rOp);
void LoadResMatChan(
    const ResMatChan chan, u32 maskDiffColor, u32 maskDiffAlpha, u32 maskSpecColor, u32 maskSpecAlpha, GXColor amb,
    bool lightOff
);
void LoadResMatTexCoordGen(const ResMatTexCoordGen gen);
void LoadResTexSrt(const ResTexSrt srt);
void LoadResShpPrePrimitive(const ResShp shp);
void LoadResShpPrimitive(const ResShp shp, const math::MTX34 *pViewPos, const math::MTX34 *pViewNrm);

void SetViewPosNrmMtxArray(
    const math::MTX34 *pViewPosMtxArray, const math::MTX33 *pViewNrmMtxArray, const math::MTX34 *pViewEnvTexMtxArray
);
const math::MTX33 *GetViewNrmMtxPtr(u32 id);

void SetScnDependentTexMtxFunc(u32 id, ScnDependentTexMtxFuncPtr func, ScnDependentTexMtxFuncType type);
bool GetScnDependentTexMtxFunc(u32 id, ScnDependentTexMtxFuncPtr *pFunc, ScnDependentTexMtxFuncType *pType);

IndMtxOp *GetIndMtxOp();

void SetFog(const Fog fog, int id);
void LoadFog(int id);

void SetLightSetting(const LightSetting &rSetting);
const LightObj *GetLightObj(int id);
void LoadLightSet(
    int id, u32 *pDiffColorMask, u32 *pDiffAlphaMask, u32 *pSpecColorMask, u32 *pSpecAlphaMask, AmbLightObj *pAmb
);
void LoadLightSet(int id, u32 *pDiffMask, u32 *pSpecMask, AmbLightObj *pAmb);

void SetCameraProjMtx(const Camera &rCam, int id, bool view);
const math::MTX34 *GetCameraMtxPtr();
const math::MTX34 *GetInvCameraMtxPtr();
const math::MTX34 *GetCameraMtxPtr(int id);
const math::MTX34 *GetProjectionTexMtxPtr();
const math::MTX34 *GetProjectionTexMtxPtr(int id);
const math::MTX34 *GetEnvironmentTexMtxPtr();

void SetRenderModeObj(const GXRenderModeObj &rObj);
const GXRenderModeObj *GetRenderModeObj();

void Invalidate(u32 flag = INVALIDATE_ALL);

} // namespace G3DState
} // namespace g3d
} // namespace nw4r

#endif
