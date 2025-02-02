#ifndef NW4R_G3D_RES_RES_ANM_CHR_H
#define NW4R_G3D_RES_RES_ANM_CHR_H

/** NOTICE: Revision change from 4->5. Structures, Enums, and/or Functions may have changed and not yet done so
 * (Zeldex72, Feb 1, 2025) */

#include <nw4r/types_nw4r.h>

#include <nw4r/g3d/res/g3d_resanm.h>
#include <nw4r/g3d/res/g3d_rescommon.h>
#include <nw4r/g3d/res/g3d_resmdl.h>

#include <nw4r/math.h>

namespace nw4r {
namespace g3d {

/******************************************************************************
 *
 * ChrAnmResult
 *
 ******************************************************************************/
struct ChrAnmResult {
    enum Flag {
        FLAG_ANM_EXISTS = (1 << 0),
        FLAG_MTX_IDENT = (1 << 1),
        FLAG_ROT_TRANS_ZERO = (1 << 2),
        FLAG_SCALE_ONE = (1 << 3),
        FLAG_SCALE_UNIFORM = (1 << 4),
        FLAG_ROT_ZERO = (1 << 5),
        FLAG_TRANS_ZERO = (1 << 6),
        FLAG_PATCH_SCALE = (1 << 7),
        FLAG_PATCH_ROT = (1 << 8),
        FLAG_PATCH_TRANS = (1 << 9),

        // Maya Scale Compensation
        FLAG_SSC_APPLY = (1 << 10),
        FLAG_SSC_PARENT = (1 << 11),

        // Softimage Hierarchical Scaling
        FLAG_XSI_SCALING = (1 << 12),

        FLAG_ROT_RAW_FMT = (1 << 31)
    };

    u32 flags;       // at 0x0
    math::VEC3 s;    // at 0x4
    math::VEC3 rawR; // at 0x10
    math::MTX34 rt;  // at 0x1C

    void GetScale(math::VEC3 *pScale) const;
    bool GetRotateDeg(math::VEC3 *pRotate) const;
    void GetTranslate(math::VEC3 *pTrans) const;
    void GetRotTrans(math::MTX34 *pRotTrans) const;

    void SetScale(const math::VEC3 *pScale);
    void SetRotTrans(const math::MTX34 *pRotTrans);
};

/******************************************************************************
 *
 * Frame values (FVS) animation data
 *
 ******************************************************************************/
struct ResAnmChrFrm32Data {
    union {
        u32 fvsU32; // at 0x0

        struct {
            u8 frame; // at 0x0
            u8 vs[3]; // at 0x1
        } fvs;
    };
};
struct ResAnmChrFVS32Data {
    f32 scale;                         // at 0x0
    f32 offset;                        // at 0x4
    ResAnmChrFrm32Data frameValues[1]; // at 0x8
};

struct ResAnmChrFrm48Data {
    s16 frame; // at 0x0
    u16 value; // at 0x2
    s16 slope; // at 0x4
};
struct ResAnmChrFVS48Data {
    f32 scale;                         // at 0x0
    f32 offset;                        // at 0x4
    ResAnmChrFrm48Data frameValues[1]; // at 0x8
};

struct ResAnmChrFrm96Data {
    f32 frame; // at 0x0
    f32 value; // at 0x4
    f32 slope; // at 0x8
};
struct ResAnmChrFVS96Data {
    ResAnmChrFrm96Data frameValues[1]; // at 0x0
};

struct ResAnmChrFVSData {
    u16 numFrameValues;        // at 0x0
    u8 PADDING_0x2[0x4 - 0x2]; // at 0x2
    f32 invKeyFrameRange;      // at 0x4

    union {
        ResAnmChrFVS32Data fvs32; // at 0x8
        ResAnmChrFVS48Data fvs48; // at 0x8
        ResAnmChrFVS96Data fvs96; // at 0x8
    };
};

/******************************************************************************
 *
 * Const value (CV) animation data
 *
 ******************************************************************************/
struct ResAnmChrCV8Data {
    f32 scale;    // at 0x0
    f32 offset;   // at 0x4
    u8 values[1]; // at 0x8
};
struct ResAnmChrCV16Data {
    f32 scale;     // at 0x0
    f32 offset;    // at 0x4
    u16 values[1]; // at 0x8
};
struct ResAnmChrCV32Data {
    f32 values[1]; // at 0x0
};

struct ResAnmChrCVData {
    union {
        ResAnmChrCV8Data cv8;   // at 0x0
        ResAnmChrCV16Data cv16; // at 0x0
        ResAnmChrCV32Data cv32; // at 0x0
    };
};

/******************************************************************************
 *
 * ResAnmChr
 *
 ******************************************************************************/
struct ResAnmChrAnmData {
    union {
        ResAnmChrFVSData fvs; // at 0x0
        ResAnmChrCVData cv;   // at 0x0
    };
};

struct ResAnmChrNodeData {
    enum Flag {
        FLAG_ANM_EXISTS = (1 << 0),
        FLAG_MTX_IDENT = (1 << 1),
        FLAG_ROT_TRANS_ZERO = (1 << 2),
        FLAG_SCALE_ONE = (1 << 3),
        FLAG_SCALE_UNIFORM = (1 << 4),
        FLAG_ROT_ZERO = (1 << 5),
        FLAG_TRANS_ZERO = (1 << 6),
        FLAG_PATCH_SCALE = (1 << 7),
        FLAG_PATCH_ROT = (1 << 8),
        FLAG_PATCH_TRANS = (1 << 9),

