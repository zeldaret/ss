#include <d/d_rawarchive.h>
#include <egg/gfx/eggLight.h>
#include <m/m3d/m3d.h>
#include <toBeSorted/arc_managers/oarc_manager.h>
#include <toBeSorted/arc_managers/current_stage_arc_manager.h>
#include <nw4r/g3d/g3d_resfile.h>
#include <nw4r/g3d/g3d_resmdl.h>
#include <nw4r/g3d/g3d_resmat.h>

ArcCallbackHandler ArcCallbackHandler::sInstance;

#define NAME_DZB  'dzb '
#define NAME_G3D  'g3d '
#define NAME_KCL  'kcl '
#define NAME_OARC 'oarc'
#define NAME_RARC 'rarc'

extern "C" void replaceModelTextures__Q23EGG19LightTextureManagerCFPQ34nw4r3g3d6ResMdl(void *, nw4r::g3d::ResMdl);
extern "C" void FUN_804a7260(nw4r::g3d::ResMdl, const char *prefix);


void BindSystemModelsAndLighting(nw4r::g3d::ResFile file) {
    nw4r::g3d::ResFile sysFile = OarcManager::sInstance->getMdlFromArc2("System");
    if (sysFile.mFile.IsValid()) {
        file.Bind(sysFile);
    }

    EGG::LightManager *mgr = m3d::getLightMgr(0);
    if (mgr != nullptr && mgr->GetTextureMgr() != nullptr) {
        void *lightTexMgr = mgr->GetTextureMgr();
        for (int i = 0; i < file.GetResMdlNumEntries(); i++) {
            nw4r::g3d::ResMdl mdl = file.GetResMdl(i);
            replaceModelTextures__Q23EGG19LightTextureManagerCFPQ34nw4r3g3d6ResMdl(lightTexMgr, mdl);
            FUN_804a7260(mdl, "Lm");
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

extern "C" void fn_8033A140(void *data);
extern "C" void dBgWKCol__initKCollision(void *dat);

void ArcCallbackHandlerBase::CreateArcEntry(void *data, const char *path) {
    if (mPrefix == NAME_G3D) {
        nw4r::g3d::ResFile file = data;
        file.Init();
        file.Bind();
        BindSystemModelsAndLighting(file);
    } else if (mPrefix == NAME_KCL) {
        dBgWKCol__initKCollision(data);
    } else if (mPrefix == NAME_DZB) {
        fn_8033A140(data);
    } else if (mPrefix == NAME_OARC) {
        SizedString<64> oarcPath = path;
        char buf[64];
        sscanf(&oarcPath, "/oarc/%31[^.]arc", buf);
        u32 oldPrefix = mPrefix;
        OarcManager::sInstance->addEntryFromSuperArc(buf, data, nullptr);
        mPrefix = oldPrefix;
    } else if (mPrefix == NAME_RARC) {
        SizedString<64> oarcPath = path;
        char buf[64];
        sscanf(&oarcPath, "/rarc/%31[^.]arc", buf);
        u32 oldPrefix = mPrefix;
        CurrentStageArcManager::sInstance->addEntryFromSuperArc(buf, data);
        mPrefix = oldPrefix;
    }
}

void ArcCallbackHandlerBase::DestroyArcEntry(const char *path) {
    if (mPrefix == NAME_OARC) {
        SizedString<64> oarcPath = path;
        char buf[64];
        sscanf(&oarcPath, "/oarc/%31[^.]arc", buf);
        u32 oldPrefix = mPrefix;
        OarcManager::sInstance->decrement(buf);
        mPrefix = oldPrefix;
    } else if (mPrefix == NAME_RARC) {
        SizedString<64> oarcPath = path;
        char buf[64];
        sscanf(&oarcPath, "/rarc/%31[^.]arc", buf);
        u32 oldPrefix = mPrefix;
        CurrentStageArcManager::sInstance->decrement(buf);
        mPrefix = oldPrefix;
    }
}
