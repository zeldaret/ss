#include "toBeSorted/d_camera_map.h"

#include "common.h"
#include "d/a/d_a_player.h"
#include "d/a/obj/d_a_obj_base.h"
#include "d/col/bg/d_bg_s.h"
#include "d/col/bg/d_bg_s_lin_chk.h"
#include "d/col/cc/d_cc_s.h"
#include "d/d_sc_game.h"
#include "d/lyt/d_lyt_map.h"
#include "m/m_vec.h"
#include "toBeSorted/d_camera_math.h"
#include "toBeSorted/deg_angle_util.h"

class dCameraMap_HIO_c {
public:
    dCameraMap_HIO_c();
    virtual ~dCameraMap_HIO_c();

    /* 0x04 */ f32 field_0x04;
    /* 0x08 */ f32 field_0x08;
    /* 0x0C */ f32 field_0x0C;
    /* 0x10 */ f32 field_0x10;
    /* 0x14 */ f32 field_0x14;
    /* 0x18 */ f32 field_0x18;
    /* 0x1C */ f32 field_0x1C;
    /* 0x20 */ f32 field_0x20;
    /* 0x24 */ f32 field_0x24;
    /* 0x28 */ s16 field_0x28;
    /* 0x2C */ f32 field_0x2C;
    /* 0x30 */ f32 field_0x30;
    /* 0x34 */ f32 field_0x34;
    /* 0x38 */ u8 _0x38[0x3C - 0x38];
    /* 0x3C */ u16 field_0x3C;
    /* 0x3E */ u16 field_0x3E;
};

dCameraMap_HIO_c::~dCameraMap_HIO_c() {}
dCameraMap_HIO_c::dCameraMap_HIO_c() {
    field_0x04 = 2000.0f;
    field_0x08 = 1000.0f;
    field_0x0C = 0.6f;
    field_0x10 = 0.75f;
    field_0x18 = 10000.0f;
    field_0x14 = 2500.0f;
    field_0x20 = 89.0f;
    field_0x1C = 3.0f;
    field_0x24 = 90.0f;
    field_0x28 = 5;
    field_0x3E = 2;
    field_0x2C = 0.7f;
    field_0x30 = 0.2f;
    field_0x34 = 0.9f;
    field_0x3C = 0;
}

static dCameraMap_HIO_c sHio;

mVec3_c fn_800929C0(const mVec3_c &p2, const mVec3_c &p3, dAcObjBase_c *obj) {
    mVec3_c ret;

    mVec3_c v1 = p3;

    dAcPy_c *link = dAcPy_c::LINK;

    dAcObjBase_c *other = nullptr;
    if (obj != nullptr && obj != link) {
        other = obj;
    }

    mVec3_c v2 = p3;

    dAcObjBase_c *controllingActor = (dAcObjBase_c *)link->getLinkage().mControllingActor.p_owner;

    for (int i = 0; i < 3; i++) {
        ret = v1;
        if (!dCcS::GetInstance()->ChkCameraPoint(p2, v2, v1, 15.0f, link, other, controllingActor)) {
            break;
        }
        if (p2 == v1) {
            break;
        }
        mVec3_c tmp = p3 - v1;
        tmp.normalize();
        v2 = v1 - tmp * 2.0f;
    }

    return ret;
}

void dCameraMap_c::onBecomeActive() {
    mTimer = 0;
    mSourceView = mView;
    mTargetView = mView;
    if (!(sHio.field_0x3C & 0x100)) {
        mTargetView.mPosition = mTargetView.mTarget;
    }
    mTargetView.mPosition.y += sHio.field_0x08;
    mTargetView.mFov = sHio.field_0x24;
    mTargetView.mTilt = 0.0f;
    dLytMap_c::GetInstance()->doFn_8013B350();
    dPolar polar(mView.mPosition - mView.mTarget);
    field_0xC4 = polar.V.value;
    mIsMovingOut = false;
    mIsActiveOrAnimating = true;
}

void dCameraMap_c::onBecomeInactive() {
    mIsMovingOut = true;
    dScGame_c::getCamera()->getGameCam1()->setView(mView);
}

