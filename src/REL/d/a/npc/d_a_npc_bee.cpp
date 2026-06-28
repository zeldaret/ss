#include "d/a/npc/d_a_npc_bee.h"
#include "c/c_lib.h"
#include "c/c_math.h"
#include "common.h"
#include "d/a/d_a_itembase.h"
#include "d/a/d_a_player.h"
#include "d/a/e/d_a_e_bc.h"
#include "d/a/npc/d_a_npc_honeycomb.h"
#include "d/a/npc/d_a_npc_insect_capture_game_kobun.h"
#include "d/a/obj/d_a_obj_base.h"
#include "d/a/obj/d_a_obj_spore.h"
#include "d/col/bg/d_bg_s.h"
#include "d/col/bg/d_bg_s_gnd_chk.h"
#include "d/col/bg/d_bg_s_lin_chk.h"
#include "d/col/c/c_cc_d.h"
#include "d/col/cc/d_cc_d.h"
#include "d/d_camera.h"
#include "d/d_player_act.h"
#include "d/d_rumble.h"
#include "d/d_sc_game.h"
#include "d/flag/sceneflag_manager.h"
#include "d/snd/d_snd_wzsound.h"
#include "f/f_base.h"
#include "f/f_manager.h"
#include "f/f_profile_name.h"
#include "m/m_angle.h"
#include "m/m_mtx.h"
#include "m/m_vec.h"
#include "nw4r/g3d/res/g3d_resfile.h"
#include "nw4r/g3d/res/g3d_resmat.h"
#include "nw4r/g3d/res/g3d_resmdl.h"
#include "nw4r/g3d/res/g3d_resshp.h"
#include "nw4r/math/math_arithmetic.h"
#include "nw4r/math/math_triangular.h"
#include "s/s_Math.h"
#include "toBeSorted/d_emitter.h"
#include <cmath>

SPECIAL_ACTOR_PROFILE(NPC_BEE, dAcNpcBee_c, fProfile::NPC_BEE, 0x123, 0, 0);

bool dAcNpcBee_c::createHeap() {
    mRes = nw4r::g3d::ResFile(getOarcResFile("Bee"));
    static const char* sModelNames[4] = {
        "fly_A",
        "fly_B",
        "walk_A",
        "walk_B",
    };
    mSwarmBeeCount = 0x14;
    for (int i = 0; i < (int)ARRAY_LENGTH(sModelNames); i++) {
        nw4r::g3d::ResMdl mdl = mRes.GetResMdl(sModelNames[i]);
        TRY_CREATE(mBeeModels[i].create(mdl, mSwarmBeeCount, &mAllocator,  true, nullptr));
    }
    return SUCCEEDED;
}
static dCcD_SrcSph beeColliderSrc = {
    /* mObjInf */ {
        /* mObjAt */ {AT_TYPE_0x2000000, 0x20000F, {0, 0, 0}, 1, 0, 0, 0, 0, 0},
        /* mObjTg */ {AT_TYPE_BUGNET | AT_TYPE_BELLOWS | AT_TYPE_BOMB, 0x00004011, {0, 00, 0x40F}, 8, 0},
        /* mObjCo */ {0}},
     /* mSphInf */ {15.0f}
};

// non matching
int dAcNpcBee_c::create() {
    mSceneflag = getFromParams(0x10, 0xFF);
    // mRoom regalloc
    if (mSceneflag != 0xFF && SceneflagManager::sInstance && SceneflagManager::sInstance->checkFlag(getRoomId(), mSceneflag)) {
        return FAILED;
    }
    CREATE_ALLOCATOR(dAcNpcBee_c);
    dAcNpcBeeSingleBee* bee = &mBees[0];
    for (int i = 0; i < mSwarmBeeCount; i++, bee++) {
        bee->mIndex = i;
        bee->mBeeState = dAcNpcBeeSingleBee::BEE_STATE_CRAWL;
        bee->mPos = mPosition;
        bee->mTimer = cM::rndF(65536);
        mColliderList.addCc(bee->mCollider, beeColliderSrc);
        bee->mCollider.mTg.OffSPrm(0x100);
        bee->mCollider.mAt.OnSPrm(0x100);
        if (i < mSwarmBeeCount / 2) {
            bee->mHide = 1;
        }
    }
    // probably inline here, regswaps
    mStts.SetRank(3);
    mInteractionFlags |= 0x80;
    mColliderList.SetStts(mStts);
    mFrameCounter = cM::rndF(65536);
    dAcObjBase_c* honeycomb = dAcObjBase_c::create(fProfile::NPC_HONEYCOMB, -1, mParams, &mPosition, nullptr, nullptr, -1);
    if (honeycomb == nullptr) {
        return FAILED;
    } else {
        mHiveRef.link(static_cast<dAcNpcHc_c*>(honeycomb));
        mAttackRef.link(dAcPy_c::GetLinkM());
    }
    return SUCCEEDED;
}

