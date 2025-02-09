#ifndef NW4R_G3D_ANM_VIS_H
#define NW4R_G3D_ANM_VIS_H
#include <nw4r/types_nw4r.h>

#include <nw4r/g3d/g3d_anmobj.h>
#include <nw4r/g3d/res/g3d_resanmvis.h>

namespace nw4r {
namespace g3d {

void ApplyVisAnmResult(ResMdl mdl, AnmObjVis *pObj);
void ApplyVisAnmResult(u8 *pByteVec, ResMdl mdl, AnmObjVis *pObj);

/******************************************************************************
 *
 * AnmObjVis
 *
 ******************************************************************************/
// Forward declarations
class AnmObjVisRes;

class AnmObjVis : public AnmObj {
public:
    AnmObjVis(MEMAllocator *pAllocator, u16 *pBindingBuf, int numBinding);
    virtual void G3dProc(u32 task, u32 param, void *pInfo); // at 0xC
    virtual ~AnmObjVis() {}                                 // at 0x10

    virtual void SetFrame(f32 frame) = 0; // at 0x1C
    virtual f32 GetFrame() const = 0;     // at 0x20
    virtual void UpdateFrame() = 0;       // at 0x24

    virtual void SetUpdateRate(f32 rate) = 0; // at 0x28
    virtual f32 GetUpdateRate() const = 0;    // at 0x2C

    virtual bool Bind(const ResMdl mdl) = 0; // at 0x30
    virtual void Release();                  // at 0x34

    virtual bool GetResult(u32 idx) = 0; // at 0x38

    virtual AnmObjVisRes *Attach(int idx, AnmObjVisRes *pRes); // at 0x3C
    virtual AnmObjVisRes *Detach(int idx);                     // at 0x40
    void DetachAll();

    bool TestExistence(u32 idx) const;
    bool TestDefined(u32 idx) const;

protected:
    enum BindingFlag {
        BINDING_ID_MASK = (1 << 14) - 1,
        BINDING_INVALID = (1 << 14),
        BINDING_UNDEFINED = (1 << 15),
    };

protected:
    static const int MAX_CHILD = 4;

protected:
    int mNumBinding;      // at 0x10
    u16 *const mpBinding; // at 0x14

    NW4R_G3D_RTTI_DECL_DERIVED(AnmObjVis, AnmObj);
};

/******************************************************************************
 *
 * AnmObjVisNode
 *
 ******************************************************************************/
class AnmObjVisNode : public AnmObjVis {
public:
    AnmObjVisNode(MEMAllocator *pAllocator, u16 *pBindingBuf, int numBinding);
    virtual void G3dProc(u32 task, u32 param, void *pInfo); // at 0xC
    virtual ~AnmObjVisNode();                               // at 0x10

    virtual void SetFrame(f32 frame); // at 0x1C
    virtual f32 GetFrame() const;     // at 0x20
    virtual void UpdateFrame();       // at 0x24

    virtual void SetUpdateRate(f32 rate); // at 0x28
    virtual f32 GetUpdateRate() const;    // at 0x2C

    virtual bool Bind(const ResMdl mdl); // at 0x30
    virtual void Release();              // at 0x34

    virtual bool GetResult(u32 idx) = 0; // at 0x38

    virtual AnmObjVisRes *Attach(int idx, AnmObjVisRes *pRes); // at 0x3C
    virtual AnmObjVisRes *Detach(int idx);                     // at 0x40

protected:
    AnmObjVisRes *mpChildren[MAX_CHILD]; // at 0x18

    NW4R_G3D_RTTI_DECL_DERIVED(AnmObjVisNode, AnmObjVis);
};

/******************************************************************************
 *
 * AnmObjVisOR
 *
 ******************************************************************************/
class AnmObjVisOR : public AnmObjVisNode {
public:
    static AnmObjVisOR *Construct(MEMAllocator *pAllocator, u32 *pSize, ResMdl mdl);

    AnmObjVisOR(MEMAllocator *pAllocator, u16 *pBindingBuf, int numBinding)
        : AnmObjVisNode(pAllocator, pBindingBuf, numBinding) {}

    virtual ~AnmObjVisOR() {} // at 0x10

    virtual bool GetResult(u32 idx); // at 0x38

    NW4R_G3D_RTTI_DECL_DERIVED(AnmObjVisOR, AnmObjVisNode);
};

/******************************************************************************
 *
 * AnmObjVisRes
 *
 ******************************************************************************/
class AnmObjVisRes : public AnmObjVis, protected FrameCtrl {
public:
    static AnmObjVisRes *Construct(MEMAllocator *pAllocator, u32 *pSize, ResAnmVis vis, ResMdl mdl);

    AnmObjVisRes(MEMAllocator *pAllocator, ResAnmVis vis, u16 *pBindingBuf, int numBinding);
    virtual ~AnmObjVisRes() {} // at 0x10

    virtual void SetFrame(f32 frame); // at 0x1C
    virtual f32 GetFrame() const;     // at 0x20
    virtual void UpdateFrame();       // at 0x24

    virtual void SetUpdateRate(f32 rate); // at 0x28
    virtual f32 GetUpdateRate() const;    // at 0x2C

    virtual bool Bind(const ResMdl mdl); // at 0x30

    virtual bool GetResult(u32 idx); // at 0x38

    ResAnmVis GetResAnm() {
        return mRes;
    }

    void SetPlayPolicy(PlayPolicyFunc pFunc) {
        FrameCtrl::SetPlayPolicy(pFunc);
    }

private:
    ResAnmVis mRes; // at 0x2C

    NW4R_G3D_RTTI_DECL_DERIVED(AnmObjVisRes, AnmObjVis);
};

} // namespace g3d
} // namespace nw4r

#endif
