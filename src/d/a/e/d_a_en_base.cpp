#include "d/a/e/d_a_en_base.h"

#include "c/c_math.h"
#include "common.h"
#include "d/a/d_a_player.h"
#include "d/a/obj/d_a_obj_base.h"
#include "d/a/obj/d_a_obj_bomb.h"
#include "d/col/bg/d_bg_pc.h"
#include "d/col/bg/d_bg_s_gnd_chk.h"
#include "d/col/bg/d_bg_s_lin_chk.h"
#include "d/col/bg/d_bg_s_wtr_chk.h"
#include "d/col/c/c_cc_d.h"
#include "d/d_sc_game.h"
#include "d/d_stage_mgr.h"
#include "d/d_vec.h"
#include "d/snd/d_snd_wzsound.h"
#include "d/t/d_t_siren.h"
#include "f/f_base.h"
#include "f/f_list_mg.h"
#include "f/f_profile_name.h"
#include "m/m_vec.h"
#include "nw4r/g3d/g3d_camera.h"
#include "sized_string.h"
#include "toBeSorted/attention.h"
#include "toBeSorted/d_emitter.h"

const dAcEnData dAcEnBase_c::sDefaultEnData = {};

fLiMgBa_c dAcEnBase_c::sEnemyList;

dAcEnBase_c::dAcEnBase_c() : mEnemyLink(this) {
    mInteractionFlags = INTERACT_0x4;
    mpEnemyData = &sDefaultEnData;
    sEnemyList.append(&mEnemyLink);

    mTargetFiRelated = 1;
}

dAcEnBase_c::~dAcEnBase_c() {
    sEnemyList.remove(&mEnemyLink);
}

int dAcEnBase_c::preExecute() {
    if (dAcObjBase_c::preExecute() == NOT_READY) {
        return NOT_READY;
    }

    if (mPosition.y < mStartingPos.y - 10000.0f) {
        if (field_0x346) {
            killWithFlagNoItemDrop();
        } else {
            deleteWithFlagNoItemDrop();
        }
        return NOT_READY;
    }

    if (dTgSiren_c::isOutOfTime() && !checkInteractionFlags(INTERACT_0x10)) {
        return NOT_READY;
    }
    return SUCCEEDED;
}

void dAcEnBase_c::postExecute(MAIN_STATE_e state) {
    field_0x338 = 0;

    if (state != CANCELED && checkInteractionFlags(INTERACT_0x4)) {
        AttentionManager::GetInstance()->addUnk3Target(
            *this, 1, mpEnemyData->field_0x0C, mpEnemyData->field_0x10, mpEnemyData->field_0x14
        );
    }

    return dAcObjBase_c::postExecute(state);
}

int dAcEnBase_c::preDraw() {
    if (dAcObjBase_c::preDraw() == NOT_READY) {
        return NOT_READY;
    }

    if (dTgSiren_c::isOutOfTime() && !checkInteractionFlags(INTERACT_0x10)) {
        return NOT_READY;
    }

    return SUCCEEDED;
}

dAcObjBase_c *dAcEnBase_c::isPlayerOwnedActorNearby(ProfileName profile, const mVec3_c &pos, f32 radius) {
    dAcPy_c *pPlayer = dAcPy_c::GetLinkM();

    f32 rr = radius * radius;
    dAcObjBase_c *pRet = nullptr;
    for (int i = 0; i < pPlayer->getBeetleActorReferencesSize(); ++i) {
        dAcObjBase_c *pObj = pPlayer->checkBeetleActorReferences(i);
        if (pObj == nullptr) {
            break;
        }
        if (pObj->mProfileName == profile) {
            if (rr == 0.0f || pObj->mPosition.squareDistanceToXZ(pos) < rr) {
                pRet = pObj;
                break;
            }
        }
    }
    return pRet;
}

dAcBomb_c *dAcEnBase_c::isPlayerOwnedBombNearby(const mVec3_c &pos, f32 radius) {
    return static_cast<dAcBomb_c *>(isPlayerOwnedActorNearby(fProfile::BOMB, pos, radius));
}

