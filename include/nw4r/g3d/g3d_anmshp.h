#ifndef NW4R_G3D_ANM_SHP_H
#define NW4R_G3D_ANM_SHP_H
#include <nw4r/types_nw4r.h>

#include <nw4r/g3d/g3d_anmobj.h>
#include <nw4r/g3d/res/g3d_resanmshp.h>

namespace nw4r {
namespace g3d {

/******************************************************************************
 *
 * AnmObjShp
 *
 ******************************************************************************/
// Forward declarations
class AnmObjShpRes;

class AnmObjShp : public AnmObj {
public:
    AnmObjShp(MEMAllocator *pAllocator, u16 *pBindingBuf, int numBinding);
    virtual void G3dProc(u32 task, u32 param, void *pInfo) = 0; // at 0xC
    virtual ~AnmObjShp() {}                                     // at 0x10

    virtual void SetFrame(f32 frame) = 0; // at 0x1C
    virtual f32 GetFrame() const = 0;     // at 0x20
    virtual void UpdateFrame() = 0;       // at 0x24

    virtual void SetUpdateRate(f32 rate) = 0; // at 0x28
    virtual f32 GetUpdateRate() const = 0;    // at 0x2C

    virtual bool Bind(const ResMdl mdl) = 0; // at 0x30
    virtual void Release();                  // at 0x34

    virtual const ShpAnmResult *GetResult(ShpAnmResult *pResult,
                                          u32 idx) = 0; // at 0x38

    virtual AnmObjShpRes *Attach(int idx, AnmObjShpRes *pRes); // at 0x3C
    virtual AnmObjShpRes *Detach(int idx);                     // at 0x40
    virtual void DetachAll();                                  // at 0x44

    virtual void SetWeight(int idx, f32 weight); // at 0x48
    virtual f32 GetWeight(int idx) const;        // at 0x4C

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

    NW4R_G3D_RTTI_DECL_DERIVED(AnmObjShp, AnmObj);
};

/******************************************************************************
 *
 * AnmObjShpNode
 *
 ******************************************************************************/
class AnmObjShpNode : public AnmObjShp {
public:
    AnmObjShpNode(
        MEMAllocator *pAllocator, u16 *pBindingBuf, int numBinding, AnmObjShpRes **ppChildrenBuf, int numChildren
    );
    virtual void G3dProc(u32 task, u32 param, void *pInfo); // at 0xC
    virtual ~AnmObjShpNode();                               // at 0x10

    virtual void SetFrame(f32 frame); // at 0x1C
    virtual f32 GetFrame() const;     // at 0x20
    virtual void UpdateFrame();       // at 0x24

    virtual void SetUpdateRate(f32 rate); // at 0x28
    virtual f32 GetUpdateRate() const;    // at 0x2C

    virtual bool Bind(const ResMdl mdl); // at 0x30
    virtual void Release();              // at 0x34

    virtual AnmObjShpRes *Attach(int idx, AnmObjShpRes *pRes); // at 0x3C
    virtual AnmObjShpRes *Detach(int idx);                     // at 0x40
    virtual void DetachAll();                                  // at 0x44

protected:
    int mChildrenArraySize;         // at 0x18
    AnmObjShpRes **mpChildrenArray; // at 0x1C

    NW4R_G3D_RTTI_DECL_DERIVED(AnmObjShpNode, AnmObjShp);
};

/******************************************************************************
 *
 * AnmObjShpBlend
 *
 ******************************************************************************/
class AnmObjShpBlend : public AnmObjShpNode {
    static AnmObjShpBlend *Construct(MEMAllocator *pAllocator, u32 *pSize, ResMdl mdl, int numChildren);

    AnmObjShpBlend(
        MEMAllocator *pAllocator, u16 *pBindingBuf, int numBinding, AnmObjShpRes **ppChildrenBuf, int numChildren,
        f32 *pWeightBuf
    );

    virtual ~AnmObjShpBlend() {} // at 0x10

    virtual const ShpAnmResult *GetResult(ShpAnmResult *pResult,
                                          u32 idx); // at 0x38

    virtual void SetWeight(int idx, f32 weight); // at 0x48
    virtual f32 GetWeight(int idx) const;        // at 0x4C

private:
    f32 *mpWeightArray; // at 0x20

    NW4R_G3D_RTTI_DECL_DERIVED(AnmObjShpBlend, AnmObjShpNode);
};

/******************************************************************************
 *
 * AnmObjShpRes
 *
 ******************************************************************************/
class AnmObjShpRes : public AnmObjShp, protected FrameCtrl {
public:
    static AnmObjShpRes *Construct(MEMAllocator *pAllocator, u32 *pSize, ResAnmShp shp, ResMdl mdl, bool cache);

    AnmObjShpRes(
        MEMAllocator *pAllocator, ResAnmShp shp, u16 *pBindingBuf, ShpAnmVtxSet *pVtxSetBuf, int numBinding,
        ShpAnmResult *pCacheBuf
    );
    virtual void G3dProc(u32 task, u32 param, void *pInfo); // at 0xC
    virtual ~AnmObjShpRes() {}                              // at 0x10

    virtual void SetFrame(f32 frame); // at 0x1C
    virtual f32 GetFrame() const;     // at 0x20
    virtual void UpdateFrame();       // at 0x24

    virtual void SetUpdateRate(f32 rate); // at 0x28
    virtual f32 GetUpdateRate() const;    // at 0x2C

    virtual bool Bind(const ResMdl mdl); // at 0x30

    virtual const ShpAnmResult *GetResult(ShpAnmResult *pResult,
                                          u32 idx); // at 0x38

    void UpdateCache();

    ResAnmShp GetResAnm() {
        return mRes;
    }

    void SetPlayPolicy(PlayPolicyFunc pFunc) {
        FrameCtrl::SetPlayPolicy(pFunc);
    }

private:
    ResAnmShp mRes;                    // at 0x2C
    ShpAnmVtxSet *const mpVtxSetArray; // at 0x30
    ShpAnmResult *const mpResultCache; // at 0x34

    NW4R_G3D_RTTI_DECL_DERIVED(AnmObjShpRes, AnmObjShp);
};

} // namespace g3d
} // namespace nw4r

#endif