        // Maya Scale Compensation
        FLAG_SSC_APPLY = (1 << 10),
        FLAG_SSC_PARENT = (1 << 11),

        // Softimage Hierarchical Scaling
        FLAG_XSI_SCALING = (1 << 12),

        FLAG_SCALE_X_CONST = (1 << 13),
        FLAG_SCALE_Y_CONST = (1 << 14),
        FLAG_SCALE_Z_CONST = (1 << 15),

        FLAG_ROT_X_CONST = (1 << 16),
        FLAG_ROT_Y_CONST = (1 << 17),
        FLAG_ROT_Z_CONST = (1 << 18),

        FLAG_TRANS_X_CONST = (1 << 19),
        FLAG_TRANS_Y_CONST = (1 << 20),
        FLAG_TRANS_Z_CONST = (1 << 21),

        FLAG_HAS_SCALE = (1 << 22),
        FLAG_HAS_ROT = (1 << 23),
        FLAG_HAS_TRANS = (1 << 24),

        FLAG_SCALE_FVS32_FMT = (1 << 25),
        FLAG_SCALE_FVS48_FMT = (1 << 26),
        FLAG_SCALE_FVS96_FMT = FLAG_SCALE_FVS32_FMT | FLAG_SCALE_FVS48_FMT,

        FLAG_ROT_FVS32_FMT = (1 << 27),
        FLAG_ROT_FVS48_FMT = (1 << 28),
        FLAG_ROT_FVS96_FMT = FLAG_ROT_FVS32_FMT | FLAG_ROT_FVS48_FMT,

        FLAG_ROT_CV8_FMT = (1 << 29),
        FLAG_ROT_CV16_FMT = (1 << 27) | (1 << 29),
        FLAG_ROT_CV32_FMT = (1 << 28) | (1 << 29),

        FLAG_TRANS_FVS32_FMT = (1 << 30),
        FLAG_TRANS_FVS48_FMT = (1 << 31),
        FLAG_TRANS_FVS96_FMT = FLAG_TRANS_FVS32_FMT | FLAG_TRANS_FVS48_FMT,

        FLAG_HAS_SRT_MASK = FLAG_HAS_SCALE | FLAG_HAS_ROT | FLAG_HAS_TRANS,

        FLAG_SCALE_FMT_MASK = FLAG_SCALE_FVS32_FMT | FLAG_SCALE_FVS48_FMT | FLAG_SCALE_FVS96_FMT,

        FLAG_ROT_FMT_MASK = FLAG_ROT_FVS32_FMT | FLAG_ROT_FVS48_FMT | FLAG_ROT_FVS96_FMT | FLAG_ROT_CV8_FMT |
                            FLAG_ROT_CV16_FMT | FLAG_ROT_CV32_FMT,

        FLAG_TRANS_FMT_MASK = FLAG_TRANS_FVS32_FMT | FLAG_TRANS_FVS48_FMT | FLAG_TRANS_FVS96_FMT
    };

    union AnmData {
        s32 toResAnmChrAnmData; // at 0x0
        f32 constValue;         // at 0x0
    };

    s32 name;        // at 0x0
    u32 flags;       // at 0x4
    AnmData anms[1]; // at 0x8
};

struct ResAnmChrInfoData {
    u16 numFrame;            // at 0x0
    u16 numNode;             // at 0x2
    AnmPolicy policy;        // at 0x4
    ScalingRule scalingRule; // at 0x8
};

struct ResAnmChrData {
    ResBlockHeaderData header; // at 0x0
    u32 revision;              // at 0x8
    s32 toResFileData;         // at 0xC
    s32 toChrDataDic;          // at 0x10
    s32 toResUserData;         // at 0x14
    s32 name;                  // at 0x18
    s32 original_path;         // at 0x1C
    ResAnmChrInfoData info;    // at 0x20
};

class ResAnmChr : public ResCommon<ResAnmChrData> {
public:
    static const u32 SIGNATURE = 'CHR0';
    static const int REVISION = 5;

public:
    NW4R_G3D_RESOURCE_FUNC_DEF(ResAnmChr);

    u32 GetRevision() const {
        return ref().revision;
    }

    bool CheckRevision() const {
        return GetRevision() == REVISION;
    }

    void GetAnmResult(ChrAnmResult *pResult, u32 idx, f32 frame) const;

    const ResAnmChrNodeData *GetNodeAnm(int idx) const {
        return static_cast<ResAnmChrNodeData *>(ofs_to_obj<ResDic>(ref().toChrDataDic)[idx]);
    }
    const ResAnmChrNodeData *GetNodeAnm(u32 idx) const {
        return static_cast<ResAnmChrNodeData *>(ofs_to_obj<ResDic>(ref().toChrDataDic)[idx]);
    }

    s32 GetNodeAnmIndex(const ResName name) const {
        return ofs_to_obj<ResDic>(ref().toChrDataDic).GetIndex(name);
    }

    int GetNumFrame() const {
        return ref().info.numFrame;
    }

    int GetNumNode() const {
        return ref().info.numNode;
    }

    AnmPolicy GetAnmPolicy() const {
        return ref().info.policy;
    }
};

} // namespace g3d
} // namespace nw4r

#endif
