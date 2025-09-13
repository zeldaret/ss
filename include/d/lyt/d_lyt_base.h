#ifndef D_LYT_BASE_H
#define D_LYT_BASE_H

#include "common.h"
#include "d/d_base.h"
#include "m/m_allocator.h"

class dLytBase_c : public dBase_c {
public:
    dLytBase_c();
    virtual ~dLytBase_c();

    /* 0x4C */ virtual bool build();

protected:
    bool allocateLytWork1Heap(const char *name, bool exp);
    void removeLyt();

private:
    /* 0x68 */ mHeapAllocator_c mAllocator;
    /* 0x84 */ UNKWORD field_0x84;
    /* 0x88 */ u8 field_0x88;
};

#endif
