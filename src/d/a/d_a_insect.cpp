#include "d/a/d_a_insect.h"

#include "c/c_math.h"
#include "common.h"
#include "d/a/d_a_player.h"
#include "d/col/bg/d_bg_pc.h"
#include "d/col/bg/d_bg_s.h"
#include "d/col/bg/d_bg_s_acch.h"
#include "d/col/bg/d_bg_s_lin_chk.h"
#include "d/col/bg/d_bg_w_base.h"
#include "d/col/c/c_cc_d.h"
#include "d/col/c/c_m3d_g_pla.h"
#include "egg/math/eggMath.h"
#include "m/m_angle.h"
#include "m/m_mtx.h"
#include "m/m_vec.h"
#include "rvl/MTX/mtxvec.h"
#include "toBeSorted/attention.h"

#include <cmath.h>

// Very Hack ??
static inline bool IsZero(f32 in) {
    return in <= EGG::Math<f32>::epsilon();
}

// void AcOInsectFloats() {
//     50.0f;
//     1.0f;
//     30.0f;
//     5000.0f;
//     0.1f;
//     10.0f;
//     500.0f;
//     0.0f;
//     400.0f;
//     -200.0f;
//     200.0f;
//     20.0f;
//     EGG::Math<f32>::epsilon();
//     5.0f;
//     -5.0f;
//     0.99f;
//     int x = 0;
//     (f32) x;
// }

// void AcOInsectFloats2() {
//     -1.0f;
//     0.5f;
//     2250000.0f;
// }

extern "C" void fn_800298B0(u16, mVec3_c *, mVec3_c *, u32, u32, u32, u32, u32);
extern "C" u16 PARTICLE_RESOURCE_ID_MAPPING_394_;

void dAcOInsect_c::kill() {
    // Small Ordering issue between loading particle id and position
    fn_800298B0(PARTICLE_RESOURCE_ID_MAPPING_394_, &position, nullptr, 0, 0, 0, 0, 0);
    playSound(0x1236); // TODO (Sound ID)
    deleteRequest();
}

bool dAcOInsect_c::checkForLinkScare() {
    if (dAcPy_c::LINK != nullptr) {
        if (dAcPy_c::LINK->getCurrentAction() == 12 /* Rolling */) { // TODO (Player Action Enum)
            if (dAcPy_c::LINK->getDistanceTo(position) < 50.f) {
                mLinkNearby = 1;
                return true;
            }
        } else if (dAcPy_c::LINK->forwardSpeed > 1.f) {
            if (dAcPy_c::LINK->getDistanceTo(position) < 30.f) {
                mLinkNearby = 1;
                return true;
            }
        }
    }

    return false;
}

void dAcOInsect_c::checkDeath(dBgS_Acch &acch) {
    if (acch.ChkWaterHit() && acch.mWtr.GetGroundH() >= position.y) {
        kill();
    }
    if (acch.ChkGroundLanding() && dBgS::GetInstance()->GetSpecialCode(acch.mGnd) == POLY_ATTR_LAVA) {
        kill();
    }
    if (position.y < pos_copy.y - 5000.f) {
        deleteRequest();
    }
}

bool dAcOInsect_c::isLinkUsingBugNet() {
    if (dAcPy_c::LINK != nullptr && !dAcPy_c::LINK->isUsingBugnet()) {
        return false;
    }
    return true;
}

s32 dAcOInsect_c::getLinkSpeedLevel() {
    if (dAcPy_c::LINK != nullptr) {
        f32 speed = fabsf(dAcPy_c::LINK->forwardSpeed);
        if (speed < 0.1f) {
            return 0;
        } else if (speed < 10.0f) {
            return 1;
        } else {
            return 2;
        }
    }

    return 3;
}

bool dAcOInsect_c::checkPlayerRadius(f32 rad) {
    if (dAcPy_c::LINK != nullptr && isWithinPlayerRadius(rad)) {
        return true;
    }
    return false;
}

