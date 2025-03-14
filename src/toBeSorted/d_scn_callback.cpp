#include "common.h"
#include "d/a/obj/d_a_obj_base.h"
#include "d/d_bzs_types.h"
#include "m/m3d/m3d.h"
#include "nw4r/g3d/g3d_camera.h"
#include "nw4r/g3d/g3d_scnmdl.h"
#include "nw4r/g3d/g3d_scnmdlsmpl.h"
#include "nw4r/g3d/g3d_scnobj.h"
#include "nw4r/g3d/res/g3d_resmat.h"
#include "nw4r/g3d/res/g3d_resmdl.h"
#include "nw4r/g3d/res/g3d_respltt.h"
#include "rvl/GX/GXTypes.h"
#include "sized_string.h"
#include "toBeSorted/d_d3d.h"
#include "toBeSorted/stage_render_stuff.h"

void dScnCallback_c::ExecCallback_CALC_MAT(nw4r::g3d::ScnObj::Timing, nw4r::g3d::ScnObj *obj, u32 param, void *pObj) {
    const LightingInfo &info = mpActor->mLightingInfo;
    mColor tev0 = info.mTev0Color;
    mColor tev1 = info.mTev1Color;
    mColor tev2 = info.mTev2Color;
    mColor tevPrev = info.mTevPrevColor;
    mColor tevK0 = info.mTevK0Color;
    mColor tevK1 = info.mTevK1Color;
    mColor tevK2 = info.mTevK2Color;
    mColor tevK3 = info.mTevK3Color;

    bool bTevK3 = false;
    bool bTev0 = info.mUseTev0;
    bool bTev1 = info.mUseTev1;
    bool bTev2 = info.mUseTev2;
    bool bTevPrev = info.mUseTevPrev;
    bool bTevK0 = info.mUseTevK0;
    bool bTevK1 = info.mUseTevK1;
    bool bTevK2 = info.mUseTevK2;
    if (!field_0x08 && info.mUseTevK3) {
        bTevK3 = true;
    }

    nw4r::g3d::ScnMdl *pMdl = nw4r::g3d::ScnObj::DynamicCast<nw4r::g3d::ScnMdl>(obj);
    if (pMdl != nullptr) {
        nw4r::g3d::ResMdl resMdl = pMdl->GetResMdl();
        for (u32 i = 0; i < resMdl.GetResMatNumEntries(); i++) {
            nw4r::g3d::ScnMdl::CopiedMatAccess mat(pMdl, i);
            nw4r::g3d::ResMatTevColor clr = mat.GetResMatTevColor(false);
            if (bTev0) {
                clr.GXSetTevColor(GX_TEVREG0, tev0);
            }
            if (bTevK0) {
                clr.GXSetTevKColor(GX_KCOLOR0, tevK0);
            }
            if (bTevK3) {
                clr.GXSetTevKColor(GX_KCOLOR3, tevK3);
            }
            if (bTev1) {
                clr.GXSetTevColor(GX_TEVREG1, tev1);
            }
            if (bTev2) {
                clr.GXSetTevColor(GX_TEVREG2, tev2);
            }
            if (bTevPrev) {
                clr.GXSetTevColor(GX_TEVPREV, tevPrev);
            }
            if (bTevK1) {
                clr.GXSetTevKColor(GX_KCOLOR1, tevK1);
            }
            if (bTevK2) {
                clr.GXSetTevKColor(GX_KCOLOR2, tevK2);
            }
            clr.DCStore(false);
        }
        return;
    }

    nw4r::g3d::ScnMdlSimple *pMdlSimple = nw4r::g3d::ScnObj::DynamicCast<nw4r::g3d::ScnMdlSimple>(obj);

    nw4r::g3d::ResMdl resMdl = pMdlSimple->GetResMdl();
    for (u32 i = 0; i < resMdl.GetResMatNumEntries(); i++) {
        nw4r::g3d::ResMat mat = resMdl.GetResMat(i);
        nw4r::g3d::ResMatTevColor clr = mat.GetResMatTevColor();
        if (bTev0) {
            clr.GXSetTevColor(GX_TEVREG0, tev0);
        }
        if (bTevK0) {
            clr.GXSetTevKColor(GX_KCOLOR0, tevK0);
        }
        if (bTevK3) {
            clr.GXSetTevKColor(GX_KCOLOR3, tevK3);
        }
        if (bTev1) {
            clr.GXSetTevColor(GX_TEVREG1, tev1);
        }
        if (bTev2) {
            clr.GXSetTevColor(GX_TEVREG2, tev2);
        }
        if (bTevPrev) {
            clr.GXSetTevColor(GX_TEVPREV, tevPrev);
        }
        if (bTevK1) {
            clr.GXSetTevKColor(GX_KCOLOR1, tevK1);
        }
        if (bTevK2) {
            clr.GXSetTevKColor(GX_KCOLOR2, tevK2);
        }
        clr.DCStore(false);
    }
}

void dScnCallback_c::attach(m3d::scnLeaf_c &leaf) {
    leaf.swapCallback(this);
    leaf.enableCallbackTiming(nw4r::g3d::ScnObj::CALLBACK_TIMING_C);
    leaf.enableCallbackOp(nw4r::g3d::ScnObj::EXECOP_CALC_MAT);
}

void setPCAMPos2(PCAM *pcam) {
    static nw4r::g3d::Camera::PostureInfo sPosture = {
        nw4r::g3d::Camera::POSTURE_AIM,
        nw4r::math::VEC3(0.0f, 0.0f, 0.0f),
        nw4r::math::VEC3(0.0f, 0.0f, 0.0f),
        nw4r::math::VEC3(0.0f, 0.0f, 0.0f),
        0.0f,
    };

    nw4r::g3d::Camera cam = m3d::getCamera(pcam->id);
    cam.SetPosition(pcam->position1.x, pcam->position1.y, pcam->position1.z);
    // TODO
    sPosture.cameraTarget = pcam->position2;
    cam.SetPosture(sPosture);
    cam.SetOrtho(pcam->field_0x1C, -pcam->field_0x1C, -pcam->field_0x1C, pcam->field_0x1C, 1.0f, 1000000.0f);
    cam.SetTexMtxParam(0.5f, 0.5f, 0.5f, 0.5f);
}

// TODO
bool d3d::UnkWithWater::linkMdl(nw4r::g3d::ResMdl &mdl, d3d::UnkWithWater *thing) {
    bool result = false;
    
    for (u32 i = 0; i < mdl.GetResMatNumEntries(); i++) {
        nw4r::g3d::ResMat mat = mdl.GetResMat(i);
        nw4r::g3d::ResTexSrt srt = mat.GetResTexSrt();

        for (u32 j = 0; j < mat.GetNumResTexPlttInfo(); j++) {
            nw4r::g3d::ResTexPlttInfo paletteInfo = mat.GetResTexPlttInfo(j);
            const char *name = paletteInfo.GetTexName();
            if (strequals(name, "DummyWater") || strequals(name, "TimeDoorB_Dummy")) {
                nw4r::g3d::ResTexObj obj = mat.GetResTexObj();
                thing->init(obj, paletteInfo.ptr()->mapID);
                
                u32 mode;
                int camRef;
                int lightRef;
                srt.GetMapMode(j, &mode, &camRef, &lightRef);
                srt.SetMapMode(j, mode, 0, lightRef);
                result = true;
            }
        }
    }

    return result;
}
