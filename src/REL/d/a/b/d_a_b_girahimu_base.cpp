#include "d/a/b/d_a_b_girahimu_base.h"

#include "c/c_lib.h"
#include "c/c_math.h"
#include "common.h"
#include "d/a/d_a_player.h"
#include "d/a/obj/d_a_obj_girahimu_sword_link.h"
#include "d/col/c/c_cc_d.h"
#include "d/d_vec.h"
#include "d/flag/sceneflag_manager.h"
#include "d/snd/d_snd_wzsound.h"
#include "m/m3d/m_fanm.h"
#include "m/m_angle.h"
#include "m/m_mtx.h"
#include "m/m_quat.h"
#include "m/m_vec.h"
#include "nw4r/g3d/res/g3d_resmdl.h"
#include "nw4r/math/math_arithmetic.h"
#include "rvl/MTX/mtx.h"
#include "rvl/VI/vi.h"
#include "s/s_Math.h"

#include <cstdint>

STATE_VIRTUAL_DEFINE(dAcGirahimuBase_c, Wait);
STATE_VIRTUAL_DEFINE(dAcGirahimuBase_c, Walk);
STATE_VIRTUAL_DEFINE(dAcGirahimuBase_c, Panch);
STATE_VIRTUAL_DEFINE(dAcGirahimuBase_c, Catch);
STATE_VIRTUAL_DEFINE(dAcGirahimuBase_c, CatchDamage);
STATE_VIRTUAL_DEFINE(dAcGirahimuBase_c, GetSword);
STATE_VIRTUAL_DEFINE(dAcGirahimuBase_c, Link_SwordWait);
STATE_VIRTUAL_DEFINE(dAcGirahimuBase_c, Link_SwordWalk);
STATE_VIRTUAL_DEFINE(dAcGirahimuBase_c, Link_SwordAttack);
STATE_VIRTUAL_DEFINE(dAcGirahimuBase_c, ReturnSword);
STATE_VIRTUAL_DEFINE(dAcGirahimuBase_c, LinkSwordGuardJust);
STATE_VIRTUAL_DEFINE(dAcGirahimuBase_c, SearchSword);
STATE_VIRTUAL_DEFINE(dAcGirahimuBase_c, HomeWarp);
STATE_VIRTUAL_DEFINE(dAcGirahimuBase_c, PickUpSword);
STATE_VIRTUAL_DEFINE(dAcGirahimuBase_c, ReleaseSword);
STATE_VIRTUAL_DEFINE(dAcGirahimuBase_c, BackStep);
STATE_VIRTUAL_DEFINE(dAcGirahimuBase_c, Escape);
STATE_VIRTUAL_DEFINE(dAcGirahimuBase_c, EscapeBack);
STATE_VIRTUAL_DEFINE(dAcGirahimuBase_c, FrontWarp);
STATE_VIRTUAL_DEFINE(dAcGirahimuBase_c, FrontAttack);
STATE_VIRTUAL_DEFINE(dAcGirahimuBase_c, BackWarp);
STATE_VIRTUAL_DEFINE(dAcGirahimuBase_c, BackAttack);
STATE_VIRTUAL_DEFINE(dAcGirahimuBase_c, Counter);
STATE_VIRTUAL_DEFINE(dAcGirahimuBase_c, Run);
STATE_VIRTUAL_DEFINE(dAcGirahimuBase_c, RunAttack);
STATE_VIRTUAL_DEFINE(dAcGirahimuBase_c, BackWalk);
STATE_VIRTUAL_DEFINE(dAcGirahimuBase_c, G_SwordDamage);
STATE_VIRTUAL_DEFINE(dAcGirahimuBase_c, KnifeDamage);

SPECIAL_ACTOR_PROFILE(B_GIRAHIMU_BASE, dAcGirahimuBase_c, fProfile::B_GIRAHIMU_BASE, 0x10D, 0, 0);

void dAcGirahimuBase_c::callback_c::fn_226_DC0(mMtx_c &out, const mAng3_c &rot, const mAng yRot) {
    mVec3_c translation;
    mMtx_c transform;
    out.getTranslation(translation);
    transform.YrotS(yRot);
    transform.YrotM(rot.y);
    transform.XrotM(rot.x);
    transform.YrotM(-yRot);
    MTXConcat(transform, out, out);
    out.setTranslation(translation);
}

void dAcGirahimuBase_c::callback_c::fn_226_E80() {
    mVec3_c v0 = field_0x164;

    mVec3_c v1 = v0 - field_0x21C;
    v1.normalizeRS();

    mVec3_c v2 = field_0x260 - v0;
    v2.normalizeRS();

    mVec3_c v3 = field_0x21C - v0;
    v3.normalizeRS();

    mVec3_c v4;
    vecCross(v4, v2, v3);
    v4.normalizeRS();

    mVec3_c v5;
    vecCross(v5, v4, v1);
    v5.normalizeRS();

    mVec3_c v6 = v5;
    v5 = -v4;
    v4 = -v6;

    mMtx_c m0;
    m0.setBase(0, v1);
    m0.setBase(1, v4);
    m0.setBase(2, v5);

    mQuat_c q0;
    m0.toQuat(q0);
    field_0x1CC.slerpTo(q0, 1.0f, field_0x1CC);
    field_0x19C.fromQuat(field_0x1CC);
    field_0x19C.setTranslation(field_0x21C);

    field_0x19C.scaleM(mpGhirahim->field_0x8BC);
}

void dAcGirahimuBase_c::callback_c::fn_226_1160() {
    mVec3_c v0 = field_0x164;

    mVec3_c v1 = field_0x260 - v0;
    v1.normalizeRS();

    mVec3_c v2 = field_0x260 - v0;
    v2.normalizeRS();

    mVec3_c v3;
    v3 = field_0x21C - v0;
    v3.normalizeRS();

    mVec3_c v4;
    vecCross(v4, v2, v3);
    v4.normalizeRS();

    mVec3_c v5;
    vecCross(v5, v4, v1);
    v5.normalizeRS();

    mVec3_c v6 = v5;
    v5 = -v4;
    v4 = -v6;

    mMtx_c m0;
    m0.setBase(0, v1);
    m0.setBase(1, v4);
    m0.setBase(2, v5);

    mQuat_c q0;
    m0.toQuat(q0);
    field_0x118.slerpTo(q0, 1.0f, field_0x118);
    field_0x0E8.fromQuat(field_0x118);

    if (mpGhirahim != nullptr && mpGhirahim->field_0xD71) {
        mVec3_c diff = field_0x260 - field_0x21C;
        mAng ang = (s32)diff.atan2sX_Z();
        ang = ang - field_0x084;
        if (ang > 0) {
            field_0x0E8.XrotM(-ang);
        }
    }
    field_0x0E8.setTranslation(field_0x164);
    field_0x19C.scaleM(mpGhirahim->field_0x8BC);
}

void dAcGirahimuBase_c::callback_c::init(dAcGirahimuBase_c *pGhirahim) {
    field_0x014 = 1.0f;
    mMtx_c m0;
    MTXIdentity(m0);
    (void)mVec3_c(1.0f, 0.0f, 0.0f);
    (void)mVec3_c(0.0f, 0.0f, -1.0f);
    (void)mVec3_c(0.0f, 1.0f, 0.0f);
    m0.toQuat(field_0x004);
    m0.toQuat(field_0x118);
    m0.toQuat(field_0x1CC);
    mpGhirahim = pGhirahim;
    field_0x2D4 = -1.0f;
    field_0x2D0 = -1.0f;
}