int dAcNpcBee_c::doDelete()  {
    return SUCCEEDED;
}

// hopefully equivalent
void dAcNpcBee_c::actuallyUpdateSwarmBees() {
    if (mHiveRef.isLinked()) {
        setStartingPosition(mHiveRef.get()->field_0x928);
    }
    dAcPy_c *player = dAcPy_c::GetLinkM();
    if ((mFrameCounter & 0xF) == 0) {
        mAttackRef.link(player);
        bool noSporeFound = true;
        fLiNdBa_c *current = dAcOSpore_c::getListHead();
        if (current != nullptr) {
            for (;current != nullptr;current = current->getNext()) {
                dAcOSpore_c* spore = static_cast<dAcOSpore_c*>(current->p_owner);
                if (spore != nullptr && spore->isGlitteringSpore()) {
                    mVec3_c& sporePos = spore->mPosition;
                    if (spore->mPosition.squareDistanceToXZ(mAliveBeePos) < 1000000.f &&
                    std::fabsf(spore->mPosition.y - mAliveBeePos.y) < 500.f) {
                        if (!dBgS_ObjLinChk::LineCross(&mAliveBeePos, &sporePos, this)) {
                            mAttackRef.link(spore);
                            noSporeFound = false;
                            mSporeActor = spore;
                            break;
                        }
                    }
                }
            }
        }
        if (noSporeFound) {
            dAcEbc_c *boko = nullptr;
            while((boko = static_cast<dAcEbc_c*>(fManager_c::searchBaseByProfName(fProfile::E_BC, boko)))) {
                if (boko->getField_0x130D()) continue;
                f32 distToPlayer = mAliveBeePos.distance(dAcPy_c::GetLink()->mPositionCopy2);
                f32 distToBoko = mAliveBeePos.distance(boko->mPositionCopy2);
                if (distToBoko < distToPlayer) {
                    mAttackRef.link(boko);
                    break;
                }
            }
        }
    }
    mAttackActor = mAttackRef.get();
    if (mAttackActor == nullptr) {
        mAttackActor = player;
    }
    dAcNpcBeeSingleBee* bee = &mBees[0];
    u8 beesAlive = 0;
    int beeModelIndex = 0;
    static mVec3_c colliderDisabledPos(10000.f, -20000.f, 50000.f);
    dCamera_c* camera = dScGame_c::getCamera();
    bool isFirst = true;
    int flyingBeeCount = 0;
    mAttackActorDistFromHome = mStartingPos.distance(mAttackActor->mPositionCopy2);
    mPosition = mStartingPos;
    for (int i = 0; i < mSwarmBeeCount; i++, bee++) {
        bool hideBee = true;
        mMtx_c mtx;
        if (bee->mBeeState != dAcNpcBeeSingleBee::BEE_STATE_DEAD) {
            beesAlive++;
            bee->mOldPos = bee->mPos;
            if (bee->mActionTimer) {
                bee->mActionTimer--;
            }
            if(bee->mDisableAttackTimer) {
                bee->mDisableAttackTimer--;
            }
            if (bee->mBeeState == dAcNpcBeeSingleBee::BEE_STATE_CRAWL) {
                handleBeeCrawlingOnHive(bee);
                bee->mDisableAttackTimer = 5;
            } else {
                bee->mTimer++;
                if (bee->mBeeState <= dAcNpcBeeSingleBee::BEE_STATE_FLY_UP) {
                    handleBeeFlyingStates(bee);
                    flyingBeeCount++;
                    if (isFirst) {
                        isFirst = false;
                        mPosition = bee->mPos;
                    }
                } else if (bee->mBeeState == dAcNpcBeeSingleBee::BEE_STATE_BLOWN_AWAY) {
                    handleBeeBlownAway(bee);
                } else if (bee->mBeeState == dAcNpcBeeSingleBee::BEE_STATE_RISE) {
                    handleBeeRising(bee);
                }
            }
            mVec3_c localA4;
            mVec3_c localB0;
            if (bee->mHide == 0) {
                localA4 = bee->mPos - camera->getPosition();
                // float constants should be in a different order
                if (localA4.absXZ() < 3000.f) {
                    localA4.normalize();
                    localB0 = camera->getTarget() - camera->getPosition();
                    localB0.normalize();
                    if (localB0.dot(localA4) > 0.5f) {
                        if (bee->mIsFlying) {
                            if (bee->mTimer & 1) {
                                beeModelIndex = 0;
                            } else {
                                beeModelIndex = 1;
                            }
                        } else {
                            beeModelIndex = (bee->mTimer >> 2 & 1) ? 2 : 3;
                        }
                        if (bee->mBeeState == dAcNpcBeeSingleBee::BEE_STATE_CRAWL) {
                            mtx.transS(bee->mPos);
                            mtx.ZrotM(bee->mCrawlingRotation.x);
                            mtx.XrotM(bee->mCrawlingRotation.z);
                            mtx.YrotM(bee->mCrawlingRotation.y);
                            mtx.XrotM(0x8000);
                        } else {
                            mtx.transS(bee->mPos);
                            mtx.YrotM(bee->mModelRotation.y);
                            mtx.XrotM(bee->mModelRotation.x);
                            mtx.ZrotM(bee->mModelRotation.z);
                        }
                        hideBee = false;
                    }
                }
            }
            mAliveBeePos = bee->mPos;
        }
        for (u8 j = 0; j < ARRAY_LENGTH(mBeeModels); j++) {
            if (!hideBee && j == (u8)beeModelIndex) {
                mBeeModels[j].setDrawDisabled(i, false);
                mBeeModels[j].setTransform(i, mtx);
            } else {
                mBeeModels[j].setDrawDisabled(i, true);
            }
        }
        if (!hideBee && bee->mDisableAttackTimer == 0) {
            bee->mCollider.SetC(bee->mPos);
        } else {
            bee->mCollider.SetC(colliderDisabledPos);
        }
    }
    if (flyingBeeCount != 0) {
        holdSoundWithIntParam(SE_Bee_FLY_LV, flyingBeeCount);
    }
    mColliderList.registerColliders();
    if (!beesAlive) {
        deleteRequest();
    }
}

