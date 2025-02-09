#ifndef NW4R_G3D_RES_RES_MAT_H
#define NW4R_G3D_RES_RES_MAT_H
#include <nw4r/types_nw4r.h>

#include <nw4r/g3d/res/g3d_resanmtexsrt.h>
#include <nw4r/g3d/res/g3d_rescommon.h>
#include <nw4r/g3d/res/g3d_respltt.h>
#include <nw4r/g3d/res/g3d_restev.h>
#include <nw4r/g3d/res/g3d_restex.h>

#include <nw4r/math.h>

#include <rvl/GX.h>

namespace nw4r {
namespace g3d {

/******************************************************************************
 *
 * ResGenMode
 *
 ******************************************************************************/
struct ResGenModeData {
    u8 nTexGens;         // at 0x0
    u8 nChans;           // at 0x1
    u8 nTevs;            // at 0x2
    u8 nInds;            // at 0x3
    GXCullMode cullMode; // at 0x4
};

class ResGenMode : public ResCommon<ResGenModeData> {
public:
    NW4R_G3D_RESOURCE_FUNC_DEF(ResGenMode);

    ResGenMode CopyTo(void *pDst) const;

    void GXSetNumTexGens(u8 num);
    void GXSetNumChans(u8 num);
    void GXSetNumTevStages(u8 num);
    void GXSetNumIndStages(u8 num);
    void GXSetCullMode(GXCullMode mode);

    u8 GXGetNumTexGens() const {
        return IsValid() ? ptr()->nTexGens : 0;
    }

    u8 GXGetNumChans() const {
        return IsValid() ? ptr()->nChans : 0;
    }

    u8 GXGetNumTevStages() const {
        return IsValid() ? ptr()->nTevs : 0;
    }

    u8 GXGetNumIndStages() const {
        return IsValid() ? ptr()->nInds : 0;
    }

    GXCullMode GXGetCullMode() const {
        return IsValid() ? ptr()->cullMode : GX_CULL_ALL;
    }

    void EndEdit() {}
};

/******************************************************************************
 *
 * ResMatMisc
 *
 ******************************************************************************/
struct ResMatMiscData {
    enum IndirectMethod {
        WARP,
        NORMAL_MAP,
        NORMAL_MAP_SPECULAR,
        FUR,

        _RESERVED0,
        _RESERVED1,

        USER0,
        USER1,

        NUM_OF_INDIRECT_METHOD,
    };

    GXBool zCompLoc;                       // at 0x0
    s8 light_set_idx;                      // at 0x1
    s8 fog_idx;                            // at 0x2
    u8 PADDING_0x3;                        // at 0x3
    u8 indirect_method[GX_ITM_2 + 1];      // at 0x4
    s8 normal_map_ref_light[GX_ITM_2 + 1]; // at 0x8
};

class ResMatMisc : public ResCommon<ResMatMiscData> {
public:
    NW4R_G3D_RESOURCE_FUNC_DEF(ResMatMisc);

    ResMatMisc CopyTo(void *pDst) const;

    GXBool GXGetZCompLoc() const;
    int GetLightSetIdx() const;
    int GetFogIdx() const;

    void GetIndirectTexMtxCalcMethod(GXIndTexMtxID id, ResMatMiscData::IndirectMethod *pMethod, s8 *pLightRef);

    void EndEdit() {}
};

/******************************************************************************
 *
 * ResMatTexCoordGen
 *
 ******************************************************************************/
struct ResTexCoordGenDL {
    union {
        struct {
            u8 texCoordGen[GX_MAX_TEXCOORD][GX_XF_CMD_SZ * 2]; // at 0x0
            u8 PADDING_0x90[0xA0 - 0x90];                      // at 0x90
        } dl;

        u8 data[0xA0];
    };
};

class ResMatTexCoordGen : public ResCommon<ResTexCoordGenDL> {
public:
    NW4R_G3D_RESOURCE_FUNC_DEF_EX(ResMatTexCoordGen, ResTexCoordGenDL);

    void DCStore(bool sync);
    ResMatTexCoordGen CopyTo(void *pDst) const;

    void CallDisplayList(u8 numGens, bool sync) const;

