#include "d/d_linkage.h"

#include "common.h"
#include "d/a/d_a_player.h"
#include "d/a/obj/d_a_obj_base.h"
#include "d/col/bg/d_bg_s.h"
#include "d/col/bg/d_bg_s_acch.h"
#include "f/f_profile_name.h"
#include "m/m_mtx.h"
#include "m/m_vec.h"
#include "rvl/MTX/mtx.h"

dLinkage_c::dLinkage_c()
    : mControllingActor(nullptr),
      pLinkedActor(nullptr),
      carryFlags(0),
      mType(CONNECTION_0),
      field_0x18(0),
      field_0x1C(0.0f),
      field_0x20(0.f),
      field_0x24(0.f),
      carryTransMtx(mMtx_c::Identity),
      field_0x58(mMtx_c::Identity),
      mState(STATE_0),
      field_0x8C(0.0f),
      field_0x90(0.0f),
      field_0x94(0.0f),
      field_0x98(0.0f) {}

dLinkage_c::~dLinkage_c() {
    fn_80051190(nullptr);
    fn_80050EB0(nullptr);
}

// Ignore the parameter names. I am unsure what they are
void dLinkage_c::set(u32 flags, f32 x, f32 y, f32 z, void *) {
    onFlag(0x8000000 | flags);
    field_0x1C = x;
    field_0x20 = y;
    field_0x24 = z;

    if (flags & 2) {
        field_0x8C = 28.f;
        field_0x90 = 36.f;
        field_0x94 = 28.f;
        field_0x98 = 0.f;
    } else if (flags & 4) {
        field_0x8C = 18.f;
        field_0x90 = 25.f;
        field_0x94 = 18.f;
        field_0x98 = 0.f;
    } else if (flags & 8) {
        field_0x8C = 16.7f;
        field_0x90 = 25.f;
        field_0x94 = 16.7f;
        field_0x98 = 0.f;
    } else {
        field_0x8C = 18.f;
        field_0x90 = 30.f;
        field_0x94 = 18.f;
        field_0x98 = 0.f;
    }
}

bool dLinkage_c::tryAttach(
    dAcObjBase_c *pActor, dAcObjBase_c *pSrc, dAcRefBase_c *pRef, dLinkage_c::ConnectionType_e type, bool replace
) {
    if (checkState(STATE_ACTIVE)) {
        if (replace) {
            fn_80050EB0(pActor);
        } else {
            return false;
        }
    }
    pActor->clearObjectProperty(0x1000);
    pActor->clearObjectProperty(0x4);
    mControllingActor.link(pSrc);
    mState = STATE_ACTIVE;
    mType = type;

    if (type == CONNECTION_4) {
        pActor->setObjectProperty(0x4);
    } else if (type == CONNECTION_7) {
        pActor->setObjectProperty(0x1000);
        field_0x1A = 4;
    }
    MTXIdentity(carryTransMtx);
    pLinkedActor = pRef;
    pRef->link(pActor);
    if ((pSrc == dAcPy_c::GetLink2() || type == CONNECTION_5) &&
        pActor->getConnectParent()->profile_name == fProfile::ROOM) {
        pActor->addActorToRoom(-1);
        onFlag(0x20000000);
        pActor->setActorProperty(0x2000000);
        pActor->changeLoadedEntitiesWithSet();
    }
    pActor->clearActorProperty(0x1);
    return true;
}

void dLinkage_c::fn_80050DC0(dAcObjBase_c *pActor, const f32 speed, const f32 velocityY, mAng rot) {
    mState = STATE_2;
    fn_80050E40(pActor, speed, velocityY, rot);
}

void dLinkage_c::fn_80050E00(dAcObjBase_c *pActor, const f32 speed, const f32 velocityY, mAng rot) {
    mState = STATE_3;
    fn_80050E40(pActor, speed, velocityY, rot);
}

void dLinkage_c::fn_80050E40(dAcObjBase_c *pActor, const f32 speed, const f32 velocityY, mAng rot) {
    fn_80050EB0(pActor);
    pActor->forwardSpeed = speed;
    pActor->velocity.y = velocityY;
    pActor->angle.y = rot;
}

void dLinkage_c::fn_80050EA0(dAcObjBase_c *pActor) {
    mState = STATE_DELETE;
    fn_80050EB0(pActor);
}