void dAcGirahimuBase_c::callback_c::fn_226_1580(u32 node, nw4r::g3d::WorldMtxManip *result, nw4r::g3d::ResMdl) {
    if (node == mNodeID_ArmR) {
        field_0x2C0 = result;
        result->GetMtx(field_0x19C);
        field_0x19C.getTranslation(field_0x21C);
        mVec3_c v0(-40.0f, -20.0f, -15.0f);
        v0.rotY(field_0x084);
        v0 += field_0x21C;
        field_0x18C.set(v0);
        if (!field_0x1E8) {
            field_0x1E8 = true;
        } else {
            if (field_0x04E) {
                fn_226_1DD0();
                fn_226_24F0();
                fn_226_24E0();
                fn_226_1160();
                fn_226_E80();
            } else {
                field_0x2BA = 0;
                sLib::chaseAngle(field_0x0B4.ref(), 0, 0x1000);
                mQuat_c q0;
                field_0x19C.toQuat(q0);
                field_0x1CC.slerpTo(q0, field_0x2D0, field_0x1CC);
                if (field_0x2D0 < 1.0f) {
                    mMtx_c m0;
                    field_0x19C.fromQuat(field_0x1CC);
                    m0.scaleS(mpGhirahim->field_0x8BC);
                    MTXConcat(m0, field_0x19C, field_0x19C);
                    field_0x19C.setTranslation(field_0x21C);
                }
            }
            result->SetMtx(field_0x19C);
        }
    } else if (node == mNodeID_ElbowR) {
        field_0x2C4 = result;
        if (!field_0x198) {
            result->GetMtx(field_0x0E8);
            field_0x0E8.getTranslation(field_0x164);
            field_0x128.set(1.0f, 0.0f, 0.0f);
            field_0x198 = true;
            field_0x0E8.getTranslation(field_0x18C);
        }

        if (!field_0x04E) {
            mMtx_c m0;
            mQuat_c q0;
            result->GetMtx(m0);
            m0.toQuat(q0);

            if (field_0x2D0 < 1.0f) {
                field_0x118.slerpTo(q0, field_0x2D0, field_0x118);
                field_0x0E8.fromQuat(field_0x118);
                mMtx_c m1;
                m1.scaleS(mpGhirahim->field_0x8BC);
                MTXConcat(m1, field_0x0E8, field_0x0E8);
                mVec3_c v0(field_0x234, 0.0f, 0.0f);
                field_0x19C.multVec(v0, v0);
                field_0x164.set(v0);
                field_0x0E8.setTranslation(field_0x164);
            } else {
                field_0x118.slerpTo(q0, field_0x2D0, field_0x118);
                result->GetMtx(field_0x0E8);
                field_0x0E8.getTranslation(field_0x164);
            }
        }
        result->SetMtx(field_0x0E8);
    } else if (node == mNodeID_HandR) {
        if (!field_0x2B8) {
            field_0x2B8 = true;
            result->GetMtx(field_0x278);
        }
        if (field_0x04E) {
            if (mpGhirahim && !mpGhirahim->field_0xD71) {
                mMtx_c m0;
                result->GetMtx(m0);
                m0.setTranslation(field_0x260);
                sLib::chaseAngle(field_0x2BA.ref(), 0x2000, 0x200);
                m0.YrotM(field_0x2BA);
                mQuat_c q0;
                m0.toQuat(q0);
                field_0x2A8.slerpTo(q0, field_0x2D0, field_0x2A8);
                field_0x278.fromQuat(field_0x2A8);
                field_0x278.setTranslation(field_0x260);
            }
        } else {
            field_0x2BA = 0;
            if (field_0x2D0 < 1.0f) {
                mMtx_c m0;
                mQuat_c q0;
                result->GetMtx(m0);
                m0.toQuat(q0);
                field_0x2A8.slerpTo(q0, 1.0f, field_0x2A8);
                field_0x278.fromQuat(field_0x2A8);

                mMtx_c scale;
                scale.scaleS(mpGhirahim->field_0x8BC);
                MTXConcat(scale, field_0x278, field_0x278);
                mVec3_c v0(field_0x188, 0.0f, 0.0f);
                field_0x0E8.multVec(v0, v0);
                field_0x260.set(v0);
                field_0x278.setTranslation(field_0x260);
            } else {
                result->GetMtx(field_0x278);
                field_0x278.getTranslation(field_0x260);
                field_0x278.toQuat(field_0x2A8);
            }

            if (mpGhirahim) {
                field_0x254 = field_0x260 - field_0x21C;
                field_0x254.rotY(-mpGhirahim->mRotation.y);
            }
        }
        result->SetMtx(field_0x278);
    }
}

void dAcGirahimuBase_c::callback_c::fn_226_1B60(mVec3_c &out) {
    mMtx_c swordMtx;
    dAcPy_c::GetLinkM()->getSwordModelMatrix(&swordMtx);
    swordMtx.m[0][3] = 0.0f;
    swordMtx.m[1][3] = 0.0f;
    swordMtx.m[2][3] = 0.0f;

    mVec3_c v0(-1.0f, 0.0f, 0.0f);
    mVec3_c v1(0.0f, -1.0f, 0.0f);
    mVec3_c v2(0.0f, 0.0f, -1.0f);
    swordMtx.multVec(v0, v0);
    swordMtx.multVec(v1, v1);
    swordMtx.multVec(v2, v2);

    (void)v2.atan2sX_Z();

    mVec3_c v4(-5.0f, -2.0f, 15.0f);
    if (mpGhirahim != nullptr && !mpGhirahim->field_0xD71) {
        v4.z = 0.0f;
    }
    swordMtx.multVec(v4, v4);

    mVec3_c v5 = dAcPy_c::GetLinkM()->getSwordPos() - dAcPy_c::GetLinkM()->vt_0x278();
    if (mpGhirahim != nullptr && !mpGhirahim->field_0xD71) {
        v5 *= 1.0f;
    } else {
        v5 *= 0.9f;
    }
    const mVec3_c &vt278 = dAcPy_c::GetLinkM()->vt_0x278();

    v5 += vt278;
    v5 += v4;
    if (mpGhirahim != nullptr && !mpGhirahim->field_0xD71) {
        v5.y -= 5.0f;
    }

    out.set(v5);
}

void dAcGirahimuBase_c::callback_c::fn_226_1DD0() {
    mMtx_c swordMtx;
    dAcPy_c::GetLinkM()->getSwordModelMatrix(&swordMtx);
    swordMtx.m[0][3] = 0.0f;
    swordMtx.m[1][3] = 0.0f;
    swordMtx.m[2][3] = 0.0f;

    mVec3_c v0(-1.0f, 0.0f, 0.0f);
    mVec3_c v1(0.0f, -1.0f, 0.0f);
    mVec3_c v2(0.0f, 0.0f, -1.0f);
    swordMtx.multVec(v0, v0);
    swordMtx.multVec(v1, v1);
    swordMtx.multVec(v2, v2);

    mVec3_c v3;
    fn_226_1B60(v3);

    mVec3_c v4 = v3;
    mVec3_c v5;
    v5 = v3 - field_0x21C;

    if (v5.mag() >= (field_0x234 + field_0x188) * 0.9f) {
        field_0x050 = true;
    } else {
        field_0x050 = false;
    }
    field_0x0B4.set(0);
    if (mpGhirahim != nullptr && !mpGhirahim->field_0xD71) {
        if (field_0x050) {
            v3 -= field_0x21C;
            v3.normalizeRS();
            v3 *= (field_0x234 + field_0x188) * 0.9f;
            v3 += field_0x21C;
        }
        mVec3_c v6 = v3 - field_0x21C;

        dAcGirahimuBase_c *pAc = mpGhirahim;
        mAng ang = mAng((s32)v6.atan2sX_Z());
        ang = ang - pAc->mRotation.y;
        mAng a;
        switch (ang) {
            case 0x2000 ... INT32_MAX: {
                a = 0x2000 - ang;
                v6.rotY(a);
                v6 += field_0x21C;
                v3 = v6;
            } break;
            case -0x1FFF ... 0x1FFF: {
                // Do Nothing
            } break;
            default: {
                a = -0x2000 - ang;
                v6.rotY(a);
                v6 += field_0x21C;
                v3 = v6;
            } break;
        }

        mVec3_c v7 = v3 - field_0x21C;
        v7.rotY(-mpGhirahim->mRotation.y);

        if (mpGhirahim->mHealth >= 100) {
            if (--field_0x2BC <= 0) {
                field_0x2BC = 60;
                f32 z = v7.z;
                v7.z = 0.0f;
                v7.normalizeRS();
                v7 *= 40.0f;
                v7.z = z;
                field_0x26C.set(v7);
            }
            v7.set(field_0x26C);
        }
        if (mpGhirahim->field_0xD50 > 0) {
            cLib::addCalcPos2(&field_0x254, v7, 0.2f, 1000.0f);
        } else {
            cLib::addCalcPos2(&field_0x254, v7, 0.2f, field_0x2CC);
        }

        mVec3_c v8 = v7 - field_0x254;
        f32 magV8 = v8.mag();

        mVec3_c v9 = field_0x21C;
        v9.y -= 10.0f;

        field_0x098 = magV8;

        mMtx_c m0;
        m0.transS(v9);
        m0.YrotM(mpGhirahim->mRotation.y);
        m0.inverse();

        mVec3_c v10 = field_0x260;
        m0.multVec(v10, v10);
        v10.z = 0.0f;
        field_0x2E0.set(v10);

        field_0x2D8 = v10.mag();

        m0.transS(field_0x260);
        m0.YrotM(mpGhirahim->mRotation.y);
        m0.inverse();
        m0.multVec(v4, v4);

        v4.z = 0.0f;
        field_0x2EC.set(v4);
        mVec3_c v11 = field_0x254;
        v11.rotY(mpGhirahim->mRotation.y);
        v11 += field_0x21C;
        cLib::addCalcPos2(&field_0x260, v11, 0.5f, 100.0f);
    } else {
        field_0x2BA = 0;
        field_0x260.set(v3);
        if (mpGhirahim != nullptr) {
            field_0x254.set(field_0x260 - field_0x21C);
            field_0x254.rotY(-mpGhirahim->mRotation.y);
        }
        mMtx_c m1;
        m1.setBase(0, v2);
        m1.setBase(1, v1);
        m1.setBase(2, v0);

        mQuat_c q0;
        m1.toQuat(q0);

        field_0x2A8.slerpTo(q0, field_0x2D0, field_0x2A8);
        field_0x278.fromQuat(field_0x2A8);
        field_0x278.setTranslation(field_0x260);
        field_0x278.scaleM(1.05f, 1.05f, 1.05f);
    }
}

