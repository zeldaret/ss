#include "m/m3d/m_anmmdl.h"

#include "nw4r/g3d/res/g3d_resfile.h"

namespace m3d {

mdlAnmChr::~mdlAnmChr() {}

void mdlAnmChr::play() {
    mMdl.play();
}

void mdlAnmChr::setFrame(f32 f) {
    mAnm.setFrameOnly(f);
}

void mdlAnmChr::setAnm(const char *name, playMode_e mode) {
    setAnm(name, mode, 0.0f);
}

void mdlAnmChr::setRate(f32 rate) {
    mAnm.setRate(rate);
}

bool mdlAnmChr::create(
    void *mdlFile, void *anmFile, const char *mdlName, const char *anmName, mAllocator_c *alloc, u32 bufferOption,
    int nView, u32 *pSize
) {
    return create(mdlFile, anmFile, mdlName, anmName, nullptr, alloc, bufferOption, nView, pSize);
}

bool mdlAnmChr::create2(
    void *mdlFile, void *anmFile, const char *mdlName, const char *anmName, mAllocator_c *alloc, u32 bufferOption,
    int nView, u32 *pSize
) {
    return create(mdlFile, anmFile, mdlName, anmName, alloc, bufferOption, nView, pSize);
}

bool mdlAnmChr::create(
    void *mdlFile, void *anmFile, const char *mdlName, const char *anmName, mdl_c::mdlCallback_c *callback,
    mAllocator_c *alloc, u32 bufferOption, int nView, u32 *pSize
) {
    mMdlFile = static_cast<nw4r::g3d::ResFile>(mdlFile);
    mAnmFile = static_cast<nw4r::g3d::ResFile>(anmFile);

    nw4r::g3d::ResMdl resMdl = mMdlFile.GetResMdl(mdlName);
    if (!mMdl.create(resMdl, callback, alloc, bufferOption, nView, pSize)) {
        return false;
    }

    nw4r::g3d::ResAnmChr resAnm = mAnmFile.GetResAnmChr(anmName);
    if (!resAnm.IsValid()) {
        resAnm = mMdlFile.GetResAnmChr(anmName);
    }
    u32 oldSize;
    if (pSize != nullptr) {
        oldSize = *pSize;
    }

    if (!mAnm.create(resMdl, resAnm, alloc, pSize)) {
        mMdl.remove();
        return false;
    }

    if (pSize != nullptr) {
        *pSize += oldSize;
    }
    return true;
}

bool mdlAnmChr::create(
    void *resFile, const char *mdlName, const char *anmName, mAllocator_c *alloc, u32 bufferOption, int nView,
    u32 *pSize
) {
    return create(resFile, resFile, mdlName, anmName, nullptr, alloc, bufferOption, nView, pSize);
}

void mdlAnmChr::setAnm(const char *name, playMode_e mode, f32 blend) {
    nw4r::g3d::ResAnmChr anm = mAnmFile.GetResAnmChr(name);
    if (!anm.IsValid()) {
        anm = mMdlFile.GetResAnmChr(name);
    }
    mAnm.setAnm(mMdl, anm, mode);
    mMdl.setAnm(mAnm, blend);
}

} // namespace m3d
