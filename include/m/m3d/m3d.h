#ifndef M3D_H
#define M3D_H

#include <egg/core/eggHeap.h>
#include <m/m_allocator.h>
#include <nw4r/g3d/g3d_scnroot.h>

namespace m3d {

namespace internal {

extern mAllocator_c *l_allocator_p;
extern nw4r::g3d::ScnRoot *l_scnRoot_p;

}

bool create(EGG::Heap *heap, u32 u1, u32 u2, u32 u3, u32 u4);

nw4r::g3d::ScnRoot *getScnRoot();
nw4r::g3d::Camera getCamera(int id);
nw4r::g3d::Camera getCurrentCamera();
int getCurrentCameraID();

// TODO EGG

void calcWorld(int idx);
void calcMaterial();

void drawOpa();
void drawXlu();

bool pushBack(nw4r::g3d::ScnObj *obj);
void clear();

void reset();

void resetMaterial();

} // namespace m3d

#endif