void dAcGirahimuBase_c::callback_c::fn_226_24E0() {
    return;
}

void dAcGirahimuBase_c::callback_c::fn_226_24F0() {
    mVec3_c v3 = field_0x260;
    mVec3_c v4 = field_0x18C;
    mVec3_c v5 = v4 - v3;
    v5.normalizeRS();
    v5 *= field_0x188;
    v5 += v3;
    mVec3_c v1 = field_0x21C - v3;
    v4.set(v5);
    v4.normalizeRS();
    v1 *= field_0x188;
    v1 += v3;
    if (v1.y <= v4.y) {
        v4.y += v1.y - v4.y;
    }
    mVec3_c v6 = field_0x21C - v4;
    v6.normalizeRS();
    v6 *= field_0x234;
    v6 += v4;

    mVec3_c v2;
    v1 = v6 - v3;
    v2 = field_0x21C - v3;

    v1.angle(v2);

    mQuat_c q;
    q.slerp(v1, v2, 1.0f);
    mMtx_c m;
    m.fromQuat(q);

    mVec3_c v7 = v4 - v3;
    m.multVec(v7, v7);
    v7 += v3;
    v1 = v7 - field_0x21C;
    v4 = v7;
    v1.normalizeRS();
    v4 = v1 * field_0x234;
    v4 += field_0x21C;
    v1.set(v4);
    v1 -= field_0x260;
    v1.normalizeRS();
    v4 = v1 * field_0x188;
    v4 += field_0x260;
    v1.set(v4);
    cLib::addCalcPos2(&field_0x164, v4, 0.5f, 100.0f);
}

void dAcGirahimuBase_c::callback_c::timingB(u32 node, nw4r::g3d::WorldMtxManip *result, nw4r::g3d::ResMdl i_mdl) {
    fn_226_1580(node, result, i_mdl);

    mMtx_c m0;
    if (node == mNodeID_Head) {
        result->GetMtx(m0);
        m0.getTranslation(field_0x08C);

        mMtx_c m1;
        result->GetMtx(m1);
        fn_226_3150(m1);

        mAng3_c a0(field_0x07E, field_0x07C, 0);
        fn_226_DC0(m0, a0, field_0x084);
        result->SetMtx(m0);
    } else if (node == mNodeID_Hair1) {
        mMtx_c m1;
        result->GetMtx(m1);
        m1.YrotM(-field_0x062.y);
        m1.ZrotM(field_0x062.z);
        result->SetMtx(m1);
    } else if (node == mNodeID_Hair2) {
        mMtx_c m1;
        result->GetMtx(m1);
        m1.YrotM(-field_0x062.y);
        m1.ZrotM(field_0x062.z);
        result->SetMtx(m1);
    } else if (node == mNodeID_Hair3) {
        mMtx_c m1;
        result->GetMtx(m1);
        m1.YrotM(-field_0x062.y);
        m1.ZrotM(field_0x062.z);
        result->SetMtx(m1);
    } else if (node == mNodeID_Spine1) {
        if (field_0x04E) {
            if (mpGhirahim->field_0xD66 <= 0) {
                mVec3_c v0;
                fn_226_1B60(v0);
                dAcGirahimuBase_c *pGhirahim = mpGhirahim;
                mAng ang = cLib::targetAngleY(mpGhirahim->mPosition, v0);
                ang = ang - pGhirahim->mRotation.y;
                // What the....
                if (mpGhirahim->field_0x8C8 != 1) {
                    sLib::chaseAngle(field_0x062.x.ref(), ang / 3, 0x80);
                } else {
                    static s32 divVal = {2};
                    sLib::chaseAngle(field_0x062.x.ref(), ang / divVal, 0x80);
                }
            }
        } else {
            sLib::chaseAngle(field_0x062.x.ref(), 0, 0x80);
        }
        result->GetMtx(m0);
        mAng3_c a0(field_0x058.y, field_0x058.x, 0);
        fn_226_DC0(m0, a0, field_0x060);
        m0.XrotM(field_0x062.x);
        result->SetMtx(m0);
    } else if (node == mNodeID_Spine2) {
        result->GetMtx(m0);
        mAng3_c a0(field_0x058.y, field_0x058.x, 0);
        fn_226_DC0(m0, a0, field_0x060);
        m0.XrotM(field_0x062.x);
        result->SetMtx(m0);
    } else if (node == mNodeID_ElbowR) {
        fn_226_2FD0();
        result->GetMtx(m0);
        m0.YrotM(field_0x238);
        result->SetMtx(m0);
    } else if (node == mNodeID_ElbowL) {
        fn_226_3080();
        result->GetMtx(m0);
        m0.YrotM(field_0x23A);
        result->SetMtx(m0);
    } else if (node == mNodeID_KneeL) {
        result->GetMtx(m0);
        mAng3_c a0(0, field_0x052.y / 2, 0);
        fn_226_DC0(m0, a0, mpGhirahim->mRotation.y);
        result->SetMtx(m0);
    } else if (node == mNodeID_KneeR) {
        result->GetMtx(m0);
        mAng3_c a0(0, field_0x052.y / 2, 0);
        fn_226_DC0(m0, a0, mpGhirahim->mRotation.y);
        result->SetMtx(m0);
    } else if (node == mNodeID_LegL) {
        result->GetMtx(m0);
        mAng3_c a0(0, field_0x052.y / 2, 0);
        fn_226_DC0(m0, a0, mpGhirahim->mRotation.y);
        result->SetMtx(m0);
    } else if (node == mNodeID_LegR) {
        result->GetMtx(m0);
        mAng3_c a0(0, field_0x052.y / 2, 0);
        fn_226_DC0(m0, a0, mpGhirahim->mRotation.y);
        result->SetMtx(m0);
    }
}

void dAcGirahimuBase_c::callback_c::fn_226_2FD0() {
    sLib::chaseAngle(field_0x23C.ref(), 0, 0x150);
    field_0x240 += 0x3500;
    field_0x238 = field_0x23C * field_0x240.sin();
}

void dAcGirahimuBase_c::callback_c::fn_226_3080() {
    sLib::chaseAngle(field_0x23E.ref(), 0, 0x150);
    field_0x242 += 0x3500;
    field_0x23A = field_0x23E * field_0x242.sin();
}

void dAcGirahimuBase_c::callback_c::fn_226_3130() {
    sLib::chase(&field_0x2D0, field_0x2D4, 0.05f);
}

void dAcGirahimuBase_c::callback_c::fn_226_3150(mMtx_c &m) {
    if (field_0x088 && mpGhirahim->field_0xD5C <= 10) {
        mVec3_c v0;
        m.getTranslation(v0);
        m.m[2][3] = 0.0f;
        m.m[1][3] = 0.0f;
        m.m[0][3] = 0.0f;
        mVec3_c v1(0.0f, 1.0f, 0.0f);
        m.multVec(v1, v1);
        s16 a0 = v1.atan2sX_Z();
        s16 a1 = v1.atan2sY_XZ();
        field_0x080 = cLib::targetAngleY(v0, mpGhirahim->field_0x88C) - a0;
        field_0x082 = -(cLib::targetAngleX(v0, mpGhirahim->field_0x88C) - a1);
    } else {
        field_0x080 = 0;
        field_0x082 = 0;
    }

    if (field_0x080 > 0x3000) {
        field_0x080.set(0x3000);
    } else if (field_0x080 < -0x3000) {
        field_0x080 = -0x3000;
    }

    sLib::addCalcAngle(field_0x07C.ref(), field_0x080, 2, 0x500);
    sLib::addCalcAngle(field_0x07E.ref(), field_0x082, 2, 0x500);

    if (field_0x07E < 0x2000) {
        if (field_0x07E + 0x1FFF > 0x3FFEu) {
            field_0x07E = -0x2000;
        }
    } else {
        field_0x07E.set(0x2000);
    }
}

