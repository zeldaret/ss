#ifndef NW4R_G3D_ANMSHP_H
#define NW4R_G3D_ANMSHP_H
#include "common.h"
#include "nw4r/g3d/g3d_anmobj.h"
#include "nw4r/g3d/g3d_resanmshp.h"

// This header is based on the SS ghidra, the TypeNames in the binary,
// and an assumed symmetry to AnmObjVis. Everything about it could be wrong.

namespace nw4r {
namespace g3d {

class AnmObjShpRes;

class AnmObjShp : public AnmObj {
public:
    AnmObjShp(MEMAllocator *, u16 *, int);
    virtual bool IsDerivedFrom(TypeObj other) const // at 0x8
    {
        return (other == GetTypeObjStatic()) ? true : AnmObj::IsDerivedFrom(other);
    }
    virtual void G3dProc(u32, u32, void *);  // at 0xC
    virtual ~AnmObjShp() {}                  // at 0x10
    virtual const TypeObj GetTypeObj() const // at 0x14
    {
        return TypeObj(TYPE_NAME);
    }
    virtual const char *GetTypeName() const // at 0x18
    {
        return GetTypeObj().GetTypeName();
    }
    virtual void SetFrame(f32) = 0;                    // at 0x1C
    virtual f32 GetFrame() const = 0;                  // at 0x20
    virtual void UpdateFrame() = 0;                    // at 0x24
    virtual void SetUpdateRate(f32) = 0;               // at 0x28
    virtual f32 GetUpdateRate() const = 0;             // at 0x2C
    virtual bool Bind(ResMdl) = 0;                     // at 0x30
    virtual void Release();                            // at 0x34
    virtual bool GetResult(u32) = 0;                   // at 0x38
    virtual AnmObjShpRes *Attach(int, AnmObjShpRes *); // at 0x3C
    virtual AnmObjShpRes *Detach(int);                 // at 0x40

    static const TypeObj GetTypeObjStatic() {
        return TypeObj(TYPE_NAME);
    }

    bool TestDefined(u32 idx) const;
    bool TestExistence(u32 idx) const;
    void DetachAll();

protected:
    static const int MAX_RESOURCES = 4;

    int mNumBinds; // at 0x10
    u16 *mBinds;   // at 0x14

    NW4R_G3D_TYPE_OBJ_DECL(AnmObjShp);
};

class AnmObjShpRes : public AnmObjShp, public FrameCtrl {
public:
    AnmObjShpRes(MEMAllocator *, ResAnmShp, u16 *, int);

    virtual bool IsDerivedFrom(TypeObj other) const // at 0x8
    {
        return (other == GetTypeObjStatic()) ? true : AnmObjShp::IsDerivedFrom(other);
    }
    virtual ~AnmObjShpRes() {}               // at 0x10
    virtual const TypeObj GetTypeObj() const // at 0x14
    {
        return TypeObj(TYPE_NAME);
    }
    virtual const char *GetTypeName() const // at 0x18
    {
        return GetTypeObj().GetTypeName();
    }
    virtual void SetFrame(f32);        // at 0x1C
    virtual f32 GetFrame() const;      // at 0x20
    virtual void UpdateFrame();        // at 0x24
    virtual void SetUpdateRate(f32);   // at 0x28
    virtual f32 GetUpdateRate() const; // at 0x2C
    virtual bool Bind(ResMdl);         // at 0x30
    virtual bool GetResult(u32);       // at 0x38

    static const TypeObj GetTypeObjStatic() {
        return TypeObj(TYPE_NAME);
    }

    static AnmObjShpRes *Construct(MEMAllocator *, u32 *, ResAnmShp, ResMdl, bool);

private:
    ResAnmShp mResAnmShp; // at 0x2C

    NW4R_G3D_TYPE_OBJ_DECL(AnmObjShpRes);
};
} // namespace g3d
} // namespace nw4r

#endif