void dLinkage_c::fn_80050EB0(dAcObjBase_c *pActor) {
    mControllingActor.unlink();
    if (pLinkedActor) {
        pLinkedActor->unlink();
        pLinkedActor = nullptr;
    }

    if (checkType_1_3_5_6_9()) {
        MTXConcat(carryTransMtx, field_0x58, field_0x58);
        field_0x58.fn_802F1C40(2, 0);
        field_0x18 = 5;
        MTXIdentity(carryTransMtx);
    } else if (mType == CONNECTION_4) {
        if (pActor) {
            pActor->clearObjectProperty(0x4);
        }
    } else if (mType == CONNECTION_7) {
        if (pActor) {
            pActor->clearObjectProperty(0x1000);
        }
    }

    if (pActor && checkFlag(0x20000000)) {
        pActor->addActorToRoom(pActor->getRoomId());
        offFlag(0x20000000);
    }

    field_0x1A = 4;
    mType = CONNECTION_0;
    offFlag(0x10000000);
}

void dLinkage_c::bushTpFunc(dBgS_Acch &acch) {
    mVec3_c *pos = acch.GetPos();
    mVec3_c origPos = *pos;

    if (field_0x1A != 0) {
        if (field_0x1A == 4 && !acch.ChkLineCheck()) {
            acch.OnLineCheck();
            onFlag(0x4000000);
        }

        if (--field_0x1A == 0 && checkFlag(0x4000000)) {
            acch.OffLineCheck();
            offFlag(0x4000000);
        }
    }

    acch.CrrPos(*dBgS::GetInstance());
    if (mState == STATE_ACTIVE) {
        dAcPy_c *player = dAcPy_c::GetLink2();
        if (mType == CONNECTION_1 && mControllingActor.p_owner == player) {
            player->fn_80202E00(nullptr, pos->x - origPos.x, pos->z - origPos.z);
        }

        if (mType == CONNECTION_7 && mControllingActor.p_owner == player) {
            if (!checkFlag(0x10000)) {
                if (acch.ChkWallHit(nullptr)) {
                    fn_80050EA0(player->getWhippedItem());
                    return;
                }
                field_0x1A = 3;
            }
        }

        *pos = origPos;
    }
}

void dLinkage_c::fn_80051190(dAcObjBase_c *pActor) {
    // If not active, return
    if (!checkState(STATE_ACTIVE)) {
        return;
    }

    // If currently Linked
    if (mControllingActor.p_owner) {
        // if the actor being linked to is empty, no need to delink
        if (pLinkedActor == nullptr) {
            return;
        }

        // If the Actor is linked to itself, return?
        dAcObjBase_c *pLinked = static_cast<dAcObjBase_c *>(pLinkedActor->p_owner);
        if (pLinked && &pLinked->GetLinkage() == this) {
            return;
        }
    }
    pLinkedActor = nullptr;

    fn_80050EA0(pActor);
}

void dLinkage_c::fn_800511E0(dAcObjBase_c *pActor) {
    bool b = checkType_1_3_5_6_9();

    if (!(checkFlag(0x200) || b || field_0x18 == 0)) {
        field_0x58.slerpTo(mMtx_c::Identity, field_0x58, 1.f / field_0x18);
        field_0x58.fn_802F1C40(2, 0);
        field_0x18--;
    }
    mMtx_c &mtx = pActor->mWorldMtx;
    f32 y = field_0x1C * pActor->mScale.y;
    mtx.transS(pActor->GetPosition().x, pActor->GetPosition().y + pActor->GetYOffset(), pActor->GetPosition().z);
    if (mType == CONNECTION_1) {
        MTXConcat(mtx, carryTransMtx, mtx);
    }
    mMtx_c m1;
    MTXTrans(m1, 0.f, y, 0.f);
    MTXConcat(mtx, m1, mtx);

    if (b && mType != CONNECTION_1) {
        MTXConcat(mtx, carryTransMtx, mtx);
    }
    mMtx_c m2;
    MTXConcat(mtx, field_0x58, mtx);
    MTXTrans(m2, 0.f, -y, 0.f);
    MTXConcat(mtx, m2, mtx);

    mtx.ZXYrotM(pActor->rotation);
    if (mControllingActor.p_owner) {
        pActor->roomid = static_cast<dAcObjBase_c *>(mControllingActor.p_owner)->roomid;
    }
}

bool dLinkage_c::fn_800513C0() const {
    return (mState == STATE_ACTIVE && (mType == CONNECTION_1 || mType == CONNECTION_6)) &&
           !(mControllingActor.p_owner && mControllingActor.p_owner->profile_name == fProfile::E_GEROCK);
}
