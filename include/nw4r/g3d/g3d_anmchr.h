#ifndef NW4R_G3D_ANM_CHR_H
#define NW4R_G3D_ANM_CHR_H
#include <nw4r/types_nw4r.h>

#include <nw4r/g3d/g3d_anmobj.h>
#include <nw4r/g3d/res/g3d_resanmchr.h>

namespace nw4r {
namespace g3d {

/******************************************************************************
 *
 * AnmObjChr
 *
 ******************************************************************************/
// Forward declarations
class AnmObjChrRes;

class AnmObjChr : public AnmObj {
public:
    enum BindOption {
        BIND_ONE,
        BIND_PARTIAL,
        NUM_OF_BIND_OPTION
    };

public:
    AnmObjChr(MEMAllocator *pAllocator, u16 *pBindingBuf, int numBinding);
    virtual void G3dProc(u32 task, u32 param, void *pInfo) = 0; // at 0xC
    virtual ~AnmObjChr() {}                                     // at 0x10

    virtual void SetFrame(f32 frame) = 0; // at 0x1C
    virtual f32 GetFrame() const = 0;     // at 0x20
    virtual void UpdateFrame() = 0;       // at 0x24

    virtual void SetUpdateRate(f32 rate) = 0; // at 0x28
    virtual f32 GetUpdateRate() const = 0;    // at 0x2C

    virtual bool Bind(const ResMdl mdl) = 0; // at 0x30
    virtual void Release();                  // at 0x34

    virtual const ChrAnmResult *GetResult(ChrAnmResult *pResult,
                                          u32 idx) = 0; // at 0x38

    virtual AnmObjChrRes *Attach(int idx, AnmObjChrRes *pRes); // at 0x3C
    virtual AnmObjChrRes *Detach(int idx);                     // at 0x40
    virtual void DetachAll();                                  // at 0x44

    virtual void SetWeight(int idx, f32 weight); // at 0x48
    virtual f32 GetWeight(int idx) const;        // at 0x4C

    virtual bool Bind(const ResMdl mdl, u32 target,
                      BindOption option) = 0; // at 0x50
    virtual void Release(const ResMdl mdl, u32 target,
                         BindOption option) = 0; // at 0x54

    bool TestExistence(u32 idx) const;
    bool TestDefined(u32 idx) const;

    void UseQuaternionBlend(bool enable) {
        SetAnmFlag(FLAG_USE_QUATERNION_ROTATION_BLEND, enable);
    }

    void UseAccurateScaleBlend(bool enable) {
        SetAnmFlag(FLAG_USE_ACCURATE_SCALE_BLEND, enable);
    }

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

    NW4R_G3D_RTTI_DECL_DERIVED(AnmObjChr, AnmObj);
};

/******************************************************************************
 *
 * AnmObjChrNode
 *
 ******************************************************************************/
class AnmObjChrNode : public AnmObjChr {
public:
    AnmObjChrNode(
        MEMAllocator *pAllocator, u16 *pBindingBuf, int numBinding, AnmObjChrRes **ppChildrenBuf, int numChildren
    );
    virtual void G3dProc(u32 task, u32 param, void *pInfo); // at 0xC
    virtual ~AnmObjChrNode();                               // at 0x10

    virtual void SetFrame(f32 frame); // at 0x1C
    virtual f32 GetFrame() const;     // at 0x20
    virtual void UpdateFrame();       // at 0x24

    virtual void SetUpdateRate(f32 rate); // at 0x28
    virtual f32 GetUpdateRate() const;    // at 0x2C

    virtual bool Bind(const ResMdl mdl); // at 0x30
    virtual void Release();              // at 0x34

    virtual AnmObjChrRes *Attach(int idx, AnmObjChrRes *pRes); // at 0x3C
    virtual AnmObjChrRes *Detach(int idx);                     // at 0x40
    virtual void DetachAll();                                  // at 0x44

    virtual bool Bind(const ResMdl mdl, u32 target,
                      BindOption option); // at 0x50
    virtual void Release(const ResMdl mdl, u32 target,
                         BindOption option); // at 0x54

    inline int Size() {
        return mChildrenArraySize;
    }

    AnmObjChrRes *GetNode(int i) {
        return mpChildrenArray[i];
    }

protected:
    int mChildrenArraySize;         // at 0x18
    AnmObjChrRes **mpChildrenArray; // at 0x1C

    NW4R_G3D_RTTI_DECL_DERIVED(AnmObjChrNode, AnmObjChr);
};

/******************************************************************************
 *
 * AnmObjChrBlend
 *
 ******************************************************************************/
class AnmObjChrBlend : public AnmObjChrNode {
public:
    static AnmObjChrBlend *Construct(MEMAllocator *pAllocator, u32 *pSize, ResMdl mdl, int numChildren);

    AnmObjChrBlend(
        MEMAllocator *pAllocator, u16 *pBindingBuf, int numBinding, AnmObjChrRes **ppChildrenBuf, int numChildren,
        f32 *pWeightBuf
    );

    virtual ~AnmObjChrBlend() {} // at 0x10

    virtual const ChrAnmResult *GetResult(ChrAnmResult *pResult,
                                          u32 idx); // at 0x38

    virtual void SetWeight(int idx, f32 weight); // at 0x48
    virtual f32 GetWeight(int idx) const;        // at 0x4C

private:
    f32 *mpWeightArray; // at 0x20

    NW4R_G3D_RTTI_DECL_DERIVED(AnmObjChrBlend, AnmObjChrNode);
};

/******************************************************************************
 *
 * AnmObjChrRes
 *
 ******************************************************************************/
class AnmObjChrRes : public AnmObjChr, protected FrameCtrl {
public:
    static AnmObjChrRes *Construct(MEMAllocator *pAllocator, u32 *pSize, ResAnmChr chr, ResMdl mdl, bool cache);

    AnmObjChrRes(MEMAllocator *pAllocator, ResAnmChr chr, u16 *pBindingBuf, int numBinding, ChrAnmResult *pCacheBuf);
    virtual void G3dProc(u32 task, u32 param, void *pInfo); // at 0xC
    virtual ~AnmObjChrRes() {}                              // at 0x10

    virtual void SetFrame(f32 frame); // at 0x1C
    virtual f32 GetFrame() const;     // at 0x20
    virtual void UpdateFrame();       // at 0x24

    virtual void SetUpdateRate(f32 rate); // at 0x28
    virtual f32 GetUpdateRate() const;    // at 0x2C

    virtual bool Bind(const ResMdl mdl); // at 0x30
    using AnmObjChr::Release;            // at 0x40

    virtual const ChrAnmResult *GetResult(ChrAnmResult *pResult,
                                          u32 idx); // at 0x38

    virtual bool Bind(const ResMdl mdl, u32 target,
                      BindOption option); // at 0x50
    virtual void Release(const ResMdl mdl, u32 target,
                         BindOption option); // at 0x54

    void UpdateCache();

    ResAnmChr GetResAnm() {
        return mRes;
    }

    void SetPlayPolicy(PlayPolicyFunc pFunc) {
        FrameCtrl::SetPlayPolicy(pFunc);
    }

private:
    ResAnmChr mRes;                    // at 0x2C
    ChrAnmResult *const mpResultCache; // at 0x30

    NW4R_G3D_RTTI_DECL_DERIVED(AnmObjChrRes, AnmObjChr);
};

} // namespace g3d
} // namespace nw4r

#endif
