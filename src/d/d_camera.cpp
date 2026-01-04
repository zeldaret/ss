#include "d/d_camera.h"

#include "common.h"
#include "d/a/d_a_player.h"
#include "d/col/bg/d_bg_s_gnd_chk.h"
#include "d/col/bg/d_bg_s_roof_chk.h"
#include "d/d_gfx.h"
#include "d/d_pad.h"
#include "d/d_sc_game.h"
#include "d/d_stage_mgr.h"
#include "egg/gfx/eggFrustum.h"
#include "f/f_base.h"
#include "f/f_profile.h"
#include "m/m_mtx.h"
#include "m/m_vec.h"
#include "toBeSorted/d_camera_math.h"
#include "toBeSorted/event_manager.h"

class dCamera_HIO_c {
public:
    dCamera_HIO_c();
    virtual ~dCamera_HIO_c();

    /* 0x04 */ f32 field_0x04;
    /* 0x08 */ f32 field_0x08;
    /* 0x0C */ f32 field_0x0C;
    /* 0x10 */ s16 field_0x10;
    /* 0x12 */ s16 field_0x12;
    /* 0x14 */ f32 field_0x14;
    /* 0x18 */ u16 field_0x18;
    /* 0x1C */ f32 field_0x1C;
    /* 0x20 */ f32 field_0x20;
    /* 0x24 */ s16 field_0x24;
    /* 0x28 */ f32 field_0x28;
    /* 0x2C */ f32 field_0x2C;
    /* 0x30 */ s16 field_0x30;
};

dCamera_HIO_c::~dCamera_HIO_c() {}
dCamera_HIO_c::dCamera_HIO_c() {
    field_0x04 = 1.0f;
    field_0x08 = 240000.0f;
    field_0x14 = 0.1f;
    field_0x18 = 0x1555;
    field_0x1C = 30.0f;
    field_0x20 = 30.0f;
    field_0x24 = 8;
    field_0x28 = 0.7f;
    field_0x2C = 0.2f;
    field_0x30 = 0;
    field_0x0C = 1.0f;
    field_0x10 = 1;
    field_0x12 = 1;
}

static dCamera_HIO_c sHio;

SPECIAL_BASE_PROFILE(CAMERA, dCamera_c, fProfile::CAMERA, 0xA, 0x30);

void debugWarn2(const char *fmt, ...);
void debugPrintf7(const char *fmt, ...);

dCamera_c::dCamera_c()
    : mMyCameraIndex(0),
      mpScreen(dStageMgr_c::GetInstance()->getScreen(0)),
      field_0x098(0),
      field_0x09A(0),
      field_0xD94(0.0f),
      field_0x1F8(0),
      field_0x1FC(0),
      mFlags(0) {
    field_0xDAC = this;
    mScreenShakeIntensity = 0.0f;
    field_0xDB4.set(mVec3_c::Zero.x, mVec3_c::Zero.y, mVec3_c::Zero.z);
    field_0xDC0.setCamera(this);
    field_0x08C = mVec3_c(0.0f, 1.0f, 0.0f);

    mActiveCameraIdx = 0;
    for (int i = 0; i < CAM_MAX; i++) {
        mpCameras[i] = nullptr;
    }
}

