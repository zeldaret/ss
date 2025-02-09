#ifndef NW4R_G3D_ANM_TEX_PAT_H
#define NW4R_G3D_ANM_TEX_PAT_H
#include <nw4r/types_nw4r.h>

#include <nw4r/g3d/g3d_anmobj.h>
#include <nw4r/g3d/res/g3d_resanmtexpat.h>

namespace nw4r {
namespace g3d {

void ApplyTexPatAnmResult(ResTexObj texObj, ResTlutObj tlutObj, const TexPatAnmResult *pResult);

/******************************************************************************
 *
 * AnmObjTexPat
 *
 ******************************************************************************/
// Forward declarations
class AnmObjTexPatRes;

class AnmObjTexPat : public AnmObj {
public:
    AnmObjTexPat(MEMAllocator *pAllocator, u16 *pBindingBuf, int numBinding);
    virtual void G3dProc(u32 task, u32 param, void *pInfo) = 0; // at 0xC
    virtual ~AnmObjTexPat() {}                                  // at 0x10

    virtual void SetFrame(f32 frame) = 0; // at 0x1C
    virtual f32 GetFrame() const = 0;     // at 0x20
    virtual void UpdateFrame() = 0;       // at 0x24

    virtual void SetUpdateRate(f32 rate) = 0; // at 0x28
    virtual f32 GetUpdateRate() const = 0;    // at 0x2C

    virtual bool Bind(const ResMdl mdl) = 0; // at 0x30
    virtual void Release();                  // at 0x34

    virtual const TexPatAnmResult *GetResult(TexPatAnmResult *pResult,
                                             u32 idx) = 0; // at 0x38

    virtual AnmObjTexPatRes *Attach(int idx, AnmObjTexPatRes *pRes); // at 0x3C
    virtual AnmObjTexPatRes *Detach(int idx);                        // at 0x40
    virtual void DetachAll();                                        // at 0x44

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

    NW4R_G3D_RTTI_DECL_DERIVED(AnmObjTexPat, AnmObj);
};

/******************************************************************************
 *
 * AnmObjTexPatNode
 *
 ******************************************************************************/
class AnmObjTexPatNode : public AnmObjTexPat {
public:
    AnmObjTexPatNode(
        MEMAllocator *pAllocator, u16 *pBindingBuf, int numBinding, AnmObjTexPatRes **ppChildrenBuf, int numChildren
    );
    virtual void G3dProc(u32 task, u32 param, void *pInfo); // at 0xC
    virtual ~AnmObjTexPatNode();                            // at 0x10

    virtual void SetFrame(f32 frame); // at 0x1C
    virtual f32 GetFrame() const;     // at 0x20
    virtual void UpdateFrame();       // at 0x24

    virtual void SetUpdateRate(f32 rate); // at 0x28
    virtual f32 GetUpdateRate() const;    // at 0x2C

    virtual bool Bind(const ResMdl mdl); // at 0x30
    virtual void Release();              // at 0x34

    virtual AnmObjTexPatRes *Attach(int idx, AnmObjTexPatRes *pRes); // at 0x3C
    virtual AnmObjTexPatRes *Detach(int idx);                        // at 0x40
    virtual void DetachAll();                                        // at 0x44

    inline int Size() {
        return mChildrenArraySize;
    }

    AnmObjTexPatRes *GetNode(int i) {
        return mpChildrenArray[i];
    }

protected:
    int mChildrenArraySize;            // at 0x18
    AnmObjTexPatRes **mpChildrenArray; // at 0x1C

    NW4R_G3D_RTTI_DECL_DERIVED(AnmObjTexPatNode, AnmObjTexPat);
};

/******************************************************************************
 *
 * AnmObjTexPatOverride
 *
 ******************************************************************************/
class AnmObjTexPatOverride : public AnmObjTexPatNode {
public:
    static AnmObjTexPatOverride *Construct(MEMAllocator *pAllocator, u32 *pSize, ResMdl mdl, int numChildren);

    AnmObjTexPatOverride(
        MEMAllocator *pAllocator, u16 *pBindingBuf, int numBinding, AnmObjTexPatRes **ppChildrenBuf, int numChildren
    )
        : AnmObjTexPatNode(pAllocator, pBindingBuf, numBinding, ppChildrenBuf, numChildren) {}

    virtual ~AnmObjTexPatOverride() {} // at 0x10

    virtual const TexPatAnmResult *GetResult(TexPatAnmResult *pResult,
                                             u32 idx); // at 0x38

    NW4R_G3D_RTTI_DECL_DERIVED(AnmObjTexPatOverride, AnmObjTexPatNode);
};

/******************************************************************************
 *
 * AnmObjTexPatRes
 *
 ******************************************************************************/
class AnmObjTexPatRes : public AnmObjTexPat, protected FrameCtrl {
public:
    static AnmObjTexPatRes *Construct(MEMAllocator *pAllocator, u32 *pSize, ResAnmTexPat pat, ResMdl mdl, bool cache);

    AnmObjTexPatRes(
        MEMAllocator *pAllocator, ResAnmTexPat pat, u16 *pBindingBuf, int numBinding, TexPatAnmResult *pCacheBuf
    );
    virtual void G3dProc(u32 task, u32 param, void *pInfo); // at 0xC
    virtual ~AnmObjTexPatRes() {}                           // at 0x10

    virtual void SetFrame(f32 frame); // at 0x1C
    virtual f32 GetFrame() const;     // at 0x20
    virtual void UpdateFrame();       // at 0x24

    virtual void SetUpdateRate(f32 rate); // at 0x28
    virtual f32 GetUpdateRate() const;    // at 0x2C

    virtual bool Bind(const ResMdl mdl); // at 0x30

    virtual const TexPatAnmResult *GetResult(TexPatAnmResult *pResult,
                                             u32 idx); // at 0x38

    void UpdateCache();

    ResAnmTexPat GetResAnm() {
        return mRes;
    }

    void SetPlayPolicy(PlayPolicyFunc pFunc) {
        FrameCtrl::SetPlayPolicy(pFunc);
    }

private:
    ResAnmTexPat mRes;                    // at 0x2C
    TexPatAnmResult *const mpResultCache; // at 0x30

    NW4R_G3D_RTTI_DECL_DERIVED(AnmObjTexPatRes, AnmObjTexPat);
};

} // namespace g3d
} // namespace nw4r

#endif