void dAcNpcBee_c::handleBeeCrawlingOnHive(dAcNpcBeeSingleBee* bee) {
    #define CRAWLING_SUBSTATE_NONMOVING 0
    #define CRAWLING_SUBSTATE_MOVE 1
    #define CRAWLING_SUBSTATE_FLY 2
    #define CRAWLING_SUBSTATE_LAND 3
    #define CRAWLING_SUBSTATE_PRE_RISE 4
    f32 targetSpeed = 0.0f;
    dAcNpcHc_c* hive = mHiveRef.get();
    if (hive == nullptr || hive->mHealth == 0) {
        bee->mBeeState = dAcNpcBeeSingleBee::BEE_STATE_DEAD;
    } else {
        if (bee->mHide) {
            if (hive->mHealth == 1) {
                bee->mHide = 0;
                bee->field_0x024.x = cM::rndFX(300.f);
                bee->field_0x024.z = cM::rndFX(300.f);
            } else {
                bee->mPos = mStartingPos;
                return;
            }
        }
        if (dAcPy_c::GetLink()->getRidingActorType() != daPlayerActBase_c::RIDING_LOFTWING && (mAttackActor->getPosition().y - getPosition().y) < 0.f && bee->mSubState != CRAWLING_SUBSTATE_PRE_RISE) {
            // using getSpeed inline breaks the match
            f32 aggroDistance = mAttackActor->mSpeed * 20.f + 350.f;
            if (hive->field_0x915 >= 5 || hive->mHealth == 1 || (hive->field_0x916 == 0 && mAttackActorDistFromHome < aggroDistance)) {
                bee->mSubState = CRAWLING_SUBSTATE_PRE_RISE;
                bee->mActionTimer = cM::rndF(15.f);
            } else if (bee->mSubState != CRAWLING_SUBSTATE_FLY && hive->field_0x915 != 0) {
                bee->mActionTimer = cM::rndF(70.f) + 30.f;
                bee->mSubState = CRAWLING_SUBSTATE_FLY;
            }
        }
        bee->mIsFlying = 0;
        mVec3_c local_74;
        local_74 = bee->mTargetPos - bee->field_0x024;
        if (bee->mSubState == CRAWLING_SUBSTATE_NONMOVING) {
            bee->mCrawlingSpeed = 0.f;
            if (bee->mActionTimer == 0) {
                if (cM::rnd() < 0.02f) {
                    bee->mActionTimer = cM::rndF(70.f) + 30.f;
                    bee->mSubState = CRAWLING_SUBSTATE_FLY;
                } else {
                    bee->mActionTimer = cM::rndF(30.f) + 5.f;
                    bee->mTargetPos.x = cM::rndFX(300.f);
                    bee->mTargetPos.z = cM::rndFX(300.f);
                    bee->mSubState = CRAWLING_SUBSTATE_MOVE;
                }
            }
        } else if (bee->mSubState == CRAWLING_SUBSTATE_MOVE) {
            bee->mTimer++;
            sLib::addCalcAngle(bee->mCrawlingRotation.y.ref(), local_74.atan2sX_Z(), 2, 0x400);
            if (bee->mActionTimer == 0 || local_74.absXZ() < 10.f) {
                bee->mSubState = CRAWLING_SUBSTATE_NONMOVING;
                bee->mActionTimer = cM::rndF(15.f) + 2.f;
            }
            targetSpeed = 2.f;
        } else if (bee->mSubState == CRAWLING_SUBSTATE_FLY) {
            bee->mIsFlying = 1;
            sLib::addCalcScaledDiff(&bee->mCrawlingSpeed, 50.f, 0.1f, 5.f);
            sLib::addCalcAngle(bee->mCrawlingRotation.y.ref(), local_74.atan2sX_Z(), 2, 0x800);
            if ((bee->mActionTimer & 0x7) == 0) {
                bee->mTargetPos.x = cM::rndFX(300.f);
                bee->mTargetPos.z = cM::rndFX(300.f);
            }
            if (bee->mActionTimer == 0) {
                bee->mSubState = CRAWLING_SUBSTATE_LAND;
            }
            targetSpeed = 5.f;
        } else if (bee->mSubState == CRAWLING_SUBSTATE_LAND) {
            bee->mIsFlying = 1;
            sLib::addCalcScaled(&bee->mCrawlingSpeed, 1.f, 5.f);
            if (bee->mCrawlingSpeed <= 0.1f) {
                bee->mSubState = CRAWLING_SUBSTATE_NONMOVING;
            }
        } else if (bee->mSubState == CRAWLING_SUBSTATE_PRE_RISE && bee->mActionTimer == 0) {
            bee->mBeeState = dAcNpcBeeSingleBee::BEE_STATE_RISE;
            bee->mSubState = 0;
        }
        sLib::addCalcScaledDiff(&bee->mSpeed, targetSpeed, 0.5f, 0.5f);
        mMtx_c mtx;
        mtx.YrotS(bee->mCrawlingRotation.y);
        local_74.set(0.f, 0.f, bee->mSpeed);
        mtx.multVec(local_74, bee->field_0x048);
        bee->field_0x024 += bee->field_0x048;
        s16 xRot = -bee->field_0x024.x * 32768.f / 200.f;
        s16 zRot = bee->field_0x024.z * 32768.f / 200.f;
        local_74.set(0.f, -68.f - bee->mCrawlingSpeed, 0.f);
        mtx.scaleS(1.f, 1.15f, 1.f);
        mtx.YrotM(hive->mRotation.y);
        mtx.ZrotM(xRot);
        mtx.XrotM(zRot);
        mtx.multVec(local_74, bee->mPos);
        bee->mPos += mStartingPos;
        if (bee->mIsFlying == 0) {
            sLib::addCalcAngle(bee->mCrawlingRotation.x.ref(), xRot, 2, 0x800);
            sLib::addCalcAngle(bee->mCrawlingRotation.z.ref(), zRot, 2, 0x800);
        } else {
            bee->mTimer++;
            sLib::addCalcAngle(bee->mCrawlingRotation.x.ref(), 0, 1, 0x1000);
            sLib::addCalcAngle(bee->mCrawlingRotation.z.ref(), -0x8000, 1, 0x1000);
        }
    }
}

