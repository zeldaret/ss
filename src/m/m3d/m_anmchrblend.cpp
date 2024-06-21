#include <m/m3d/m3d.h>
#include <m/m3d/m_anmchrblend.h>
#include <nw4r/g3d/g3d_anmchr.h>
#include <nw4r/g3d/g3d_resanmchr.h>

namespace m3d {

anmChrBlend_c::~anmChrBlend_c() {}

int anmChrBlend_c::getType() const {
    return 0;
}

bool anmChrBlend_c::create(nw4r::g3d::ResMdl mdl, int num, mAllocator_c *alloc, u32 *pSize) {
    if (alloc == nullptr) {
        alloc = internal::l_allocator_p;
    }

    u32 size;
    if (pSize == nullptr) {
        pSize = &size;
    }

    nw4r::g3d::AnmObjChrBlend::Construct(nullptr, pSize, mdl, num);
    if (!createAllocator(alloc, pSize)) {
        return false;
    }

    mpAnmObj = nw4r::g3d::AnmObjChrBlend::Construct(&mAllocator, &size, mdl, num);
    return true;
}

bool anmChrBlend_c::attach(int idx, nw4r::g3d::AnmObjChrRes *anm, f32 weight) {
    nw4r::g3d::AnmObjChrBlend *obj = nw4r::g3d::G3dObj::DynamicCast<nw4r::g3d::AnmObjChrBlend>(mpAnmObj);
    obj->SetWeight(idx, weight);
    obj->Attach(idx, anm);
}

bool anmChrBlend_c::attach(int idx, anmChr_c *anm, f32 weight) {
    nw4r::g3d::AnmObjChrRes *res = nw4r::g3d::G3dObj::DynamicCast<nw4r::g3d::AnmObjChrRes>(anm->getAnimObj());
    attach(idx, res, weight);
}

void anmChrBlend_c::detach(int idx) {
    nw4r::g3d::G3dObj::DynamicCast<nw4r::g3d::AnmObjChrBlend>(mpAnmObj)->Detach(idx);
}

void anmChrBlend_c::setWeight(int idx, f32 weight) {
    nw4r::g3d::G3dObj::DynamicCast<nw4r::g3d::AnmObjChrBlend>(mpAnmObj)->SetWeight(idx, weight);
}

f32 anmChrBlend_c::getWeight(int idx) const {
    nw4r::g3d::G3dObj::DynamicCast<nw4r::g3d::AnmObjChrBlend>(mpAnmObj)->GetWeight(idx);
}

} // namespace m3d