    bool GXGetTexCoordGen2(GXTexCoordID id, GXTexGenType *pFunc, GXTexGenSrc *pParam, GXBool *pNormalize, u32 *pPostMtx)
        const;
    void GXSetTexCoordGen2(GXTexCoordID id, GXTexGenType func, GXTexGenSrc param, GXBool normalize, u32 postMtx);

    void EndEdit() {
        DCStore(false);
    }
};

/******************************************************************************
 *
 * ResTexObj
 *
 ******************************************************************************/
struct ResTexObjData {
    u32 flagUsedTexMapID;           // at 0x0
    GXTexObj texObj[GX_MAX_TEXMAP]; // at 0x4
};

class ResTexObj : public ResCommon<ResTexObjData> {
public:
    NW4R_G3D_RESOURCE_FUNC_DEF(ResTexObj);

    ResTexObj CopyTo(void *pDst) const;

    const GXTexObj *GetTexObj(GXTexMapID id) const;
    GXTexObj *GetTexObj(GXTexMapID id);

    bool IsValidTexObj(GXTexMapID id) const;

    void Validate(GXTexMapID id);
    void Invalidate(GXTexMapID id);

    void EndEdit() {}
};

/******************************************************************************
 *
 * ResTlutObj
 *
 ******************************************************************************/
struct ResTlutObjData {
    u32 flagUsedTlutID;                     // at 0x0
    GXTlutObj tlutObj[GX_TLUT8 - GX_TLUT0]; // at 0x4
};

class ResTlutObj : public ResCommon<ResTlutObjData> {
public:
    NW4R_G3D_RESOURCE_FUNC_DEF(ResTlutObj);

    ResTlutObj CopyTo(void *pDst) const;

    const GXTlutObj *GetTlut(GXTlut tlut) const;
    GXTlutObj *GetTlut(GXTlut tlut);

    bool IsValidTlut(GXTlut tlut) const;

    void Validate(GXTlut tlut);
    void Invalidate(GXTlut tlut);

    void EndEdit() {}
};

/******************************************************************************
 *
 * ResTexSrt
 *
 ******************************************************************************/
struct TexMtxEffect : TexSrtTypedef {
    enum Flag {
        FLAG_IDENT = (1 << 0),
    };

    s8 ref_camera;          // at 0x0
    s8 ref_light;           // at 0x1
    u8 map_mode;            // at 0x2
    u8 misc_flag;           // at 0x3
    math::_MTX34 effectMtx; // at 0x4
};

struct ResTexSrtData : TexSrtTypedef {
    static const int NUM_OF_TEXTURE = 8;

    u32 flag;                            // at 0x0
    TexMatrixMode texMtxMode;            // at 0x4
    TexSrt texSrt[NUM_OF_TEXTURE];       // at 0x8
    TexMtxEffect effect[NUM_OF_TEXTURE]; // at 0xA8
};

class ResTexSrt : public ResCommon<ResTexSrtData> {
public:
    NW4R_G3D_RESOURCE_FUNC_DEF(ResTexSrt);

    ResTexSrt CopyTo(void *pDst) const;

    bool GetEffectMtx(u32 id, math::MTX34 *pMtx) const;
    bool SetEffectMtx(u32 id, const math::MTX34 *pMtx);

    bool GetMapMode(u32 id, u32 *pMode, int *pCamRef, int *pLightRef) const;
    bool SetMapMode(u32 id, u32 mode, int camRef, int lightRef);

    TexSrt::Flag GetTexSrtFlag(u32 id) const {
        return static_cast<TexSrt::Flag>(
            (ref().flag >> id * TexSrt::NUM_OF_FLAGS) &
            (TexSrt::FLAG_ANM_EXISTS | TexSrt::FLAG_SCALE_ONE | TexSrt::FLAG_ROT_ZERO | TexSrt::FLAG_TRANS_ZERO)
        );
    }

    bool IsExist(u32 id) const {
        if (IsValid()) {
            return ptr()->flag & (1 << id * TexSrt::NUM_OF_FLAGS);
        } else {
            return false;
        }
    }

    bool IsIdentity(u32 id) const {
        return (((ref().flag >> id * TexSrt::NUM_OF_FLAGS) & TexSrt::FLAGSET_IDENTITY) == TexSrt::FLAGSET_IDENTITY) &&
               (ref().effect[id].misc_flag & TexMtxEffect::FLAG_IDENT);
    }

