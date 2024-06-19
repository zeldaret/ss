#include <d/a/obj/d_a_obj_base.h>
#include <d/d_player.h>
#include <f/f_list_nd.h>
#include <s/s_State.hpp>
#include <s/s_StateMgr.hpp>

// Look I don't know why these levels of subclasses work
class fLiNdBa_Wood_c : public fLiNdBaAutoUnlink_c {
public:
    fLiNdBa_Wood_c() : fLiNdBaAutoUnlink_c(nullptr) {}
};

class dAcTWoodArea_c : public dAcObjBase_c {
public:
    dAcTWoodArea_c() : mStateMgr(*this, sStateID::null) {}
    virtual ~dAcTWoodArea_c() {}
    virtual int actorCreate() override;
    virtual int actorPostCreate() override;
    virtual int actorExecute() override;
    virtual int draw() override;
    virtual int doDelete() override;

    void attachCloseObjects(ProfileName profID);
    void dropItems();
    bool attachObject(dAcObjBase_c *obj);

    f32 getAttachRadius() {
        return scale.x * scaleX;
    }
    f32 getAttachRadiusSquare() {
        return getAttachRadius() * getAttachRadius();
    }

    f32 getAttachHeight() {
        return scale.y * scaleY;
    }

    STATE_FUNC_DECLARE(dAcTWoodArea_c, Init);
    STATE_FUNC_DECLARE(dAcTWoodArea_c, Wait);

    static const f32 scaleX;
    static const f32 scaleY;

private:
    STATE_MGR_DECLARE(dAcTWoodArea_c);

    fLiNdBa_Wood_c mRefs[8];
};

const f32 dAcTWoodArea_c::scaleX = 100.0f;
const f32 dAcTWoodArea_c::scaleY = 100.0f;

SPECIAL_ACTOR_PROFILE(TAG_WOOD_AREA, dAcTWoodArea_c, fProfile::WOODAREA_TAG, 0x166, 0, 2);

STATE_DEFINE(dAcTWoodArea_c, Init);
STATE_DEFINE(dAcTWoodArea_c, Wait);

int dAcTWoodArea_c::actorCreate() {
    mStateMgr.changeState(StateID_Init);
    PSMTXTrans(worldMatrix.m, position.x, position.y, position.z);
    boundingBox.min = mVec3_c(-0.0f, -0.0f, -0.0f);
    boundingBox.max = mVec3_c(0.0f, 0.0f, 0.0f);
    return 1;
}

static const ProfileName FILTER_PROFILE[] = {
        fProfile::OBJ_FRUIT,
        fProfile::ITEM,
        fProfile::NPC_KYUI_THIRD,
        fProfile::OBJ_IVY_ROPE,
        fProfile::NPC_SORAJIMA_BOY,
};

int dAcTWoodArea_c::actorPostCreate() {
    for (u32 i = 0; i < 5; i++) {
        attachCloseObjects(FILTER_PROFILE[i]);
    }
    return 1;
}

int dAcTWoodArea_c::doDelete() {
    return 1;
}

int dAcTWoodArea_c::actorExecute() {
    mStateMgr.executeState();
    return 1;
}

int dAcTWoodArea_c::draw() {
    return 1;
}

void dAcTWoodArea_c::initializeState_Init() {}
void dAcTWoodArea_c::executeState_Init() {
    mStateMgr.changeState(StateID_Wait);
}
void dAcTWoodArea_c::finalizeState_Init() {}

extern "C" void fn_800298B0(u16, mVec3_c *, mAng3_c *, u32, u32, u32, u32, u32);
extern u16 PARTICLE_RESOURCE_ID_MAPPING[];

void dAcTWoodArea_c::initializeState_Wait() {}
void dAcTWoodArea_c::executeState_Wait() {
    if (dPlayer::LINK != nullptr && (dPlayer::LINK->someFlags & 0x2000)) {
        // This is a bit messed up
        mVec3_c dist = position - dPlayer::LINK->position;
        f32 attachRadius = getAttachRadius();
        attachRadius = attachRadius * attachRadius;
        if (dist.x * dist.x + dist.z * dist.z < attachRadius) {
            bool someEffectThing = subtype != 1 ? (params & 0xF) != 0 ? false : true : true;
            if (someEffectThing) {
                mVec3_c tmp(position.x, position.y + getAttachHeight(), position.z);
                fn_800298B0(PARTICLE_RESOURCE_ID_MAPPING[8], &tmp, nullptr, 0, 0, 0, 0, 0);
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
        if (mRefs[i].p_owner != nullptr) {
            if (static_cast<dAcObjBase_c *>(mRefs[i].p_owner)->drop()) {
                mRefs[i].unlink();
            }
        }
    }
}

bool dAcTWoodArea_c::attachObject(dAcObjBase_c *obj) {
    for (int i = 0; i < 8; i++) {
        if (mRefs[i].p_owner == nullptr) {
            mRefs[i].link(obj);
            return true;
        }
    }
    return false;
}
