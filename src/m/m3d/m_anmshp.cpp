#include <m/m3d/m3d.h>
#include <m/m3d/m_anmshp.h>
#include <nw4r/g3d/g3d_anmshp.h>

namespace m3d {

anmShp_c::~anmShp_c() {}

int anmShp_c::getType() const {
    return 0x5;
}

bool anmShp_c::create(nw4r::g3d::ResMdl mdl, nw4r::g3d::ResAnmShp anm, mAllocator_c *alloc, u32 *pSize) {
    if (alloc == nullptr) {
        alloc = internal::l_allocator_p;
    }

    u32 tmp;
    if (pSize == nullptr) {
        pSize = &tmp;
    }

    // This might be an nw4r inline? Note that some of the roundings
    // are no-ops that still emit instructions because mwcc doesn't do
    // math
    u32 numEntries = mdl.GetResVtxPosNumEntries();
    *pSize = ROUND_UP(ROUND_UP(numEntries * 2 + 0x38, 0x04) + numEntries * 0x0C, 0x04);

    if (!createAllocator(alloc, pSize)) {
        return false;
    }

    mpAnmObj = nw4r::g3d::AnmObjShpRes::Construct(&mAllocator, &tmp, anm, mdl, 0);
    if (!mpAnmObj->Bind(mdl)) {
        remove();
        return false;
    } else {
        setFrmCtrlDefault(anm, PLAY_MODE_4);
        return true;
    }
}

void anmShp_c::setAnm(m3d::bmdl_c &mdl, nw4r::g3d::ResAnmShp anm, m3d::playMode_e mode) {
    mdl.removeAnm((nw4r::g3d::ScnMdlSimple::AnmObjType)getType());
    mpAnmObj->Release();
    mpFrameHeap->free(0x3);
    u32 tmp;
    mpAnmObj = nw4r::g3d::AnmObjShpRes::Construct(&mAllocator, &tmp, anm, mdl.getResMdl(), 0);
    mpAnmObj->Bind(mdl.getResMdl());
    setFrmCtrlDefault(anm, mode);
}

void anmShp_c::setFrmCtrlDefault(nw4r::g3d::ResAnmShp &anm, m3d::playMode_e mode) {
    if (mode == PLAY_MODE_4) {
        mode = anm.GetAnmPolicy() == nw4r::g3d::ANM_POLICY_ONETIME ? PLAY_MODE_1 : PLAY_MODE_0;
    }
    set(anm.GetNumFrame(), mode, 1.0f, -1.0f);
}

} // namespace m3d