    TexSrtTypedef::TexMatrixMode GetTexMtxMode() const {
        return ref().texMtxMode;
    }

    void EndEdit() {}
};

/******************************************************************************
 *
 * ResMatChan
 *
 ******************************************************************************/
struct Chan {
    enum Flag {
        FLAG_MAT_COLOR_ENABLE = (1 << 0),
        FLAG_MAT_ALPHA_ENABLE = (1 << 1),

        FLAG_AMB_COLOR_ENABLE = (1 << 2),
        FLAG_AMB_ALPHA_ENABLE = (1 << 3),

        FLAG_CTRL_COLOR_ENABLE = (1 << 4),
        FLAG_CTRL_ALPHA_ENABLE = (1 << 5),
    };

    u32 flag;           // at 0x0
    GXColor matColor;   // at 0x4
    GXColor ambColor;   // at 0x8
    u32 paramChanCtrlC; // at 0xC
    u32 paramChanCtrlA; // at 0x10
};

struct ResChanData {
    Chan chan[2]; // at 0x0
};

class ResMatChan : public ResCommon<ResChanData> {
public:
    NW4R_G3D_RESOURCE_FUNC_DEF_EX(ResMatChan, ResChanData);

    ResMatChan CopyTo(void *pDst) const;

    bool GXGetChanMatColor(GXChannelID id, GXColor *pColor) const;
    void GXSetChanMatColor(GXChannelID id, GXColor color);

    bool GXGetChanAmbColor(GXChannelID id, GXColor *pColor) const;
    void GXSetChanAmbColor(GXChannelID id, GXColor color);

    bool GXGetChanCtrl(
        GXChannelID id, GXBool *pEnable, GXColorSrc *pAmbSrc, GXColorSrc *pMatSrc, GXLightID *pLightMask,
        GXDiffuseFn *pDiffuseFn, GXAttnFn *pAttnFn
    ) const;
    void GXSetChanCtrl(
        GXChannelID id, GXBool enable, GXColorSrc ambSrc, GXColorSrc matSrc, GXLightID lightMask, GXDiffuseFn diffuseFn,
        GXAttnFn attnFn
    );

    void EndEdit() {}
};

/******************************************************************************
 *
 * ResMatPix
 *
 ******************************************************************************/
struct ResPixDL {
    union {
        struct {
            u8 alphaCompare[GX_BP_CMD_SZ];  // at 0x0
            u8 zMode[GX_BP_CMD_SZ];         // at 0x5
            u8 blendMode[GX_BP_CMD_SZ * 2]; // at 0xA
            u8 setDstAlpha[GX_BP_CMD_SZ];   // at 0x14
            u8 PADDING_0x19[32 - 0x19];     // at 0x19
        } dl;

        u8 data[32]; // at 0x0
    };
};

class ResMatPix : public ResCommon<ResPixDL> {
public:
    NW4R_G3D_RESOURCE_FUNC_DEF_EX(ResMatPix, ResPixDL);

    void DCStore(bool sync);
    ResMatPix CopyTo(void *pDst) const;

    void CallDisplayList(bool sync) const;

    bool GXGetAlphaCompare(GXCompare *pComp0, u8 *pRef0, GXAlphaOp *pLogic, GXCompare *pComp1, u8 *pRef1) const;
    void GXSetAlphaCompare(GXCompare comp0, u8 ref0, GXAlphaOp logic, GXCompare comp1, u8 ref1);

    bool GXGetZMode(GXBool *pTest, GXCompare *pCompare, GXBool *pUpdate) const;
    void GXSetZMode(GXBool test, GXCompare compare, GXBool update);

    bool
    GXGetBlendMode(GXBlendMode *pMode, GXBlendFactor *pSrcFactor, GXBlendFactor *pDstFactor, GXLogicOp *pLogic) const;
    void GXSetBlendMode(GXBlendMode mode, GXBlendFactor srcFactor, GXBlendFactor dstFactor, GXLogicOp logic);

    bool GXGetDstAlpha(GXBool *pEnable, u8 *pAlpha) const;
    void GXSetDstAlpha(GXBool enable, u8 alpha);

