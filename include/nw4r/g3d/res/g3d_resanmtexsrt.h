#ifndef NW4R_G3D_RES_RES_ANM_TEX_SRT_H
#define NW4R_G3D_RES_RES_ANM_TEX_SRT_H

/** NOTICE: Revision change from 4->5. Structures, Enums, and/or Functions may have changed and not yet done so
 * (Zeldex72, Feb 1, 2025) */

#include <nw4r/types_nw4r.h>

#include "nw4r/g3d/res/g3d_resanm.h"
#include "nw4r/g3d/res/g3d_rescommon.h"
#include "nw4r/g3d/res/g3d_resdict.h"

namespace nw4r {
namespace g3d {

/******************************************************************************
 *
 * Common types
 *
 ******************************************************************************/
struct ResAnmTexSrtDataTypedef {
    static const int NUM_OF_MAT_TEX_MTX = 8;
    static const int NUM_OF_IND_TEX_MTX = 3;
    static const int NUM_OF_TEX_MTX = NUM_OF_MAT_TEX_MTX + NUM_OF_IND_TEX_MTX;
};

struct TexSrtTypedef {
    enum TexMatrixMode {
        TEXMATRIXMODE_MAYA,
        TEXMATRIXMODE_XSI,
        TEXMATRIXMODE_3DSMAX
    };
};

/******************************************************************************
 *
 * TexSrtAnmResult
 *
 ******************************************************************************/
struct TexSrt : TexSrtTypedef {
    enum Flag {
        FLAG_ANM_EXISTS = (1 << 0),
        FLAG_SCALE_ONE = (1 << 1),
        FLAG_ROT_ZERO = (1 << 2),
        FLAG_TRANS_ZERO = (1 << 3),

        FLAGSET_IDENTITY = (1 << 0) | (1 << 1) | (1 << 2) | (1 << 3),
        NUM_OF_FLAGS = 4
    };

    f32 Su; // at 0x0
    f32 Sv; // at 0x4
    f32 R;  // at 0x8
    f32 Tu; // at 0xc
    f32 Tv; // at 0x10
};

struct TexSrtAnmResult : ResAnmTexSrtDataTypedef, TexSrtTypedef {
    enum Flag {
        FLAG_ANM_EXISTS = (1 << 0),
        FLAG_SCALE_ONE = (1 << 1),
        FLAG_ROT_ZERO = (1 << 2),
        FLAG_TRANS_ZERO = (1 << 3),

        // Four bits in 'flags' for each animation
        NUM_OF_FLAGS = 4
    };

    u32 flags;                  // at 0x0
    u32 indFlags;               // at 0x4
    TexMatrixMode texMtxMode;   // at 0x8
    TexSrt srt[NUM_OF_TEX_MTX]; // at 0xC
};

/******************************************************************************
 *
 * ResAnmTexSrt
 *
 ******************************************************************************/
struct ResAnmTexSrtTexData {
    enum Flag {
        FLAG_ANM_EXISTS = (1 << 0),
        FLAG_SCALE_ONE = (1 << 1),
        FLAG_ROT_ZERO = (1 << 2),
        FLAG_TRANS_ZERO = (1 << 3),

        FLAG_SCALE_UNIFORM = (1 << 4),
        FLAG_SCALE_U_CONST = (1 << 5),
        FLAG_SCALE_V_CONST = (1 << 6),

        FLAG_ROT_CONST = (1 << 7),
        FLAG_TRANS_U_CONST = (1 << 8),
        FLAG_TRANS_V_CONST = (1 << 9),
    };

    u32 flags;          // at 0x0
    ResAnmData anms[1]; // at 0x4
};

struct ResAnmTexSrtMatData : ResAnmTexSrtDataTypedef {
    enum Flag {
        FLAG_ANM_EXISTS = (1 << 0),

        NUM_OF_FLAGS = 1
    };

    s32 name;                     // at 0x0
    u32 flags;                    // at 0x4
    u32 indFlags;                 // at 0x8
    s32 toResAnmTexSrtTexData[1]; // at 0xC
};

struct ResAnmTexSrtInfoData : TexSrtTypedef {
    u16 numFrame;             // at 0x0
    u16 numMaterial;          // at 0x2
    TexMatrixMode texMtxMode; // at 0x4
    AnmPolicy policy;         // at 0x8
};

struct ResAnmTexSrtData {
    ResBlockHeaderData header; // at 0x0
    u32 revision;              // at 0x8
    s32 toResFileData;         // at 0xC
    s32 toTexSrtDataDic;       // at 0x10
    s32 toResUserData;         // at 0x14
    s32 name;                  // at 0x18
    s32 original_path;         // at 0x1C
    ResAnmTexSrtInfoData info; // at 0x20
};

class ResAnmTexSrt : public ResCommon<ResAnmTexSrtData> {
public:
    static const u32 SIGNATURE = 'SRT0';
    static const int REVISION = 5;

public:
    NW4R_G3D_RESOURCE_FUNC_DEF(ResAnmTexSrt);

    u32 GetRevision() const {
        return ref().revision;
    }

    bool CheckRevision() const {
        return GetRevision() == REVISION;
    }

    void GetAnmResult(TexSrtAnmResult *pResult, u32 idx, f32 frame) const;

    const ResAnmTexSrtMatData *GetMatAnm(int idx) const {
        return static_cast<ResAnmTexSrtMatData *>(ofs_to_obj<ResDic>(ref().toTexSrtDataDic)[idx]);
    }
    const ResAnmTexSrtMatData *GetMatAnm(u32 idx) const {
        return static_cast<ResAnmTexSrtMatData *>(ofs_to_obj<ResDic>(ref().toTexSrtDataDic)[idx]);
    }

    int GetNumFrame() const {
        return ref().info.numFrame;
    }

    int GetNumMaterial() const {
        return ref().info.numMaterial;
    }

    AnmPolicy GetAnmPolicy() const {
        return ref().info.policy;
    }
};

} // namespace g3d
} // namespace nw4r

#endif
