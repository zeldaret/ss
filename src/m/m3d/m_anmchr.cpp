#include <m/m3d/m_anmchr.h>
#include <nw4r/g3d/g3d_anmobj.h>
#include <nw4r/g3d/g3d_resanmchr.h>

namespace m3d {

anmChr_c::~anmChr_c() {}

int anmChr_c::getType() {
    return 0;
}

void anmChr_c::setAnm(bmdl_c &mdl, nw4r::g3d::ResAnmChr anm, playMode_e mode) {
    mdl.removeAnm((nw4r::g3d::ScnMdlSimple::AnmObjType)getType());
    setAnmAfter(mdl, anm, mode);
}

void anmChr_c::setAnmAfter(bmdl_c &mdl, nw4r::g3d::ResAnmChr anm, playMode_e mode) {
    // TODO g3d headers
    // void *parent = nw4r::g3d::G3dObj::DynamicCast<nw4r::g3d::AnmObjChrNode>(mpAnmObj->GetParent());
}

void anmChr_c::setFrmCtrlDefault(nw4r::g3d::ResAnmChr &anm, m3d::playMode_e mode) {
    if (mode == 4) {
        // TODO g3d headers
    }
    set(1.0f, mode, 1.0f, -1.0f);
}

} // namespace m3d