dAcBomb_c *dAcEnBase_c::getBombWithinRadius(f32 radius) {
    return isPlayerOwnedBombNearby(mPosition, radius);
}

dAcObjBase_c *dAcEnBase_c::isPlayerOwnedProjectileNearby(const mVec3_c &pos, f32 radius) {
    dAcPy_c *pPlayer = dAcPy_c::GetLinkM();

    f32 rr = radius * radius;
    dAcObjBase_c *pRet = nullptr;
    for (int i = 0; i < pPlayer->sizeOfActorRefs_16(); ++i) {
        dAcObjBase_c *pObj = pPlayer->checkRefArrayAtIndex(i);
        if (pObj == nullptr) {
            break;
        }
        if (rr == 0.0f || pObj->mPosition.squareDistanceToXZ(pos) < rr) {
            pRet = pObj;
            break;
        }
    }
    return pRet;
}

dAcObjBase_c *dAcEnBase_c::isProjectileNearby(f32 radius) {
    return isPlayerOwnedProjectileNearby(mPosition, radius);
}

void dAcEnBase_c::playDeathEffect0(const mVec3_c &pos, const mVec3_c *pScale, bool skipKill) {
    dJEffManager_c::spawnEffect(PARTICLE_RESOURCE_ID_MAPPING_319_, pos, nullptr, pScale, nullptr, nullptr, 0, 0);
    startSound(SE_E_DISAPPEAR);
    if (!skipKill) {
        killWithFlag();
    }
}

void dAcEnBase_c::playDeathEffect1(const mVec3_c &pos, const mVec3_c *pScale, bool skipKill) {
    dJEffManager_c::spawnEffect(PARTICLE_RESOURCE_ID_MAPPING_966_, pos, nullptr, pScale, nullptr, nullptr, 0, 0);
    startSound(SE_E_DISAPPEAR);
    if (!skipKill) {
        killWithFlag();
    }
}

void dAcEnBase_c::playDeathEffect2(const mVec3_c &pos, const mVec3_c *pScale, bool skipKill) {
    if (dScGame_c::isCurrentStage("F403")) {
        dJEffManager_c::spawnEffect(PARTICLE_RESOURCE_ID_MAPPING_318_, pos, nullptr, pScale, nullptr, nullptr, 0, 0);
        startSound(SE_E_DISAPPEAR);
        if (!skipKill) {
            killWithFlag();
        }
    } else {
        playDeathEffect0(pos, pScale, skipKill);
    }
}

void dAcEnBase_c::setEndDat(const char *oarc, const char *endatName) {
    if (oarc != nullptr && endatName != nullptr) {
        SizedString<32> datName;
        datName.sprintf("dat/%s.%s", endatName, "endat");

        const void *pEnData = getOarcSubEntry(oarc, datName);
        if (pEnData != nullptr) {
            mpEnemyData = static_cast<const dAcEnData *>(pEnData);
        }
    }

    mHealth = mpEnemyData->health;
}

int getHitRelatedIndex(u32 tgAtType, u16 tgDamageFlags, u32 idk) {
    if (tgAtType & AT_TYPE_BELLOWS) {
        if (tgDamageFlags & AT_MOD_FIRE) {
            return 6;
        }
        if (tgDamageFlags & AT_MOD_0x2) {
            return 7;
        }
        if (tgDamageFlags & AT_MOD_ELECTRIC) {
            return 8;
        }
        if (tgDamageFlags & AT_MOD_WIND) {
            return 9;
        }
    } else {
        if (tgAtType & AT_TYPE_BOMB) {
            return 2;
        }
        if (tgAtType & (AT_TYPE_BEETLE | AT_TYPE_0x40)) {
            return 3;
        }
        if (tgAtType & AT_TYPE_ARROW) {
            return 4;
        }
        if (tgAtType & AT_TYPE_CLAWSHOT) {
            return 5;
        }
        if (tgAtType & AT_TYPE_SLINGSHOT) {
            return 10;
        }
        if (tgAtType & AT_TYPE_SWORD) {
            return idk;
        }
    }

    return 31;
}

