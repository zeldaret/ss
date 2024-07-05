#include <m/m3d/m_anmchr.h>
#include <m/m3d/m3d.h>
#include <nw4r/g3d/g3d_anmchr.h>
#include <nw4r/g3d/g3d_resanmchr.h>

namespace m3d {

anmChr_c::~anmChr_c() {}

int anmChr_c::getType() const {
    return 0;
}

bool anmChr_c::create(nw4r::g3d::ResMdl mdl, nw4r::g3d::ResAnmChr anm, mAllocator_c* alloc, u32* pSize) {
    if (alloc == nullptr) {
        alloc = internal::l_allocator_p;
    }

    u32 size;
    if (pSize == nullptr) {
        pSize = &size;
    }

    nw4r::g3d::AnmObjChrRes::Construct(nullptr, pSize, anm, mdl, false);
    if (!createAllocator(alloc, pSize)) {
        return false;
    }

    mpAnmObj = nw4r::g3d::AnmObjChrRes::Construct(&mAllocator, &size, anm, mdl, false);
    if (!mpAnmObj->Bind(mdl)) {
        remove();
        return false;
    }

    setFrmCtrlDefault(anm, PLAY_MODE_4);
    return true;
}

void anmChr_c::setAnm(bmdl_c &mdl, nw4r::g3d::ResAnmChr anm, playMode_e mode) {
    mdl.removeAnm((nw4r::g3d::ScnMdlSimple::AnmObjType)getType());
    setAnmAfter(mdl, anm, mode);
}

void anmChr_c::setAnmAfter(bmdl_c &mdl, nw4r::g3d::ResAnmChr anm, playMode_e mode) {
    nw4r::g3d::AnmObjChrNode *node;
    nw4r::g3d::G3dObj *parent = mpAnmObj->GetParent();
    f32 weight;
    int idx;
    if (parent != nullptr) {
        node = nw4r::g3d::G3dObj::DynamicCast<nw4r::g3d::AnmObjChrNode>(parent);
        int size = node->Size();
        idx = 0;
        for (int i = 0; i < size; i++, idx++) {
            if (node->GetNode(i) == mpAnmObj) {
                break;
            }
        }
        weight = node->GetWeight(idx);
        node->Detach(idx);
    }
    mpAnmObj->Release();
    mpAnmObj->Destroy();
    mpFrameHeap->free(3);
    u32 tmp;
    mpAnmObj = nw4r::g3d::AnmObjChrRes::Construct(&mAllocator, &tmp, anm, mdl.getResMdl(), false);
    mpAnmObj->Bind(mdl.getResMdl());
    setFrmCtrlDefault(anm, mode);

    if (parent != nullptr) {
        nw4r::g3d::AnmObjChrRes *o = nw4r::g3d::G3dObj::DynamicCast<nw4r::g3d::AnmObjChrRes>(mpAnmObj);
        node->SetWeight(idx, weight);
        node->Attach(idx, o);

    }

}

void anmChr_c::setFrmCtrlDefault(nw4r::g3d::ResAnmChr &anm, m3d::playMode_e mode) {
    if (mode == PLAY_MODE_4) {
        mode = anm.GetAnmPolicy() == nw4r::g3d::ANM_POLICY_ONETIME ? PLAY_MODE_1 : PLAY_MODE_0;
    }
    set(anm.GetNumFrame(), mode, 1.0f, -1.0f);
}

} // namespace m3d
