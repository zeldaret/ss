#ifndef D_D2D_H
#define D_D2D_H

#include "egg/core/eggHeap.h"

namespace d2d {

void create(EGG::Heap *);
void defaultSet();
void drawBefore();
void draw();

void setLytAllocator();

} // namespace d2d

#endif
