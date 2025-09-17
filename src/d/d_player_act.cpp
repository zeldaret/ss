#include "d/d_player_act.h"

#include "common.h"
#include "d/a/d_a_item.h"
#include "d/a/d_a_itembase.h"
#include "d/a/d_a_player.h"
#include "d/a/obj/d_a_obj_base.h"
#include "m/m_angle.h"
#include "m/m_color.h"
#include "m/m_vec.h"
#include "toBeSorted/event_manager.h"

mVec3_c daPlayerActBase_c::sPos1(mVec3_c::Zero);
mVec3_c daPlayerActBase_c::sPos2(mVec3_c::Zero);
mAng daPlayerActBase_c::sRot1(0);
mAng daPlayerActBase_c::sRot2(0);
const f32 daPlayerActBase_c::sPushRelatedConstant = 75.0f;

int daPlayerActBase_c::preExecute() {
    if (dAcObjBase_c::preExecute() == NOT_READY) {
        return NOT_READY;
    }
    someFlags_0x350 = 0;
    someFlags_0x354 = 0;
    return SUCCEEDED;
}

void daPlayerActBase_c::postExecute(MAIN_STATE_e state) {
    if (state != CANCELED) {
        someFlags_0x358 = 0;
        mForceOrPreventActionFlags = 0;
    }
    dAcObjBase_c::postExecute(state);
}

bool daPlayerActBase_c::isInEvent() {
    return EventManager::isInEvent();
}

bool daPlayerActBase_c::isAttackingLeft() const {
    return mAttackDirection == ATTACK_SPIN_LEFT || mAttackDirection == ATTACK_SWING_LEFT;
}

bool daPlayerActBase_c::isAttackingRight() const {
    return mAttackDirection == ATTACK_SPIN_RIGHT || mAttackDirection == ATTACK_SWING_RIGHT;
}

bool daPlayerActBase_c::isAttackingDown() const {
    return mAttackDirection == ATTACK_SWING_DOWN || mAttackDirection == ATTACK_SPIN_DOWN ||
           mAttackDirection == ATTACK_JUMP_SLASH;
}

bool daPlayerActBase_c::isAttackingUp() const {
    return mAttackDirection == ATTACK_SPIN_UP || mAttackDirection == ATTACK_SWING_UP;
}

bool daPlayerActBase_c::isAttackingStab() const {
    return mAttackDirection == ATTACK_STAB;
}

bool daPlayerActBase_c::isAttackingSpinHorizontal() const {
    return mAttackDirection == ATTACK_SPIN_LEFT || mAttackDirection == ATTACK_SPIN_RIGHT;
}

bool daPlayerActBase_c::isAttackingSpinVertical() const {
    return mAttackDirection == ATTACK_SPIN_DOWN || mAttackDirection == ATTACK_SPIN_UP;
}

void daPlayerActBase_c::setBonkRelatedAnimFlag(bool b) {
    if (b) {
        field_0x360 |= 2;
    } else {
        field_0x360 |= 1;
    }
}

void daPlayerActBase_c::setPosRot(const mVec3_c *pos, const mAng3_c *rot, bool force, UNKWORD, UNKWORD) {
    if (isInEvent() || force) {
        mStts.ClrCcMove();
        if (pos != nullptr) {
            position = *pos;
            mOldPosition = position;
            velocity.y = 0.0f;
        }
        if (rot != nullptr) {
            mRotation = *rot;
            angle.y = mRotation.y;
        }
    }
}

void daPlayerActBase_c::setPosYRot(const mVec3_c &pos, mAng rot, bool force, UNKWORD a4, UNKWORD a5) {
    mAng3_c ang(0, rot, 0);
    setPosRot(&pos, &ang, force, a4, a5);
}

void daPlayerActBase_c::setTransform(const mMtx_c &mtx, bool force, UNKWORD a4, UNKWORD a5) {
    mVec3_c translate;
    mtx.getTranslation(translate);
    mAng3_c rot;
    mtx.toRot(rot);
    setPosRot(&translate, &rot, force, a4, a5);
}

extern "C" bool fn_80213090(dAcEnBase_c *);

bool daPlayerActBase_c::someTargetedActorCheck() const {
    return fn_80213090(getTargetedActor1());
}

mAng daPlayerActBase_c::fn_8005BA90() {
    return dAcPy_c::LINK2->fn_8023B520();
}

mAng daPlayerActBase_c::fn_8005BAA0() {
    return dAcPy_c::LINK2->fn_8023B590();
}

const mColor &daPlayerActBase_c::getEarringsColor() {
    static const mColor sNoFireshieldEarrings = mColor(45, 80, 127, 255);
    static const mColor sFireshieldEarrings = mColor(222, 57, 20, 255);
    if (dAcItem_c::hasItem(ITEM_FIRESHIELD_EARRINGS)) {
        return sFireshieldEarrings;
    } else {
        return sNoFireshieldEarrings;
    }
}
