#include <egg/core/eggAssertHeap.h>
#include <egg/core/eggExpHeap.h>
#include <egg/core/eggFrmHeap.h>
#include <m/m_heap.h>

namespace mHeap {

u8 g_DefaultGameHeapId = 1;

#define MIN_ALIGN 0x20

EGG::ExpHeap *g_gameHeaps[4];
EGG::ExpHeap *s_SavedCurrentHeap;
EGG::ExpHeap *g_archiveHeap;
EGG::ExpHeap *g_commandHeap;
EGG::ExpHeap *g_dylinkHeap;
EGG::AssertHeap *g_assertHeap;

u16 copyAttribute(u32 arg) {
    u16 result = 0;

    if ((arg & 1) != 0) {
        result = 1;
    }
    if ((arg & 2) != 0) {
        result |= 2;
    }
    if ((arg & 4) != 0) {
        result |= 4;
    }
    return result;
}

EGG::Heap *setCurrentHeap(EGG::Heap *heap) {
    return heap->becomeCurrentHeap();
}
EGG::ExpHeap *createExpHeap(size_t size, EGG::Heap *parent, const char *name, unsigned long align,
        unsigned long attrs) {
    if (parent == nullptr) {
        parent = EGG::Heap::sCurrentHeap;
    }

    if (align < MIN_ALIGN) {
        align = MIN_ALIGN;
    }

    if (size != 0xFFFFFFFF) {
        size = expHeapCost(size, align);
    } else {
        size = parent->getAllocatableSize(align);
    }

    void *mem = parent->alloc(size, align);
    EGG::ExpHeap *heap = nullptr;
    if (mem != nullptr) {
        heap = EGG::ExpHeap::create(mem, size, copyAttribute(attrs));
        if (heap == nullptr) {
            parent->free(mem);
        } else if (name != nullptr) {
            heap->mName = name;
        }
    }
    return heap;
}

size_t adjustExpHeap(EGG::ExpHeap *heap) {
    int ret = 0;
    if (heap != nullptr) {
        size_t ad = heap->adjust();
        size_t cost = mHeap::expHeapCost(0, 4);
        if (ad >= cost) {
            ret = ad - cost;
        }
    }
    return ret;
}
size_t expHeapCost(size_t size, s32 align) {
    int a = align - 1;
    long b = (0x84 + a);
    
    return size + (~(a) & b);
}

EGG::FrmHeap *createFrmHeap(size_t size, EGG::Heap *parent, const char *name, size_t align, size_t attrs) {
    if (parent == nullptr) {
        parent = EGG::Heap::sCurrentHeap;
    }

    if (align < MIN_ALIGN) {
        align = MIN_ALIGN;
    }

    if (size != 0xFFFFFFFF) {
        size = frmHeapCost(size, align);
    } else {
        size = parent->getAllocatableSize(align);
    }

    void *mem = parent->alloc(size, align);
    EGG::FrmHeap *heap = nullptr;
    if (mem != nullptr) {
        heap = EGG::FrmHeap::create(mem, size, copyAttribute(attrs));
        if (heap == nullptr) {
            parent->free(mem);
        } else if (name != nullptr) {
            heap->mName = name;
        }
    }
    return heap;
}
void destroyFrmHeap(EGG::FrmHeap *heap) {
    if (heap != nullptr) {
        heap->destroy();
    }
}

size_t adjustFrmHeap(EGG::FrmHeap *heap) {
    int ret = 0;
    if (heap != nullptr) {
        size_t ad = heap->adjust();
        size_t cost = mHeap::frmHeapCost(0, 4);
        if (ad >= cost) {
            ret = ad - cost;
        }
    }
    return ret;
}

size_t frmHeapCost(size_t size, s32 align) {
    int a = align - 1;
    long b = (0x7C + a);
    
    return size + (~(a) & b);
}

EGG::Heap **setTempHeap(EGG::Heap **prevHeap, EGG::Heap *tempNewHeap) {
    *prevHeap = tempNewHeap->becomeCurrentHeap();
    return prevHeap;
}
EGG::Heap **restoreTempHeap(EGG::Heap **prevHeap, s32 size) {
    if (prevHeap != nullptr) {
        (*prevHeap)->becomeCurrentHeap();
        if (size > 0) {
            delete prevHeap;
        }
    }
    return prevHeap;
}

EGG::ExpHeap *createHeap(size_t size, EGG::Heap *block, const char *name) {
    EGG::ExpHeap *heap = EGG::ExpHeap::create(size, block, 4);
    if (heap != nullptr) {
        heap->setGroupID(0);
        if (name != nullptr) {
            heap->mName = name;
        }
    } else {
        block->dump();
    }

    return heap;
}
void saveCurrentHeap() {
    s_SavedCurrentHeap = (EGG::ExpHeap *)EGG::Heap::sCurrentHeap;
}
void restoreCurrentHeap() {
    s_SavedCurrentHeap->becomeCurrentHeap();
    s_SavedCurrentHeap = nullptr;
}

EGG::FrmHeap *makeFrmHeapAndUpdate(size_t size, EGG::Heap *parentHeap, const char *name, s32 align, u32 unk) {
    EGG::FrmHeap *heap = createFrmHeap(size, parentHeap, name, align, unk);
    if (heap != nullptr) {
        EGG::Heap *heap2 = setCurrentHeap(heap);
        s_SavedCurrentHeap = (EGG::ExpHeap *)heap2;
    }
    return heap;
}

int getDefaultGameHeapId() {
    return g_DefaultGameHeapId;
}

inline bool isValidHeapId(u32 id) {
    return id >= 1 && id <= 2;
}

const char *const s_GameHeapNames[4] = {
        0,
        "ゲーム用汎用ヒープ1(mHeap::gameHeaps[1])",
        "ゲーム用汎用ヒープ2(mHeap::gameHeaps[2])",
        0,
};

EGG::ExpHeap *createGameHeap(int heapId, size_t size, EGG::Heap *parent) {
    if (!isValidHeapId(heapId)) {
        return nullptr;
    }

    g_gameHeaps[heapId] = createHeap(size, parent, s_GameHeapNames[heapId]);
    if (heapId == g_DefaultGameHeapId) {
        g_gameHeaps[0] = g_gameHeaps[heapId];
    }
    return g_gameHeaps[heapId];
}
EGG::ExpHeap *createGameHeap1(size_t size, EGG::Heap *parent) {
    return createGameHeap(getDefaultGameHeapId(), size, parent);
}
EGG::ExpHeap *createArchiveHeap(size_t size, EGG::Heap *parent) {
    g_archiveHeap = createHeap(size, parent, "汎用ファイル読み込み用ヒープ(mHeap::archiveHeap)");
    return g_archiveHeap;
}
EGG::ExpHeap *createCommandHeap(size_t size, EGG::Heap *parent) {
    g_commandHeap = createHeap(size, parent, "DVD読み込みコマンド用ヒープ(mHeap::commandHeap)");
    return g_commandHeap;
}
EGG::ExpHeap *createDylinkHeap(size_t size, EGG::Heap *parent) {
    g_dylinkHeap = createHeap(size, parent, "ダイナミックリンク用ヒープ(mHeap::dylinkHeap)");
    return g_dylinkHeap;
}
EGG::AssertHeap *createAssertHeap(EGG::Heap *parent) {
    const char *name = "アサートヒープ(mHeap::assertHeap)";
    g_assertHeap = EGG::AssertHeap::create(EGG::AssertHeap::getSize(), parent);
    g_assertHeap->mName = name;
    return g_assertHeap;
}

EGG::Heap *makeHeapOnCurrentGameHeap(size_t size, char *name, s32 align, u32 flags) {
    return makeFrmHeapAndUpdate(size, g_gameHeaps[0], name, align, flags);
}

} // namespace mHeap
