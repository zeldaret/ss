#ifndef NW4R_G3D_ANMCLR_H
#define NW4R_G3D_ANMCLR_H
#include "common.h"
#include "nw4r/g3d/g3d_anmobj.h"
#include "nw4r/g3d/g3d_resanmclr.h"

namespace nw4r {
namespace g3d {
struct ClrAnmResult {
    enum { ANM_COUNT = 11 };

    u32 mFlags; // at 0x0
    u32 COLORS_0x4[ANM_COUNT];
    UNKWORD WORDS_0x30[ANM_COUNT];
};

class AnmObjMatClrRes;

class AnmObjMatClr : public AnmObj {
public:
    static const G3dObj::TypeObj GetTypeObjStatic() {
        return TypeObj(TYPE_NAME);
    }

    virtual ClrAnmResult *GetResult();              // at 0x38
    virtual void Attach(s32, AnmObjMatClrRes *res); // at 0x3C
    virtual void Detach(s32);                       // at 0x40

private:
    int mChildrenArraySize;
    u16 *mpChildrenArray;

    NW4R_G3D_TYPE_OBJ_DECL(AnmObjMatClr);
};

class AnmObjMatClrNode : public AnmObjMatClr {
public:
    static const G3dObj::TypeObj GetTypeObjStatic() {
        return TypeObj(TYPE_NAME);
    }

    inline int Size() {
        return mNodeArraySize;
    }

    AnmObjMatClrRes *GetNode(int i) {
        return mpNodes[i];
    }

private:
    int mNodeArraySize;
    AnmObjMatClrRes **mpNodes;

    NW4R_G3D_TYPE_OBJ_DECL(AnmObjMatClrNode);
};

class AnmObjMatClrRes : public AnmObjMatClr, public FrameCtrl {
public:
    static const G3dObj::TypeObj GetTypeObjStatic() {
        return TypeObj(TYPE_NAME);
    }

    static AnmObjMatClrRes *Construct(MEMAllocator *, u32 *, ResAnmClr, ResMdl, bool);

private:
    ResAnmClr mRes;
    TexSrtAnmResult *mpResultCache;

    NW4R_G3D_TYPE_OBJ_DECL(AnmObjMatClrRes);
};

class AnmObjMatClrOverride : public AnmObjMatClrNode {
public:
    static const G3dObj::TypeObj GetTypeObjStatic() {
        return TypeObj(TYPE_NAME);
    }

    static AnmObjMatClrOverride *Construct(MEMAllocator *, u32 *, ResMdl, int);

    NW4R_G3D_TYPE_OBJ_DECL(AnmObjMatClrOverride);
};

} // namespace g3d
} // namespace nw4r

#endif