void dAcGirahimuBase_c::initializeState_Wait() {
    field_0xD6C = 1;
    fn_226_8B70("WaitA", 0, false, m3d::PLAY_MODE_4, 30.0f, 1.0f);
    mCallback.field_0x088 = true;

    field_0xD46 = 200;
    field_0xD48 = 400;
    field_0xD4A = 300;

    field_0x1834 = 0.0f;
    mSpeed = 0.0f;
    if (dAcPy_c::GetLinkM()->isUsingSword()) {
        mCallback.field_0x04E = true;
        mCallback.field_0x2D4 = 0.2f;
        mCallback.field_0x2D0 = 0.2f;
    } else {
        mCallback.field_0x04E = false;
        mCallback.field_0x2D4 = 1.0f;
    }
}
void dAcGirahimuBase_c::executeState_Wait() {
    bool b0 = false;
    if (dAcPy_c::GetLinkM()->isUsingSword() && field_0x1838 <= 250.0f) {
        b0 = true;
    }

    if (dAcPy_c::GetLinkM()->isUsingSword() && field_0x1838 <= 370.0f) {
        fn_226_BC00();
    } else {
        mCallback.field_0x04F = false;
        mCallback.field_0x04E = false;
    }

    if (dAcPy_c::GetLinkM()->getBeetleInFlight() != nullptr) {
        return;
    }
    mVec3_c distanceFromStart = mPosition - mStartingPos;
    mVec3_c v1(0.0f, 0.0f, 1.0f);
    v1.rotY(mRotation.y);
    if (field_0x1868 >= 1500.0f && distanceFromStart.dot(v1) < 0.0f) {
        field_0x8DC = 0;
        changeState(StateID_HomeWarp);
        return;
    }
    if (field_0xD46 <= 0 && mSwordLink.get()->isStick()) {
        changeState(StateID_SearchSword);
        return;
    }

    if (vt_0x210()) {
        return;
    }

    if (mSwordLink.get()->isStick()) {
        if (field_0xD46 % 100 > 50) {
            field_0x88C = mSwordLink.get()->mPosition;
        }
        return;
    }

    if (field_0xD4A <= 0) {
        vt_0x204();
        return;
    }

    if (!b0) {
        changeState(StateID_Walk);
        return;
    }
    if (!(mSwordLink.get()->isStick() || turn(field_0x183E))) {
        if (field_0x1838 <= 215.0f && --field_0xD4A <= 0) {
            field_0xD4A = 0;
        }

        fn_226_8FF0("WaitA", ANM_WaitA, m3d::PLAY_MODE_4, 5.0f, 1.0f);
    }

    if (field_0x8CE == ANM_MovePose && mAnmChrs[4].checkFrame(43.0f)) {
        fn_226_9190("WaitA", ANM_WaitA, m3d::PLAY_MODE_4, 15.0f, 1.0f);
        fn_226_9260("WaitA", ANM_WaitA, m3d::PLAY_MODE_4, 15.0f, 1.0f);
    }
    if (field_0xD48 <= 0) {
        fn_226_9190("MovePose", ANM_MovePose, m3d::PLAY_MODE_4, 15.0f, 1.0f);
        fn_226_9260("MovePose", ANM_MovePose, m3d::PLAY_MODE_4, 15.0f, 1.0f);

        field_0xD48 = 400 + cM::rndInt(100);
    }

    sLib::chaseAngle(mRotation.y.ref(), field_0x183E, 0x500);
}
void dAcGirahimuBase_c::finalizeState_Wait() {
    field_0xD44 = 0;
    mCallback.field_0x04E = false;
    mCallback.field_0x2D4 = 1.0f;
}

void dAcGirahimuBase_c::initializeState_Walk() {
    if (dAcPy_c::GetLinkM()->getBeetleInFlight() != nullptr) {
        changeState(StateID_Wait);
        return;
    }

    field_0xD6C = 1;
    fn_226_8B70("Walk", ANM_Walk, false, m3d::PLAY_MODE_4, 15.0f, 1.0f);

    mCallback.field_0x088 = true;
    mCallback.field_0x04E = false;
    mCallback.field_0x2D4 = 1.0f;

    if (dAcPy_c::GetLinkM()->isUsingSword()) {
        mCallback.field_0x04E = true;
        mCallback.field_0x2D4 = 0.5f;
        mCallback.field_0x2D0 = 0.0f;
    } else {
        mCallback.field_0x04E = false;
        mCallback.field_0x2D4 = 1.0f;
    }
    mCallback.field_0x2CC = 0.0f;
}
void dAcGirahimuBase_c::executeState_Walk() {
    if (mSwordLink.get()->isStick()) {
        changeState(StateID_Wait);
        return;
    }

    fn_226_BC00();

    if (mCallback.field_0x04E) {
        fn_226_90C0("WaitA", ANM_WaitA, m3d::PLAY_MODE_4, 15.0f, 1.0f);
    } else {
        mCallback.field_0x2BC = 0;
        mCallback.field_0x098 = 10.0f;
        fn_226_90C0("Walk", ANM_Walk, m3d::PLAY_MODE_4, 15.0f, 1.0f);
    }

    if (dAcPy_c::GetLinkM()->isUsingSword()) {
        if (field_0x1838 <= 200.0f) {
            changeState(StateID_Wait);
            return;
        }
    } else {
        if (field_0x1838 <= 150.0f) {
            changeState(StateID_Panch);
            return;
        }
    }
    fn_226_B960(6.0f);
}
void dAcGirahimuBase_c::finalizeState_Walk() {}

void dAcGirahimuBase_c::initializeState_Panch() {
    fn_226_8B70("AttackBinta", ANM_AttackBinta, false, m3d::PLAY_MODE_4, 5.0f, 1.0f);
    mCallback.field_0x088 = true;
    field_0x1834 = 0.0f;
    mSpeed = 0.0f;
}
void dAcGirahimuBase_c::executeState_Panch() {
    field_0xD58 = 60;
    sLib::chaseAngle(mRotation.y.ref(), field_0x183E, 0x500);
    if (mAnmChrs[0].checkFrame(10.0f)) {
        mSph0.SetR(100.0f);
        mSph0.ClrCoSet();
        mSph0.OnAtSet();
        mSph0.SetAtFlagsUpper(0x40000);
        return;
    }
    if (mAnmChrs[0].checkFrame(22.0f)) {
        mSph0.SetR(40.0f);
        mSph0.ClrAtSet();
        mSph0.SetAtFlagsUpper(0);
        return;
    }

    if (mAnmChrs[0].isStop()) {
        changeState(StateID_Wait);
        return;
    }
}
void dAcGirahimuBase_c::finalizeState_Panch() {
    mSph0.OnCoSet();
    mSph0.SetR(40.0f);
    mSph0.ClrAtSet();
    mSph0.SetAtFlagsUpper(0);
    field_0xD66 = 0;
    field_0xD58 = 60;
}

void dAcGirahimuBase_c::initializeState_Catch() {
    s32 flag = getFromParams(0, 0xFF);
    if (flag != 0xFF) {
        // 0xC0 -> zone temp flag: 0x1 01
        SceneflagManager::sInstance->setFlag(mRoomID, flag);
    }
    vt_0x1FC();

    mCollider.ClrCo();
    field_0x1834 = 0.0f;
    mSpeed = 0.0f;

    field_0xD7E = false;
    if (!field_0x1871 || cM::rndInt(100) > 50) {
        field_0x1871 = true;
        field_0xD7E = true;
        fn_226_8C90("Catch", ANM_Catch, "Catch", ANM_Catch, "FaceTongue", ANM_FaceTonuge, m3d::PLAY_MODE_4, 5.0f, 1.0f);
    } else {
        fn_226_8C90("Catch", ANM_Catch, "Catch", ANM_Catch, "FaceSmaile", ANM_FaceSmaile, m3d::PLAY_MODE_4, 5.0f, 1.0f);
    }

    mCallback.field_0x088 = true;
    dAcPy_c::GetLinkM()->set0x439F(this, 7);
    mCallback.field_0x014 = 1.0f;
    mCallback.field_0x2D4 = 0.5f;
    field_0xD46 = 4;
    field_0xD5E = 300;
    if (!field_0x1872) {
        field_0xD5E = 75;
    }
}
void dAcGirahimuBase_c::executeState_Catch() {
    if (field_0xD46 <= 0) {
        field_0xD71 = true;
        mCallback.field_0x04E = true;

        if (vt_0x208()) {
            return;
        }
    }

    if (field_0xD46 <= 0 && !dAcPy_c::GetLinkM()->IfCurrentActionToActor(this, 0xA9 /* ??? */)) {
        vt_0x220();
        return;
    }

    field_0x88C += dAcPy_c::GetLink()->getSwordPos();
    field_0x88C *= 0.5f;
    if (--field_0xD5E <= 0) {
        vt_0x22C();
        return;
    }

    if (mAnmChrs[0].isStop()) {
        if (field_0xD7E) {
            fn_226_8C90(
                "CatchLoop", ANM_CatchLoop, "CatchLoop", ANM_CatchLoop, "FaceTongue", ANM_FaceTonuge, m3d::PLAY_MODE_4,
                5.0f, 1.0f
            );
        } else {
            fn_226_8C90(
                "CatchLoop", ANM_CatchLoop, "CatchLoop", ANM_CatchLoop, "FaceSmaile", ANM_FaceSmaile, m3d::PLAY_MODE_4,
                5.0f, 1.0f
            );
        }
    }

    sLib::chaseAngle(mRotation.y.ref(), cLib::targetAngleY(mPosition, dAcPy_c::GetLink()->mPosition), 0x500);
}
void dAcGirahimuBase_c::finalizeState_Catch() {
    mCallback.field_0x04E = false;
    field_0xD71 = false;
    mCollider.CoSet();
    mCallback.field_0x2D4 = 1.0f;
    s32 flag = getFromParams(0, 0xFF);
    if (flag != 0xFF) {
        // 0xC0 -> zone temp flag: 0x1 01
        SceneflagManager::sInstance->unsetFlag(mRoomID, flag);
    }
}

