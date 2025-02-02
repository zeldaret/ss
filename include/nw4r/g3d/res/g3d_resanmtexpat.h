#ifndef NW4R_G3D_RES_RES_ANM_TEX_PAT_H
#define NW4R_G3D_RES_RES_ANM_TEX_PAT_H

/** NOTICE: Revision change from 3->4. Structures, Enums, and/or Functions may have changed and not yet done so
 * (Zeldex72, Feb 1, 2025) */

#include <nw4r/types_nw4r.h>

#include <nw4r/g3d/res/g3d_resanm.h>
#include <nw4r/g3d/res/g3d_rescommon.h>
#include <nw4r/g3d/res/g3d_resdict.h>
#include <nw4r/g3d/res/g3d_resmat.h>
#include <nw4r/g3d/res/g3d_respltt.h>
#include <nw4r/g3d/res/g3d_restex.h>

namespace nw4r {
namespace g3d {

/******************************************************************************
 *
 * TexPatAnmResult
 *
 ******************************************************************************/
struct TexPatAnmResult {
    static const int NUM_OF_ANMS = 8;

    u8 bTexExist;              // at 0x0
    u8 bPlttExist;             // at 0x1
    u8 PADDING_0x2[0x4 - 0x2]; // at 0x2
    ResTex tex[NUM_OF_ANMS];   // at 0x4
    ResPltt pltt[NUM_OF_ANMS]; // at 0x24
};

/******************************************************************************
 *
 * ResAnmTexPat
 *
 ******************************************************************************/
struct ResAnmTexPatFrmData {
    f32 frame;     // at 0x0
    u16 texIndex;  // at 0x4
    u16 plttIndex; // at 0x6
};

struct ResAnmTexPatAnmData {
    u16 numFrameValues;                 // at 0x0
    u8 PADDING_0x2[0x4 - 0x2];          // at 0x2
    f32 invKeyFrameRange;               // at 0x4
    ResAnmTexPatFrmData frameValues[1]; // at 0x8
};

struct ResAnmTexPatMatData {
    enum Flag {
        FLAG_ANM_EXISTS = (1 << 0),
        FLAG_ANM_CONST = (1 << 1),
        FLAG_ANM_TEX = (1 << 2),
        FLAG_ANM_PLTT = (1 << 3),

        // Four bits in 'flags' for each animation
        NUM_OF_FLAGS = 4
    };

    union AnmData {
        s32 toResAnmTexPatAnmData; // at 0x0

        struct {
            u16 texIndex;  // at 0x0
            u16 plttIndex; // at 0x2
        } constValue;
    };

    s32 name;        // at 0x0
    u32 flags;       // at 0x4
    AnmData anms[1]; // at 0x8
};

struct ResAnmTexPatInfoData {
    u16 numFrame;     // at 0x0
    u16 numMaterial;  // at 0x2
    u16 numTexture;   // at 0x4
    u16 numPalette;   // at 0x6
    AnmPolicy policy; // at 0x8
};

struct ResAnmTexPatData {
    ResBlockHeaderData header; // at 0x0
    u32 revision;              // at 0x8
    s32 toResFileData;         // at 0xC
    s32 toTexPatDataDic;       // at 0x10
    s32 toTexNameArray;        // at 0x14
    s32 toPlttNameArray;       // at 0x18
    s32 toResTexArray;         // at 0x1C
    s32 toResPlttArray;        // at 0x20
    s32 toResUserData;         // at 0x24
    s32 name;                  // at 0x28
    s32 original_path;         // at 0x2C
    ResAnmTexPatInfoData info; // at 0x30
};

class ResAnmTexPat : public ResCommon<ResAnmTexPatData> {
public:
    static const u32 SIGNATURE = 'PAT0';
    static const int REVISION = 4;

public:
    NW4R_G3D_RESOURCE_FUNC_DEF(ResAnmTexPat);

    bool Bind(const ResFile file);
    void Release();

    ResFile GetParent() const;

    u32 GetRevision() const {
        return ref().revision;
    }

    bool CheckRevision() const {
        return GetRevision() == REVISION;
    }

    void GetAnmResult(TexPatAnmResult *pResult, u32 idx, f32 frame) const;

    const ResAnmTexPatMatData *GetMatAnm(int idx) const {
        return static_cast<ResAnmTexPatMatData *>(ofs_to_obj<ResDic>(ref().toTexPatDataDic)[idx]);
    }
    const ResAnmTexPatMatData *GetMatAnm(u32 idx) const {
        return static_cast<ResAnmTexPatMatData *>(ofs_to_obj<ResDic>(ref().toTexPatDataDic)[idx]);
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
