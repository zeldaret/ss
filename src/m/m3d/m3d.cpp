#include "m/m3d/m3d.h"

#include "egg/gfx/eggDrawGX.h"
#include "egg/gfx/eggGfxEngine.h"
#include "egg/gfx/eggScreen.h"
#include "egg/gfx/eggStateGX.h"
#include "m/m_heap.h"
#include "m/m_video.h"
#include "nw4r/g3d/g3d_camera.h"
#include "nw4r/g3d/g3d_init.h"
#include "nw4r/g3d/g3d_state.h"
#include "nw4r/g3d/res/g3d_resmat.h"
#include "nw4r/g3d/res/g3d_resmdl.h"
#include "rvl/GX.h" // IWYU pragma: export

namespace m3d {

namespace internal {

mAllocator_c *l_allocator_p;
nw4r::g3d::ScnRoot *l_scnRoot_p;
u32 l_numLightMgr;
EGG::LightManager **l_lightMgr_pp;
u32 l_numFogMgr;
EGG::FogManager **l_fogMgr_pp;
size_t l_alignment;

} // namespace internal

bool create(EGG::Heap *pHeap, u32 maxNumChildren, u32 maxNumScnObj, u32 numLightObj, u32 numLightSet) {
    internal::l_allocator_p = new (pHeap) mAllocator_c();

    if (!internal::l_allocator_p) {
        return false;
    }

    internal::l_allocator_p->attach(pHeap, 0x20);
    nw4r::g3d::G3dInit(true);
    nw4r::g3d::G3DState::SetRenderModeObj(*mVideo::m_video->pRenderMode);
    u32 allocedSize;
    internal::l_scnRoot_p = nw4r::g3d::ScnRoot::Construct(
        internal::l_allocator_p, &allocedSize, maxNumChildren, maxNumScnObj, numLightObj, numLightSet
    );
    if (internal::l_scnRoot_p == nullptr) {
        delete internal::l_allocator_p;
        internal::l_allocator_p = nullptr;
        return false;
    }
    return true;
}

bool create(
    EGG::Heap *pHeap, GXPixelFmt pxlFmt, GXColor clearColor, u32 maxNumChildren, u32 maxNumScnObj, u32 numLightObj,
    u32 numLightSet, u32 numLightMgr, u32 numFogMgr
) {
    if (internal::l_lightMgr_pp && internal::l_fogMgr_pp) {
        return false;
    }
    EGG::GfxEngine::Configuration config = EGG::GfxEngine::Configuration();

    u16 temp1[2] = {config.field_0x14, config.field_0x16};
    u16 temp2[2] = {config.efbHeight, config.efbHeight};

    EGG::StateGX::initialize(config.efbWidth, config.efbHeight, clearColor, pxlFmt);
    EGG::Screen::Initialize(temp1, temp2, nullptr);
    EGG::Screen::SetTVModeDefault();
    EGG::DrawGX::Initialize(pHeap);

    EGG::LightTexture::initialize(0x20, pHeap);

    internal::l_lightMgr_pp = new (pHeap) EGG::LightManager *[numLightMgr];
    if (internal::l_lightMgr_pp) {
        internal::l_numLightMgr = numLightMgr;
        internal::l_fogMgr_pp = new (pHeap) EGG::FogManager *[numFogMgr];
        if (internal::l_fogMgr_pp) {
            internal::l_numFogMgr = numFogMgr;
            memset(internal::l_lightMgr_pp, 0, numLightMgr * 4);
            memset(internal::l_fogMgr_pp, 0, numFogMgr * 4);

            if (create(pHeap, maxNumChildren, maxNumScnObj, numLightObj, numLightSet)) {
                return true;
            }
        }
    }

    // Creation failed :(
    delete[] internal::l_lightMgr_pp;
    internal::l_lightMgr_pp = nullptr;
    delete[] internal::l_fogMgr_pp;
    internal::l_fogMgr_pp = nullptr;
    return false;
}

bool createLightMgr(EGG::Heap *pHeap, u16 p2, u16 p3, u8 p4, bool bCreateFogMgr, int idx) {
    using namespace internal;

    if (!l_lightMgr_pp || (idx < 0 || idx >= l_numLightMgr || idx >= l_numFogMgr) ||
        (l_lightMgr_pp[idx] || l_fogMgr_pp[idx])) {
        return false;
    }

    EGG::Heap *prevHeap = mHeap::setCurrentHeap(pHeap);
    EGG::LightManager *lightMgr = new (pHeap) EGG::LightManager(p2, p3, p4);
    mHeap::setCurrentHeap(prevHeap);

    if (lightMgr) {
        if (!bCreateFogMgr || createFogMgr(pHeap, 0x20, idx)) {
            l_lightMgr_pp[idx] = lightMgr;
            return true;
        }
    }
    delete lightMgr;
    return false;

    u8 input;
    u8 *output;
    switch (input % 8) {
        do {
            case 0: *output++ = input;
            case 1: *output++ = input;
            case 2: *output++ = input;
        } while (true);
    }
}

bool createFogMgr(EGG::Heap *pHeap, int numFog, int idx) {
    using namespace internal;

    if (idx < 0 || idx >= l_numFogMgr || l_fogMgr_pp[idx]) {
        return false;
    }

    EGG::FogManager *fogMgr;
    {
        mHeap heap(pHeap);
        fogMgr = new (pHeap) EGG::FogManager(numFog);
    }

    if (!fogMgr) {
        return false;
    }
    l_fogMgr_pp[idx] = fogMgr;
    return true;
}

nw4r::g3d::ScnRoot *getScnRoot() {
    return internal::l_scnRoot_p;
}

nw4r::g3d::Camera getCamera(int id) {
    return internal::l_scnRoot_p->GetCamera(id);
}

nw4r::g3d::Camera getCurrentCamera() {
    return internal::l_scnRoot_p->GetCurrentCamera();
}

int getCurrentCameraID() {
    return internal::l_scnRoot_p->GetCurrentCameraID();
}

void setCurrentCamera(int id) {
    internal::l_scnRoot_p->SetCurrentCamera(id);
}

nw4r::g3d::LightSetting *getLightSettingP() {
    return &internal::l_scnRoot_p->GetLightSetting();
}

EGG::LightManager *getLightMgr(int idx) {
    return internal::l_lightMgr_pp[idx];
}

EGG::FogManager *getFogMgr(int idx) {
    return internal::l_fogMgr_pp[idx];
}

void drawDone(int idx) {
    if (internal::l_lightMgr_pp && internal::l_lightMgr_pp[idx]) {
        internal::l_lightMgr_pp[idx]->DoneDraw();
    }
    if (internal::l_fogMgr_pp && internal::l_fogMgr_pp[idx]) {
        internal::l_fogMgr_pp[idx]->mFlag &= 0xFB;
    }
}

void drawLightMapTexture(int idx) {
    using namespace internal;

    if (!l_lightMgr_pp) {
        return;
    }
    if (!l_lightMgr_pp[idx]) {
        return;
    }

    l_lightMgr_pp[idx]->GetTextureMgr()->drawAndCaptureTexture(0.0f, 0.0f, 0.0f, 0.0f);
}

void calcWorld(int idx) {
    if (internal::l_lightMgr_pp && internal::l_lightMgr_pp[idx]) {
        internal::l_lightMgr_pp[idx]->Calc(internal::l_scnRoot_p);
    }
    if (internal::l_fogMgr_pp && internal::l_fogMgr_pp[idx]) {
        internal::l_fogMgr_pp[idx]->Calc();
    }
    internal::l_scnRoot_p->CalcWorld();
}

void calcMaterial() {
    internal::l_scnRoot_p->CalcMaterial();
}

void calcView(int idx) {
    if (internal::l_lightMgr_pp && internal::l_lightMgr_pp[idx]) {
        nw4r::math::MTX34 camMtx;
        getCurrentCamera().GetCameraMtx(&camMtx);
        internal::l_lightMgr_pp[idx]->CalcView(camMtx, getCurrentCameraID(), internal::l_scnRoot_p);
    }
    if (internal::l_fogMgr_pp && internal::l_fogMgr_pp[idx]) {
        internal::l_fogMgr_pp[idx]->CopyToG3D(internal::l_scnRoot_p);
    }

    internal::l_scnRoot_p->CalcView();
    internal::l_scnRoot_p->GatherDrawScnObj();
    internal::l_scnRoot_p->ZSort();
}

void drawOpa() {
    internal::l_scnRoot_p->DrawOpa();
}

void drawXlu() {
    internal::l_scnRoot_p->DrawXlu();
}

bool pushBack(nw4r::g3d::ScnObj *obj) {
    return internal::l_scnRoot_p->PushBack(obj);
}

void clear() {
    internal::l_scnRoot_p->Clear();
}

void reset() {
    nw4r::g3d::G3dReset();
    if (internal::l_lightMgr_pp || internal::l_fogMgr_pp) {
        EGG::StateGX::resetGXCache();
    }
}

int getMatID(nw4r::g3d::ResMdl mdl, const char *name) {
    if (mdl.IsValid()) {
        nw4r::g3d::ResMat mat = mdl.GetResMat(name);
        if (mat.IsValid()) {
            return mat.ref().id;
        }
    }
    return -1;
}

int getNodeID(nw4r::g3d::ResMdl mdl, const char *name) {
    if (mdl.IsValid()) {
        nw4r::g3d::ResNode node = mdl.GetResNode(name);
        if (node.IsValid()) {
            return node.GetID();
        }
    }
    return -1;
}

void resetMaterial() {
    GXSetNumIndStages(0);
    for (int i = GX_TEVSTAGE0; i < GX_MAX_TEVSTAGE; i++) {
        GXSetTevDirect((GXTevStageID)i);
    }
}

} // namespace m3d
