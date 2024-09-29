#ifndef NW4R_LYT_LAYOUT_H
#define NW4R_LYT_LAYOUT_H

#include "common.h"
#include <new.h>
#include <nw4r/lyt/lyt_animation.h>
#include <nw4r/lyt/lyt_material.h>
#include <nw4r/lyt/lyt_types.h>
#include <nw4r/ut/ut_ResFont.h>
#include <rvl/MEM/mem_allocator.h>

namespace nw4r {
namespace lyt {

namespace res {
struct Layout {
    DataBlockHeader blockheader; // at 0x00
    u8 originType;               // at 0x08
    u8 padding[3];               // at 0x09
    Size layoutSize;             // at 0x0C
};

} // namespace res

class Layout {
public:
    ut::Rect GetLayoutRect() const;
    static Pane *BuildPaneObj(s32 kind, const void *dataPtr, const ResBlockSet &ResBlockSet);

    Layout();
    virtual ~Layout();                                                                                   // at 0x08
    virtual bool Build(const void *lytResBuf, ResourceAccessor *pResAcsr);                               // at 0x0C
    virtual AnimTransform *CreateAnimTransform();                                                        // at 0x10
    virtual AnimTransform *CreateAnimTransform(const void *animResBuf, ResourceAccessor *pResAcsr);      // at 0x14
    virtual AnimTransform *CreateAnimTransform(const AnimResource &animRes, ResourceAccessor *pResAcsr); // at 0x18
    virtual void BindAnimation(AnimTransform *pAnimTrans);                                               // at 0x1C
    virtual void UnbindAnimation(AnimTransform *pAnimTrans);                                             // at 0x20
    virtual void UnbindAllAnimation();                                                                   // at 0x24
    virtual bool BindAnimationAuto(const AnimResource &animRes, ResourceAccessor *pResAcsr);             // at 0x28
    virtual void SetAnimationEnable(AnimTransform *pAnimTrans, bool bEnable);                            // at 0x2C
    virtual void CalculateMtx(const DrawInfo &drawInfo);                                                 // at 0x30
    virtual void Draw(const DrawInfo &drawInfo);                                                         // at 0x34
    virtual void Animate(u32 option);                                                                    // at 0x38
    virtual void SetTagProcessor(ut::TagProcessorBase<wchar_t> *pTagProcessor);                          // at 0x3C

    ut::LinkList<AnimTransform, 4> &GetAnimTransformList() {
        return mAnimTransList;
    }

    Pane *GetRootPane() const {
        return mpRootPane;
    }

    void SetRootPane(Pane* root) {
        mpRootPane = root;
    }

    GroupContainer *GetGroupContainer() const {
        return mpGroupContainer;
    }

    void SetGroupContainer(GroupContainer *container) {
        mpGroupContainer = container;
    }

private:
    ut::LinkList<AnimTransform, 4> mAnimTransList; // at 0x04
    Pane *mpRootPane;                              // at 0x10
    GroupContainer *mpGroupContainer;              // at 0x14
    Size mLayoutSize;                              // at 0x18

public:
    // STATICS
    static void FreeMemory(void *p) {
        MEMFreeToAllocator(mspAllocator, p);
    }

    static void *AllocMemory(size_t n) {
        return MEMAllocFromAllocator(mspAllocator, n);
    }

    template <typename T>
    static void DeleteArray(T *p, size_t n) {
        if (p) {
            for (size_t i = 0; i < n; i++) {
                p[i].~T();
            }

            FreeMemory(p);
        }
    }

    template <typename T>
    static T *NewArray(size_t n) {
        T *array = (T *)AllocMemory(n * sizeof(T));
        if (!array) {
            return nullptr;
        }

        for (size_t i = 0; i < n; i++) {
            new (&array[i]) T();
        }

        return array;
    }

    template <typename T>
    static void DeleteObj(T *t) {
        if (t) {
            t->~T();
            FreeMemory(t);
        }
    }

    template <typename T>
    static void DeletePrimArray(T *objAry) {
        if (objAry) {
            FreeMemory(objAry);
        }
    }

    template <typename T>
    static T *NewObj() {
        T *pMem = (T *)AllocMemory(sizeof(T));
        if (pMem) {
            return new (pMem) T();
        } else {
            return nullptr;
        }
    }

    template <typename T, typename P1>
    static T *NewObj(P1 param1) {
        T *pMem = (T *)AllocMemory(sizeof(T));
        if (pMem) {
            return new (pMem) T(param1);
        } else {
            return nullptr;
        }
    }

    template <typename T, typename P1, typename P2>
    static T *NewObj(P1 param1, P2 param2) {
        T *pMem = (T *)AllocMemory(sizeof(T));
        if (pMem) {
            return new (pMem) T(param1, param2);
        } else {
            return nullptr;
        }
    }

    static MEMAllocator *mspAllocator;
};
} // namespace lyt
} // namespace nw4r

#endif
