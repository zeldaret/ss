#ifndef NW4R_G3D_ANM_CLR_H
#define NW4R_G3D_ANM_CLR_H
#include <nw4r/types_nw4r.h>

#include "nw4r/g3d/g3d_anmobj.h"
#include "nw4r/g3d/res/g3d_resanmclr.h"
#include "nw4r/g3d/res/g3d_resmat.h"

namespace nw4r {
namespace g3d {

void ApplyClrAnmResult(ResMatChan chan, ResMatTevColor tev, const ClrAnmResult *pResult);

/******************************************************************************
 *
 * AnmObjMatClr
 *
 ******************************************************************************/
// Forward declarations
class AnmObjMatClrRes;

class AnmObjMatClr : public AnmObj {
public:
    AnmObjMatClr(MEMAllocator *pAllocator, u16 *pBindingBuf, int numBinding);
    virtual void G3dProc(u32 task, u32 param, void *pInfo) = 0; // at 0xC
    virtual ~AnmObjMatClr() {}                                  // at 0x10

    virtual void SetFrame(f32 frame) = 0; // at 0x1C
    virtual f32 GetFrame() const = 0;     // at 0x20
    virtual void UpdateFrame() = 0;       // at 0x24

    virtual void SetUpdateRate(f32 rate) = 0; // at 0x28
    virtual f32 GetUpdateRate() const = 0;    // at 0x2C

    virtual bool Bind(const ResMdl mdl) = 0; // at 0x30
    virtual void Release();                  // at 0x34

    virtual const ClrAnmResult *GetResult(ClrAnmResult *pResult,
                                          u32 idx) = 0; // at 0x38

    virtual AnmObjMatClrRes *Attach(int idx, AnmObjMatClrRes *pRes); // at 0x3C
    virtual AnmObjMatClrRes *Detach(int idx);                        // at 0x40
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
    static const int DEFAULT_MAX_CHILDREN = 4;

protected:
    int mNumBinding;      // at 0x10
    u16 *const mpBinding; // at 0x14

    NW4R_G3D_RTTI_DECL_DERIVED(AnmObjMatClr, AnmObj);
};

/******************************************************************************
 *
 * AnmObjMatClrNode
 *
 ******************************************************************************/
class AnmObjMatClrNode : public AnmObjMatClr {
public:
    AnmObjMatClrNode(
        MEMAllocator *pAllocator, u16 *pBindingBuf, int numBinding, AnmObjMatClrRes **ppChildrenBuf, int numChildren
    );
    virtual void G3dProc(u32 task, u32 param, void *pInfo); // at 0xC
    virtual ~AnmObjMatClrNode();                            // at 0x10

    virtual void SetFrame(f32 frame); // at 0x1C
    virtual f32 GetFrame() const;     // at 0x20
    virtual void UpdateFrame();       // at 0x24

    virtual void SetUpdateRate(f32 rate); // at 0x28
    virtual f32 GetUpdateRate() const;    // at 0x2C

    virtual bool Bind(const ResMdl mdl); // at 0x30
    virtual void Release();              // at 0x34

    virtual AnmObjMatClrRes *Attach(int idx, AnmObjMatClrRes *pRes); // at 0x3C
    virtual AnmObjMatClrRes *Detach(int idx);                        // at 0x40
    virtual void DetachAll();                                        // at 0x44

    int Size() const {
        return mChildrenArraySize;
    }

protected:
    int mChildrenArraySize;            // at 0x18
    AnmObjMatClrRes **mpChildrenArray; // at 0x1C

    NW4R_G3D_RTTI_DECL_DERIVED(AnmObjMatClrNode, AnmObjMatClr);
};

/******************************************************************************
 *
 * AnmObjMatClrOverride
 *
 ******************************************************************************/
class AnmObjMatClrOverride : public AnmObjMatClrNode {
public:
    static AnmObjMatClrOverride *Construct(MEMAllocator *pAllocator, u32 *pSize, ResMdl mdl, int numChildren);

    AnmObjMatClrOverride(
        MEMAllocator *pAllocator, u16 *pBindingBuf, int numBinding, AnmObjMatClrRes **ppChildrenBuf, int numChildren
    )
        : AnmObjMatClrNode(pAllocator, pBindingBuf, numBinding, ppChildrenBuf, numChildren) {}

    virtual ~AnmObjMatClrOverride() {} // at 0x10

    virtual const ClrAnmResult *GetResult(ClrAnmResult *pResult,
                                          u32 idx); // at 0x38

    NW4R_G3D_RTTI_DECL_DERIVED(AnmObjMatClrOverride, AnmObjMatClrNode);
};

/******************************************************************************
 *
 * AnmObjMatClrRes
 *
 ******************************************************************************/
class AnmObjMatClrRes : public AnmObjMatClr, protected FrameCtrl {
public:
    static AnmObjMatClrRes *Construct(MEMAllocator *pAllocator, u32 *pSize, ResAnmClr clr, ResMdl mdl, bool cache);

    AnmObjMatClrRes(MEMAllocator *pAllocator, ResAnmClr clr, u16 *pBindingBuf, int numBinding, ClrAnmResult *pCacheBuf);
    virtual void G3dProc(u32 task, u32 param, void *pInfo); // at 0xC
    virtual ~AnmObjMatClrRes() {}                           // at 0x10

    virtual void SetFrame(f32 frame); // at 0x1C
    virtual f32 GetFrame() const;     // at 0x20
    virtual void UpdateFrame();       // at 0x24

    virtual void SetUpdateRate(f32 rate); // at 0x28
    virtual f32 GetUpdateRate() const;    // at 0x2C

    virtual bool Bind(const ResMdl mdl); // at 0x30

    virtual const ClrAnmResult *GetResult(ClrAnmResult *pResult,
                                          u32 idx); // at 0x38

    void UpdateCache();

    ResAnmClr GetResAnm() {
        return mRes;
    }

    void SetPlayPolicy(PlayPolicyFunc pFunc) {
        FrameCtrl::SetPlayPolicy(pFunc);
    }

private:
    ResAnmClr mRes;                    // at 0x2C
    ClrAnmResult *const mpResultCache; // at 0x30

    NW4R_G3D_RTTI_DECL_DERIVED(AnmObjMatClrRes, AnmObjMatClr);
};

} // namespace g3d
} // namespace nw4r

#endif
