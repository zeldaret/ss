#ifndef NW4R_G3D_ANMCHR_H
#define NW4R_G3D_ANMCHR_H
#include "common.h"
#include "nw4r/g3d/g3d_anmobj.h"
#include "nw4r/math/math_types.h"

namespace nw4r {
namespace g3d {
struct ChrAnmResult {
    u32 mFlags; // at 0x0
    math::VEC3 VEC3_0x4;
    math::VEC3 VEC3_0x10;
    math::MTX34 mMtx; // at 0x1C
};

class AnmObjChrRes;

class AnmObjChr : public AnmObj {
public:

    enum BindOption {};

    static const TypeObj GetTypeObjStatic() {
        return TypeObj(TYPE_NAME);
    }

    virtual ChrAnmResult *GetResult(ChrAnmResult *, u32); // at 0x38
    virtual void Attach(int, AnmObjChrRes*);              // at 0x3C
    virtual void Detach(int);                             // at 0x40
    virtual void DetachAll();                             // at 0x44
    virtual void SetWeight(int, f32);                     // at 0x48
    virtual f32  GetWeight(int) const;                    // at 0x4C
    virtual void Bind(ResMdl, u32, BindOption) = 0;       // at 0x50
    virtual void Release(ResMdl, u32, BindOption) = 0;    // at 0x54

private:
    UNKWORD field_0x10;
    UNKWORD field_0x14;

protected:
    NW4R_G3D_TYPE_OBJ_DECL(AnmObjChr);
};

class AnmObjChrNode : public AnmObjChr {
public:
    static const TypeObj GetTypeObjStatic() {
        return TypeObj(TYPE_NAME);
    }

    inline int Size() {
        return mNodeArraySize;
    }

    AnmObjChrRes *GetNode(int i) {
        return mpNodes[i];
    }

private:
    int mNodeArraySize;
    AnmObjChrRes **mpNodes;

protected:
    NW4R_G3D_TYPE_OBJ_DECL(AnmObjChrNode);
};

class AnmObjChrBlend : public AnmObjChrNode {
public:
    static const TypeObj GetTypeObjStatic() {
        return TypeObj(TYPE_NAME);
    }

    static AnmObjChrBlend *Construct(MEMAllocator*, u32*, ResMdl, int);

protected:
    NW4R_G3D_TYPE_OBJ_DECL(AnmObjChrBlend);
};

class AnmObjChrRes : public AnmObjChr, public FrameCtrl {
public:
    static const TypeObj GetTypeObjStatic() {
        return TypeObj(TYPE_NAME);
    }

    static AnmObjChrRes *Construct(MEMAllocator*, u32*, ResAnmChr, ResMdl, bool);

protected:
    NW4R_G3D_TYPE_OBJ_DECL(AnmObjChrRes);
};

} // namespace g3d
} // namespace nw4r

#endif
