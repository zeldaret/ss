#ifndef NW4R_G3D_RES_RES_ANM_VIS_H
#define NW4R_G3D_RES_RES_ANM_VIS_H

/** NOTICE: Revision change from 3->4. Structures, Enums, and/or Functions may have changed and not yet done so
 * (Zeldex72, Feb 1, 2025) */

#include <nw4r/types_nw4r.h>

#include "nw4r/g3d/res/g3d_resanm.h"
#include "nw4r/g3d/res/g3d_rescommon.h"
#include "nw4r/g3d/res/g3d_resdict.h"

namespace nw4r {
namespace g3d {

struct ResAnmVisAnmData {
    enum Flag {
        FLAG_ENABLE = (1 << 0),
        FLAG_CONST = (1 << 1)
    };

    s32 name;                        // at 0x0
    u32 flags;                       // at 0x4
    ResBoolAnmFramesData visibility; // at 0x8
};

struct ResAnmVisInfoData {
    u16 numFrame;     // at 0x0
    u16 numNode;      // at 0x2
    AnmPolicy policy; // at 0x4
};

struct ResAnmVisData {
    ResBlockHeaderData header; // at 0x0
    u32 revision;              // at 0x8
    s32 toResFileData;         // at 0xC
    s32 toVisDataDic;          // at 0x10
    s32 toResUserData;         // at 0x14
    s32 name;                  // at 0x18
    s32 original_path;         // at 0x1C
    ResAnmVisInfoData info;    // at 0x20
};

class ResAnmVis : public ResCommon<ResAnmVisData> {
public:
    static const u32 SIGNATURE = 'VIS0';
    static const int REVISION = 3;

public:
    NW4R_G3D_RESOURCE_FUNC_DEF(ResAnmVis);

    u32 GetRevision() const {
        return ref().revision;
    }

    bool CheckRevision() const {
        return GetRevision() == REVISION;
    }

    bool GetAnmResult(u32 idx, f32 frame) const;

    const ResAnmVisAnmData *GetNodeAnm(int idx) const {
        return static_cast<ResAnmVisAnmData *>(ofs_to_obj<ResDic>(ref().toVisDataDic)[idx]);
    }
    const ResAnmVisAnmData *GetNodeAnm(u32 idx) const {
        return static_cast<ResAnmVisAnmData *>(ofs_to_obj<ResDic>(ref().toVisDataDic)[idx]);
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