bool dAcOInsect_c::checkPlayerElevationDiff(f32 dist) {
    if (dAcPy_c::LINK != nullptr && fabsf(dAcPy_c::LINK->position.y - position.y) > dist) {
        return false;
    }
    return true;
}

void dAcOInsect_c::someBaseFunction() {
    dAcPy_c *link = dAcPy_c::LINK;
    if (link != nullptr) {
        if (checkPlayerElevationDiff(500.0f) && checkPlayerRadius(dAcOInsect_0x8C())) {
            field_0x335 = field_0x334;
            field_0x334 = link->isUsingBugnet1();
            if (link->isUsingBugnet1()) {
                field_0x330++;
                return;
            }
            field_0x330 = 0;
            return;
        } else {
            field_0x334 = 0;
            field_0x335 = 0;
            field_0x330 = 0;
        }
    }
}

bool dAcOInsect_c::someBaseFunction0() {
    f32 rad = dAcOInsect_0x80();
    if (checkPlayerElevationDiff(500.0f) && checkPlayerRadius(rad)) {
        field_0x338 = 1;
        return true;
    }
    bool b = (field_0x335 != 0 && field_0x334 == 0) || field_0x330 > 3;
    if (b) {
        field_0x338 = 2;
        return true;
    } else {
        field_0x338 = 0;
        return false;
    }
}

void dAcOInsect_c::addAttentionTarget() {
    preAttention();
    static InteractionTargetDef tmpTarget = {1, 0, 0, UNK_18, 0x2, 150.0f, 0.0f, 0.0f, -300.0f, 100.0f, 50.0f, 1.0f};
    if (shouldAddAttentionTarget()) {
        AttentionManager *attn = AttentionManager::sInstance;
        if (attn != nullptr) {
            attn->addCatchTarget(*this, 0x1, 400.0f, -200.0f, 200.0f);
            tmpTarget.field_0x14 = getAttentionField();
            attn->addTarget(*this, tmpTarget, 0, nullptr);
        }
    }
}

void dAcOInsect_c::preAttention() {
    // It's always the simplest vector functions that cause problems
    poscopy2 = position;
    poscopy2.y += 20.f;
    poscopy3 = poscopy2;
}

f32 dAcOInsect_c::dAcOInsect_0x80() {
    switch (getLinkSpeedLevel()) {
        case 1:  return dAcOInsect_0x84();
        case 2:  return dAcOInsect_0x88();
        default: return dAcOInsect_0x84();
    }
}

bool dAcOInsect_c::isLinkCloseAndFlag() {
    dAcPy_c *link = dAcPy_c::LINK;
    if (link != nullptr && link->checkFlags0x350(0x2000)) {
        f32 d = dAcOInsect_0x94();
        return getSquareDistanceTo(link->position) < d;
    }
    return false;
}

bool dAcOInsect_c::checkDeath(cCcD_Obj &col) {
    if (field_0x3F4 != 0) {
        field_0x3F4--;
        return false;
    }

    if (col.ChkTgHit() &&
        (col.ChkTgAtHitType(AT_TYPE_0x2) || col.ChkTgAtHitType(AT_TYPE_0x8) || col.ChkTgAtHitType(AT_TYPE_0x20) ||
         col.ChkTgAtHitType(AT_TYPE_0x40) || col.ChkTgAtHitType(AT_TYPE_0x80) || col.ChkTgAtHitType(AT_TYPE_0x2000) ||
         col.ChkTgAtHitType(AT_TYPE_0x4000) || col.ChkTgAtHitType(AT_TYPE_0x800000) ||
         col.ChkTgAtHitType(AT_TYPE_0x800))) {
        mLinkNearby = 0;
        return true;
    }

    return false;
}

bool dAcOInsect_c::checkCaught(cCcD_Obj &col) {
    if (col.ChkTgHit() && col.ChkTgAtHitType(AT_TYPE_BUGNET)) {
        return true;
    }
    return false;
}

