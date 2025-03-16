#include "m/m3d/m_bmdl.h"

#include "m/m3d/m3d.h"
#include "nw4r/g3d/g3d_scnmdl.h"

namespace m3d {

bmdl_c::~bmdl_c() {
    remove();
}

int bmdl_c::getType() const {
    return SCN_LEAF_MODEL;
}

int bmdl_c::getMatID(const char *name) const {
    return m3d::getMatID(getResMdl(), name);
}

int bmdl_c::getNodeID(const char *name) const {
    return m3d::getNodeID(getResMdl(), name);
}

bool bmdl_c::getNodeWorldMtx(u32 p1, nw4r::math::MTX34 *out) const {
    return nw4r::g3d::G3dObj::DynamicCast<nw4r::g3d::ScnMdlSimple>(mpScnLeaf)->GetScnMtxPos(
        out, nw4r::g3d::ScnObj::MTX_WORLD, p1
    );
}

bool bmdl_c::getNodeWorldMtxMultVecZero(u32 p1, nw4r::math::VEC3 &out) const {
    nw4r::math::MTX34 mtx;
    if (!getNodeWorldMtx(p1, &mtx)) {
        return false;
    } else {
        out.x = mtx[0][3];
        out.y = mtx[1][3];
        out.z = mtx[2][3];
        return true;
    }
}

bool bmdl_c::getNodeWorldMtxMultVec(u32 p1, nw4r::math::VEC3 &in, nw4r::math::VEC3 &out) const {
    nw4r::math::MTX34 mtx;
    if (!getNodeWorldMtx(p1, &mtx)) {
        return false;
    } else {
        PSMTXMultVec(mtx, in, out);
        return true;
    }
}

bool bmdl_c::setAnm(banm_c &anm) {
    nw4r::g3d::ScnMdlSimple *mdl;
    if (anm.getType() == nw4r::g3d::ScnMdlSimple::ANMOBJTYPE_SHP) {
        mdl = nw4r::g3d::G3dObj::DynamicCast<nw4r::g3d::ScnMdl>(mpScnLeaf);
        return mdl->SetAnmObj(anm.getAnimObj(), nw4r::g3d::ScnMdlSimple::ANMOBJTYPE_NOT_SPECIFIED);
    } else {
        mdl = nw4r::g3d::G3dObj::DynamicCast<nw4r::g3d::ScnMdlSimple>(mpScnLeaf);
        if (anm.getType() == nw4r::g3d::ScnMdlSimple::ANMOBJTYPE_CHR) {
            mpCurrentAnm = &anm;
        }
        return mdl->SetAnmObj(anm.getAnimObj(), nw4r::g3d::ScnMdlSimple::ANMOBJTYPE_NOT_SPECIFIED);
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
    nw4r::g3d::ScnMdlSimple *mdl = nw4r::g3d::G3dObj::DynamicCast<nw4r::g3d::ScnMdlSimple>(mpScnLeaf);
    if (type == nw4r::g3d::ScnMdlSimple::ANMOBJTYPE_CHR) {
        mpCurrentAnm = nullptr;
    }
    mdl->RemoveAnmObj(type);
}

nw4r::g3d::AnmObj *bmdl_c::getAnmObj(nw4r::g3d::ScnMdlSimple::AnmObjType type) const {
    nw4r::g3d::ScnMdlSimple *mdl = nw4r::g3d::G3dObj::DynamicCast<nw4r::g3d::ScnMdlSimple>(mpScnLeaf);
    return mdl->GetAnmObj(type);
}

void bmdl_c::setTevColor(u32 idx, GXTevRegID tevId, GXColor color, bool bMarkDirty) {
    nw4r::g3d::ScnMdl *mdl = nw4r::g3d::G3dObj::DynamicCast<nw4r::g3d::ScnMdl>(mpScnLeaf);

    if (mdl) {
        nw4r::g3d::ScnMdl::CopiedMatAccess matAccess(mdl, idx);
        nw4r::g3d::ResMatTevColor resMatTevClr = matAccess.GetResMatTevColor(bMarkDirty);
        resMatTevClr.GXSetTevColor(tevId, color);
        resMatTevClr.DCStore(false);
    } else {
        nw4r::g3d::ResMatTevColor resMatTevClr = getResMat(idx).GetResMatTevColor();
        resMatTevClr.GXSetTevColor(tevId, color);
        resMatTevClr.DCStore(false);
    }
}

void bmdl_c::setTevColorAll(GXTevRegID tevId, GXColor color, bool bMarkDirty) {
    nw4r::g3d::ResMdl resMdl = getResMdl();
    nw4r::g3d::ScnMdl *mdl = nw4r::g3d::G3dObj::DynamicCast<nw4r::g3d::ScnMdl>(mpScnLeaf);
    if (mdl) {
        for (u32 i = 0; i < resMdl.GetResMatNumEntries(); i++) {
            nw4r::g3d::ScnMdl::CopiedMatAccess matAccess(mdl, i);
            nw4r::g3d::ResMatTevColor resMatTevClr = matAccess.GetResMatTevColor(bMarkDirty);
            resMatTevClr.GXSetTevColor(tevId, color);
            resMatTevClr.DCStore(false);
        }
    } else {
        for (u32 i = 0; i < resMdl.GetResMatNumEntries(); i++) {
            nw4r::g3d::ResMatTevColor resMatTevClr = resMdl.GetResMat(i).GetResMatTevColor();
            resMatTevClr.GXSetTevColor(tevId, color);
            resMatTevClr.DCStore(false);
        }
    }
}

void bmdl_c::setTevKColor(u32 idx, GXTevKColorID tevKId, GXColor color, bool bMarkDirty) {
    nw4r::g3d::ScnMdl *mdl = nw4r::g3d::G3dObj::DynamicCast<nw4r::g3d::ScnMdl>(mpScnLeaf);

    if (mdl) {
        nw4r::g3d::ScnMdl::CopiedMatAccess matAccess(mdl, idx);
        nw4r::g3d::ResMatTevColor resMatTevClr = matAccess.GetResMatTevColor(bMarkDirty);
        resMatTevClr.GXSetTevKColor(tevKId, color);
        resMatTevClr.DCStore(false);
    } else {
        nw4r::g3d::ResMatTevColor resMatTevClr = getResMat(idx).GetResMatTevColor();
        resMatTevClr.GXSetTevKColor(tevKId, color);
        resMatTevClr.DCStore(false);
    }
}

void bmdl_c::setTevKColorAll(GXTevKColorID tevKId, GXColor color, bool bMarkDirty) {
    nw4r::g3d::ResMdl resMdl = getResMdl();
    nw4r::g3d::ScnMdl *mdl = nw4r::g3d::G3dObj::DynamicCast<nw4r::g3d::ScnMdl>(mpScnLeaf);
    if (mdl) {
        for (u32 i = 0; i < resMdl.GetResMatNumEntries(); i++) {
            nw4r::g3d::ScnMdl::CopiedMatAccess matAccess(mdl, i);
            nw4r::g3d::ResMatTevColor resMatTevClr = matAccess.GetResMatTevColor(bMarkDirty);
            resMatTevClr.GXSetTevKColor(tevKId, color);
            resMatTevClr.DCStore(false);
        }
    } else {
        for (u32 i = 0; i < resMdl.GetResMatNumEntries(); i++) {
            nw4r::g3d::ResMatTevColor resMatTevClr = resMdl.GetResMat(i).GetResMatTevColor();
            resMatTevClr.GXSetTevKColor(tevKId, color);
            resMatTevClr.DCStore(false);
        }
    }
}

void bmdl_c::setBlendModeAll(
    GXBlendMode blendMode, GXBlendFactor srcFactor, GXBlendFactor dstFactor, GXLogicOp op, bool bMarkDirty
) {
    nw4r::g3d::ResMdl resMdl = getResMdl();
    nw4r::g3d::ScnMdl *mdl = nw4r::g3d::G3dObj::DynamicCast<nw4r::g3d::ScnMdl>(mpScnLeaf);
    if (mdl) {
        for (u32 i = 0; i < resMdl.GetResMatNumEntries(); i++) {
            nw4r::g3d::ScnMdl::CopiedMatAccess matAccess(mdl, i);
            nw4r::g3d::ResMatPix resMatPix = matAccess.GetResMatPix(bMarkDirty);
            resMatPix.GXSetBlendMode(blendMode, srcFactor, dstFactor, op);
            resMatPix.DCStore(false);
        }
    } else {
        for (u32 i = 0; i < resMdl.GetResMatNumEntries(); i++) {
            nw4r::g3d::ResMatPix resMatPix = resMdl.GetResMat(i).GetResMatPix();
            resMatPix.GXSetBlendMode(blendMode, srcFactor, dstFactor, op);
            resMatPix.DCStore(false);
        }
    }
}

void bmdl_c::setCullMode(u32 matId, GXCullMode cullMode, bool bMarkDirty) {
    nw4r::g3d::ScnMdl *mdl = nw4r::g3d::G3dObj::DynamicCast<nw4r::g3d::ScnMdl>(mpScnLeaf);
    if (mdl) {
        nw4r::g3d::ScnMdl::CopiedMatAccess matAccess(mdl, matId);
        nw4r::g3d::ResGenMode resGenMode = matAccess.GetResGenMode(bMarkDirty);
        resGenMode.GXSetCullMode(cullMode);
    } else {
        nw4r::g3d::ResGenMode resGenMode = getResMat(matId).GetResGenMode();
        resGenMode.GXSetCullMode(cullMode);
    }
}

void bmdl_c::setCullModeAll(GXCullMode cullMode, bool bMarkDirty) {
    // TODO Extra stack allocation?
    nw4r::g3d::ResMdl resMdl = getResMdl();
    nw4r::g3d::ScnMdl *mdl = nw4r::g3d::G3dObj::DynamicCast<nw4r::g3d::ScnMdl>(mpScnLeaf);
    if (mdl) {
        for (u32 i = 0; i < resMdl.GetResMatNumEntries(); i++) {
            nw4r::g3d::ScnMdl::CopiedMatAccess matAccess(mdl, i);
            nw4r::g3d::ResGenMode resGenMode = matAccess.GetResGenMode(bMarkDirty);
            resGenMode.GXSetCullMode(cullMode);
        }
    } else {
        for (u32 i = 0; i < resMdl.GetResMatNumEntries(); i++) {
            nw4r::g3d::ResMat resMat = getResMat(i);
            nw4r::g3d::ResGenMode resGenMode = resMat.GetResGenMode();
            resGenMode.GXSetCullMode(cullMode);
        }
    }
}

void bmdl_c::setMatVisible(u32 matId, bool bVisble) {
    // TODO
}

int bmdl_c::setMatTexture(char *name, GXTexObj *texObj, bool copy, void *unk, int, int) {
    // TODO
}

bool bmdl_c::getBounds(mVec3_c *min, mVec3_c *max) const {
    // TODO
}

void bmdl_c::remove() {
    mpCurrentAnm = nullptr;
    scnLeaf_c::remove();
}

} // namespace m3d
