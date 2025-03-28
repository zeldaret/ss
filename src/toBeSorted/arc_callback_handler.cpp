#include "d/col/bg/d_bg_s.h"
#include "d/col/bg/d_bg_w_kcol.h"
#include "d/d_rawarchive.h"
#include "egg/gfx/eggG3DUtility.h"
#include "egg/gfx/eggLightTextureMgr.h"
#include "egg/gfx/eggLightManager.h"
#include "m/m3d/m3d.h"
#include "nw4r/g3d.h" // IWYU pragma: export
#include "toBeSorted/arc_managers/current_stage_arc_manager.h"
#include "toBeSorted/arc_managers/oarc_manager.h"

ArcCallbackHandler ArcCallbackHandler::sInstance;

#define NAME_DZB 'dzb '
#define NAME_G3D 'g3d '
#define NAME_KCL 'kcl '
#define NAME_OARC 'oarc'
#define NAME_RARC 'rarc'

void BindSystemModelsAndLighting(nw4r::g3d::ResFile file) {
    nw4r::g3d::ResFile sysFile(OarcManager::GetInstance()->getMdlFromArc2("System"));
    if (sysFile.IsValid()) {
        file.Bind(sysFile);
    }

    EGG::LightManager *mgr = m3d::getLightMgr(0);
    if (mgr != nullptr && mgr->GetTextureMgr() != nullptr) {
        EGG::LightTextureManager *lightTexMgr = mgr->GetTextureMgr();
        for (int i = 0; i < file.GetResMdlNumEntries(); i++) {
            nw4r::g3d::ResMdl mdl = file.GetResMdl(i);
            lightTexMgr->replaceModelTextures(mdl);
            EGG::G3DUtility::ApplyLightMat(mdl, "Lm");
            for (int j = 0; j < mdl.GetResMatNumEntries(); j++) {
                nw4r::g3d::ResMat mat = mdl.GetResMat(j);
                if (mat.IsOpaque()) {
                    nw4r::g3d::ResMatPix pix = mat.GetResMatPix();
                    u8 bAlpha;
                    u8 uAlpha;
                    pix.GXGetDstAlpha(&bAlpha, &uAlpha);
                    if (!bAlpha) {
                        pix.GXSetDstAlpha(1, 0x80);
                        pix.DCStore(0);
                    }
                }
            }
        }
    }
}

void ArcCallbackHandlerBase::CreateArcEntry(void *data, const char *path) {
    if (mPrefix == NAME_G3D) {
        nw4r::g3d::ResFile file(data);
        file.Init();
        file.Bind();
        BindSystemModelsAndLighting(file);
    } else if (mPrefix == NAME_KCL) {
        dBgWKCol::initKCollision(data);
    } else if (mPrefix == NAME_DZB) {
        cBgS::ConvDzb(data);
    } else if (mPrefix == NAME_OARC) {
        SizedString<64> oarcPath = path;
        char buf[64];
        sscanf(oarcPath, "/oarc/%31[^.]arc", buf);
        u32 oldPrefix = mPrefix;
        OarcManager::GetInstance()->addEntryFromSuperArc(buf, data, nullptr);
        mPrefix = oldPrefix;
    } else if (mPrefix == NAME_RARC) {
        SizedString<64> oarcPath = path;
        char buf[64];
        sscanf(oarcPath, "/rarc/%31[^.]arc", buf);
        u32 oldPrefix = mPrefix;
        CurrentStageArcManager::GetInstance()->addEntryFromSuperArc(buf, data);
        mPrefix = oldPrefix;
    }
}

void ArcCallbackHandlerBase::DestroyArcEntry(const char *path) {
    if (mPrefix == NAME_OARC) {
        SizedString<64> oarcPath = path;
        char buf[64];
        sscanf(oarcPath, "/oarc/%31[^.]arc", buf);
        u32 oldPrefix = mPrefix;
        OarcManager::GetInstance()->decrement(buf);
        mPrefix = oldPrefix;
    } else if (mPrefix == NAME_RARC) {
        SizedString<64> oarcPath = path;
        char buf[64];
        sscanf(oarcPath, "/rarc/%31[^.]arc", buf);
        u32 oldPrefix = mPrefix;
        CurrentStageArcManager::GetInstance()->decrement(buf);
        mPrefix = oldPrefix;
    }
}
