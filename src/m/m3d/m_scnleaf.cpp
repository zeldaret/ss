#include "m/m3d/m_scnleaf.h"

#include "m/m3d/m3d.h"

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

int scnLeaf_c::entry() {
    return pushBack(mpScnLeaf);
}

void scnLeaf_c::setOption(u32 flag, u32 set) {
    mpScnLeaf->SetScnObjOption(flag, set);
}

void scnLeaf_c::setScale(f32 x, f32 y, f32 z) {
    mpScnLeaf->SetScale(x, y, z);
}

void scnLeaf_c::setScale(const nw4r::math::VEC3 &scale) {
    mpScnLeaf->SetScale(scale);
}

void scnLeaf_c::getScale(nw4r::math::VEC3 *vec) const {
    mpScnLeaf->GetScale(vec);
}

void scnLeaf_c::setLocalMtx(const nw4r::math::MTX34 *mtx) {
    mpScnLeaf->SetMtx(nw4r::g3d::ScnObj::MTX_LOCAL, mtx);
}

void scnLeaf_c::getLocalMtx(nw4r::math::MTX34 *mtx) const {
    mpScnLeaf->GetMtx(nw4r::g3d::ScnObj::MTX_LOCAL, mtx);
}

const nw4r::math::MTX34 *scnLeaf_c::getLocalMtx() const {
    return mpScnLeaf->GetMtxPtr(nw4r::g3d::ScnObj::MTX_LOCAL);
}

void scnLeaf_c::getWorldMtx(nw4r::math::MTX34 *mtx) const {
    mpScnLeaf->GetMtx(nw4r::g3d::ScnObj::MTX_WORLD, mtx);
}

void scnLeaf_c::getViewMtx(nw4r::math::MTX34 *mtx) const {
    mpScnLeaf->GetMtx(nw4r::g3d::ScnObj::MTX_VIEW, mtx);
}

// unofficial
void scnLeaf_c::setCallback(nw4r::g3d::IScnObjCallback *cb) {
    mpScnLeaf->SetCallback(cb);
}

nw4r::g3d::IScnObjCallback *scnLeaf_c::swapCallback(nw4r::g3d::IScnObjCallback *cb) {
    nw4r::g3d::IScnObjCallback *tmp = mpScnLeaf->GetScnObjCallback();
    mpScnLeaf->SetCallback(cb);
    return tmp;
}

void scnLeaf_c::enableCallbackTiming(nw4r::g3d::ScnObj::Timing timing) {
    mpScnLeaf->EnableScnObjCallbackTiming(timing);
}

void scnLeaf_c::enableCallbackOp(nw4r::g3d::ScnObj::ExecOp op) {
    mpScnLeaf->EnableScnObjCallbackExecOp(op);
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

/** Unknown name/arg type */
void scnLeaf_c::calcView(void *p, bool b) {
    setOption(/* DISABLE_CALC_VIEW */ 0x05, false);
    mpScnLeaf->G3dProc(4, 0, p);
    if (b == false) {
        setOption(/* DISABLE_CALC_VIEW */ 0x05, true);
    }
}

void scnLeaf_c::setPriorityDraw(int opa, int xlu) {
    mpScnLeaf->SetPriorityDrawOpa(opa);
    mpScnLeaf->SetPriorityDrawXlu(xlu);
}

} // namespace m3d