void dAcGirahimuBase_c::initializeState_ReleaseSword() {
    field_0xD5A = 0;
    field_0xD66 = 0;

    mCallback.field_0x088 = false;
    field_0xD6C = 1;
    mSph0.ClrTgSet();
    mCyl0.OnTgSet();

    field_0x1834 = 0.0f;
    mSpeed = 0.0f;
}
void dAcGirahimuBase_c::executeState_ReleaseSword() {
    field_0xD81 = true;
    fn_226_CC40();
    mSph0.ClrTgSet();

    sLib::chaseAngle(mRotation.y.ref(), field_0x183E, 0x500);
    mCallback.field_0x04E = false;

    if (mAnmChrs[0].checkFrame(35.0f)) {
        mCallback.field_0x04E = false;
    }
    if (mAnmChrs[0].isStop()) {
        vt_0x220();
    }
}
void dAcGirahimuBase_c::finalizeState_ReleaseSword() {
    mCallback.field_0x2BC = 0;
    mSph0.OnTgSet();
}

void dAcGirahimuBase_c::initializeState_G_SwordDamage() {}
void dAcGirahimuBase_c::executeState_G_SwordDamage() {}
void dAcGirahimuBase_c::finalizeState_G_SwordDamage() {}

void dAcGirahimuBase_c::initializeState_KnifeDamage() {
    mCyl2.OnTgSet();
    fn_226_8B70("KnifeDamage", ANM_KnifeDamage, false, m3d::PLAY_MODE_4, 5.0f, 1.0f);
    mVec3_c pos = mPosition;
    pos.y += 100.0f;
    getSoundSource()->holdSoundAtPosition(SE_Girahim_DMG_KNIFE, pos);

    mCallback.field_0x088 = false;
    field_0xD44 = 0;
    mSpeed = 0.0f;
    field_0x1834 = 0.0f;
    field_0xD6C = 8;
    field_0xD8E = false;
}
void dAcGirahimuBase_c::executeState_KnifeDamage() {
    field_0xD8D = true;
    if (mAnmChrs[0].isStop()) {
        field_0xD8E = false;

        if (field_0x1838 < 500.0f) {
            changeState(StateID_BackStep);
        } else {
            vt_0x220();
        }
    }
}
void dAcGirahimuBase_c::finalizeState_KnifeDamage() {
    field_0xD8E = false;
}

void dAcGirahimuBase_c::initializeState_CatchDamage() {}
void dAcGirahimuBase_c::executeState_CatchDamage() {}
void dAcGirahimuBase_c::finalizeState_CatchDamage() {}

void dAcGirahimuBase_c::initializeState_LinkSwordGuardJust() {
    field_0xD6C = 1;
    mCallback.field_0x2D4 = 1.0f;
    if (field_0x8C8 == ANM_AttackLSwordA) {
        fn_226_8B70("GuardLSwordA", ANM_GuardLSwordA, false, m3d::PLAY_MODE_4, 5.0f, 1.0f);
    } else {
        fn_226_8B70("GuardLSwordB", ANM_GuardLSwordB, false, m3d::PLAY_MODE_4, 5.0f, 1.0f);
    }
    mCallback.field_0x088 = 0;
    field_0xD72 = false;

    mVec3_c throwForce(0.0f, 0.0f, 600.0f);
    throwForce.rotY(mRotation.y - 0x7500);
    throwForce += mPosition;
    mSwordLink.get()->setThrow(throwForce, 0x1200, 29.0f);
}
void dAcGirahimuBase_c::executeState_LinkSwordGuardJust() {
    if (mAnmChrs[0].isStop()) {
        changeState(StateID_SearchSword);
    }
}
void dAcGirahimuBase_c::finalizeState_LinkSwordGuardJust() {}

void dAcGirahimuBase_c::initializeState_GetSword() {
    field_0xD6C = 2;
    field_0xD66 = 0;
    field_0xD71 = true;

    fn_226_8B70("GetSword", ANM_GetSword, false, m3d::PLAY_MODE_4, 5.0f, 1.0f);

    mCallback.field_0x014 = 1.0f;
    mCallback.field_0x088 = true;
    mCallback.field_0x2D4 = 1.0f;
    mCallback.field_0x04E = true;

    s32 flag = getFromParams(0, 0xFF);
    if (flag != 0xFF) {
        // 0xC1 -> zone temp flag: 0x1 02
        SceneflagManager::sInstance->unsetFlag(mRoomID, flag + 1);
    }
}
void dAcGirahimuBase_c::executeState_GetSword() {
    if (vt_0x208()) {
        field_0xD5A = 0;
        mSwordLink.get()->setHide();
        return;
    }

    if (mAnmChrs[0].getFrame() >= 8.0f) {
        field_0x88C = mSwordLink.get()->mPosition;
    }

    if (dAcPy_c::GetLinkM()->IfCurrentActionToActor(this, 0xA9 /* ??? */) && mAnmChrs[0].checkFrame(8.0f)) {
        if (dAcPy_c::GetLinkM()->vt_0x084(this, 0xA9 /* ??? */)) {
            mCallback.field_0x04E = false;
            field_0xD71 = false;
            fn_226_AFB0();
            mSwordLink.get()->setGetSword();
        } else {
            fn_226_D070();
            fn_226_8B70("FreeUp", ANM_FreeUp, false, m3d::PLAY_MODE_4, 2.0f, 1.0f);
            changeState(StateID_ReleaseSword);
            return;
        }
    } else if (mAnmChrs[0].getFrame() < 8.0f && !dAcPy_c::GetLinkM()->IfCurrentActionToActor(this, 0xA9 /* ??? */)) {
        fn_226_D070();
        fn_226_8B70("FreeUp", ANM_FreeUp, false, m3d::PLAY_MODE_4, 2.0f, 1.0f);
        changeState(StateID_ReleaseSword);
        return;
    }

    if (mAnmChrs[0].checkFrame(36.0f)) {
        mVec3_c unused = mCallback.field_0x260 - mPosition;
        unused.rotY(-mRotation.y);

        field_0xD72 = true;
        mSwordLink.get()->setEquip();
        field_0xD4C = 250;
    }
    if (mAnmChrs[0].isStop()) {
        changeState(StateID_Link_SwordWait);
    }
}
void dAcGirahimuBase_c::finalizeState_GetSword() {
    field_0xD5A = 0;
    field_0xD71 = false;
}

void dAcGirahimuBase_c::initializeState_SearchSword() {
    mCallback.field_0x088 = true;
    fn_226_8B70("Walk", ANM_Walk, false, m3d::PLAY_MODE_4, 15.0f, 1.2f);
    mCyl2.OnTgSet();
}
void dAcGirahimuBase_c::executeState_SearchSword() {
    field_0xD7C = false;

    if (mSwordLink.get()->isHide()) {
        mVec3_c distFromHome = mStartingPos - mPosition;
        if (distFromHome.absXZ() > 800.0f) {
            changeState(StateID_HomeWarp);
        } else {
            changeState(StateID_Wait);
        }
        return;
    }

    field_0x88C = mSwordLink.get()->mPosition;
    s16 yTarget = cLib::targetAngleY(mPosition, mSwordLink.get()->mPosition);
    sLib::chaseAngle(mRotation.y.ref(), yTarget, 0x200);
    sLib::chaseAngle(mAngle.y.ref(), yTarget, 0x200);

    s32 f0 = yTarget - mRotation.y;
    f32 f = 1.f - std::abs(mAng(f0).normal_c());
    if (f <= 0.9f) {
        f = 0.0f;
        fn_226_A070(yTarget);
    } else {
        fn_226_8B70("Walk", ANM_Walk, false, m3d::PLAY_MODE_4, 15.0f, 1.2f);
    }
    field_0x1834 = f * 8.0f * mAnmChrs[0].getRate();
    mVec3_c v = mSwordLink.get()->mPosition - mPosition;
    mAng a0 = (s32)v.atan2sX_Z();
    a0 = a0 - mRotation.y;
    if (v.absXZ() < 100.0f && mAng(a0).abs() < 0x4000) {
        changeState(StateID_PickUpSword);
    }
}
void dAcGirahimuBase_c::finalizeState_SearchSword() {
    field_0xD7C = true;
    mSpeed = 0.0f;
    field_0x1834 = 0.0f;
    mCyl2.ClrTgSet();
}