u32 getAtFlagsUpper(u8 thing) {
    u32 flags = 0;
    if (thing & 0x1) {
        flags |= 0x10000;
    }
    if (thing & 0x2) {
        flags |= 0x200000;
    }
    if (thing & 0x4) {
        flags |= 0x40000;
    }
    return flags;
}

void updateCc(cCcD_Obj &pCc, const dAcEnData::ccRelated *pStuff) {
    pCc.SetAtDamage(pStuff->field_0x00);
    pCc.SetAt_0xE(pStuff->field_0x02);
    pCc.SetAtFlagsUpper(getAtFlagsUpper(pStuff->field_0x03));
}

void ccForEach_cb(cCcD_Obj *pCc, const void *pStuff) {
    updateCc(*pCc, (const dAcEnData::ccRelated *)pStuff);
}

void dAcEnBase_c::setCcFromEnemyData(cCcD_Obj &pCc, int idx) {
    updateCc(pCc, &mpEnemyData->field_0x18[idx]);
}
void dAcEnBase_c::setCcFromEnemyData(dColliderLinkedList &ccList, int idx) {
    ccList.foreachCc(ccForEach_cb, &mpEnemyData->field_0x18[idx]);
}

u16 dAcEnBase_c::calcHealth(u32 damage, cCcD_Obj *pCc) {
    if (pCc != nullptr) {
        int idx = getHitRelatedIndex(pCc->GetTgAtHitType(), pCc->GetTgDamageFlags(), pCc->ChkTgBit25());

        const dAcEnData::damgeRelated &enDataStuff = mpEnemyData->field_0x38[idx];

        if ((enDataStuff.field_0x00 == 1) || pCc->ChkTg_0x50(pCc->GetTgAtHitType()) ||
            (pCc->GetTgAtHitType() == AT_TYPE_SWORD && pCc->ChkTgElectric())) {
            return mHealth;
        }

        if (enDataStuff.field_0x00 == 2) {
            damage = enDataStuff.field_0x01;
        }
    }

    if (damage >= mHealth) {
        mHealth = 0;
    } else {
        mHealth -= damage;
    }

    return mHealth;
}

int dAcEnBase_c::someEnemyDamageCollisionStuffMaybe(dColliderLinkedList &list, u16 *pOutMod) {
    cCcD_Obj *pCc = list.findTgHit();
    if (pCc != nullptr) {
        return fn_8002FDE0(*pCc, pOutMod);
    }
    return 0;
}

extern "C" void fn_8005BBB0(u16 something);

int dAcEnBase_c::fn_8002FDE0(cCcD_Obj &cc, u16 *pOutMod) {
    if (!cc.ChkTgHit()) {
        return 0;
    }
    (void)cc.GetTgActor()->isActorPlayer();
    u16 modifier = cc.GetTgDamageFlags();
    if (pOutMod != nullptr) {
        *pOutMod = modifier;
    }

    int hitType = cc.GetTgAtHitType();
    int damage = cc.GetTgDamage();

    if (damage == 0 && (hitType & (AT_TYPE_BELLOWS | AT_TYPE_0x8000 | AT_TYPE_WIND | AT_TYPE_0x100)) != 0) {
        return 7;
    }

    if (cc.ChkTgBit1() || cc.ChkTgCutHit()) {
        return 6;
    }

    fn_8005BBB0(cc.GetTg_0x4A());

    s32 priorHealth = mHealth;
    calcHealth(damage, &cc);

    if (!checkInteractionFlags(INTERACT_0x40)) {
        (void)getSoundSource();
    }

    if (cc.ChkTgSkywardStrike()) {
        if (priorHealth == mHealth) {
            return 13;
        } else {
            return 2;
        }
    }

    if (priorHealth == mHealth) {
        return 12;
    }

    return 1;
}

