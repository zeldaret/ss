#ifndef NW4R_G3D_RES_RES_ANM_SHP_H
#define NW4R_G3D_RES_RES_ANM_SHP_H

/** NOTICE: Revision change from 3->4. Structures, Enums, and/or Functions may have changed and not yet done so
 * (Zeldex72, Feb 1, 2025) */

#include <nw4r/types_nw4r.h>

#include "nw4r/g3d/res/g3d_resanm.h"
#include "nw4r/g3d/res/g3d_rescommon.h"
#include "nw4r/g3d/res/g3d_resdict.h"
#include "nw4r/g3d/res/g3d_resvtx.h"

namespace nw4r {
namespace g3d {

/******************************************************************************
 *
 * BlendVtx
 *
 ******************************************************************************/
struct ShpAnmVtxSet {
    ResVtxPos resVtxPos; // at 0x0
    ResVtxNrm resVtxNrm; // at 0x4
    ResVtxClr resVtxClr; // at 0x8

    friend bool operator==(const ShpAnmVtxSet &rLhs, const ShpAnmVtxSet &rRhs);
};

inline bool operator==(const ShpAnmVtxSet &rLhs, const ShpAnmVtxSet &rRhs) {
    return rLhs.resVtxPos == rRhs.resVtxPos && rLhs.resVtxNrm == rRhs.resVtxNrm && rLhs.resVtxClr == rRhs.resVtxClr;
}

struct BlendVtx {
    ShpAnmVtxSet vtxSet; // at 0x0
    f32 weight;          // at 0xC
};

/******************************************************************************
 *
 * ShpAnmResult
 *
 ******************************************************************************/
struct ShpAnmResult {
    enum Flag {
        FLAG_ANM_EXISTS = (1 << 0),
        FLAG_ANM_VTXPOS = (1 << 1),
        FLAG_ANM_VTXNRM = (1 << 2),
        FLAG_ANM_VTXCLR = (1 << 3),
    };

    static const int MAX_KEY_SHAPE = 32;

    u32 flags;                        // at 0x0
    u32 numKeyShape;                  // at 0x4
    ShpAnmVtxSet baseShapeVtxSet;     // at 0x8
    f32 baseShapeWeight;              // at 0x14
    BlendVtx keyShape[MAX_KEY_SHAPE]; // at 0x18
};

/******************************************************************************
 *
 * ResAnmShp
 *
 ******************************************************************************/
struct ResAnmShpAnmData {
    enum Flag {
        FLAG_ANM_EXISTS = (1 << 0),
        FLAG_ANM_VTXPOS = (1 << 1),
        FLAG_ANM_VTXNRM = (1 << 2),
        FLAG_ANM_VTXCLR = (1 << 3),
    };

    u32 flags;                 // at 0x0
    s32 name;                  // at 0x4
    u16 baseShapeVtxIdx;       // at 0x8
    u16 numKeyShape;           // at 0xA
    u32 constFlags;            // at 0xC
    s32 toAnmIdxToVtxIdxTable; // at 0x10
    ResAnmData anms[1];        // at 0x14
};

struct ResAnmShpInfoData {
    u16 numFrame;     // at 0x0
    u16 numVtxName;   // at 0x2
    AnmPolicy policy; // at 0x4
};

struct ResAnmShpData {
    ResBlockHeaderData header; // at 0x0
    u32 revision;              // at 0x8
    s32 toResFileData;         // at 0xC
    s32 toShpDataDic;          // at 0x10
    s32 toVtxNameArray;        // at 0x14
    s32 toResUserData;         // at 0x18
    s32 name;                  // at 0x1C
    s32 original_path;         // at 0x20
    ResAnmShpInfoData info;    // at 0x24
};

class ResAnmShp : public ResCommon<ResAnmShpData> {
public:
    static const u32 SIGNATURE = 'SHP0';
    static const int REVISION = 4;

public:
    NW4R_G3D_RESOURCE_FUNC_DEF(ResAnmShp);

    u32 GetRevision() const {
        return ref().revision;
    }

    bool CheckRevision() const {
        return GetRevision() == REVISION;
    }

    void GetAnmResult(ShpAnmResult *pResult, u32 idx, f32 frame, const ShpAnmVtxSet *pShapeArray) const;

    const ResAnmShpAnmData *GetShapeAnm(int idx) const {
        return static_cast<ResAnmShpAnmData *>(ofs_to_obj<ResDic>(ref().toShpDataDic)[idx]);
    }
    const ResAnmShpAnmData *GetShapeAnm(u32 idx) const {
        return static_cast<ResAnmShpAnmData *>(ofs_to_obj<ResDic>(ref().toShpDataDic)[idx]);
    }

    u32 GetShapeAnmNumEntries() const {
        return ofs_to_obj<ResDic>(ref().toShpDataDic).GetNumData();
    }

    const s32 *GetVtxNameArray() const {
        return ofs_to_ptr<s32>(ref().toVtxNameArray);
    }

    int GetNumFrame() const {
        return ref().info.numFrame;
    }

    int GetNumVtxNames() const {
        return ref().info.numVtxName;
    }

    AnmPolicy GetAnmPolicy() const {
        return ref().info.policy;
    }
};

} // namespace g3d
} // namespace nw4r

#endif
