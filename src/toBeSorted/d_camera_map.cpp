#include "toBeSorted/d_camera_map.h"

#include "common.h"
#include "d/a/d_a_player.h"
#include "d/a/obj/d_a_obj_base.h"
#include "d/col/bg/d_bg_s.h"
#include "d/col/bg/d_bg_s_lin_chk.h"
#include "d/col/cc/d_cc_s.h"
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
        if (!dCcS::GetInstance()->ChkCameraPoint(p2, v2, v1, link, other, controllingActor, 15.0f)) {
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

bool dCameraMap_c::execute() {
    s16 unk = sHio.field_0x28;

    if ((sHio.field_0x3C & 0x200) && field_0xAC) {
        // TODO no-op
        unk = sHio.field_0x28;
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
            dPolar polar(mSourceView.mPosition - mSourceView.mTarget);
            polar.V.Set(0.0f);
            mSourceView.mPosition = mSourceView.mTarget + polar.toCartesian();
        }
        mTargetView = mSourceView;
        field_0xB8 = 1;
    }

    if (field_0xAC != 0) {
        if (field_0xB8 > 0) {
            field_0xB8--;
            if (field_0xB8 == 0) {
                field_0xB4 = 0;
            }
        }
    } else {
        if (field_0xB8 < unk) {
            field_0xB8++;
        }
    }

    f32 f = (f32)field_0xB8 / (f32)unk;
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

void dCameraMap_c::setView(const CamView &view) {
    mView1 = view;
    mView = view;
}
