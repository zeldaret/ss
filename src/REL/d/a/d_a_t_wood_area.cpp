#include "d/a/d_a_t_wood_area.h"

#include "common.h"
#include "d/a/d_a_player.h"
#include "toBeSorted/d_emitter.h"

const f32 dAcTWoodArea_c::scaleX = 100.0f;
const f32 dAcTWoodArea_c::scaleY = 100.0f;

SPECIAL_ACTOR_PROFILE(WOODAREA_TAG, dAcTWoodArea_c, fProfile::WOODAREA_TAG, 0x166, 0, 2);

STATE_DEFINE(dAcTWoodArea_c, Init);
STATE_DEFINE(dAcTWoodArea_c, Wait);

int dAcTWoodArea_c::actorCreate() {
    mStateMgr.changeState(StateID_Init);
    PSMTXTrans(mWorldMtx.m, position.x, position.y, position.z);
    boundingBox.mMin = mVec3_c(-0.0f, -0.0f, -0.0f);
    boundingBox.mMax = mVec3_c(0.0f, 0.0f, 0.0f);
    return SUCCEEDED;
}

static const ProfileName FILTER_PROFILE[] = {
    fProfile::OBJ_FRUIT, fProfile::ITEM, fProfile::NPC_KYUI_THIRD, fProfile::OBJ_IVY_ROPE, fProfile::NPC_SORAJIMA_BOY,
};

int dAcTWoodArea_c::actorPostCreate() {
    for (u32 i = 0; i < 5; i++) {
        attachCloseObjects(FILTER_PROFILE[i]);
    }
    return SUCCEEDED;
}

int dAcTWoodArea_c::doDelete() {
    return SUCCEEDED;
}

int dAcTWoodArea_c::actorExecute() {
    mStateMgr.executeState();
    return SUCCEEDED;
}

int dAcTWoodArea_c::draw() {
    return SUCCEEDED;
}

void dAcTWoodArea_c::initializeState_Init() {}
void dAcTWoodArea_c::executeState_Init() {
    mStateMgr.changeState(StateID_Wait);
}
void dAcTWoodArea_c::finalizeState_Init() {}

extern const u16 PARTICLE_RESOURCE_ID_MAPPING_8_;

void dAcTWoodArea_c::initializeState_Wait() {}

void dAcTWoodArea_c::executeState_Wait() {
    if (dAcPy_c::LINK != nullptr && (dAcPy_c::LINK->someFlags_0x350 & 0x2000)) {
        // This is a bit messed up
        mVec3_c dist = position - dAcPy_c::LINK->position;
        f32 attachRadius = getAttachRadius();
        attachRadius = attachRadius * attachRadius;
        if (dist.x * dist.x + dist.z * dist.z < attachRadius) {
            bool someEffectThing = subtype != 1 ? (params & 0xF) != 0 ? false : true : true;
            if (someEffectThing) {
                mVec3_c tmp(position.x, position.y + getAttachHeight(), position.z);
                dJEffManager_c::spawnEffect(
                    PARTICLE_RESOURCE_ID_MAPPING_8_, tmp, nullptr, nullptr, nullptr, nullptr, 0, 0
                );
            }
            dropItems();
        }
    }
}
void dAcTWoodArea_c::finalizeState_Wait() {}

void dAcTWoodArea_c::attachCloseObjects(ProfileName profID) {
    fBase_c *base = nullptr;
    f32 attachRadius = getAttachRadius() * getAttachRadius();
    while (true) {
        base = fManager_c::searchBaseByProfName(profID, base);
        if (base == nullptr) {
            break;
        }

        dAcObjBase_c *obj = static_cast<dAcObjBase_c *>(base);
        if (obj->canBeLinkedToWoodTag()) {
            if (!(obj->getSquareDistanceTo(position) > attachRadius)) {
                if (!attachObject(obj)) {
                    return;
                }
            }
        }
    }
}

void dAcTWoodArea_c::dropItems() {
    for (int i = 0; i < 8; i++) {
        if (mRefs[i].get() != nullptr) {
            if (mRefs[i].get()->drop()) {
                mRefs[i].unlink();
            }
        }
    }
}

bool dAcTWoodArea_c::attachObject(dAcObjBase_c *obj) {
    for (int i = 0; i < 8; i++) {
        if (mRefs[i].get() == nullptr) {
            mRefs[i].link(obj);
            return true;
        }
    }
    return false;
}
