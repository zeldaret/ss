#ifndef D_DVD_H
#define D_DVD_H

#include <common.h>
#include <egg/core/eggHeap.h>
#include <m/m_dvd.h>

namespace dDvd {

// difference to NSMBW: additional heap arg
void create(s32 priority, EGG::Heap*, EGG::Heap*, EGG::Heap*);

class loader_c {
public:
    loader_c();
    /** vtable at 0x805033F0 */
    virtual ~loader_c();
    void *request(const char *path, u8 mountDirection, EGG::Heap *heap);
    virtual void remove();

private:
    u32 mSize;
    mDvd_toMainRam_normal_c *mpCommand;
    EGG::Heap* mpHeap;
    void* mpBuffer;
};

class unkstruct_c {
public:
    static unkstruct_c *create(EGG::Heap *heap);
    void init();
    char getUnk();

private:
    UNKWORD field_0x0;
    char field_0x4;
};

} // dDvd

#endif
