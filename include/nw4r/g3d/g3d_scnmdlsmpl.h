#ifndef NW4R_G3D_SCN_MDL_SIMPLE_H
#define NW4R_G3D_SCN_MDL_SIMPLE_H
#include "common.h"
#include "nw4r/math/math_types.h"
#include "nw4r/g3d/g3d_resmdl.h"
#include "nw4r/g3d/g3d_scnobj.h"
#include "nw4r/g3d/g3d_calcworld.h"

namespace nw4r {
namespace g3d {
class ScnMdlSimple : public ScnLeaf {
public:
    enum ByteCodeType {
        BYTE_CODE_CALC,
        BYTE_CODE_MIX,
        BYTE_CODE_DRAW_OPA,
        BYTE_CODE_DRAW_XLU,
    };

    enum AnmObjType {
        ANMOBJ_0,
        ANMOBJ_1,
        ANMOBJ_2,
        ANMOBJ_3,
        ANMOBJ_4,
        ANMOBJ_5,
        ANMOBJ_6,
    };

public:
    ScnMdlSimple(MEMAllocator *, ResMdl, math::MTX34 *, u32 *, math::MTX34 *, math::MTX33 *, math::MTX34 *, int, int);

    static ScnMdlSimple *Construct(MEMAllocator*, unsigned long*, nw4r::g3d::ResMdl, int);

    bool GetScnMtxPos(math::MTX34* pOut, ScnObjMtxType tp, u32 nodeID) const;

    virtual bool IsDerivedFrom(TypeObj other) const // at 0x8
    {
        return (other == GetTypeObjStatic()) ? true : ScnLeaf::IsDerivedFrom(other);
    }
    static const TypeObj GetTypeObjStatic() {
        return TypeObj(TYPE_NAME);
    }
    virtual void G3dProc(u32, u32, void *);  // at 0xC
    virtual ~ScnMdlSimple();                 // at 0x10
    virtual const TypeObj GetTypeObj() const // at 0x14
    {
        return TypeObj(TYPE_NAME);
    }
    virtual const char *GetTypeName() const // at 0x18
    {
        return GetTypeObj().GetTypeName();
    }

    virtual bool SetAnmObj(AnmObj* p, AnmObjType type);
    virtual bool RemoveAnmObj(AnmObj *p);
    virtual bool RemoveAnmObj(AnmObjType type);

    const u8 *GetByteCode(ByteCodeType) const;
    void EnableScnMdlCallbackTiming(Timing);

    const ResMdl GetResMdl() const {
        return mResMdl;
    }
    ResMdl GetResMdl() {
        return mResMdl;
    }

    math::MTX34 *GetWldMtxArray() {
        return mWldMatrixArray;
    }
    u32 *GetWldMtxAttribArray() {
        return mWldMtxAttribArray;
    }

    u16 GetNumViewMtx() const {
        return mNumViewMtx;
    }

    void SetCalcWorldCallback(ICalcWorldCallback *cb) {
        mpCalcWorldCallback = cb;
    }

private:
    ResMdl mResMdl;               // at 0xE8
    math::MTX34 *mWldMatrixArray; // at 0xEC
    u32 *mWldMtxAttribArray;      // at 0xF0
    math::MTX34 *PTR_0xF4;
    math::MTX33 *PTR_0xF8;
    math::MTX34 *PTR_0xFC;
    u8 BYTE_0x100;
    u8 BYTE_0x101;
    u16 mNumViewMtx; // at 0x102
    UNKWORD WORD_0x104;
    void *mByteCodeCalc;    // at 0x108
    void *mByteCodeMix;     // at 0x10C
    void *mByteCodeDrawOpa; // at 0x110
    void *mByteCodeDrawXlu; // at 0x114
    UNKWORD WORD_0x118;
    ICalcWorldCallback *mpCalcWorldCallback; // at 0x11C

    NW4R_G3D_TYPE_OBJ_DECL(ScnMdlSimple);
};
} // namespace g3d
} // namespace nw4r

#endif
