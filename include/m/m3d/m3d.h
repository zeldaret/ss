#ifndef M3D_H
#define M3D_H

#include <egg/core/eggHeap.h>
#include <egg/gfx/eggFog.h>
#include <egg/gfx/eggLight.h>
#include <m/m_allocator.h>
#include <nw4r/g3d/g3d_scnroot.h>

namespace m3d {

namespace internal {

extern mAllocator_c *l_allocator_p;
extern nw4r::g3d::ScnRoot *l_scnRoot_p;
extern u32 l_numLightMgr;
extern EGG::LightManager **l_lightMgr_pp;
extern u32 l_numFogMgr;
extern EGG::FogManager **l_fogMgr_pp;
extern size_t l_alignment;

} // namespace internal

bool create(EGG::Heap *pHeap, u32 maxNumChildren, u32 maxNumScnObj, u32 numLightObj, u32 numLightSet);
bool create(EGG::Heap *pHeap, GXPixelFmt pxlFmt, GXColor clearColor, u32 maxNumChildren, u32 maxNumScnObj,
        u32 numLightObj, u32 numLightSet, u32 numLightMgr, u32 numFogMgr);
bool createLightMgr(EGG::Heap *, u16, u16, u8, bool, int);
bool createFogMgr(EGG::Heap *, int, int);

nw4r::g3d::ScnRoot *getScnRoot();
nw4r::g3d::Camera getCamera(int id);
nw4r::g3d::Camera getCurrentCamera();
int getCurrentCameraID();

EGG::LightManager *getLightMgr(int idx);
EGG::FogManager *getFogMgr(int idx);
void drawDone(int idx);
void drawLightMapTexture(int idx);

void calcWorld(int idx);
void calcMaterial();
void calcView(int idx);

void drawOpa();
void drawXlu();

bool pushBack(nw4r::g3d::ScnObj *obj);
void clear();

void reset();

int getMatID(nw4r::g3d::ResMdl, const char *);
int getNodeID(nw4r::g3d::ResMdl, const char *);

void resetMaterial();

} // namespace m3d

#endif
