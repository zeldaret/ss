#ifndef D_DVD_UNK_H
#define D_DVD_UNK_H

#include <common.h>
#include <egg/core/eggHeap.h>

namespace dDvdUnk {

class unkstruct_c {
public:
    static unkstruct_c *create(EGG::Heap *heap);
    void draw();
    void execute();
    void init();
    char getUnk();

    static void createFont(EGG::Heap *heap);

private:
    UNKWORD field_0x0;
    char field_0x4;
};

} // dDvdUnk

#endif
