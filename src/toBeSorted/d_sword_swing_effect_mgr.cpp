#include "toBeSorted/d_sword_swing_effect_mgr.h"

#include "c/c_math.h"
#include "egg/math/eggMatrix.h"
#include "m/m_vec.h"

// TODO: can't use EGG::Vector3f::cross because the Vector3f -> mVec3_c conversion
// forces additional stack stores. An open-coded cross function works too in this
// file but maybe pattern comes up in more files and then we can move it to d_vec?
inline void cross(mVec3_c &result, const mVec3_c &left, const mVec3_c &right) {
    result.set(
        (left.y * right.z) - (left.z * right.y), (left.z * right.x) - (left.x * right.z),
        (left.x * right.y) - (left.y * right.x)
    );
}

static bool fn_8006A8D0(const mVec2_c &v1, const mVec2_c &v2, const mVec2_c &v3, const mVec2_c &v4, mVec2_c &result) {
    mVec2_c t1 = v2 - v1;
    mVec2_c t2 = v4 - v3;

    if (cM::isZero(t1.x)) {
        if (cM::isZero(t2.x)) {
            return false;
        } else {
            result.x = v1.x;
            result.y = v3.y + (t2.y / t2.x) * (v1.x - v3.x);
        }
    } else {
        if (cM::isZero(t2.x)) {
            result.x = v3.x;
        } else {
            f32 tmp = t1.y / t1.x - t2.y / t2.x;
            if (cM::isZero(tmp)) {
                return false;
            }
            result.x = (v3.x + (((t1.y / t1.x) * v1.x - v1.y) - (t2.y / t2.x) * v3.x)) / tmp;
        }
        result.y = v1.y + (t1.y / t1.x) * (result.x - v1.x);
    }

    mVec2_c diff = result - v1;
    if (diff.x * t1.x + diff.y * t1.y >= 0.0f) {
        return false;
    }

    diff = result - v3;
    if (diff.x * t2.x + diff.y * t2.y >= 0.0f) {
        return false;
    }

    return true;
}

void dSwordSwingEffectProcMgr_c::createSwingEntries(const mVec3_c &v1, const mVec3_c &v2) {
    mVec3_c lastDiff = field_0x44 - field_0x38;
    mVec3_c diff = v1 - v2;
    
    mVec3_c cross_;
    cross(cross_, lastDiff, diff);

    f32 diffMag = diff.mag();

    lastDiff.normalize();

    diff /= diffMag;

    cross_.normalize();

    // Same code as in dowsing_target...
    f32 dot = lastDiff.dot(diff);
    dot = cM::minMaxLimit(dot, -1.0f, 1.0f);
    f32 a = 1.0f - dot * dot <= 0.0f ? 0.0f : nw4r::math::FrSqrt(1.0f - dot * dot) * (1.0f - dot * dot);
    EGG::Matrix34f mat;
    mat.setAxisRotation(cross_, cM::atan2f(a, dot) * 0.1f);

    mVec2_c p1, p2, p3, p4;
    mVec2_c res1, res2, res3;

    p1.set(field_0x38.x, field_0x38.y);
    p2.set(field_0x44.x, field_0x44.y);
    p3.set(v2.x, v2.y);
    p4.set(v1.x, v1.y);
    bool b1 = fn_8006A8D0(p1, p2, p3, p4, res1);

    p1.set(field_0x38.y, field_0x38.z);
    p2.set(field_0x44.y, field_0x44.z);
    p3.set(v2.y, v2.z);
    p4.set(v1.y, v1.z);
    bool b2 = fn_8006A8D0(p1, p2, p3, p4, res2);

    p1.set(field_0x38.z, field_0x38.x);
    p2.set(field_0x44.z, field_0x44.x);
    p3.set(v2.z, v2.x);
    p4.set(v1.z, v1.x);
    bool b3 = fn_8006A8D0(p1, p2, p3, p4, res3);

    bool b;
    mVec3_c local_124 = (v2 + field_0x38) * 0.5f;
    mVec3_c diff1;
    mVec3_c v;
    f32 f_29;
    f32 f_28;
    EGG::Matrix34f mat2;
    int i = 0;
    if (!b1) {
        i += 1;
    }
    if (!b2) {
        i += 1;
    }

    if (!b3) {
        i += 1;
    }

    if (i >= 2) {
        v = (v2 - field_0x38) * 0.1f;
        b = false;
    } else {
        if (b1 && b3) {
            local_124.x = (res1.x + res3.y) * 0.5f;
        } else if (b1) {
            local_124.x = res1.x;
        } else if (b3) {
            local_124.x = res3.y;
        }

        if (b2 && b1) {
            local_124.y = (res2.x + res1.y) * 0.5f;
        } else if (b2) {
            local_124.y = res2.x;
        } else if (b1) {
            local_124.y = res1.y;
        }

        if (b3 && b2) {
            local_124.z = (res3.x + res2.y) * 0.5f;
        } else if (b3) {
            local_124.z = res3.x;
        } else if (b2) {
            local_124.z = res2.y;
        }

        diff1 = field_0x38 - local_124;
        mVec3_c diff2 = v2 - local_124;

        f_29 = diff1.mag();
        f_28 = (diff2.mag() - f_29) * 0.1f;

        diff2.normalize();
        diff1.normalize();

        cross(cross_, diff1, diff2);
        cross_.normalize();

        // Same code as in dowsing_target...
        f32 dot = diff1.dot(diff2);
        dot = cM::minMaxLimit(dot, -1.0f, 1.0f);
        f32 a = 1.0f - dot * dot <= 0.0f ? 0.0f : nw4r::math::FrSqrt(1.0f - dot * dot) * (1.0f - dot * dot);
        mat2.setAxisRotation(cross_, cM::atan2f(a, dot) * 0.1f);

        b = true;
    }

    f32 blub = 0.1f;
    for (int i = 1; i <= 10; i++) {
        mVec3_c a;
        mVec3_c t;
        if (b) {
            MTXMultVecSR(mat2.m, diff1, diff1);
            t = local_124 + (diff1 * (f_29 + (f_28 * i)));
        } else {
            t = field_0x38 + v * i;
        }

        MTXMultVecSR(mat.m, lastDiff, lastDiff);
        a = t + lastDiff * diffMag;

        mProc.addSwing(a, mC1, t, mC2);
        // Unused
        blub += 0.1f;
    }
}

void dSwordSwingEffectProcMgr_c::calc(const mVec3_c &v1, const mVec3_c &v2) {
    mProc.calc(0.1f);
    if (mIsActive) {
        createSwingEntries(v1, v2);
    }
    field_0x44 = v1;
    field_0x38 = v2;
}

void dSwordSwingEffectProcMgr_c::entry() {
    mProc.entry();
}