    void EndEdit() {
        DCStore(false);
    }
};

/******************************************************************************
 *
 * ResMatTevColor
 *
 ******************************************************************************/
struct ResTevColorDL {
    union {
        struct {
            u8 tevColor[GX_MAX_TEVREG - GX_TEVREG0][GX_BP_CMD_SZ * 4]; // at 0x0
            u8 PADDING_0x3C[64 - 0x3C];                                // at 0x3C
            u8 tevKColor[GX_MAX_KCOLOR][GX_BP_CMD_SZ * 2];             // at 0x40
            u8 PADDING_0x68[128 - 0x68];                               // at 0x68
        } dl;

        u8 data[128];
    };
};

class ResMatTevColor : public ResCommon<ResTevColorDL> {
public:
    NW4R_G3D_RESOURCE_FUNC_DEF_EX(ResMatTevColor, ResTevColorDL);

    void DCStore(bool sync);
    ResMatTevColor CopyTo(void *pDst) const;

    void CallDisplayList(bool sync) const;

    bool GXGetTevColor(GXTevRegID id, GXColor *pColor) const;
    void GXSetTevColor(GXTevRegID id, GXColor color);

    bool GXGetTevColorS10(GXTevRegID id, GXColorS10 *pColor) const;
    void GXSetTevColorS10(GXTevRegID id, GXColorS10 color);

    bool GXGetTevKColor(GXTevKColorID id, GXColor *pColor) const;
    void GXSetTevKColor(GXTevKColorID id, GXColor color);

    void EndEdit() {
        DCStore(false);
    }
};

/******************************************************************************
 *
 * ResMatIndMtxAndScale
 *
 ******************************************************************************/
struct ResIndMtxAndScaleDL {
    union {
        struct {
            u8 indTexCoordScale[2][GX_BP_CMD_SZ]; // at 0x0
            u8 indTexMtx0[GX_BP_CMD_SZ * 3];      // at 0xA
            u8 PADDING_0x19[32 - 0x19];           // at 0x19
            u8 indTexMtx1[GX_BP_CMD_SZ * 3];      // at 0x20
            u8 indTexMtx2[GX_BP_CMD_SZ * 3];      // at 0x2F
            u8 PADDING_0x3E[64 - 0x3E];           // at 0x3E
        } dl;

        u8 data[64]; // at 0x0
    };
};

class ResMatIndMtxAndScale : public ResCommon<ResIndMtxAndScaleDL> {
public:
    NW4R_G3D_RESOURCE_FUNC_DEF_EX(ResMatIndMtxAndScale, ResIndMtxAndScaleDL);

    void DCStore(bool sync);
    ResMatIndMtxAndScale CopyTo(void *pDst) const;

    void CallDisplayList(u8 indNum, bool sync) const;

    bool GXGetIndTexMtx(GXIndTexMtxID id, math::MTX34 *pMtx) const;
    void GXSetIndTexMtx(GXIndTexMtxID id, const math::MTX34 &rMtx, s8 scaleExp);

    void EndEdit() {
        DCStore(false);
    }
};

/******************************************************************************
 *
 * ResTexPlttInfo
 *
 ******************************************************************************/
struct ResTexPlttInfoData {
    s32 nameTex;            // at 0x0
    s32 namePltt;           // at 0x4
    ResTexData *pTexData;   // at 0x8
    ResPlttData *pPlttData; // at 0xC
    GXTexMapID mapID;       // at 0x10
    GXTlut tlutID;          // at 0x14
    GXTexWrapMode wrap_s;   // at 0x18
    GXTexWrapMode wrap_t;   // at 0x1C
    GXTexFilter min_filt;   // at 0x20
    GXTexFilter mag_filt;   // at 0x24
    f32 lod_bias;           // at 0x28
    GXAnisotropy max_aniso; // at 0x2C
    bool bias_clamp;        // at 0x30
    bool do_edge_lod;       // at 0x31
    u8 PADDING_0x32;        // at 0x32
    u8 PADDING_0x33;        // at 0x33
};

class ResTexPlttInfo : public ResCommon<ResTexPlttInfoData> {
public:
    NW4R_G3D_RESOURCE_FUNC_DEF(ResTexPlttInfo);

    bool Bind(const ResFile file, ResTexObj texObj, ResTlutObj tlutObj);
    void Release(ResTexObj texObj, ResTlutObj tlutObj);

