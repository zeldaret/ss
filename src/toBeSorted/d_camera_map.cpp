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
        mView3 = mView2;
    }

    dBgS_CamLinChk chk;
    chk.Set(&mView2.field_0x00, &mView3.field_0x00, (dAcObjBase_c *)nullptr);
    if (dBgS::GetInstance()->LineCross(&chk)) {
        mView3 = mView2;
    }

    if (sHio.field_0x3C & 0x10) {
        if (sHio.field_0x3E == 1) {
            dPolar polar(mView2.field_0x00 - mView2.field_0x0C);
            polar.V.Set(0.0f);
            mView2.field_0x00 = mView2.field_0x0C + polar.toCartesian();
        }
        mView3 = mView2;
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
    f32 f7 = camEaseInOut(f, sHio.field_0x34);

    mView.field_0x0C = mView2.field_0x0C + (mView3.field_0x0C - mView2.field_0x0C) * f5;
    mView.field_0x00.x = mView2.field_0x00.x + (mView3.field_0x00.x - mView2.field_0x00.x) * f8;
    mView.field_0x00.y = mView2.field_0x00.y + (mView3.field_0x00.y - mView2.field_0x00.y) * f6;
    mView.field_0x00.z = mView2.field_0x00.z + (mView3.field_0x00.z - mView2.field_0x00.z) * f8;

    mView.field_0x00 = fn_800929C0(mView.field_0x0C, mView.field_0x00, field_0x44);
    mView.mFov = mView2.mFov + (mView3.mFov - mView2.mFov) * f7;
    mView.field_0x1C = mView2.field_0x1C + (mView3.field_0x1C - mView2.field_0x1C) * f7;

    return true;
}

void dCameraMap_c::setView(const CamView &view) {
    mView1 = view;
    mView = view;
}
