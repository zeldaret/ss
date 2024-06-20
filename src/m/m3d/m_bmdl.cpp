#include <m/m3d/m_bmdl.h>
#include <nw4r/g3d/g3d_scnmdl.h>

namespace m3d {

bmdl_c::~bmdl_c() {
    remove();
}

int bmdl_c::getType() const {
    return 0;
}

void bmdl_c::getNodeWorldMtx(u32 p1, nw4r::math::MTX34 *out) const {
    nw4r::g3d::G3dObj::DynamicCast<nw4r::g3d::ScnMdlSimple>(mpScnLeaf)->GetScnMtxPos(out,
            nw4r::g3d::ScnObj::MTX_TYPE_WORLD, p1);
}

void bmdl_c::setAnm(banm_c &anm) {
    nw4r::g3d::ScnMdlSimple *mdl;
    if (anm.getType() == 5) {
        mdl = nw4r::g3d::G3dObj::DynamicCast<nw4r::g3d::ScnMdl>(mpScnLeaf);
        mdl->SetAnmObj(anm.getAnimObj(), nw4r::g3d::ScnMdlSimple::ANMOBJ_6);
    } else {
        mdl = nw4r::g3d::G3dObj::DynamicCast<nw4r::g3d::ScnMdlSimple>(mpScnLeaf);
        if (anm.getType() == 0) {
            mpCurrentAnm = &anm;
        }
        mdl->SetAnmObj(anm.getAnimObj(), nw4r::g3d::ScnMdlSimple::ANMOBJ_6);
    }
}

void bmdl_c::play() {
    if (mpCurrentAnm != nullptr) {
        mpCurrentAnm->play();
    }
}

nw4r::g3d::ResMdl bmdl_c::getResMdl() const {
    return nw4r::g3d::G3dObj::DynamicCast<nw4r::g3d::ScnMdlSimple>(mpScnLeaf)->GetResMdl();
}

nw4r::g3d::ResMat bmdl_c::getResMat(u32 index) const {
    nw4r::g3d::ResMdl mdl = nw4r::g3d::G3dObj::DynamicCast<nw4r::g3d::ScnMdlSimple>(mpScnLeaf)->GetResMdl();
    return mdl.GetResMat(index);
}

void bmdl_c::removeAnm(nw4r::g3d::ScnMdlSimple::AnmObjType type) {
    // TODO regswap
    nw4r::g3d::ScnMdlSimple *mdl = nw4r::g3d::G3dObj::DynamicCast<nw4r::g3d::ScnMdlSimple>(mpScnLeaf);
    if (type == nw4r::g3d::ScnMdlSimple::ANMOBJ_0) {
        mpCurrentAnm = nullptr;
    }
    mdl->RemoveAnmObj(type);
}

void bmdl_c::remove() {
    mpCurrentAnm = nullptr;
    scnLeaf_c::remove();
}

} // namespace m3d
