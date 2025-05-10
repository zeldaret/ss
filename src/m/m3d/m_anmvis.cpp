#include "m/m3d/m_anmvis.h"

#include "m/m3d/m3d.h"
#include "nw4r/g3d/g3d_anmvis.h"
#include "nw4r/g3d/res/g3d_resanmvis.h"
#include "nw4r/g3d/res/g3d_resmdl.h"

namespace m3d {

anmVis_c::~anmVis_c() {}

int anmVis_c::getType() const {
    return nw4r::g3d::ScnMdlSimple::ANMOBJTYPE_VIS;
}

bool anmVis_c::create(nw4r::g3d::ResMdl mdl, nw4r::g3d::ResAnmVis anm, mAllocator_c *alloc, u32 *pSize) {
    if (alloc == nullptr) {
        alloc = internal::l_allocator_p;
    }

    u32 tmp;
    if (pSize == nullptr) {
        pSize = &tmp;
    }

    nw4r::g3d::AnmObjVisRes::Construct(nullptr, pSize, anm, mdl);
    if (!createAllocator(alloc, pSize)) {
        return false;
    }

    mpAnmObj = nw4r::g3d::AnmObjVisRes::Construct(&mAllocator, &tmp, anm, mdl);
    if (!mpAnmObj->Bind(mdl)) {
        remove();
        return false;
    } else {
        setFrmCtrlDefault(anm, PLAY_MODE_4);
        return true;
    }
}

void anmVis_c::setAnm(m3d::bmdl_c &mdl, nw4r::g3d::ResAnmVis anm, m3d::playMode_e mode) {
    mdl.removeAnm((nw4r::g3d::ScnMdlSimple::AnmObjType)getType());
    mpAnmObj->Release();
    mpFrameHeap->free(0x3);
    u32 tmp;
    mpAnmObj = nw4r::g3d::AnmObjVisRes::Construct(&mAllocator, &tmp, anm, mdl.getResMdl());
    mpAnmObj->Bind(mdl.getResMdl());
    setFrmCtrlDefault(anm, mode);
}

void anmVis_c::setFrmCtrlDefault(nw4r::g3d::ResAnmVis &anm, m3d::playMode_e mode) {
    if (mode == PLAY_MODE_4) {
        mode = anm.GetAnmPolicy() == nw4r::g3d::ANM_POLICY_ONETIME ? PLAY_MODE_1 : PLAY_MODE_0;
    }
    set(anm.GetNumFrame(), mode, 1.0f, -1.0f);
}

} // namespace m3d