bool dAcOInsect_c::resetScale() {
    mScale.set(1.0f, 1.0f, 1.0f);
    return true;
}

bool dAcOInsect_c::fn_8018FAA0() {
    if (field_0x3FC.get() != 0 && field_0x40D == 0) {
        return true;
    }

    return false;
}

bool dAcOInsect_c::fn_8018FAD0() {
    f32 speed = fabsf(forwardSpeed);
    if (IsZero(speed)) {
        fn_8018FDF0(mPlane_0x3A8.GetN());
        return true;
    } else {
        f32 clampSpeed = speed < 5.0f ? forwardSpeed > 0.f ? forwardSpeed : -5.f : 5.f;

        mVec3_c pos0 = position + field_0x360;
        mVec3_c pos1 = pos0 + field_0x36C * forwardSpeed;
        int linType = fn_801900B0(pos0, pos1);
        field_0x410[0] = pos0;
        field_0x434[0] = pos1;
        // TODO
    }
}

void dAcOInsect_c::fn_8018FDF0(const mVec3_c &point) {
    mMtx_c mtx;
    field_0x360 = point;
    mtx.setAxisRotation(field_0x360, rotation.y.radian2());
    // TODO
    f32 a_comparision = 0.99f;
}

int dAcOInsect_c::fn_801900B0(const mVec3_c &point0, const mVec3_c &point1) {
    cM3dGPla pla;
    if (!dBgS_ObjLinChk::LineCross(&point0, &point1, this)) {
        return 1;
    }
    if (!dBgS_ObjLinChk::GetTriPla(&pla)) {
        return 1;
    }
    if (mMaterial != dBgS_ObjLinChk::GetMaterial()) {
        return 2;
    }

    position = dBgS_ObjLinChk::GetInstance().GetLinEnd();
    mPlane_0x3A8 = pla;
    field_0x3BC = 1;
    fn_8018FDF0(pla.GetN());
    return 0;
}

// Looks like it decides on a direction and angle based on a target point?
bool dAcOInsect_c::fn_80190180(mAng &outAng) {
    // Stack and reg problems
    mVec3_c vecs[4];
    vecs[0].set(1.0f, 0.0f, 1.0f);
    vecs[1].set(1.0f, 0.0f, -1.0f);
    vecs[2].set(-1.0f, 0.0f, -1.0f);
    vecs[3].set(-1.0f, 0.0f, 1.0f);

    int i3 = -1;

    s16 angs[4];
    angs[0] = 0x2000;
    angs[1] = 0x6000;
    angs[2] = 0xA000;
    angs[3] = 0xE000;

    if (field_0x3BF != 0) {
        f32 len = fabsf(field_0x33C.y + field_0x35C * 0.5f - position.y);
        for (int i = 0; i < 4; i++) {
            // mVec3_c tmp2 = field_0x3C0.multVec(vecs[i] * 20.0f);
            f32 len2 = fabsf(field_0x33C.y + field_0x35C * 0.5f - field_0x3C0.multVec(vecs[i] * 20.0f).y);
            if (len > len2) {
                len = len2;
                i3 = i;
            }
        }
    } else if (field_0x3BE != 0) {
        mVec3_c dir = field_0x33C - position;
        f32 len = dir.squareMagXZ();
        for (int i = 0; i < 4; i++) {
            // mVec3_c tmp2 = field_0x3C0.multVec(vecs[i] * 20.0f);
            f32 len2 = (field_0x33C - field_0x3C0.multVec(vecs[i] * 20.0f)).squareMagXZ();
            if (len > len2) {
                len = len2;
                i3 = i;
            }
        }
    }

    if (i3 >= 0) {
        outAng = angs[i3];
        field_0x45C = vecs[i3];
        return true;
    }

    return false;
}

void dAcOInsect_c::fn_80190440() {
    f32 a = 2250000.f; // float order
}

void dAcOInsect_c::fn_80190160() {
    field_0x33C = field_0x348;
    field_0x354 = field_0x358;
    field_0x40C = 0;
}
