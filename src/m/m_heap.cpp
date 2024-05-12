#include <egg/core/eggAssertHeap.h>
#include <egg/core/eggExpHeap.h>
#include <egg/core/eggFrmHeap.h>
#include <m/m_heap.h>

u8 mHeap::g_DefaultGameHeapId = 1;

#define MIN_ALIGN 0x20

EGG::Heap *mHeap::g_gameHeaps[4];
EGG::Heap *mHeap::s_SavedCurrentHeap;
EGG::Heap *mHeap::g_archiveHeap;
EGG::Heap *mHeap::g_commandHeap;
EGG::Heap *mHeap::g_dylinkHeap;
EGG::AssertHeap *mHeap::g_assertHeap;
const char *const mHeap::s_GameHeapNames[4] = {
        0,
        "ゲーム用汎用ヒープ1(mHeap::gameHeaps[1])",
        "ゲーム用汎用ヒープ2(mHeap::gameHeaps[2])",
        0,
};

u16 mHeap::copyAttribute(u32 arg) {
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

EGG::Heap *mHeap::setCurrentHeap(EGG::Heap *heap) {
    return heap->becomeCurrentHeap();
}
EGG::Heap *mHeap::createExpHeap(size_t size, EGG::Heap *parentHeap, const char *name, u32 align, u32 attrs) {
    if (parentHeap == nullptr) {
        parentHeap = EGG::Heap::sCurrentHeap;
    }

    if (align < MIN_ALIGN) {
        align = MIN_ALIGN;
    }

    if (size != 0xFFFFFFFF) {
        size = expHeapCost(size, align);
    } else {
        size = parentHeap->getAllocatableSize(align);
    }

    void *mem = parentHeap->alloc(size, align);
    EGG::ExpHeap *heap = nullptr;
    if (mem != nullptr) {
        heap = EGG::ExpHeap::create(mem, size, copyAttribute(attrs));
        if (heap == nullptr) {
            parentHeap->free(mem);
        } else if (name != nullptr) {
            heap->mName = name;
        }
    }
    return heap;
}

size_t mHeap::adjustExpHeap(EGG::ExpHeap *heap) {
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
size_t mHeap::expHeapCost(size_t size, u32 align) {
    int a = align - 1;
    s32 b = (0x84 + a);

    return size + (~(a)&b);
}

EGG::FrmHeap *mHeap::createFrmHeap(size_t size, EGG::Heap *parent, const char *name, size_t align, size_t attrs) {
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
void mHeap::destroyFrmHeap(EGG::FrmHeap *heap) {
    if (heap != nullptr) {
        heap->destroy();
    }
}

size_t mHeap::adjustFrmHeap(EGG::FrmHeap *heap) {
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

size_t mHeap::frmHeapCost(size_t size, u32 align) {
    int a = align - 1;
    s32 b = (0x7C + a);

    return size + (~(a)&b);
}

mHeap::mHeap(EGG::Heap *heap) {
    this->heap = heap->becomeCurrentHeap();
}

mHeap::~mHeap() {
    heap->becomeCurrentHeap();
}

EGG::Heap *mHeap::createHeap(size_t size, EGG::Heap *block, const char *name) {
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
void mHeap::saveCurrentHeap() {
    s_SavedCurrentHeap = EGG::Heap::sCurrentHeap;
}
void mHeap::restoreCurrentHeap() {
    s_SavedCurrentHeap->becomeCurrentHeap();
    s_SavedCurrentHeap = nullptr;
}

EGG::FrmHeap *mHeap::makeFrmHeapAndUpdate(size_t size, EGG::Heap *parentHeap, const char *name, u32 align, u32 unk) {
    EGG::FrmHeap *heap = createFrmHeap(size, parentHeap, name, align, unk);
    if (heap != nullptr) {
        s_SavedCurrentHeap = setCurrentHeap(heap);
    }
    return heap;
}

int mHeap::getDefaultGameHeapId() {
    return g_DefaultGameHeapId;
}

EGG::Heap *mHeap::createGameHeap(int heapId, size_t size, EGG::Heap *parent) {
    if (!isValidHeapId(heapId)) {
        return nullptr;
    }

    g_gameHeaps[heapId] = mHeap::createHeap(size, parent, s_GameHeapNames[heapId]);
    if (heapId == g_DefaultGameHeapId) {
        g_gameHeaps[0] = g_gameHeaps[heapId];
    }
    return g_gameHeaps[heapId];
}
void mHeap::createGameHeap1(size_t size, EGG::Heap *parentHeap) {
    createGameHeap(getDefaultGameHeapId(), size, parentHeap);
}
void mHeap::createArchiveHeap(size_t size, EGG::Heap *parent) {
    g_archiveHeap = createHeap(size, parent, "汎用ファイル読み込み用ヒープ(mHeap::archiveHeap)");
}
void mHeap::createCommandHeap(size_t size, EGG::Heap *parent) {
    g_commandHeap = createHeap(size, parent, "DVD読み込みコマンド用ヒープ(mHeap::commandHeap)");
}
void mHeap::createDylinkHeap(size_t size, EGG::Heap *parent) {
    g_dylinkHeap = createHeap(size, parent, "ダイナミックリンク用ヒープ(mHeap::dylinkHeap)");
}
EGG::AssertHeap *mHeap::createAssertHeap(EGG::Heap *parent) {
    const char *name = "アサートヒープ(mHeap::assertHeap)";
    g_assertHeap = EGG::AssertHeap::create(EGG::AssertHeap::getSize(), parent);
    g_assertHeap->mName = name;
    return g_assertHeap;
}

EGG::Heap *mHeap::makeHeapOnCurrentGameHeap(size_t size, const char *name, u32 align, u32 flags) {
    return makeFrmHeapAndUpdate(size, g_gameHeaps[0], name, align, flags);
}
