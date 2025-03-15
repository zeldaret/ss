#ifndef NW4R_G3D_ANM_TEX_SRT_H
#define NW4R_G3D_ANM_TEX_SRT_H
#include <nw4r/types_nw4r.h>

#include "nw4r/g3d/g3d_anmobj.h"
#include "nw4r/g3d/res/g3d_resanmtexsrt.h"
#include "nw4r/g3d/res/g3d_resmat.h"

namespace nw4r {
namespace g3d {

void ApplyTexSrtAnmResult(ResTexSrt srt, const TexSrtAnmResult *pResult);
void ApplyTexSrtAnmResult(ResTexSrt srt, ResMatIndMtxAndScale ind, const TexSrtAnmResult *pResult);

/******************************************************************************
 *
 * AnmObjTexSrt
 *
 ******************************************************************************/
// Forward declarations
class AnmObjTexSrtRes;

class AnmObjTexSrt : public AnmObj {
public:
    AnmObjTexSrt(MEMAllocator *pAllocator, u16 *pBindingBuf, int numBinding);
    virtual void G3dProc(u32 task, u32 param, void *pInfo) = 0; // at 0xC
    virtual ~AnmObjTexSrt() {}                                  // at 0x10

    virtual void SetFrame(f32 frame) = 0; // at 0x1C
    virtual f32 GetFrame() const = 0;     // at 0x20
    virtual void UpdateFrame() = 0;       // at 0x24

    virtual void SetUpdateRate(f32 rate) = 0; // at 0x28
    virtual f32 GetUpdateRate() const = 0;    // at 0x2C

    virtual bool Bind(const ResMdl mdl) = 0; // at 0x30
    virtual void Release();                  // at 0x34

    virtual const TexSrtAnmResult *GetResult(TexSrtAnmResult *pResult,
                                             u32 idx) = 0; // at 0x38

    virtual AnmObjTexSrtRes *Attach(int idx, AnmObjTexSrtRes *pRes); // at 0x3C
    virtual AnmObjTexSrtRes *Detach(int idx);                        // at 0x40
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

    NW4R_G3D_RTTI_DECL_DERIVED(AnmObjTexSrt, AnmObj);
};

/******************************************************************************
 *
 * AnmObjTexSrtNode
 *
 ******************************************************************************/
class AnmObjTexSrtNode : public AnmObjTexSrt {
public:
    AnmObjTexSrtNode(
        MEMAllocator *pAllocator, u16 *pBindingBuf, int numBinding, AnmObjTexSrtRes **ppChildrenBuf, int numChildren
    );
    virtual void G3dProc(u32 task, u32 param, void *pInfo); // at 0xC
    virtual ~AnmObjTexSrtNode();                            // at 0x10

    virtual void SetFrame(f32 frame); // at 0x1C
    virtual f32 GetFrame() const;     // at 0x20
    virtual void UpdateFrame();       // at 0x24

    virtual void SetUpdateRate(f32 rate); // at 0x28
    virtual f32 GetUpdateRate() const;    // at 0x2C

    virtual bool Bind(const ResMdl mdl); // at 0x30
    virtual void Release();              // at 0x34

    virtual AnmObjTexSrtRes *Attach(int idx, AnmObjTexSrtRes *pRes); // at 0x3C
    virtual AnmObjTexSrtRes *Detach(int idx);                        // at 0x40
    virtual void DetachAll();                                        // at 0x44

    int Size() const {
        return mChildrenArraySize;
    }

protected:
    int mChildrenArraySize;            // at 0x18
    AnmObjTexSrtRes **mpChildrenArray; // at 0x1C

    NW4R_G3D_RTTI_DECL_DERIVED(AnmObjTexSrtNode, AnmObjTexSrt);
};

/******************************************************************************
 *
 * AnmObjTexSrtOverride
 *
 ******************************************************************************/
class AnmObjTexSrtOverride : public AnmObjTexSrtNode {
public:
    static AnmObjTexSrtOverride *Construct(MEMAllocator *pAllocator, u32 *pSize, ResMdl mdl, int numChildren);

    AnmObjTexSrtOverride(
        MEMAllocator *pAllocator, u16 *pBindingBuf, int numBinding, AnmObjTexSrtRes **ppChildrenBuf, int numChildren
    )
        : AnmObjTexSrtNode(pAllocator, pBindingBuf, numBinding, ppChildrenBuf, numChildren) {}

    virtual ~AnmObjTexSrtOverride() {} // at 0x10

    virtual const TexSrtAnmResult *GetResult(TexSrtAnmResult *pResult,
                                             u32 idx); // at 0x38

    NW4R_G3D_RTTI_DECL_DERIVED(AnmObjTexSrtOverride, AnmObjTexSrtNode);
};

/******************************************************************************
 *
 * AnmObjTexSrtRes
 *
 ******************************************************************************/
class AnmObjTexSrtRes : public AnmObjTexSrt, protected FrameCtrl {
public:
    static AnmObjTexSrtRes *Construct(MEMAllocator *pAllocator, u32 *pSize, ResAnmTexSrt srt, ResMdl mdl, bool cache);

    AnmObjTexSrtRes(
        MEMAllocator *pAllocator, ResAnmTexSrt srt, u16 *pBindingBuf, int numBinding, TexSrtAnmResult *pCacheBuf
    );
    virtual void G3dProc(u32 task, u32 param, void *pInfo); // at 0xC
    virtual ~AnmObjTexSrtRes() {}                           // at 0x10

    virtual void SetFrame(f32 frame); // at 0x1C
    virtual f32 GetFrame() const;     // at 0x20
    virtual void UpdateFrame();       // at 0x24

    virtual void SetUpdateRate(f32 rate); // at 0x28
    virtual f32 GetUpdateRate() const;    // at 0x2C

    virtual bool Bind(const ResMdl mdl); // at 0x30

    virtual const TexSrtAnmResult *GetResult(TexSrtAnmResult *pResult,
                                             u32 idx); // at 0x38

    void UpdateCache();

    ResAnmTexSrt GetResAnm() {
        return mRes;
    }

    void SetPlayPolicy(PlayPolicyFunc pFunc) {
        FrameCtrl::SetPlayPolicy(pFunc);
    }

private:
    ResAnmTexSrt mRes;                    // at 0x2C
    TexSrtAnmResult *const mpResultCache; // at 0x30

    NW4R_G3D_RTTI_DECL_DERIVED(AnmObjTexSrtRes, AnmObjTexSrt);
};

} // namespace g3d
} // namespace nw4r

#endif