// non matching
void dAcNpcBee_c::handleBeeFlyingStates(dAcNpcBeeSingleBee* bee) {
    bee->mIsFlying = 1;
    dAcNpcHc_c* hive = mHiveRef.get();
    bee->mCollider.ClrAtSet();
    bool isFlyingAway = false;
    bool isAttacking = false;
    bool dodgedSword;
    mMtx_c mtx;
    mVec3_c offsetToTarget;
    mVec3_c local_98;
    if (bee->mBeeState == dAcNpcBeeSingleBee::BEE_STATE_CHASE) {
        if (bee->mAttackTimer != 0) {
            bee->mAttackTimer--;
        }
        if (bee->mSwordDodgeTimer != 0) {
            bee->mSwordDodgeTimer--;
            bee->mPos += bee->mSwordDodgeOffset;
            bee->mSwordDodgeOffset *= 0.8f;
        } else {
            if (mAttackActor == dAcPy_c::GetLink() && dAcPy_c::GetLink()->checkFlags0x354(4) && bee->mBeeState == dAcNpcBeeSingleBee::BEE_STATE_CHASE) {
                dodgedSword = false;
                local_98 = dAcPy_c::GetLink()->getPosition() - bee->mPos;
                if (local_98.squareMagXZ() < 90000.f) {
                    if (dAcPy_c::GetLink()->getSpecificAttackDirection() == (s16)daPlayerActBase_c::ATTACK_DIRECTION_LEFT || 
                    dAcPy_c::GetLink()->getSpecificAttackDirection() == (s16)daPlayerActBase_c::ATTACK_DIRECTION_RIGHT) {
                        local_98.y += 150.f;
                        if (std::fabsf(local_98.y) < 50.f) {
                            if (local_98.y < 0.f) {
                                bee->mSwordDodgeOffset.y = 10.f;
                            } else {
                                bee->mSwordDodgeOffset.y = -10.f;
                            }
                            bee->mSwordDodgeOffset.y *= (50.f - std::fabsf(local_98.y)) * (cM::rndF(0.02f) + 0.02f);
                            bee->mSwordDodgeOffset.x = cM::rndFX(5.f);
                            bee->mSwordDodgeOffset.z = cM::rndFX(5.f);
                            dodgedSword = true;
                        }
                    } else {
                        mtx.YrotS(-dAcPy_c::GetLink()->mRotation.y);
                        mtx.multVec(local_98, local_98);
                        f32 xAbs = std::fabsf(local_98.x);
                        if (xAbs < 130.f) {
                            if (local_98.x > 0.f) {
                                local_98.x = -15.f;
                            } else {
                                local_98.x = 15.f;
                            }
                            local_98.x *= (130.f - xAbs) * (cM::rndF(0.005f) + 0.005f);
                            local_98.y = cM::rndFX(7.f);
                            local_98.z = cM::rndFX(7.f);
                            mtx.YrotS(dAcPy_c::GetLink()->mRotation.y);
                            mtx.multVec(local_98, bee->mSwordDodgeOffset);
                            dodgedSword = true;
                        }
                    }
                    if (dodgedSword) {
                        bee->mActionTimer = bee->mSwordDodgeTimer = cM::rndF(5.f) + 10.f;
                        bee->mSpeed = 0.f;
                    }
                }
            }
        }
        offsetToTarget = mAttackActor->mPositionCopy2 - bee->mPos;
        f32 distToTarget = offsetToTarget.squareMagXZ();
        // targeted actor is less than 300 units away and not moving
        if (distToTarget < 90000.f && mAttackActor->getSpeed() < 10.f) {
            if ((bee->mAttackTimer != 0 && bee->mSpeed > 10.f) || distToTarget < 400.f) {
                bee->mActionTimer = 0;
            }
            if (bee->mActionTimer == 0) {
                bee->mIsAttacking = 0;
                if (bee->mAttackTimer != 0 || cM::rnd() < 0.95f) {
                    bee->mSpeed = cM::rndF(5.f) + 5.f;
                    bee->mTargetPos.x = cM::rndFX(200.f);
                    bee->mTargetPos.y = cM::rndFX(50.f);
                    bee->mTargetPos.z = cM::rndF(50.f) + 100.f;
                    mtx.YrotS(mAttackActor->mRotation.y);
                    mtx.multVec(bee->mTargetPos, bee->mTargetPos);
                    bee->mActionTimer = cM::rndF(15.f) + 15.f;
                } else {
                    bee->mSpeed = cM::rndF(5.f) + 17.f;
                    bee->mTargetPos.x = 0.f;
                    bee->mTargetPos.y = 0.f;
                    bee->mTargetPos.z = 0.f;
                    bee->mActionTimer = 0x14;
                    bee->mIsAttacking = 1;
                    isAttacking = true;
                }
            }
        } else {
            bee->mIsAttacking = 0;
            if (bee->mActionTimer == 0) {
                bee->mActionTimer = cM::rndF(15.f) + 15.f;
                bee->mSpeed = cM::rndF(5.f) + 17.f + 2.f;
                bee->mTargetPos.x = cM::rndFX(100.f);
                bee->mTargetPos.y = cM::rndFX(50.f);
                bee->mTargetPos.z = cM::rndFX(100.f);
            }
            bee->mAttackTimer = 0x50;
            
        }
        offsetToTarget += bee->mTargetPos;
        f32 maxDist = 1000000.f;
        if ((bee->mFlyingStatesTimer == 0 || distToTarget > maxDist) || (
            mAttackActor == dAcPy_c::LINK && dAcPy_c::LINK->checkActionFlags(0x40000)
        )) {
            if (mSporeActor == mAttackActor || (hive != nullptr && mAttackActorDistFromHome < 300.f)) {
                bee->mFlyingStatesTimer = cM::rndF(30.f) + 20.f;
            } else {
                if (hive != nullptr) {
                    bee->mBeeState = dAcNpcBeeSingleBee::BEE_STATE_FLY_TO_HIVE;
                    bee->mTargetPos = mStartingPos;
                } else {
                    bee->mBeeState = dAcNpcBeeSingleBee::BEE_STATE_FLY_UP;
                    mtx.YrotS(cM::rndF(65536.f));
                    offsetToTarget.y = cM::rndF(2000.f) + 6000.f;
                    offsetToTarget.x = 0.f;
                    offsetToTarget.z = 8000.f;
                    mtx.multVec(offsetToTarget, local_98);
                    bee->mTargetPos = local_98 + mAttackActor->mPositionCopy2;
                    bee->mFlyingStatesTimer = cM::rndF(55.f) + 200.f;
                }
                bee->mSpeed = cM::rndF(5.f) + 17.f;
                bee->mIsAttacking = 0;
            }
        }
        if (bee->mSpeed >= 10.f && mSporeActor != mAttackActor) {
            bee->mCollider.OnAtSet();
        }
    } else if (bee->mBeeState == dAcNpcBeeSingleBee::BEE_STATE_FLY_TO_HIVE || bee->mBeeState == dAcNpcBeeSingleBee::BEE_STATE_FLY_UP) {
        isFlyingAway = true;
        offsetToTarget = bee->mTargetPos - bee->mPos;
        f32 distToTarget;
        f32 fVar17;
        if (hive != nullptr) {
            fVar17 = hive->mScale.x * 55.f;
        } else {
            fVar17 = 55.f;
        }
        distToTarget = offsetToTarget.mag();
        if (fVar17 < 55.f) {
            fVar17 = 55.f;
        }
        if (distToTarget < fVar17) {
            if (bee->mBeeState == dAcNpcBeeSingleBee::BEE_STATE_FLY_TO_HIVE) {
                bee->mBeeState = dAcNpcBeeSingleBee::BEE_STATE_CRAWL;
                bee->mSpeed = 0.f;
            }
        } else {
            if (distToTarget > 500.f) {
                distToTarget = 500.f;
            }
            // weird mul vs slwi
            offsetToTarget.x += distToTarget * nw4r::math::SinIdx(bee->mTimer * 0x300);
            offsetToTarget.y += distToTarget * nw4r::math::CosIdx(bee->mTimer * 0x300) * 0.5f;
            offsetToTarget.z += distToTarget * nw4r::math::CosIdx(bee->mTimer * 0x500);
        }
        if (bee->mBeeState == dAcNpcBeeSingleBee::BEE_STATE_FLY_UP && bee->mFlyingStatesTimer == 0) {
            bee->mBeeState = dAcNpcBeeSingleBee::BEE_STATE_DEAD;
        }
    }
    if (bee->mWallReflectTimer == 0) {
        if (isAttacking) {
            bee->mCrawlingRotation.y = offsetToTarget.atan2sX_Z();
            bee->mCrawlingRotation.x = offsetToTarget.atan2sXZ_Y();
        } else {
            s32 fVar17 = nw4r::math::SinIdx(bee->mTimer * 0xDAC) * 1500.f;
            s32 fVar16 = nw4r::math::SinIdx(bee->mTimer * 3000) * 1500.f;
            // non match
            sLib::addCalcAngle(bee->mCrawlingRotation.y.ref(), fVar17 + offsetToTarget.atan2sX_Z(), 1, 0x1000);
            sLib::addCalcAngle(bee->mCrawlingRotation.x.ref(), fVar16 - offsetToTarget.atan2sY_XZ(), 1, 0x1000);
        }
    }
    mtx.YrotS(bee->mCrawlingRotation.y);
    mtx.XrotM(bee->mCrawlingRotation.x);
    offsetToTarget.set(0.f, 0.f, bee->mSpeed);
    mtx.multVec(offsetToTarget, bee->field_0x048);
    bee->mPos += bee->field_0x048;
    if (bee->mFlyingStatesTimer != 0) {
        bee->mFlyingStatesTimer--;
    }
    if (isFlyingAway) {
        sLib::addCalcAngle(bee->mModelRotation.y.ref(), bee->mCrawlingRotation.y, 2, 0x800);
        sLib::addCalcAngle(bee->mModelRotation.x.ref(), bee->mCrawlingRotation.x, 2, 0x800);
    } else {
        local_98 = mAttackActor->mPositionCopy2 - bee->mPos;
        s16 sVar15 = local_98.atan2sX_Z();
        sLib::addCalcAngle(bee->mModelRotation.y.ref(), sVar15, 2, 0x1000);
        if (bee->mIsAttacking != 0) {
            sLib::addCalcAngle(bee->mModelRotation.x.ref(), -0x4000, 2, 0x1000);
        } else {
            sLib::addCalcAngle(bee->mModelRotation.x.ref(), 0, 2, 0x1000);

        }
    }
    if (bee->mWallReflectTimer == 0) {
        // if wall hit
        // potential @bug, doesn't catch when both wall and floor are hit
        if (beeCheckWallFloorCollision(bee) == 2) {
            bee->mWallReflectTimer = cM::rndF(5.f) + 10.f;
            bee->mCrawlingRotation.y += ((s16)cM::rndFX(8000.f)) + 0x8000;
        }
    } else {
        bee->mWallReflectTimer--;
    }
    if ((bee->mTimer & 0xF) == 0 && cM::rnd() < 0.5f) {
        bee->mCrawlingRotation.z = cM::rndFX(5000.f);
    }
    sLib::addCalcAngle(bee->mModelRotation.z.ref(), bee->mCrawlingRotation.z, 2, 0x200);
}

