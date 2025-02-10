#ifndef NW4R_G3D_RES_RES_SHP_H
#define NW4R_G3D_RES_RES_SHP_H

#include <nw4r/types_nw4r.h>

#include <nw4r/g3d/res/g3d_rescommon.h>
// #include <nw4r/g3d/res/g3d_resmdl.h>
#include <nw4r/g3d/res/g3d_resvtx.h>

#include <rvl/GX.h>

namespace nw4r {
namespace g3d {

/******************************************************************************
 *
 * ResShpPrePrim
 *
 ******************************************************************************/
struct ResPrePrimDL {
    union {
        struct {
            u8 cullMode[10];                                          // at 0x0
            u8 vtxDescv[21];                                          // at 0xA
            u8 PADDING_0x1F;                                          // at 0x1F
            u8 vtxFmtv[GX_CP_CMD_SZ * 3];                             // at 0x20
            u8 array[GX_POS_MTX_ARRAY - GX_VA_POS][GX_CP_CMD_SZ * 2]; // at 0x32
            u8 PADDING_0xC2[0xE0 - 0xC2];                             // at 0xC2
        } dl;

        u8 data[0xE0]; // at 0x0
    };
};

class ResShpPrePrim : public ResCommon<ResPrePrimDL> {
public:
    NW4R_G3D_RESOURCE_FUNC_DEF_EX(ResShpPrePrim, ResPrePrimDL);

    void DCStore(bool sync);
};

/******************************************************************************
 *
 * ResShp
 *
 ******************************************************************************/
struct ResCacheVtxDescv {
    union {
        u32 data_u32[0xC / sizeof(u32)];
        u8 data[0xC / sizeof(u8)];
    }; // at 0x0

    void Clear() {
        data_u32[0] = data_u32[1] = data_u32[2] = 0;
    }

    bool operator==(const ResCacheVtxDescv &rRhs) const {
        return data_u32[0] == rRhs.data_u32[0] && data_u32[1] == rRhs.data_u32[1] && data_u32[2] == rRhs.data_u32[2];
    }
};

struct ResMtxSetUsed {
    u32 numMtxID;    // at 0x0
    u16 vecMtxID[2]; // at 0x4
};

struct ResShpData {
    enum IDFlag {
        ID_FLAG_ENVELOPE = (1 << 31),
    };

    enum Flag {
        FLAG_INVISIBLE = (1 << 1),
    };

    u32 size;                                         // at 0x0
    s32 toResMdlData;                                 // at 0x4
    s32 curMtxIdx;                                    // at 0x8
    ResCacheVtxDescv cache;                           // at 0xC
    ResTagDLData tagPrePrimDL;                        // at 0x18
    ResTagDLData tagPrimDL;                           // at 0x24
    u32 vcdBitmap;                                    // at 0x30
    u32 flag;                                         // at 0x34
    s32 name;                                         // at 0x38
    u32 id;                                           // at 0x3C
    u32 numVtx;                                       // at 0x40
    u32 numPolygon;                                   // at 0x44
    s16 idVtxPosition;                                // at 0x48
    s16 idVtxNormal;                                  // at 0x4A
    s16 idVtxColor[GX_VA_TEX0 - GX_VA_CLR0];          // at 0x4C
    s16 idVtxTexCoord[GX_POS_MTX_ARRAY - GX_VA_TEX0]; // at 0x50
    s16 idVtxFurVec;                                  // at 0x60
    s16 idVtxFurPos;                                  // at 0x62
    s32 toMtxSetUsed;                                 // at 0x64
    ResMtxSetUsed msu;                                // at 0x68
};

class ResShp : public ResCommon<ResShpData> {
public:
    NW4R_G3D_RESOURCE_FUNC_DEF(ResShp);

    void Init();
    void Terminate();

    ResMdl GetParent() const;

    bool GXGetVtxDescv(GXVtxDescList *pList) const;
    bool GXGetVtxAttrFmtv(GXVtxAttrFmtList *pList) const;
    void GXSetArray(GXAttr attr, const void *pBase, u8 stride);
    void DisableSetArray(GXAttr attr);

    ResVtxPos GetResVtxPos() const;
    ResVtxNrm GetResVtxNrm() const;
    ResVtxClr GetResVtxClr(u32 idx) const;
    ResVtxTexCoord GetResVtxTexCoord(u32 idx) const;

    void CallPrePrimitiveDisplayList(bool sync, bool cacheIsSame) const;
    void CallPrimitiveDisplayList(bool sync) const;

    ResTagDL GetPrePrimDLTag() {
        return ResTagDL(&ref().tagPrePrimDL);
    }
    ResTagDL GetPrePrimDLTag() const {
        return ResTagDL(const_cast<ResTagDLData *>(&ref().tagPrePrimDL));
    }

    ResTagDL GetPrimDLTag() {
        return ResTagDL(&ref().tagPrimDL);
    }
    ResTagDL GetPrimDLTag() const {
        return ResTagDL(const_cast<ResTagDLData *>(&ref().tagPrimDL));
    }

    ResShpPrePrim GetResShpPrePrim() {
        return ResShpPrePrim(GetPrePrimDLTag().GetDL());
    }
    ResShpPrePrim GetResShpPrePrim() const {
        return ResShpPrePrim(GetPrePrimDLTag().GetDL());
    }

    bool ExistVtxDesc(GXAttr attr) const {
        return ref().vcdBitmap & (1 << attr);
    }

    bool IsVisible() const {
        return !(ref().flag & ResShpData::FLAG_INVISIBLE);
    }
};

} // namespace g3d
} // namespace nw4r

#endif