int dCamera_c::create() {
    mFlags = 0;
    mMyCameraIndex = getFromParams(0, 0xF);
    dScGame_c::setCamera(mMyCameraIndex, nullptr);
    mpCameras[0] = &mGameCam1;
    mpCameras[1] = &mGameCam2;
    mpCameras[2] = &mEventCam;
    mpCameras[3] = &mMapCam;

    for (int i = 0; i < CAM_MAX; i++) {
        if (mpCameras[i] == nullptr) {
            continue;
        }
        if (!mpCameras[i]->doCreate(i)) {
            return NOT_READY;
        }
    }

    debugWarn2("u can NOT use debug camera!!"); // aww
    mActiveCameraIdx = 0;

    mpCameras[0]->vt_0x28();

    mLetterboxAmount = fn_8019E1F0();
    field_0x290 = 1.0f;
    mFlags |= CAM_FLAGS_0x80;

    // TODO maybe struct + inline
    mGlobalAlpha = 1.0f;
    field_0x298 = field_0x299 = 0;

    field_0x29C = 1.0f;
    field_0x2A0 = field_0x2A1 = 0;

    mView1 = mpCameras[mActiveCameraIdx]->getView();

    field_0xDCC = 0;
    field_0x098 = 0;
    field_0x09A = 0;

    dScGame_c::setCamera(mMyCameraIndex, this);
    fn_8019DCE0();
    fn_8019DE70();

    const STIF *stif = dStageMgr_c::GetInstance()->getStif();
    if (stif != nullptr) {
        sHio.field_0x04 = stif->field_0x00;
        sHio.field_0x08 = stif->field_0x04;
    }

    field_0x1FC = 0;
    if ((dScGame_c::isCurrentStage("F000") && 26 <= dScGame_c::currentSpawnInfo.layer &&
         dScGame_c::currentSpawnInfo.layer <= 28) ||
        dScGame_c::isInCredits() || dScGame_c::isSeekerStoneStageAndLayer()) {
        mFlags |= 0x200;
        dScGame_c::GetInstance()->setTargetingScreenLetterboxAmount(0.0f);
    }

    debugPrintf7("create ok!!");

    return SUCCEEDED;
}

// Maybe inlines that couldn't be inlined due to variadics...
void debugWarn2(const char *fmt, ...) {
    // no-op
}

void debugPrintf7(const char *fmt, ...) {
    // no-op
}

int dCamera_c::doDelete() {
    debugPrintf7("delete!!");
    for (int i = 0; i < CAM_MAX; i++) {
        // No null check needed :)
        mpCameras[i]->doRemove();
    }
    dScGame_c::setCamera(mMyCameraIndex, nullptr);
    return SUCCEEDED;
}

int dCamera_c::execute() {
    dAcPy_c *link = dAcPy_c::GetLinkM();

    fn_8019DB80();

    for (int i = 0; i < CAM_MAX; i++) {
        if (mpCameras[i] != nullptr) {
            mpCameras[i]->doExecute();
        }
    }

    if (mActiveCameraIdx == CAM_EVENT) {
        // hmmmm
        static_cast<dCameraGame_c *>(mpCameras[0])->clearCamIds();
    }

    fn_8019DCE0();
    fn_8019DE70();
    field_0xDC0.fn_8019E940();

    field_0xDCC = 0;
    if (mActiveCameraIdx == CAM_EVENT) {
        mFlags |= CAM_FLAGS_0x80;
    }

    if ((mFlags & (CAM_FLAGS_0x100 | CAM_FLAGS_0x80)) != 0) {
        field_0x290 += (1.0f / sHio.field_0x24);
        if (field_0x290 > 1.0f) {
            field_0x290 = 1.0f;
        }
    } else {
        field_0x290 = field_0x290 - (1.0f / sHio.field_0x24);
        if (field_0x290 < 0.0f) {
            field_0x290 = 0.0f;
        }
    }

    f32 scale = (mFlags & CAM_FLAGS_0x80) != 0 ? fn_8019E1F0() : sHio.field_0x20;
    f32 target = camEaseInOut(field_0x290, 1.0f);
    mLetterboxAmount += (target * scale - mLetterboxAmount) * 0.5f;

    if (mActiveCameraIdx == CAM_EVENT && (mFlags & CAM_FLAGS_0x200) != 0) {
        mLetterboxAmount = 0.0f;
    }

    dScGame_c::GetInstance()->setTargetingScreenLetterboxAmount(mLetterboxAmount);
    mFlags &= ~(CAM_FLAGS_0x100 | CAM_FLAGS_0x80);

    if (!link->checkActionFlagsCont(0x400000)) {
        field_0x299 = 0;
    }

    f32 alphaTarget;
    f32 alphaUpdateSpeed = sHio.field_0x2C;
    if (EventManager::isInEvent()) {
        alphaUpdateSpeed = 1.0f;
        if (field_0x2A1 != 0) {
            alphaTarget = field_0x2A0 ? sHio.field_0x28 : 0.0f;
        } else {
            alphaTarget = 1.0f;
        }
    } else {
        if (field_0x299 != 0) {
            alphaTarget = field_0x298 ? sHio.field_0x28 : 0.0f;
        } else {
            alphaTarget = 1.0f;
        }
    }

    mGlobalAlpha += alphaUpdateSpeed * (alphaTarget - mGlobalAlpha);
    dStageMgr_c::GetInstance()->setGlobalAlpha((u8)(mGlobalAlpha * 255.1f) & 0xFF);

    return SUCCEEDED;
}

