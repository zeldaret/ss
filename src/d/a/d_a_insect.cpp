#include "d/a/d_a_insect.h"

#include "common.h"
#include "d/a/d_a_player.h"
#include "egg/math/eggMath.h"
#include "m/m_vec.h"
#include "rvl/MTX/mtxvec.h"
#include "toBeSorted/attention.h"

void AcOInsectFloats() {
    50.0f;
    1.0f;
    30.0f;
    5000.0f;
    0.1f;
    10.0f;
    500.0f;
    0.0f;
    400.0f;
    -200.0f;
    200.0f;
    20.0f;
    EGG::Math<f32>::epsilon();
    5.0f;
    -5.0f;
    0.99f;
    int x = 0;
    (f32) x;
}

void AcOInsectFloats2() {
    -1.0f;
    0.5f;
    2250000.0f;
}

bool dAcOInsect_c::checkForLinkScare() {
    if (dAcPy_c::LINK != nullptr) {
        // TODO
    }

    return false;
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

bool dAcOInsect_c::isLinkNearby(f32 rad) {
    if (dAcPy_c::LINK != nullptr && isWithinPlayerRadius(rad)) {
        return true;
    }
    return false;
}

bool dAcOInsect_c::isLinkNearbyZ(f32 dist) {
    if (dAcPy_c::LINK != nullptr && fabsf(dAcPy_c::LINK->position.y - position.y) > dist) {
        return false;
    }
    return true;
}

void dAcOInsect_c::someBaseFunction() {
    dAcPy_c *link = dAcPy_c::LINK;
    if (link != nullptr) {
        if (isLinkNearbyZ(500.0f) && isLinkNearby(dAcOInsect_0x8C())) {
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
    if (isLinkNearbyZ(500.0f) && isLinkNearby(rad)) {
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
    f32 y = position.y;
    y += 20.0f;
    poscopy2.y = y;
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
            mVec3_c tmp = vecs[i] * 20.0f;
            // ???
            mVec3_c tmp2 = tmp;
            PSMTXMultVec(field_0x3C0, tmp2, tmp2);
            f32 len2 = fabsf(field_0x33C.y + field_0x35C * 0.5f - tmp2.y);
            if (len > len2) {
                len = len2;
                i3 = i;
            }
        }
    } else if (field_0x3BE != 0) {
        mVec3_c dir = field_0x33C - position;
        f32 len = dir.x * dir.x + dir.z * dir.z;
        for (int i = 0; i < 4; i++) {
            mVec3_c tmp = vecs[i] * 20.0f;
            // ???
            mVec3_c tmp2 = tmp;
            PSMTXMultVec(field_0x3C0, tmp2, tmp2);
            mVec3_c dir2 = field_0x33C - tmp2;
            f32 len2 = dir2.x * dir2.x + dir2.z * dir2.z;
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
