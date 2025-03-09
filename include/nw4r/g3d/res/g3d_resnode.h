#ifndef NW4R_G3D_RES_RES_NODE_H
#define NW4R_G3D_RES_RES_NODE_H
#include <nw4r/types_nw4r.h>

#include <nw4r/g3d/res/g3d_rescommon.h>

#include <nw4r/math.h>

namespace nw4r {
namespace g3d {

// Forward declarations
struct ChrAnmResult;

/******************************************************************************
 *
 * Common types
 *
 ******************************************************************************/
struct ResNodeDataTypedef {
    enum Billboard {
        BILLBOARD_OFF,
        BILLBOARD_STD,
        BILLBOARD_PERSP_STD,
        BILLBOARD_ROT,
        BILLBOARD_PERSP_ROT,
        BILLBOARD_Y_OFF,
        BILLBOARD_PERSP_Y,

        NUM_BILLBOARD,
    };
};

/******************************************************************************
 *
 * ResNode
 *
 ******************************************************************************/
struct ResNodeData : ResNodeDataTypedef {
    enum Flag {
        FLAG_IDENTITY = (1 << 0),
        FLAG_TRANS_ZERO = (1 << 1),
        FLAG_ROT_ZERO = (1 << 2),
        FLAG_SCALE_ONE = (1 << 3),
        FLAG_SCALE_UNIFORM = (1 << 4),

        // Maya Scale Compensation
        FLAG_SSC_APPLY = (1 << 5),
        FLAG_SSC_PARENT = (1 << 6),

        // Softimage Hierarchical Scaling
        FLAG_XSI_SCALING = (1 << 7),

        FLAG_VISIBLE = (1 << 8),
        FLAG_GEOMETRY = (1 << 9),
        FLAG_BILLBOARD_PARENT = (1 << 10)
    };

    u32 size;                 // at 0x0
    s32 toResMdlData;         // at 0x4
    s32 name;                 // at 0x8
    u32 id;                   // at 0xC
    u32 mtxID;                // at 0x10
    u32 flags;                // at 0x14
    Billboard bbmode;         // at 0x18
    u32 bbref_nodeid;         // at 0x1C
    math::_VEC3 scale;        // at 0x20
    math::_VEC3 rot;          // at 0x2C
    math::_VEC3 translate;    // at 0x38
    math::_VEC3 volume_min;    // at 0x44
    math::_VEC3 volume_max;    // at 0x50
    s32 toParentNode;         // at 0x5C
    s32 toChildNode;          // at 0x60
    s32 toNextSibling;        // at 0x64
    s32 toPrevSibling;        // at 0x68
    s32 toResUserData;        // at 0x6C
    math::_MTX34 modelMtx;    // at 0x70
    math::_MTX34 invModelMtx; // at 0xA0
};

class ResNode : public ResCommon<ResNodeData>, public ResNodeDataTypedef {
public:
    NW4R_G3D_RESOURCE_FUNC_DEF(ResNode);

    void PatchChrAnmResult(ChrAnmResult *pResult) const;
    void CalcChrAnmResult(ChrAnmResult *pResult) const;

    ResName GetResName() const {
        const ResNodeData &r = ref();

        if (r.name != 0) {
            return NW4R_G3D_OFS_TO_RESNAME(&r, r.name);
        }

        return ResName(NULL);
    }

    u32 GetID() const {
        if (IsValid()) {
            return ptr()->id;
        }

        return 0;
    }

    u32 GetMtxID() const {
        if (IsValid()) {
            return ptr()->mtxID;
        }

        return 0;
    }

    bool IsVisible() const {
        if (IsValid()) {
            return ptr()->flags & ResNodeData::FLAG_VISIBLE;
        }

        return false;
    }

    void SetVisibility(bool visible) {
        if (!IsValid()) {
            return;
        }

        if (visible) {
            ptr()->flags |= ResNodeData::FLAG_VISIBLE;
        } else {
            ptr()->flags &= ~ResNodeData::FLAG_VISIBLE;
        }
    }

    Billboard GetBillboardMode() const {
        if (IsValid()) {
            return ptr()->bbmode;
        }

        return BILLBOARD_OFF;
    }

    const math::VEC3 &GetTranslate() const {
        return ref().translate;
    }

    // not in the dwarf
    const math::VEC3 &GetBoundsMin() const {
        return *(const math::VEC3*)&ref().volume_min;
    }

    // not in the dwarf
    const math::VEC3 &GetBoundsMax() const {
        return *(const math::VEC3*)&ref().volume_max;
    }

    ResNode GetParentNode() {
        return ofs_to_obj<ResNode>(ref().toParentNode);
    }
    ResNode GetParentNode() const {
        return ofs_to_obj<ResNode>(ref().toParentNode);
    }

    ResNode GetChildNode() {
        return ofs_to_obj<ResNode>(ref().toChildNode);
    }
    ResNode GetChildNode() const {
        return ofs_to_obj<ResNode>(ref().toChildNode);
    }

    ResNode GetNextSibling() {
        return ofs_to_obj<ResNode>(ref().toNextSibling);
    }
    ResNode GetNextSibling() const {
        return ofs_to_obj<ResNode>(ref().toNextSibling);
    }

    ResNode GetPrevSibling() {
        return ofs_to_obj<ResNode>(ref().toPrevSibling);
    }
    ResNode GetPrevSibling() const {
        return ofs_to_obj<ResNode>(ref().toPrevSibling);
    }

    void EndEdit() {}
};

} // namespace g3d
} // namespace nw4r

#endif