void dAcNpcBee_c::handleBeeRising(dAcNpcBeeSingleBee* bee) {
    bee->mIsFlying = 1;
    switch (bee->mSubState) {
        case 0: {
            bee->mTargetPos = mStartingPos + (bee->mPos - mStartingPos) * (cM::rndF(0.5f) + 1.5f);
            bee->mActionTimer = 0x14;
            bee->mSubState = 1;
            // fallthrough
        }
        case 1: {
            cLib::addCalcPos2(&bee->mPos, bee->mTargetPos, 0.2f, 15.0f);
            sLib::addCalcAngle(bee->mCrawlingRotation.y.ref(), cLib::targetAngleY(bee->mPos, mAttackActor->mPositionCopy2), 2, 0x800);
            sLib::addCalcAngle(bee->mCrawlingRotation.x.ref(), 0, 2, 0x800);
            sLib::addCalcAngle(bee->mCrawlingRotation.z.ref(), 0, 2, 0x800);
            if (bee->mActionTimer == 0) {
                bee->mBeeState = dAcNpcBeeSingleBee::BEE_STATE_CHASE;
                bee->mSubState = 0;
                bee->mFlyingStatesTimer = cM::rndF(155.f) + 300.f; 
            }
        }
    }   
}

// TODO: data
void dAcNpcBee_c::handleBeeBlownAway(dAcNpcBeeSingleBee* bee) {
    bee->mPos += bee->field_0x048;
    bee->mPos.x += nw4r::math::SinIdx(bee->mTimer * 0x900) * 10.f;
    bee->mPos.y += nw4r::math::SinIdx(bee->mTimer * 0xa00) * 10.f;
    bee->mPos.z += nw4r::math::SinIdx(bee->mTimer * 0xb00) * 10.f;
    if ((bee->mIndex & 1) != 0) {
        bee->mModelRotation.y += 0xD00;
    } else {
        bee->mModelRotation.y -= 0xD00;
    }
    bee->mModelRotation.x += 0xA00;
    if (bee->mActionTimer == 0) {
        bee->mBeeState = dAcNpcBeeSingleBee::BEE_STATE_CHASE;
        bee->mSubState = 0;
        bee->mFlyingStatesTimer = cM::rndF(155.f) + 300.f;
    }
    beeCheckWallFloorCollision(bee);
}