int dCamera_c::draw() {
    if (mpCameras[mActiveCameraIdx] != nullptr) {
        mpCameras[mActiveCameraIdx]->doDraw();
    }
    return SUCCEEDED;
}

bool dCamera_c::isUnderwater() const {
    return mFlags & CAM_FLAGS_0x40;
}

void dCamera_c::setFrustum(f32 fov, f32 near, f32 far) {
    if (dGfx_c::isTvMode4To3()) {
        mpScreen->Reset(
            0.0f, dGfx_c::getEFBHeightDifferenceF(), dGfx_c::getCurrentScreenWidthF(), dGfx_c::getHeightScaledF(),
            mpScreen->GetCanvasMode()
        );
    } else {
        mpScreen->Reset(
            0.0f, 0.0f, dGfx_c::getCurrentScreenWidthF(), dGfx_c::getCurrentScreenHeightF(), mpScreen->GetCanvasMode()
        );
        if (dGfx_c::isTvMode4To3()) {
            // Unreachable...
            fov *= sHio.field_0x0C;
        }
    }

    mpScreen->SetProjectionType(EGG::Frustum::PROJ_PERSP);
    mpScreen->SetNearZ(near);
    mpScreen->SetFarZ(far);
    mpScreen->SetFovy(fov);

    // Possible fakematch: Either the internals of EGG::Matrix34f allow
    // copying the data via compiler-generated struct copies (lwz/stw),
    // or this is really what they did. NSMBW symbols confirm that this function
    // takes mMtx_c while it's clear that EGG cameras return EGG matrices.
    mMtx_c mtx = *(mMtx_c *)&mLookAtCamera.getViewMatrix();
    mFrustum.set(fov, mpScreen->GetAspect(), near, far, mtx);
}

void dCamera_c::updateUnderwaterDepth(const mVec3_c &pos) {
    mVec3_c chkPos = pos;
    dBgS_RoofChk roofChk;
    roofChk.SetPos(&chkPos);
    roofChk.SetUnderwaterRoof();
    roofChk.SetField_0x7C(1);

    f32 f = dBgS::GetInstance()->RoofChk(&roofChk);
    if (f != 1e9f) {
        chkPos.y = f;
    } else {
        chkPos.y += 10000.0f;
    }

    dBgS_ObjGndChk_Wtr wtrChk;
    wtrChk.SetPos(chkPos);
    f32 waterHeight = dBgS::GetInstance()->GroundCross(&wtrChk);
    if (waterHeight != 1e9f) {
        mIsUnderwater = waterHeight > pos.y;
        mWaterHeight = waterHeight;
    } else {
        mIsUnderwater = false;
        mWaterHeight = -1e9f;
    }

    if (mIsUnderwater) {
        mUnderwaterDepth = waterHeight - pos.y;
    } else {
        mUnderwaterDepth = 0.0f;
    }
}

bool dCamera_c::isUnderwater_() const {
    return mIsUnderwater;
}

f32 dCamera_c::getUnderwaterDepth() const {
    return mUnderwaterDepth;
}

void dCamera_c::substruct_1::setCamera(dCamera_c *cam) {
    mpCamera = cam;
}

void dCamera_c::substruct_1::fn_8019E890() {
    fn_8019E8D0(mpCamera->fn_8019E3B0());
}

void dCamera_c::substruct_1::fn_8019E8D0(const mAng &ang) {
    if (!mActive) {
        mActive = true;
        field_0x04 = ang;
        mFSStickAngle = dPad::getFSStickAngle();
    }
}

mAng dCamera_c::substruct_1::fn_8019E930() {
    return field_0x04;
}

void dCamera_c::substruct_1::fn_8019E940() {
    if (mActive) {
        mAng angle = mFSStickAngle - dPad::getFSStickAngle();
        f32 distance = dPad::getFSStickDistance();
        if (distance < sHio.field_0x14 || mAng::abs(angle) > sHio.field_0x18) {
            mActive = false;
        }
    }

    if (!mActive) {
        field_0x04 = mpCamera->fn_8019E3B0();
        mFSStickAngle = dPad::getFSStickAngle();
    }
}