void dAcGirahimuBase_c::initializeState_HomeWarp() {
    mSpeed = 0.0f;
    field_0x1834 = 0.0f;
    fn_226_8B70("MovePose", ANM_MovePose, false, m3d::PLAY_MODE_4, 15.0f, 1.5f);
    mCallback.field_0x088 = true;
    field_0xD44 = 0;
    field_0xD6C = 0;
}
void dAcGirahimuBase_c::executeState_HomeWarp() {
    mSph0.ClrTgSet();
    mCyl2.ClrTgSet();
    mCollider.ClrCo();

    fn_226_BC00();

    switch (field_0xD44) {
        case 0: {
            if (mAnmChrs[0].getFrame() >= 13.0f && sLib::chase(&field_0x8B0.x, 0.0f, 0.2f)) {
                field_0xD44++;
                mVec3_c v0 = dAcPy_c::GetLink()->mPosition - mStartingPos;
                if (v0.absXZ() - 500.0f > 1000.0f) {
                    f32 f = 1000.0f;
                    v0.y = 0.0f;
                    v0.normalizeRS();
                    v0 *= f;
                    v0 += mStartingPos;
                    setPosition(v0);
                    setOldPosition(v0);
                } else {
                    setPosition(mStartingPos);
                    setOldPosition(mStartingPos);
                }
                mRotation.y = cLib::targetAngleY(mPosition, dAcPy_c::GetLink()->mPosition);
                field_0x8B0.set(0.0f, 0.0f, 0.0f);
                field_0xD46 = 20;
            }
        } break;
        case 1: {
            if (field_0xD46 <= 0) {
                field_0xD44++;
                field_0x8B0.set(0.0f, 1.05f, 1.05f);
            }
        } break;
        case 2: {
            mCollider.CoSet();
            if (sLib::chase(&field_0x8B0.x, 1.05f, 0.2f)) {
                if (field_0xD72) {
                    changeState(StateID_Link_SwordWait);
                } else {
                    vt_0x220();
                }
            }
        } break;
    }
}
void dAcGirahimuBase_c::finalizeState_HomeWarp() {
    mCallback.field_0x2BC = 0;
    field_0xD44 = 0;
}

void dAcGirahimuBase_c::initializeState_PickUpSword() {
    field_0xD4C = 250;

    fn_226_8B70("PickUp02", ANM_PickUp02, false, m3d::PLAY_MODE_4, 5.0f, 1.0f);
    mCallback.field_0x088 = true;

    field_0x1834 = 0.0f;
    mSpeed = 0.0f;
}
void dAcGirahimuBase_c::executeState_PickUpSword() {
    if (mSwordLink.get()->isHide()) {
        vt_0x220();
        return;
    }

    field_0x88C = mSwordLink.get()->mPosition;

    if (mAnmChrs[0].checkFrame(16.0f)) {
        field_0xD72 = true;
        mSwordLink.get()->setEquip();
    }

    if (mAnmChrs[0].isStop()) {
        changeState(StateID_Link_SwordWait);
    }
}
void dAcGirahimuBase_c::finalizeState_PickUpSword() {}

void dAcGirahimuBase_c::initializeState_Link_SwordWait() {
    field_0xD6C = 2;
    field_0x1872 = true;
    if (!mbLinkSwordWaitFirstComplete) {
        // Hah, quite the sword you have here. ...
        mFlowMgr.triggerEntryPoint(201, 0x39, 0, 0);
        fn_226_8B70("LookSword", ANM_LookSword, false, m3d::PLAY_MODE_4, 5.0f, 1.0f);
        mCallback.field_0x088 = false;
    } else {
        fn_226_8B70("WaitBtB", ANM_WaitBtB, false, m3d::PLAY_MODE_4, 5.0f, 1.0f);
        mCallback.field_0x088 = true;
    }
    field_0xD72 = true;
    mSwordLink.get()->setEquip();
    field_0xD46 = 10;
    field_0x1834 = 0.0f;
    mSpeed = 0.0f;
}
void dAcGirahimuBase_c::executeState_Link_SwordWait() {
    if (!mbLinkSwordWaitFirstComplete) {
        mFlowMgr.checkFinished();
    }

    mCyl2.SetR(40.0f);
    if (field_0xD4C <= 0) {
        changeState(StateID_ReturnSword);
        return;
    }

    if (field_0x8C8 == ANM_LookSword) {
        if (mAnmChrs[0].isStop()) {
            changeState(StateID_Link_SwordWait);
            return;
        }
    } else if (field_0xD46 <= 0) {
        changeState(StateID_Link_SwordWalk);
        return;
    }

    if (mCallback.field_0x07C.abs() < 0x1500 && field_0x1838 < 200.0f) {
        changeState(StateID_Link_SwordAttack);
    }
}
void dAcGirahimuBase_c::finalizeState_Link_SwordWait() {
    mbLinkSwordWaitFirstComplete = true;
}

void dAcGirahimuBase_c::initializeState_Link_SwordWalk() {
    fn_226_8B70("WalkBt", ANM_Walk, false, m3d::PLAY_MODE_4, 15.0f, 1.2f);
    mCallback.field_0x088 = true;
    field_0xD72 = true;
    mSwordLink.get()->setEquip();
    field_0xD6C = 2;
}
void dAcGirahimuBase_c::executeState_Link_SwordWalk() {
    mCyl2.SetR(60.0f);
    if (field_0xD4C <= 0) {
        changeState(StateID_ReturnSword);
        return;
    }

    sLib::chaseAngle(mRotation.y.ref(), field_0x183E, 0x500);
    s32 f = field_0x183E - mRotation.y;

    f32 f0 = 1.0f - std::abs(mAng(f).normal_c());
    mAngle.y = mRotation.y;

    if (f0 <= 0.6f) {
        fn_226_A070(field_0x183E);
        f0 = 2.0f;
    } else if (field_0x8C8 != ANM_Walk) {
        fn_226_8B70("WalkBt", ANM_Walk, false, m3d::PLAY_MODE_4, 15.0f, 1.2f);
    }
    field_0x1834 = f0 * 8.0f * mAnmChrs[0].getRate();

    if (mCallback.field_0x07C.abs() < 0x1500 && field_0x1838 < 200.0f) {
        changeState(StateID_Link_SwordAttack);
    }
}
void dAcGirahimuBase_c::finalizeState_Link_SwordWalk() {
    mSpeed = 0.0f;
    field_0x1834 = 0.0f;
}

void dAcGirahimuBase_c::initializeState_Link_SwordAttack() {
    if (cM::rndInt(100) > 50) {
        fn_226_8B70("AttackLSwordA", ANM_AttackLSwordA, false, m3d::PLAY_MODE_4, 5.0f, 1.0f);
    } else {
        fn_226_8B70("AttackLSwordB", ANM_AttackLSwordB, false, m3d::PLAY_MODE_4, 5.0f, 1.0f);
    }
    mCallback.field_0x088 = true;
    field_0xD72 = true;
    mSwordLink.get()->setEquip();
    mCallback.field_0x2D4 = 1.0f;
    mCallback.field_0x2D0 = 1.0f;
    mSpeed = 0.0f;
    field_0x1834 = 0.0f;
}
void dAcGirahimuBase_c::executeState_Link_SwordAttack() {
    mCyl2.SetR(90.0f);
    sLib::chase(&field_0x8B0.x, 1.05f, 0.2f);
    if (mAnmChrs[0].getFrame() < 29.0f) {
        sLib::chaseAngle(mRotation.y.ref(), field_0x183E, 0x500);
    }

    if (mAnmChrs[0].checkFrame(30.0f)) {
        mSwordLink.get()->getMdl().setProcActive(true);
        mSwordLink.get()->getMdl().enableAttack();
        return;
    }
    if (mAnmChrs[0].checkFrame(35.0f)) {
        mSwordLink.get()->getMdl().setInactive();
        mSwordLink.get()->getMdl().setProcActive(false);
        return;
    }

    if (mAnmChrs[0].isStop()) {
        changeState(StateID_Link_SwordWait);
        return;
    }
}
void dAcGirahimuBase_c::finalizeState_Link_SwordAttack() {
    mSwordLink.get()->getMdl().setInactive();
    mSwordLink.get()->getMdl().setProcActive(false);
}