int dAcNpcBee_c::actorExecute() {
    mFrameCounter++;
    updateSwarmBeeColliders();
    actuallyUpdateSwarmBees();
    if (mRumbleTimer) {
        mRumbleTimer--;
        if (mRumbleTimer == 7) {
            dRumble_c::start(dRumble_c::sRumblePreset1, 1);
        } else if (mRumbleTimer == 5) {
            dRumble_c::stop(-1);
        }
    }
    return SUCCEEDED;
}

u32 dAcNpcBee_c::updateSwarmBeeColliders() {
    mMtx_c mtx;
    mtx.YrotS(dAcPy_c::LINK->mRotation.y);
    dAcNpcBeeSingleBee* bee = &mBees[0];
    for (int i = 0; i < mSwarmBeeCount; i++, bee++) {
        if (bee->mBeeState != dAcNpcBeeSingleBee::BEE_STATE_DEAD) {
            if (bee->mCollider.ChkTgHit()) {
                if (bee->mCollider.ChkTgAtHitType(AT_TYPE_BUGNET)) {
                    bee->mBeeState = dAcNpcBeeSingleBee::BEE_STATE_DEAD;
                    dAcPy_c::LINK->bugNetCollectTreasure(ITEM_DEKU_HORNET);
                    dAcNpcIcgKobun_c* npc = static_cast<dAcNpcIcgKobun_c*>(fManager_c::searchBaseByProfName(fProfile::NPC_ICGK));
                    if (npc != nullptr) {
                        npc->fn_66_30C0(ITEM_DEKU_HORNET);
                    }
                } else if (bee->mCollider.ChkTgAtHitType(AT_TYPE_BOMB)) {
                    bee->mBeeState = dAcNpcBeeSingleBee::BEE_STATE_DEAD;
                    dJEffManager_c::spawnEffect(PARTICLE_RESOURCE_ID_MAPPING_394_, bee->mPos, nullptr, nullptr, nullptr, nullptr, 0, 0);
                } else {
                    bee->mBeeState = dAcNpcBeeSingleBee::BEE_STATE_BLOWN_AWAY;
                    bee->field_0x048 = bee->mCollider.GetTgAtHitDir();
                    bee->field_0x048.normalize();
                    if (bee->mCollider.ChkTgAtHitType(AT_TYPE_BELLOWS)) {
                        bee->field_0x048 *= cM::rndF(5.f) + 15.f;
                        bee->mActionTimer = cM::rndF(5.f) + 15.f;
                        mRumbleTimer |= 8;
                    } else {
                        // can this even be reached? all possible AT types should be handled
                        bee->field_0x048 *= cM::rndF(5.f) + 25.f;
                        bee->field_0x048.y = cM::rndF(5.f) +15.f;
                        mVec3_c local_b8(cM::rndFX(20.f), 0.f, 0.f);
                        mtx.multVec(local_b8, local_b8);
                        bee->field_0x048 += local_b8;
                        bee->mActionTimer = cM::rndF(50.f) + 200.f;
                    }
                }
            }
        }
    }
    return 1;
}

