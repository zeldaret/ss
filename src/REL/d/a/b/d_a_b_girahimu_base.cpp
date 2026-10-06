#include "d/a/b/d_a_b_girahimu_base.h"

#include "c/c_lib.h"
#include "c/c_math.h"
#include "common.h"
#include "d/a/d_a_player.h"
#include "d/a/e/d_a_en_base.h"
#include "d/a/obj/d_a_obj_base.h"
#include "d/a/obj/d_a_obj_girahimu_sword_link.h"
#include "d/col/bg/d_bg_s_lin_chk.h"
#include "d/col/c/c_cc_d.h"
#include "d/col/cc/d_cc_d.h"
#include "d/d_cc.h"
#include "d/d_stage_mgr.h"
#include "d/d_vec.h"
#include "d/flag/sceneflag_manager.h"
#include "d/lyt/d_lyt_boss_caption.h"
#include "d/lyt/msg_window/d_lyt_msg_window.h"
#include "d/snd/d_snd_wzsound.h"
#include "egg/math/eggMath.h"
#include "f/f_base.h"
#include "f/f_profile_name.h"
#include "m/m3d/m3d.h"
#include "m/m3d/m_fanm.h"
#include "m/m3d/m_shadow.h"
#include "m/m_angle.h"
#include "m/m_mtx.h"
#include "m/m_quat.h"
#include "m/m_sphere.h"
#include "m/m_vec.h"
#include "nw4r/g3d/g3d_anmchr.h"
#include "nw4r/g3d/res/g3d_resanmchr.h"
#include "nw4r/g3d/res/g3d_resanmtexpat.h"
#include "nw4r/g3d/res/g3d_resfile.h"
#include "nw4r/g3d/res/g3d_resmdl.h"
#include "nw4r/math/math_arithmetic.h"
#include "nw4r/math/math_types.h"
#include "rvl/MTX/mtx.h"
#include "rvl/VI/vi.h"
#include "s/s_Math.h"
#include "sized_string.h"
#include "toBeSorted/attention.h"
#include "toBeSorted/d_emitter.h"

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

static dCcD_SrcCps sSrcCps = {
    {
     {AT_TYPE_DAMAGE, 0x3E, {0, 0, 0}, 4, 0, 0, 0, CUT_DIR_NONE, 0},
     {~(AT_TYPE_COMMON0), 0x102, {0, 0, 0x40F}, 0, CUT_DIR_NONE},
     {0xE5},
     },
    {30.0f},
};

