#include <m/m3d/m3d.h>
#include <nw4r/g3d/g3d_init.h>
#include <nw4r/g3d/g3d_state.h>
#include <rvl/GX.h>

namespace m3d {

namespace internal {

mAllocator_c *l_allocator_p;
nw4r::g3d::ScnRoot *l_scnRoot_p;

} // namespace internal

extern "C" GXRenderModeObj **lbl_80575C88;

bool create(EGG::Heap *heap, u32 u1, u32 u2, u32 u3, u32 u4) {
    internal::l_allocator_p = new (heap, 0x04) mAllocator_c();

    if (!internal::l_allocator_p) {
        return false;
    }

    internal::l_allocator_p->attach(heap, 0x20);
    nw4r::g3d::G3dInit(true);
    nw4r::g3d::G3DState::SetRenderModeObj(**lbl_80575C88);
    u32 allocedSize;
    internal::l_scnRoot_p = nw4r::g3d::ScnRoot::Construct(internal::l_allocator_p, &allocedSize, u1, u2, u3, u4);
    if (internal::l_scnRoot_p == nullptr) {
        delete internal::l_allocator_p;
        internal::l_allocator_p = nullptr;
        return false;
    }
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
    return internal::l_scnRoot_p->getLightSetting();
}

// TODO EGG

void calcWorld(int idx) {
    internal::l_scnRoot_p->CalcWorld();
}

void calcMaterial() {
    internal::l_scnRoot_p->CalcMaterial();
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
    // TODO EGG
}

void resetMaterial() {
    GXSetNumIndStages(0);
    for (int i = GX_TEVSTAGE0; i < GX_MAX_TEVSTAGE; i++) {
        GXSetTevDirect((GXTevStageID)i);
    }
}

} // namespace m3d