void dAcGirahimuBase_c::initializeState_ReturnSword() {
    field_0xD6C = 1;
    fn_226_8B70("LSwordReturn", ANM_LSwordReturn, false, m3d::PLAY_MODE_4, 5.0f, 1.0f);
    mCallback.field_0x088 = true;
    field_0xD72 = true;
    mSwordLink.get()->setEquip();
    mSpeed = 0.0f;
    field_0x1834 = 0.0f;
}
void dAcGirahimuBase_c::executeState_ReturnSword() {
    mCyl2.SetR(60.0f);
    if (mAnmChrs[0].getFrame() < 37.0f) {
        sLib::chaseAngle(mRotation.y.ref(), field_0x183E, 0x500);
    }
    if (mAnmChrs[0].checkFrame(37.0f)) {
        field_0xD72 = false;
        mVec3_c playerOffs(0.0f, 100.0f, 0.0f);
        playerOffs.rotY(dAcPy_c::GetLink()->mRotation.y);
        playerOffs += dAcPy_c::GetLink()->mPosition;
        mVec3_c throwForce = playerOffs - mCallback.field_0x260;
        throwForce.normalizeRS();
        throwForce *= 100.0f;
        mSwordLink.get()->setAtThrow(throwForce);
    }
    if (mAnmChrs[0].isStop()) {
        changeState(StateID_SearchSword);
    }
}
void dAcGirahimuBase_c::finalizeState_ReturnSword() {}

void dAcGirahimuBase_c::initializeState_BackStep() {
    if (field_0x8C8 == ANM_StepEnd) {
        field_0xD7D = false;
        fn_226_AAD0(600.0f, 15.0f);
        fn_226_8B70("StepLoop", ANM_StepLoop, false, m3d::PLAY_MODE_4, 2.0f, 1.0f);
        field_0xD44 = 2;
    } else {
        fn_226_8B70("StepStart", ANM_StepStart, false, m3d::PLAY_MODE_4, 2.0f, 1.0f);
        field_0xD44 = 0;
    }

    mCyl2.ClrTgSet();
    mSph0.ClrTgSet();

    mCallback.field_0x04E = false;
    field_0xD6C = 0;
    mCallback.field_0x088 = true;
    field_0xD46 = 3;
}
void dAcGirahimuBase_c::executeState_BackStep() {}
void dAcGirahimuBase_c::finalizeState_BackStep() {
    field_0xD44 = 0;
    mSpeed = 0.0f;
    field_0x1834 = 0.0f;
    field_0xD7C = true;
    mSph0.OnTgSet();
}

void dAcGirahimuBase_c::initializeState_Escape() {
    mSpeed = 0.0f;
    field_0x1834 = 0.0f;
    fn_226_D070();
    fn_226_8B70("StepEnd", ANM_StepEnd, false, m3d::PLAY_MODE_4, 2.0f, 1.0f);

    mCyl2.ClrTgSet();
    mSph0.ClrTgSet();

    mCallback.field_0x04E = true;
    mCallback.field_0x2D4 = 0.8f;
    mCallback.field_0x2D0 = 0.8f;

    field_0xD44 = 0;
    field_0xD4E = 0;
    field_0xD6C = 8;
    field_0xD60 = 2;
}
void dAcGirahimuBase_c::executeState_Escape() {}
void dAcGirahimuBase_c::finalizeState_Escape() {
    field_0xD44 = 0;
    mSpeed = 0.0f;
    field_0x1834 = 0.0f;
    field_0xD7C = true;
    field_0xD81 = false;
    mSph0.OnTgSet();
    field_0xD66 = 0;
}

void dAcGirahimuBase_c::initializeState_EscapeBack() {
    if (field_0xD7B) {
        fn_226_8B70("WaitBtC", ANM_WaitBtC, false, m3d::PLAY_MODE_4, 15.0f, 1.0f);
    } else {
        fn_226_8B70("WaitA", ANM_WaitA, false, m3d::PLAY_MODE_4, 15.0f, 1.0f);
    }

    mCyl2.ClrTgSet();
    mSph0.ClrTgSet();
    mCallback.field_0x04E = true;
    mCallback.field_0x2D4 = 0.8f;
    mCallback.field_0x2D0 = 0.8f;

    field_0xD44 = 0;
    field_0xD4E = 0;
    field_0xD6C = 1;
    field_0xD46 = 15;
    mCallback.field_0x05E = 0xA80;
}
void dAcGirahimuBase_c::executeState_EscapeBack() {
    mCyl2.ClrTgSet();
    mSph0.ClrTgSet();
    if (field_0xD46 <= 0) {
        vt_0x220();
    }
    sLib::chaseAngle(mRotation.y.ref(), field_0x183E, 0x500);
}
void dAcGirahimuBase_c::finalizeState_EscapeBack() {
    field_0xD44 = 0;
    mSpeed = 0.0f;
    field_0x1834 = 0.0f;
    mSph0.OnTgSet();
}

void dAcGirahimuBase_c::initializeState_FrontWarp() {
    fn_226_8B70("MovePose", ANM_MovePose, false, m3d::PLAY_MODE_4, 15.0f, 2.0f);
    mCallback.field_0x088 = true;
    field_0xD44 = 0;
    mSpeed = 0.0f;
    field_0x1834 = 0.0f;
    field_0xD6C = 0;
}
void dAcGirahimuBase_c::executeState_FrontWarp() {}
void dAcGirahimuBase_c::finalizeState_FrontWarp() {
    mCollider.CoSet();
    field_0xD44 = 0;
    mCyl2.OnTgSet();
}

void dAcGirahimuBase_c::initializeState_FrontAttack() {
    field_0x1834 = 0.0f;
    mSpeed = 0.0f;
    mCallback.field_0x088 = true;
    field_0xD6C = 0;
    if (vt_0x218()) {
        field_0xD46 = 40;
    } else {
        field_0xD46 = 50;
    }
}
void dAcGirahimuBase_c::executeState_FrontAttack() {}
void dAcGirahimuBase_c::finalizeState_FrontAttack() {}

void dAcGirahimuBase_c::initializeState_BackWarp() {
    fn_226_8B70("MovePose", ANM_MovePose, false, m3d::PLAY_MODE_4, 15.0f, 2.0f);
    mCallback.field_0x088 = true;
    field_0xD44 = 0;
    mCyl2.ClrTgSet();
    mSpeed = 0.0f;
    field_0x1834 = 0.0f;
    field_0xD6C = 0;
}
void dAcGirahimuBase_c::executeState_BackWarp() {}
void dAcGirahimuBase_c::finalizeState_BackWarp() {
    mCollider.CoSet();
    field_0xD44 = 0;
    mCyl2.OnTgSet();
}

void dAcGirahimuBase_c::initializeState_BackAttack() {
    field_0x1834 = 0.0f;
    mSpeed = 0.0f;
    vt_0x224();
    mCallback.field_0x088 = true;
    field_0xD6C = 0;
    field_0xD46 = 50;
}
void dAcGirahimuBase_c::executeState_BackAttack() {}
void dAcGirahimuBase_c::finalizeState_BackAttack() {}

void dAcGirahimuBase_c::initializeState_Counter() {
    field_0xD7E = false;
    if (field_0x8C8 == ANM_PoseR) {
        fn_226_8B70("PoseLAttack", ANM_PoseLAttack, false, m3d::PLAY_MODE_4, 5.0f, 1.0f);
    } else if (field_0x8C8 == ANM_PoseL) {
        fn_226_8B70("PoseRAttack", ANM_PoseRAttack, false, m3d::PLAY_MODE_4, 5.0f, 1.0f);
    } else if (field_0x8C8 == ANM_PoseC) {
        fn_226_8B70("PoseCAttack", ANM_PoseCAttack2, false, m3d::PLAY_MODE_4, 5.0f, 1.0f);
    } else if (field_0x8C8 == ANM_PoseC2) {
        fn_226_8B70("PoseCAttack", ANM_PoseCAttack, false, m3d::PLAY_MODE_4, 5.0f, 1.0f);
    } else {
        int rnd = cM::rndInt(90);
        if (rnd > 60) {
            fn_226_8B70("PoseLAttack", ANM_PoseLAttack, false, m3d::PLAY_MODE_4, 5.0f, 1.0f);
        } else if (rnd > 30) {
            fn_226_8B70("PoseRAttack", ANM_PoseRAttack, false, m3d::PLAY_MODE_4, 5.0f, 1.0f);
        } else {
            fn_226_8B70("PoseCAttack", ANM_PoseC, false, m3d::PLAY_MODE_4, 5.0f, 1.0f);
        }
        field_0xD7E = true;
    }
    field_0x1834 = 0.0f;
    mSpeed = 0.0f;
    mCallback.field_0x088 = true;
    field_0xD6C = 5;
}
void dAcGirahimuBase_c::executeState_Counter() {}
void dAcGirahimuBase_c::finalizeState_Counter() {
    mMdlSwordA.setInactive();
    mMdlSwordA.setProcActive(false);

    mCyl2.OnTgSet();
    field_0xD82 = false;
}

