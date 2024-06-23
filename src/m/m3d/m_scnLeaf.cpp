#include <m//m3d/m3d.h>
#include <m/m3d/m_scnleaf.h>

namespace m3d {

scnLeaf_c::scnLeaf_c() : mpScnLeaf(nullptr) {}

scnLeaf_c::~scnLeaf_c() {
    scnLeaf_c::remove();
}

void scnLeaf_c::remove() {
    if (mpScnLeaf != nullptr) {
        mpScnLeaf->Destroy();
        mpScnLeaf = nullptr;
    }
}

void scnLeaf_c::setOption(u32 flag, u32 set) {
    mpScnLeaf->SetScnObjOption(flag, set);
}

void scnLeaf_c::setScale(f32 x, f32 y, f32 z) {
    mpScnLeaf->SetScale(x, y, z);
}

void scnLeaf_c::getScale(nw4r::math::VEC3 *vec) const {
    mpScnLeaf->GetScale(vec);
}

int scnLeaf_c::entry() {
    return pushBack(mpScnLeaf);
}

void scnLeaf_c::setScale(const nw4r::math::VEC3 &scale) {
    mpScnLeaf->SetScale(scale);
}

void scnLeaf_c::setLocalMtx(const nw4r::math::MTX34 *mtx) {
    mpScnLeaf->SetMtx(nw4r::g3d::ScnObj::MTX_TYPE_LOCAL, mtx);
}

void scnLeaf_c::getLocalMtx(nw4r::math::MTX34 *mtx) const {
    mpScnLeaf->GetMtx(nw4r::g3d::ScnObj::MTX_TYPE_LOCAL, mtx);
}

const nw4r::math::MTX34 *scnLeaf_c::getLocalMtx() const {
    return mpScnLeaf->GetMtxPtr(nw4r::g3d::ScnObj::MTX_TYPE_LOCAL);
}

void scnLeaf_c::getWorldMtx(nw4r::math::MTX34 *mtx) const {
    mpScnLeaf->GetMtx(nw4r::g3d::ScnObj::MTX_TYPE_WORLD, mtx);
}

void scnLeaf_c::getViewMtx(nw4r::math::MTX34 *mtx) const {
    mpScnLeaf->GetMtx(nw4r::g3d::ScnObj::MTX_TYPE_VIEW, mtx);
}

void scnLeaf_c::calc(bool b) {
    setOption(/* DISABLE_CALC_WORLD */ 0x02, false);
    mpScnLeaf->G3dProc(1, 0, nullptr);
    if (b == false) {
        setOption(/* DISABLE_CALC_WORLD */ 0x02, true);
    }
}

void scnLeaf_c::calcVtx(bool b) {
    setOption(/* DISABLE_CALC_VTX */ 0x04, false);
    mpScnLeaf->G3dProc(3, 0, nullptr);
    if (b == false) {
        setOption(/* DISABLE_CALC_VTX */ 0x04, true);
    }
}

} // namespace m3d
