#include "d/d_camera.h"

#include "c/c_math.h"
#include "common.h"
#include "d/a/d_a_player.h"
#include "d/col/bg/d_bg_s_gnd_chk.h"
#include "d/col/bg/d_bg_s_lin_chk.h"
#include "d/col/bg/d_bg_s_roof_chk.h"
#include "d/d_gfx.h"
#include "d/d_pad.h"
#include "d/d_sc_game.h"
#include "d/d_stage_mgr.h"
#include "d/snd/d_snd_3d_manager.h"
#include "egg/gfx/eggFrustum.h"
#include "egg/math/eggMatrix.h"
#include "f/f_base.h"
#include "f/f_profile.h"
#include "m/m_angle.h"
#include "m/m_mtx.h"
#include "m/m_vec.h"
#include "nw4r/g3d/g3d_camera.h"
#include "toBeSorted/d_camera_base.h"
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
      mYAngle(0),
      mXZAngle(0),
      field_0xD94(0.0f),
      field_0x1F8(0),
      field_0x1FC(0),
      mFlags(0) {
    mScreenShaker.init(this);
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

    mpCameras[0]->activate();

    mLetterboxAmount = fn_8019E1F0();
    field_0x290 = 1.0f;
    mFlags |= CAM_FLAGS_IN_EVENT;

    // TODO maybe struct + inline
    mGlobalAlpha = 1.0f;
    field_0x298 = field_0x299 = 0;

    field_0x29C = 1.0f;
    field_0x2A0 = field_0x2A1 = 0;

    mView1 = mpCameras[mActiveCameraIdx]->getView();

    field_0xDCC = false;
    mYAngle = 0;
    mXZAngle = 0;

    dScGame_c::setCamera(mMyCameraIndex, this);
    updateView();
    apply();

    const STIF *stif = dStageMgr_c::GetInstance()->getStif();
    if (stif != nullptr) {
        sHio.field_0x04 = stif->field_0x00;
        sHio.field_0x08 = stif->field_0x04;
    }

    field_0x1FC = 0;
    if ((dScGame_c::isCurrentStage("F000") && 26 <= dScGame_c::currentSpawnInfo.layer &&
         dScGame_c::currentSpawnInfo.layer <= 28) ||
        dScGame_c::isInCredits() || dScGame_c::isSeekerStoneStageAndLayer()) {
        mFlags |= CAM_FLAGS_NO_LETTERBOX_IN_EVENT;
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

void dCamera_c::setWorldOffset(f32 x, f32 z) {
    mView.mPosition.x += x;
    mView.mPosition.z += z;
    mView.mTarget.x += x;
    mView.mTarget.z += z;
    // TODO maybe inline
    getGameCam1()->setView(mView);
    getGameCam1()->onFlag(0x1);
    getGameCam1()->onFlag(0x4000);
    updateView();
    apply();
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

    updateView();
    apply();
    field_0xDC0.fn_8019E940();

    field_0xDCC = false;
    if (mActiveCameraIdx == CAM_EVENT) {
        mFlags |= CAM_FLAGS_IN_EVENT;
    }

    if ((mFlags & (CAM_FLAGS_0x100 | CAM_FLAGS_IN_EVENT)) != 0) {
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

    f32 scale = (mFlags & CAM_FLAGS_IN_EVENT) != 0 ? fn_8019E1F0() : sHio.field_0x20;
    f32 target = camEaseInOut(field_0x290, 1.0f);
    mLetterboxAmount += (target * scale - mLetterboxAmount) * 0.5f;

    if (mActiveCameraIdx == CAM_EVENT && (mFlags & CAM_FLAGS_NO_LETTERBOX_IN_EVENT) != 0) {
        mLetterboxAmount = 0.0f;
    }

    dScGame_c::GetInstance()->setTargetingScreenLetterboxAmount(mLetterboxAmount);
    mFlags &= ~(CAM_FLAGS_0x100 | CAM_FLAGS_IN_EVENT);

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

void dCamera_c::updateView() {
    if (field_0xDCC) {
        mView = mView1;
        field_0xDCC = 0;
    } else {
        mView = mpCameras[mActiveCameraIdx]->getView();
    }

    mVec3_c dir = mView.mTarget - mView.mPosition;
    mYAngle = cM::atan2s(dir.y, dir.absXZ());
    mXZAngle = cM::atan2s(dir.x, dir.z);
}

int dCamera_c::draw() {
    if (mpCameras[mActiveCameraIdx] != nullptr) {
        mpCameras[mActiveCameraIdx]->doDraw();
    }
    return SUCCEEDED;
}

bool dCamera_c::isUnderwater() const {
    return mFlags & CAM_FLAGS_UNDERWATER;
}

void dCamera_c::apply() {
    updateUnderwaterDepth(mView.mPosition);
    if (isUnderwater_()) {
        mFlags |= CAM_FLAGS_UNDERWATER;
    } else {
        mFlags &= ~CAM_FLAGS_UNDERWATER;
    }

    if (mScreenShaker.execute()) {
        mView.mPosition += mScreenShaker.getShakeOffset();
        mView.mTarget += mScreenShaker.getShakeOffset();
    }

    mLookAtCamera.mPos = mView.mPosition;
    mLookAtCamera.mAt = mView.mTarget;
    if (mLookAtCamera.mAt.x == mLookAtCamera.mPos.x && mLookAtCamera.mAt.z == mLookAtCamera.mPos.z) {
        mLookAtCamera.mPos.z += 1.0f;
    }
    mLookAtCamera.mUp = mVec3_c::Ey;
    mLookAtCamera.doUpdateMatrix();
    mMtx.copyFrom(mLookAtCamera.getViewMatrix());
    // TODO: Maybe mutating mtx wasn't intended here but it's probably unproblematic
    mMtx.inverse();
    mMtxInv = mMtx;
    mMtxInv.m[0][3] = 0.0f;
    mMtxInv.m[1][3] = 0.0f;
    mMtxInv.m[2][3] = 0.0f;

    dSnd3DManager_c::GetInstance()->updateFromCamera(mLookAtCamera);
    applyTilt();
    setFrustum(mView.mFov, sHio.field_0x04, sHio.field_0x08);
    nw4r::g3d::Camera cam = dStageMgr_c::GetInstance()->getCamera(mMyCameraIndex);
    mpScreen->CopyToG3D(cam);
    mLookAtCamera.setG3DCamera(cam);
}

s32 dCamera_c::setActiveCamera(s32 newCamIdx) {
    if (mpCameras[newCamIdx] == nullptr) {
        return mActiveCameraIdx;
    } else if (mActiveCameraIdx == newCamIdx) {
        return newCamIdx;
    } else {
        for (int i = 0; i < CAM_MAX; i++) {
            mpCameras[i]->deactivate();
        }
        mpCameras[newCamIdx]->activate();
        debugPrintf7("unit %d -> %d", mActiveCameraIdx, newCamIdx);
        mActiveCameraIdx = newCamIdx;
        return newCamIdx;
    }

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

mAng dCamera_c::getYAngle() const {
    return mYAngle;
}

mAng dCamera_c::getYRot() const {
    return field_0xDC0.fn_8019E930();
}

mAng dCamera_c::getXZAngle() const {
    return mXZAngle;
}

void dCamera_c::applyTilt() {
    EGG::Matrix34f &mtx = mLookAtCamera.getViewMatrix();
    mMtx_c rotMtx;
    rotMtx.ZrotS(mAng::fromDeg(-mView.mTilt));
    MTXConcat(rotMtx, mtx.m, mtx.m);
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

bool dCamera_c::screen_shaker::execute() {
    mShakeOffset = mVec3_c::Zero;
    bool ret = false;
    if (mScreenShakeIntensity > 0.001f) {
        static s32 upOrDown = 0;
        upOrDown = upOrDown ^ 1;

        f32 f;
        if (upOrDown) {
            f = 400.0f;
        } else {
            f = -400.0f;
        }

        mVec3_c shakeOffset(cM::rndFX(100.0f), cM::rndF(f), 0.0f);
        shakeOffset.normalize();
        // Note: the difference in coordinates here is due to the camera view using a different
        // coordinate convention than game code.
        shakeOffset.rotX(mpCamera->getYAngle());
        shakeOffset.rotY(mpCamera->getXZAngle());

        mVec3_c chckVector = shakeOffset * (mScreenShakeIntensity + 5.0f);
        shakeOffset *= mScreenShakeIntensity;
        mVec3_c pos = mpCamera->getPosition();
        mVec3_c dest = pos + chckVector;

        dBgS_CamLinChk chk;
        chk.Set(&pos, &dest, nullptr);

        if (!dBgS::GetInstance()->LineCross(&chk)) {
            mShakeOffset = shakeOffset;
            ret = true;
        }
    }
    mScreenShakeIntensity = 0.0f;
    return ret;
}

mVec3_c dCamera_c::screen_shaker::getShakeOffset() const {
    return mShakeOffset;
}

void dCamera_c::substruct_1::setCamera(dCamera_c *cam) {
    mpCamera = cam;
}

void dCamera_c::substruct_1::fn_8019E890() {
    fn_8019E8D0(mpCamera->getXZAngle());
}

void dCamera_c::substruct_1::fn_8019E8D0(const mAng &ang) {
    if (!mActive) {
        mActive = true;
        field_0x04 = ang;
        mFSStickAngle = dPad::getFSStickAngle();
    }
}

mAng dCamera_c::substruct_1::fn_8019E930() const {
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
        field_0x04 = mpCamera->getXZAngle();
        mFSStickAngle = dPad::getFSStickAngle();
    }
}

bool dCamera_c::fn_8019EA70(bool b) {
    CamView view = getEventCam()->getView();
    mVec3_c dest;

    dest = camGetPointOnLine(view.mTarget, view.mPosition, dAcPy_c::GetLinkM()->mPositionCopy3);
    view.mTarget = dest;
    // TODO maybe inline
    getGameCam1()->setView(view);
    getGameCam1()->onFlag(0x1);
    if (b) {
        getGameCam1()->onFlag(0x4000);
    } else {
        getGameCam1()->offFlag(0x200);
    }

    return true;
}