void dAcGirahimuBase_c::initializeState_Run() {
    field_0xD6C = 3;
    field_0xD44 = 0;

    if (cM::rndInt(100) > 50) {
        fn_226_8B70("AttackPoseL", ANM_AttackPoseL, false, m3d::PLAY_MODE_4, 5.0f, 1.0f);
    } else {
        fn_226_8B70("AttackPoseR", ANM_AttackPoseR, false, m3d::PLAY_MODE_4, 5.0f, 1.0f);
    }
    mCallback.field_0x088 = false;
    field_0x1834 = 0.0f;
}
void dAcGirahimuBase_c::executeState_Run() {}
void dAcGirahimuBase_c::finalizeState_Run() {
    field_0x1834 = 0.0f;
    field_0xD81 = false;
}

void dAcGirahimuBase_c::initializeState_RunAttack() {
    field_0xD6C = 4;
    if (fn_226_CC80()) {
        fn_226_8B70("AttackSwordL", ANM_AttackSwordL, false, m3d::PLAY_MODE_4, 5.0f, 1.0f);
    } else {
        fn_226_8B70("AttackSwordR", ANM_AttackSwordR, false, m3d::PLAY_MODE_4, 5.0f, 1.0f);
    }
    mCallback.field_0x088 = true;
    field_0x1834 = 0.0f;
    field_0x1838 = 0.2f;
}
void dAcGirahimuBase_c::executeState_RunAttack() {}
void dAcGirahimuBase_c::finalizeState_RunAttack() {
    mMdlSwordA.setInactive();
    mMdlSwordA.setProcActive(false);
    field_0x1858 = 0.5f;
}

void dAcGirahimuBase_c::initializeState_BackWalk() {
    field_0xD80 = false;
    field_0xD6C = 6;
    fn_226_8B70("WalkBt", ANM_Walk, false, m3d::PLAY_MODE_2, 15.0f, 1.0f);
    mCallback.field_0x088 = true;
    field_0xD46 = 50;
    field_0x1834 = mAnmChrs[0].getRate() * -8.0f;
}
void dAcGirahimuBase_c::executeState_BackWalk() {
    if (field_0xD46 <= 0) {
        vt_0x220();
        return;
    }
    fn_226_B960(6.0f);
}
void dAcGirahimuBase_c::finalizeState_BackWalk() {}

dAcGirahimuBase_c::~dAcGirahimuBase_c() {
    if (mpSoundSource != nullptr) {
        if (mpSoundSource->hasPlayingSounds()) {
            do {
                VIWaitForRetrace();
            } while (mpSoundSource->hasPlayingSounds());
        }
    }
}

void dAcGirahimuBase_c::initNodeData() {
    nw4r::g3d::ResMdl mdl = mMdlBody.getResMdl();
    mCallback.mNodeID_ShoulderR = mdl.GetResNode("ShoulderR").GetID();
    mCallback.mNodeID_ShoulderL = mdl.GetResNode("ShoulderL").GetID();
    mCallback.mNodeID_Hip = mdl.GetResNode("Hip").GetID();
    mCallback.mNodeID_ArmR = mdl.GetResNode("ArmR").GetID();
    mCallback.mNodeID_ArmL = mdl.GetResNode("ArmL").GetID();
    mCallback.mNodeID_ElbowR = mdl.GetResNode("ElbowR").GetID();
    mCallback.mNodeID_ElbowL = mdl.GetResNode("ElbowL").GetID();
    mCallback.mNodeID_HandR = mdl.GetResNode("HandR").GetID();
    mCallback.mNodeID_Head = mdl.GetResNode("Head").GetID();
    mCallback.mNodeID_Neck = mdl.GetResNode("Neck").GetID();
    mCallback.mNodeID_Hair1 = mdl.GetResNode("Hair1").GetID();
    mCallback.mNodeID_Hair2 = mdl.GetResNode("Hair2").GetID();
    mCallback.mNodeID_Hair3 = mdl.GetResNode("Hair3").GetID();
    mCallback.mNodeID_Hair4 = mdl.GetResNode("Hair4").GetID();
    mCallback.mNodeID_Spine1 = mdl.GetResNode("Spine1").GetID();
    mCallback.mNodeID_Spine2 = mdl.GetResNode("Spine2").GetID();
    mCallback.mNodeID_BrowL = mdl.GetResNode("BrowL").GetID();
    mCallback.mNodeID_BrowR = mdl.GetResNode("BrowR").GetID();
    mCallback.mNodeID_Chin = mdl.GetResNode("Chin").GetID();
    mCallback.mNodeID_MouthL = mdl.GetResNode("MouthL").GetID();
    mCallback.mNodeID_MouthR = mdl.GetResNode("MouthR").GetID();
    mCallback.mNodeID_Upperjaw = mdl.GetResNode("Upperjaw").GetID();
    mCallback.mNodeID_SklRoot = mdl.GetResNode("SklRoot").GetID();
    mCallback.mNodeID_KneeR = mdl.GetResNode("KneeR").GetID();
    mCallback.mNodeID_KneeL = mdl.GetResNode("KneeL").GetID();
    mCallback.mNodeID_LegR = mdl.GetResNode("LegR").GetID();
    mCallback.mNodeID_LegL = mdl.GetResNode("LegL").GetID();

    mVec3_c nodeTranslation = mdl.GetResNode("ElbowR").GetTranslate();
    mCallback.field_0x234 = nodeTranslation.mag() * 1.05f;
    mCallback.field_0x17C = mdl.GetResNode("ElbowR").GetScale();

    nodeTranslation = mdl.GetResNode("HandR").GetTranslate();
    mCallback.field_0x188 = nodeTranslation.mag() * 1.05f;
    mCallback.field_0x228 = mdl.GetResNode("ArmR").GetScale();
}

// 64B0
bool dAcGirahimuBase_c::createHeap() {}

// 6A10
int dAcGirahimuBase_c::actorPostCreate() {}

// 6AC0
int dAcGirahimuBase_c::doDelete() {}

// 6AD0
int dAcGirahimuBase_c::preExecute() {}

// 6B50
void dAcGirahimuBase_c::vt_0x1DC() {}

// 90C0
int dAcGirahimuBase_c::draw() {}

// 9520
void dAcGirahimuBase_c::vt_0x1E0() {}

// 9900
void dAcGirahimuBase_c::vt_0x1E4() {}

// 9B30
void dAcGirahimuBase_c::vt_0x1E8() {}

// 9CA0
void dAcGirahimuBase_c::vt_0x1EC() {}

// 9B20
void dAcGirahimuBase_c::vt_0x1F0() {}

// 9F60
void dAcGirahimuBase_c::vt_0x21C() {}

// AE30
void dAcGirahimuBase_c::vt_0x1F4() {}

// AEB0
void dAcGirahimuBase_c::vt_0x1F8() {}

// B0E0
void dAcGirahimuBase_c::fn_226_CC40() {
    mCyl0.SetTgType(
        ~(AT_TYPE_PHYSICS | AT_TYPE_0x40 | AT_TYPE_SLINGSHOT | AT_TYPE_WIND | AT_TYPE_DAMAGE | AT_TYPE_WHIP |
          AT_TYPE_0x8000 | AT_TYPE_BELLOWS | AT_TYPE_GLITTERING_SPORES | AT_TYPE_BEETLE | AT_TYPE_BUGNET)
    );
}

// B100
void dAcGirahimuBase_c::vt_0x220() {}

// B150
void dAcGirahimuBase_c::vt_0x200() {}

// B190
void dAcGirahimuBase_c::vt_0x204() {}

// B1C0
void dAcGirahimuBase_c::vt_0x228() {}

// B300
bool dAcGirahimuBase_c::vt_0x208() {}

// B4D0
void dAcGirahimuBase_c::vt_0x20C() {}

// B4F0
void dAcGirahimuBase_c::vt_0x22C() {}

// B740
void dAcGirahimuBase_c::vt_0x214() {}