bool dAcEnBase_c::snapToGround(f32 range) {
    bool moved = false;

    dBgS_ObjLinChk linChk;
    mVec3_c end = mPosition;
    end.y += range;

    linChk.Set(&mPosition, &end, nullptr);
    if (dBgS::GetInstance()->LineCross(&linChk)) {
        end.y = linChk.GetLinEnd().y - 10.0f;
    }

    if (dBgS_ObjGndChk::CheckPos(end)) {
        moved = true;

        mStartingPos.y = mPosition.y = dBgS_ObjGndChk::GetGroundHeight();
    }

    return moved;
}

void dAcEnBase_c::checkSlope(const mVec3_c &pos, s16 &outPitch, s16 &outRoll, f32 radius) {
    mVec3_c testPos;
    s16 out1 = 0;
    s16 out0 = 0;

    testPos.x = pos.x;
    testPos.y = pos.y + 100.0f;
    testPos.z = pos.z;

    if (dBgS_ObjGndChk::CheckPos(testPos)) {
        f32 gndHeight = dBgS_ObjGndChk::GetGroundHeight();

        testPos.x = pos.x;
        testPos.y = pos.y + 100.0f;
        testPos.z = pos.z + radius;
        if (dBgS_ObjGndChk::CheckPos(testPos)) {
            testPos.y = dBgS_ObjGndChk::GetGroundHeight();
            out0 = -cM::atan2s(testPos.y - gndHeight, testPos.z - pos.z);
            if (u16(out0 + 0x3000) > 0x6000) {
                out0 = 0;
            }
        }
        testPos.x = pos.x + radius;
        testPos.y = pos.y + 100.0f;
        testPos.z = pos.z;
        if (dBgS_ObjGndChk::CheckPos(testPos)) {
            testPos.y = dBgS_ObjGndChk::GetGroundHeight();
            out1 = cM::atan2s(testPos.y - gndHeight, testPos.x - pos.x);
            if (u16(out1 + 0x3000) > 0x6000) {
                out1 = 0;
            }
        }
    }
    outPitch = out0;
    outRoll = out1;
}

int dAcEnBase_c::fn_800301b0(const mVec3_c &pos, const mAng ang, bool allowLinCross, f32 increment) {
    mVec3_c end(pos.x, pos.y + 100.0f, pos.z);

    getXZCirclePoint(end, ang, increment);

    if (!dBgS_ObjLinChk::LineCross(&pos, &end, nullptr)) {
        dBgS_ObjGndChk gndChk;
        gndChk.SetPos(&end);
        f32 gndH = dBgS::GetInstance()->GroundCross(&gndChk);
        if (gndH == -1e9f || gndH == 4.0f) {
            return 2;
        }

        if (dBgS_WtrChk::CheckPos(&end, true, 500.0f, -500.0f) && dBgS_WtrChk::GetWaterHeight() > gndH) {
            return 2;
        }

        int attr = dBgS::GetInstance()->GetSpecialCode(gndChk);
        if (attr == POLY_ATTR_SAND_DEEP_INSTANT || attr == POLY_ATTR_SAND_DEEP_SLOW || attr == POLY_ATTR_LAVA ||
            attr == POLY_ATTR_SAND_MED) {
            return 3;
        }
    } else if (allowLinCross) {
        return 1;
    }

    return 0;
}

bool dAcEnBase_c::lineCrossPlayer(f32 height) {
    mVec3_c end = mPositionCopy2;
    mVec3_c start = dAcPy_c::GetLink()->mPositionCopy2;
    start.y += height;

    return dBgS_ObjLinChk::LineCross(&start, &end, nullptr);
}

bool dAcEnBase_c::isWithinCameraView(f32 w, f32 h) {
    nw4r::g3d::Camera cam = dStageMgr_c::GetInstance()->getCamera(0);

    f32 x, y, width, height, near, far;
    cam.GetViewport(&x, &y, &width, &height, &near, &far);

    mVec3_c src;
    cam.Project(src, mPosition);

    return ((width * w <= src.x) && (src.x < width * (1.0f - w))) &&
           ((height * h <= src.y) && src.y < height * (1.0f - h)) && ((near <= src.z) && (src.z < far));
}
