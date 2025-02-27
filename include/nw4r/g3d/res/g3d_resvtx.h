#ifndef NW4R_G3D_RES_RES_VTX_H
#define NW4R_G3D_RES_RES_VTX_H
#include <nw4r/types_nw4r.h>

#include <nw4r/g3d/res/g3d_rescommon.h>

#include <nw4r/math.h>

#include <rvl/GX.h>

namespace nw4r {
namespace g3d {

/******************************************************************************
 *
 * ResVtxPos
 *
 ******************************************************************************/
struct ResVtxPosData {
    u32 size;          // at 0x0
    s32 toResMdlData;  // at 0x4
    s32 toVtxPosArray; // at 0x8
    s32 name;          // at 0xC
    u32 id;            // at 0x10
    GXCompCnt cmpcnt;  // at 0x14
    GXCompType tp;     // at 0x18
    u8 frac;           // at 0x1C
    u8 stride;         // at 0x1D
    u16 numPos;        // at 0x1E
    math::_VEC3 min;   // at 0x20
    math::_VEC3 max;   // at 0x2C
};

class ResVtxPos : public ResCommon<ResVtxPosData> {
public:
    NW4R_G3D_RESOURCE_FUNC_DEF(ResVtxPos);

    void Init() {
        DCStore(false);
    }

    void SetArray();
    void GetArray(const void **ppBase, u8 *pStride) const;

    void CopyTo(void *pDst) const;
    void DCStore(bool sync);

    u32 GetSize() const {
        return ref().size;
    }

    void *GetData() {
        return ofs_to_ptr<void>(ref().toVtxPosArray);
    }
    const void *GetData() const {
        return ofs_to_ptr<void>(ref().toVtxPosArray);
    }

    u32 GetID() const {
        return ref().id;
    }

    u16 GetNumVtxPos() const {
        return ref().numPos;
    }
};

/******************************************************************************
 *
 * ResVtxNrm
 *
 ******************************************************************************/
struct ResVtxNrmData {
    u32 size;          // at 0x0
    s32 toResMdlData;  // at 0x4
    s32 toVtxNrmArray; // at 0x8
    s32 name;          // at 0xC
    u32 id;            // at 0x10
    GXCompCnt cmpcnt;  // at 0x14
    GXCompType tp;     // at 0x18
    u8 frac;           // at 0x1C
    u8 stride;         // at 0x1D
    u16 numNrm;        // at 0x1E
};

class ResVtxNrm : public ResCommon<ResVtxNrmData> {
public:
    NW4R_G3D_RESOURCE_FUNC_DEF(ResVtxNrm);

    void Init() {
        DCStore(false);
    }

    void SetArray();
    void GetArray(const void **ppBase, u8 *pStride) const;

    void CopyTo(void *pDst) const;
    void DCStore(bool);

    u32 GetSize() const {
        return ref().size;
    }

    void *GetData() {
        return ofs_to_ptr<void>(ref().toVtxNrmArray);
    }
    const void *GetData() const {
        return ofs_to_ptr<void>(ref().toVtxNrmArray);
    }

    u32 GetID() const {
        return ref().id;
    }

    u16 GetNumVtxNrm() const {
        return ref().numNrm;
    }
};

/******************************************************************************
 *
 * ResVtxClr
 *
 ******************************************************************************/
struct ResVtxClrData {
    u32 size;          // at 0x0
    s32 toResMdlData;  // at 0x4
    s32 toVtxClrArray; // at 0x8
    s32 name;          // at 0xC
    u32 id;            // at 0x10
    GXCompCnt cmpcnt;  // at 0x14
    GXCompType tp;     // at 0x18
    u8 stride;         // at 0x1C
    u8 PADDING_0x1D;   // at 0x1D
    u16 numClr;        // at 0x1E
};

class ResVtxClr : public ResCommon<ResVtxClrData> {
public:
    NW4R_G3D_RESOURCE_FUNC_DEF(ResVtxClr);

    void Init() {
        DCStore(false);
    }

    void SetArray(GXAttr attr);
    void GetArray(const void **ppBase, u8 *pStride) const;

    void CopyTo(void *pDst) const;
    void DCStore(bool sync);

    u32 GetSize() const {
        return ref().size;
    }

    void *GetData() {
        return ofs_to_ptr<void>(ref().toVtxClrArray);
    }
    const void *GetData() const {
        return ofs_to_ptr<void>(ref().toVtxClrArray);
    }

    u32 GetID() const {
        return ref().id;
    }

    u16 GetNumVtxClr() const {
        return ref().numClr;
    }
};

/******************************************************************************
 *
 * ResVtxTexCoord
 *
 ******************************************************************************/
struct ResVtxTexCoordData {
    u32 size;            // at 0x0
    s32 toResMdlData;    // at 0x4
    s32 toTexCoordArray; // at 0x8
    s32 name;            // at 0xC
    u32 id;              // at 0x10
    GXCompCnt cmpcnt;    // at 0x14
    GXCompType tp;       // at 0x18
    u8 frac;             // at 0x1C
    u8 stride;           // at 0x1D
    u16 numTexCoord;     // at 0x1E
    math::_VEC2 min;     // at 0x20
    math::_VEC2 max;     // at 0x2C
};

class ResVtxTexCoord : public ResCommon<ResVtxTexCoordData> {
public:
    NW4R_G3D_RESOURCE_FUNC_DEF(ResVtxTexCoord);

    void Init() {
        DCStore(false);
    }

    void GetArray(const void **ppBase, u8 *pStride) const;
    void DCStore(bool sync);

    u32 GetSize() const {
        return ref().size;
    }

    void *GetData() {
        return ofs_to_ptr<void>(ref().toTexCoordArray);
    }
    const void *GetData() const {
        return ofs_to_ptr<void>(ref().toTexCoordArray);
    }

    u32 GetID() const {
        return ref().id;
    }

    u16 GetNumTexCoord() const {
        return ref().numTexCoord;
    }
};

} // namespace g3d
} // namespace nw4r

#endif
