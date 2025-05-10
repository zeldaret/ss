#ifndef NW4R_G3D_RES_RES_ANM_CLR_H
#define NW4R_G3D_RES_RES_ANM_CLR_H

/** NOTICE: Revision change from 3->4. Structures, Enums, and/or Functions may have changed and not yet done so
 * (Zeldex72, Feb 1, 2025) */

#include <nw4r/types_nw4r.h>

#include "nw4r/g3d/res/g3d_resanm.h"
#include "nw4r/g3d/res/g3d_rescommon.h"
#include "nw4r/g3d/res/g3d_resdict.h"

namespace nw4r {
namespace g3d {

/******************************************************************************
 *
 * ClrAnmResult
 *
 ******************************************************************************/
struct ClrAnmResult {
    enum ColorAnim {
        CLA_CLR0,
        CLA_CLR1,
        CLA_AMB0,
        CLA_AMB1,

        CLA_TEV0,
        CLA_TEV1,
        CLA_TEV2,

        CLA_TEVK0,
        CLA_TEVK1,
        CLA_TEVK2,
        CLA_TEVK3,

        CLA_MAX
    };

    u32 bRgbaExist;        // at 0x0
    u32 rgba[CLA_MAX];     // at 0x4
    u32 rgbaMask[CLA_MAX]; // at 0x30
};

/******************************************************************************
 *
 * ResAnmClr
 *
 ******************************************************************************/
struct ResAnmClrAnmData {
    u32 mask;              // at 0x0
    ResColorAnmData color; // at 0x4
};

struct ResAnmClrMatData {
    enum Flag {
        FLAG_ANM_EXISTS = (1 << 0),
        FLAG_ANM_CONSTANT = (1 << 1),

        // Two bits in 'flags' for each animation
        NUM_OF_FLAGS = 2
    };

    s32 name;                 // at 0x0
    u32 flags;                // at 0x4
    ResAnmClrAnmData anms[1]; // at 0x8
};

struct ResAnmClrInfoData {
    u16 numFrame;     // at 0x0
    u16 numMaterial;  // at 0x2
    AnmPolicy policy; // at 0x4
};

struct ResAnmClrData {
    ResBlockHeaderData header; // at 0x0
    u32 revision;              // at 0x8
    s32 toResFileData;         // at 0xC
    s32 toClrDataDic;          // at 0x10
    s32 toResUserData;         // at 0x14
    s32 name;                  // at 0x18
    s32 original_path;         // at 0x1C
    ResAnmClrInfoData info;    // at 0x20
};

class ResAnmClr : public ResCommon<ResAnmClrData> {
public:
    static const u32 SIGNATURE = 'CLR0';
    static const int REVISION = 4;

public:
    NW4R_G3D_RESOURCE_FUNC_DEF(ResAnmClr);

    u32 GetRevision() const {
        return ref().revision;
    }

    bool CheckRevision() const {
        return GetRevision() == REVISION;
    }

    void GetAnmResult(ClrAnmResult *pResult, u32 idx, f32 frame) const;

    const ResAnmClrMatData *GetMatAnm(int idx) const {
        return static_cast<ResAnmClrMatData *>(ofs_to_obj<ResDic>(ref().toClrDataDic)[idx]);
    }
    const ResAnmClrMatData *GetMatAnm(u32 idx) const {
        return static_cast<ResAnmClrMatData *>(ofs_to_obj<ResDic>(ref().toClrDataDic)[idx]);
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
