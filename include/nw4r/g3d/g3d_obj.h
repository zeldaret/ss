#ifndef NW4R_G3D_G3DOBJ_H
#define NW4R_G3D_G3DOBJ_H
#include "common.h"
#include "rvl/MEM/mem_allocator.h"

#define NW4R_G3D_TYPE_OBJ_DECL(VAL) static const nw4r::g3d::G3dObj::ResNameDataT<sizeof(#VAL)> TYPE_NAME
#define NW4R_G3D_TYPE_OBJ_DEF(VAL)                                                                                     \
    const nw4r::g3d::G3dObj::ResNameDataT<sizeof(#VAL)> VAL::TYPE_NAME = {sizeof(#VAL), #VAL}

namespace nw4r {
namespace g3d {
namespace detail {
inline void *AllocFromAllocator(MEMAllocator *pAllocator, u32 size) {
    return MEMAllocFromAllocator(pAllocator, size);
}

inline void FreeToAllocator(MEMAllocator *pAllocator, void *pBlock) {
    return MEMFreeToAllocator(pAllocator, pBlock);
}
} // namespace detail

class G3dObj {
public:
    enum G3dProcTask {
        G3DPROC_CALC_WORLD = 0x1,
        G3DPROC_CALC_MAT = 0x2,
        G3DPROC_CALC_VTX = 0x3,
        G3DPROC_CALC_VIEW = 0x4,
        G3DPROC_GATHER_SCNOBJ = 0x5,
        G3DPROC_DRAW_OPA = 0x6,
        G3DPROC_DRAW_XLU = 0x7,
        G3DPROC_UPDATEFRAME = 0x8,

        G3DPROC_CHILD_DETACHED = 0x10001,
        G3DPROC_ATTACH_PARENT = 0x10002,
        G3DPROC_DETACH_PARENT = 0x10003,
        G3DPROC_0x10004 = 0x10004
    };

    template <u32 N>
    struct ResNameDataT {
        u32 mLength;
        // Bug? +1 seemingly for null terminator,
        // but size N already includes it
        char mName[N + 1 + 3 & ~3];
    };

    struct TypeObj {
        struct TypeObjData {
            u32 mLength;  // at 0x0
            char mName[]; // at 0x4
        };

        template <u32 N>
        TypeObj(const ResNameDataT<N> &pRes) : mData((const TypeObjData *)&pRes) {}

        u32 GetTypeID() const {
            return (u32)mData;
        }

        const char *GetTypeName() const {
            return mData->mName;
        }

        bool operator==(const TypeObj &rhs) const {
            return GetTypeID() == rhs.GetTypeID();
        }

        const TypeObjData *mData; // at 0x0
    };

public:
    virtual bool IsDerivedFrom(TypeObj other) const // at 0x8
    {
        return other == GetTypeObjStatic();
    }
    virtual void G3dProc(u32, u32, void *) = 0; // at 0xC
    virtual ~G3dObj();                          // at 0x10
    virtual const TypeObj GetTypeObj() const    // at 0x14
    {
        return TypeObj(TYPE_NAME);
    }
    virtual const char *GetTypeName() const // at 0x18
    {
        return GetTypeObj().GetTypeName();
    }

    static const G3dObj::TypeObj GetTypeObjStatic() {
        return TypeObj(TYPE_NAME);
    }

    void Destroy();
    void DetachFromParent();

    G3dObj(MEMAllocator *pAllocator, G3dObj *pParent) : mAllocator(pAllocator), mParent(pParent) {}

    G3dObj *GetParent() const {
        return mParent;
    }
    void SetParent(G3dObj *parent) {
        mParent = parent;
    }

    static void *Alloc(MEMAllocator *pAllocator, u32 size) {
        return detail::AllocFromAllocator(pAllocator, size);
    }

    static void Dealloc(MEMAllocator *pAllocator, void *pBlock) {
        detail::FreeToAllocator(pAllocator, pBlock);
    }

    static inline void *operator new(size_t size, void *pBlock) {
        return pBlock;
    }
    static inline void operator delete(void *pBlock) {}

    template <typename T>
    static T *DynamicCast(G3dObj *obj) {
        if (obj != nullptr && obj->IsDerivedFrom(T::GetTypeObjStatic())) {
            return static_cast<T *>(obj);
        }
        return nullptr;
    }

private:
    G3dObj *mParent;          // at 0x4
    MEMAllocator *mAllocator; // at 0x8

    NW4R_G3D_TYPE_OBJ_DECL(G3dObj);
};
} // namespace g3d
} // namespace nw4r

#endif