int dAcNpcBee_c::draw() {
    dShpProc1_c* model = &mBeeModels[0];
    for (int i = 0; i < 4; i++, model++) {
        model->entry();
    }
    return 1;
}

/**
 * Returns bits when a collision is detected, 2 is wall, 1 is floor
 */
s32 dAcNpcBee_c::beeCheckWallFloorCollision(dAcNpcBeeSingleBee* bee) {
    dBgS_ObjGndChk gndChk;
    dBgS_ObjLinChk linChk;
    mVec3_c local_148;
    mVec3_c scaled;
    u32 ret = 0;
    local_148 = bee->mPos - bee->mOldPos;
    local_148.y = 0.f;
    local_148.normalize();
    scaled = local_148 * 50.f;
    local_148 = bee->mPos + scaled;
    linChk.Set(&bee->mPos, &local_148, nullptr);
    if (dBgS::GetInstance()->LineCross(&linChk)) {
        ret = 2;
        bee->mPos.x = linChk.GetLinEnd().x - scaled.x;
        bee->mPos.z = linChk.GetLinEnd().z - scaled.z;
    }
    local_148 = bee->mPos;
    local_148.y += 100.f;
    gndChk.SetPos(&local_148);
    if (bee->mPos.y <= dBgS::GetInstance()->GroundCross(&gndChk) + 15.f) {
        // imagine using temp variables...
        bee->mPos.y = dBgS::GetInstance()->GroundCross(&gndChk) + 15.f;
        ret |= 1;
    }
    return ret;
}
