#ifndef D_HEAP_H
#define D_HEAP_H

#include "egg/core/eggExpHeap.h"

class dHeap {
public:
    EGG::ExpHeap *heap;

public:
    static dHeap work1Heap;
    static dHeap work2Heap;
    static dHeap workExHeap;
    static dHeap layoutHeap;
    static dHeap layoutExHeap;
    static dHeap layoutEx2Heap;
    static dHeap layoutResHeap;
    static dHeap fontHeap;
    static dHeap HBMHeap;

public:
    EGG::ExpHeap *init(const char *name, size_t size, EGG::Heap *parent);
    static void createWork1Heap(size_t size, EGG::Heap *parent);
    static void createWork2Heap(size_t size, EGG::Heap *parent);
    static void createWorkExHeap(size_t size, EGG::Heap *parent);
    static void createLayoutHeap(size_t size, EGG::Heap *parent);
    static void createLayoutExHeap(size_t size, EGG::Heap *parent);
    static void createLayoutEx2Heap(size_t size, EGG::Heap *parent);
    static void createLayoutResHeap(size_t size, EGG::Heap *parent);
    static void createFontHeap(size_t size, EGG::Heap *parent);
    static void createHBMHeap(size_t size, EGG::Heap *parent);
};

#endif