    ResName GetTexResName() const {
        const ResTexPlttInfoData &r = ref();

        if (r.nameTex != 0) {
            return NW4R_G3D_OFS_TO_RESNAME(&r, r.nameTex);
        }

        return ResName(NULL);
    }

    ResName GetPlttResName() const {
        const ResTexPlttInfoData &r = ref();

        if (r.namePltt != 0) {
            return NW4R_G3D_OFS_TO_RESNAME(&r, r.namePltt);
        }

        return ResName(NULL);
    }

    bool IsCIFmt() const {
        return ref().namePltt != 0;
    }

private:
    void BindTex_(const ResTex tex, ResTexObj texObj);
    void BindPltt_(const ResPltt pltt, ResTlutObj tlutObj);
};

/******************************************************************************
 *
 * ResMat
 *
 ******************************************************************************/
struct ResMatDLData {
    ResPixDL dlPix;                       // at 0x0
    ResTevColorDL dlTevColor;             // at 0x20
    ResIndMtxAndScaleDL dlIndMtxAndScale; // at 0xA0
    ResTexCoordGenDL dlTexCoordGen;       // at 0xE0
};

struct ResMatData {
    u32 size;                   // at 0x0
    s32 toResMdlData;           // at 0x4
    s32 name;                   // at 0x8
    u32 id;                     // at 0xC
    u32 flag;                   // at 0x10
    ResGenModeData genMode;     // at 0x14
    ResMatMiscData misc;        // at 0x1C
    s32 toResTevData;           // at 0x28
    u32 numResTexPlttInfo;      // at 0x2C
    s32 toResTexPlttInfo;       // at 0x30
    s32 toResMatFurData;        // at 0x34
    s32 toResUserData;          // at 0x38
    s32 toResMatDLData;         // at 0x40
    ResTexObjData texObjData;   // at 0x44
    ResTlutObjData tlutObjData; // at 0x144
    ResTexSrtData texSrtData;   // at 0x1A8
    ResChanData chan;           // at 0x3F0
};

class ResMat : public ResCommon<ResMatData> {
public:
    NW4R_G3D_RESOURCE_FUNC_DEF(ResMat);

    void Init();

    bool Bind(const ResFile file);
    void Release();

    bool IsOpaque() const;
    ResMdl GetParent();

    u32 GetID() const {
        return ref().id;
    }

    ResGenMode GetResGenMode() {
        return ResGenMode(&ref().genMode);
    }

    ResMatMisc GetResMatMisc() {
        return ResMatMisc(&ref().misc);
    }

    ResTev GetResTev();
    ResTev GetResTev() const;

    u32 GetNumResTexPlttInfo() const {
        return ref().numResTexPlttInfo;
    }

    ResTexPlttInfo GetResTexPlttInfo(u32 id) {
        ResTexPlttInfoData *pData = ofs_to_ptr<ResTexPlttInfoData>(ref().toResTexPlttInfo);

        return ResTexPlttInfo(&pData[id]);
    }

    ResMatDLData *GetResMatDLData() {
        return ofs_to_ptr<ResMatDLData>(ref().toResMatDLData);
    }

    ResMatPix GetResMatPix() {
        return ResMatPix(&GetResMatDLData()->dlPix);
    }

    ResMatTevColor GetResMatTevColor() {
        return ResMatTevColor(&GetResMatDLData()->dlTevColor);
    }

    ResMatIndMtxAndScale GetResMatIndMtxAndScale() {
        return ResMatIndMtxAndScale(&GetResMatDLData()->dlIndMtxAndScale);
    }

    ResMatTexCoordGen GetResMatTexCoordGen() {
        return ResMatTexCoordGen(&GetResMatDLData()->dlTexCoordGen);
    }

    ResTlutObj GetResTlutObj() {
        return ResTlutObj(&ref().tlutObjData);
    }

    ResTexObj GetResTexObj() {
        return ResTexObj(&ref().texObjData);
    }

    ResTexSrt GetResTexSrt() {
        return ResTexSrt(&ref().texSrtData);
    }

    ResMatChan GetResMatChan() {
        return ResMatChan(&ref().chan);
    }
};

} // namespace g3d
} // namespace nw4r

#endif