bool dCameraMap_c::create() {
    dAcPy_c *link = dAcPy_c::GetLinkM();
    if (link == nullptr) {
        return false;
    }

    field_0x44 = link;
    mView.mTarget = link->mPosition + mVec3_c(0.0f, 100.0f, 0.0f);

    mVec3_c v(0.0f, 200.0f, 500.0f);
    v.rotY(link->mRotation.y - 0x8000);

    mView.mPosition = mView.mTarget + v;
    mView.mFov = 55.0f;
    mView1 = mView;

    field_0x3C = sHio.field_0x04;
    field_0x40 = 0.0f;
    field_0x38 = 0.0f;

    field_0x2C = mVec3_c::Zero;
    mIsActiveOrAnimating = false;
    mIsMovingOut = true;
    field_0xB0 = 0;
    mTimer = 0;

    return true;
}

bool dCameraMap_c::remove() {
    return true;
}

bool dCameraMap_c::execute() {
    s16 duration = sHio.field_0x28;

    if ((sHio.field_0x3C & 0x200) && mIsMovingOut) {
        // TODO no-op
        duration = sHio.field_0x28;
    }
    field_0x40 = 1.0f;

    if (sHio.field_0x3C & 0x8) {
        mTargetView = mSourceView;
    }

    dBgS_CamLinChk chk;
    chk.Set(&mSourceView.mPosition, &mTargetView.mPosition, (dAcObjBase_c *)nullptr);
    if (dBgS::GetInstance()->LineCross(&chk)) {
        mTargetView = mSourceView;
    }

    if (sHio.field_0x3C & 0x10) {
        if (sHio.field_0x3E == 1) {
            // Reorients the main camera to point to north when opening the map without a transition... unused
            dPolar polar(mSourceView.mPosition - mSourceView.mTarget);
            polar.V.Set(0.0f);
            mSourceView.mPosition = mSourceView.mTarget + polar.toCartesian();
        }
        mTargetView = mSourceView;
        mTimer = 1;
    }

    if (mIsMovingOut) {
        if (mTimer > 0) {
            mTimer--;
            if (mTimer == 0) {
                mIsActiveOrAnimating = false;
            }
        }
    } else {
        if (mTimer < duration) {
            mTimer++;
        }
    }

    f32 f = (f32)mTimer / (f32)duration;
    f32 f5 = camEaseInOut(f, sHio.field_0x2C);
    f32 f6 = camEaseInOut(f, sHio.field_0x30);
    f32 f8 = f5 * 0.99f;
    f32 tiltFovInterp = camEaseInOut(f, sHio.field_0x34);

    mView.mTarget = mSourceView.mTarget + (mTargetView.mTarget - mSourceView.mTarget) * f5;
    mView.mPosition.x = mSourceView.mPosition.x + (mTargetView.mPosition.x - mSourceView.mPosition.x) * f8;
    mView.mPosition.y = mSourceView.mPosition.y + (mTargetView.mPosition.y - mSourceView.mPosition.y) * f6;
    mView.mPosition.z = mSourceView.mPosition.z + (mTargetView.mPosition.z - mSourceView.mPosition.z) * f8;

    mView.mPosition = fn_800929C0(mView.mTarget, mView.mPosition, field_0x44);
    mView.mFov = mSourceView.mFov + (mTargetView.mFov - mSourceView.mFov) * tiltFovInterp;
    mView.mTilt = mSourceView.mTilt + (mTargetView.mTilt - mSourceView.mTilt) * tiltFovInterp;

    return true;
}

bool dCameraMap_c::vt_0x34() {
    return true;
}

bool dCameraMap_c::draw() {
    return true;
}

bool dCameraMap_c::isActiveOrAnimating() const {
    return mIsActiveOrAnimating;
}

void dCameraMap_c::startOut() {
    mIsMovingOut = true;
    if (sHio.field_0x3C & 0x200) {
        mTimer = dLytMap_c::GetInstance()->getOutDuration() + 0.1f;
    }
}

void dCameraMap_c::fn_80093340() {
    sHio.field_0x3E = 2;
}

void dCameraMap_c::setNoCameraTransition() {
    sHio.field_0x3C |= 0x10 | 0x8;
}

void dCameraMap_c::setCameraTransition() {
    sHio.field_0x3C &= ~(0x10 | 0x8);
}

void dCameraMap_c::setView(const CamView &view) {
    mView1 = view;
    mView = view;
}