static dCcD_SrcCyl sSrcCyl = {
    {
     {AT_TYPE_DAMAGE, 0x3E, {0, 0, 1}, 2, 0, 0, 0, CUT_DIR_NONE, 0},
     {~(AT_TYPE_COMMON0 & ~AT_TYPE_WIND), 0x102, {0, 0, 0x44F}, 8, CUT_DIR_NONE},
     {0xE5},
     },
    {
     120.0f, 220.f,
     }
};
static dCcD_SrcSph sSrcSph = {
    {
     {AT_TYPE_DAMAGE, 0x3E, {0, 0, 0}, 4, 0, 0, 0, CUT_DIR_NONE, 0},
     {AT_TYPE_SWORD, 0x103, {0, 1, 0x44F}, 0, CUT_DIR_NONE},
     {0xE5},
     },
    {40.f}
};
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
                if (mpGhirahim->mAnmIDBody != ANM_WaitA) {
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
        mAng3_c a0(field_0x05A, field_0x058, 0);
        fn_226_DC0(m0, a0, field_0x060);
        m0.XrotM(field_0x062.x);
        result->SetMtx(m0);
    } else if (node == mNodeID_Spine2) {
        result->GetMtx(m0);
        mAng3_c a0(field_0x05A, field_0x058, 0);
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
        mAng3_c a0(0, field_0x054 / 2, 0);
        fn_226_DC0(m0, a0, mpGhirahim->mRotation.y);
        result->SetMtx(m0);
    } else if (node == mNodeID_KneeR) {
        result->GetMtx(m0);
        mAng3_c a0(0, field_0x054 / 2, 0);
        fn_226_DC0(m0, a0, mpGhirahim->mRotation.y);
        result->SetMtx(m0);
    } else if (node == mNodeID_LegL) {
        result->GetMtx(m0);
        mAng3_c a0(0, field_0x054 / 2, 0);
        fn_226_DC0(m0, a0, mpGhirahim->mRotation.y);
        result->SetMtx(m0);
    } else if (node == mNodeID_LegR) {
        result->GetMtx(m0);
        mAng3_c a0(0, field_0x054 / 2, 0);
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
    setAnm("WaitA", 0, nullptr, m3d::PLAY_MODE_4, 30.0f, 1.0f);
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

        setAnmHip("WaitA", ANM_WaitA, m3d::PLAY_MODE_4, 5.0f, 1.0f);
    }

    if (mAnmIDShoulderL == ANM_MovePose && mAnmChrs[ANMIDX_ShoulderL].checkFrame(43.0f)) {
        setAnmShoulderL("WaitA", ANM_WaitA, m3d::PLAY_MODE_4, 15.0f, 1.0f);
        setAnmHead("WaitA", ANM_WaitA, m3d::PLAY_MODE_4, 15.0f, 1.0f);
    }
    if (field_0xD48 <= 0) {
        setAnmShoulderL("MovePose", ANM_MovePose, m3d::PLAY_MODE_4, 15.0f, 1.0f);
        setAnmHead("MovePose", ANM_MovePose, m3d::PLAY_MODE_4, 15.0f, 1.0f);

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
    setAnm("Walk", ANM_Walk, nullptr, m3d::PLAY_MODE_4, 15.0f, 1.0f);

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
        setAnmShoulderR("WaitA", ANM_WaitA, m3d::PLAY_MODE_4, 15.0f, 1.0f);
    } else {
        mCallback.field_0x2BC = 0;
        mCallback.field_0x098 = 10.0f;
        setAnmShoulderR("Walk", ANM_Walk, m3d::PLAY_MODE_4, 15.0f, 1.0f);
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
    setAnm("AttackBinta", ANM_AttackBinta, nullptr, m3d::PLAY_MODE_4, 5.0f, 1.0f);
    mCallback.field_0x088 = true;
    field_0x1834 = 0.0f;
    mSpeed = 0.0f;
}
void dAcGirahimuBase_c::executeState_Panch() {
    field_0xD58 = 60;
    sLib::chaseAngle(mRotation.y.ref(), field_0x183E, 0x500);
    if (mAnmChrs[ANMIDX_Body].checkFrame(10.0f)) {
        mSph0.SetR(100.0f);
        mSph0.ClrCoSet();
        mSph0.OnAtSet();
        mSph0.SetAtFlagsUpper(0x40000);
        return;
    }
    if (mAnmChrs[ANMIDX_Body].checkFrame(22.0f)) {
        mSph0.SetR(40.0f);
        mSph0.ClrAtSet();
        mSph0.SetAtFlagsUpper(0);
        return;
    }

    if (mAnmChrs[ANMIDX_Body].isStop()) {
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
        setAnm("Catch", ANM_Catch, "Catch", ANM_Catch, "FaceTongue", ANM_FaceTonuge, m3d::PLAY_MODE_4, 5.0f, 1.0f);
    } else {
        setAnm("Catch", ANM_Catch, "Catch", ANM_Catch, "FaceSmaile", ANM_FaceSmaile, m3d::PLAY_MODE_4, 5.0f, 1.0f);
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

    if (mAnmChrs[ANMIDX_Body].isStop()) {
        if (field_0xD7E) {
            setAnm(
                "CatchLoop", ANM_CatchLoop, "CatchLoop", ANM_CatchLoop, "FaceTongue", ANM_FaceTonuge, m3d::PLAY_MODE_4,
                5.0f, 1.0f
            );
        } else {
            setAnm(
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

    if (mAnmChrs[ANMIDX_Body].checkFrame(35.0f)) {
        mCallback.field_0x04E = false;
    }
    if (mAnmChrs[ANMIDX_Body].isStop()) {
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
    setAnm("KnifeDamage", ANM_KnifeDamage, nullptr, m3d::PLAY_MODE_4, 5.0f, 1.0f);
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
    if (mAnmChrs[ANMIDX_Body].isStop()) {
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
    if (mAnmIDBody == ANM_AttackLSwordA) {
        setAnm("GuardLSwordA", ANM_GuardLSwordA, nullptr, m3d::PLAY_MODE_4, 5.0f, 1.0f);
    } else {
        setAnm("GuardLSwordB", ANM_GuardLSwordB, nullptr, m3d::PLAY_MODE_4, 5.0f, 1.0f);
    }
    mCallback.field_0x088 = 0;
    field_0xD72 = false;

    mVec3_c throwForce(0.0f, 0.0f, 600.0f);
    throwForce.rotY(mRotation.y - 0x7500);
    throwForce += mPosition;
    mSwordLink.get()->setThrow(throwForce, 0x1200, 29.0f);
}
void dAcGirahimuBase_c::executeState_LinkSwordGuardJust() {
    if (mAnmChrs[ANMIDX_Body].isStop()) {
        changeState(StateID_SearchSword);
    }
}
void dAcGirahimuBase_c::finalizeState_LinkSwordGuardJust() {}

void dAcGirahimuBase_c::initializeState_GetSword() {
    field_0xD6C = 2;
    field_0xD66 = 0;
    field_0xD71 = true;

    setAnm("GetSword", ANM_GetSword, nullptr, m3d::PLAY_MODE_4, 5.0f, 1.0f);

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

    if (mAnmChrs[ANMIDX_Body].getFrame() >= 8.0f) {
        field_0x88C = mSwordLink.get()->mPosition;
    }

    if (dAcPy_c::GetLinkM()->IfCurrentActionToActor(this, 0xA9 /* ??? */) && mAnmChrs[ANMIDX_Body].checkFrame(8.0f)) {
        if (dAcPy_c::GetLinkM()->vt_0x084(this, 0xA9 /* ??? */)) {
            mCallback.field_0x04E = false;
            field_0xD71 = false;
            fn_226_AFB0();
            mSwordLink.get()->setGetSword();
        } else {
            fn_226_D070();
            setAnm("FreeUp", ANM_FreeUp, nullptr, m3d::PLAY_MODE_4, 2.0f, 1.0f);
            changeState(StateID_ReleaseSword);
            return;
        }
    } else if (mAnmChrs[ANMIDX_Body].getFrame() < 8.0f &&
               !dAcPy_c::GetLinkM()->IfCurrentActionToActor(this, 0xA9 /* ??? */)) {
        fn_226_D070();
        setAnm("FreeUp", ANM_FreeUp, nullptr, m3d::PLAY_MODE_4, 2.0f, 1.0f);
        changeState(StateID_ReleaseSword);
        return;
    }

    if (mAnmChrs[ANMIDX_Body].checkFrame(36.0f)) {
        mVec3_c unused = mCallback.field_0x260 - mPosition;
        unused.rotY(-mRotation.y);

        field_0xD72 = true;
        mSwordLink.get()->setEquip();
        field_0xD4C = 250;
    }
    if (mAnmChrs[ANMIDX_Body].isStop()) {
        changeState(StateID_Link_SwordWait);
    }
}
void dAcGirahimuBase_c::finalizeState_GetSword() {
    field_0xD5A = 0;
    field_0xD71 = false;
}

void dAcGirahimuBase_c::initializeState_SearchSword() {
    mCallback.field_0x088 = true;
    setAnm("Walk", ANM_Walk, nullptr, m3d::PLAY_MODE_4, 15.0f, 1.2f);
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
        setAnm("Walk", ANM_Walk, nullptr, m3d::PLAY_MODE_4, 15.0f, 1.2f);
    }
    field_0x1834 = f * 8.0f * mAnmChrs[ANMIDX_Body].getRate();
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
    setAnm("MovePose", ANM_MovePose, nullptr, m3d::PLAY_MODE_4, 15.0f, 1.5f);
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
            if (mAnmChrs[ANMIDX_Body].getFrame() >= 13.0f && sLib::chase(&field_0x8B0.x, 0.0f, 0.2f)) {
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

    setAnm("PickUp02", ANM_PickUp02, nullptr, m3d::PLAY_MODE_4, 5.0f, 1.0f);
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

    if (mAnmChrs[ANMIDX_Body].checkFrame(16.0f)) {
        field_0xD72 = true;
        mSwordLink.get()->setEquip();
    }

    if (mAnmChrs[ANMIDX_Body].isStop()) {
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
        setAnm("LookSword", ANM_LookSword, nullptr, m3d::PLAY_MODE_4, 5.0f, 1.0f);
        mCallback.field_0x088 = false;
    } else {
        setAnm("WaitBtB", ANM_WaitBtB, nullptr, m3d::PLAY_MODE_4, 5.0f, 1.0f);
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

    if (mAnmIDBody == ANM_LookSword) {
        if (mAnmChrs[ANMIDX_Body].isStop()) {
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
    setAnm("WalkBt", ANM_Walk, nullptr, m3d::PLAY_MODE_4, 15.0f, 1.2f);
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
    } else if (mAnmIDBody != ANM_Walk) {
        setAnm("WalkBt", ANM_Walk, nullptr, m3d::PLAY_MODE_4, 15.0f, 1.2f);
    }
    field_0x1834 = f0 * 8.0f * mAnmChrs[ANMIDX_Body].getRate();

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
        setAnm("AttackLSwordA", ANM_AttackLSwordA, nullptr, m3d::PLAY_MODE_4, 5.0f, 1.0f);
    } else {
        setAnm("AttackLSwordB", ANM_AttackLSwordB, nullptr, m3d::PLAY_MODE_4, 5.0f, 1.0f);
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
    if (mAnmChrs[ANMIDX_Body].getFrame() < 29.0f) {
        sLib::chaseAngle(mRotation.y.ref(), field_0x183E, 0x500);
    }

    if (mAnmChrs[ANMIDX_Body].checkFrame(30.0f)) {
        mSwordLink.get()->getMdl().setProcActive(true);
        mSwordLink.get()->getMdl().enableAttack();
        return;
    }
    if (mAnmChrs[ANMIDX_Body].checkFrame(35.0f)) {
        mSwordLink.get()->getMdl().setInactive();
        mSwordLink.get()->getMdl().setProcActive(false);
        return;
    }

    if (mAnmChrs[ANMIDX_Body].isStop()) {
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
    setAnm("LSwordReturn", ANM_LSwordReturn, nullptr, m3d::PLAY_MODE_4, 5.0f, 1.0f);
    mCallback.field_0x088 = true;
    field_0xD72 = true;
    mSwordLink.get()->setEquip();
    mSpeed = 0.0f;
    field_0x1834 = 0.0f;
}
void dAcGirahimuBase_c::executeState_ReturnSword() {
    mCyl2.SetR(60.0f);
    if (mAnmChrs[ANMIDX_Body].getFrame() < 37.0f) {
        sLib::chaseAngle(mRotation.y.ref(), field_0x183E, 0x500);
    }
    if (mAnmChrs[ANMIDX_Body].checkFrame(37.0f)) {
        field_0xD72 = false;
        mVec3_c playerOffs(0.0f, 100.0f, 0.0f);
        playerOffs.rotY(dAcPy_c::GetLink()->mRotation.y);
        playerOffs += dAcPy_c::GetLink()->mPosition;
        mVec3_c throwForce = playerOffs - mCallback.field_0x260;
        throwForce.normalizeRS();
        throwForce *= 100.0f;
        mSwordLink.get()->setAtThrow(throwForce);
    }
    if (mAnmChrs[ANMIDX_Body].isStop()) {
        changeState(StateID_SearchSword);
    }
}
void dAcGirahimuBase_c::finalizeState_ReturnSword() {}

void dAcGirahimuBase_c::initializeState_BackStep() {
    if (mAnmIDBody == ANM_StepEnd) {
        field_0xD7D = false;
        calcJumpMovement(600.0f, 15.0f);
        setAnm("StepLoop", ANM_StepLoop, nullptr, m3d::PLAY_MODE_4, 2.0f, 1.0f);
        field_0xD44 = 2;
    } else {
        setAnm("StepStart", ANM_StepStart, nullptr, m3d::PLAY_MODE_4, 2.0f, 1.0f);
        field_0xD44 = 0;
    }

    mCyl2.ClrTgSet();
    mSph0.ClrTgSet();

    mCallback.field_0x04E = false;
    field_0xD6C = 0;
    mCallback.field_0x088 = true;
    field_0xD46 = 3;
}
void dAcGirahimuBase_c::executeState_BackStep() {
    s32 _weird_zero = 0;

    field_0xD7C = false;
    mCyl2.ClrAtSet();
    mSph0.ClrAtSet();

    switch (field_0xD44) {
        case 0: {
            field_0xD7D = false;
            if (mAng(mRotation.y - field_0x183E).abs() < 0x4000 && field_0xD56 <= 0) {
                fn_226_D070();
                setAnm("StepStart", ANM_StepStart, nullptr, m3d::PLAY_MODE_4, 2.0f, 1.0f);
                field_0xD44++;
            }
        } break;
        case 1: {
            field_0xD7D = false;
            if (mAnmChrs[ANMIDX_Body].checkFrame(1.0f)) {
                calcJumpMovement(600.0f, _weird_zero + 15.0f);
                setAnm("StepLoop", ANM_StepLoop, nullptr, m3d::PLAY_MODE_4, 2.0f, 1.0f);
                field_0xD44++;
            }
        } break;
        case 2: {
            field_0xD7D = false;
            if (mVelocity.y <= 0.0f && mAcch.ChkGndHit() && mPosition.y <= mStartingPos.y + 100.0f) {
                dJEffManager_c::spawnGroundEffect(mPosition, mPolyAttr0, mPolyAttr1, field_0x1B4, 0, 1.0f, field_0x1B0);
                mAngle.y = mVelocity.atan2sX_Z();
                field_0x1834 = 0.0f;
                mSpeed = mVelocity.absXZ();
                mVelocity.set(0.0f, 0.0f, 0.0f);
                setAnm("StepEnd", ANM_StepEnd, nullptr, m3d::PLAY_MODE_4, 2.0f, 1.0f);
                field_0xD44++;
            }
        } break;
        case 3: {
            field_0x1838 = 0.1f;
            if (mAnmChrs[ANMIDX_Body].isStop()) {
                vt_0x220();
            }
        } break;
    }

    sLib::chaseAngle(mRotation.y.ref(), field_0x183E, 0x2000);
}
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
    setAnm("StepEnd", ANM_StepEnd, nullptr, m3d::PLAY_MODE_4, 2.0f, 1.0f);

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
void dAcGirahimuBase_c::executeState_Escape() {
    field_0xD81 = true;
    field_0xD60 = 2;
    fn_226_CC40();
    mCyl2.ClrTgSet();
    mSph0.ClrTgSet();
    mCallback.field_0x04E = false;
    if (mAnmChrs[ANMIDX_Body].getFrame() >= 15.0f) {
        if (dAcPy_c::GetLinkM()->isUsingSword()) {
            mCallback.field_0x2D4 = 0.8f;
            mCallback.field_0x2D0 = 0.8f;
        } else {
            mCallback.field_0x2D4 = 1.0f;
        }
    }

    if (mAnmChrs[ANMIDX_Body].isStop()) {
        vt_0x220();
    }

    sLib::chaseAngle(mRotation.y.ref(), field_0x183E, 0x2000);
    field_0xD58 = 60;
    field_0xD68 = 0;
    mCallback.field_0x2BC = 0;
}
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
        setAnm("WaitBtC", ANM_WaitBtC, nullptr, m3d::PLAY_MODE_4, 15.0f, 1.0f);
    } else {
        setAnm("WaitA", ANM_WaitA, nullptr, m3d::PLAY_MODE_4, 15.0f, 1.0f);
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
    setAnm("MovePose", ANM_MovePose, nullptr, m3d::PLAY_MODE_4, 15.0f, 2.0f);
    mCallback.field_0x088 = true;
    field_0xD44 = 0;
    mSpeed = 0.0f;
    field_0x1834 = 0.0f;
    field_0xD6C = 0;
}
void dAcGirahimuBase_c::executeState_FrontWarp() {
    mCyl2.ClrTgSet();
    switch (field_0xD44) {
        case 0: {
            if (mAnmChrs[ANMIDX_Body].getFrame() >= 18.0f) {
                mCollider.ClrCo();
                if (sLib::chase(&field_0x8B0.x, 0.0f, 0.2f) != 0) {
                    field_0xD44++;
                    field_0x8B0.set(0.0f, 0.0f, 0.0f);
                    field_0xD46 = 20 + cM::rndInt(30);
                    vt_0x1F8();
                }
            }
        } break;
        case 1: {
            mRotation.y = cLib::targetAngleY(mPosition, dAcPy_c::GetLink()->mPosition);
            if (field_0xD46 <= 0) {
                mVec3_c start = mPosition;
                mVec3_c end = mPosition;
                for (int i = 0; i < 4; ++i) {
                    mAng yRot = (i * 0x4000) + dAcPy_c::GetLink()->mRotation.y;
                    start.set(dAcPy_c::GetLink()->mPosition);
                    start.y += 100.0f;
                    end.set(0.0f, 0.0f, 130.0f);
                    end.rotY(yRot);
                    end += start;
                    if (!dBgS_ObjLinChk::LineCross(&start, &end, nullptr)) {
                        break;
                    }
                }
                end.y = mPosition.y;
                setPosition(end);
                setOldPosition(end);
                mRotation.y = cLib::targetAngleY(mPosition, dAcPy_c::GetLink()->mPosition);
                field_0x8B0.set(0.0f, 1.05f, 1.05f);
                field_0xD44++;
                vt_0x20C();
                return;
            }

            if (!turn(field_0x183E)) {
                vt_0x228();
            }
        } break;
    }
    sLib::chaseAngle(mRotation.y.ref(), field_0x183E, 0x500);
}
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
void dAcGirahimuBase_c::executeState_FrontAttack() {
    mCollider.ClrCo();
    if (field_0x8B0.x >= 0.5f) {
        mCollider.CoSet();
    }
    if (field_0xD8C) {
        if (!fn_226_A120(field_0x183E, 200.0f)) {
            vt_0x228();
        }
        sLib::chaseAngle(mRotation.y.ref(), field_0x183E, 0x500);
    } else {
        if (!turn(field_0x183E)) {
            vt_0x228();
        }
    }

    if (sLib::chase(&field_0x8B0.x, 1.05f, 0.2f) != FALSE) {
        field_0xD6C = 5;
    }

    sLib::chaseAngle(mRotation.y.ref(), field_0x183E, 0x500);
    if (field_0xD46 <= 0) {
        if (field_0x1838 <= 350.0f) {
            changeState(StateID_Counter);
        } else {
            vt_0x220();
        }
    }
}
void dAcGirahimuBase_c::finalizeState_FrontAttack() {}

void dAcGirahimuBase_c::initializeState_BackWarp() {
    setAnm("MovePose", ANM_MovePose, nullptr, m3d::PLAY_MODE_4, 15.0f, 2.0f);
    mCallback.field_0x088 = true;
    field_0xD44 = 0;
    mCyl2.ClrTgSet();
    mSpeed = 0.0f;
    field_0x1834 = 0.0f;
    field_0xD6C = 0;
}
void dAcGirahimuBase_c::executeState_BackWarp() {
    mCyl2.ClrTgSet();

    switch (field_0xD44) {
        case 0: {
            if (mAnmChrs[ANMIDX_Body].getFrame() >= 18.0f) {
                mCollider.ClrCo();
                if (sLib::chase(&field_0x8B0.x, 0.0, 0.2f)) {
                    field_0xD44++;
                    field_0x8B0.set(0.0f, 0.0f, 0.0f);
                    field_0xD46 = 20 + cM::rndInt(30);
                    if (vt_0x218()) {
                        field_0xD46 /= 2;
                    }
                    vt_0x1F8();
                }
            }
        } break;
        case 1: {
            mRotation.y = cLib::targetAngleY(mPosition, dAcPy_c::GetLink()->mPosition);
            if (field_0xD46 <= 0) {
                mVec3_c start = mPosition;
                mVec3_c end = mPosition;
                for (int i = 0; i < 4; ++i) {
                    mAng yRot = dAcPy_c::GetLink()->mRotation.y + 0x8000 + (i * 0x4000);
                    start.set(dAcPy_c::GetLink()->mPosition);
                    start.y += 100.0f;
                    end.set(0.0f, 0.0f, 130.0f);
                    end.rotY(yRot);
                    end += start;
                    if (!dBgS_ObjLinChk::LineCross(&start, &end, nullptr)) {
                        break;
                    }
                }
                end.y = mPosition.y;
                setPosition(end);
                setOldPosition(end);
                mRotation.y = cLib::targetAngleY(mPosition, dAcPy_c::GetLink()->mPosition);
                field_0x8B0.set(0.0f, 1.05f, 1.05f);
                field_0xD44++;
                changeState(StateID_BackAttack);
                return;
            }
        } break;
    }
    sLib::chaseAngle(mRotation.y.ref(), field_0x183E, 0x500);
}
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
void dAcGirahimuBase_c::executeState_BackAttack() {
    mCollider.ClrCo();
    if (field_0x8B0.x >= 0.5f) {
        mCollider.CoSet();
    }

    if (!turn(field_0x183E)) {
        vt_0x228();
    }
    sLib::chaseAngle(mRotation.y.ref(), field_0x183E, 0x500);

    if (sLib::chase(&field_0x8B0.x, 1.05f, 0.2f) != FALSE) {
        field_0xD6C = 5;
    } else {
        field_0xD6C = 0;
    }

    if (field_0xD46 <= 0) {
        if (field_0x1838 <= 350.0f) {
            changeState(StateID_Counter);
        } else {
            vt_0x220();
        }
    }
}
void dAcGirahimuBase_c::finalizeState_BackAttack() {}

void dAcGirahimuBase_c::initializeState_Counter() {
    field_0xD7E = false;
    if (mAnmIDBody == ANM_PoseR) {
        setAnm("PoseLAttack", ANM_PoseLAttack, nullptr, m3d::PLAY_MODE_4, 5.0f, 1.0f);
    } else if (mAnmIDBody == ANM_PoseL) {
        setAnm("PoseRAttack", ANM_PoseRAttack, nullptr, m3d::PLAY_MODE_4, 5.0f, 1.0f);
    } else if (mAnmIDBody == ANM_PoseC) {
        setAnm("PoseCAttack", ANM_PoseCAttack2, nullptr, m3d::PLAY_MODE_4, 5.0f, 1.0f);
    } else if (mAnmIDBody == ANM_PoseC2) {
        setAnm("PoseCAttack", ANM_PoseCAttack, nullptr, m3d::PLAY_MODE_4, 5.0f, 1.0f);
    } else {
        int rnd = cM::rndInt(90);
        if (rnd > 60) {
            setAnm("PoseLAttack", ANM_PoseLAttack, nullptr, m3d::PLAY_MODE_4, 5.0f, 1.0f);
        } else if (rnd > 30) {
            setAnm("PoseRAttack", ANM_PoseRAttack, nullptr, m3d::PLAY_MODE_4, 5.0f, 1.0f);
        } else {
            setAnm("PoseCAttack", ANM_PoseC, nullptr, m3d::PLAY_MODE_4, 5.0f, 1.0f);
        }
        field_0xD7E = true;
    }
    field_0x1834 = 0.0f;
    mSpeed = 0.0f;
    mCallback.field_0x088 = true;
    field_0xD6C = 5;
}
void dAcGirahimuBase_c::executeState_Counter() {
    sLib::chaseAngle(mRotation.y.ref(), field_0x183E, 0x500);
    if (16.0f <= mAnmChrs[ANMIDX_Body].getFrame() && mAnmChrs[ANMIDX_Body].getFrame() <= 20.0f) {
        mMdlSwordA.enableAttack();
        mMdlSwordA.setProcActive(true);
    } else {
        mMdlSwordA.setInactive();
        mMdlSwordA.setProcActive(false);
    }

    if (mAnmChrs[ANMIDX_Body].isStop()) {
        vt_0x220();
    }

    if (mAnmChrs[ANMIDX_Body].getFrame() <= 16.0f) {
        field_0xD6C = 5;
    } else {
        field_0xD6C = 6;
    }
}
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
        setAnm("AttackPoseL", ANM_AttackPoseL, nullptr, m3d::PLAY_MODE_4, 5.0f, 1.0f);
    } else {
        setAnm("AttackPoseR", ANM_AttackPoseR, nullptr, m3d::PLAY_MODE_4, 5.0f, 1.0f);
    }
    mCallback.field_0x088 = false;
    field_0x1834 = 0.0f;
}
void dAcGirahimuBase_c::executeState_Run() {
    s32 _weird_zero = 0;

    fn_226_CC40();
    switch (field_0xD44) {
        case 0: {
            field_0xD81 = true;
            field_0x1858 = 0.1f;
            if (mAnmChrs[ANMIDX_Body].getFrame() >= 40.0f) {
                field_0xD46 = 0;
                field_0xD44++;
                field_0xD81 = false;
            }
            sLib::chaseAngle(mRotation.y.ref(), field_0x183E, 0x500);
        } break;
        case 1: {
            if (field_0xD46 <= 0 || field_0x1838 <= 500.0f) {
                if (fn_226_CC80()) {
                    setAnm("AttackLRun", ANM_AttackLRun, nullptr, m3d::PLAY_MODE_4, 5.0f, 1.0f);

                } else {
                    setAnm("AttackRRun", ANM_AttackRRun, nullptr, m3d::PLAY_MODE_4, 5.0f, 1.0f);
                }

                field_0xD44++;
                if (vt_0x218()) {
                    field_0x1834 = _weird_zero + 52.0f;
                } else {
                    field_0x1834 = _weird_zero + 45.0f;
                }
                field_0x1840.set(mPosition);
                mMtx_c m;
                MTXIdentity(m);
                dStageMgr_c::GetInstance()->procfn_800192F0(0xDC, m, 0x14);
            }
            sLib::chaseAngle(mRotation.y.ref(), field_0x183E, 0x500);
        } break;
        case 2: {
            mVec3_c v1;
            mVec3_c v0 = dAcPy_c::GetLink()->mPosition - field_0x1840;
            if (fn_226_CC80()) {
                v1.set(-100.0f, 0.0f, 50.0f);
                v1.rotY(v0.atan2sX_Z());
                v1 += dAcPy_c::GetLink()->mPosition;
                sLib::chaseAngle(mRotation.y.ref(), cLib::targetAngleY(mPosition, v1), 0x2000);
            } else {
                v1.set(100.0f, 0.0f, 50.0f);
                v1.rotY(v0.atan2sX_Z());
                v1 += dAcPy_c::GetLink()->mPosition;
                sLib::chaseAngle(mRotation.y.ref(), cLib::targetAngleY(mPosition, v1), 0x2000);
            }
            v0 = v1 - mPosition;

            f32 dir = v0.absXZ();
            if (dir <= 200.0f || mAcch.ChkWallHit(nullptr)) {
                field_0xD44++;
                field_0x184C.set(dAcPy_c::GetLink()->mPosition);
            } else if ((dAcPy_c::GetLink()->getCurrentAction() == 98 /* BACKFLIP */ ||
                        dAcPy_c::GetLink()->checkFlags0x350(0x80000)) &&
                       dir <= 500.0f) {
                field_0xD44++;
                field_0x184C.set(dAcPy_c::GetLink()->mPosition);
            }
        } break;
        case 3: {
            mVec3_c v1;
            mVec3_c v0 = field_0x184C - field_0x1840;
            if (fn_226_CC80()) {
                v1.set(-100.0f, 0.0f, 50.0f);
                v1.rotY(v0.atan2sX_Z());
                v1 += field_0x184C;
                sLib::chaseAngle(mRotation.y.ref(), cLib::targetAngleY(mPosition, v1), 0x2000);
            } else {
                v1.set(100.0f, 0.0f, 50.0f);
                v1.rotY(v0.atan2sX_Z());
                v1 += field_0x184C;
                s16 target = cLib::targetAngleY(mPosition, v1);
                sLib::chaseAngle(mRotation.y.ref(), target, 0x2000);
            }
            v0 = v1 - mPosition;

            f32 dir = v0.absXZ();
            if (dir <= 150.0f || mAcch.ChkWallHit(nullptr)) {
                changeState(StateID_RunAttack);
            }
        } break;
    }
}
void dAcGirahimuBase_c::finalizeState_Run() {
    field_0x1834 = 0.0f;
    field_0xD81 = false;
}

void dAcGirahimuBase_c::initializeState_RunAttack() {
    field_0xD6C = 4;
    if (fn_226_CC80()) {
        setAnm("AttackSwordL", ANM_AttackSwordL, nullptr, m3d::PLAY_MODE_4, 5.0f, 1.0f);
    } else {
        setAnm("AttackSwordR", ANM_AttackSwordR, nullptr, m3d::PLAY_MODE_4, 5.0f, 1.0f);
    }
    mCallback.field_0x088 = true;
    field_0x1834 = 0.0f;
    field_0x1838 = 0.2f;
}
void dAcGirahimuBase_c::executeState_RunAttack() {
    if (mAnmChrs[ANMIDX_Body].getFrame() > 20.0f) {
        field_0xD6C = 8;
        field_0xD81 = true;
    }

    if (mAnmChrs[ANMIDX_Body].checkFrame(4.0f)) {
        mMdlSwordA.enableAttack();
    }

    if (mAnmChrs[ANMIDX_Body].checkFrame(5.0f)) {
        mMdlSwordA.setProcActive(true);
    } else if (mAnmChrs[ANMIDX_Body].checkFrame(15.0f)) {
        mMdlSwordA.setInactive();
        mMdlSwordA.setProcActive(false);
    } else if (mAnmChrs[ANMIDX_Body].isStop()) {
        vt_0x220();
    }
}
void dAcGirahimuBase_c::finalizeState_RunAttack() {
    mMdlSwordA.setInactive();
    mMdlSwordA.setProcActive(false);
    field_0x1858 = 0.5f;
}

void dAcGirahimuBase_c::initializeState_BackWalk() {
    field_0xD80 = false;
    field_0xD6C = 6;
    setAnm("WalkBt", ANM_Walk, nullptr, m3d::PLAY_MODE_2, 15.0f, 1.0f);
    mCallback.field_0x088 = true;
    field_0xD46 = 50;
    field_0x1834 = mAnmChrs[ANMIDX_Body].getRate() * -8.0f;
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

bool dAcGirahimuBase_c::createHeap() {
    nw4r::g3d::ResFile bodyRes;
    void *rawRes = getOarcResFile("GirahimBt");
    mResAnmChr0 = nw4r::g3d::ResFile(rawRes);
    vt_0x200();
    nw4r::g3d::ResMdl resMdl = mResAnmChr0.GetResMdl("GirahimBt");
    if (!mMdlBody.create(resMdl, &mAllocator, 0x137, 1, nullptr)) {
        return false;
    }
    TRY_CREATE(mAnmChrBlend.create(resMdl, 10, &mAllocator, nullptr));

    nw4r::g3d::ResAnmChr resAnm = mResAnmChr1.GetResAnmChr("WaitA");
    if (!resAnm.IsValid()) {
        resAnm = mResAnmChr0.GetResAnmChr("WaitA");
    }
    for (int i = 0; i < 5; ++i) {
        if (!mAnmChrs[i].create(resMdl, resAnm, &mAllocator, nullptr)) {
            mMdlBody.remove();
            return false;
        }
    }

    for (int i = 0; i < 5; ++i) {
        mAnmChrBlend.attach(i, &mAnmChrs[i], 1.0f);
    }

    nw4r::g3d::ResAnmTexPat pat = mResAnmChr1.GetResAnmTexPat("GirahimBtWink");
    TRY_CREATE(mAnmTexPatWink.create(resMdl, pat, &mAllocator, nullptr, 1));

    mMdlBody.setAnm(mAnmTexPatWink);
    mMdlBody.setCallback(&mCallback);
    mMdlBody.setAnm(mAnmChrBlend, 5.0f);

    initNodeData();

    bodyRes = nw4r::g3d::ResFile(rawRes);
    mMdlPias.create(bodyRes.GetResMdl("Pias"), &mAllocator, 0x20);

    mVec3_c v0(0.0f, -200.0f, 0.0f);
    TRY_CREATE(
        mMdlSwordA.create(&mAllocator, rawRes, "GirahimSwordA", 0x130, mVec3_c::Zero, v0, mStts, nullptr, 1, nullptr)
    );

    mVec3_c v1(0.0f, -200.0f, 0.0f);
    TRY_CREATE(
        mMdlSwordB.create(&mAllocator, rawRes, "GirahimSwordB", 0x130, mVec3_c::Zero, v1, mStts, nullptr, 1, nullptr)
    );

    int id = m3d::getMatID(mMdlBody.getResMdl(), "m_Eyeball");
    mEyeMdlCallback.create(mMdlBody, id, id, 1, 1, nullptr);
    mEyeMdlCallback.mBlendWeight = 1;
    pSoundIface = getSoundSource();

    return true;
}

void dAcGirahimuBase_c::createCollision() {
    mEmitter.init(this);

    mStts.SetRank(12);

    mAcch.Set(this, 1, &mAcchCir);
    mAcchCir.SetWall(50.0f, 100.0f);

    mMdlSwordA.setDamageMaybe(4);
    mMdlSwordB.setDamageMaybe(4);

    mCollider.addCc(mSph0, sSrcSph);
    mCollider.addCc(mCps0, sSrcCps);
    mCollider.addCc(mCps1, sSrcCps);
    mCollider.addCc(mCyl2, sSrcCyl);

    mCps0.SetTg_0x4C(-1);
    mCps1.SetTg_0x4C(-1);

    mCyl1.Set(sSrcCyl);
    mCyl1.ClrTgSet();
    mCyl1.OnCoSet();
    mCyl1.SetStts(mStts);

    mCyl0.Set(sSrcCyl);
    mCyl0.OnTgSet();
    mCyl0.ClrCoSet();
    mCyl0.OnTg_0x40();

    mCyl2.SetR(30.0f);

    mCyl0.SetStts(mStts);

    field_0x20E4 = 30.0f;
    field_0x20E8 = 30.0f;

    mCollider.SetStts(mStts);
    field_0xD56 = 30;
    mCyl2.ClrTgSet();

    mCallback.init(this);

    mBoundingBox.Set(mVec3_c(-100.0f, -500.0f, -100.0f), mVec3_c(100.0f, 1000.0f, 100.0f));

    mAnmIDBody = ANM_NONE;
    mAcceleration = -6.0f;
    mMaxSpeed = -80.0f;
    field_0x1858 = 0.5f;
    field_0xD7C = true;
    field_0x8B0.set(1.05f, 1.05f, 1.05f);
    mHealth = 100;

    field_0x8FC.ZrotS(-0x4000);
    mMdlSwordA.fn_8006B7A0(0x10000);
    mMdlSwordB.fn_8006B7A0(0x10000);
    field_0x186C = 40.0f;
}

int dAcGirahimuBase_c::actorPostCreate() {
    if ((s32)getFromParams(0, 0xFF) != 0xFF) {
        // zone temp flag: 0x1 01
        SceneflagManager::sInstance->unsetFlag(mRoomID, getFromParams(0, 0xFF));
    }
    if ((s32)getFromParams(0, 0xFF) != 0xFF) {
        // zone temp flag: 0x1 04
        SceneflagManager::sInstance->unsetFlag(mRoomID, getFromParams(8, 0xFF));
    }

    dAcObjGirahimuSwordLink_c *swLink = static_cast<dAcObjGirahimuSwordLink_c *>(
        create(fProfile::OBJ_GH_SW_L, mRoomID, 0, &mPosition, nullptr, nullptr, 0xFFFFFFFF)
    );
    mSwordLink.link(swLink);
    return SUCCEEDED;
}

int dAcGirahimuBase_c::doDelete() {
    return SUCCEEDED;
}

int dAcGirahimuBase_c::preExecute() {
    if ((dAcPy_c::GetLink()->getCurrentAction() == 74 /* DIE */ || dLytMsgWindow_c::getInstance()->isVisible()) &&
        field_0xD6A > 0) {
        field_0xD6A = 0;
        dLytBossCaption_c::GetInstance()->unk_inline1();
    }
    dAcEnBase_c::preExecute();
}

void dAcGirahimuBase_c::vt_0x1DC() {
    mSph0.SetTgType(AT_TYPE_DAMAGE | AT_TYPE_SWORD);
    field_0x8A4.set(field_0x8B0);

    field_0xD8D = false;
    field_0xD7C = true;
    field_0xD7D = true;

    field_0x1858 = 0.5f;

    mVec3_c diffToLink = mPosition - dAcPy_c::GetLink()->mPosition;
    field_0x1838 = diffToLink.absXZ();

    field_0x183E = cLib::targetAngleY(mPosition, dAcPy_c::GetLink()->mPosition);

    sLib::chase(&field_0x20E4, field_0x20E4, 10.0f);

    mVec3_c diffFromHome = mPosition - mStartingPos;
    field_0x1868 = diffFromHome.absXZ();

    // clang-format off
    if (--field_0xD46 <= 0) { field_0xD46 = 0; }
    if (--field_0xD48 <= 0) { field_0xD48 = 0; }
    if (--field_0xD50 <= 0) { field_0xD50 = 0; }
    if (--field_0xD54 <= 0) { field_0xD54 = 0; }
    if (--field_0xD4C <= 0) { field_0xD4C = 0; }
    if (--field_0xD5C <= 0) { field_0xD5C = 0; }
    if (--field_0xD6A <= 0) { field_0xD6A = 0; }
    if (--field_0xD68 <= 0) { field_0xD68 = 0; }
    if (--field_0xD58 <= 0) { field_0xD58 = 0; }
    // clang-format on
    if (--field_0xD60 <= 0) {
        field_0xD60 = 0;
        mSph0.OnTgSet();
        mCyl2.OnTgSet();
    } else {
        mSph0.ClrTgSet();
        mCyl2.ClrTgSet();
    }

    mCyl2.SetR(30.0f);
    mCyl2.OffTg_0x4C(0x400 | 0x2);
    mCyl2.SetTgType(~AT_TYPE_COMMON0);

    mCyl0.SetR(200.0f);
    mCyl2.SetH(mCallback.field_0x08C.y - mPosition.y);

    mCps0.ClrTgSet();
    mCps1.ClrTgSet();
    if (field_0xD5C <= 0) {
        dAcObjBase_c *pObj = dAcPy_c::GetLink()->getBeetleInFlight();
        if (pObj != nullptr) {
            field_0x88C.set(pObj->mPosition);
        } else {
            field_0x88C.set(getPlayerHeadOffset());
        }
    }

    if (!dAcPy_c::GetLinkM()->isAttacking()) {
        field_0x898 = mCallback.field_0x260 - mCallback.field_0x21C;
        field_0x898.rotY(-mRotation.y);
        field_0x898.z = 0.0f;
    }

    f32 swordLen = 120.0f;
    s32 swordType = getPlayerSwordType();

    if (dAcPy_c::GetLinkM()->isUsingSword()) {
        if (swordType == 4 || swordType == 5) {
            swordLen *= 1.4f;
        }
        field_0x20E8 = swordLen;
    } else {
        field_0x20E8 = 30.0f;
    }

    mCyl0.OnTgSet();
    if (dAcPy_c::GetLinkM()->isAttacking() && !field_0xD7A) {
        field_0xD7A = true;
        mPlayerAttackDir = dAcPy_c::GetLink()->getSpecificAttackDirection();
    } else if (!dAcPy_c::GetLinkM()->isAttacking()) {
        field_0xD7A = false;
    }

    if (dAcPy_c::GetLink()->isAttacking()) {
        field_0xD54 = 20;
    }
    sLib::chaseAngle(mCallback.field_0x056.ref(), 0, 0x200);
    sLib::addCalcAngle(mCallback.field_0x054.ref(), mCallback.field_0x056, 2, 0x20C);
}

s32 dAcGirahimuBase_c::getPlayerSwordType() {
    return dAcPy_c::getCurrentSwordTypeInline();
}

bool dAcGirahimuBase_c::setAnm(
    const char *anmName, s32 anmId, const char *anmName1, m3d::playMode_e playMode, f32 f0, f32 rate
) {
    bool ret = false;
    if (mAnmIDBody != anmId) {
        if (setAnmBody(anmName, anmId, playMode, f0, rate)) {
            setAnmHip(anmName, anmId, playMode, f0, rate);
            setAnmShoulderR(anmName, anmId, playMode, f0, rate);
            setAnmShoulderL(anmName, anmId, playMode, f0, rate);
            if (anmName1 != nullptr) {
                setAnmShoulderL(anmName1, anmId, playMode, f0, rate);
            } else {
                setAnmShoulderL(anmName, anmId, playMode, f0, rate);
            }
        }

        ret = true;
    }
    return ret;
}

bool dAcGirahimuBase_c::setAnm(
    const char *anmName, s32 anmId, const char *anmNameHip, s32 anmIdHip, const char *anmNameShoulderL,
    s32 anmIdShoulderL, m3d::playMode_e playMode, f32 f0, f32 rate
) {
    bool ret = false;
    if (mAnmIDBody != anmId) {
        if (setAnmBody(anmName, anmId, playMode, f0, rate)) {
            setAnmHip(anmNameHip, anmIdHip, playMode, f0, rate);
            setAnmShoulderR(anmName, anmId, playMode, f0, rate);
            setAnmShoulderL(anmName, anmId, playMode, f0, rate);
            if (anmNameShoulderL != nullptr) {
                setAnmShoulderL(anmNameShoulderL, anmIdShoulderL, playMode, f0, rate);
            } else {
                setAnmShoulderL(anmName, anmIdShoulderL, playMode, f0, rate);
            }
        }

        ret = true;
    }
    return ret;
}

bool dAcGirahimuBase_c::setAnm(const char *anmName, s32 anmId, m3d::playMode_e playMode, f32 f0, f32 rate) {
    bool ret = false;
    if (mAnmIDBody != anmId) {
        nw4r::g3d::ResAnmChr resAnm = mResAnmChr1.GetResAnmChr(anmName);
        if (!resAnm.IsValid()) {
            resAnm = mResAnmChr0.GetResAnmChr(anmName);
        }
        setAnmBody(anmName, anmId, playMode, f0, rate);
        setAnmShoulderR(anmName, anmId, playMode, f0, rate);
        setAnmShoulderL(anmName, anmId, playMode, f0, rate);
        setAnmHead(anmName, anmId, playMode, f0, rate);
        ret = true;
    }
    return ret;
}

bool dAcGirahimuBase_c::setAnmBody(const char *anmName, s32 anmId, m3d::playMode_e playMode, f32 f0, f32 rate) {
    bool ret = false;
    if (mAnmIDBody != anmId) {
        nw4r::g3d::ResAnmChr resAnm = mResAnmChr1.GetResAnmChr(anmName);
        if (!resAnm.IsValid()) {
            resAnm = mResAnmChr0.GetResAnmChr(anmName);
        }
        mAnmChrs[ANMIDX_Body].setAnm(mMdlBody, resAnm, playMode);
        bindAnmBody();
        mAnmIDBody = anmId;
        mMdlBody.setAnm(mAnmChrBlend, f0);

        ret = true;
        fn_226_BE90(anmName);

        for (int i = 0; i < 5; ++i) {
            mAnmChrs[i].setRate(rate);
        }

        if (mAnmChrs[ANMIDX_Body].getPlayMode() == m3d::PLAY_MODE_2) {
            rate = -rate;
        }

        if (pSoundIface != nullptr) {
            pSoundIface->setRate(rate);
        }
    }
    return ret;
}

bool dAcGirahimuBase_c::setAnmHip(const char *anmName, s32 anmId, m3d::playMode_e playMode, f32 f0, f32 rate) {
    if (mAnmIDHip != anmId) {
        mAnmIDHip = anmId;
        nw4r::g3d::ResAnmChr resAnm = mResAnmChr1.GetResAnmChr(anmName);
        if (!resAnm.IsValid()) {
            resAnm = mResAnmChr0.GetResAnmChr(anmName);
        }
        mAnmChrs[ANMIDX_Hip].setAnm(mMdlBody, resAnm, playMode);
        mAnmChrs[ANMIDX_Hip].setRate(rate);
        bindAnmHip();
        mMdlBody.setAnm(mAnmChrBlend, f0);
    }
    return true;
}

bool dAcGirahimuBase_c::setAnmShoulderR(const char *anmName, s32 anmId, m3d::playMode_e playMode, f32 f0, f32 rate) {
    if (mAnmIDShoulderR != anmId) {
        mAnmIDShoulderR = anmId;
        fn_226_CE00();
        nw4r::g3d::ResAnmChr resAnm = mResAnmChr1.GetResAnmChr(anmName);
        if (!resAnm.IsValid()) {
            resAnm = mResAnmChr0.GetResAnmChr(anmName);
        }
        mAnmChrs[ANMIDX_ShoulderR].setAnm(mMdlBody, resAnm, playMode);
        mAnmChrs[ANMIDX_ShoulderR].setRate(rate);
        bindAnmShoulderR();
        mMdlBody.setAnm(mAnmChrBlend, f0);
    }
    return true;
}

bool dAcGirahimuBase_c::setAnmShoulderL(const char *anmName, s32 anmId, m3d::playMode_e playMode, f32 f0, f32 rate) {
    if (mAnmIDShoulderL != anmId) {
        mAnmIDShoulderL = anmId;
        fn_226_CDA0();
        nw4r::g3d::ResAnmChr resAnm = mResAnmChr1.GetResAnmChr(anmName);
        if (!resAnm.IsValid()) {
            resAnm = mResAnmChr0.GetResAnmChr(anmName);
        }
        mAnmChrs[ANMIDX_ShoulderL].setAnm(mMdlBody, resAnm, playMode);
        mAnmChrs[ANMIDX_ShoulderL].setRate(rate);
        bindAnmShoulderL();
        mMdlBody.setAnm(mAnmChrBlend, f0);
    }
    return true;
}

bool dAcGirahimuBase_c::setAnmHead(const char *anmName, s32 anmId, m3d::playMode_e playMode, f32 f0, f32 rate) {
    if (mAnmIDHead != anmId) {
        mAnmIDHead = anmId;
        nw4r::g3d::ResAnmChr resAnm = mResAnmChr1.GetResAnmChr(anmName);
        if (!resAnm.IsValid()) {
            resAnm = mResAnmChr0.GetResAnmChr(anmName);
        }
        mAnmChrs[ANMIDX_Head].setAnm(mMdlBody, resAnm, playMode);
        mAnmChrs[ANMIDX_Head].setRate(rate);
        bindAnmHead();
        mMdlBody.setAnm(mAnmChrBlend, f0);
    }
    return true;
}

void dAcGirahimuBase_c::bindAnmHead() {
    nw4r::g3d::AnmObjChr *pAnmObj = static_cast<nw4r::g3d::AnmObjChr *>(mAnmChrs[ANMIDX_Head].getAnimObj());
    pAnmObj->Release();
    pAnmObj->Bind(mMdlBody.getResMdl(), mCallback.mNodeID_Head, nw4r::g3d::AnmObjChr::BIND_PARTIAL);
}

void dAcGirahimuBase_c::bindAnmBody() {
    nw4r::g3d::AnmObjChr *pAnmObj = static_cast<nw4r::g3d::AnmObjChr *>(mAnmChrs[ANMIDX_Body].getAnimObj());
    pAnmObj->Release();
    pAnmObj->Bind(mMdlBody.getResMdl(), mCallback.mNodeID_Head, nw4r::g3d::AnmObjChr::BIND_ONE);
    pAnmObj->Bind(mMdlBody.getResMdl(), mCallback.mNodeID_Spine1, nw4r::g3d::AnmObjChr::BIND_ONE);
    pAnmObj->Bind(mMdlBody.getResMdl(), mCallback.mNodeID_Spine2, nw4r::g3d::AnmObjChr::BIND_ONE);
}

void dAcGirahimuBase_c::bindAnmHip() {
    nw4r::g3d::AnmObjChr *pAnmObj = static_cast<nw4r::g3d::AnmObjChr *>(mAnmChrs[ANMIDX_Hip].getAnimObj());
    pAnmObj->Release();
    pAnmObj->Bind(mMdlBody.getResMdl(), mCallback.mNodeID_SklRoot, nw4r::g3d::AnmObjChr::BIND_ONE);
    pAnmObj->Bind(mMdlBody.getResMdl(), mCallback.mNodeID_Hip, nw4r::g3d::AnmObjChr::BIND_PARTIAL);
}

void dAcGirahimuBase_c::bindAnmShoulderR() {
    nw4r::g3d::AnmObjChr *pAnmObj = static_cast<nw4r::g3d::AnmObjChr *>(mAnmChrs[ANMIDX_ShoulderR].getAnimObj());
    pAnmObj->Release();
    pAnmObj->Bind(mMdlBody.getResMdl(), mCallback.mNodeID_ShoulderR, nw4r::g3d::AnmObjChr::BIND_PARTIAL);
}

void dAcGirahimuBase_c::bindAnmShoulderL() {
    nw4r::g3d::AnmObjChr *pAnmObj = static_cast<nw4r::g3d::AnmObjChr *>(mAnmChrs[ANMIDX_ShoulderL].getAnimObj());
    pAnmObj->Release();
    pAnmObj->Bind(mMdlBody.getResMdl(), mCallback.mNodeID_ShoulderL, nw4r::g3d::AnmObjChr::BIND_PARTIAL);
}

void dAcGirahimuBase_c::fn_226_9620() {
    sLib::addCalcAngle(mCallback.field_0x058.ref(), mCallback.field_0x05C, 2, 0x1000);
    sLib::chaseAngle(mCallback.field_0x05C.ref(), 0, 0x100);
    sLib::addCalcAngle(mCallback.field_0x05A.ref(), mCallback.field_0x05E, 2, 0x1000);
    sLib::chaseAngle(mCallback.field_0x05E.ref(), 0, 0x100);
}

void dAcGirahimuBase_c::fn_226_9690() {
    nw4r::g3d::ResMdl mdl = mMdlBody.getResMdl();

    mMtx_c transform;
    mMdlBody.getNodeWorldMtx(mdl.GetResNode("Head").GetID(), transform);

    mVec3_c headTranslation;
    transform.getTranslation(headTranslation);
    if (field_0x8B0.x > 0.0f && cM::isZero(nw4r::math::VEC3LenSq(field_0x185C))) {
        mPositionCopy3.set(headTranslation);
        mPositionCopy3.y += 30.0f;
        mPositionCopy2.set(mPositionCopy3);
        AttentionManager::GetInstance()->addUnk3Target(*this, 0x3, 10000.0f, 50.0f, -200.0f, 200.0f);
    }
    mVec3_c hairTranslation(0.0f, 0.0f, 0.0f);
    mMdlBody.getNodeWorldMtx(mdl.GetResNode("Hair1").GetID(), transform);
    transform.getTranslation(hairTranslation);

    mVec3_c v1 = mCallback.field_0x070 - headTranslation;
    mVec3_c v0 = hairTranslation - headTranslation;

    s16 a1 = (s32)(v1.atan2sX_Z() - v0.atan2sX_Z());
    s16 a2 = (s32)(v1.atan2sY_XZ() - v0.atan2sY_XZ());

    if (a1 < 0x2001) {
        if (u16(a1 + 500) > 0x2000 + 500) {
            a1 = -500;
        }
    } else {
        a1 = 0x2000;
    }
    if (mAnmIDBody == ANM_Walk) {
        a1 = a1 / 2;
    }

    if (a2 > -0x2001) {
        if (u16(a2 + 0x2000U) > 0x2000) {
            a2 = 0;
        }
    } else {
        a2 = -0x2000;
    }
    sLib::addCalcAngle(mCallback.field_0x06E.ref(), 0, 2, 0x500);
    sLib::addCalcAngle(mCallback.field_0x06C.ref(), 0, 2, 0x500);
    sLib::addCalcAngle(mCallback.field_0x068.ref(), a1, 2, 0x1000);
    sLib::addCalcAngle(mCallback.field_0x06A.ref(), a2, 2, 0x1000);

    sLib::addCalcAngle(mCallback.field_0x062.y.ref(), mCallback.field_0x06E + mCallback.field_0x068, 2, 0x1000);
    sLib::addCalcAngle(mCallback.field_0x062.z.ref(), mCallback.field_0x06C + mCallback.field_0x06A, 2, 0x1000);

    mCallback.field_0x070.set(hairTranslation);
}

void dAcGirahimuBase_c::fn_256_99F0() {
    mMtx_c swordRTransform;
    mMdlBody.getNodeWorldMtx(mMdlBody.getResMdl().GetResNode("SwordR_loc").GetID(), swordRTransform);
    swordRTransform.getTranslation(field_0x12D0);

    if (field_0x8B0.x < 1.05f) {
        swordRTransform.scaleM(field_0x8B0);
    } else {
        swordRTransform.scaleM(mVec3_c(1.05f, field_0x12DC, 1.05f));
    }

    mVec3_c v0(0.0f, 0.0f, 1.0f);
    v0.rotY(mRotation.y);
    mAng a = mAng::d2s_c(15.0f);
    swordRTransform.ZrotM(a);
    mMdlSwordA.calc(swordRTransform, v0, false);

    mMtx_c swordLTransform;
    mMdlBody.getNodeWorldMtx(mMdlBody.getResMdl().GetResNode("SwordL_loc").GetID(), swordLTransform);
    swordLTransform.getTranslation(field_0x1820);

    if (field_0x8B0.x < 1.05f) {
        swordLTransform.scaleM(field_0x8B0);
    } else {
        swordLTransform.scaleM(mVec3_c(1.05f, field_0x12DC, 1.05f));
    }

    mVec3_c v1(0.0f, 0.0f, 1.0f);
    v1.rotY(mRotation.y);
    swordLTransform.ZrotM(a);
    mMdlSwordB.calc(swordLTransform, v0, false);
}

void dAcGirahimuBase_c::fn_256_9C90() {
    if (field_0xD81) {
        fn_226_CC40();
    } else {
        fn_226_CC50();
    }
    mVec3_c v0(0.0f, 0.0f, 40.0f);
    v0.rotY(mRotation.y);

    mCps0.ClrCoSet();
    mCps1.ClrCoSet();

    mCyl1.SetC(mPosition);
    mCyl0.SetC(mPosition);
    mCyl2.SetC(mPosition);

    mVec3_c v1 = mCallback.field_0x260;
    if (dAcPy_c::GetLink()->isAttackingSpin()) {
        if (v1.y > mPosition.y + 150.0f) {
            v1.y = mPosition.y + 150.0f;
        }
    }
    mSph0.SetC(v1);

    mVec3_c cpsStart(0.0f, 0.0f, 0.0f);
    mVec3_c cpsEnd(0.0f, 0.0f, 0.0f);
    mVec3_c v4(0.0f, -200.0f, 0.0f);
    mMtx_c swordTransform;
    nw4r::g3d::ResMdl mdl = mMdlBody.getResMdl();
    mMdlBody.getNodeWorldMtx(mdl.GetResNode("SwordR_loc").GetID(), swordTransform);
    swordTransform.getTranslation(cpsStart);
    swordTransform.m[0][3] = swordTransform.m[1][3] = swordTransform.m[2][3] = 0.0f;
    swordTransform.ZrotM(2000);
    mVec3_c v5;
    swordTransform.multVec(v4, v5);
    cpsEnd.set(cpsStart);
    cpsEnd += v0;
    cpsEnd += v5;
    cpsStart += v0;

    if (mAnmIDBody == ANM_PoseLAttack || mAnmIDBody == ANM_PoseRAttack) {
        cpsStart.y -= 50.0f;
        cpsEnd.y -= 50.0f;
    }
    mCps0.Set(cpsStart, cpsEnd);

    mMdlBody.getNodeWorldMtx(mdl.GetResNode("SwordL_loc").GetID(), swordTransform);
    swordTransform.getTranslation(cpsStart);
    swordTransform.m[0][3] = swordTransform.m[1][3] = swordTransform.m[2][3] = 0.0f;
    swordTransform.ZrotM(2000);

    swordTransform.multVec(v4, v5);
    cpsStart += v0;
    cpsEnd = cpsStart;

    if (mAnmIDBody == ANM_PoseLAttack || mAnmIDBody == ANM_PoseRAttack) {
        cpsStart.y -= 50.0f;
        cpsEnd.y -= 50.0f;
    }
    mCps1.Set(cpsStart, cpsEnd);

    mCollider.registerColliders();
    dCcS::GetInstance()->Set(&mCyl0);
    mCyl1.SetR(field_0x20E4);
    dCcS::GetInstance()->Set(&mCyl1);
}

bool dAcGirahimuBase_c::fn_226_A070(mAng rotTarget) {
    mAng rot = mRotation.y;
    s32 angDiff = rotTarget - mRotation.y;
    if (rot == rotTarget) {
        return false;
    }

    if ((s16)angDiff > 0) {
        if (mAnmIDBody != ANM_TurnL) {
            setAnm("TurnL", ANM_TurnL, nullptr, m3d::PLAY_MODE_4, 15.0f, 1.0f);
        }
    } else {
        if (mAnmIDBody != ANM_TurnR) {
            setAnm("TurnR", ANM_TurnR, nullptr, m3d::PLAY_MODE_4, 15.0f, 1.0f);
        }
    }
    return true;
}

bool dAcGirahimuBase_c::fn_226_A120(mAng rot, f32 f0) {
    if (mAnmIDHip == ANM_Walk) {
        if (mAnmChrs[ANMIDX_Hip].getPlayMode() != m3d::PLAY_MODE_2) {
            field_0x1834 = mAnmChrs[ANMIDX_Body].getRate() * 8.0f;
            mSpeed = field_0x1834;
        } else {
            field_0x1834 = mAnmChrs[ANMIDX_Body].getRate() * -8.0f;
            mSpeed = field_0x1834;
        }
    }

    if (mRotation.y == rot && field_0x1838 <= f0) {
        field_0x1834 = 0.0f;
        return false;
    }

    if (field_0x1838 > f0 + 100.0f) {
        if (mAnmIDHip != ANM_Walk) {
            setAnmHip("WalkBt", ANM_Walk, m3d::PLAY_MODE_4, 5.0f, 1.0f);
        }
    } else {
        if (field_0x1838 <= f0) {
            return turn(field_0x183E);
        }
        if (mAnmIDHip != ANM_Walk) {
            turn(field_0x183E);
        }
    }
    return true;
}

bool dAcGirahimuBase_c::fn_226_A280(mAng rot) {
    mAng roty = mRotation.y;
    s32 diff = rot - mRotation.y;
    mAng diff0 = roty - field_0x183E;
    if (mCallback.field_0x056 == 0) {
        if (diff0.abs() != 0) {
            if ((s16)diff > 0) {
                mCallback.field_0x056 = mAng(0x3000);
            } else {
                mCallback.field_0x056 = mAng(-0x3000);
            }
        }
    }
    return mCallback.field_0x056 != 0;
}

bool dAcGirahimuBase_c::turn(mAng rotTarget) {
    mAng rot = mRotation.y;
    s32 angDiff = rotTarget - mRotation.y;
    if (rot == field_0x183E) {
        return false;
    }

    if (mAnmIDHip == ANM_TurnL || mAnmIDHip == ANM_TurnR) {
        return true;
    }

    if ((s16)angDiff > 0) {
        if (mAnmIDHip != ANM_TurnL) {
            setAnmHip("TurnL", ANM_TurnL, m3d::PLAY_MODE_4, 5.0f, 1.0f);
            field_0x1834 = 0.0f;
            mSpeed = 0.0f;
        }
    } else {
        if (mAnmIDHip != ANM_TurnR) {
            setAnmHip("TurnR", ANM_TurnR, m3d::PLAY_MODE_4, 5.0f, 1.0f);
            field_0x1834 = 0.0f;
            mSpeed = 0.0f;
        }
    }
    return true;
}

void dAcGirahimuBase_c::fn_226_A400(cCcD_Obj *pObj) {
    switch (pObj->GetTgAtCutDir()) {
        case CUT_DIR_U: {
            mCallback.field_0x05E = mAng(0x1500);
            mCallback.field_0x05C = 0;
        } break;
        case CUT_DIR_D: {
            mCallback.field_0x05E = -0x1500;
            mCallback.field_0x05C = 0;
        } break;
        case CUT_DIR_LU: {
            mCallback.field_0x05E = mAng(0x1500);
            mCallback.field_0x05C = mAng(0x1500);
        } break;
        case CUT_DIR_RD: {
            mCallback.field_0x05E = -0x1500;
            mCallback.field_0x05C = -0x1500;
        } break;
        case CUT_DIR_L: {
            mCallback.field_0x05C = mAng(0x1500);
        } break;
        case CUT_DIR_R: {
            mCallback.field_0x05C = -0x1500;
        } break;
        case CUT_DIR_LD: {
            mCallback.field_0x05E = -0x1500;
            mCallback.field_0x05C = mAng(0x1500);
        } break;
        case CUT_DIR_RU: {
            mCallback.field_0x05E = mAng(0x1500);
            mCallback.field_0x05C = -0x1500;
        } break;
        default: {
            mCallback.field_0x05E = 0xA80;
            mCallback.field_0x05C = 0;
        } break;
    }

    if (mAng(mRotation.y - field_0x183E).abs() > 0x4000) {
        mCallback.field_0x05C = -mCallback.field_0x05C;
    }
    mCallback.field_0x060 = mRotation.y;
}

bool dAcGirahimuBase_c::fn_226_A570() {
    bool ret = false;
    mVec3_c v0;
    switch (mPlayerAttackDir) {
        case CUT_DIR_U:  v0.set(0.0f, -1.0f, 0.0f); break;
        case CUT_DIR_D:  v0.set(0.0f, 1.0f, 0.0f); break;
        case CUT_DIR_LU: v0.set(1.0f, -1.0f, 0.0f); break;
        case CUT_DIR_RD: v0.set(-1.0f, 1.0f, 0.0f); break;
        case CUT_DIR_L:  v0.set(1.0f, 0.0f, 0.0f); break;
        case CUT_DIR_R:  v0.set(-1.0f, 0.0f, 0.0f); break;
        case CUT_DIR_LD: v0.set(1.0f, 1.0f, 0.0f); break;
        case CUT_DIR_RU: v0.set(-1.0f, -1.0f, 0.0f); break;
        default:         {
            return true;
        } break;
    }
    v0.normalizeRS();
    if (field_0xD68 <= 0) {
        return true;
    }

    if (dAcPy_c::GetLink()->isAttackingSpin()) {
        f32 ang = field_0x898.angle(v0);
        if (mAng::fromRad(ang) > 0x2000) {
            ret = true;
        }
    } else {
        f32 ang = mCallback.field_0x2E0.angle(v0);
        if (mAng::fromRad(ang) > 0x5500) {
            ret = true;
        }
    }

    return ret;
}

mVec3_c dAcGirahimuBase_c::getPlayerHeadOffset() {
    mVec3_c headTranslation = dAcPy_c::GetLink()->getHeadTranslation();
    headTranslation.y -= 30.0f;
    return headTranslation;
}

void dAcGirahimuBase_c::executeEarringTransform() {
    mVec3_c v0(8.0f, 0.0f, 0.0f);
    field_0x8FC.multVec(v0, v0);
    v0 += field_0x92C;

    mMtx_c earringTransform;
    nw4r::g3d::ResMdl mdl = mMdlBody.getResMdl();
    mMdlBody.getNodeWorldMtx(mdl.GetResNode("Earring").GetID(), earringTransform);
    earringTransform.getTranslation(field_0x92C);

    mVec3_c diff = v0 - field_0x92C;
    diff.y -= 3.0f;
    diff.normalizeRS();

    mVec3_c from(1.0f, 0.0f, 0.0f);
    mQuat_c q0;
    q0.slerp(from, diff, 1.0f);
    field_0x8FC.fromQuat(q0);
    mMtx_c transform;
    transform.transS(field_0x92C);
    transform.concat(field_0x8FC);
    transform.scaleM(field_0x8B0);
    mMdlPias.setLocalMtx(transform);
}

void dAcGirahimuBase_c::calcJumpMovement(f32 f0, f32 f1) {
    s32 _weird_zero = 0;
    mVec3_c diff = mPosition - mStartingPos;
    (void)diff.absXZ();
    mAngle.y = field_0x183E + 0x8000;

    mVec3_c v0(0.0f, 0.0f, f0);
    v0.rotY(mAngle.y);
    v0 += mPosition;
    { mAcceleration = 5.0f; } // used to force rodata pool in preamble
    mAcceleration = _weird_zero + 6.0f;
    calcVelocity(v0, f1);
    mAcceleration = -mAcceleration;
}

int dAcGirahimuBase_c::draw() {
    if (field_0x8B0.x <= 0.0f) {
        return SUCCEEDED;
    }

    drawModelType1(&mMdlBody);
    drawModelType1(&mMdlPias);
    static mSphere_c bodySph(mVec3_c(0.0f, 100.0f, 0.0f), 300.0f);
    static mSphere_c swordSph(mVec3_c(0.0f, 0.0f, 0.0f), 0.0f);

    fn_8002edb0(mShadow, mMdlBody, &bodySph, -1, -1, 0.0f);
    if (field_0xD7B) {
        mMdlSwordA.entry(this, nullptr, nullptr);
        m3d::mShadow_c::GetInstance()->addMdlToCircle(&mShadow, mMdlSwordA.mMdl, swordSph);

        if (field_0x12E0) {
            mMdlSwordB.entry(this, nullptr, nullptr);
            m3d::mShadow_c::GetInstance()->addMdlToCircle(&mShadow, mMdlSwordB.mMdl, swordSph);
        }
    }

    return SUCCEEDED;
}

void dAcGirahimuBase_c::fn_226_ADC0(cCcD_Obj *pObj) {
    s32 currentCutDir = pObj->GetTgAtCutDir();
    s32 previousCutDir = mPreviousHitCutDir;

    if (previousCutDir == CUT_DIR_U || previousCutDir == CUT_DIR_D) {
        if (currentCutDir == CUT_DIR_U || currentCutDir == CUT_DIR_D) {
            field_0xD8E++;
        } else {
            field_0xD8E = 1;
        }
    } else if (previousCutDir == CUT_DIR_LU || previousCutDir == CUT_DIR_RD) {
        if (currentCutDir == CUT_DIR_LU || currentCutDir == CUT_DIR_RD) {
            field_0xD8E++;
        } else {
            field_0xD8E = 1;
        }
    } else if (previousCutDir == CUT_DIR_LD || previousCutDir == CUT_DIR_RU) {
        if (currentCutDir == CUT_DIR_LD || currentCutDir == CUT_DIR_RU) {
            field_0xD8E++;
        } else {
            field_0xD8E = 1;
        }
    } else if (field_0xD8E <= 0) {
        field_0xD8E = 1;
    }
    mPreviousHitCutDir = currentCutDir;
}

void dAcGirahimuBase_c::setSwordLinkTransform() {
    if (!field_0xD72) {
        return;
    }

    mVec3_c swordTranslation(0.0f, 0.0f, 0.0f);
    mMtx_c swordTransform;
    nw4r::g3d::ResMdl mdl = mMdlBody.getResMdl();
    mMdlBody.getNodeWorldMtx(mdl.GetResNode("SwordR_loc").GetID(), swordTransform);
    swordTransform.getTranslation(swordTranslation);
    swordTransform.m[2][3] = swordTransform.m[1][3] = swordTransform.m[0][3] = 0.0f;
    swordTransform.ZrotM(0x1000);
    mVec3_c v0(swordTranslation.x, swordTranslation.y, swordTranslation.z);

    mSwordLink.get()->setTransform(swordTransform, v0);
}

void dAcGirahimuBase_c::setSwordLinkTransformPlayer() {
    mMtx_c swordTransform;
    mVec3_c swordTranslation;
    dAcPy_c::GetLinkM()->getSwordModelMatrix(&swordTransform);
    swordTransform.getTranslation(swordTranslation);
    swordTransform.m[2][3] = swordTransform.m[1][3] = swordTransform.m[0][3] = 0.0f;
    swordTransform.ZrotM(0x4000);
    swordTransform.YrotM(0x4000);

    mVec3_c v0(swordTranslation.x, swordTranslation.y, swordTranslation.z);
    mSwordLink.get()->setTransform(swordTransform, v0);
}

bool dAcGirahimuBase_c::vt_0x1E0() {
    someEnemyDamageCollisionStuffMaybe(mCollider, nullptr);
    if (mCyl0.ChkTgHit()) {
        u32 hitType = mCyl0.GetTgAtHitType();

        if (field_0xD81) {
            if (hitType == AT_TYPE_SWORD) {
                if (isState(StateID_Run)) {
                    changeState(StateID_BackStep);
                    return true;
                }
                mVec3_c v0 = mPosition - mStartingPos;
                mVec3_c v1(0.0f, 0.0f, 1.0f);
                v1.rotY(mRotation.y);
                if (field_0x1868 >= 1500.0f && v0.dot(v1) < 0.0f) {
                    field_0x8DC = 0;
                    changeState(StateID_HomeWarp);
                    return true;
                }
                s32 cutDir = mCyl0.GetTgAtCutDir();
                if (cutDir == CUT_DIR_LU || cutDir == CUT_DIR_LD || cutDir == CUT_DIR_RU || cutDir == CUT_DIR_RD ||
                    cutDir == CUT_DIR_R || cutDir == CUT_DIR_L) {
                    if (isState(StateID_Escape)) {
                        changeState(StateID_BackStep);
                        return true;
                    }

                    vt_0x214();
                    return true;
                } else {
                    changeState(StateID_BackStep);
                    return true;
                }
            } else {
                if (hitType == AT_TYPE_BELLOWS) {
                    mCallback.field_0x06C = -0x2000;
                } else if (hitType == AT_TYPE_ARROW || hitType == AT_TYPE_CLAWSHOT || hitType == AT_TYPE_0x800000 ||
                           hitType == AT_TYPE_BOMB) {
                    vt_0x21C(true);
                }
                return false;
            }
        }
        if (hitType == AT_TYPE_ARROW || hitType == AT_TYPE_CLAWSHOT || hitType == AT_TYPE_0x800000 ||
            hitType == AT_TYPE_BOMB) {
            vt_0x21C(true);
        }
    }
    if (mCyl2.ChkTgHit() && mCyl2.GetTgAtHitType() == AT_TYPE_BELLOWS) {
        mCallback.field_0x06C = -0x2000;
    }
    return false;
}

void dAcGirahimuBase_c::vt_0x1E4() {
    if (field_0xD81) {
        return;
    }

    cCcD_Obj *pCc = mCollider.findTgHit();
    if (pCc != nullptr) {
        switch (pCc->GetTgAtHitType()) {
            case AT_TYPE_SWORD: {
                if (pCc->GetTg_0x4C() != 0) {
                    changeState(StateID_Catch);
                } else {
                    fn_226_A400(pCc);
                    changeState(StateID_CatchDamage);
                    field_0xD68 = 0;
                }
            } break;
            case AT_TYPE_DAMAGE: {
                u16 health = mHealth;
                if (health != 0) {
                    health--;
                }
                mHealth = health;

                mVec3_c v0 = mPosition;
                v0.y += 100.0f;
                getSoundSource()->startSoundAtPosition(SE_Girahim_DMG_KNIFE, v0);
                changeState(StateID_CatchDamage);
            } break;
        }
    }

    bool b0 = fn_226_A570();
    if (mAnmIDBody == ANM_Damage) {
        b0 = false;
    } else {
        field_0xD4E = 0;
    }

    if (field_0xD58 > 0) {
        b0 = true;
    }

    if (b0) {
        mSph0.SetTg_0x4C(~0x400);
        mCyl2.SetTg_0x4C(~0x400);
    } else {
        mSph0.SetTg_0x4C(0);
        mCyl2.OffTg_0x4C(0x400 | 0x2);
    }

    if (field_0xD50 <= 0 && !isState(StateID_Panch)) {
        return;
    }

    mSph0.SetTg_0x4C(~0);
}

void dAcGirahimuBase_c::vt_0x1F0() {}

void dAcGirahimuBase_c::vt_0x1E8() {
    cCcD_Obj *pCc = mSwordLink.get()->mMdl.mCcList.findAtHit();
    if (pCc != nullptr) {
        if (pCc->GetAtFlag0x8()) {
            changeState(StateID_LinkSwordGuardJust);
        }
    }

    if (mCyl2.ChkTgHit()) {
        u32 hitType = mCyl2.GetTgAtHitType();

        if (hitType == AT_TYPE_BELLOWS) {
            mCallback.field_0x06C = -0x2000;
        } else if (hitType != AT_TYPE_SWORD && hitType != AT_TYPE_DAMAGE) {
            field_0x88C.set(const_cast<const dCcD_Linked<dCcD_Cyl> &>(mCyl2).GetTgHitPos());
            field_0xD5C = 20;
        }
    }
}

void dAcGirahimuBase_c::vt_0x1EC() {
    someEnemyDamageCollisionStuffMaybe(mCollider, nullptr);
    mCyl2.OffTg_0x4C(AT_TYPE_DAMAGE | AT_TYPE_SWORD);
    cCcD_Obj *pCc = mCollider.findTgHit();
    if (pCc != nullptr) {
        u32 hitType = pCc->GetTgAtHitType();
        switch (hitType) {
            case AT_TYPE_SWORD: {
                startSound(SE_BGh_V_DAMAGE);
                fn_226_A400(pCc);
                fn_226_ADC0(pCc);
                fn_226_D070();
                setAnm("DamageHit", ANM_DamageHit, nullptr, m3d::PLAY_MODE_4, 5.0f, 1.0f);
            } break;
            case AT_TYPE_DAMAGE: {
                u16 health = mHealth;
                if (health != 0) {
                    health--;
                }
                mHealth = health;

                mVec3_c v0 = mPosition;
                v0.y += 100.0f;
                getSoundSource()->startSoundAtPosition(SE_Girahim_DMG_KNIFE, v0);
                field_0xD62 += 50;
            } break;
        }
    }
}

void dAcGirahimuBase_c::fn_226_B960(f32 f0) {
    if (mAnmIDHip != ANM_Walk) {
        return;
    }

    field_0xD7C = false;
    sLib::chaseAngle(mRotation.y.ref(), field_0x183E, 0x500);
    s32 f = field_0x183E - mRotation.y;
    f32 f1 = 1.0f - std::abs(mAng(f).normal_c());
    if (f1 <= 0.6f) {
        f1 = 0.0f;
    }

    if (mAnmChrs[ANMIDX_Hip].getPlayMode() == m3d::PLAY_MODE_2) {
        f32 rate = mAnmChrs[ANMIDX_Hip].getRate();
        mAngle.y = mRotation.y;
        field_0x1834 = -f0 * rate;
    } else {
        f32 rate = mAnmChrs[ANMIDX_Hip].getRate();
        mAngle.y = mRotation.y;
        field_0x1834 = f0 * f1;
        field_0x1834 *= rate;
    }

    if (dAcPy_c::GetLinkM()->isUsingSword()) {
        field_0x20E8 = 120.0f;
    } else {
        field_0x20E8 = 30.0f;
    }
}

bool dAcGirahimuBase_c::vt_0x21C(bool bStartSound) {
    if (!cM::isZero(nw4r::math::VEC3LenSq(field_0x185C))) {
        return false;
    }

    field_0xD60 = 0;
    int rnd = cM::rndInt(100);
    if (rnd > 50) {
        field_0x185C.set(0.0f, 0.0f, 50.0f);
        field_0x185C.rotY(field_0x183E + 0x4000);
    } else {
        field_0x185C.set(0.0f, 0.0f, 50.0f);
        field_0x185C.rotY(field_0x183E - 0x4000);
    }

    if (bStartSound) {
        startSound(SE_Girahim_SHY);
        mMtx_c m0;
        MTXIdentity(m0);
        dStageMgr_c::GetInstance()->procfn_800192F0(0xEE, m0, 10);
    }
    return true;
}

void dAcGirahimuBase_c::fn_226_BC00() {
    bool b = true;
    if (field_0xD66 <= 0 && field_0xD58 <= 0 && mCallback.field_0x2D8 >= 25.0f && mCallback.field_0x098 <= 4.0f) {
        b = false;
    }
    if (!b) {
        mCallback.field_0x2CC = 0.0f;
        field_0xD66 = 90;
        if (mHealth >= 100) {
            field_0xD66 = 120;
        }
        field_0xD68 = 90;
        if (mHealth >= 100) {
            field_0xD68 = 120;
        }
    } else if (--field_0xD66 <= 0) {
        field_0xD66 = 0;
        sLib::chase(&mCallback.field_0x2CC, 30.0f, 5.0f);
    }

    if (dAcPy_c::GetLinkM()->isUsingSword() && field_0x1838 <= 500.0f &&
        ((u16)(s16)mAnmIDBody == ANM_WaitA || (u16)(s16)mAnmIDBody == ANM_Walk)) {
        if (!mCallback.field_0x04E) {
            mCallback.field_0x04F = true;
            field_0xD50 = 5;
        } else {
            mCallback.field_0x04F = false;
        }
        mCallback.field_0x04E = true;
        field_0x88C += dAcPy_c::GetLink()->getSwordPos();
        field_0x88C *= 0.5f;
        field_0x88C.y = getPlayerHeadOffset().y;
        mCallback.field_0x2D4 = 0.2f;
        return;
    }

    mCallback.field_0x04F = false;
    mCallback.field_0x04E = false;
    mCallback.field_0x2D4 = 1.0f;
}

void dAcGirahimuBase_c::setFrame(f32 frame) {
    for (int i = 0; i < 5; ++i) {
        mAnmChrs[i].setFrameOnly(frame);
    }

    if (pSoundIface != nullptr) {
        pSoundIface->setFrame(mAnmChrs[ANMIDX_Body].getFrame());
    }
}

bool dAcGirahimuBase_c::fn_226_BE90(const char *soundName) {
    if (pSoundIface == nullptr || !pSoundIface->hasAnimSound()) {
        return true;
    }
    SizedString<64> str;
    str.sprintf("%s.brasd", soundName);
    mpSoundBrasd = mResAnmChr1.GetExternalData(str);
    if (mpSoundBrasd == nullptr) {
        mpSoundBrasd = mResAnmChr0.GetExternalData(str);
    }

    pSoundIface->load(mpSoundBrasd, soundName);
    if (pSoundIface != nullptr) {
        pSoundIface->setFrame(mAnmChrs[ANMIDX_Body].getFrame());
    }
    return true;
}

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
