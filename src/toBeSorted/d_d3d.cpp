#include "toBeSorted/d_d3d.h"

#include "common.h"
#include "d/a/obj/d_a_obj_base.h"
#include "d/d_sc_game.h"
#include "d/t/d_t_siren.h"
#include "egg/gfx/eggCpuTexture.h"
#include "egg/gfx/eggScreen.h"
#include "m/m3d/m_anmmdl.h"
#include "m/m3d/m_proc.h"
#include "m/m3d/m_scnleaf.h"
#include "m/m_color.h"
#include "nw4r/g3d/g3d_scnobj.h"
#include "nw4r/g3d/res/g3d_resanmchr.h"
#include "nw4r/g3d/res/g3d_resfile.h"
#include "nw4r/g3d/res/g3d_resmat.h"
#include "nw4r/g3d/res/g3d_resmdl.h"
#include "nw4r/g3d/res/g3d_restev.h"
#include "rvl/GX/GXTexture.h"
#include "rvl/GX/GXTypes.h"
#include "rvl/VI/vi.h"
#include "sized_string.h"
#include "toBeSorted/blur_and_palette_manager.h"

namespace d3d {

static EGG::Screen sSomeScreen;
static SomeList sSomeList;

AnmMdlWrapper::AnmMdlWrapper() : mpSoundSource(nullptr), mpSoundData(nullptr) {}

AnmMdlWrapper::~AnmMdlWrapper() {
    if (mpSoundSource != nullptr) {
        if (mpSoundSource->shutdown()) {
            do {
                VIWaitForRetrace();
            } while (mpSoundSource->shutdown());
        }
    }
}

bool AnmMdlWrapper::create(
    dAcBase_c &ac, void *mdlFile, void *anmFile, const char *mdlName, const char *anmName, u32 bufferOption
) {
    if (!m3d::mdlAnmChr::create(mdlFile, anmFile, mdlName, anmName, &ac.heap_allocator, bufferOption, 1, nullptr)) {
        return false;
    }
    mpSoundSource = ac.getSoundSource();
    return true;
}

bool AnmMdlWrapper::create2(
    dAcBase_c &ac, void *mdlFile, void *anmFile, const char *mdlName, const char *anmName, u32 bufferOption
) {
    return create(ac, mdlFile, anmFile, mdlName, anmName, bufferOption);
}

bool AnmMdlWrapper::create(
    dAcBase_c &ac, void *mdlFile, void *anmFile, const char *mdlName, const char *anmName,
    m3d::mdl_c::mdlCallback_c *callback, u32 bufferOption
) {
    if (!m3d::mdlAnmChr::create(
            mdlFile, anmFile, mdlName, anmName, callback, &ac.heap_allocator, bufferOption, 1, nullptr
        )) {
        return false;
    }
    mpSoundSource = ac.getSoundSource();
    return true;
}

bool AnmMdlWrapper::create2(
    dAcBase_c &ac, void *mdlFile, void *anmFile, const char *mdlName, const char *anmName,
    m3d::mdl_c::mdlCallback_c *callback, u32 bufferOption
) {
    return create(ac, mdlFile, anmFile, mdlName, anmName, callback, bufferOption);
}

bool AnmMdlWrapper::create(dAcBase_c &ac, void *resFile, const char *mdlName, const char *anmName, u32 bufferOption) {
    return create(ac, resFile, resFile, mdlName, anmName, bufferOption);
}

bool AnmMdlWrapper::create2(dAcBase_c &ac, void *resFile, const char *mdlName, const char *anmName, u32 bufferOption) {
    return create(ac, resFile, mdlName, anmName, bufferOption);
}

bool AnmMdlWrapper::create(
    dAcBase_c &ac, void *mdlFile, const char *mdlName, const char *anmName, m3d::mdl_c::mdlCallback_c *callback,
    u32 bufferOption
) {
    return create(ac, mdlFile, mdlFile, mdlName, anmName, callback, bufferOption);
}

bool AnmMdlWrapper::create2(
    dAcBase_c &ac, void *mdlFile, const char *mdlName, const char *anmName, m3d::mdl_c::mdlCallback_c *callback,
    u32 bufferOption
) {
    return create(ac, mdlFile, mdlName, anmName, callback, bufferOption);
}

void AnmMdlWrapper::play() {
    m3d::mdlAnmChr::play();
    syncAnmFrame();
}

void AnmMdlWrapper::setFrame(f32 frame) {
    m3d::mdlAnmChr::setFrame(frame);
    syncAnmFrame();
}

bool AnmMdlWrapper::setAnm(const char *name, m3d::playMode_e mode, f32 frame) {
    if (!m3d::mdlAnmChr::setAnm(name, mode, frame)) {
        return false;
    }
    return loadSounds(name);
}

void AnmMdlWrapper::setRate(f32 rate) {
    m3d::mdlAnmChr::setRate(rate);
    setSoundRate(rate);
}

void AnmMdlWrapper::setSoundSource(SoundSource *pSource) {
    mpSoundSource = pSource;
}

bool AnmMdlWrapper::loadSounds(const char *name) {
    if (mpSoundSource == nullptr || !mpSoundSource->isReadyMaybe()) {
        return true;
    }

    SizedString<64> resPath;
    resPath.sprintf("%s.brasd", name);
    mpSoundData = mAnmFile.GetExternalData(resPath);
    if (mpSoundData == nullptr) {
        mpSoundData = mMdlFile.GetExternalData(resPath);
    }
    mpSoundSource->load(mpSoundData, name);

    return true;
}

void AnmMdlWrapper::setSoundRate(f32 rate) {
    if (mpSoundSource != nullptr) {
        mpSoundSource->setRate(rate);
    }
}

void AnmMdlWrapper::syncAnmFrame() {
    if (mpSoundSource != nullptr) {
        mpSoundSource->setFrame(getAnm().getFrame());
    }
}

bool AnmMdlWrapper2::createExt(
    dAcBase_c &ac, void *mdlFile, void *anmFile, void *ext1, void *ext2, const char *mdlName, const char *anmName,
    m3d::mdl_c::mdlCallback_c *callback, u32 bufferOption
) {
    if (!create2(ac, mdlFile, anmFile, mdlName, anmName, callback, bufferOption)) {
        return false;
    }

    if (ext1 != nullptr) {
        mExt1 = nw4r::g3d::ResFile(ext1);
        mHasExt1 = true;
    }

    if (ext2 != nullptr) {
        mExt2 = nw4r::g3d::ResFile(ext2);
        mHasExt2 = true;
    }

    if (mHasExt1) {
        mExt1.Bind(mMdlFile);
    }

    if (mHasExt2) {
        mExt2.Bind(mMdlFile);
    }

    return true;
}

bool AnmMdlWrapper::create3(
    dAcBase_c &ac, void *mdlFile, void *anmFile, const char *mdlName, const char *anmName,
    m3d::mdl_c::mdlCallback_c *callback, u32 bufferOption
) {
    return create2(ac, mdlFile, anmFile, mdlName, anmName, callback, bufferOption);
}

bool AnmMdlWrapper2::setAnm(const char *anmFile, m3d::playMode_e mode, f32 frame) {
    nw4r::g3d::ResAnmChr res = mAnmFile.GetResAnmChr(anmFile);
    if (!res.IsValid()) {
        if (mHasExt1) {
            res = mExt1.GetResAnmChr(anmFile);
            if (!res.IsValid()) {
                if (mHasExt2) {
                    res = mExt2.GetResAnmChr(anmFile);
                    if (!res.IsValid()) {
                        res = mMdlFile.GetResAnmChr(anmFile);
                    }
                }
            }
        }
    }

    mAnm.setAnm(mMdl, res, mode);
    if (mMdl.setAnm(mAnm, frame)) {
        return loadSounds(anmFile);
    }
    return false;
}

extern "C" f32 lbl_80575150;
extern "C" f32 lbl_8057514C;
extern "C" f32 lbl_80576A0C;

void insertNode(SomeListNode *node, f32 a, f32 b) {
    node->field_0x04 = ((a * lbl_80575150) - 0.5f) + lbl_8057514C;
    node->field_0x06 = (lbl_80575150 - 0.5f) - b;
    sSomeList.append(node);
}

void clearList() {
    // TODO
    for (SomeList::Iterator it = sSomeList.GetBeginIter(); it != sSomeList.GetEndIter(); ++it) {
        SomeListNode *nd = &*it;
        SomeList::Iterator itCopy = sSomeList.GetPosition(nd);
        sSomeList.remove(nd);
        it = itCopy;
    }
}

// More screen / list stuff...

void unk2(nw4r::g3d::ResMat &mat, nw4r::g3d::ResMatTevColor &clr, LightingInfo *info) {
    BlurAndPaletteManager &pllt = BlurAndPaletteManager::GetInstance();
    const char *name = mat.GetName();
    if (name != nullptr && name[0] == 'M' && name[1] == 'A' && name[2] == '0') {
        char idx = name[3];
        if (idx == '0') {
            if (clr.IsValid()) {
                clr.GXSetTevKColor(GX_KCOLOR3, pllt.GetCurrentDefaultMcf().MA00_kColor3);
                clr.DCStore(false);
                if (info != nullptr) {
                    info->mTevK3Color = pllt.GetCurrentDefaultMcf().MA00_kColor3;
                    info->mUseTevK3 = true;
                }
            }
        } else if (idx == '1') {
            if (clr.IsValid()) {
                clr.GXSetTevColor(GX_TEVREG1, pllt.GetCurrentDefaultMcf().MA01_tevReg1);
                clr.GXSetTevKColor(GX_KCOLOR2, pllt.GetCurrentDefaultMcf().MA01_kColor2);
                clr.DCStore(false);
                if (info != nullptr) {
                    info->mTev1Color = pllt.GetCurrentDefaultMcf().MA01_tevReg1;
                    info->mUseTev1 = true;
                    info->mTevK2Color = pllt.GetCurrentDefaultMcf().MA01_kColor2;
                    info->mUseTevK2 = true;
                }
            }
        } else if (idx == '2') {
            if (clr.IsValid()) {
                clr.GXSetTevColor(GX_TEVREG1, pllt.GetCurrentDefaultMcf().MA02_tevReg1);
                clr.GXSetTevKColor(GX_KCOLOR2, pllt.GetCurrentDefaultMcf().MA02_kColor2);
                clr.DCStore(false);
                if (info != nullptr) {
                    info->mTev1Color = pllt.GetCurrentDefaultMcf().MA02_tevReg1;
                    info->mUseTev1 = true;
                    info->mTevK2Color = pllt.GetCurrentDefaultMcf().MA02_kColor2;
                    info->mUseTevK2 = true;
                }
            }
        } else if (idx == '3') {
            if (clr.IsValid()) {
                clr.GXSetTevKColor(GX_KCOLOR2, pllt.GetCurrentDefaultMcf().MA03_kColor3);
                clr.DCStore(false);
                if (info != nullptr) {
                    info->mTevK3Color = pllt.GetCurrentDefaultMcf().MA03_kColor3;
                    info->mUseTevK3 = true;
                }
            }
        }
        // TODO
    }
}

void setRoomTevColors(nw4r::g3d::ResMdl mdl, int b1, bool bUnk) {
    BlurAndPaletteManager &pllt = BlurAndPaletteManager::GetInstance();
    SpawnInfo &spawn = dScGame_c::currentSpawnInfo;

    for (u32 i = 0; i < mdl.GetResMatNumEntries(); i++) {
        nw4r::g3d::ResMat mat = mdl.GetResMat(i);
        nw4r::g3d::ResMatMisc misc = mat.GetResMatMisc();
        nw4r::g3d::ResMatTevColor clr = mat.GetResMatTevColor();

        unk2(mat, clr, nullptr);

        nw4r::g3d::ResMat mat2 = mdl.GetResMat(i);
        nw4r::g3d::ResTev tev = mat2.GetResTev();
        // Apply false colors in trials
        if (tev.IsValid() && spawn.getTrial() == SpawnInfo::TRIAL) {
            if (pllt.get0x2DE8() == 0) {
                if (!dTgSiren_c::isOutOfTime()) {
                    // blue -> green
                    tev.GXSetTevSwapModeTable(GX_TEV_SWAP0, GX_CH_RED, GX_CH_GREEN, GX_CH_GREEN, GX_CH_ALPHA);
                    tev.DCStore(false);
                } else {
                    // red -> green, blue -> red
                    tev.GXSetTevSwapModeTable(GX_TEV_SWAP0, GX_CH_GREEN, GX_CH_GREEN, GX_CH_RED, GX_CH_ALPHA);
                    tev.DCStore(false);
                }
            } else {
                // no change
                tev.GXSetTevSwapModeTable(GX_TEV_SWAP0, GX_CH_RED, GX_CH_GREEN, GX_CH_BLUE, GX_CH_ALPHA);
                tev.DCStore(false);
            }
        }

        if (b1 == 1) {
            misc.SetLightSetIdx(b1);
        }
        nw4r::g3d::ResMatChan chan = mat.GetResMatChan();
        chan.GXSetChanAmbColor(GX_COLOR0, mColor(0xFF, 0xFF, 0xFF, 0xFF));
    }
}

void UnkWithWater::calc(EGG::CpuTexture *tex) {
    GXTexObj obj;
    if (tex != nullptr) {
        tex->getTexObj(&obj);
    }

    for (NodeList::Iterator it = mList.GetBeginIter(); it != mList.GetEndIter(); ++it) {
        it->apply(&obj);
    }
}

bool UnkWithWater::init(nw4r::g3d::ResTexObj &obj, GXTexMapID mapId) {
    UnkWithWaterNode *nd = new UnkWithWaterNode(obj, mapId);
    if (nd == nullptr) {
        return false;
    }
    mList.append(nd);
    return true;
}

void UnkWithWater::remove() {
    // TODO register usage
    NodeList::Iterator nextIt;
    for (NodeList::Iterator it = mList.GetBeginIter(); it != mList.GetEndIter(); it = nextIt) {
        nextIt = it;
        ++nextIt;
        UnkWithWaterNode *nd = &*it;
        mList.remove(nd);
        delete nd;
    }
}

void UnkWithWaterNode::apply(const GXTexObj *obj) {
    GXTexObj *o = mObj.GetTexObj(mMapId);
    *o = *obj;
}

int EggTextureProc::entry() {
    field_0x50 = field_0x51;
    if (field_0x50) {
        field_0x51 = (f32)field_0x50 * ((f32)field_0x52 / (f32)(field_0x52 + 1));
        return m3d::scnLeaf_c::entry();
    } else {
        doRemove();
        return 0;
    }
}

void EggTextureProc::doDraw() {}

void EggTextureProc::doRemove() {}

bool EggTextureProc::create(int prioOpa, int prioXlu, mAllocator_c *alloc) {
    if (!m3d::proc_c::create(alloc, nullptr)) {
        return false;
    }
    if (prioOpa >= 0) {
        setPriorityDraw(prioOpa, 0);
        setOption(nw4r::g3d::ScnObj::OPTION_DISABLE_DRAW_XLU, true);
    } else {
        setPriorityDraw(0, prioXlu);
        setOption(nw4r::g3d::ScnObj::OPTION_DISABLE_DRAW_OPA, true);
    }
    return true;
}

bool EggTextureProc::createXlu(int prioXlu, mAllocator_c *alloc) {
    if (!m3d::proc_c::create(alloc, nullptr)) {
        return false;
    }

    setPriorityDraw(0, prioXlu);
    setOption(nw4r::g3d::ScnObj::OPTION_DISABLE_DRAW_OPA, true);

    return true;
}

void XluProc::drawXlu() {
    // TODO
}

} // namespace d3d
